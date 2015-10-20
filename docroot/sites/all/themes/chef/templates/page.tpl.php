<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see html.tpl.php
 */
?>
<header id="header" class="header" role="header">
  <div class="container">
    <div id="navigation" class="navbar">
      <div class="navbar-inner">
        <div class="container clearfix">
          <!-- .btn-navbar is used as the toggle for collapsed navbar content -->
          <a class="btn btn-navbar btn-navbar-menu" data-toggle="collapse" data-target=".nav-menu-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </a>
          <?php if ($search_form): ?>
            <a class="btn btn-navbar btn-navbar-search" data-toggle="collapse" data-target=".nav-search-collapse">
              <span class="icon-search"></span>
            </a>
          <?php endif; ?>

          <?php if ($site_name): ?>
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo" class="pull-left brand">
              <?php print $site_name; ?>
            </a>
          <?php endif; ?>

          <div class="nav-collapse nav-menu-collapse">
            <div class="inner">
              <?php if ($main_menu): ?>
                <nav id="main-menu" class="main-menu pull-left" role="navigation">
                  <?php print render($main_menu); ?>
                </nav> <!-- /#main-menu -->
              <?php endif; ?>
            </div>
          </div>

          <?php if ($search_form): ?>
            <div class="nav-collapse nav-search-collapse">
              <div class="inner">
                <?php if ($search_form): ?>
                  <?php print $search_form; ?>
                <?php endif; ?>
              </div>
            </div>
          <?php endif; ?>
      </div>
    </div> <!-- /#navigation -->
  </div>
</header>

<div id="main-wrapper">
  <div id="main" class="main <?php if (!$is_panel) { print 'container'; } ?>">
    <div id="content">
      <a id="main-content"></a>
      <div id="page-header">
          <?php if ($title): ?>
            <div class="page-header">
              <div class="container">
                <h1 class="title"><?php print $title; ?></h1>
              </div>
            </div>
          <?php endif; ?>
          <?php if ($messages): ?>
            <div id="messages">
              <div class="container">
                <?php print $messages; ?>
              </div>
            </div>
          <?php endif; ?>
          <div class="container">
            <?php if ($tabs): ?>
              <div class="tabs">
                <?php print render($tabs); ?>
              </div>
            <?php endif; ?>
            <?php if ($action_links): ?>
              <ul class="action-links">
                <?php print render($action_links); ?>
              </ul>
            <?php endif; ?>
          </div>
      </div>
      <?php print render($page['content']); ?>
    </div>
  </div>
</div>

<footer id="footer" class="footer" role="footer">
  <div class="container">
    <?php if (isset($footer_links)): ?>
      <?php print $footer_links; ?>
    <?php else: ?>
      <div class="span12">
        <?php if (isset($footer_menu)): ?>
          <nav id="footer-menu" class="footer-menu">
            <?php print render($footer_menu); ?>
          </nav> <!-- /#footer-menu -->
        <?php endif; ?>
      </div>
    <?php endif; ?>
  </div>
</footer>
