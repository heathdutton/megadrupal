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
 *   or themes/garland.
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
 */
?>

  <div id="wrapper">
    <?php if ($site_name && $is_front): ?>
      <h1 id="site-name"><a href="<?php print $front_page; ?>" title="<?php print check_plain($site_name); ?>" rel="home">
				<?php print check_plain($site_name); ?>
      </a></h1>
    <?php else: ?>
      <div id="site-name">
        <a href="<?php print $front_page; ?>" title="<?php print check_plain($site_name); ?>" rel="home">
					<?php print check_plain($site_name); ?>
        </a>
      </div>
    <?php endif; ?>
    
    <div id="content">
      <div class="content-wrapper">
        <div class="content-box">
          <?php print $messages; ?>
        <?php if ($page['highlight']): ?><div id="highlight"><?php print render($page['highlight']); ?></div><?php endif; ?>
        <a id="main-content" name="skip-link"></a>
        
        <?php print render($title_prefix); ?>
        <?php if ($title && !$is_front): ?>
          <h1 class="title" id="page-title"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        
        <?php if ($tabs): print render($tabs); endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?>
										<ul class="action-links">
												<?php print render($action_links); ?>
										</ul>
								<?php endif; ?>
        
        <?php if ($page['content_top']): ?>
          <div id="content-top">
            <?php print render($page['content_top']); ?>
          </div> <!-- content-top -->
        <?php endif; ?>
        
        <?php print render($page['content']); ?>
        
        <?php if ($page['content_bottom']): ?>
          <div id="content-bottom">
            <?php print render($page['content_bottom']); ?>
          </div> <!-- content-bottom -->
        <?php endif; ?>
        </div><!-- content-box -->
      </div><!-- content-wrapper -->
    </div><!-- content -->
    
    <div id="top-bar">
						<?php if ($main_menu): ?>
								<?php print theme('links__system_main_menu', array(
								  'links' => $main_menu,
								  'attributes' => array(
								    'id' => 'main-menu',
								    'class' => array('links', 'main-menu-links'),
								  ),
								  'heading' => array(
								      'text' => t('Main menu'),
								      'level' => 'h2',
								      'class' => array('element-invisible'),
						    ),
								)); ?>
						<?php endif; ?>
      <?php if ($breadcrumb): print $breadcrumb; endif; ?>
      <?php print render($page['banner']); ?>
    </div> <!-- top-bar -->
    
    <div id="bottom-bar">
      <?php if ($page['footer_primary'] || $logo): ?>
        <div id="footer-primary">
          <?php print render($page['footer_primary'])?>
          <div id="logo">
            <a href="<?php print $front_page; ?>" title="<?php print check_plain($site_name); ?>" rel="home">
              <img src="<?php print $logo; ?>" alt="<?php print check_plain($site_name); ?>" />
            </a>
          </div> <!-- logo -->
        </div> <!-- footer-primary -->
      <?php endif; ?>
     
    <div id="footer-secondary">
			<?php if ($page['footer_secondary']): ?>
				<?php print render($page['footer_secondary']); ?>
      <?php endif; ?>
      <?php print $feed_icons; ?>
    </div> <!-- footer-secondary -->
      
      <div id="footer-bottom">
        <?php if ($page['footer']): ?>
          <?php print render($page['footer']); ?>
        <?php endif; ?>
        <?php if ($secondary_menu): ?> 
          <div id="footer-links">
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
          </div> <!-- footer-links -->
        <?php endif; ?>
      </div> <!-- footer-bottom-->
    </div> <!-- bottom-bar -->
    
    <div id="bg-image">
      <div>
        <table cellspacing="0" cellpadding="0">
          <tr>
            <td>
              <?php if($is_front): ?>
                <img id="main-image" src="<?php print $base_path . $directory; ?>/images/home-bg.jpg" alt="<?php print $site_name; ?>" />
              <?php else: ?>
                <img id="content-image" src="<?php print $base_path . $directory; ?>/images/content-bg.jpg" alt="<?php print $site_name; ?>" />
              <?php endif; ?>
            </td>
          </tr>
        </table>
      </div>
    </div>
    
  </div> <!-- wrapper -->
