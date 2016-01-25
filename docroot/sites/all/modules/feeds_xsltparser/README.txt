Feeds XSLT Pipeline Parser
==========================

This module provides a Feeds parser plugin implementing simple XSLT pipelines.
XSLT is especially useful when importing document-style XML files.

Requirements & Installation
---------------------------

Make sure you have the following pieces of software available in your drupal
environment:

* Feeds
  http://drupal.org/project/feeds
* PHP XSL extension
  http://php.net/manual/book.xsl.php

Usage
-----

After installing and enabling the Feeds XSLT Pipeline Parser as well as the
Feeds UI module, follow these steps to setup an XSLT importer:

1. Navigate to "Structure" > "Feeds Importers" and click on the tab "New
   importer"
2. Setup basic settings and fetcher according to your needs
3. Change the parser to "XSLT Pipeline Parser"
4. Upload your XSLT stylesheets to sites/all/libraries/xslt_pipelines, one for
   each field you want to extract from the original document. You also may
   place folders containing XSLT files under the XSLT pipelines path. In this
   case the stylesheets will be executed in alphabetic order, each operating on
   the result of its precedessor.
5. You may change the XSLT pipelines path in the parser settings. This is
   especially useful if you want to setup multiple importers based on Feeds
   XSLT Pipeline Parser and want to use a different set of stylesheets for each
   of them.
6. Finally setup the mapping in the processor section

Refer to the feeds documentation for detailed instructions on how to setup and
customize importers.

Acknowledgments
---------------

This module was initially implemented by s_leu at MD Systems and sponsored by
the swiss weekly "Die Wochenzeitung".
