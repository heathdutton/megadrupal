<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
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
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>

<?php global $base_url; ?>
<section id="page">
  <?php if($page['top_bar']): ?>
    <nav class="top-bar" role="navigation">
      <?php print render($page['top_bar']); ?>
    </nav>
  <?php endif; ?>

  <header id="masthead" class="site-header" role="banner">
    <div class="header-wrapper container">
      <div class="header-left">
        <div id="logo" class="site-branding">
          <?php if ($logo): ?>
            <div class="site-logo">
              <?php print l("<img src='$logo' alt='$site_name - $site_slogan' />", $base_url, array('html' => TRUE, 'attributes' => array('title' => $site_name . ' - ' . $site_slogan))); ?>
            </div>
          <?php endif; ?>
        </div>
      </div>
      <div class="header-right">
        <div id="name-slogan">
          <div class="site-name">
              <h1><a href="<?php print $base_url; ?>/" title="<?php print $site_name . ' - ' . $site_slogan; ?>"><?php print $site_name; ?></a></h1>
          </div>
          <div class="site-slogan">
              <?php print $site_slogan; ?>
          </div>
        </div>
      </div>
    </div>
  </header>

  <?php if($page['preface']) : ?>
  <div id="preface">
    <?php print render($page['preface']); ?>
  </div>
  <?php endif; ?>
  <?php if ($main_menu || $secondary_menu): ?>
      <div id="navigation"><div class="section container">
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu',
            'class' => array('links', 'inline', 'clearfix')),
          'heading' => t('Main menu'))); ?>
        <?php print theme('links__system_secondary_menu', array(
          'links' => $secondary_menu,
          'attributes' => array(
            'id' => 'secondary-menu',
            'class' => array('links', 'inline', 'clearfix')),
          'heading' => t('Secondary menu'))); ?>
      </div></div> <!-- /.section, /#navigation -->
  <?php endif; ?>
  <div id="main-content" class="main-content container">
    <?php if (theme_get_setting('styleguide') && module_exists('styleguide') && module_exists('jquery_update')): ?>
      <div>
        <a class="style-guide-modal-fire" href="#">Style Guide</a>
      </div>
    <?php endif; ?>
    <?php if (theme_get_setting('breadcrumbs')): ?>
      <?php if ($breadcrumb): ?>
        <div id="breadcrumbs" class="clearfix"><?php print $breadcrumb; ?></div>
      <?php endif;?>
    <?php endif; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?><div class="hx"><h1 <?php print $title_attributes; ?>><?php print $title; ?></h1></div><?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php print $messages; ?>
    <?php if ($page['content_top']): ?><div id="content_top"><?php print render($page['content_top']); ?></div><?php endif; ?>

    <section id="content" role="main">
      <?php if (!empty($tabs['#primary'])): ?><div class="tabs-wrapper clearfix"><?php print render($tabs); ?></div><?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <?php print render($page['content']); ?>
    </section>

    <?php if ($page['sidebar_first']): ?>
      <aside id="sidebar-first" role="complementary">
        <?php print render($page['sidebar_first']); ?>
      </aside>
    <?php endif; ?>
    <?php if ($page['sidebar_second']): ?>
      <aside id="sidebar-second" class="" role="complementary">
        <?php print render($page['sidebar_second']); ?>
      </aside>
    <?php endif; ?>
  </div>

  <div class="backtotop">
   <?php print l(t('Come up'), '#', array('external' => TRUE));?>
  </div>

  <?php if($page['footer']) : ?>
    <footer class="site-footer">
      <?php print render($page['footer']); ?>
    </footer>
  <?php endif; ?>
</section>
