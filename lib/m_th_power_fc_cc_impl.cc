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

#include <vector>
#include <math.h>
#include <gnuradio/io_signature.h>
#include "m_th_power_fc_cc_impl.h"

namespace gr {
  namespace mthpower {

    m_th_power_fc_cc::sptr
    m_th_power_fc_cc::make(unsigned int M, unsigned int N, float symbol_rate)
    {
      return gnuradio::get_initial_sptr
        (new m_th_power_fc_cc_impl(M, N, symbol_rate));
    }

    /*
     * The private constructor
     */
    m_th_power_fc_cc_impl::m_th_power_fc_cc_impl(unsigned int M, unsigned int N, float symbol_rate)
      : gr::sync_block("m_th_power_fc_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make2(2, 2, sizeof(gr_complex), sizeof(float))),
                d_M(M),
                d_symbol_rate(double(symbol_rate)),
                d_derotation_phase(0.0),
                d_f_est(0.0),
                d_last_sample(0.0,0.0),
                d_avg(),
                d_N(N)
    {
      d_avg.reserve(N);
    }

    /*
     * Our virtual destructor.
     */
    m_th_power_fc_cc_impl::~m_th_power_fc_cc_impl()
    {
    }


    void 
    m_th_power_fc_cc_impl::set_M(unsigned int M) {
      d_M = M;
    }
    void 
    m_th_power_fc_cc_impl::set_symbol_rate(double symbol_rate){
      d_symbol_rate = symbol_rate;
    }
    void
    m_th_power_fc_cc_impl::set_N(unsigned int N) {
      d_avg.clear();
      d_avg.reserve(N);
      d_N = N;
    }

    unsigned int 
    m_th_power_fc_cc_impl::M() const {
      return d_M;
    }
    double        
    m_th_power_fc_cc_impl::symbol_rate() const {
      return d_symbol_rate;
    }
    unsigned int 
    m_th_power_fc_cc_impl::N() const {
      return d_N;
    }

    void
    m_th_power_fc_cc_impl::block_average_filter(double f_est) {
      if ( d_avg.size() < d_N )
        d_avg.push_back(f_est);
      else
      {
        int i;
        double sum = 0.0;
        for (i = 0; i<d_N; ++i )
          sum += d_avg[i];
        sum /= d_N;
        d_f_est = sum;
        d_avg.clear();
      }
    }

    int
    m_th_power_fc_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      float *f_out    = (float * ) output_items[1];

      // Do <+signal processing+>
      int i;
      for ( i = 0 ; i < noutput_items ; ++i ) {
        d_derotation_phase += -2.0*M_PI*d_f_est * 1.0/d_symbol_rate;
        d_derotation_phase = fmod(d_derotation_phase, 2*M_PI);
        if ( d_derotation_phase >= 2*M_PI || d_derotation_phase <= -2*M_PI ) {
          std::cout << "warn " << d_derotation_phase << std::endl;
        }
        // mod 2pi the derotation phase
        gr_complexd correction = std::polar(double(1.0), d_derotation_phase);
        gr_complex current_sample = in[i];
        out[i] = gr_complex(gr_complexd(current_sample) * correction);
        f_out[i] = float(d_f_est);

        if ( current_sample != gr_complex(0.0) ) {
          current_sample *= 1.0/std::abs(current_sample);
          //not really necessary for 1 sample processing
        }

        double phase_offset = 1.0/d_M * \
          std::arg( \
            std::pow(current_sample, d_M) * \
            std::conj(std::pow(d_last_sample, d_M)) \
          );

        double w_est = d_symbol_rate * phase_offset;
        block_average_filter(w_est/(2*M_PI));
        d_last_sample = current_sample;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace mthpower */
} /* namespace gr */

