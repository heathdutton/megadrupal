<?php

/**
 * @file
 * PageFlip MegaZine3 Viewer XML document/book template. Renders the
 * entire XML document (most of which is in $content).
 */
?>
<?php print '<?' ?>xml version="1.0" encoding="utf-8"?>
<!DOCTYPE book SYSTEM "http://megazine.mightypirates.de/megazine2.dtd">
<book <?php print $book_attributes ?>>
  <?php print $content ?>
</book>

