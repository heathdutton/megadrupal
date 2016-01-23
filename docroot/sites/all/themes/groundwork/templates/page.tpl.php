<?php

/**
 * @file
 * Groundwork Framework's implementation to display a single Drupal page.
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
 * - $main_navigation: Containing the Main menu links for the site, if they have
 *   been configured.
 * - $secondary_navigation: Containing the Secondary menu links for the site, if
 *   they have been configured.
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
 * - $page['topbar']: Items for the topbar region.
 * - $page['header_annex']: Items for the header annex region.
 * - $page['navigation']: Items for the navigation region.
 * - $page['billboard']: Items for the billboard region.
 * - $page['placard']: Items for the rider region.
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['supplement']: Items for the supplement region.
 * - $page['appendix']: Items for the appendix region.
 * - $page['rider']: Items for the rider region.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see html.tpl.php
 *
 */
?>

<div id="page">

  <?php if ($page['topbar']): ?>
    <?php print render($page['topbar']); ?>
  <?php endif; ?>

  <header id="header">
    <div id="branding" role="banner">
      <?php if ($logo): ?>
        <a id="site-logo" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
          <img src="<?php print $logo; ?>" alt="<?php print t('Site logo'); ?>">
        </a>
      <?php endif; ?>
      <?php if ($site_name): ?>
        <?php if ($title): ?>
          <p id="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
              <?php print $site_name; ?>
            </a>
          </p>
        <?php else: /* Use h1 when the content title is empty */ ?>
          <h1 id="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
              <?php print $site_name; ?>
            </a>
          </h1>
        <?php endif; ?>
      <?php endif; ?>
      <?php if ($site_slogan): ?>
        <p id="site-slogan">
          <?php print $site_slogan; ?>
        </p>
      <?php endif; ?>
    </div><!--/#branding -->
    <?php print render($page['header_annex']); ?>
  </header><!--/#header -->

  <?php if ($page['navigation']): ?>
    <?php print render($page['navigation']); ?>
  <?php elseif ($main_menu): ?>
    <nav id="main-menu" class="region region-main-menu" role="navigation">
      <?php print $main_navigation; ?>
    </nav><!--/#main-menu -->
  <?php endif; ?>

  <?php if ($page['billboard']): ?>
    <?php print render($page['billboard']); ?>
  <?php endif; ?>

  <?php if ($page['placard']): ?>
    <?php print render($page['placard']); ?>
  <?php endif; ?>

  <div id="main-container">
    <main id="main" role="main">
      <?php if ($breadcrumb): ?>
        <?php print $breadcrumb; ?>
      <?php endif; ?>
      <?php print $messages; ?>
      <?php if ($page['highlighted']): ?>
        <?php print render($page['highlighted']); ?>
      <?php endif; ?>
      <?php if ($title): ?>
        <?php print render($title_prefix); ?>
          <h1 class="title page-title">
            <?php print $title; ?>
          </h1>
        <?php print render($title_suffix); ?>
      <?php endif; ?>
      <?php if ($tabs): ?>
        <?php print render($tabs); ?>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </main><!--/#main -->
    <?php if ($page['sidebar_first']): ?>
      <?php print render($page['sidebar_first']); ?>
    <?php endif; ?>
    <?php if ($page['sidebar_second']): ?>
      <?php print render($page['sidebar_second']); ?>
    <?php endif; ?>
  </div><!--/#main-container -->

  <?php if ($page['supplement']): ?>
    <?php print render($page['supplement']); ?>
  <?php endif; ?>

  <?php if ($page['appendix']): ?>
    <?php print render($page['appendix']); ?>
  <?php endif; ?>

  <?php if ($page['rider']): ?>
    <?php print render($page['rider']); ?>
  <?php endif; ?>

  <?php if ($page['footer']): ?>
    <?php print render($page['footer']); ?>
  <?php endif; ?>

  <?php if ($secondary_menu): ?>
    <nav id="secondary-menu" class="region region-secondary-menu" role="navigation">
      <?php print $secondary_navigation; ?>
    </nav><!--/#secondary-menu -->
  <?php endif; ?>

  <?php print $theme_attribution; ?>

</div><!--/#page -->
