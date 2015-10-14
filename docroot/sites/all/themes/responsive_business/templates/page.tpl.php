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
  <header id="header" class="clearfix">
    <hgroup id="logo">
      <?php if ($logo): ?><div id="logoimg"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>"/></a></div><?php endif; ?>
      <div id="sitename"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></div>
    </hgroup>
    <nav id="navigation" role="navigation" class="clearfix">
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
    </nav>
  </header>

  <div id="main" class="clearfix">
    <?php if ($is_front): ?>
    <?php if (theme_get_setting('slideshow_display','responsive_business')): ?>
    <?php 
    $slide1_desc = check_markup(theme_get_setting('slide1_desc','responsive_business'), 'full_html'); 
    $slide2_desc = check_markup(theme_get_setting('slide2_desc','responsive_business'), 'full_html'); 
    $slide3_desc = check_markup(theme_get_setting('slide3_desc','responsive_business'), 'full_html'); 
    ?>
    <div id="slider-wrap">
      <div class="full-slides flexslider clearfix">
        <ul class="slides">
        <li class="slide">
          <img src="<?php print base_path() . drupal_get_path('theme', 'responsive_business') . '/images/slide-image-1.jpg'; ?>" />
          <?php if($slide1_desc) { print '<div class="caption">' . $slide1_desc . '</div>'; } ?>
        </li><!--/slide -->
        <li class="slide">
          <img src="<?php print base_path() . drupal_get_path('theme', 'responsive_business') . '/images/slide-image-2.jpg'; ?>" />
          <?php if($slide1_desc) { print '<div class="caption">' . $slide2_desc . '</div>'; } ?>
        </li><!--/slide -->
        <li class="slide">
          <img src="<?php print base_path() . drupal_get_path('theme', 'responsive_business') . '/images/slide-image-3.jpg'; ?>" />
          <?php if($slide1_desc) { print '<div class="caption">' . $slide3_desc . '</div>'; } ?>
        </li><!--/slide -->
        </ul>
      </div>
    </div>
    <?php endif; ?>
    <?php endif; ?>

    <?php if($page['preface_first'] || $page['preface_middle'] || $page['preface_last']) : ?>
    <div id="preface-wrap" class="clearfix">
      <div class="preface-block">
        <?php print render ($page['preface_first']); ?>
      </div>
      <div class="preface-block">
        <?php print render ($page['preface_middle']); ?>
      </div>
      <div class="preface-block remove-margin">
        <?php print render ($page['preface_last']); ?>
      </div>
      <div class="clear"></div>
    </div>
    <?php endif; ?>

    <?php print render($page['header']); ?>
    

    <section id="content" role="main" class="clearfix">
      <?php if (theme_get_setting('breadcrumbs')): ?><?php if ($breadcrumb): ?><div id="breadcrumbs"><?php print $breadcrumb; ?></div><?php endif;?><?php endif; ?>
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

    <?php if ($page['sidebar_first']): ?>
      <aside id="sidebar" role="complementary" class="clearfix">
       <?php print render($page['sidebar_first']); ?>
      </aside> 
    <?php endif; ?>

    <div class="clear"></div>
    
    <?php print render($page['footer']); ?>

  </div>

  <div id="footer-wrap" class="clearfix">
    <div id="footer">
    <?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third'] || $page['footer_fourth']): ?>
        <div id="footer-block-wrap" class="clearfix">
          <div id="footer-one">
            <?php print render ($page['footer_first']); ?>
          </div>
          <div id="footer-two">
            <?php print render ($page['footer_second']); ?>
          </div>
          <div id="footer-three">
            <?php print render ($page['footer_third']); ?>
          </div>
          <div id="footer-four">
            <?php print render ($page['footer_fourth']); ?>
          </div>
          <div class="clear"></div>
      </div>

      <?php endif; ?>

      <div id="footer-bottom" class="clearfix">
        <div id="copyright" class="clearfix">
          <?php print t('Copyright'); ?> &copy; <?php echo date("Y"); ?>, <a href="<?php print $front_page; ?>"><?php print $site_name; ?></a>  //  
          <?php print t('Theme by'); ?>  <a href="http://www.devsaran.com" target="_blank">Devsaran</a>
        </div>
        <div id="back-to-top" class="clearfix">
          <a href="#toplink">back up ↑</a>
        </div>
      </div>
    </div>
  </div>
</div>