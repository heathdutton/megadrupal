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
 * - $logo: DISABLED.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: DISABLED.
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
 * regions[page_top] = Page top
 * regions[header] = Header
 * regions[preface_first] = Preface First
 * regions[preface] = Preface Middle
 * regions[preface_last] = Preface Last
 * regions[highlighted] = Highlighted
 * regions[help] = Help
 * regions[title_prefix] = Page title prefix
 * regions[title_suffix] = Page title suffix
 * regions[content_first] = Content First
 * regions[content] = Content
 * regions[content_last] = Content last
 * regions[secondary] = Secondary Middle
 * regions[tertiary] = Tertiary Middle
 * regions[postscript_first] = Postscript First
 * regions[postscript] = Postscript Middle
 * regions[postscript_last] = Postscript Last
 * regions[navigation] = Navigation
 * regions[footer] = Footer
 * regions[page_bottom] = Page bottom
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
$plink = Plink::singleton();
?>
<!-- START PAGE TPL //-->
	<?php print render($page['header']); ?>	
	<?php print render($page['preface_first']); ?>
	<?php print render($page['preface']); ?>
	<?php print render($page['preface_last']); ?>


	<div id="container" class="content-container clearfix <?php print $container_classes; ?>" >
		
		<div id="primary" class="clearfix <?php print $primary_classes; ?>" data-role="content" <?php print $attributes; ?>>
			
			<?php if($site_name): ?>
			 <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="site-name">
				<span><?php print $site_name; ?></span>
			</a>
			<?php endif; ?>
			
			<header id="content-first" class="clearfix">
				 			
				<?php if ($breadcrumbs_enabled && !empty($breadcrumb)): ?>
		      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
		    <?php endif; ?>
				
				<?php if ($page['highlighted']): ?>
					<div id="highlighted" class="clearfix"><?php print render($page['highlighted']); ?></div>
				<?php endif; ?>

        <?php if ($title && !isset($node)): /* node title belongs to the article */ ?>
	        <?php print render($title_prefix); ?>
					<h1 class="title" id="page-title"><?php print $title; ?></h1>
					<?php print render($title_suffix); ?>
				<?php endif; ?>
        
        <?php if ($tabs): ?>
					<nav class="tabs clear clearfix"><?php print render($tabs); ?></nav>
				<?php endif; ?>

        <?php print render($page['help']); ?>

        <?php if ($action_links): ?>
					<nav class="action-links">
						<ul><?php print render($action_links); ?></ul>
					</nav>
				<?php endif; ?>

			</header> <!-- /primary > header //-->
			
			<?php print $messages; ?>
			
			<!-- Content First Region //-->
			<?php print render($page['content_first']); ?>
			<!-- END content first region //-->
			
			<!-- START MAIN CONTENT //-->
			<a id="ank-main-content"></a>
			<?php print render($page['content']); ?>
			<!-- END MAIN CONTENT //-->
			
			<footer id="content-last" class="clearfix">
				<?php print render($page['content_last']); ?>
				<?php print $feed_icons; ?>
			</footer><!-- /primary > footer //-->
			
		</div> <!-- /primary //-->
		
			<aside id="secondary" class="<?php print $secondary_classes; ?>">
    		<?php if($page['secondary']) : ?>
    		  <?php print render($page['secondary']); ?>
  		  <?php endif; ?>	
			</aside> <!-- /secondary //-->

			<aside id="tertiary" class="<?php print $tertiary_classes; ?>">
    		<?php if($page['tertiary']) : ?>
    		  <?php print render($page['tertiary']); ?>
    		<?php endif; ?>		
			</aside><!-- /tertiary //-->

	</div><!-- /container --> 

	<?php print render($page['postscript_first']); ?>
	<?php print render($page['postscript']); ?>
	<?php print render($page['postscript_last']); ?>
	<?php print render($page['footer']); ?>
	<?php print render($page['navigation']); ?>

	
<!-- END PAGE TPL //-->
