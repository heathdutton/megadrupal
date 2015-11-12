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

<div id="wrapper">

  <div class="inwrap">

    <div id="menu-container">
      <div class="logo">
        <?php if ($logo): ?>
           <div id="logoimg">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
            </div>
          <?php endif; ?>
          <div class="sitename">
            <h1><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
            <h2><?php print $site_slogan; ?></h2>
         </div>
        </div>
     <div id="main-menu" class="menu-menu-container">
        <?php 
        $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu')); 
        print drupal_render($main_menu_tree);
        ?>
      </div>
    </div>
    </div>

    <div id="header-top">
        <div class="sitename_wrapper">
         
        <?php if (theme_get_setting('social_links', 'classic_theme')): ?>
          <div class="social-icons">
           <ul>
            <li><a href="<?php print $front_page; ?>/rss.xml"><img src="<?php print base_path() . drupal_get_path('theme', 'classic_theme') . '/images/rss.png'; ?>" alt="RSS Feed"/></a></li>
            <li><a href="http://www.facebook.com/<?php echo theme_get_setting('facebook_username', 'classic_theme'); ?>" target="_blank" rel="me"><img src="<?php print base_path() . drupal_get_path('theme', 'classic_theme') . '/images/facebook.png'; ?>" alt="Facebook"/></a></li>
            <li><a href="http://www.twitter.com/<?php echo theme_get_setting('twitter_username', 'classic_theme'); ?>" target="_blank" rel="me"><img src="<?php print base_path() . drupal_get_path('theme', 'classic_theme') . '/images/twitter.png'; ?>" alt="Twitter"/></a></li>
           </ul>
          </div>
        <?php endif; ?>
      </div>
    
  <div id="content-container">

  <?php if ($is_front): ?>
  <?php if (theme_get_setting('slideshow_display', 'classic_theme')): ?>
  <div id="slideshow">
    <div id="slides">
      <div class="slides_container">
        <img src="<?php print base_path() . drupal_get_path('theme', 'classic_theme') . '/images/slide-image-1.jpg'; ?>"/>
        <img src="<?php print base_path() . drupal_get_path('theme', 'classic_theme') . '/images/slide-image-2.jpg'; ?>"/>
        <img src="<?php print base_path() . drupal_get_path('theme', 'classic_theme') . '/images/slide-image-3.jpg'; ?>"/>
      </div>
      <div class="slides_nav">
        <a href="#" class="prev"></a>
        <a href="#" class="next"></a>
      </div>
    </div>
  </div>
  <?php endif; ?>
  
    <?php if ($page['front_welcome']): ?>
      <div id="front-welcome"> <?php print render($page['front_welcome']); ?></div>
    <?php endif; ?>
  <?php endif; ?>
  
 <div id="page-container">
   <?php if ($is_front): ?>
   <?php if($page['preface_first'] || $page['preface_middle'] || $page['preface_last']) : ?>
    <div id="preface-wrapper" class="in<?php print (bool) $page['preface_first'] + (bool) $page['preface_middle'] + (bool) $page['preface_last']; ?>">
          <?php if($page['preface_first']) : ?>
          <div class="column A">
            <?php print render ($page['preface_first']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['preface_middle']) : ?>
          <div class="column B">
            <?php print render ($page['preface_middle']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['preface_last']) : ?>
          <div class="column C">
            <?php print render ($page['preface_last']); ?>
          </div>
          <?php endif; ?>
    </div>
    <div class="clear"></div>
    <?php endif; ?>
    <?php endif; ?>
    
   <?php print render($page['header']); ?>
  
  <?php if ($page['sidebar_first']): ?>
    <aside id="sidebar-first" role="complementary" class="sidebar clearfix">
      <?php print render($page['sidebar_first']); ?>
    </aside>  <!-- /#sidebar-first -->
  <?php endif; ?>

  <div id="content">
  <?php if (theme_get_setting('breadcrumbs', 'classic_theme')): ?><div class="breadcrumb"><?php if ($breadcrumb): print $breadcrumb; endif;?></div><?php endif; ?>
  <section id="main" role="main" class="post">
    <?php print $messages; ?>
    <a id="main-content"></a>
    <?php if ($page['content_top']): ?><div id="content_top"><?php print render($page['content_top']); ?></div><?php endif; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?><div class="title"><h2 class="title" id="page-title"><?php print $title; ?></h2></div><?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if (!empty($tabs['#primary'])): ?><div class="tabs-wrapper clearfix"><?php print render($tabs); ?></div><?php endif; ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    <?php print render($page['content']); ?>
  </section> <!-- /#main -->
  </div>

  <?php if ($page['sidebar_second']): ?>
    <aside id="sidebar-second" role="complementary" class="sidebar clearfix">
     <?php print render($page['sidebar_second']); ?>
    </aside>  <!-- /#sidebar-second -->
  <?php endif; ?>
 </div>
</div> 
 <?php if ($is_front): ?>
  <?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third']): ?>
    <div class="bottom">
    <div class="bottom-container in<?php print (bool) $page['footer_first'] + (bool) $page['footer_second'] + (bool) $page['footer_third']; ?>">
     <?php if ($page['footer_first']): ?><!-- / start first footer block -->
        <div class="column A">
          <?php print render($page['footer_first']); ?>
        </div> <!-- / end first footer -->
      <?php endif; ?>
     <?php if ($page['footer_second']): ?><!-- / start second footer block -->
        <div class="column B">
          <?php print render($page['footer_second']); ?>
        </div> <!-- / end second footer -->
      <?php endif; ?>
     <?php if ($page['footer_third']): ?><!-- / start third footer block -->
        <div class="column C">
          <?php print render($page['footer_third']); ?>
        </div> <!-- / end third footer -->
      <?php endif; ?>
    <div class="clear"></div> 
    </div> 
    </div> 
   <?php endif; ?>
   <?php endif; ?>
   
    <?php if($page['bottom_1'] || $page['bottom_2'] || $page['bottom_3'] || $page['bottom_4']) : ?>
    <div id="bottom-wrapper" class="in<?php print (bool) $page['bottom_1'] + (bool) $page['bottom_2'] + (bool) $page['bottom_3'] + (bool) $page['bottom_4']; ?>">
          <?php if($page['bottom_1']) : ?>
          <div class="column A">
            <?php print render ($page['bottom_1']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['bottom_2']) : ?>
          <div class="column B">
            <?php print render ($page['bottom_2']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['bottom_3']) : ?>
          <div class="column C">
            <?php print render ($page['bottom_3']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['bottom_4']) : ?>
          <div class="column D">
            <?php print render ($page['bottom_4']); ?>
          </div>
          <?php endif; ?>
      <div class="clear"></div>
    </div><!-- end bottom -->
    <?php endif; ?>

   <?php if (theme_get_setting('footer_copyright', 'classic_theme') || theme_get_setting('footer_credits', 'classic_theme')): ?>
    <div id="footer">
      <div class="footer_wrapper">
       <?php print render($page['footer']) ?>
      <?php if (theme_get_setting('footer_copyright', 'classic_theme')): ?>
        <div class="copyright"><?php print t('Copyright'); ?> &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?>.</div>
      <?php endif; ?>
      <?php if (theme_get_setting('footer_credits', 'classic_theme')): ?>
        <div class="credit"><?php print t('Designed by'); ?> <a href="http://www.zymphonies.com" target="_blank">Zymphonies</a>.</div>
      <?php endif; ?>
    </div>
  </div>
   <?php endif; ?>
  </div>
</div>