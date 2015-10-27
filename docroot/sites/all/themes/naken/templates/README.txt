Place your custom templates on this folder to override Drupal and Groundwork
templates.

For your convenience, Groundwork has provided some ready-made custom
templates which you can use. These templates can be found at:
groundwork/resources/templates

* page-with-wrapper.tpl.php

  Adds wrappers to the #topbar, #header, #main-menu, #navigation,
  #billboard, #main, #supplement, #appendix, #rider, and #footer sections. The
  wrappers can be used to style these containers with a full-width background.
  Copy the file to this folder and rename to page.tpl.php

* block-content-with-wrapper.tpl.php

  Wraps the block contents with <div class="block-content"></div>. Use if you
  want to target the style of block contents. Copy the file to this folder and
  rename to block.tpl.php

* region-no-wrapper.tpl.php

  In creating additional regions with the desire to not wrap the blocks in div,
  this template can be used. Copy the file to this folder and rename to
  region--regionname.tpl.php (where regionname is the name of the region).
  Groundwork uses this template by default for the highlighted, content, and
  footer regions.