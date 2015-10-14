IDNA Convert
---------------------

Generally speaking, the module provides two functions that allow to convert domain names
in Punycode (http://en.wikipedia.org/wiki/Punycode) to human-readable national domain names in Unicode and vice versa.

Examples of use:
print idna_convert_decode("xn--d1acufc.xn--p1ai"); // Prints "домен.рф".
print idna_convert_encode("домен.рф"); // Prints "xn--d1acufc.xn--p1ai".

This is a simple Drupal wrapper for IDNA Convert (http://www.phpclasses.org/browse/file/5845.html) class
of Matthias Sommerfeld.
IDNA Convert class is distributed under the Lesser General Public License (LGPL) 2.1
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
