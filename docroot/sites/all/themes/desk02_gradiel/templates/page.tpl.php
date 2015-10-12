<?php
// $Id: page.tpl.php,v 1.1 2011/02/04 14:35:58 jax Exp $

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
<div id="container">
  <div id="wrapper">
  <div id="header">

    <div id="header-top">
      <div id="secondary-menu" class="navigation">
        <?php if ($secondary_menu): ?>
          <?php print theme('links__system_secondary_menu', array(
            'links' => array_reverse($secondary_menu),
            'attributes' => array(
              'id' => 'secondary-menu-links',
              'class' => array('links', 'inline', 'clearfix'),
            ),
          )); ?>
        <?php endif; ?>
      </div> <!-- end secondary-menu -->

      <div id="main-menu" class="navigation">
        <?php if ($main_menu): ?>
          <?php print theme('links__system_main_menu', array(
            'links' => $main_menu,
            'attributes' => array(
              'id' => 'main-menu-links',
              'class' => array('links', 'clearfix'),
            ),
          )); ?>
        <?php endif; ?>
      </div> <!-- end main-menu -->
      
      <?php if ($page['search_box']): ?>
        <div id="search-box">
          <?php print render($page['search_box']); ?>
        </div><!-- end search-box -->
      <?php endif; ?>
    </div><!-- end header-top -->

  <?php if ($logo || $site_name): ?>
    <div id="header-branding">
        <?php
          if ($logo) {
            print '<a href="'. check_url($front_page) .'" title="'. $site_name .'"><img src="'. check_url($logo) .'" alt="'. $site_name .'" id="logo" /></a>';
          }
          print '<h1><a href="'. check_url($front_page) .'" title="'. $site_name .'">'. $site_name .'</a></h1>';
        ?>
    </div><!-- end header-branding -->
  <?php endif; ?>
  </div><!-- end header -->

<?php if ($page['top'] && $is_front): ?>
  <div id="top" class="region region-top">
    <?php print render($page['top']); ?>
  </div> <!-- end top -->
<?php endif; ?>

  <div id="main">
    <div id="content">
      <?php if ($breadcrumb): ?>
      <div id="breadcrumb">
        <?php print render($breadcrumb); ?>
      </div><!-- /breadcrumb -->
      <?php endif; ?>
      <?php print ($messages) ? $messages : '' ; ?>
      <?php if ($page['help']): ?>
        <div id="help">
          <?php print render($page['help']); ?>
        </div>
      <?php endif; ?>
      <?php if ($title): ?>
      <h1 class="title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php if ($tabs): ?>
      <div id="content-tabs">
        <?php print render($tabs); ?>
      </div><!-- end content-tabs -->
      <?php endif; ?>
      <?php if ($action_links): ?>
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
    </div>

  <?php if ($page['sidebar_first']): ?>
    <div id="sidebar-right" class="region region-right">
      <?php print render($page['sidebar_first']); ?>
    </div><!-- end sidebar-right -->
  <?php endif; ?>

  </div><!-- end main -->

  <?php if ($page['footer']): ?>
    <div id="footer">
      <?php print render($page['footer']); ?>
    </div> <!-- end footer -->
    <!-- Credit where credit is due -->
    <p class="credit"><a href="http://www.desk02.be" title="Design by Desk02">Desk02 theme</a></p>
  <?php endif; ?>

  </div><!-- end wrapper -->
</div><!-- end container -->