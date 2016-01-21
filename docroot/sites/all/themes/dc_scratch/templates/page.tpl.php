<?php
/**
 * @file
 * DC Scratch theme implementation to display a page.
 *
 * Available variables:
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title: The page title, for use in the actual HTML content.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 *
 * Regions:
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>
<div id="page-wrapper"><div id="page">
    <div class="header"><div class="header-inside">
        <div class="header-inside-left">
          <div class="logo"><?php if ($logo): ?>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
            <?php endif; ?></div>
        </div>
        <div class="header-inside-right">
          <div class="dc_scratch-user-menu">
            <ul id="login-menu">
              <?php if (user_is_logged_in()) : ?>
                <li class="user_menu"><?php print l(t('Log Out'), 'user/logout'); ?></li>
              <?php else : ?>
                <li class="user_menu"><?php print l(t('Log In'), 'user/login', array('query' => drupal_get_destination())) ?></li>
              <?php endif; ?>
            </ul>
            <?php print render($page['user_menu']); ?>
          </div> <!-- /.user-menu -->

          <div class="dc_scratch-main-menu">
            <input type="checkbox" id="navbar-checkbox" class="navbar-checkbox">
            <nav class="menu">
              <?php
              print theme('links__system_main_menu', array(
                'links' => $main_menu,
                'attributes' => array(
                  'id' => 'menu',
                  'class' => array('links', 'clearfix'),
                ),
                'heading' => array(
                  'text' => t('Main menu'),
                  'level' => 'h2',
                  'class' => array('element-invisible'),
                ),
              ));
              ?>
              <label for="navbar-checkbox" class="navbar-handle"></label>
            </nav>
          </div><!-- /.main-menu -->

        </div>
      </div></div> <!-- /.header-inside /.header -->

    <div class="dc_scratch-content-container clearfix">
      <div id="breadcrumb"><div class="section clearfix">
          <?php print render($page['breadcrumb']); ?>
        </div></div>

      <?php if ($messages): ?>
        <div id="messages"><div class="section clearfix">
            <?php print $messages; ?>
          </div></div> <!-- /.section, /#message -->
      <?php endif; ?>

      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-first" class="column sidebar"><div class="section">
            <?php print render($page['sidebar_first']); ?>
          </div></div> <!-- /.section, /#sidebar-first -->
      <?php endif; ?>

      <div id="content" class="column"><div class="section">
          <h1> <?php print $title; ?> </h1>
          <?php print render($page['content']); ?>
        </div></div> <!-- /.section, /#content -->

      <?php if ($page['sidebar_second']): ?>
        <div id="sidebar-second" class="column sidebar"><div class="section">
            <?php print render($page['sidebar_second']); ?>
          </div></div> <!-- /.section, /#sidebar-second -->
      <?php endif; ?>
    </div>

    <div id="footer-wrapper" class="clearfix"><div class="section">
        <div class="footer_firstcolumn"><?php print render($page['footer_firstcolumn']); ?></div>
        <div class="footer_secondcolumn"><?php print render($page['footer_secondcolumn']); ?></div>
        <div class="footer_thirdcolumn"><?php print render($page['footer_thirdcolumn']); ?></div>
        <div class="footer_fourthcolumn"><?php print render($page['footer_fourthcolumn']); ?></div>
      </div></div> <!-- /.section, /#footer-wrapper -->

    <div class="dc_scratch-footer">
      <?php if ($page['footer']): ?>
        <div id="dc_scratch-footer" class="clearfix">
          <?php print render($page['footer']); ?>
        </div> <!-- /#footer -->
      <?php endif; ?>            
    </div>

  </div></div> <!-- /#page, /#page-wrapper -->
