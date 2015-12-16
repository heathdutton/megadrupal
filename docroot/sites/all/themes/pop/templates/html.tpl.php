<?php
/**
 * @file
 *
 * For more info on Drupal default for this template, refer to
 * http://api.drupal.org/api/drupal/modules--system--html.tpl.php/7
 */
?>
<?php print $doctype; ?>
<!-- HTML5 Mobile Boilerplate -->
<!--[if IEMobile 7]> <html class="no-js iem7" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $rdf->version . $rdf->namespaces; ?>><![endif]-->
<!--[if (gt IEMobile 7)|!(IEMobile)]><!--><html class="no-js" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $rdf->version . $rdf->namespaces; ?>><!--<![endif]-->

<!-- HTML5 Boilerplate -->
<!--[if lt IE 7]> <html class="no-js lt-ie9 lt-ie8 lt-ie7" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $rdf->version . $rdf->namespaces; ?>> <![endif]-->
<!--[if (IE 7)&!(IEMobile)]> <html class="no-js lt-ie9 lt-ie8" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $rdf->version . $rdf->namespaces; ?>> <![endif]-->
<!--[if (IE 8)&!(IEMobile)]>    <html class="no-js lt-ie9" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $rdf->version . $rdf->namespaces; ?>> <![endif]-->
<!--[if gt IE 8]><!--> <html class="no-js" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>" <?php print $rdf->version . $rdf->namespaces; ?>> <!--<![endif]-->
<head<?php print $rdf->profile; ?>>

  <?php print $head; ?>
  <title><?php print $head_title; ?></title>

  <!-- Prevent blocking -->
  <!--[if IE 6]><![endif]-->

  <?php print $styles; ?>
  <?php print $scripts; ?>

<!--[if lt IE 9]>
  <script src="//html5shiv.googlecode.com/svn/trunk/html5.js"></script>
<![endif]-->

<!--[if (lt IE 9) & (!IEMobile)]>
  <script src="<?php print base_path() . drupal_get_path('theme', 'pop'); ?>/scripts/selectivizr-min.js"></script>
<![endif]-->

</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <!--[if lt IE 7]><p class=chromeframe>Your browser is <em>ancient!</em> <a href="http://browsehappy.com/">Upgrade to a different browser</a> or <a href="http://www.google.com/chromeframe/?redirect=true">install Google Chrome Frame</a> to experience this site.</p><![endif]-->

  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>

<!--[if (lt IE 9) & (!IEMobile)]>
  <script src="<?php print base_path() . drupal_get_path('theme', 'pop'); ?>/scripts/imgsizer.js"></script>
<![endif]-->

</body>
</html>
