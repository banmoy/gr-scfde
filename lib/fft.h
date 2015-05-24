#pragma once

#include <gnuradio/gr_complex.h>

namespace spinal
{

namespace fft
{

class fft_complex
{
    int	      d_fft_size;
    int         d_nthreads;
    gr_complex *d_inbuf;
    gr_complex *d_outbuf;
    void	     *d_plan;

public:
    fft_complex(int fft_size, bool forward = true, int nthreads = 1);
    virtual ~fft_complex();

    /*
     * These return pointers to buffers owned by fft_impl_fft_complex
     * into which input and output take place. It's done this way in
     * order to ensure optimal alignment for SIMD instructions.
     */
    gr_complex* get_inbuf()  const { return d_inbuf; }
    gr_complex* get_outbuf() const { return d_outbuf; }

    int inbuf_length()  const { return d_fft_size; }
    int outbuf_length() const { return d_fft_size; }

    /*!
     *  Set the number of threads to use for caclulation.
     */
    void set_nthreads(int n);

    /*!
     *  Get the number of threads being used by FFTW
     */
    int nthreads() const { return d_nthreads; }

    /*!
     * compute FFT. The input comes from inbuf, the output is placed in
     * outbuf.
     */
    void execute();
};

} // fft

} // spinal
