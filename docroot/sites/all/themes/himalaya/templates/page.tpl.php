<?php

/**
 * @file
 * Default theme implementation to display a page.
 *
 * Available variables:
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
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar']: Items for the first sidebar.
 * - $page['first_footer']: Items for the first footer column.
 * - $page['second_footer']: Items for the second footer column.
 * - $page['third_footer']: Items for the third footer column.
 * - $page['forth_footer']: Items for the fourth footer column.
 * - $page['footer']: Items for the footer region.
 */
?>
<div id="page-wrapper" class="section with-arrow">
  <div id="page">
    <div id="header">
      <nav id="navigation" class="margin_bottom">
        <div class="container mean-container">
          <?php if ($main_menu): ?>
            <div class="mean-bar">
              <a href="#nav" class="meanmenu-reveal" style="background:;color:#fff;right:0;left:auto;">
          	    <span></span>
		        <span></span>
                <span></span>
              </a>
              <nav class="mean-nav submenu">
                <?php print theme('links__system_main_menu', array(
                  'links' => $main_menu,
                  'attributes' => array(
                    'id' => 'menu',
                  ),
                  'heading' => array(
                    'text' => t('Main menu'),
                    'level' => 'h2',
                    'class' => array(
                      'element-invisible',
                    ),
                  ),
                 ));
                ?>
              </nav>
            </div>
          <?php endif; ?>          
          <?php if ($logo): ?>
            <a id="logo" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
              <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
            </a>
          <?php endif; ?>
          <?php if ($main_menu): ?>
            <div id="main-menu">
              <?php print theme('links__system_main_menu', array(
                'links' => $main_menu,
                'attributes' => array(
                  'id' => 'menu',
                ),
                'heading' => array(
                  'text' => t('Main menu'),
                  'level' => 'h2',
                  'class' => array(
                    'element-invisible',
                  ),
                ),
               ));
              ?>
            </div> <!-- /#main-menu -->
            <?php endif; ?>
               
        </div>
      </nav>
    </div> <!-- /.section, /#header -->
    <div class="col-100 float_left">
      <?php if ($site_name || $site_slogan): ?>
        <div class="site-slogan-name container">
          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <h1 id="site-name">
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
              </h1>
            <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
              </h1>
            <?php endif; ?>
          <?php endif; ?>
          <?php if ($site_slogan): ?>
            <div id="site-slogan">
              <?php print $site_slogan; ?>
            </div>
          <?php endif; ?>
        </div> <!-- /#name-and-slogan -->
      <?php endif; ?>
    </div>
    <div class="message col-100 float_left">
      <?php if ($messages): ?>
        <div id="messages" class="container">
          <div class="section clearfix">
            <?php print $messages; ?>
          </div>
        </div> <!-- /.section, /#messages -->
      <?php endif; ?>
    </div>
    <?php if ($page['header']): ?>
      <div class="container">
        <div class="header-region col-100 float_left">
          <?php print render($page['header']); ?>
        </div>
      </div>
    <?php endif; ?>
    <?php if ($page['help']): ?>
      <div class="container">
        <div class="help-wrapper float_left col-100">
          <?php print render($page['help']); ?>
        </div>
      </div>
    <?php endif; ?>
    <div id="main-wrapper" class="clearfix paddingt">
      <div id="main" class="clearfix container">
        <?php if ($page['sidebar']): ?>
          <div id="sidebar" class="column sidebar float_left">
            <div class="section">
              <?php print render($page['sidebar']); ?>
            </div>
          </div> <!-- /.section, /#sidebar -->
      <div id="content" class="two-column float_left with-left-sidebar">
        <?php if ($breadcrumb): ?>
          <div class="breadcrumb col-100 float_left">
     	      <div id="breadcrumb">
              <?php print $breadcrumb; ?>
            </div>
          </div>
   		  <?php endif; ?>
      	<div class="section col-100 float_left">        
          <a id="main-content"></a>
            <?php print render($title_prefix); ?>
              <?php if ($title): ?>
                <h1 class="title" id="page-title">
                  <?php print $title; ?>
                </h1>
              <?php endif; ?>
            <?php print render($title_suffix); ?>
              <?php if (!empty($tabs)): ?>
                <?php print render($tabs); ?>
              <?php endif; ?>
            <?php if ($action_links): ?>
            <ul class="action-links">
              <?php print render($action_links); ?>
            </ul>
            <?php endif; ?>
            <?php print render($page['content']); ?>
            <?php print $feed_icons; ?>
        </div>
      </div> <!-- /.section, /#content -->
    <?php endif; ?>
    <?php if (!$page['sidebar']): ?>
      <div class="help-wrapper col-100 float_left">
        <?php print render($page['help']); ?>
      </div>     
      <div id="content" class="column col-100 float_left"><div class="section">        
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1 class="title" id="page-title">
            <?php print $title; ?>
          </h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if (!empty($tabs)): ?>
            <?php print render($tabs); ?>
        <?php endif; ?>
        <?php if ($action_links): ?>
          <ul class="action-links">
            <?php print render($action_links); ?>
          </ul>
        <?php endif; ?>
        <?php print render($page['content']); ?>
        <?php print $feed_icons; ?>
      </div></div> <!-- /.section, /#content -->
    <?php endif; ?>      
  </div>
  </div> <!-- /#main, /#main-wrapper -->
</div>
<div class="footer-section container">
  <div class="col-100 float_left footer-inner-section">
    <?php if ($page['first_footer'] || $page['second_footer'] || $page['third_footer'] || $page['forth_footer']): ?>
      <div id="footer-columns" class="clearfix col-100 float_left">
        <div class="footer-col footer_first_wrapper column-3  float_left"><?php print render($page['first_footer']); ?></div>
        <div class="footer-col footer_second_wrapper column-3 float_left"><?php print render($page['second_footer']); ?></div>
        <div class="footer-col footer_third_wrapper column-3 float_left"><?php print render($page['third_footer']); ?></div>
        <div class="footer-col footer_fourth_wrapper column-3 float_left"><?php print render($page['forth_footer']); ?></div>
      </div> <!-- /#footer-columns -->
    <?php endif; ?>
  </div>
</div>
</div> <!-- /#page, /#page-wrapper -->
<div id="footer">
  <div class="section">  
    <?php if ($page['footer']): ?>
      <div class="section section-full-colored">
      	<div class="section-content center" >
        <?php print render($page['footer']); ?>
</div>
</div> <!-- /#footer -->
<?php endif; ?>
</div><!-- /.section -->
</div>
<!-- /#footer-wrapper -->
