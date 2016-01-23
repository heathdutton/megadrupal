<?php
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
 * - $secondary_menu_heading: The title of the menu used by the secondary links.
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
 * - $page['header']: Items for the header region.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['top_first']: Dynamic text, mostly for front page.
 * - $page['top_second']: Dynamic text, mostly for front pages.
 * - $page['top_third']: Dynamic text, mostly for front pages.
 * - $page['front_page']: Content only for the first page.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['bottom_first']: Items for the first bottom element.
 * - $page['bottom_second']: Items for the second bottom element.
 * - $page['bottom_third']: Items for the third bottom element.
 * - $page['links_first']: Links for the first bottom block.
 * - $page['links_second']: Links for the second bottom block.
 * - $page['links_third']: Links for the third bottom block.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see omega_preprocess_page()
 */
?>
<div class="l-page">
  <header class="l-header" style="background-image: url(<?php print $header_image_path ?>)" role="banner">

    <div class="l-header-wrapper">
      
      <div class="l-branding">

        <div class="l-mini-branding">
          <div class="mini-logo">
            <span class="azul"></span>
            <span class="rojo"></span>
          </div>
          <?php if ($site_name): ?>
            <h1 class="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        </div>
        
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="site-logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <h2 class="site-slogan"><?php print $site_slogan; ?></h2>
        <?php endif; ?>
        
        <?php if (isset($main_menu) && !empty($main_menu)) : ?>
          <div class="l-main-menu">
            <a id="gob-cl-toggle-menu" href="#">Menú</a>
            <?php $menu = menu_tree_output(menu_tree_all_data('main-menu')); ?>
            <?php print drupal_render($menu); ?>
          </div>
        <?php endif; ?>
      </div>
     

      <?php print render($page['header']); ?>

    </div>
    
  </header>

  <div class="l-main">
    
    <?php if ($breadcrumb): ?>
      <div class="breadcrumb-wrapper">
        <?php print $breadcrumb; ?>
      </div>
    <?php endif ?>

    <div class="l-region--top-wrapper">
      <?php print render($page['top_first']); ?>
      <?php print render($page['top_second']); ?>
      <?php print render($page['top_third']); ?>
    </div>

    <div class="l-content" role="main">
      <?php print render($page['highlighted']); ?>
      <a id="main-content"></a>
      <?php print $messages; ?>
      <?php print render($tabs); ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <div class="content-wrapper">
        
        
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1 class="title"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        
        <?php print render($page['content']); ?>
        
        <?php // print $feed_icons; ?>
      </div>
    </div>

    <?php if ($page['sidebar_first'] || $page['sidebar_second']  ): ?>
      <div class="l-region-sidebars-wrapper">
        <?php print render($page['sidebar_first']); ?>
        <?php print render($page['sidebar_second']); ?>
      </div>
    <?php endif ?>
    
    <div class="l-region--bottom-wrapper">
      <?php print render($page['bottom_first']); ?>
      <?php print render($page['bottom_second']); ?>
      <?php print render($page['bottom_third']); ?>
    </div>
    
  </div>

  <footer class="l-footer" role="contentinfo">
    <div class="bicolor">
      <span class="azul"></span>
      <span class="rojo"></span>
    </div>
    <?php print render($page['links_first']); ?>
    <?php print render($page['links_second']); ?>
    <?php print render($page['links_third']); ?>
    <?php print render($page['footer']); ?>

    <div class="bottom">
      <div class="bicolor">
        <span class="azul"></span>
        <span class="rojo"></span>
      </div>
    </div>
  </footer>
</div>
