<?php
/**
 * @file
 * icebusiness theme's implementation to display a single Drupal page.
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
 * - $secondary_menu_heading: The title of the menu used by the secondary links.
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
 * - $page['navigation']: Items for the navigation region, below the main menu (if any).
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['footer']: Items for the footer region.
 * - $page['bottom']: Items to appear at the bottom of the page below the footer.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see icebusiness_preprocess_page()
 * @see template_process()
 */
?>
<div id="page-wrapper">
<div id="page">
<?php //$main_menu = menu_navigation_links('main-menu'); ?>

  <header id="header" class="clearfix" role="banner"> <div class="section clearfix"><div id="header-inner">

    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <hgroup id="name-and-slogan">
        <?php if ($site_name): ?>
          <h1 id="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
          </h1>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <h2 id="site-slogan"><?php print $site_slogan; ?></h2>
        <?php endif; ?>
      </hgroup><!-- /#name-and-slogan -->
    <?php endif; ?>  
<?php print render($page['header']); ?>

   <?php if ($page['header_menu']): ?>
 <div id="header_top_menu"><?php print render($page['header_menu']); ?></div>
 <?php endif; ?>
 
  <?php if (($main_menu_expanded) && !($page['header_menu'])) : ?>
  
  <div id="main-menu" style="margin-top:45px;">
  
  <?php else: ?>
  <div id="main-menu">
  
  <?php endif; ?>
  
  <div id="main-menu-inner">
  <?php print render($main_menu_expanded); ?>
  </div></div>

    <?php print render($page['header']); ?>

  </div></div></header>
  
   <?php if ($page['navigation']): ?>
        <div id="navigation"><div class="section clearfix">
                <?php print render($page['navigation']); ?>	  
        </div></div><!-- /.section, /#navigation -->
      <?php endif; ?><!-- /#navigation -->
      
      <!-- *********** #slider ************ -->
	  <?php   $sld = theme_get_setting('use_slider');
	      if (($page['slider']) || $sld): ?>
			   <div id="slider">
			   <div id="slid_show">
			   <?php if ($page['slider']): ?><?php print render($page['slider']); ?>
			 <?php else: ?>
					<div class="content">
					<div class="slideshow">
				     <?php  if (theme_get_setting('image1_path')): ?>
					<img alt="slide1" title="slide1" height="290" width="914" src="<?php print file_create_url(theme_get_setting('image1_path')); ?> " />
					<?php endif; ?> 
					<?php  if (theme_get_setting('image2_path')): ?>
					<img alt="slide1" title="slide1" height="290" width="914" src="<?php print file_create_url(theme_get_setting('image2_path')); ?> " />
					<?php endif; ?> 
					<?php  if (theme_get_setting('image3_path')): ?>
					<img alt="slide1" title="slide1" height="290" width="914" src="<?php print file_create_url(theme_get_setting('image3_path')); ?> " />
					<?php endif; ?> 
				   </div> 
					</div> 
            <?php endif; ?> 
	   </div><!-- slid_show -->
	   </div><!-- slider -->
	   <?php endif; ?> 
	  
	   <!-- *********** End #slider ************ -->
 <?php if ($page['tab_boxes']): ?>
 <div id="tabs-wrp"> <div class="tabs-bg"><?php print render($page['tab_boxes']); ?></div></div>
 <?php endif; ?>

  <div id="main" class="clearfix">

    <div id="content" class="column" role="main">
      <?php print render($page['highlighted']); ?>
      <?php print $breadcrumb; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <?php print render($tabs); ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      
      <?php print render($page['content']); ?>
     
      <?php print $feed_icons; ?>
    </div><!-- /#content -->



    <?php
      // Render the sidebars to see if there's anything in them.
      $sidebar_first  = render($page['sidebar_first']);
      $sidebar_second = render($page['sidebar_second']);
	  $testimonials = render($page['testimonials']);
	  $right_sidee_block = render($page['right_sidee_block']);
    ?>
      <?php if ($sidebar_first || $testimonials || $right_sidee_block ): ?>
	   <div id="sidebar-first">
      <?php print $sidebar_first; ?>
	    <?php if ($testimonials): ?>
          <div class="testimonials"><?php print $testimonials; ?></div>
        <?php endif; ?>
         <?php if ($right_sidee_block): ?>
          <div class="right_sidee_block"><?php print $right_sidee_block; ?></div>
        <?php endif; ?>
       </div>
		  <?php endif; ?>
      <?php print $sidebar_second; ?>
      


  </div><!-- /#main -->

 

</div></div><!-- /#page -->


<div id="main-footer">
<div class="section">
	<div id="footer-message"><?php print render($page['footer_message']); ?></div>
	<?php if($page['user_1']): ?>
		<div class="user_1"><?php print render($page['user_1']); ?></div>
	<?php endif; ?>
	<?php if ($page['user_2']): ?>
		<div class="user_2"><?php print render($page['user_2']); ?>	 
        </div>		
      <?php endif; ?>


	
	
		<div class="user_3"><?php if ($page['user_3']): ?><?php print render($page['user_3']); ?><?php endif; ?>	
		 <h2>Recent Work</h2>
		<div><img src="<?php  print base_path().drupal_get_path('theme','icebusiness');?>/images/recent-work.jpg" alt="Our Recent Work" /></div> 
		</div>
	


	
		<div class="user_4"><?php if ($page['user_4']): ?><?php print render($page['user_4']); ?><?php endif; ?>	
	<div class="region-user-4" style=" max-width: 150px;">
	<h2>Social Network</h2><a href="<?php echo theme_get_setting('facebook_username'); ?>" style="float: left; margin-top:16px;"><img src="<?php  print base_path().drupal_get_path('theme','icebusiness');?>/images/social-networking/facebook.png" title="Facebook"></a>
<a href="<?php echo theme_get_setting('twitter_username'); ?>"><img src="<?php  print base_path().drupal_get_path('theme','icebusiness');?>/images/social-networking/twitter.png" title="Twitter"></a>
<a href="<?php echo theme_get_setting('linkedin_username'); ?>"><img src="<?php  print base_path().drupal_get_path('theme','icebusiness');?>/images/social-networking/LinkedIn.png" title="LinkedIn"></a>
<a href="<?php echo theme_get_setting('blog_username'); ?>"><img src="<?php  print base_path().drupal_get_path('theme','icebusiness');?>/images/social-networking/blogger.png" title="Blog"></a></div>	

		 </div>
         <div class="copy_right"><?php print render($page['footer']); ?></div>
 

<?php

print render($page['bottom']); ?>
