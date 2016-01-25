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
<header id="header">
  <div class="top-bg"></div>
  <div class="layout-978">
    <hgroup id="logo-wrap">
      <h1 id="site-name">
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
           <?php if ($logo): ?><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /><?php endif; ?>
          <span><?php print $site_name; ?></span>
        </a>
      </h1>
       <?php if ($site_slogan): ?><h2 id="site-slogan"><?php print $site_slogan; ?></h2><?php endif; ?>
    </hgroup>
    <?php if (theme_get_setting('socialicon_display', 'creative')): ?>
      <?php 
      $twitter_url = check_plain(theme_get_setting('twitter_url', 'creative')); 
      $facebook_url = check_plain(theme_get_setting('facebook_url', 'creative')); 
      $google_plus_url = check_plain(theme_get_setting('google_plus_url', 'creative')); 
      $pinterest_url = check_plain(theme_get_setting('pinterest_url', 'creative'));
      ?>
    <div class="social-icon">
      <ul class="social-profile">
        <?php if ($facebook_url): ?><li class="facebook">
          <a target="_blank" title="<?php print $site_name; ?> in Facebook" href="<?php print $facebook_url; ?>"><?php print $site_name; ?> Facebook </a>
        </li><?php endif; ?>
        <?php if ($twitter_url): ?><li class="twitter">
          <a target="_blank" title="<?php print $site_name; ?> in Twitter" href="<?php print $twitter_url; ?>"><?php print $site_name; ?> Twitter </a>
        </li><?php endif; ?>
        <?php if ($google_plus_url): ?><li class="google-plus">
          <a target="_blank" title="<?php print $site_name; ?> in Google+" href="<?php print $google_plus_url; ?>"><?php print $site_name; ?> Google+ </a>
        </li><?php endif; ?>
        <?php if ($pinterest_url): ?><li class="pinterest">
          <a target="_blank" title="<?php print $site_name; ?> in Pinterest" href="<?php print $pinterest_url; ?>"><?php print $site_name; ?> Twitter </a>
        </li><?php endif; ?>
        <li class="rss">
          <a target="_blank" title="<?php print $site_name; ?> in RSS" href="<?php print $front_page; ?>rss.xml"><?php print $site_name; ?> RSS </a>
        </li>
      </ul>
    </div>
    <?php endif; ?>
    <div class="row-end"></div>
    <nav id="navigation" role="navigation">
      <div id="main-menu">
        <?php 
          if (module_exists('i18n_menu')) {
            $main_menu_tree = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
          } else {
            $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
          }
          print drupal_render($main_menu_tree);
        ?>
      </div>
    </nav>
    <div class="row-end"></div>
    <?php if ($is_front): ?>
    <?php if (theme_get_setting('slideshow_display','creative')): ?>
    <?php 
    $slide1_desc = check_markup(theme_get_setting('slide1_desc', 'creative'), 'full_html'); 
    $slide2_desc = check_markup(theme_get_setting('slide2_desc', 'creative'), 'full_html'); 
    $slide3_desc = check_markup(theme_get_setting('slide3_desc', 'creative'), 'full_html'); 
    ?>
    <div class="featured-slider">
      <div class="slides displayblock">
        <div class="featured">
          <div class="slide-image"><span class="img-effect pngfix"></span><img width="976" height="313" src="<?php print base_path() . drupal_get_path('theme', 'creative') . '/images/slide-image-1.jpg'; ?>" class="pngfix"/>
          </div> <!-- .slide-image -->
        </div> <!-- .featured -->
        <?php if($slide1_desc) { print '<div class="featured-text">' . $slide1_desc . '</div>'; } ?><!-- .featured-text -->
      </div> <!-- .slides -->

      <div class="slides displaynone">
        <div class="featured">
          <div class="slide-image"><span class="img-effect pngfix"></span><img width="976" height="313" src="<?php print base_path() . drupal_get_path('theme', 'creative') . '/images/slide-image-2.jpg'; ?>" class="pngfix"/>
          </div> <!-- .slide-image -->
        </div> <!-- .featured -->
        <?php if($slide1_desc) { print '<div class="featured-text">' . $slide2_desc . '</div>'; } ?><!-- .featured-text -->
      </div> <!-- .slides -->

      <div class="slides displaynone">
        <div class="featured">
          <div class="slide-image"><span class="img-effect pngfix"></span><img width="976" height="313" src="<?php print base_path() . drupal_get_path('theme', 'creative') . '/images/slide-image-3.jpg'; ?>" class="pngfix"/>
          </div> <!-- .slide-image -->
        </div> <!-- .featured -->
        <?php if($slide1_desc) { print '<div class="featured-text">' . $slide3_desc . '</div>'; } ?><!-- .featured-text -->
      </div> <!-- .slides -->

    </div>
    <div id="controllers"></div><!-- #controllers --> 
    <?php endif; ?>
    <?php endif; ?>
  </div>
</header>

<div id="preface-head" class="layout-978">
  <?php if($page['preface_first'] || $page['preface_middle'] || $page['preface_last']) : ?>
  <div id="preface-wrap">
    <div class="preface-block col4">
      <?php print render ($page['preface_first']); ?>
    </div>
    <div class="preface-block col4">
      <?php print render ($page['preface_middle']); ?>
    </div>
    <div class="preface-block col4">
      <?php print render ($page['preface_last']); ?>
    </div>
    <div class="row-end"></div>
  </div>
  <?php endif; ?>

  <?php print render($page['header']); ?>
  <div class="row-end"></div>
</div>

<div id="main" class="layout-978">
  <?php if($page['sidebar_first']) { $col = "col8"; } else { $col= "col12"; } ?>
  <section id="content" role="main" class="<?php print $col; ?> no-margin-left">
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
    <aside id="sidebar" role="complementary" class="col4">
     <?php print render($page['sidebar_first']); ?>
    </aside> 
  <?php endif; ?>
  <div class="row-end"></div>
</div>

<div id="footer-bottom" class="layout-978">
  <?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third']): ?>
    <div id="footer-one" class="col4">
      <?php print render ($page['footer_first']); ?>
    </div>
    <div id="footer-two" class="col4">
      <?php print render ($page['footer_second']); ?>
    </div>
    <div id="footer-three" class="col4">
      <?php print render ($page['footer_third']); ?>
    </div>
    <div class="row-end"></div>
  <?php endif; ?>
  
  <?php print render($page['footer']); ?>

</div>

<div id="footer">
  <div class="layout-978">
    <div class="col7 copyright no-margin-left">
      <?php print t('Copyright'); ?> &copy; <?php echo date("Y"); ?>, <a href="<?php print $front_page; ?>"><?php print $site_name; ?></a>
    </div>
    <div class="col5 powered-by">
      <?php print t('Theme by'); ?>  <a href="http://www.devsaran.com" target="_blank">Devsaran</a>
    </div>
  </div>
</div>