<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 */
?>

  <div id="page" class="<?php print $classes; ?>">
    <header id="header" role="banner">
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

      <?php print render($page['header']); ?>

    </header> <!-- /header -->

    <?php print $messages; ?>

    <div id="main-wrapper">

      <div id="main" role="main" class="clearfix">
        <?php print render($page['highlighted']); ?>
        <?php if ($breadcrumb): ?>
          <nav id="breadcrumb"><?php print $breadcrumb; ?></nav>
        <?php endif; ?>

        <div id="title-tabs-content-wrapper" class="<?php print $title_tabs_content_wrapper_classes; ?>">
          <?php print render($title_prefix); ?>
          <?php if ($title): ?><h1 id="page-title" class="<?php print $title_classes; ?>"<?php print $title_attributes; ?>><?php print $title; ?></h1><?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs = render($tabs)): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links = render($action_links)): ?><ul class="action-links"><?php print $action_links; ?></ul><?php endif; ?>
          <a id="main-content"></a>
          <?php print render($page['content_top']); ?>
          <?php print render($page['content']); ?>
          <?php print render($page['content_bottom']); ?>
          <?php print $feed_icons; ?>
        </div>

        <?php if ($page['navigation'] || $main_menu): ?>
          <nav id="nav" role="navigation">
            <?php print $main_menu_rendered; ?>
            <?php print render($page['navigation']); ?>
          </nav> <!-- /nav -->
        <?php endif; ?>

        <?php print render($page['sidebar_first']); ?>

        <?php print render($page['sidebar_second']); ?>

      </div> <!-- /main -->
    </div> <!-- /#main -->

    <?php print render($page['footer']); ?>

  </div> <!-- /#page -->
