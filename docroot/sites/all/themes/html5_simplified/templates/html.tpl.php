<?php

/**
 * @file
 * Custom theme implementation to display the basic html structure of a single Drupal page.
 */

?>
<!DOCTYPE html>
<html lang="<?php print $language->language ?>" class="no-js">
  <head profile="<?php print $grddl_profile ?>">
    <meta http-equiv="X-UA-Compatible" content="IE=edge, chrome=1">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <?php print $head ?>
    <title><?php print $head_title ?></title>
    <?php print $styles ?>
    <?php print $scripts ?>
    <?php// Print $head_bottom ?>
  </head>
  <body class="<?php print $classes ?>"<?php print $attributes ?>>
   <div id="skel-layers-parent" style="position: absolute; left: 0px; top: 0px; min-height: 100%; width: 100%;"><div id="skel-layers-wrapper" style="position: absolute; left: 0px; right: 0px; top: 0px; transition: top 0.5s ease, right 0.5s ease, bottom 0.5s ease, left 0.5s ease, opacity 0.5s ease; -webkit-transition: top 0.5s ease, right 0.5s ease, bottom 0.5s ease, left 0.5s ease, opacity 0.5s ease; z-index: auto; width: auto; padding-bottom: 0px;">
    <?php print $page_top ?>
    <?php print $page ?>
    <?php print $page_bottom ?>
   <div>
  </body>
</html>
