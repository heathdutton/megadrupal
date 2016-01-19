<?php

class locSampleText extends loc {

  // Regular expression to determine if an URL in a MARC 856 tag leads to a
  //   sample text page.
  // Example: http://www.loc.gov/catdir/samples/cam031/94044080.html
  // TODO: Some 856s might link directly to PDFs, how would we handle those?
  public $regex = '/loc.gov.catdir.samples.*html/';

  // Header to show on node view
  public $viewHeader = "Sample text"; // Example: "Table of contents"

  // Weight of $node->content element on node view
  public $viewWeight = 5;

}
