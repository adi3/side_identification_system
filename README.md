Side Identification System for LMRST-Sat
==========================

A solution that unambiguously calculates the kinematic attributes of an orbiting CubeSat facing a ground station at any given instant.

Four modulated ASCII characters unique to the four lateral sides of a CubeSat are transmitted through IR LEDs, and converted from TTL to USB signals by the receiving end for computer interpretation. The ASCII character corresponding to the lateral face that is pointing at the receiver is then printed on the ground station's computer screen. Based on these patterns of ASCII characters, the instantaneous spin rate, spin direction and angular acceleration of the CubeSat can be calculated.