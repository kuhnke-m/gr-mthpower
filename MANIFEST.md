title: The KUHNKE OOT Module
brief: Short description of gr-kuhnke
tags: # Tags are arbitrary, but look at CGRAN what other authors are using
  - sdr
author:
  - Maximilian Kuhnke
copyright_owner:
  - Maximilian Kuhnke
license: GPL v3
repo: https://github.com/kuhnke-m/gr-kuhnke.git
#website: <module_website> # If you have a separate project website, put it here
#icon: <icon_url> # Put a URL to a square image here that will be used as an icon on CGRAN
---
Implementation of the M-Th-power algorithm.
It can remove phase and frequency offsets for M-PSK Modulation formats after Symbol synchronisation.
E.g. it works on one sample per symbol at the optimum sampling instance.
Can be used to replace Costas Loops and similar.
You need to use differentially encoded signals because of phase ambiguity.
