<?php
/**
 * @file
 *
 * For more info on Drupal default for this template, refer to
 * http://api.drupal.org/api/drupal/modules--system--page.tpl.php/7
 */
?>

<div id="page-wrapper">
  <div class="skip-link">
    <a class="element-invisible element-focusable" title="<?php print t('Jump to Content'); ?>" href="#content"><?php print t('Jump to Content'); ?></a>
    <?php if ($main_menu): ?>
      <a class="element-invisible element-focusable" title="<?php print t('Jump to Navigation'); ?>" href="#navigation"><?php print t('Jump to Navigation'); ?></a>
    <?php endif; ?>
  </div>

  <div id="page">

    <header id="header" role="banner">
      <div class="section clearfix">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
        <?php endif; ?>

        <?php if ($site_name || $site_slogan): ?>
          <div id="name-and-slogan">
            <?php if ($site_name): ?>
              <?php if ($title): ?>
                <div id="site-name">
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </div>
              <?php else: /* Use h1 when the content title is empty */ ?>
                <h1 id="site-name">
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
                </h1>
              <?php endif; ?>
            <?php endif; ?>

            <?php if ($site_slogan): ?>
              <div id="site-slogan"><?php print $site_slogan; ?></div>
            <?php endif; ?>
          </div>
        <?php endif; ?>

        <?php print render($page['header']); ?>

      </div>
    </header>

    <div id="main-wrapper">
      <div id="main" class="clearfix<?php if ($page['navigation']) { print ' with-navigation'; } ?>">
        <?php if ($page['navigation']): ?>
          <nav id="navigation" role="navigation">
            <div class="section">
              <?php print render($page['navigation']); ?>
            </div>
          </nav>
        <?php endif; ?>

        <div id="content" class="column" role="main">
          <div class="section">
            <?php if ($page['highlighted']): ?>
              <div id="highlighted"><?php print render($page['highlighted']); ?></div>
            <?php endif; ?>
            <?php if ($breadcrumb): ?>
              <div id="breadcrumb"><?php print $breadcrumb; ?></div>
            <?php endif; ?>
            <?php print $messages; ?>
            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
              <h1 class="title" id="page-title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php if ($tabs): ?>
              <div class="tabs"><?php print render($tabs); ?></div>
            <?php endif; ?>
            <?php print render($page['help']); ?>
            <?php if ($action_links): ?>
              <ul class="action-links"><?php print render($action_links); ?></ul>
            <?php endif; ?>
            <?php print render($page['content']); ?>
            <?php print $feed_icons; ?>
          </div>
        </div>

        <?php if ($page['sidebar_first']): ?>
          <div id="sidebar-first" class="column sidebar" role="complementary">
            <div class="section">
              <?php print render($page['sidebar_first']); ?>
            </div>
          </div>
        <?php endif; ?>

        <?php if ($page['sidebar_second']): ?>
          <div id="sidebar-second" class="column sidebar" role="complementary">
            <div class="section">
              <?php print render($page['sidebar_second']); ?>
            </div>
          </div>
        <?php endif; ?>
      </div>
    </div>

    <footer id="footer" role="contentinfo">
      <div class="section">
        <?php print render($page['footer']); ?>
      </div>
    </footer>

  </div>
</div>
