<?php
// $Id$

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>
    <div id="header">
      <div id="header-inner">
        <div id="header-top">
          <?php if (!empty($logo)): ?>
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
              <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
            </a>
          <?php endif; ?>

          <?php if (!empty($site_slogan)): ?>
            <span id="site-slogan"><?php print $site_slogan; ?></span>
          <?php endif; ?>

          <?php if (($page['search_box'])): ?>
          <div id="search-box"><?php print render($page['search_box']); ?></div>
          <?php endif; ?>
        </div> <!-- /header-top -->

        <div id="header-bottom">
          <?php if (!empty($main_menu)): ?>
            <div id="primary">
              <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'primary-links')))); ?>
            </div>
          <?php endif; ?>
        </div> <!-- /header-bottom -->
      </div> <!-- /header-inner -->
    </div> <!-- /header -->

    <div id="front-page-wrapper">
      <div id="front-page">
        <div id="front-page-inner">
          <?php if (!empty($secondary_menu)): ?>
            <div id="secondary">
              <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'secondary-links')))); ?>
            </div>
          <?php endif; ?>
          <?php if ($page['front_top']): ?>
            <?php print render($page['front_top']); ?>
          <div class="c-b"></div>
          <?php endif; ?>

        </div> <!-- /front-page-inner -->
      </div> <!-- /front-page -->
    </div> <!-- /front-page-wrapper -->

    <div id="page">

      <?php if (!empty($breadcrumb)): ?><div id="breadcrumb"><?php print $breadcrumb; ?></div><?php endif; ?>

      <div id="container">

      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-left" class="sidebar">
          <?php print render($page['sidebar_first']); ?>
        </div> <!-- /sidebar-left -->
      <?php endif; ?>

      <div id="main">
        <?php print render($title_prefix); ?>
        <?php if (!empty($title)): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if (!empty($tabs)): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
        <?php if (!empty($messages)): print $messages; endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        <?php print render($page['content']); ?>
      </div> <!-- /main -->

      <?php if ($page['sidebar_second']): ?>
        <div id="sidebar-right" class="sidebar">
          <?php print render($page['sidebar_second']); ?>
        </div> <!-- /sidebar-left -->
      <?php endif; ?>

      </div> <!-- /container -->

    </div> <!-- /page -->

    <div id="footer" class="c-b">
      <div class="footer-inner">
        <?php if ($page['footer']): print render($page['footer']); endif; ?>
        <div class="c-b"></div>
      </div>
      <div id="footer-message">
        <div class="footer-inner">
          <a href="http://premiumcmsthemes.com" target="_blank" class="pt_logo"></a>
          <?php print $feed_icons; ?>
        </div>
      </div> <!-- /footer-message -->
    </div> <!-- /footer -->