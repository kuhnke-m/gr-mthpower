/* -*- c++ -*- */
/* 
 * Copyright 2019 Maximilian Kuhnke.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include <gnuradio/io_signature.h>
#include "m_th_power_v1_cc_impl.h"

namespace gr {
  namespace mthpower {

    m_th_power_v1_cc::sptr
    m_th_power_v1_cc::make(unsigned int N, unsigned int M, float phase)
    {
      return gnuradio::get_initial_sptr
        (new m_th_power_v1_cc_impl(N, M, phase));
    }

    /*
     * The private constructor
     */
    m_th_power_v1_cc_impl::m_th_power_v1_cc_impl(unsigned int N, unsigned int M, float phase)
      : gr::sync_block("m_th_power_v1_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*N),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*N)),
                d_N(N),
                d_M(M),
                d_phase(phase),
                d_n(0),
                d_phase_est_old(0.0),
                d_started(true)
    {
      //std::cout << "using phase " << phase << std::endl;
    }

    /*
     * Our virtual destructor.
     */
    m_th_power_v1_cc_impl::~m_th_power_v1_cc_impl()
    {
    }

    void 
    m_th_power_v1_cc_impl::set_phase(float phase)
    {
      d_phase = phase;
    }

    void 
    m_th_power_v1_cc_impl::set_M(unsigned int M)
    {
      d_M = M;
    }

    float
    m_th_power_v1_cc_impl::phase() const
    {
      return d_phase;
    }

    unsigned int 
    m_th_power_v1_cc_impl::M() const
    {
      return d_M;
    }

    void
    m_th_power_v1_cc_impl::seg_change_detector(double phase_est)
    {
      // initial conditions, for the testbench
      if ( d_started ) {
        d_started = false;
        d_phase_est_old = phase_est;
      }

      double delta = phase_est - d_phase_est_old;
      if ( delta < -M_PI/d_M )
          d_n += 1;
      else if ( delta > M_PI/d_M )
          d_n -= 1;
      d_n %= d_M;
      d_phase_est_old = phase_est;
    }

    int
    m_th_power_v1_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex*) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];


      // Do <+signal processing+>
      int i;
      int j;
      for ( i = 0; i < noutput_items; ++i ) {
        gr_complex raised_sum = gr_complex(0,0);
        for ( j = 0; j < d_N; ++j ) {
         //std::cout << "i: " << i << ", j: " << j << " = " << in[i+j] << std::endl;
          //out[i+j] = in[i+j]; // test: just copy in -> out
          if ( in[i*d_N+j] != gr_complex(0,0) ) {
            gr_complex raised_sample = std::pow(in[i*d_N+j], gr_complex(d_M));
            gr_complex norm_sample = raised_sample*gr_complex(1/std::abs(raised_sample));
            raised_sum += norm_sample;
          }
        }
        float phase_est = std::arg(raised_sum) * 1/d_M;
        seg_change_detector(phase_est);
        gr_complex correction = std::polar(float(1.0), float(-(phase_est + d_n*2.0*M_PI/d_M - d_phase)));
        for ( j = 0; j < d_N; ++j) {
          out[i*d_N+j] = in[i*d_N+j] * correction;
          //if ( std::abs(out[i+j]) < 0. ) {
          //  std::cout << "lol what" << in[i+j] << " " << i << " " << j << std::endl;
          //}
        }
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace mthpower */
} /* namespace gr */

