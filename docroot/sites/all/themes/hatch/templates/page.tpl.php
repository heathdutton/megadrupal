<?php
/**
 * @file
 * Drupal Hatch theme implementation to display a single Drupal front page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template normally located in the
 * modules/system folder.
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
 * - $hide_site_name: TRUE if the site name has been toggled off on the theme
 *   settings page. If hidden, the "element-invisible" class is added to make
 *   the site name visually hidden, but still accessible.
 * - $hide_site_slogan: TRUE if the site slogan has been toggled off on the
 *   theme settings page. If hidden, the "element-invisible" class is added to
 *   make the site slogan visually hidden, but still accessible.
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
 */?>
<div id="container">
  
    <div class="container-12">
      <div id="header" class="grid-12">
        <div id="branding" class="grid-4">
          <?php if ($site_name): ?>
          <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>">
            <?php print $site_name; ?>
          </a>
        <?php endif; ?>
          <h2 id="site-description"><span><?php print render($site_slogan)?></span></h2>	
        </div><!--closing of branding div-->
        <!--<div id="menu-primary" class="menu-container grid-7">-->
      <?php// if ($main_menu): ?>
        <!--<div class="menu" >-->
        <div id="nav">
          <?php /*print theme('links__system_main_menu', array(
                          'links' => $main_menu,
                            'attributes' => array(
                            'id' => 'menu-primary-items',
                            'class' => array('links','inline', 'sf-js-enabled'),
                            ),
                            'heading' => array(
                            'text' => t('Main menu'),
                            'level' => 'h2',
                            'class' => array('element-invisible'),
                            ),
                          )); */
          ?>
                                 <?php 
          if (module_exists('i18n')) { 
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
        </div><!--closing of menu div-->
      <?php// endif;?>
    <!--</div><!--closing of menu-primary div-->
      </div><!--closing of header div-->
      <div id="main" class="grid-12" >
        <div id="masthead"  >
          <?php print render($tabs);?>
          <div id="content">
            <div class="hfeed alpha grid-8">
            <!--<div class="entry-content clearfix">-->
            <?php print render($page['content']); ?>
            <!--</div>-->
            </div>
          </div><!-- </div>closing of content div-->
          <div id="sidebar-primary" class="sidebar clearfix grid-4 omega">
			      <?php if ($page['primary_sidebar']): ?>
			        <?php print render($page['primary_sidebar']); ?>
            <?php endif; ?> 
          </div><!--closing of sidebar-primary div-->
				</div><!--closing of masthead div--> 
      </div><!--closing of main div-->
      <div id="sidebar-subsidiary" class="sidebar grid-12">
        <?php if ($page['footer1']): ?>
          <div class="textwidget alpha grid-4">
            <?php print render($page['footer1']); ?>
          </div>
        <?php endif;?>
        <!--<div id="hybrid-tags-2" class="widget tags widget-tags grid-4">-->
          <?php if ($page['footer2']): ?>
            <div class="textwidget grid-4">
              <?php print render($page['footer2']); ?> 
            </div>
          <?php endif;?>
        <!--</div>-->
        <!--<div id="hybrid-calendar-2" class="widget calendar widget-calendar grid-4">-->
          <?php if ($page['footer3']): ?>
      	    <div class="textwidget omega grid-4">
      	      <?php print render($page['footer3']); ?>
      	    </div>
      	  <?php endif;?>
        <!--</div>-->
      </div><!--closing of sidebar-subsidiary div-->
      <div id="footer" class="grid-1">
        <?php if ($page['footer']): ?>
          <?php print render($page['footer']); ?>
        <?php endif;?>
        <div class="footer_zyxware">Theme by <?php print l('Zyxware Technologies','http://www.zyxware.com');?></div>
      </div>
      </div><!--closing of container-12 div-->
  
</div><!--closing of container div-->

