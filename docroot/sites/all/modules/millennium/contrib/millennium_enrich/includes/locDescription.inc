<?php

class locDescription extends loc {

  // Regular expression to determine if an URL in a MARC 856 tag leads to a
  //   description.
  // Example: http://www.loc.gov/catdir/description/wiley032/00020122.html
  public $regex = '/loc.gov.catdir.description|loc.gov.catdir.enhancements.*-d.html/';

  // Header to show on node view
  public $viewHeader = "Publisher's description"; // Example: "Table of contents"

  // Weight of $node->content element on node view
  public $viewWeight = 5;

}
