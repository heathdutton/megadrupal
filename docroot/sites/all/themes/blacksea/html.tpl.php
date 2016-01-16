<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <?php require_once ('includes/region_layout.php');?>
  <?php 
      $style = theme_get_setting('style');
      $layout = theme_get_setting('layout');
      $fixedwidth = theme_get_setting('fixedwidth');
      $leftwidth = theme_get_setting('leftwidth');
      $rightwidth = theme_get_setting('rightwidth');
      $font_family = theme_get_setting('font_family');
	  $font_size = theme_get_setting('font_size');
	  $show_breadcrumb = theme_get_setting('show_breadcrumb');
  ?> 
  <?php require_once ('includes/fonts.php'); ?>
  <?php require_once ('includes/layout.php'); ?>	
  
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
  
</body>
</html>  

