<?php
/**
 * @file
 * adaptIC's implementation to display a page.
 */

 ?>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>"
lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
  <head>
    <?php print $head; ?>
    <title><?php print $head_title; ?></title>
    <?php print $styles; ?>
    <?php print $scripts; ?>

    <!--[if lt IE 9]>
      <script src="<?php print $base_path . $adaptic_path; ?>/js/shim.min.js"></script>
      <script src="<?php print $base_path . $adaptic_path; ?>/js/respond.min.js"></script>
    <![endif]-->

    <meta name = "viewport" content = "width = device-width">
  </head>
  <body class="<?php print $classes; ?>" <?php print $attributes;?>>
    <?php print $page_top; ?>
    <?php print $page; ?>
    <?php print $page_bottom; ?>
  </body>
</html>
