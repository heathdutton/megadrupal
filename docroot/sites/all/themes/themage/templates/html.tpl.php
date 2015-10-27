<?php
/**
 * @file
 * Themage's implementation to display the basic html structure
 * of a single Drupal page.
 */
?>
<!DOCTYPE html>
<html<?php print $html_attributes; ?>>
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <nav id="skip-link" role="navigation">
    <a href="#main-content" class="element-invisible element-focusable"
      onclick="jQuery('#main-content').attr('tabIndex',-1).focus();">
        <?php print t('Skip to main content'); ?>
    </a>
  </nav>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
