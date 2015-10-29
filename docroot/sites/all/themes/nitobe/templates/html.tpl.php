<?php
// $Id: html.tpl.php,v 1.4 2010/11/16 15:36:36 shannonlucas Exp $
/**
 * @file
 * Provide's Nitobe's rendering for content contained in HTML.
 *
 * Variables:
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 * - $css: An array of CSS files for the current page.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or
 *   'rtl'.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $styles: Style tags necessary to import all CSS files for the page.
 *
 * @see nitobe_preprocess_html()
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php echo $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php echo $language->dir; ?>"<?php echo $rdf_namespaces; ?>>
<head profile="<?php echo $grddl_profile; ?>">
  <?php echo $head; ?>
  <title><?php echo $head_title; ?></title>
  <?php echo $styles; ?>
  <?php echo $scripts; ?>
</head>
<body class="<?php echo $classes; ?>" <?php echo $attributes;?>>
  <div class="element-invisible">
    <a href="#main-content"><?php echo t("Skip to main content"); ?></a>
  </div>
  <?php echo $page_top; ?>
  <?php echo $page; ?>
  <?php echo $page_bottom; ?>
</body>
</html>
