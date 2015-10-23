reStructuredText filter module
==============================

Provides reStructuredText filter integration for Drupal input formats. The
reStructuredText syntax is designed to co-exist with HTML, so you can set up
input formats with both HTML and reStructuredText support. It is also meant to
be as human-readable as possible when left as "source".

Read more about reStructuredText at <http://docutils.sourceforge.net/rst.html>

Dependencies
------------

https://www.drupal.org/project/xautoload

Important note about running Markdown with other input filters:
--------------------------------------------------------------

reStructuredText may conflict with other input filters, depending on the order
in which filters are configured to apply. 

Installation
------------
1. Download and unpack the reStructuredText module directory in your modules folder
   (this will usually be "sites/all/modules/").
2. Go to "Administer" -> "Modules" and enable the module.
3. Set up a new text format or add reStructuredText support to an text format at
   Administer -> Configuration -> Content Authoring -> Text formats

Credits
-------

RST <https://github.com/Gregwar/RST> By Grégoire Passault <http://www.gregwar.com>

Current maintainer
------------------

Qiangjun Ran (jungle) <http://ranqiangjun.com>