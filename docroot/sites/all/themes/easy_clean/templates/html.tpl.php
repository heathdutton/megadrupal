<?php
// $Id$

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
 * - $head_title: A modified version of the page title, for use in the TITLE tag.
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
$html_attributes = ' lang="' . $language->language . '" dir="' . $language->dir . '"';
?>
<!DOCTYPE html>
<!--[if IE 7 ]><html<?php print $html_attributes; ?> class="no-js ie7"><![endif]-->
<!--[if IE 8 ]><html<?php print $html_attributes; ?> class="no-js ie8"><![endif]-->
<!--[if IE 9 ]><html<?php print $html_attributes; ?> class="no-js ie9"><![endif]-->
<!--[if gt IE 9]><!--><html<?php print $html_attributes; ?> class="no-js"><!--<![endif]-->
<head>
<?php print $head; ?>
  
<!-- Always force latest IE rendering engine (even in intranet) & Chrome Frame  -->
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">

<!--  Mobile viewport optimized: j.mp/bplateviewport -->
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<!-- Prevent blocking -->
<!--[if IE 6]><![endif]-->

<title><?php print $head_title; ?></title>
<?php print $styles; ?>
<?php print $scripts; ?>
<!--[if lt IE 9]>
  <script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
<![endif]-->
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
<?php print $page_top; ?> <?php print $page; ?> <?php print $page_bottom; ?>
</body>
</html>
