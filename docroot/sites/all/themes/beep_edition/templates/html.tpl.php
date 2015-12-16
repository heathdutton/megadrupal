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
?><!doctype html>
<!--[if lt IE 7 ]> <html class="no-js ie6" lang="en" dir="<?php print $language->dir; ?>" xmlns:fb="http://ogp.me/ns/fb#"> <![endif]-->
<!--[if IE 7 ]>    <html class="no-js ie7" lang="en" dir="<?php print $language->dir; ?>" xmlns:fb="http://ogp.me/ns/fb#"> <![endif]-->
<!--[if IE 8 ]>    <html class="no-js ie8" lang="en" dir="<?php print $language->dir; ?>" xmlns:fb="http://ogp.me/ns/fb#"> <![endif]-->
<!--[if (gte IE 9)|!(IE)]><!--> <html class="no-js" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>><!--<![endif]-->
<head>
<title><?php print $head_title; ?></title>
<?php print $head; ?>
<?php //print $appletouchicon; ?>
<meta http-equiv="cleartype" content="on">
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
  <!-- Mobile viewport optimized: j.mp/bplateviewport -->
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <?php //For third-generation iPad with high-resolution Retina display ?>
  <link rel="apple-touch-icon" sizes="144x144" href="<?php print base_path().path_to_theme(); ?>/apple-touch-icon-144x144.png">
  <?php //For iPhone with high-resolution Retina display ?>
  <link rel="apple-touch-icon" sizes="114x114" href="<?php print base_path().path_to_theme(); ?>/apple-touch-icon-114x114.png">
  <?php //For first- and second-generation iPad: ?>
  <link rel="apple-touch-icon" sizes="72x72" href="<?php print base_path().path_to_theme(); ?>/apple-touch-icon-72x72.png">
  <?php //For non-Retina iPhone, iPod Touch, and Android 2.1+ devices ?>
  <link rel="apple-touch-icon" href="<?php print base_path().path_to_theme(); ?>/apple-touch-icon.png">


<?php print $styles; ?>
<?php print $scripts; ?>
<script type="text/javascript">
  WebFontConfig = {
    google: { families: [ 'Roboto+Slab:400,700:latin', 'PT+Sans:700,400,700italic,400italic:latin' ] }
  };
  (function() {
    var wf = document.createElement('script');
    wf.src = ('https:' == document.location.protocol ? 'https' : 'http') +
      '://ajax.googleapis.com/ajax/libs/webfont/1/webfont.js';
    wf.type = 'text/javascript';
    wf.async = 'true';
    var s = document.getElementsByTagName('script')[0];
    s.parentNode.insertBefore(wf, s);
  })(); 
</script>
<noscript>
	<link href='http://fonts.googleapis.com/css?family=Roboto+Slab:400,700|PT+Sans:400,700,400italic,700italic' rel='stylesheet' type='text/css'>
</noscript>

<link href='http://fonts.googleapis.com/css?family=PT+Sans:400,700,400italic,700italic' rel='stylesheet' type='text/css'>
<!--[if lt IE 9 ]>
    <script src="<?php print base_path().path_to_theme(); ?>/js/respond.min.js"></script>
<![endif]-->
<!--[if lt IE 7 ]>
    <script src="<?php print base_path().path_to_theme(); ?>/js/dd_belatedpng.js"></script>
    <script>DD_belatedPNG.fix("img, .png_bg"); // Fix any <img> or .png_bg bg-images. Also, please read goo.gl/mZiyb </script>
<![endif]-->
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
<a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
<?php print $page_top; //stuff from modules always render first ?>
<?php print $page; // uses the page.tpl ?>

<?php print $page_bottom; //stuff from modules always render last ?>
</body>
</html>

