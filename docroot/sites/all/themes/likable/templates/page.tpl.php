<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
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
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
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
<div id="page-wrapper"><div id="page">

  <div id="header"><div class="section">
		<?php if ($logo): ?>
			<div id="logo">
			    <?php if($is_front): ?>
			    	<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
			    <?php else: ?>
			      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
			    <?php endif; ?>
			</div>
		<?php endif; ?>
		
		<?php if ($site_name || $site_slogan): ?>
			<div id="sitename">
			<h1>
			    <?php if($is_front): ?>
			    	<?php print $site_name; ?>
			    <?php else: ?>
			     <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
			    <?php endif; ?>
			</h1>
			<h2><?php print $site_slogan; ?></h2>
			</div>
		<?php endif; ?>

	    <div id="main-menu">
	       	<?php 
              $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
              print drupal_render($main_menu_tree);
            ?>
	    </div>
	    
    	<?php print render($page['header']); ?>
    	
  </div></div> <!-- /#header -->
  
  

  <div id="main-wrapper" class="clearfix"><div id="main" class="clearfix">
  
  		<?php if($is_front): ?><div id="slideshow"><?php print render($page['slideshow']); ?></div><?php endif; ?>		
  
		<?php if($page['top_first'] || $page['top_second'] || $page['top_third'] || $page['top_fourth']): ?>
		    <div id="top-columns" class="col-<?php print (bool) $page['top_first'] + (bool) $page['top_second'] + (bool) $page['top_third'] + (bool) $page['top_fourth']; ?>">
		          <?php if($page['top_first']): ?>
		          <div class="column">
		            <?php print render($page['top_first']); ?>
		          </div>
		          <?php endif; ?>
		          
		          <?php if($page['top_second']): ?>
		          <div class="column">
		            <?php print render($page['top_second']); ?>
		          </div>
		          <?php endif; ?>
		          
		          <?php if($page['top_third']): ?>
		          <div class="column">
		            <?php print render($page['top_third']); ?>
		          </div>
		          <?php endif; ?>
		          
		          <?php if($page['top_fourth']): ?>
		          <div class="column">
		            <?php print render($page['top_fourth']); ?>
		          </div>
		          <?php endif; ?>
		          <div class="clear"></div>
		    </div>
	    <?php endif; ?>
    
	    <?php if ($page['sidebar_first']): ?>
	      <div id="sidebar-first" class="column sidebar"><div class="section">
	        <?php print render($page['sidebar_first']); ?>
	      </div></div> <!-- /.section, /#sidebar-first -->
	    <?php endif; ?>

    	<div id="content" class="column"><div class="section">
    
	    	<?php if ($breadcrumb): ?>
		      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
		    <?php endif; ?>
	    
		     <?php if ($messages): ?>
			    <div id="messages"><div class="section clearfix">
			      <?php print $messages; ?>
			    </div></div> <!-- /.section, /#messages -->
			  <?php endif; ?>
			  
			  <?php if ($page['top_content']): ?><div id="top_content"><?php print render($page['top_content']); ?></div><?php endif; ?>
			  
		      <?php print render($title_prefix); ?>
		      <?php if (!$is_front && $title): ?>
		        <h1 class="title" id="page-title">
		          <?php print $title; ?>
		        </h1>
		      <?php endif; ?>
		      <?php print render($title_suffix); ?>
		      <?php if ($tabs): ?>
		        <div class="tabs">
		          <?php print render($tabs); ?>
		        </div>
		      <?php endif; ?>
		      <?php if ($action_links): ?>
		        <ul class="action-links">
		          <?php print render($action_links); ?>
		        </ul>
		      <?php endif; ?>
     
		      <?php if ($is_front && $page['front_content']): ?>
		      	<div id="front_content"> 
		      		<?php print render($page['front_content']); ?>
		      		<div class="clear"></div>
		      	</div>
		      <?php endif; ?>
  
      		  <?php print render($page['content']); ?>
      
    	</div></div> <!-- /.section, /#content -->

	    <?php if ($page['sidebar_second']): ?>
	      <div id="sidebar-second" class="column sidebar"><div class="section">
	      	<?php print render($page['sidebar_second']); ?>
	      </div></div> <!-- /.section, /#sidebar-second -->
	    <?php endif; ?>
    
    	<div class="clear"></div>
    
    	<?php if($page['bottom_first'] || $page['bottom_second'] || $page['bottom_third'] || $page['bottom_fourth']): ?>
		    <div id="bottom-columns" class="col-<?php print (bool) $page['bottom_first'] + (bool) $page['bottom_second'] + (bool) $page['bottom_third'] + (bool) $page['bottom_fourth']; ?>">
		          <?php if($page['bottom_first']): ?>
		          <div class="column">
		            <?php print render($page['bottom_first']); ?>
		          </div>
		          <?php endif; ?>
		          
		          <?php if($page['bottom_second']): ?>
		          <div class="column">
		            <?php print render($page['bottom_second']); ?>
		          </div>
		          <?php endif; ?>
		          
		          <?php if($page['bottom_third']): ?>
		          <div class="column">
		            <?php print render($page['bottom_third']); ?>
		          </div>
		          <?php endif; ?>
		          
		          <?php if($page['bottom_fourth']): ?>
		          <div class="column">
		            <?php print render($page['bottom_fourth']); ?>
		          </div>
		          <?php endif; ?>
		          <div class="clear"></div>
		    </div>
	    <?php endif; ?>

  </div></div> <!-- /#main, /#main-wrapper -->

</div></div>
  

	<div id="footer-wrapper"><div class="section">
  		
		<?php if($page['footer_first'] || $page['footer_second'] || $page['footer_third'] || $page['footer_fourth']): ?>
		    <div id="footer-columns" class="col-<?php print (bool) $page['footer_first'] + (bool) $page['footer_second'] + (bool) $page['footer_third'] + (bool) $page['footer_fourth']; ?>">
				<?php if($page['footer_first']): ?>
				<div class="column">
					<?php print render($page['footer_first']); ?>
				</div>
				<?php endif; ?>
				
				<?php if($page['footer_second']): ?>
				<div class="column">
					<?php print render($page['footer_second']); ?>
				</div>
				<?php endif; ?>
				
				<?php if($page['footer_third']): ?>
				<div class="column">
					<?php print render($page['footer_third']); ?>
				</div>
				<?php endif; ?>
				
				<?php if($page['footer_fourth']): ?>
				<div class="column">
					<?php print render($page['footer_fourth']); ?>
				</div>
				<?php endif; ?>
		    	<div class="clear"></div>
		    </div>
	    <?php endif; ?>
    
	    <?php if ($page['footer']): ?>
	        <?php print render($page['footer']); ?>
	    <?php endif; ?>
    
	    <div id="copyright"><?php print t('Copyright'); ?> &copy; <?php echo date("Y"); ?> <?php print $site_name; ?></div>
		
		<div id="developed" style="text-align: right"><?php print t('Developed by'); ?>  <a href="http://www.best-drupal.com">Katrin</a>

		</div>
		<div class="clear"></div>
	
  </div></div> <!-- /.section, /#footer-wrapper -->

 <!-- /#page, /#page-wrapper -->
