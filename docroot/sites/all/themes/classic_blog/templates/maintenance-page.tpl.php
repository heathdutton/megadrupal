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
  <!--[if lt IE 9]><script src="<?php print base_path() . drupal_get_path('theme', 'classic_blog') . '/js/html5.js'; ?>"></script><![endif]-->
</head>

<body class="<?php print $classes; ?>" <?php print $attributes;?>>


<div id="page" class="container_6">
  <div id="main" class="site-main cf">
    <aside id="secondary" class="widget-area grid_2" role="complementary">&nbsp;</aside>
    <div id="primary" class="content-area grid_4">
      <header id="masthead" class="site-header cf" role="banner">
        <div class="top clearfix">
          <div class="logo">
            <?php if (theme_get_setting('image_logo','classic_blog')): ?>
              <?php if ($logo): ?><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
                <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
              </a><?php endif; ?>
            <?php else: ?>
              <h1 class="site-title">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
              </h1>
            <?php endif; ?>
          </div>
        </div>
      </header>
      <section id="content" class="site-content" role="main">
        <?php print $messages; ?>
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