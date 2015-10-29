<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
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
 * @see bootstrap_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see bootstrap_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
<header class="wrap header">
	<div class="container wrap-inner">
		<div class="row">
      <div class="col-md-4">
      	<div class="social-list">
        	<ul>
            <li><a href="<?php print theme_get_setting('facebook'); ?>" class="facebook">Facebook</a></li>
            <li><a href="<?php print theme_get_setting('twitter'); ?>" class="twitter">Twitter</a></li>
            <li><a href="<?php print theme_get_setting('pinterest'); ?>" class="pinterest">Pinterest</a></li>
          </ul>
        </div>        
      </div>
      <div class="col-md-4">
      	<div class="site-logo">
        	<?php if (!empty($site_name)): ?>
          <a class="logo" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
          	<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a>
          <?php endif; ?>
          <?php if (!empty($site_slogan)): ?>
          	<h3 class="slogan"><?php print $site_slogan; ?></h3>
          <?php endif; ?>	        
        </div>        
      </div>
      <div class="col-md-4">
      	<div class="search-form">
        	<?php print render($page['search_form']); ?>
        </div><!--	/.search-form	!-->
      </div>
    </div>    
	</div>
</header><!--	/.header	!-->

<section class="wrap main">
	<div class="container wrap-inner">
    <nav class="wrap header-menu navbar navbar-default">
      <!-- Brand and toggle get grouped for better mobile display -->
      <div class="navbar-header">
        <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#header-menu">
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
      </div>
      <div class="collapse navbar-collapse" id="header-menu">
      	<?php if (!empty($page['header_menu'])): ?>
					<?php print render($page['header_menu']); ?>
        <?php endif; ?>
      </div>
    </nav><!--	/.header-menu	!-->
    
    <div class="row main-inner">
    	<div class="main-table">
      	<div class="main-row">
		      <section class="col-md-9 main-content">
            <div class="main-content-inner">
            	<?php if (!empty($breadcrumb)): print $breadcrumb; endif;?>
              <a id="main-content"></a>
              <?php print render($title_prefix); ?>
							<?php if (!empty($title)): ?>
                <h1 class="page-header"><?php print $title; ?></h1>
              <?php endif; ?>
              <?php print render($title_suffix); ?>
              <?php print $messages; ?>
              <?php if (!empty($tabs)): ?>
                <?php print render($tabs); ?>
              <?php endif; ?>
              <?php if (!empty($page['help'])): ?>
								<?php print render($page['help']); ?>
              <?php endif; ?>
              <?php if (!empty($action_links)): ?>
                <ul class="action-links"><?php print render($action_links); ?></ul>
              <?php endif; ?>              
              <div class="region region-content">
                <?php print render($page['content']); ?>
              </div>
            </div><!--	/.main-content-inner	!-->
          </section>
          <aside class="col-md-3 sidebar-right" role="complementary">
            <?php print render($page['sidebar_right']); ?>
          </aside>
      	</div>
      </div>
      <div class="clr"></div>
      
      <div class="before-footer">
        <div class="col-md-3 bf-column1">
          <div class="region region-footer">
            <?php print render($page['before_footer1']); ?>
          </div>
        </div>
        <div class="col-md-3 bf-column2">
          <div class="region region-footer">
            <?php print render($page['before_footer2']); ?>
          </div>
        </div>
        <div class="col-md-3 bf-column3">
          <div class="region region-footer">
            <?php print render($page['before_footer3']); ?>
          </div>
        </div>
        <div class="col-md-3 bf-column4">
          <div class="region region-footer">
            <?php print render($page['before_footer4']); ?>
          </div>
        </div>
      </div><!--	/.before-footer	!-->      
          
      <footer class="footer">
        <div class="col-md-6 copyright">
					<?php print theme_get_setting('copyright'); ?>
          <?php $credit = theme_get_setting('credit');
          if (!isset($credit) || (theme_get_setting('credit') != 1)):
          print t('Theme by <a href="@url" title="New looks for Drupal">cDrupal</a>', array('@url' => 'http://cdrupal.com/'));
          endif; ?>
          </div>
        <div class="col-md-6 footer-menu">
          <?php print render($page['footer_menu']); ?>
        </div>
      </footer>
      
    </div><!--	/.main-inner	!-->
  </div>
</section>
