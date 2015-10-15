<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in page.tpl.php. Some may be left
 * blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */
?>
<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>

<div class="container">

  <header id="head" role="banner">
    <hgroup class="sixteen columns alpha">
       <div id="logo">
        <?php if ($logo): ?><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/></a><?php endif; ?>
        <?php if ($site_slogan): ?><div class="site-slogan"><?php print $site_slogan; ?></div><!--site slogan--><?php endif; ?>
        </div>
    </hgroup>
    
  </header>
</div>

<div class="container" id="content-contain">
 <div id="content" class="sixteen columns">
  <section id="main" role="main" class="clearfix">
    <?php print $messages; ?>
    <a id="main-content"></a>
    <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
    <?php print $content; ?>
  </section> <!-- /#main -->
 </div>
</div> 

<div id="copyright" class="container">
 <div class="credit"><?php print t('Copyright'); ?> &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?> | <?php print t('Theme by'); ?>  <a href="http://www.devsaran.com">Devsaran</a></div>
  <div class="clear"></div>
</div>

</body>
</html>
