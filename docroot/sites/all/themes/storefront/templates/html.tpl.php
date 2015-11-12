<?php
/**
 * @file
 * The HTML frame around the website.
 *
 * @param $html_attributes
 *  Attributes of the HTML frame of the page.
 * @param $head_title
 *  Output of the page title in the browser window.
 * @param $styles
 *  Prints drupal css files.
 * @param $scripts
 *  Prints drupal js files.
 * @param $classes
 *  Classes for the body element.
 * @param $attributes
 *  Additional attributes added to the body element.
 * @param $page_top
 *  Output from drupal and other modules that is required to come before the
 *  page load.
 * @param $page_bottom
 *  Output from drupal and other modules that is required to come after the
 *  page load.
 * @param $page
 *  The output of the page.tpl.php file.
 */

?>
<!DOCTYPE html>
<!--[if IEMobile 7]><html class="iem7" <?php print $html_attributes; ?>><![endif]-->
<!--[if (lte IE 6)&(!IEMobile)]><html class="ie6 ie6-7 ie6-8" <?php print $html_attributes; ?>><![endif]-->
<!--[if (IE 7)&(!IEMobile)]><html class="ie7 ie6-7 ie6-8" <?php print $html_attributes; ?>><![endif]-->
<!--[if (IE 8)&(!IEMobile)]><html class="ie8 ie6-8" <?php print $html_attributes; ?>><![endif]-->
<!--[if (gte IE 9)|(gt IEMobile 7)]><!--><html <?php print $html_attributes . $rdf_namespaces; ?>><!--<![endif]-->
<head>
<?php print $head; ?>
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="MobileOptimized" content="width">
<meta name="HandheldFriendly" content="true">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta http-equiv="cleartype" content="on">
<meta http-equiv="X-UA-Compatible" content="IE=edge, chrome=1">
<title><?php print $head_title; ?></title>
<?php print $styles; ?>
<?php print $scripts; ?>
<!--[if lte IE 9]>
  <script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
  <link rel="stylesheet" type="text/css"
        href="<?php print base_path() . path_to_theme();?>/stylesheets/ie.css">
<![endif]-->
</head>
<body class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
