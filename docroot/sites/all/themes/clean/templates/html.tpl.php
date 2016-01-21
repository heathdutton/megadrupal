<?php
/**
 * @file
 * Default theme implementation to display the basic HTML structure of a page.
 */
?>
<?php print $doctype; ?>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body<?php print $attributes; ?>>
  <?php print render($skip_link); ?>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
