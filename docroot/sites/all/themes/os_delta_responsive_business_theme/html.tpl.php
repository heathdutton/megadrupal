<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">
<head>
  <?php global $base_path; global $base_root; ?>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>  
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link type="text/css" rel="stylesheet" href="<?php print $base_root . $base_path . path_to_theme() ?>/css/bootstrap.css" />
  <link type="text/css" rel="stylesheet" href="<?php print $base_root . $base_path . path_to_theme() ?>/css/bootstrap-responsive.css" media="screen" />
  <link type="text/css" rel="stylesheet" href="<?php print $base_root . $base_path . path_to_theme() ?>/css/font-awesome.css" />
  <link type="text/css" rel="stylesheet" href="<?php print $base_root . $base_path . path_to_theme() ?>/css/jquery.fancybox.css" />
  <!--[if lte IE 8]> <link type="text/css" rel="stylesheet" href="<?php print $base_root . $base_path . path_to_theme() ?>/css/lte-ie8.css" /> <![endif]-->
  <?php print $styles; ?>
  <?php print $scripts; ?>
<script type="text/javascript">jQuery.noConflict();</script>
<script type="text/javascript" src="<?php print $base_root . $base_path . path_to_theme() ?>/js/jquery-1.9.1.min.js"></script>
<script type="text/javascript" src="<?php print $base_root . $base_path . path_to_theme() ?>/js/jquery.tubular.1.0.js"></script>
<?php if(drupal_is_front_page()) { ?>
 <script type="text/javascript" src="<?php print $base_root . $base_path . path_to_theme() ?>/js/TweenMax.min.js"></script>
 <script type="text/javascript" src="<?php print $base_root . $base_path . path_to_theme() ?>/js/jquery.superscrollorama.js"></script>
 <script type="text/javascript" src="<?php print $base_root . $base_path . path_to_theme() ?>/js/front-custom.js"></script>
<?php } ?>
<script type="text/javascript" src="<?php print $base_root . $base_path . path_to_theme() ?>/js/bootstrap.js"></script>
<script type="text/javascript" src="<?php print $base_root . $base_path . path_to_theme() ?>/js/custom.js"></script>
<script type="text/javascript" src="<?php print $base_root . $base_path . path_to_theme() ?>/js/jquery.fancybox.js"></script>

<style type="text/css">
  body {
    background-image: url(<?php print $base_root . $base_path . path_to_theme() . "/images/" . $layout_pattern?>.png);
    background-repeat: repeat;
    font-family: <?php print $body_font ?>;
  }
  a {
    font-family: <?php print $body_links_font ?>;
    text-decoration: <?php print (($b_decor == 1) ? 'underline' : 'none'); ?>;
  }
  a:hover {text-decoration: <?php print (($b_decor_hover == 1) ? 'underline' : 'none'); ?>;}
  .mainMenu li a {
    font-family: <?php print $main_menu_font ?>;
    text-decoration: <?php print (($m_decor == 1) ? 'underline' : 'none'); ?>;
  }
  .mainMenu li a:hover {text-decoration: <?php print (($m_decor_hover == 1) ? 'underline' : 'none'); ?>;}
  #footer a {
    font-family: <?php print $footer_links_font ?>;
    text-decoration: <?php print (($f_decor == 1) ? 'underline' : 'none'); ?>;
  }
  #footer a:hover {text-decoration: <?php print (($f_decor_hover == 1) ? 'underline' : 'none'); ?>;}
  
  h1 {font-family: <?php print $h1_font ?>;}
  h2 {font-family: <?php print $h2_font ?>;}
  h3 {font-family: <?php print $h3_font ?>;}
  h4 {font-family: <?php print $h4_font ?>;}
  h5 {font-family: <?php print $h5_font ?>;}
  h6 {font-family: <?php print $h6_font ?>;}
</style>
</head>

<body class="<?php print $classes;?>" <?php print $attributes;?>>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>