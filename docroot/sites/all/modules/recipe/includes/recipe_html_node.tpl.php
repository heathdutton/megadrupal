<?php
/**
 * @file
 * Default theme implementation for html version of recipe nodes.
 */
?>
<!DOCTYPE html>
<html>
  <head>
    <title><?php print $title; ?></title>
    <?php print $styles ?>
  </head>
  <body>
    <h2><?php print $title; ?></h2>
    <hr/>
    <?php print $contents; ?>
  </body>
</html>
