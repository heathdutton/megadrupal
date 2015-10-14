<!DOCTYPE html>
<!--[if IEMobile 7]><html class="iem7" <?php print $html_attributes; ?>><![endif]-->
<!--[if lte IE 6]><html class="lt-ie9 lt-ie8 lt-ie7" <?php print $html_attributes; ?>><![endif]-->
<!--[if (IE 7)&(!IEMobile)]><html class="lt-ie9 lt-ie8" <?php print $html_attributes; ?>><![endif]-->
<!--[if IE 8]><html class="lt-ie9" <?php print $html_attributes; ?>><![endif]-->
<!--[if (gte IE 9)|(gt IEMobile 7)]><!--><html<?php print $html_attributes . $rdf_namespaces; ?>><!--<![endif]-->

<title><?php print $head_title ?></title>
<meta property="ver" content="bh7-3.2"/>
<?php print $head ?>
<?php print $styles ?>
<?php print $scripts ?>
</head>

<body id="<?php print $body_id; ?>" class="<?php print $classes; ?>" <?php print $attributes;?>>
<div id="skip-nav"><a href="#main"><?php print t('Jump to Navigation'); ?></a></div>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
  <?php print $page_b; ?>

</body>
</html>