This folder contains templates that can be used in replacement of Drupal and
Groundwork templates.

* block-content-with-wrapper.tpl.php

Wraps the block's content in a div for easy styling. Copy the file
to the template folder of your subtheme and rename to block.tpl.php


* page-with-wrapper.tpl.php

  Wraps the #topbar, #header, #main-menu, #navigation,
  #billboard, #main, #supplement, #appendix, and #footer sections in thier own div.wrapper.
  The wrappers can be used to style these containers with a full-width background. Copy the file
  to the template folder of your subtheme and rename to page.tpl.php


* region-no-wrapper.tpl.php

  In creating additional regions with the desire to not wrap the blocks in div,
  this template can be used. Copy the file to the template folder of the
  subtheme  and rename to region--regionname.tpl.php (where regionname is the
  name of the region). Groundwork uses this template by default for the
  highlighted, content, and footer regions.
