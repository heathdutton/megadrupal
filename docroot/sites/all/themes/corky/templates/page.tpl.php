<?php
// $Id: page.tpl.php,v 1.1 2010/11/16 05:25:24 chevy Exp $

/**
 * @file page.tpl.php
 * @desc Dossier's theme implementation to display a single Drupal page.
 */

?>

<div id="page-wrapper"><div id="page">

  <div id="header" class="<?php print $secondary_menu ? 'with-secondary-menu': 'without-secondary-menu'; ?>"><div class="section clearfix">

    <?php if ($site_name || $site_slogan): ?>
      <div id="name-and-slogan"<?php if ($hide_site_name && $hide_site_slogan) { print ' class="element-invisible"'; } ?>>
        <div class="tape-left"><div class="tape-right"><div class="tape-middle">
          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <div id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
                <strong>
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </strong>
              </div>
            <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
            <?php endif; ?>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <div id="site-slogan"<?php if ($hide_site_slogan) { print ' class="element-invisible"'; } ?>>
              <?php print $site_slogan; ?>
            </div>
          <?php endif; ?>
        </div></div></div> <!-- /.tape-middle /.tape-left /.tape-right -->
      </div> <!-- /#name-and-slogan -->
      <div class="clearfix"></div>
    <?php endif; ?>

    <?php if ($logo): ?>
      <div id="logo-container" class="clearfix">
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      </div>
    <?php endif; ?>

    <?php print render($page['header']); ?>

    <?php if ($main_menu): ?>
      <div id="main-menu" class="navigation">
        <div class="tape-left"><div class="tape-right"><div class="tape-middle">
          <?php print theme('links__system_main_menu', array(
            'links' => $main_menu,
            'attributes' => array(
              'id' => 'main-menu-links',
              'class' => array('links', 'clearfix'),
            ),
            'heading' => array(
              'text' => t('Main menu'),
              'level' => 'h2',
              'class' => array('element-invisible'),
            ),
          )); ?>
        </div></div></div>
      </div> <!-- /#main-menu -->
    <?php endif; ?>

  </div></div> <!-- /.section, /#header -->

  <div id="main-wrapper" class="clearfix"><div id="main" class="clearfix">

    <?php if ($secondary_menu): ?>
      <div id="secondary-menu" class="navigation">
        <?php print theme('links__system_secondary_menu', array(
          'links' => $secondary_menu,
          'attributes' => array(
            'id' => 'secondary-menu-links',
            'class' => array('links', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Secondary menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </div> <!-- /#secondary-menu -->
    <?php endif; ?>

    <?php if ($messages): ?>
      <div id="messages"><div class="section clearfix">
        <?php print $messages; ?>
      </div></div> <!-- /.section, /#messages -->
    <?php endif; ?>

    <?php if ($page['featured']): ?>
      <div id="featured"><div class="section clearfix">
        <?php print render($page['featured']); ?>
      </div></div> <!-- /.section, /#featured -->
    <?php endif; ?>

    <span>
      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-first" class="column sidebar"><div class="section">
          <?php print render($page['sidebar_first']); ?>
        </div></div> <!-- /.section, /#sidebar-first -->
      <?php endif; ?>

      <?php if ($page['sidebar_second']): ?>
        <div id="sidebar-second" class="column sidebar"><div class="section">
          <?php print render($page['sidebar_second']); ?>
        </div></div> <!-- /.section, /#sidebar-second -->
      <?php endif; ?>

      <div id="content"><div class="section">
        <div id="pages-top"><div id="pages-bottom"><div id="pages-left"><div id="pages-right">
        <div id="content-wrapper">
          <div id="paperclip-bottom">
            <?php if ($breadcrumb): ?>
              <div id="breadcrumb"><?php print $breadcrumb; ?></div>
            <?php endif; ?>

            <?php if ($page['search_bar_area']): ?>
              <div id="search-bar-area" class="clearfix">
                <?php print render($page['search_bar_area']); ?>
              </div> <!-- /#post-script -->
            <?php endif; ?>
          </div> <!-- /#paperclip-bottom -->
          <div id="content-inner">

            <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
            <a id="main-content"></a>
            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
              <h1 class="title" id="page-title">
                <?php print $title; ?>
              </h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php if ($tabs): ?>
              <div class="tabs">
                <?php print render($tabs); ?>
              </div>
            <?php endif; ?>
            <?php print render($page['help']); ?>
            <?php if ($action_links): ?>
              <ul class="action-links">
                <?php print render($action_links); ?>
              </ul>
            <?php endif; ?>
            <?php print render($page['content']); ?>
            <?php print $feed_icons; ?>

          </div> <!-- /#content-inner -->
        </div> <!-- /#content-wrapper -->
        </div></div></div></div>
      </div></div> <!-- /.section, /#content -->

    </span>

    <?php if ($page['footer_firstcolumn'] || $page['footer_secondcolumn'] || $page['footer_thirdcolumn'] || $page['footer_fourthcolumn']): ?>
      <div class="clearfix"></div>
      <div id="footer-columns" class="clearfix">
        <?php print render($page['footer_firstcolumn']); ?>
        <?php print render($page['footer_secondcolumn']); ?>
        <?php print render($page['footer_thirdcolumn']); ?>
        <?php print render($page['footer_fourthcolumn']); ?>
      </div> <!-- /#footer-columns -->
    <?php endif; ?>

  </div></div> <!-- /#main, /#main-wrapper -->

  <div class="section">

    <?php if ($page['footer']): ?>
      <div id="footer" class="clearfix">
        <?php print render($page['footer']); ?>
      </div> <!-- /#footer -->
    <?php endif; ?>

  </div> <!-- /.section -->

  <div class="tape-left"><div class="tape-right"><div class="tape-middle">
    <div id="post-script-wrapper">
      <?php if ($page['post_script']): ?>
        <div id="post-script" class="clearfix">
          <?php print render($page['post_script']); ?>
        </div> <!-- /#post-script -->
      <?php endif; ?>
      <div id="designby" class="clearfix" style="text-align: center;"><?php print t('Design by ') . l('Rapid Doodle Designs', 'http://www.rapiddoodle.ca'); ?></div>
    </div> <!-- /#post-script-wrapper -->
  </div></div></div>

</div></div> <!-- /#page, /#page-wrapper -->
