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

<div id="page" class="page-default"> <a name="Top" id="Top"></a>
  <?php if(isset($page['show_skins_menu']) && $page['show_skins_menu']):?>
  <?php print $page['show_skins_menu'];?>
  <?php endif;?>
  <?php if($headline = render($page['headline'])): ?>
  <div id="headline-wrapper" class="wrapper">
    <div class="container">
      <div class="row">
        <div class="span12 clearfix"> <?php print $headline; ?> </div>
      </div>
    </div>
  </div>
  <?php endif;?>
  
  <!-- HEADER -->
  <div id="header-wrapper" class="wrapper">
    <div class="container">
      <div class="row">
        <div class="span12 clearfix">
          <div id="header" class="clearfix">
            <?php if ($logo): ?>
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo"> <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /> </a>
            <?php endif; ?>
            <?php if ($site_name || $site_slogan): ?>
            <div id="name-and-slogan" class="hgroup">
              <?php if ($site_name): ?>
              <h1 class="site-name"> <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"> <?php print $site_name; ?> </a> </h1>
              <?php endif; ?>
              <?php if ($site_slogan): ?>
              <p class="site-slogan"><?php print $site_slogan; ?></p>
              <?php endif; ?>
            </div>
            <?php endif; ?>
            <?php if ($header = render($page['header'])): print $header; endif; ?>
            <?php if ($secondary_menu): ?>
            <div id="secondary-menu" class="navigation"> <?php print theme('links__system_secondary_menu', array(
	              'links' => $secondary_menu,
	              'attributes' => array(
	                'id' => 'secondary-menu-links',
	                'class' => array('links', 'inline', 'clearfix'),
	              ),
	              'heading' => array(
	                'text' => t('Secondary menu'),
	                'level' => 'h2',
	                'class' => array('element-invisible'),
	              ),
	            )); ?> </div>
            <!-- /#secondary-menu -->
            <?php endif; ?>
            <?php if($main_menu = render($page['main_menu'])): ?>
            <div id="main-menu-wrapper">
              <div class="clearfix">
			    <a title="Navigation Icon" href="javascript:void(0);" class="responsive-menu-button">Menu</a>
                <?php print $main_menu; ?> </div>
            </div>
            <?php endif;?>
          </div>
        </div>
      </div>
    </div>
  </div>
  <!-- //HEADER -->
  
  <?php if(($featured = render($page['featured'])) || $title): ?>
  <div id="featured-wrapper" class="wrapper">
    <div class="featured-inner">
      <div class="featured-inner2">
        <div class="container">
          <div class="row">
            <div class="span12 clearfix"> <?php print $featured; ?> <?php print render($title_prefix); ?>
              <?php if ($title): ?>
              <h1 id="page-title"><?php print $title; ?></h1>
              <?php endif; ?>
              <?php print render($title_suffix); ?> </div>
          </div>
        </div>
      </div>
    </div>
  </div>
  <?php endif;?>
  <?php if($messages || $page['help']): ?>
  <div id="system-messages-wrapper" class="wrapper">
    <div class="container">
      <div class="row">
        <div class="grid-inner span12 clearfix"> <?php print $messages . render($page['help']); ?> </div>
      </div>
    </div>
  </div>
  <?php endif; ?>
  <div id="main-wrapper" class="wrapper">
    <div class="container">
      <div class="row clearfix"> 
        <!-- MAIN CONTENT -->
        <div id="main-content" class="span<?php print $regions_width['content']?>">
          <div class="grid-inner clearfix">
            <?php if ($tabs = render($tabs)): ?>
            <div class="tabs"><?php print $tabs; ?></div>
            <?php endif; ?>
            <?php if ($highlighted = render($page['highlighted'])): print $highlighted; endif; ?>
            <?php if ($action_links = render($action_links)): ?>
            <ul class="action-links">
              <?php print $action_links; ?>
            </ul>
            <?php endif; ?>
            <?php if ($content = render($page['content'])): print $content; endif; ?>
          </div>
        </div>
        <!-- //MAIN CONTENT -->
        
        <?php if ($regions_width['sidebar_first']): ?>
        <div id="sidebar-first-wrapper" class="span<?php print $regions_width['sidebar_first'];?>">
          <div class="grid-inner clearfix"> <?php print render($page['sidebar_first']); ?> </div>
        </div>
        <?php endif; ?>
        <?php if ($regions_width['sidebar_second']): ?>
        <div id="sidebar-second-wrapper" class="span<?php print $regions_width['sidebar_second'];?>">
          <div class="grid-inner clearfix"> <?php print render($page['sidebar_second']); ?> </div>
        </div>
        <?php endif; ?>
      </div>
    </div>
  </div>
  <?php if($panel_first): ?>
  <!-- PANEL FIRST -->
  <div id="panel-first-wrapper" class="wrapper panel panel-first">
    <div class="panel-first-inner">
      <div class="panel-first-inner-2">
        <div class="container">
          <div class="row clearfix"> <?php print $panel_first;?> </div>
        </div>
      </div>
    </div>
  </div>
  <!-- //PANEL FIRST -->
  <?php endif; ?>
  <?php if ($footer = render($page['footer'])): ?>
  <!-- FOOTER -->
  <div id="footer-wrapper" class="wrapper">
    <div class="container">
      <div class="row">
        <?php if ($breadcrumb): ?>
        <!-- BREADCRUMB -->
        <div class="span12 clearfix">
          <div id="breadcrumb-wrapper" class="clearfix">
            <?php if ($breadcrumb):?>
            <?php print $breadcrumb; ?>
            <?php endif; ?>
            <a title="<?php print t('Back to Top')?>" class="btn-btt" href="#Top"><?php print t('Back to Top')?></a> </div>
        </div>
        <!-- //BREADCRUMB -->
        <?php endif; ?>
        <div class="span12 clearfix">
          <div id="footer" class="clearfix"> <?php print $footer; ?> </div>
        </div>
      </div>
    </div>
  </div>
  <!-- //FOOTER -->
  <?php endif; ?>
</div>
