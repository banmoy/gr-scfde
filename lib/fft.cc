#include "fft.h"
#include <fftw3.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cassert>

#include <gnuradio/sys_paths.h>

#include <boost/thread.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

static boost::mutex  planning_mutex;

namespace spinal
{

namespace fft
{

static const char*
wisdom_filename()
{
    static fs::path path;
    path = fs::path(gr::appdata_path()) / ".spinal_fftw_wisdom";
    return path.string().c_str();
}

static void
import_wisdom()
{
    const char *filename = wisdom_filename();
    FILE *fp = fopen(filename, "r");
    if (fp != 0) {
        int r = fftwf_import_wisdom_from_file(fp);
        fclose(fp);
        if (!r) {
            fprintf(stderr, "gr::fft: can't import wisdom from %s\n", filename);
        }
    }
}

static void
config_threading(int nthreads)
{
    static int fftw_threads_inited = 0;

#ifdef FFTW3F_THREADS
    if (fftw_threads_inited == 0) {
        fftw_threads_inited = 1;
        fftwf_init_threads();
    }

    fftwf_plan_with_nthreads(nthreads);
#endif
}

static void
export_wisdom()
{
    const char *filename = wisdom_filename();
    FILE *fp = fopen(filename, "w");
    if (fp != 0) {
        fftwf_export_wisdom_to_file(fp);
        fclose(fp);
    } else {
        fprintf(stderr, "fft_impl_fftw: ");
        perror(filename);
    }
}

fft_complex::fft_complex(int fft_size, bool forward, int nthreads)
{
    boost::mutex::scoped_lock lock(planning_mutex);

    assert(sizeof(fftwf_complex) == sizeof(gr_complex));

    if (fft_size <= 0)
        throw std::out_of_range("fft_impl_fftw: invalid fft_size");

    d_fft_size = fft_size;
    d_inbuf = (gr_complex *)fftwf_malloc(sizeof(gr_complex) * inbuf_length());
    if (d_inbuf == 0)
        throw std::runtime_error("fftwf_malloc");

    d_outbuf = (gr_complex *)fftwf_malloc(sizeof(gr_complex) * outbuf_length());
    if (d_outbuf == 0) {
        fftwf_free(d_inbuf);
        throw std::runtime_error("fftwf_malloc");
    }

    d_nthreads = nthreads;
    config_threading(nthreads);
    import_wisdom();  // load prior wisdom from disk

    d_plan = fftwf_plan_dft_1d(fft_size,
                               reinterpret_cast<fftwf_complex *>(d_inbuf),
                               reinterpret_cast<fftwf_complex *>(d_outbuf),
                               forward ? FFTW_FORWARD : FFTW_BACKWARD,
                               FFTW_MEASURE);

    if (d_plan == NULL) {
        fprintf(stderr, "gr::fft: error creating plan\n");
        throw std::runtime_error("fftwf_plan_dft_1d failed");
    }
    export_wisdom();  // store new wisdom to disk
}

fft_complex::~fft_complex()
{
    // Hold global mutex during plan construction and destruction.
    boost::mutex::scoped_lock lock(planning_mutex);

    fftwf_destroy_plan((fftwf_plan)d_plan);
    fftwf_free(d_inbuf);
    fftwf_free(d_outbuf);
}

void
fft_complex::set_nthreads(int n)
{
    if (n <= 0)
        throw std::out_of_range("gr::fft: invalid number of threads");
    d_nthreads = n;

#ifdef FFTW3F_THREADS
    fftwf_plan_with_nthreads(d_nthreads);
#endif
}

void
fft_complex::execute()
{
    fftwf_execute((fftwf_plan)d_plan);
}


} // fft

} // spinal
