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

#ifndef INCLUDED_KUHNKE_M_TH_POWER_V1_CC_IMPL_H
#define INCLUDED_KUHNKE_M_TH_POWER_V1_CC_IMPL_H

#include <mthpower/m_th_power_v1_cc.h>

namespace gr {
  namespace mthpower {

    class m_th_power_v1_cc_impl : public m_th_power_v1_cc
    {
     private:
      // Nothing to declare in this block.
      unsigned int d_N;
      unsigned int d_M;
      float        d_phase;
      int          d_n;
      float        d_phase_est_old;
      bool         d_started;

      void seg_change_detector(double phase_est);
     public:
      m_th_power_v1_cc_impl(unsigned int N, unsigned int M, float phase);
      ~m_th_power_v1_cc_impl();
    
      void set_phase(float phase);
      void set_M(unsigned int M);
      float phase() const;
      unsigned int M() const;

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace mthpower
} // namespace gr

#endif /* INCLUDED_KUHNKE_M_TH_POWER_V1_CC_IMPL_H */

