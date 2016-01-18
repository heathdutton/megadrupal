<?php
/**
 * @file
 * Min theme implementation#.
 */
?>

<!DOCTYPE html>
<!--[if lt IE 7]><html class="no-js lt-ie9 lt-ie8 lt-ie7"> <![endif]-->
<!--[if IE 7]><html class="no-js lt-ie9 lt-ie8"> <![endif]-->
<!--[if IE 8]><html class="no-js lt-ie9"> <![endif]-->
<!--[if gt IE 8]><!--><html class="no-js"><!--<![endif]-->
  <head>
  	<meta charset="utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title><?php print $head_title; ?></title>
    <meta name="description" content="">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" type="image/x-icon" href="<?php print path_to_theme() ?>/favicon.ico"/>
		<!-- START Styles & Fonts  -->
		<?php print $styles; ?>
		<link href='http://fonts.googleapis.com/css?family=Medula+One' rel='stylesheet' type='text/css'>
		<link href='http://fonts.googleapis.com/css?family=Roboto+Slab:100' rel='stylesheet' type='text/css'>
		<!-- END Styles & Fonts  -->
  </head>
  <body class="<?php print $classes; ?>" <?php print $attributes;?>>
    <div id="skip-link"><a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a></div>
    <?php print $page_top; ?>
		<?php print $page; ?>
		<?php print $page_bottom; ?>
		<!-- START BODY SCRIPTS -->
    <?php print $scripts; ?>
    <!-- END BODY SCRIPTS -->
  </body>
</html>
