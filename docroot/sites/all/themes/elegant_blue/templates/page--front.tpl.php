<?php
/**
 * @file
 * Elegnat Blue theme implementation to display a single Drupal page.
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
  <div id="header" class="clearfix">
    <div>
      <?php if ($logo): ?>
       <div id="logo">
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
        </div>
      <?php endif; ?>
      <div id="sitename">
        <p><?php if ($site_slogan): ?><?php print $site_slogan; ?><?php endif; ?></p><!--site slogan-->
      </div>
    </div>
    <div class="searchblock">
      <?php print render($searchblock); ?>
    </div>
  </div>
  <div id="Menu-shadow">
     <div id="navigation" class="clearfix">
      <div id="main-menu">
      <div class="home <?php if ($is_front): print "act"; endif; ?>"><a href="<?php print $front_page; ?>" ><span></span></a></div>
        <?php print drupal_render($main_menu_tree); ?>
      </div>
      <?php if ($display): ?>
      <div id="socialbar">
        <ul class="social">
          <span class="follow"><?php print t('Follow us :'); ?> </span>
          <?php $options['attributes'] = array('target' => '_blank'); ?>
          <?php if ($facebook): ?><li class="fb"><?php print l(t(''), $facebook, $options); ?></li> <?php endif; ?>
          <?php if ($twitter): ?><li class="tw"><?php print l(t(''), $twitter, $options); ?></li> <?php endif; ?>
          <?php if ($linkedin): ?><li class="ln"><?php print l(t(''), $linkedin, $options); ?></li> <?php endif; ?>
        </ul>
      </div>
      <?php endif; ?>
    </div>
    </div>
    <!-- end main-menu -->
  <?php print render($page['header']); ?>
  <!-- Slider Starts -->
    <?php if ($slideshow_display): ?>
    
    <div id="slider">
        <div id ="slide" class="main_view">
            <div class="slide-area">
                <div class="slide_image">
                  <?php print $slideimage1; ?>
                  <?php print $slideimage2; ?>
                  <?php print $slideimage3; ?>
                </div>
                <div class="descriptions">
                    <div class="desc" style="display: none;"><?php print $slide1; ?></div>
                    <div class="desc" style="display: none;"><?php print $slide2; ?></div>
                    <div class="desc" style="display: none;"><?php print $slide3; ?></div>
                </div>
            </div>
            <div class="paging">
                <a rel="1" href="#"></a>
                <a rel="2" href="#"></a>
                <a rel="3" href="#"></a>
            </div>
        </div>
      </div>
  <?php endif; ?>
  <!-- Slider Ends -->
  <?php print $messages; ?>
  <div class="clear"></div>
  <!--Welcome Blocks Starts -->
    <?php if ($wtitle || $wtext):?>
      <div class="welcome_content">
        <?php if ($wtitle):?>
          <div class="welcometitle"><h2><?php print ($wtitle); ?></h2></div>
        <?php endif; ?>
        <?php if ($wtext):?>
        <div class="welcometext"><?php print ($wtext); ?></div>
        <?php endif; ?>
      </div>
      <?php endif; ?>
  <!--Welcome Blocks Ends -->
  <!--Three Column Blocks Starts -->
  <?php if (!empty($col1title)  || !empty($col1)): $num1 = 1;  endif; ?>
  <?php if (!empty($col2title)  || !empty($col2)): $num2 = 1;  endif; ?>
  <?php if (!empty($col3title) || !empty($col3)): $num3 = 1;  endif; ?>
  <?php
    $sum = (isset($num1) . isset($num2) . isset($num3));
    $result = strlen($sum);
      if ($result == 0):$value = "zero";endif;
      if ($result == 1):$value = "one";endif;
      if ($result == 2):$value = "two";endif;
      if ($result == 3):$value = "three";endif;
  ?>
  <div id="column-wrapper" class="clearfix <?php print $value; ?>">
  <?php if($col1): ?> <div class="column"><h2><?php print ($col1title); ?></h2><?php print ($col1); ?></div> <?php endif; ?>
  <?php if($col2): ?> <div class="column"><h2><?php print ($col2title); ?></h2><?php print ($col2); ?></div> <?php endif; ?>
  <?php if($col3): ?> <div class="column lastcolumn"><h2><?php print ($col3title); ?></h2><?php print ($col3); ?></div> <?php endif; ?>
  </div>
  <!--Three Column Blocks Ends -->
  <!--Bottom Blocks Start -->
  <?php if ($page['bottom_first'] || $page['bottom_second']): ?>
  <div id="footer-wrap">
    <?php if ($page['bottom_first']): ?>
      <div class="footer-box1"><?php print render($page['bottom_first']); ?></div>
    <?php endif; ?>
    <?php if ($page['bottom_second']): ?>
        <div class="footer-box2"><?php print render($page['bottom_second']); ?></div>
    <?php endif; ?>
  </div>
  <div class="clear"></div>
  <?php endif; ?>
  <!--Bottom Blocks Ends -->
  <!--Footer Start -->
  <?php print render($page['footer']) ?>
  <?php if ($footer_copyright || $footer_developed): ?>
  <div class="clear"></div>
  <div id="copyright">
    <?php if ($footer_copyright): ?>
      <?php print t('Copyright'); ?> &copy; <?php print date("Y"); ?>, <?php print $site_name; ?>.
    <?php endif; ?>
    <?php if ($footer_developed): ?>
      <span class="developed"><?php print t('Designed & Develop by'); ?>
      <a href="<?php print ($footer_developedby_url); ?> " target="_blank">
      <?php print ($footer_developedby); ?></a> </span>
    <?php endif; ?>
  </div>
  <?php endif; ?>
  </div>
  <!--Footer Ends -->
