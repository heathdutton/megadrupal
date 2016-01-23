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
  <div id="header">
    <div class="header-left">
      <div class="logo">
        <div class="site-title">
        <span>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
          <?php print $site_name; ?></a>
        </span>
        </div>
        <div class="site-desc"><?php if ($site_slogan): ?><?php print $site_slogan; ?><?php endif; ?></div><!--site slogan-->
      </div><!--end logo-->
      
      <nav id="access" role="navigation">
      <div class="menu">
      <?php 
      $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu')); 
      print drupal_render($main_menu_tree);
      ?>
      </div>
      </nav><!-- end main-menu -->
    </div>
    <div class="header-right">
    <?php if ($page['search_box']): ?><!-- / start search box region -->
    <div class="search">
      <?php print render($page['search_box']); ?><br/>
    </div> <!-- / end search box region -->
    <?php endif; ?>
    
   <?php if (theme_get_setting('social_icons')): ?>
   <div class="syndicate">
    <ul>
    <li><a class="s1" title="RSS" href="<?php print $front_page; ?>rss.xml"><em>RSS Feed</em></a></li>
    <li><a class="s2" title="Twitter" href="http://www.twitter.com/<?php echo theme_get_setting('twitter_username'); ?>" target="_blank" rel="me"><em>Twitter</em></a></li>
    <li><a class="s3" title="FaceBook" href="http://www.facebook.com/<?php echo theme_get_setting('facebook_username'); ?>" target="_blank" rel="me"><em>FaceBook</em></a></li>
    </ul><!--end header-social-->
   </div><?php endif; ?>
   </div>
   <div class="header-image" role="banner">
    <img alt="" src="<?php global $base_url; echo $base_url.'/'.$directory.'/'; ?>images/headers/geometric-<?php echo rand(1, 5); ?>.jpg">
   </div>

  </div><!--end header-top-->


  <div id="container">
  <div class="SC" role="main">
    <?php if ($page['header']): ?>
    <div class="container-head">
      <?php print render($page['header']); ?>
     </div>
    <?php endif; ?>
  <?php if ($breadcrumb): print $breadcrumb; endif;?>
  <div class="SL">
   <?php if ($page['highlighted']): ?><div class="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
   <div class="post">
    <?php print $messages; ?>
    <a id="main-content"></a>
    
    <?php print render($title_prefix); ?>
    <?php if ($title): ?><h1 class="post-head title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if (!empty($tabs['#primary'])): ?><div class="tabs-wrapper clearfix"><?php print render($tabs); ?></div><?php endif; ?>
    <?php print render($page['help']); ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    <?php print render($page['content']); ?>
   </div>
  </div> 
  <?php if ($page['sidebar_first']): ?>
    <div class="SR">
    <div class="widget-area">
      <?php print render($page['sidebar_first']); ?>
    </div>
    </div>
  <?php endif; ?>

  </div><!-- /#main -->
   <div class="container-bot"></div>
  </div>
  
  
  <div id="footer" role="contentinfo">

      <?php if ($page['footer_first']): ?><!-- / start first footer block -->
        <div class="first-footer">
          <?php print render($page['footer_first']); ?>
        </div> <!-- / end first footer -->
      <?php endif; ?>
     <?php if ($page['footer_second']): ?><!-- / start second footer block -->
        <div class="second-footer">
          <?php print render($page['footer_second']); ?>
        </div> <!-- / end second footer -->
      <?php endif; ?>
     <?php if ($page['footer_third']): ?><!-- / start third footer block -->
        <div class="third-footer">
          <?php print render($page['footer_third']); ?>
        </div> <!-- / end third footer -->
      <?php endif; ?>
    <div class="bottom-footer">
     <?php print render($page['footer']) ?>
    </div>


    <?php if (theme_get_setting('footer_mega_menu')): ?>
    <div id="footer-widget-area" role="complementary">
        <div id="first" class="widget-area">
          <?php echo theme_get_setting('footer_mega_menu_1'); ?>
        </div> <!-- / end first footer -->
        <div id="second" class="widget-area">
          <?php echo theme_get_setting('footer_mega_menu_2'); ?>
        </div> <!-- / end second footer -->
        <div id="third" class="widget-area">
          <?php echo theme_get_setting('footer_mega_menu_3'); ?>
        </div> <!-- / end third footer -->
        <div id="fourth" class="widget-area">
          <?php echo theme_get_setting('footer_mega_menu_4'); ?>
        </div> <!-- / end fourth footer -->
        <div id="fifth" class="widget-area">
          <?php echo theme_get_setting('footer_mega_menu_5'); ?>
        </div> <!-- / end fifth footer -->
    </div>
    <div id="footer-widget-area-bot"></div>
    <?php endif; ?>

    <div class="site-info">
    <?php if (theme_get_setting('footer_copyright')): ?>
    Copyright &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?>.
    <?php endif; ?>
    </div>
  </div> <!-- /#footer -->
</div> <!-- /#wrapper -->