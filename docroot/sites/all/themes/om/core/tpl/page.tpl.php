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
 * - $page['header_block']: Items for the header region.
 * - $page['menu_bar']: Items for the header region.
 * - $page['highlighted']: Items for the highlighted content region. 
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_second']: Items for the second sidebar. 
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 *
 * Legacy
 * Creating a Region 
 * - param: 
 *   region_wrapper - function name to process the regions
 *   _region_vars   - array($region_name, $region_classes, $region_inner)
 *
 * <?php print theme('region_wrapper', _region_vars($page['region_name'], $region_classes, 0)); ?>
 * 
 * or you can break it to:
 *
 * <?php $region_name_array = _region_vars($page['region_name'], $region_classes, 0); ?>
 * <?php print theme('region_wrapper', $region_name_array); ?>
 *
 * New
 * All region settings are in regions.php
 * Just print region variable name + _region, ex. <?php print $content_region; ?>
 *
 * Adds grid-x, automatic value based on the presence and widths of side bar first and second.
 * <?php print $wrapper_middle_grid; ?>
 *
 */
?>

<div class="wrapper-outer">
  <div id="header" class="wrapper wrapper-header"><div class="wrapper-inner">
    <?php print theme('identity', $identity_vars); ?>
    <?php print $header_block_region; ?>
    <div class="om-clearfix"></div>
  </div></div><!-- /.wrapper-inner, /#header -->
  <div id="nav" class="wrapper wrapper-nav"><div class="wrapper-inner">
    <?php print theme('menu', $main_menu_vars); ?>
    <?php print theme('menu', $secondary_menu_vars); ?>
    <?php print $menu_bar_region; ?>      
  </div></div> <!-- /.wrapper-inner, /#nav -->
  <?php print $highlighted_region; ?>
  <div class="wrapper wrapper-main"><div class="wrapper-inner">
    <?php print $sidebar_first_region; ?>
    <div class="wrapper wrapper-middle<?php print $wrapper_middle_grid; ?>">
      <?php print $breadcrumb; ?>
      <div class="wrapper-inner">
        <?php print theme('content_elements', $content_elements_vars); ?>
        <?php print render($page['help']); ?>
        <?php print $messages; ?>
        <?php print $content_region; ?>
        <?php print $feed_icons; ?>
      </div> <!-- /.wrapper-inner -->
    </div> <!-- /.wrapper-middle -->
    <?php print $sidebar_second_region; ?>
    <div class="om-clearfix"></div>
  </div></div> <!-- /.wrapper-inner, /.wrapper-main -->
	<?php print $footer_region; ?>
</div> <!-- /#wrapper-outer -->


