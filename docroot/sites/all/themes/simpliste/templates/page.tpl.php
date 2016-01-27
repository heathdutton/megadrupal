<?php

/**
 * @file
 * Simpliste theme implementation to display a single Drupal page.
 */
?>
<div id="container">

  <header id="header" class="clearfix">
    <?php if ($secondary_menu): ?>
      <nav id="secondary-menu"><?php print $secondary_menu; ?></nav>
    <?php endif; ?>
    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>

    <div id="name-slogan">
      <?php if ($site_name): ?>
        <h1 id="site-name">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
        </h1>
      <?php endif; ?>
      <?php if ($site_slogan): ?>
        <div id="site-slogan"><?php print $site_slogan; ?></div>
      <?php endif; ?>
    </div>
    <?php print render($page['header']); ?>
    <nav id="main-menu">
      <?php print $main_menu; ?>
    </nav>
  </header>

  <?php if ($page['featured']): ?>
    <article id="featured" class="clearfix">
      <?php print render($page['featured']); ?>
    </article>
  <?php endif; ?>

    <?php print render($page['content_top']) ?>
  <article class="article clearfix">

    <?php if ($breadcrumb): ?>
      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
    <?php endif; ?>
    <div class="clearfix"></div>

    <?php if ($sidebar_position == 'left'): ?>
      <div id="sidebar" class="col_33 sidebar-left"><?php print render($page['sidebar']); ?></div>
    <?php endif; ?>

    <div id="main" class="<?php print $content_classes; ?> ">
      <?php print $messages; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): ?>
        <div class="tabs"><?php print render($tabs); ?></div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <?php print render($page['content']); ?>
    </div>
      
    <?php if ($sidebar_position == 'right'): ?>
      <div id="sidebar" class="col_33 sidebar-right"><?php print render($page['sidebar']); ?></div>
    <?php endif; ?>
    <div class="clearfix"></div>

    <div class="col_100" id="page_bottom"></div>
    <div class="clearfix"></div>

  </article>

  <?php print render($page['content_bottom']) ?>

  <footer id="footer" class="clearfix">
    <?php if ($footer_message): ?>
    <div class="copyright"><?php print $footer_message; ?></div>
    <?php endif; ?>
    <?php print render($page['footer']) ?>
    <nav class="menu_bottom">
      <?php print $secondary_menu; ?>
    </nav>
    <div class="clearfix"></div>
    <?php print $feed_icons ?>
  </footer>

</div>