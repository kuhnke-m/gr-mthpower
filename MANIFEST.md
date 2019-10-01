title: The KUHNKE OOT Module
brief: Feed Forward Frequency and Phase recovery for M-PSK
tags: # Tags are arbitrary, but look at CGRAN what other authors are using
  - sdr
  - m-psk
  - psk
  - estimation
  - correction
author:
  - Maximilian Kuhnke
copyright_owner:
  - Maximilian Kuhnke
license: GPL v3
repo: https://github.com/kuhnke-m/gr-mthpower.git
#website: <module_website> # If you have a separate project website, put it here
#icon: <icon_url> # Put a URL to a square image here that will be used as an icon on CGRAN
---
Implementation of the M-Th-power algorithm.
It can remove phase and frequency offsets for M-PSK Modulation formats after Symbol synchronisation.
E.g. it works on one sample per symbol at the optimum sampling instance.
Can be used to replace Costas Loops and similar.
You need to use differentially encoded signals because of phase ambiguity.
