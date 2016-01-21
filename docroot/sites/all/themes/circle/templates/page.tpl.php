<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 */
?>

<div class="page-wrapper">

  <header class="page-header" role="header">
    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <div class="name-and-slogan">
        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div class="site-name"><b>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </b></div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 class="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div class="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div> <!-- /.name-and-slogan -->
    <?php endif; ?>
    <?php print render($page['header']); ?>
  </header> <!-- /.page-header -->

  <?php if ($main_menu || $secondary_menu): ?>
    <nav class="page-navigation" role="navigation">
      <?php print theme('links__system_main_menu', array(
        'links' => $main_menu,
        'attributes' => array(
          'id' => 'main-menu',
          'class' => array('links', 'inline', 'clearfix')),
          'heading' => t('Main menu'),
        ));
      ?>
      <?php print theme('links__system_secondary_menu', array(
        'links' => $secondary_menu,
        'attributes' => array(
          'id' => 'secondary-menu',
          'class' => array('links', 'inline', 'clearfix')),
          'heading' => t('Secondary menu'),
        ));
      ?><?php print render($page['navigation']); ?>
    </nav> <!-- /.page-navigation -->
  <?php endif; ?>

  <?php if ($breadcrumb): ?>
    <div class="breadcrumb-wrapper"><?php print $breadcrumb; ?></div>
  <?php endif; ?>

  <?php print $messages; ?>

  <div class="main-wrapper" role="main">
    <?php if (isset($is_panel_page) && $is_panel_page): ?>
      <section class="page-content panel-page-content">
    <?php else: ?>
      <section class="page-content">
    <?php endif; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <header class="page-content-header">
          <h1 class="page-title" id="page-title">
            <?php print $title; ?>
          </h1>
        </header>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): ?>
        <div class="tabs" role="tablist">
          <?php print render($tabs); ?>
        </div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links" role="list">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </section>
  </div>

  <section class="footer" role="footer">
    <?php print render($page['footer']); ?>
  </section>

</div> <!-- /.page-wrapper -->
