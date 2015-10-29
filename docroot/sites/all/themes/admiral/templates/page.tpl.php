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
  <div class="container-fluid">
    <div id="navigation" class="navbar">
      <div class="navbar-inner">
        <div class="container clearfix">
          <!-- .btn-navbar is used as the toggle for collapsed navbar content -->
          <a class="btn btn-navbar btn-navbar-menu" data-toggle="collapse" data-target=".nav-menu-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </a>
          <!-- .btn-navbar-search for collapsed search form -->
          <?php if ($search_form): ?>
            <a class="btn btn-navbar btn-navbar-search" data-toggle="collapse" data-target=".nav-search-collapse">
              <span class="icon-search"></span>
            </a>
          <?php endif; ?>

          <?php if ($logo): ?>  
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo" class="pull-left brand">
              <?php print $site_name; ?>
            </a>
          <?php endif; ?>

          <div class="nav-collapse nav-menu-collapse nav-menu-main-menu">
            <div class="inner">
              <?php if ($main_menu): ?>
                <nav id="main-menu" class="main-menu pull-left" role="navigation">
                  <?php print render($main_menu); ?>
                </nav> <!-- /#main-menu -->
              <?php endif; ?>
            </div>
          </div>

          <div class="nav-collapse nav-search-collapse">
            <div class="inner">
                <form name="admin-quick-menu" class="search-form navbar-form pull-right">
                  <div class="container-inline form-wrapper" id="admin-quick-menu-wrapper">
                    <div class="form-item form-type-textfield form-item-keys">
                      <input placeholder="Menu Search" data-toggle="tooltip" title="Search the Admin menu (Ctrl + Shift + F)" data-placement="bottom" class="search-query form-text typehead" type="text" id="admin-quick-menu-keys" name="keys" value="" size="40" maxlength="255">
                    </div>
                    <input type="submit" id="admin-quick-menu-submit" name="op" value="Search" class="form-submit btn">
                  </div>
                </form>
            </div>
          </div>

      </div>
    </div> 
  </div> <!-- /#navigation -->
</header>

<div id="main-wrapper">
  <div id="main" class="main <?php print (!$is_panel) ? 'container-fluid' : ''; ?>">
    <?php if ($breadcrumb): ?>
      <div id="breadcrumb" class="visible-desktop">
        <div class="container-fluid">
          <?php print $breadcrumb; ?>
        </div>
      </div>
    <?php endif; ?>
    <?php if ($messages): ?>
      <div id="messages">
        <div class="container-fluid">
          <?php print $messages; ?>
        </div>
      </div>
    <?php endif; ?>
    <div id="content">
      <a id="main-content"></a>
      <div id="page-header">
          <div class="container-fluid">
            <?php if ($title): ?>
              <div class="page-header">
                <h1 class="title"><?php print $title; ?></h1>
              </div>
            <?php endif; ?>
            <?php if ($tabs): ?>
              <div class="tabs">
                <?php print render($tabs); ?>
              </div>
            <?php endif; ?>
            <?php if ($action_links): ?>
              <ul class="nav nav-pills action-links">
                <?php print render($action_links); ?>
              </ul>
            <?php endif; ?>
          </div>
      </div>
      <div class="container-fluid">
        <?php print render($page['content']); ?>
      </div>
    </div>
  </div>
</div> <!-- /#main-wrapper -->

<footer id="footer" class="footer" role="footer">
  <div class="container-fluid">
    <?php if ($copyright): ?>
      <small class="copyright pull-left"><?php print $copyright; ?></small>
    <?php endif; ?>
    <small class="pull-right"><a href="#"><?php print t('Back to Top'); ?></a></small>
  </div>
</footer>