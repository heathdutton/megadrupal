<?php

/**
 * @file
 * Template for widget html.
 */
?>

<!DOCTYPE html>
<html id="childId">
  <head lang="en">
    <meta charset="UTF-8">
    <title>
      <?php print render($title); ?>
    </title>
    <?php print render($css); ?>
    <?php print render($js); ?>
  </head>
  <body>
    <?php print render($content); ?>
  </body>
</html>
