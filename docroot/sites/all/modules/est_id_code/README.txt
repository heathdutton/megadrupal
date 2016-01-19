INTRODUCTION
------------

This module provides an Estonian Personal identification code (isikukood) field
type. More details http://en.wikipedia.org/wiki/National_identification_number#Estonia

The code form is GYYMMDDSSSC, where G shows sex and century of birth
(odd number male, even number female, 1-2 19th century,
3-4 20th century, 5-6 21st century), SSS is a serial number separating
persons born on the same date and C a checksum.

This module has checksum validation. Also there are two widgets and displaying
formatters:
  - Plain, this is just the code without any whitespace or delimiters, the usual
  way of representing the code,
  - Formatted, the code is broken into segments: G YY MM DD SSS C.


INSTALLATION
------------

Standard Drupal module installation.

Because there are privacy issues concerning ID codes, there are also permissions
to view and edit codes, set them accordingly.
