<!DOCTYPE html>
<!--[if IEMobile 7]><html class="iem7" <?php print $html_attributes ?>><![endif]-->
<!--[if lte IE 6]><html class="lt-ie9 lt-ie8 lt-ie7" <?php print $html_attributes ?>><![endif]-->
<!--[if (IE 7)&(!IEMobile)]><html class="lt-ie9 lt-ie8" <?php print $html_attributes ?>><![endif]-->
<!--[if IE 8]><html class="lt-ie9" <?php print $html_attributes ?>><![endif]-->
<!--[if (gte IE 9)|(gt IEMobile 7)]><!--><html<?php print $html_attributes . $rdf_namespaces ?>><!--<![endif]-->

<head><!-- zp7-4.x -->
<title><?php print $head_title ?></title>
<?php if (theme_get_setting('grid_responsive')): ?>
<meta name="HandheldFriendly" content="true" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta name="MobileOptimized" content="width" />
<?php endif ?>
<?php print $head ?>
<?php print $styles ?>
<?php print $scripts ?>
</head>

<body id="<?php print $body_id ?>" class="<?php print $classes ?>" <?php print $attributes ?>>
  <div id="skip-link">
    <a href="#main" class="element-invisible element-focusable"><?php print t('Skip to main content') ?></a>
    <a href="#search-block-form" class="element-invisible element-focusable"><?php print t('Skip to search') ?></a>
  </div>

<?php print $page_top ?>
<?php print $page ?>
<?php print $page_bottom ?>
<?php print $page_b ?>

<!--[if IE 9]>
<script type="text/javascript" async src="<?php print $base_path . $path_to_zeropoint ?>/js/classList.min.js"></script>
<![endif]-->
<!--[if gte IE 9]><!-->
<script type="text/javascript" async src="<?php print $base_path . $path_to_zeropoint ?>/js/toggles.min.js"></script>
<!--<![endif]-->
</body>
</html>