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
<div id="header-top" class="clearfix">
<div id="logo"><!--start logo-->
<div id="site-name"><?php if ($site_name): ?><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><h1><?php print $site_name; ?></h1></a><?php endif; ?></div>
<div id="site-slogan"><?php if ($site_slogan): ?><?php print $site_slogan; ?><?php endif; ?></div><!--site slogan-->
</div><!--end logo-->
<?php if (theme_get_setting('social_icons')): ?>
<ul id="header-social">
<li><a href="http://www.twitter.com/<?php echo theme_get_setting('twitter_username'); ?>" target="_blank" rel="me"><img src="<?php global $base_url; echo $base_url.'/'.$directory.'/'; ?>/images/twitter.png" alt="twitter"/></a></li>
<li><a href="http://www.facebook.com/<?php echo theme_get_setting('facebook_username'); ?>" target="_blank" rel="me"><img src="<?php global $base_url; echo $base_url.'/'.$directory.'/'; ?>/images/facebook.png" alt="facebook"/></a></li>
</ul><!--end header-social-->
<?php endif; ?>
</div><!--end header-top-->
<div id="wrapper" class="clearfix">
<div id="maintenance">
  <section id="main" role="main" class="clearfix">
    <?php print $messages; ?>
    <a id="main-content"></a>
    <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
    <?php print $content; ?>
  </section> <!-- /#main -->
</div> <!-- /#maintenance -->
</div> <!-- /#wrapper -->

</body>
</html>