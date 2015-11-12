<?php

/**
 * @file
 * Default theme implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head_title_array: (array) An associative array containing the string parts
 *   that were used to generate the $head_title variable, already prepared to be
 *   output as TITLE tag. The key/value pairs may contain one or more of the
 *   following, depending on conditions:
 *   - title: The title of the current page, if any.
 *   - name: The name of the site.
 *   - slogan: The slogan of the site, if any, and if there is no title.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 *
 * @ingroup themeable
 */
?><!DOCTYPE html>
<!--[if lt IE 9]><html class="no-js lt-ie9" lang="<?php print $language->language; ?>" dir="ltr"><![endif]--><!--[if gt IE 8]><!-->
<html class="no-js" lang="<?php print $language->language; ?>" dir="ltr">
<!--<![endif]-->

<head>
  <meta charset="utf-8">
  <meta content="width=device-width,initial-scale=1" name="viewport">
  <?php print $head; ?>
<!-- Meta data-->
 <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!--[if gte IE 9 | !IE ]><!-->
  <link rel="stylesheet" href="/sites/all/themes/wet4/css/wet-boew.min.css">
  <!--<![endif]-->
  <link rel="stylesheet" href="/sites/all/themes/wet4/css/theme.min.css">
  <!--[if lt IE 9]>
  <link href="/sites/all/themes/wet4/assets/favicon.ico" rel="shortcut icon" />
  <link rel="stylesheet" href="/sites/all/themes/wet4/css/ie8-wet-boew.min.css" />
  <link rel="stylesheet" href="/sites/all/themes/wet4/css/ie8-theme.min.css" />
  <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
  <script src="/sites/all/themes/wet4/js/ie8-wet-boew.min.js"></script>
  <![endif]-->
  <noscript><link rel="stylesheet" href="/sites/all/themes/wet4/css/noscript.min.css" /></noscript>
</head>
<body vocab="http://schema.org/" typeof="WebPage" class="<?php print $classes; ?>">
<ul id="wb-tphp">
  <li class="wb-slc">
    <a class="wb-sl" href="#wb-cont">Skip to main content</a>
  </li>
  <li class="wb-slc visible-sm visible-md visible-lg">
    <a class="wb-sl" href="#wb-info">Skip to "About this site"</a>
  </li>
</ul>
<?php print $page_top; ?>
<?php print $page; ?>
<?php print $page_bottom; ?>
<!--[if gte IE 9 | !IE ]><!-->
<script src="http://ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js"></script>
<script src="/sites/all/themes/wet4/js/wet-boew.min.js"></script>
<!--<![endif]-->
<!--[if lt IE 9]>
<script src="/sites/all/themes/wet4/js/ie8-wet-boew2.min.js"></script>
<![endif]-->
</body>
</html>