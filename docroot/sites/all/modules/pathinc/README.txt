Pathinc
=======

This is an object-orientated replacement for the path.inc file in Drupal.

It allows module developers to override just the bit of path.inc they need to
and let pathinc module handle all the others.


Installation
------------

Just enable the pathinc module in the usual way.


Developers
----------

To change the actual class used for path handling, set the class name in the
`pathinc_class_name` variable. The Drupal registry is available when the class
is instantiated so specify the file containing the class in your info file in
the usual way.
