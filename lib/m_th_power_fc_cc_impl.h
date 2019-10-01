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

#ifndef INCLUDED_KUHNKE_M_TH_POWER_FC_CC_IMPL_H
#define INCLUDED_KUHNKE_M_TH_POWER_FC_CC_IMPL_H

#include <mthpower/m_th_power_fc_cc.h>
namespace gr {
  namespace mthpower {

    class m_th_power_fc_cc_impl : public m_th_power_fc_cc
    {
     private:
      // Nothing to declare in this block.
      unsigned int d_M;
      unsigned int d_N;
      unsigned int d_symbol_rate;

      double        d_derotation_phase;
      double        d_f_est;
      gr_complex   d_last_sample;
      std::vector<double>  d_avg;

      void block_average_filter(double f_est) ;

     public:
      m_th_power_fc_cc_impl(unsigned int M, unsigned int N, float symbol_rate);
      ~m_th_power_fc_cc_impl();


      void set_M(unsigned int M);
      void set_N(unsigned int N);
      void set_symbol_rate(double symbol_rate);

      unsigned int M() const;
      unsigned int N() const;
      double       symbol_rate() const;

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace mthpower
} // namespace gr

#endif /* INCLUDED_KUHNKE_M_TH_POWER_FC_CC_IMPL_H */

