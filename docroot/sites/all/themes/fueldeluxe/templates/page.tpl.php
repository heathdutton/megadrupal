<?php
/*  Drupal starter theme provided by gazwal.com - Freelance Drupal Development Services  */

/**
 * @file
 * FuelDeLuxeD7's theme implementation to display a single Drupal page.
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

<div id="page-wrapper">
<div id="page" class="<?php print $classes; ?>" <?php print $attributes; ?>>

<!-- ______________________ HEADER _______________________ -->

<div id="header-wrapper">
	<div id="header">
		<div class="section clearfix">

	  	<div id="logo-title">

	  	<?php if ($logo): ?>
			<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
				<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
			</a>
		<?php endif; ?>

		<?php if ($site_name || $site_slogan): ?>
			<div id="name-and-slogan"<?php if ($hide_site_name && $hide_site_slogan) { print ' class="element-invisible"'; } ?>>
			<?php if ($site_name): ?>
				<?php if ($title): ?>
				<div id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
				<a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span>
				</a>
				</div>
				<?php else: /* Use h1 when the content title is empty */ ?>
				<h1 id="site-name"<?php if ($hide_site_name) { print ' class="element-invisible"'; } ?>>
				<a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span>
				</a>
				</h1>
				<?php endif; ?>
			<?php endif; ?>
			<?php if ($site_slogan): ?>
				<div id="site-slogan"<?php if ($hide_site_slogan) { print ' class="element-invisible"'; } ?>>
					<?php print $site_slogan; ?>
				</div>
			<?php endif; ?>
			</div><!-- /name-and-slogan -->
		<?php endif; ?>

		</div><!-- /logo-title -->

		<?php print render($page['header']); ?>

		<?php if ($secondary_menu): ?>
		<div id="secondary-menu">
        	<?php print theme('links__system_secondary_menu', array(
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
    )); ?>
		</div> <!-- /#secondary-menu -->
		<?php endif; ?>

		</div><!-- /section -->
	</div><!-- /header -->

	<!-- ______________________ NAVIGATION _______________________ -->

	<?php if ($main_menu): ?>
	<div id="main-menu" class="navigation">
	<div class="section clearfix">
        <?php print theme('links__system_main_menu', array(
      'links' => $main_menu,
      'attributes' => array(
        'id' => 'main-menu-links',
        'class' => array('links', 'inline', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      ),
    )); ?>
	</div><!-- /section -->
	</div> <!-- /#main-menu -->
	<?php endif; ?>

</div><!-- /header-wrapper -->


<!-- ______________________ TOP REGIONS _______________________ -->

<?php if ($page['top01']): ?>
	<div id="top01">
		<div class="section clearfix">
			<?php print render($page['top01']); ?>
		</div>
	</div>
<?php endif; ?>

<?php if ($page['top02']): ?>
	<div id="top02">
		<div class="section clearfix">
			<?php print render($page['top02']); ?>
		</div>
	</div>
<?php endif; ?>

<!-- ______________________ MAIN _______________________ -->

<div id="main-out">
<div id="main-in">
<div id="main" class="clearfix">

	<div id="content"><!-- CONTENT -->
	<div id="content-inner" class="inner column center">

	<?php if ($breadcrumb): ?>
		<div id="breadcrumb">
			<?php print $breadcrumb; ?>
		</div>
	<?php endif; ?>

	<?php if ($page['highlighted']): //mission ?>
		<div id="highlighted">
			<?php print render($page['highlighted']); ?>
		</div>
	<?php endif; ?>

	<?php if ($page['content_top01']): ?>
		<div id="content-top01">
			<div class="clearfix">
				<?php print render($page['content_top01']); ?>
			</div>
		</div>
	<?php endif; ?>

	<div id="content-header">

		<?php print render($title_prefix); ?>
		<?php if ($title): ?>
			<h1 class="page-title">
				<?php print $title; ?>
			</h1>
		<?php endif; ?>
		<?php print render($title_suffix); ?>

		<?php print $messages; ?>

		<?php print render($page['help']); ?>

		<?php if ($tabs): ?>
			<div class="tabs">
				<?php print render($tabs); ?>
			</div>
		<?php endif; ?>

	</div><!-- /content-header -->

	<?php if ($page['content_top02']): ?>
		<div id="content-top02">
			<div class="clearfix">
				<?php print render($page['content_top02']); ?>
			</div>
		</div>
	<?php endif; ?>

	<!-- CONTENT AREA -->
	<div id="content-area">
		<?php if ($action_links): ?>
			<ul class="action-links"><?php print render($action_links); ?></ul>
		<?php endif; ?>
		<?php print render($page['content']); ?>
	</div>
	<!-- / CONTENT AREA -->

	<?php print $feed_icons; ?>

	<?php if ($page['content_bottom']): ?>
		<div id="content-bottom">
				<?php print render($page['content_bottom']); ?>
		</div>
	<?php endif; ?>

	</div><!--  /content-inner -->
	</div><!-- /content -->

	<?php if ($page['sidebar_first']): ?><!-- SIDEBAR FIRST -->
		<div id="sidebar-first" class="column sidebar">
			<div id="sidebar-first-inner" class="inner">
				<?php print render($page['sidebar_first']); ?>
			</div>
		</div>
	<?php endif; ?><!-- /sidebar-first -->

	<?php if ($page['sidebar_second']): ?><!-- SIDEBAR SECOND -->
		<div id="sidebar-second" class="column sidebar">
			<div id="sidebar-second-inner" class="inner">
				<?php print render($page['sidebar_second']); ?>
			</div>
		</div>
	<?php endif; ?><!-- /sidebar-second -->

</div><!-- /main -->
    
    <!-- ______________________ BOTTOM REGION _______________________ -->
	<?php if ($page['bottom01']): ?>
		<div id="bottom01">
				<?php print render($page['bottom01']); ?>
		</div>
	<?php endif; ?>

</div><!-- /main-in -->
</div><!-- /main-out -->

<!-- ______________________ FOOTER _______________________ -->

<?php if ($page['footer']): ?>
	<div id="footer">
		<div class="section clearfix">
		<?php print render($page['footer']); ?>
		</div><!-- /section -->
	</div><!-- /#footer -->
<?php endif; ?>

</div><!-- /page -->
</div><!-- /page-wrapper -->
