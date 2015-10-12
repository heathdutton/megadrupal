<?php

// COPY THIS FILE TO YOUR CHILD THEME TO ADJUST TO YOUR GRID DESIGN

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

<div id="wrapper" class="<?php print $max_width; ?>">

  <header class="section sg-35 clearfix" role="banner">

    <?php if ($logo): ?>
      <?php print $linked_logo_img; ?>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <?php if ($site_name && $site_slogan): ?>
        <hgroup id="name-and-slogan" role="heading">
      <?php endif; ?>
        <?php if ($site_name): ?>
          <h1 id="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" role="landmark"><span><?php print $site_name; ?></span></a>
          </h1>
        <?php endif; ?>
        <?php if ($site_slogan): ?>
          <h2 id="site-slogan"><?php print $site_slogan; ?></h2>
        <?php endif; ?>
      <?php if ($site_name && $site_slogan): ?>
        </hgroup> <!-- /#name-and-slogan -->
      <?php endif; ?>
    <?php endif; ?>

    <?php print render($page['header']); ?>

  </header> 

  <?php if ($main_menu || $secondary_menu): ?>
    <nav id="navigation" class="sg-35 clear">
    <?php print $main_menu_links; ?>
    <?php print $secondary_menu_links; ?>
    </nav> 
  <?php endif; ?>

  <?php if ($breadcrumb): ?>
    <nav id="breadcrumb" class="sg-35 clear"><?php print $breadcrumb; ?></nav>
  <?php endif; ?>

  <?php if ($messages): ?>
    <aside class="sg-35 clear"><?php print $messages; ?></aside>
  <?php endif; ?>

  <div id="content-wrapper" class="sg-35 clear clearfix">

    <main id="main-content" role="main" class="column <?php print $class_content; ?>">
      <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>

      <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>

      <a id="main-content"></a>
      <?php if ($title): ?>
        <?php
        // open article tag if page is a node
        if (($page) && (arg(0) == 'node')): ?>
          <article role="article">
        <?php endif; ?>
      <?php print render($title_prefix); ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>

      <?php print render($page['help']); ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>

      <?php print render($page['content']); ?>

      <?php print $feed_icons; ?>
    </main> <!-- /#main-content -->

    <?php if ($page['sidebar_first']): ?>
      <aside id="sidebar-first" role="complementary" class="column sidebar <?php print ($class_sidebar_first); ?>">
        <?php print render($page['sidebar_first']); ?>
      </aside> 
    <?php endif; ?>

    <?php if ($page['sidebar_second']): ?>
      <aside id="sidebar-second" role="complementary" class="column sidebar <?php print ($class_sidebar_second); ?>"> 
        <?php print render($page['sidebar_second']); ?>
      </aside>
    <?php endif; ?>

  </div> <!-- /#content-wrapper -->
  <?php if ($page['footer']): ?>
    <footer class="sg-35 clear" role="contentinfo">
      <?php print render($page['footer']); ?>
    </footer>
  <?php endif; ?>

</div> <!-- /#wrapper -->
