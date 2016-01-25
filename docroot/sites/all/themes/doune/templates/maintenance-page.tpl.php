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
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>">
  <div id="page" class="<?php print $page_classes; ?>">
    <header role="banner">
      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>

      <?php if ($site_name || $site_slogan): ?>
        <hgroup id="name-and-slogan">
          <?php if ($site_name): ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <h2 id="site-slogan"><?php print $site_slogan; ?></h2>
          <?php endif; ?>
        </hgroup> <!-- /#name-and-slogan -->
      <?php endif; ?>

      <?php print $secondary_menu_rendered; ?>

      <?php print render($header); ?>

    </header> <!-- /header -->

    <?php print $messages; ?>

    <hr class="top" />

    <div id="main-wrapper">
      <div id="main" role="main" class="clearfix">
        <?php print render($highlighted); ?>

        <div id="title-tabs-content-wrapper" class="<?php print $title_tabs_content_wrapper_classes; ?>">
          <?php print render($title_prefix); ?>
          <?php if ($title): ?><h1 id="page-title" class="<?php print $title_classes; ?>"<?php print $title_attributes; ?>><?php print $title; ?></h1><?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs = render($tabs)): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
          <?php print render($help); ?>
          <?php if ($action_links = render($action_links)): ?><ul class="action-links"><?php print $action_links; ?></ul><?php endif; ?>
          <a id="main-content"></a>
          <div class="main-content">
            <?php print render($content_top); ?>
            <?php print render($content); ?>
            <?php print render($content_bottom); ?>
            <?php print $feed_icons; ?>
          </div>
        </div>

        <?php print render($sidebar_first); ?>

        <?php print render($sidebar_second); ?>

      </div> <!-- /main -->
    </div> <!-- /#main -->

    <hr class="bottom" />

    <?php if ($navigation || $main_menu): ?>
      <nav id="nav" role="navigation">
        <?php print $main_menu_rendered; ?>
        <?php print render($navigation); ?>
      </nav> <!-- /nav -->
    <?php endif; ?>

    <?php print render($footer); ?>

  </div> <!-- /#page -->
</body>
</html>
