<?php
/**
 * @file
 * Theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. themes/simpleclean.
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
 * Regions:
 * - $page['header']: Items for the header region.
 * - $page['featured']: Items for the featured region.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 */
?>
<div id="wrap">
  <div id="header">
    <?php if ($page['header']): ?>
      <?php print render($page['header']); ?>
    <?php endif; ?>
    
    <div id="logo">
      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
      <?php else: ?>
        <?php if ($site_name): ?>
          <h1 id="logo-text"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
        <?php endif; ?>
      <?php endif; ?>
      <?php if ($site_slogan): ?>
        <p id="slogan"><?php print $site_slogan; ?></p>
      <?php endif; ?>

    </div>
    
    <div class="clear"></div>

    <?php if ($main_menu): ?>
      <div  id="nav">
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu-links',
            'class' => array('links', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </div>
      <?php endif; ?>
  </div>

  <div id="content-wrap"> 
    <?php 
      $simpleclean_mission = theme_get_setting('simpleclean_mission');
      if ($simpleclean_mission && $is_front): ?>
        <div id="mission"><?php print render($simpleclean_mission); ?></div>
    <?php endif; ?>
    <?php if ($page['highlighted']): ?>
      <div id="highlighted"><?php print render($page['highlighted']); ?></div>
    <?php endif; ?>

    <div id="content">
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="node-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): ?>
        <div class="tabs"><?php print render($tabs); ?></div>
      <?php endif; ?>
      <?php if ($show_messages): ?>
        <?php print $messages; ?>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
      
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div>
    
    <div id="sidebar">
      <?php if ($page['sidebar_first']): ?>
        <?php print render($page['sidebar_first']); ?>
      <?php endif; ?>
    </div>

  </div>

  <div class="clear"></div>

  <div id="footer-wrap">
  
  <?php if ($page['footer']): ?>
    <?php print render($page['footer']); ?>
  <?php endif; ?>     
  
    <div id="footer">
  <p>This site is powered by <a href="http://drupal.org/">Drupal</a>. Theme: <a href="http://drupal.org/project/simpleclean">Simple Clean</a> by <a href="http://drupal.org/user/765764">acke</a> @ <a href="http://www.happiness.se/">happiness</a>.</p>
    <?php // Feel free to remove credits if you want your site even cleaner ;) /acke ?>    
    </div>
  </div>
    
  <div class="clear"></div>

</div>
