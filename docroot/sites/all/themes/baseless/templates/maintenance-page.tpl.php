<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */
?>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <!--[if lt IE 9]><script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script><![endif]-->	
</head>
<body class="<?php print $classes; ?>">
<div id="page-wrapper">

    <div id="header" class="section"><div class="limiter clearfix">

      <?php if ($logo): ?><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a><?php endif; ?>

      <?php if ($site_name): ?>
         <?php if ($title): ?>
            <div id="site-name"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a></div>
         <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a></h1>
         <?php endif; ?>
      <?php endif; ?>

      <?php if ($site_slogan): ?><div id="site-slogan"><?php print $site_slogan; ?></div><?php endif; ?>

      <?php print render($page['header']); ?>

    </div></div> <!-- /.limiter, /#header -->
   
  <div id="main-content" class="section"><div class="limiter clearfix">

    <div id="content" class="content-column">
      <a id="main-content"></a>
      <?php print render($title_prefix); ?><?php if ($title): ?><h1 id="page-title"><?php print $title; ?></h1><?php endif; ?><?php print render($title_suffix); ?>
      <?php if ($tabs): ?><div id="page-tabs"><?php print render($tabs); ?></div><?php endif; ?>
      <?php print render($page['help']); ?>
      <?php print $messages; ?>

      <?php print render($page['content']); ?>
    </div><!-- /#content -->
  </div></div> <!-- /.limiter, /#main-content -->

  <div id="footer" class="section"><div class="limiter clearfix">
    <?php print render($page['footer']); ?>
  </div></div> <!-- /.limiter, /#footer -->

</div> <!-- /#page-wrapper -->

</body>
</html>
