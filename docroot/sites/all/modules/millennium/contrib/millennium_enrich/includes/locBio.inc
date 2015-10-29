<?php

class locBio extends loc {

  // Regular expression to determine if an URL in a MARC 856 tag leads to a
  //   table of contents.
  // Example: http://catdir.loc.gov/catdir/enhancements/fy0624/2005043747-b.html
  public $regex = '/loc.gov.catdir.bios|loc.gov.catdir.enhancements.*-b.html/';

  // Header to show on node view
  public $viewHeader = "Collaborator biography";

  // Weight of $node->content element on node view
  public $viewWeight = 5;
}
