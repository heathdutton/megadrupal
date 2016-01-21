<?php

/**
 * @file
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
 * - $front_page: The URL of the front page. 
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
 *
 */
?>
<p><a id="top"></a></p>
<div id="wrapper-header">
  <div id="header">
    <?php if($is_front): ?>
      <h1 id="site-name">
        <a href="<?php print $front_page; ?>" title="<?php print strip_tags($site_name); ?>" rel="home">
          <?php if ($logo): ?>
            <img src="<?php print $logo; ?>" alt="<?php print strip_tags($site_name) . t(' Home'); ?>" id="logo" />
          <?php endif; ?>
          <?php print strip_tags($site_name); ?>
        </a>
      </h1>
    <?php else: ?>
      <div id="site-name">
        <a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>">
          <?php if ($logo): ?>
            <img src="<?php print $logo; ?>" alt="<?php print strip_tags($site_name) . t(' Home'); ?>" id="logo" />
          <?php endif; ?>
          <?php print strip_tags($site_name); ?>
        </a>
      </div><!-- #site-name -->
    <?php endif; ?>
    
    <?php if ($main_menu): ?>
      <?php print theme('links__system_main_menu', array(
        'links' => $main_menu,
        'attributes' => array(
          'id' => 'main-menu-links',
          'class' => array('links', 'main-menu'),
        ),
        'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
    <?php endif; ?>
    
    <?php if($secondary_menu):?>  
      <?php print theme('links__system_secondary_menu', array(
        'links' => $secondary_menu,
        'attributes' => array(
          'id' => 'secondary-menu-links',
          'class' => array('links', 'secondary-menu'),
        ),
        'heading' => array(
            'text' => t('Secondary menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
    <?php endif; ?>
    
  </div><!-- #header -->
</div><!-- #wrapper-header -->

<div id="wrapper-main"> 
  <div id="main">
    <?php if ($page['header']): ?>
			<div id="topbar">
				<?php print render($page['header']); ?>
			</div><!-- #topbar -->
	  <?php endif; ?>
    
    <?php if ($site_slogan): ?>
      <h2 id='site-slogan'>
        <?php print $site_slogan; ?>
      </h2>
    <?php endif; ?>
    
    <div id="content">
      <div id="center">
        <a id="main-content"></a>
        <?php if ($breadcrumb): print $breadcrumb; endif; ?>
        
        <?php if ($page['highlighted']): ?>
          <div id="mission">
            <?php print render($page['highlighted']); ?>
          </div>
        <?php endif; ?>
        
        <?php if ($tabs): print '<div id="tabs-wrapper" class="clear">'; endif; ?>
          <?php print render($title_prefix); ?>
          <?php if ($title && !$is_front): ?>
            <?php print '<h1'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h1>'; ?>
          <?php endif; ?>
          <?php print render($title_suffix); ?>
        <?php if ($tabs): print render($tabs) . '</div>'; endif; ?>
        <?php if (!empty($tabs2)): print render($tabs2); endif; ?>
        <?php if ($action_links): ?>
          <ul class="action-links">
            <?php print render($action_links); ?>
          </ul>
        <?php endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($messages): print $messages; endif; ?>
        <?php print render($page['content']); ?>
        <?php print $feed_icons; ?>
        <p><a href="#top" class="to-top"><?php print t('Back to top'); ?></a></p>
      </div><!-- #center -->
      
      <?php if ($page['sidebar_second']): ?>
				<div id="sidebar-second" class="sidebar">
					<?php print render($page['sidebar_second']); ?>
				</div><!-- #sidebar-second -->
      <?php endif; ?>
    </div><!-- #content -->
    
    <?php if ($page['sidebar_first']): ?>
      <div id="sidebar-first" class="sidebar">
				<?php print render($page['sidebar_first']); ?>
			</div><!-- #sidebar-first -->
		<?php endif; ?>
  </div><!-- #main -->
  
  <div id="footer">
    <?php if ($page['footer']): ?>
      <?php print render($page['footer']); ?>
    <?php endif; ?>
	</div><!-- #footer -->
</div><!-- #wrapper-main -->


