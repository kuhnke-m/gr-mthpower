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


#ifndef INCLUDED_KUHNKE_M_TH_POWER_V1_CC_H
#define INCLUDED_KUHNKE_M_TH_POWER_V1_CC_H

#include <mthpower/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace mthpower {

    /*!
     * \brief <+description of block+>
     * \ingroup mthpower
     *
     */
    class MTHPOWER_API m_th_power_v1_cc : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<m_th_power_v1_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mthpower::m_th_power_v1_cc.
       *
       * To avoid accidental use of raw pointers, mthpower::m_th_power_v1_cc's
       * constructor is in a private implementation
       * class. mthpower::m_th_power_v1_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int N=4, unsigned int M=4, float phase=0);
    };

  } // namespace mthpower
} // namespace gr

#endif /* INCLUDED_KUHNKE_M_TH_POWER_V1_CC_H */

