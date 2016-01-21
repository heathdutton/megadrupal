<?php

/**
 * @file
 * Default theme implementation to display the basic html structure of an
 * embedded Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language_dir contains the language direction. It will either be 'ltr' or
 *   'rtl'.
 * - $base_url: The base URL of the Drupal installation.
 * - $title: A modified version of the page title, for use in the TITLE tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $content: View content
 *
 * @see template_preprocess_views_share_preview()
 *
 * @ingroup themeable
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html lang="<?php print $language->language; ?>" dir="<?php print $language_dir; ?>">
<head>
  <?php print $head; ?>
  <title><?php print $title; ?></title>
  <base href="<?php print $base_url; ?>">
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="views-share-preview">

	<?php print $form; ?>

</body>
</html>
