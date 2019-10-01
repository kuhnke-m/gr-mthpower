#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# 
# Copyright 2019 Maximilian Kuhnke.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 
# -*- coding: utf-8 -*-

##################################################
# Maximilian Kuhnke
# --------------------------------------------------
# Testbench testing the m-th power algorithm
#
#   Tests:
#       1) simple io averaging test
#       2) frequency offset drift test
# --------------------------------------------------
##################################################


##################################################
# GNU Radio Python Flow Graph
# Title: Testbench M Th Power
# GNU Radio version: 3.7.13.5
##################################################

from distutils.version import StrictVersion

#if __name__ == '__main__':
#    import ctypes
#    import sys
#    if sys.platform.startswith('linux'):
#        try:
#            x11 = ctypes.cdll.LoadLibrary('libX11.so')
#            x11.XInitThreads()
#        except:
#            print "Warning: failed to XInitThreads()"

import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
#from gnuradio import qtgui
from gnuradio import gr_unittest
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from math import pi
from math import sqrt
import math
import cmath
from optparse import OptionParser
#import sip
from gnuradio import gr, gr_unittest
from gnuradio import blocks
import mthpower_swig as kuhnke

class qa_m_th_power_v1_cc (gr_unittest.TestCase):

    def setDefaultMthPowerValues(self):
        self.N = 1
        self.M = 4
        self.phase = 0


    def setUp (self):
        self.tb = gr.top_block()
        self.setDefaultMthPowerValues()
        self.precision = 6 # digits to compare in tests, currently the maximum


    def tearDown (self):
        self.tb = None


    ###################################################
    #
    #  For some reason, GNURadio works with more samples than necessary
    #  (probably due to performance optimisations)
    #
    ###################################################
    def base(self, source_data):
        source_data_big = source_data
        length = len(source_data)
        source_data_big += 512*[0,]
        src = blocks.vector_source_c(source_data_big, False, self.N)
        dut = kuhnke.m_th_power_v1_cc(
            M=self.M,
            N=self.N,
            phase=self.phase,
        )
        snk = blocks.vector_sink_c(self.N)

        self.tb.connect(src, dut)
        self.tb.connect(dut, snk)
        self.tb.run()

        result_data = snk.data()
        return result_data[0:length]


    # test general rotation
    def test_001(self):
        self.N = 1
        self.M = 1
        self.phase = 0

        source_data = 4*self.N*[1+1j,]
        expected_data = 4*self.N*[sqrt(2),]

        result_data = self.base(source_data)

        self.assertComplexTuplesAlmostEqual(expected_data, result_data, self.precision)


    # test some more rotations
    def test_002(self):
        self.N = 1
        self.M = 4
        source_data = [1, cmath.rect(1, pi/10), cmath.rect(1, pi/8), cmath.rect(0.7, pi/5)]
        expected_data = [1, 1, 1, 0.7]

        result_data = self.base(source_data)

        self.assertComplexTuplesAlmostEqual(expected_data, result_data, self.precision)

    # test the other quadrants
    def test_003(self):
        self.N = 1
        self.M = 4

        a = 1.11
        b = [pi/8, pi/2+pi/8, pi+pi/8, 3*pi/2+pi/8]
        source_data = [cmath.rect(a, b[0]), cmath.rect(a, b[1]), cmath.rect(a, b[2]), cmath.rect(a,b[3])]
        expected_data = [a, a*1j, -a, -a*1j] #not exact but close..

        result_data = self.base(source_data)
        self.assertComplexTuplesAlmostEqual(expected_data, result_data, self.precision)



    ###################################################
    #
    #   Elaborate tests
    #       - averaging test
    #       - test 2*pi ambiguity
    #       - test phase unwrapping
    #       (- test frequency response)
    #
    ###################################################

    # averaging test
    def test_average1(self):
        self.N = 4 # average over 4 samples
        self.M = 4 # QPSK
        self.phase = 0 # (symbols = 1,j,-1,-j)

        a = 1.11
        b_base = pi/4
        b_offset = pi/8
        b = 2*[b_base+b_offset,] + 2*[b_base-b_offset,]
        source_data = []
        expected_data = []
        for i in range(len(b)):
            source_data.append(cmath.rect(a, b[i]))

        # this doesn't work with the algorithm because it averages before taking the angle
        # (it doesn't really result in the average angle)
        #
        #for i in range(len(b)):
        #    expected_data.append(source_data[i] * cmath.rect(1, -b_base))

        # But this does. It emulates what the algorithm is doing then compares the result.
        summe = 0
        for i in range(len(b)):
            summe = summe + cmath.rect(pow(a, self.M), b[i]*self.M)
        angle_correct = -cmath.phase(summe)/self.M
        for i in range(len(b)):
            expected_data.append(source_data[i]*cmath.rect(1, angle_correct))

        result_data = self.base(source_data)
        self.assertComplexTuplesAlmostEqual(expected_data, result_data, self.precision)

    # averaging test
    def test_average2(self):
        self.N = 4 # average over 4 samples
        self.M = 4 # QPSK
        self.phase = 0 # (symbols = 1,j,-1,-j)

        a = 1.11
        b_base = pi/4
        b_offset = pi/8
        b = 1*[b_base+b_offset,] + 3*[b_base-b_offset/3,]
        source_data = []
        expected_data = []
        for i in range(len(b)):
            source_data.append(cmath.rect(a, b[i]))

        # this doesn't work with the algorithm because it averages before taking the angle
        # (it doesn't really result in the average angle)
        #
        #for i in range(len(b)):
        #    expected_data.append(source_data[i] * cmath.rect(1, -b_base))

        # But this does. It emulates what the algorithm is doing then compares the result.
        summe = 0
        for i in range(len(b)):
            summe = summe + cmath.rect(pow(a, self.M), b[i]*self.M)
        angle_correct = -cmath.phase(summe)/self.M
        for i in range(len(b)):
            expected_data.append(source_data[i]*cmath.rect(1, angle_correct))

        result_data = self.base(source_data)
        self.assertComplexTuplesAlmostEqual(expected_data, result_data, self.precision)

    def base_phase_unwrapping(self, phase_increment=2*pi/100):
        self.N = 1
        self.M = 4
        self.phase = 0

        a = 1.11
        b_start = 0
        b_end = 2*pi
        b = []
        source_data = []
        n = int(math.floor((b_start+b_end)/phase_increment))
        # generate range of phase values, slowly increasing
        for i in range(n):
            b.append((b_start+phase_increment*i))

        for i in range(n):
            source_data.append(cmath.rect(a, b[i]))

        expected_data = n*[cmath.rect(a, 0),]

        result_data = self.base(source_data)
        #doesnt work because it measured from zero instead of significant digits
        #self.assertComplexTuplesAlmostEqual(expected_data, result_data, self.precision)
        self.assertComplexTuplesAlmostEqual2(expected_data, result_data, 1e-12, 1e-06)

    # test 2*pi ambiguity / phase unwrapping
    def test_phase_unwrapping(self):
        self.base_phase_unwrapping()

    # test unwrapping limit (amount of phase increment our algorithm can handle)
    # ..Extensive test..
    def test_phase_unwrapping2(self):
        inc = 2*pi/50
        start = pi/8
        end = 2*pi/3
        for i in range(int(math.floor((end-start)/inc))):
            phase_inc = start+inc*i
            try:
                self.base_phase_unwrapping(phase_inc)
            except:
                cur_phase_inc = phase_inc
                pre_phase_inc = phase_inc-inc
                print 'Stopped working on: i=', i, ' previous_phase_inc=', (pre_phase_inc)*180/pi, ' current_phase_inc=', cur_phase_inc*180/pi
                if pre_phase_inc < (pi / self.M) and cur_phase_inc > (pi / self.M):
                    return
                raise self.FailureException('algorithm doesn\'t work as expected, phase increments dont match theory')
        raise self.FailureException('this should not occur?!?!')


if __name__ == '__main__':
    gr_unittest.run(qa_m_th_power_v1_cc, "qa_m_th_power_v1_cc.xml")
