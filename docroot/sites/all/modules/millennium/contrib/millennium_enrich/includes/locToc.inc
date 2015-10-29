<?php

class locToc extends loc {

  // Regular expression to determine if an URL in a MARC 856 tag leads to a
  //   table of contents.
  // Example: $regex = "/loc.gov.catdir.toc|loc.gov.catdir.enhancements.*-t.html/";
  // http://catdir.loc.gov/catdir/toc/ecip0710/2007003576.html
  public $regex = "/loc.gov.catdir.toc|loc.gov.catdir.enhancements.*-t.html/";

  // Header to show on node view
  public $viewHeader = "Table of contents"; // Example: "Table of contents"

  // Weight of $node->content element on node view
  public $viewWeight = 5;

}
