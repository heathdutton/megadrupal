<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 */
?>

  <div role="banner" class="masthead clearfix">
    <div class="mast">
      <div class="block logo-container">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="logo block">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        <?php endif; ?>
        <?php if ($site_name): ?>
          <h1 class="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
          </h1>
        <?php endif; ?>
        <?php if ($site_slogan): ?>
          <h2 class="site-slogan block"><?php print $site_slogan; ?></h2>
        <?php endif; ?>
      </div> <!-- end .logo-container -->

      <?php print render($page['header']); ?>

      <?php if ($main_menu_links): ?>
        <nav class="block primary-menu" role="navigation">
          <?php print $main_menu_links; ?>
        </nav>
      <?php endif; ?>

      <?php if ($secondary_menu_links): ?>
        <nav class="block secondary-menu" role="navigation">
          <?php print $secondary_menu_links; ?>
        </nav>
      <?php endif; ?>
    </div><!-- end .mast -->
  </div>

  <div id="page" class="mast">

    <?php print render($page['preblocks']); ?>

    <?php if ($breadcrumb): ?>
      <nav class="breadcrumb block boxed"><?php print $breadcrumb; ?></nav>
    <?php endif; ?>

    <?php print $messages; ?>

    <div class="wrap-columns clearfix">

      <div class="content-column">
        <div role="main" class="main">
        <?php print render($page['highlighted']); ?>
        <a class="main-content"></a>
        <?php print render($page['featured']); ?>
        <?php print render($page['content_top']); ?>
        <?php if ($tabs = render($tabs)): ?><div class="block tabs"><?php print $tabs; ?></div><?php endif; ?>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?><h1 class="block boxed title page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links = render($action_links)): ?><ul class="block action-links"><?php print $action_links; ?></ul><?php endif; ?>
        <?php print render($page['content']); ?>
        <?php print render($page['content_bottom']); ?>
        <div class="feed-icons block"><?php print $feed_icons; ?></div>
        </div> <!-- end .main -->
      </div> <!-- end .content-column -->

      <?php print render($page['sidebar_first']); ?>
      <?php print render($page['sidebar_second']); ?>

    </div> <!-- end .wrap-columns -->

    <?php print render($page['postblocks']); ?>

  </div> <!-- end #page -->

  <?php print render($page['footer']); ?>
  <?php if ($arctica_attr_link): print $arctica_attr_link; endif; ?>
