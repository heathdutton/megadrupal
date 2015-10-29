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
 *   or themes/garland.
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
 * - $page['content']: The main content of the current page.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>
<div id="page">

  <header id="header" role="banner">
    <div class="pagewidth clearfix">
      
      <?php if ($logo): ?>
        <a id="logo" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>
      
      <a id="motto" title="Live Better!"></a>

      <div id="navbar" class="grid_12">
        <?php print drupal_render(menu_tree_output(menu_tree_page_data('main-menu'))); ?>
      </div>

    </div>
  </header>


  <div id="main" role="main">
    <div class="pagewidth clearfix">
    
      <?php if (!empty($messages) || !empty($help)): ?>
        <div id="messages">
          <?php if (!empty($messages)) {print $messages;} ?>
          <?php if (!empty($help)) {print $help;} ?>
        </div> <!-- /#messages -->
      <?php endif; ?>
        
      <div id="content" class="clearfix">
        <?php if ($tabs) { render($tabs); }   ?>
        <?php print render($page['content']); ?>    
      </div>
    
    </div>
  </div> <!-- /#main -->


  <footer id="footer" role="contentinfo">
    <div class="pagewidth clearfix">
    
      <div id="footerbar">
        <?php print drupal_render(menu_tree_output(menu_tree_page_data('main-menu'))); ?>
      </div>
      
      <a id="fogo" href="<?php print base_path(); ?>"></a>
      <div id="copyright">
        &copy; 2010 Panels 960gs, Inc.<br/>123 Sheep Drive, Doughnutville, PA 18018<br/>
        <a href="/about/terms-of-use">Terms</a> &#124; <a href="/about/privacy-policy">Privacy Policy</a> &#124; <a href="/sitemap.xml">Site Map</a>
      </div> 
      
    </div>
  </footer>

</div> <!-- /#page -->