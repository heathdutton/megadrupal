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
 *   $language->dir contains the language direction. It will either be 'ltr' or
 *   'rtl'.
 * - $base_url: The base URL of the Drupal installation.
 * - $title: A modified version of the page title, for use in the TITLE tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $content: Node content defined in "embed" view_mode.
 *
 * @see template_preprocess_share_this_thing_html()
 *
 * @ingroup themeable
 */
?><!doctype html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
  <?php print $head; ?>
  <title><?php print $title; ?></title>
  <base href="<?php print $base_url; ?>">
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="share-this-thing">

  <?php print $content; ?>

</body>
</html>
