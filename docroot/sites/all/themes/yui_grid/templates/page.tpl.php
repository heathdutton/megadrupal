<?php

/**
 * @file
 * YUI Grid theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * YUI Grid variables:
 * - $yui_sidebar_width: Unit size of the sidebar;
 * - $yui_sidebar_location: Left: 1; Right 2; None: 0;
 * - $yui_breadcrumbs: Display breadcrumbs or not;
 * - $yui_responsive: Responsive layout or not;
 * - $yui_main_width: Unit size of the main content;
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
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<div id="page-wrapper">
  <div id="page" class="yui3-g<?php if ($yui_responsive): print '-r'; endif; ?>">
    <div id="hd" class="yui3-u-1">
      <div class="hd-inner">
        <?php print render($page['header']); ?>
        <div  role="banner">
          <div id="logo">
            <?php if ($logo): ?><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a><?php endif; ?>
          </div>
          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <div id="site-name">
		<strong>
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
		</strong>
              </div>
              <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name">
		<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
              <?php endif; ?>
              <?php endif; ?>
          <?php if ($site_slogan): ?><div class='site-slogan'><?php print $site_slogan ?></div><?php endif; ?>
        </div>
        <div id="menu" role="navigation">
          <?php if ($main_menu): ?>
            <div id="navigation"><div class="section clearfix">
              <?php print theme('links__system_main_menu', array(
                'links' => $main_menu,
                'attributes' => array(
                  'id' => 'main-menu',
                  'class' => array('links', 'clearfix'),
                ),
                'heading' => array(
                  'text' => t('Main menu'),
                  'level' => 'h2',
                  'class' => array('element-invisible'),
                ),
              )); ?>
            </div></div> <!-- /.section, /#navigation -->
          <?php endif; ?>
        </div>
      </div>
    </div> <!-- /#hd -->

    <div id="bd" class="yui3-g<?php if ($yui_responsive): print '-r'; endif; ?>">
      <?php if ($yui_sidebar_location == 1 && !empty($page['sidebar'])): ?>
        <div class="yui3-u-<?php print $yui_sidebar_width;?>" id="sidebar" role="complementary">
          <?php print render($page['sidebar']); ?>
        </div>
      <?php endif; ?>
        <div class="yui3-u-<?php print (!empty($page['sidebar']) ? $yui_main_width : '1');?>" id="main" role="main">
	  <div class="main-inner">
	    <?php if ($yui_breadcrumbs): print $breadcrumb; endif; ?>
	      <a id="main-content"></a>
              <?php print render($title_prefix); ?>
	      <h1 class="title"><?php print $title; ?></h1>
              <?php print render($title_suffix); ?>
	      <div class="tabs">
	        <?php print render($tabs); ?>
	      </div>
	      <?php if ($show_messages): print $messages; endif; ?>
	        <?php print render($page['help']); ?>
	        <?php if ($action_links): ?>
		  <ul class="action-links">
		    <?php print render($action_links); ?>
		  </ul>
	        <?php endif; ?>
	        <?php print render($page['content']); ?>
	        <?php print $feed_icons; ?>
	  </div>
        </div>
        <?php if ($yui_sidebar_location == 2 && !empty($page['sidebar'])): ?>
          <div class="yui3-u-<?php print $yui_sidebar_width;?>" id="sidebar" role="complementary">
            <?php print render($page['sidebar']); ?>
          </div>
        <?php endif; ?>
    </div> <!-- /#bd -->

    <?php if (!empty($page['footer'])): ?>
      <div id="ft" role="contentinfo" class="yui3-u-1 clearfix">
        <?php print render($page['footer']); ?>
        <?php if ($secondary_menu): ?>
          <div id="secondary-menu" class="navigation">
            <?php print theme('links__system_secondary_menu', array(
              'links' => $secondary_menu,
              'attributes' => array(
                'id' => 'secondary-menu-links',
                'class' => array('links', 'clearfix'),
              ),
              'heading' => array(
                'text' => t('Secondary menu'),
                'level' => 'h2',
                'class' => array('element-invisible'),
              ),
            )); ?>
          </div> <!-- /#secondary-menu -->
        <?php endif; ?>
      </div> <!-- /#ft -->
    <?php endif; ?>

  </div> <!-- /#page -->
</div> <!-- /#page-wrapper -->
