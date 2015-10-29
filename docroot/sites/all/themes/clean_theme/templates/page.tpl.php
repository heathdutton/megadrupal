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

<div id="wrap" class="clearfix">
  <header id="header" class="clearfix" role="banner">
    <div id="header-top" class="clearfix">
      <hgroup id="logo">
        <?php if ($logo): ?>
         <div id="logoimg">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
         </div>
        <?php endif; ?>
        <div id="sitename">
          <h1><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
        </div>
      </hgroup>
      <?php if ($page['search_box']): ?><!-- / start search box region -->
        <div class="search-block">
          <?php print render($page['search_box']); ?>
        </div> <!-- / end search box region -->
       <?php endif; ?>
	  </div>
    <?php if($site_slogan) { print '<div id="site-slogan">' . $site_slogan . '</div>'; } ?>
  </header>
  <nav id="navigation" class="clearfix" role="navigation">
    <div id="menu-menu-container">
      <div id="main-menu">
        <?php 
          if (module_exists('i18n')) {
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
      </div>
    </div>
  </nav>
  
    <?php if($page['preface_first'] || $page['preface_middle'] || $page['preface_last']) : ?>
    <div id="preface-wrapper" class="in<?php print (bool) $page['preface_first'] + (bool) $page['preface_middle'] + (bool) $page['preface_last']; ?> clearfix">
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
  
    <?php print render($page['header']); ?>
  
  
  <div id="main" class="clearfix">
  
    <?php if ($is_front): ?>
    <?php if (theme_get_setting('slideshow_display','clean_theme')): ?>
      <?php 
      $slide1_head = check_plain(theme_get_setting('slide1_head','clean_theme')); $slide1_desc = check_markup(theme_get_setting('slide1_desc','clean_theme'), 'full_html'); 
      $slide2_head = check_plain(theme_get_setting('slide2_head','clean_theme')); $slide2_desc = check_markup(theme_get_setting('slide2_desc','clean_theme'), 'full_html'); 
      $slide3_head = check_plain(theme_get_setting('slide3_head','clean_theme')); $slide3_desc = check_markup(theme_get_setting('slide3_desc','clean_theme'), 'full_html'); 
      $slide1_cap = ($slide1_head) || ($slide1_desc);
      $slide2_cap = ($slide2_head) || ($slide2_desc);
      $slide3_cap = ($slide3_head) || ($slide3_desc);
      ?>
   <div id="slider-wrap">
    <div id="home-slides" class="slides clearfix">
      <div class="slides_container">
        <div class="slide">
           <img src="<?php print base_path() . drupal_get_path('theme', 'clean_theme') . '/images/slide-image-1.jpg'; ?>" height="350" width="660"/>
           <?php if ($slide1_cap): ?><div class="caption">
             <?php if ($slide1_head) { print '<h3>' . $slide1_head . '</h3>';  }?>
             <?php if ($slide1_desc) { print $slide1_desc;  }?>
           </div><?php endif; ?>
        </div>
        <div class="slide">
           <img src="<?php print base_path() . drupal_get_path('theme', 'clean_theme') . '/images/slide-image-2.jpg'; ?>" height="350" width="660"/>
           <?php if ($slide2_cap): ?><div class="caption">
             <?php if ($slide2_head) { print '<h3>' . $slide2_head . '</h3>';  }?>
             <?php if ($slide2_desc) { print $slide2_desc;  }?>
           </div><?php endif; ?>
        </div>
        <div class="slide">
           <img src="<?php print base_path() . drupal_get_path('theme', 'clean_theme') . '/images/slide-image-3.jpg'; ?>" height="350" width="660"/>
           <?php if ($slide3_cap): ?><div class="caption">
             <?php if ($slide3_head) { print '<h3>' . $slide3_head . '</h3>';  }?>
             <?php if ($slide3_desc) { print $slide3_desc;  }?>
           </div><?php endif; ?>
        </div>
      </div>
    </div>
   </div>
    <?php endif; ?>
    <?php endif; ?>
    
    <?php print render($page['secondary_content']); ?>
    
    <section id="post-content" role="main">
      <?php if (theme_get_setting('breadcrumbs')): ?><div id="breadcrumbs"><?php if ($breadcrumb): print $breadcrumb; endif;?></div><?php endif; ?>
      <?php print $messages; ?>
      <?php if ($page['content_top']): ?><div id="content_top"><?php print render($page['content_top']); ?></div><?php endif; ?>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?><h1 class="page-title"><?php print $title; ?></h1><?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if (!empty($tabs['#primary'])): ?><div class="tabs-wrapper clearfix"><?php print render($tabs); ?></div><?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <?php print render($page['content']); ?>
    </section> <!-- /#main -->
    
  </div>

    <?php if ($page['sidebar_first']): ?>
      <aside id="sidebar" role="complementary" class="clearfix">
       <?php print render($page['sidebar_first']); ?>
      </aside> 
    <?php endif; ?>

    <div class="clear"></div>
    
    <?php print render($page['footer']); ?>

    <?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third']): ?>
    <div id="footer-wrap">
      <div id="footer">
        <div id="footer-widget-wrap" class="clearfix">
          <?php if ($page['footer_first']): ?>
          <div id="footer-left"><?php print render($page['footer_first']); ?></div>
          <?php endif; ?>
          <?php if ($page['footer_second']): ?>
          <div id="footer-middle"><?php print render($page['footer_second']); ?></div>
          <?php endif; ?>
          <?php if ($page['footer_third']): ?>
          <div id="footer-right"><?php print render($page['footer_third']); ?></div>
          <?php endif; ?>
        </div>
      </div>
    </div>
    <?php endif; ?>
  
  <div id="copyright" class="clearfix">
    <div class="one-half">
        <?php print t('Copyright'); ?> &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?>
    </div>
    <div class="one-half column-last" id="credit">
       <?php print t('Theme by'); ?>  <a title="Free Premium Drupal Themes" href="http://www.devsaran.com" target="_blank">Devsaran</a>
    </div>
  </div>
</div>
