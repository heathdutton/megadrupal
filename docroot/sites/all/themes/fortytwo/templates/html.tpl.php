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
?>
<!DOCTYPE html>
<!--[if IE 7]><html class="no-js lt-ie10 lt-ie9 lt-ie8" lang="<?php print $language->language; ?>" data-staticurl="<?php print $staticpath; ?>"><![endif]-->
<!--[if IE 8]><html class="no-js lt-ie10 lt-ie9" lang="<?php print $language->language; ?>" data-staticurl="<?php print $staticpath; ?>"><![endif]-->
<!--[if IE 9]><html class="no-js lt-ie10 " lang="<?php print $language->language; ?>" data-staticurl="<?php print $staticpath; ?>"><![endif]-->
<!--[if gt IE 9]><!--><html class="no-js" lang='<?php print $language->language; ?>' data-staticurl="<?php print $staticpath; ?>"><!--<![endif]-->

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>

<?php if ($grid) : ?>
  <div id="svg-grid-background">
    <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" x="0px" y="0px" width="1218px" height="100%" enable-background="new 0 0 1218 100%" xml:space="preserve">

      <rect x="0"   fill="#29ABE2" width="5" height="100%"/>
      <rect x="10"  fill="#29ABE2" width="5" height="100%"/>
      <rect x="20"  fill="#29ABE2" width="5" height="100%"/>
      <rect x="30"  fill="#29ABE2" width="5" height="100%"/>
      <rect x="40"  fill="#29ABE2" width="5" height="100%"/>
      <rect x="50"  fill="#29ABE2" width="5" height="100%"/>
      <rect x="60"  fill="#29ABE2" width="5" height="100%"/>
      <rect x="70"  fill="#29ABE2" width="5" height="100%"/>
      <rect x="80"  fill="#29ABE2" width="5" height="100%"/>
      <rect x="90"  fill="#29ABE2" width="5" height="100%"/>
      <rect x="100" fill="#29ABE2" width="5" height="100%"/>
      <rect x="110" fill="#29ABE2" width="5" height="100%"/>
      <rect x="120" fill="#29ABE2" width="5" height="100%"/>
      <rect x="130" fill="#29ABE2" width="5" height="100%"/>
      <rect x="140" fill="#29ABE2" width="5" height="100%"/>
      <rect x="150" fill="#29ABE2" width="5" height="100%"/>
    </svg>
  </div>
<?php endif; ?>

  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>

  <?php if ($responsive_identifier) : ?>
    <div class="responsive-identifier"></div>
  <?php endif; ?>

</body>
</html>
