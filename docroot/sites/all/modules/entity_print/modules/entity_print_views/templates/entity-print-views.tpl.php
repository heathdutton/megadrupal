<?php

/**
 * @file
 * PDF template for printing.
 *
 * Available variables are:
 *  - $view - The view itself.
 *  - $view_html - The rendered $view html.
 */
?>

<html>
<head>
  <title>PDF</title>
  <?php print drupal_get_css($entity_print_css);?>
</head>
<body>
<div class="page">
  <?php print $view_html; ?>
</div>
</body>
</html>
