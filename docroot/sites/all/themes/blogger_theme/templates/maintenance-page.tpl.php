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
  <!--[if lt IE 9]><script src="<?php print base_path() . drupal_get_path('theme', 'blogger_theme') . '/js/html5.js'; ?>"></script><![endif]-->
</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>


<div id="wrap" class="clr container">
  <div id="header-wrap" class="clr">
    <header id="header" class="site-header clr">
      <div id="logo" class="clr">
        <?php if (theme_get_setting('image_logo','blogger_theme')): ?>
        <?php if ($logo): ?><div class="site-img-logo clr"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a></div><?php endif; ?>
        <?php else: ?>
        <div class="site-text-logo clr">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
          <?php if ($site_slogan): ?><div class="blog-description"><?php print $site_slogan; ?></div><?php endif; ?>
        </div>
        <?php endif; ?>
      </div>
    </header>
  </div>

  <div id="main" class="site-main clr">
    <div id="primary" class="content-area clr">
      <section id="content" role="main" class="site-content clr">
        <div id="content-wrap">
          <?php print $messages; ?>
          <a id="main-content"></a>
          <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
          <?php print $content; ?><br/><br/>
        </div>
      </section> <!-- /#main -->
    </div>
  </div>
  
</div>

</body>
</html>
