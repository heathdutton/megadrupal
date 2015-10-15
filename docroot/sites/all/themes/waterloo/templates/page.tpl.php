<?php

/**
 * @file
 * Waterloo's theme implementation to display a single Drupal page.
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
 * - $page['header']: Items for the header region, next to the logo or site 
     title
 * - $page['featured']: Items for the dark grey featured area
 * - $page['content']: The main content of the page.
 * - $page['sidebar_first']: Items for the first sidebar, located on the right 
     side on larger screens
 * - $page['upper_footer']: Items for the upper footer region.
 * - $page['lower_footer']: Items for the lower footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>

<div id="header">
  <div id="logo-sitename">
    <?php if ($logo): ?>
      <a id="logo" href="<?php print $front_page;?>">
         <img src="<?php print $logo; ?>" alt="<?php print $site_name;?>" />
      </a>
    <?php endif; ?>
	
    <?php if ($site_name || $site_slogan): ?>
      <div id="name-and-slogan">
        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div id="site-name"><strong>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </strong></div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>
	
        <?php if ($site_slogan): ?>
          <div id="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div> <!-- /#name-and-slogan -->
    <?php endif; ?>
  </div>

  <?php if ($page['header']): ?>    
    <?php print render($page['header']); ?>
  <?php endif; ?>

</div>

<?php if ($main_menu): ?>
  <div id="main-menu-region"><div id="menu"><?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu'))); ?></div></div>
<?php endif; ?>

<?php if ($page['featured']): ?>    
  <div id="featured"><?php print render($page['featured']); ?></div>
<?php endif; ?>

<div id="content-wrap">
  <div id="content">
  <?php print render($messages); ?>
    <?php if ($breadcrumb): ?><div id="breadcrumb"><?php print $breadcrumb; ?></div><?php endif; ?>
	
    <?php print render($page['help']); ?>
    <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
	
    <?php print render($title_prefix); ?>
      <?php if ($title): ?><h1><?php print $title; ?></h1><?php endif; ?>
    <?php print render($title_suffix); ?>
	
    <?php print render($page['content']); ?>
    <?php print $feed_icons; ?>
  </div>

  <?php if ($page['sidebar_first']): ?>    
    <?php print render($page['sidebar_first']); ?>
  <?php endif; ?>

</div>

<?php if ($page['upper_footer']): ?>    
  <div id="upper_footer"><?php print render($page['upper_footer']); ?></div>
<?php endif; ?>

<?php if ($page['lower_footer']): ?>    
  <?php print render($page['lower_footer']); ?>
<?php endif; ?>
