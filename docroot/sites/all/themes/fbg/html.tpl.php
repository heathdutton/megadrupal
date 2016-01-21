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
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>
<head profile="<?php print $grddl_profile; ?>">
  <!--
  *****************************************************************
    FBG - Fluid Baseline Grid - http://drupal.org/project/fbg
	A Drupal 7 theme by Jason More and Arbor Web Development, http://arborwebdev.com.
	FBG Drupal theme is based on Fluid Baseline Grid v1.0.1.
    Fluid Baseline Grid was Designed & Built by Josh Hopkins and 40 Horse, http://40horse.com.
    FBG is Licensed under Unlicense, http://unlicense.org/.
  *****************************************************************
  -->
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1"/>
  <!-- Optimized mobile viewport -->
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <!-- Place favicon.ico and apple-touch-icon.png in root directory -->
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <link href="/<?php print path_to_theme(); ?>/css/style.css" rel="stylesheet" />
  <?php print $scripts; ?>
</head>
<body>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <!-- JavaScript at the bottom for fast page loading -->
  <!-- Minimized jQuery from Google CDN -->
  <script src="https://ajax.googleapis.com/ajax/libs/jquery-ui/1.8.21/jquery-ui.min.js"></script>
  <!-- HTML5 IE Enabling Script -->
  <!--[if lt IE 9]><script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script><![endif]-->
  <!-- CSS3 Media Queries -->
  <script src="<?php print path_to_theme();?>/js/respond.min.js"></script>
  <!-- Optimized Google Analytics. Change UA-XXXXX-X to your site ID -->
  <script>var _gaq=[['_setAccount','UA-XXXXX-X'],['_trackPageview']];(function(d,t){var g=d.createElement(t),s=d.getElementsByTagName(t)[0];g.src='//www.google-analytics.com/ga.js';s.parentNode.insertBefore(g,s)}(document,'script'))</script>
  <?php print $page_bottom; ?>
</body>
</html>