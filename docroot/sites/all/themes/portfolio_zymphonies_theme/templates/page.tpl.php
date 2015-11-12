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
 * - $is_admin: TRUE if the user has permission to main-menu administration pages.
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


<div class="page-wrap">

  <div class="user-menu-wrapper">
    <div class="full-wrap">
      <?php print render($page['user_menu']) ?>
    </div>
  </div>

  <div class="menu-wrap">
    <div class="full-wrap clearfix">
      <nav id="main-menu" role="navigation">
        <a class="nav-toggle" href="#">Navigation</a>
        <div class="menu-navigation-container">
          <?php $main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu')); 
            print drupal_render($main_menu_tree);
          ?>
        </div>
        <div class="clear"></div>
      </nav>
    </div>
  </div>

  <header class="siteheader">

    <div class="zymphonies bounce">
      
      <?php if ($logo): ?>
        <div id="logo">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
            <img src="<?php print $logo; ?>"/>
          </a>
        </div>
      <?php endif; ?>

      <h1 id="site-title">
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
        <div id="site-description"><?php print $site_slogan; ?></div>
      </h1>

    </div>
  
  </header>

<div class="front-blocks">

  <?php if ($is_front): ?>

    <div class="toplayer"></div>

    <div class="frontblockwrap">
      <?php print render($page['aboutme']); ?>
    </div>

    <div class="frontblockwrap">
      <?php print render($page['myworks']); ?> 
    </div>

    <div class="frontblockwrap zymphonies bounceInUp">
      <?php print render($page['keyskills']); ?> 
    </div>

    <div class="frontblockwrap">
      <?php print render($page['education']); ?> 
    </div>

    <div class="frontblockwrap zymphonies bounceInDown">
      <?php print render($page['awards']); ?> 
    </div>

  <?php endif; ?>

</div>


  <div class="content-wrap">

    <div id="container">

      <div class="container-wrap">

        <div class="content-sidebar-wrap">

            <!-- First Sidebar -->

            <?php if ($page['sidebar_first']): ?>
                <aside id="sidebar-first" role="complementary">
                  <?php print render($page['sidebar_first']); ?>
                </aside>
            <?php endif; ?>

            <!-- End First Sidebar -->

            <div id="content">

              <section id="post-content" role="main">

                <?php print $messages; ?>

                <?php print render($title_prefix); ?>

                <?php if ($title): ?><h1 class="page-title"><?php print $title; ?></h1><?php endif; ?>

                <?php if (theme_get_setting('breadcrumbs', 'portfolio_zymphonies_theme')): ?>

                  <div id="breadcrumbs"> <?php if ($breadcrumb): print $breadcrumb; endif;?> </div>

                <?php endif; ?>

                <?php print render($title_suffix); ?>

                <?php if (!empty($tabs['#primary'])): ?>
                  <div class="tabs-wrapper"><?php print render($tabs); ?></div>
                <?php endif; ?>

                <?php print render($page['help']); ?>

                <?php if ($action_links): ?>
                  <ul class="action-links"><?php print render($action_links); ?></ul>
                <?php endif; ?>

                <?php print render($page['content']); ?>

              </section>

            </div> 
        
          </div>

          <!-- Second Sidebar -->

          <?php if ($page['sidebar_second']): ?>
            <aside id="sidebar-second" role="complementary">
              <?php print render($page['sidebar_second']); ?>
            </aside>
          <?php endif; ?>

          <!-- End Second Sidebar -->

      </div>

    </div>

  </div>

<div class="front-blocks">

  <?php if ($is_front): ?>

    <div class="frontblockwrap zymphonies bounceInUp">
      <?php print render($page['resume']); ?>
    </div> 

    <div class="frontblockwrap">
      <?php print render($page['contact']); ?> 
    </div>

  <?php endif; ?>

</div>

  <!-- Footer -->

  <div id="footer">
    
    <div class="footer_credit">

      <div class="social-media-wrap">
      
        <?php if (theme_get_setting('social_links', 'portfolio_zymphonies_theme')): ?>
          <span class="social-icons">
           <ul>
            <li><a class="rss" href="<?php print $front_page; ?>rss.xml"><i class="fa fa-rss"></i></a></li>
            <li><a class="fb" href="<?php echo theme_get_setting('facebook_profile_url', 'portfolio_zymphonies_theme'); ?>" target="_blank" rel="me"><i class="fa fa-facebook"></i></a></li>
            <li><a class="twitter" href="<?php echo theme_get_setting('twitter_profile_url', 'portfolio_zymphonies_theme'); ?>" target="_blank" rel="me"><i class="fa fa-twitter"></i></a></li>
            <li><a class="gplus" href="<?php echo theme_get_setting('gplus_profile_url', 'portfolio_zymphonies_theme'); ?>" target="_blank" rel="me"><i class="fa fa-google-plus"></i></a></li>
            <li><a class="linkedin" href="<?php echo theme_get_setting('linkedin_profile_url', 'portfolio_zymphonies_theme'); ?>" target="_blank" rel="me"><i class="fa fa-linkedin"></i></a></li>
            <li><a class="pinterest" href="<?php echo theme_get_setting('pinterest_profile_url', 'portfolio_zymphonies_theme'); ?>" target="_blank" rel="me"><i class="fa fa-pinterest"></i></a></li>
            <li><a class="youtube" href="<?php echo theme_get_setting('youtube_profile_url', 'portfolio_zymphonies_theme'); ?>" target="_blank" rel="me"><i class="fa fa-youtube"></i></a></li>
           </ul>
          </span>
        <?php endif; ?>
        
      </div>
        
      <div id="copyright">

        <div class="copyright">
          <?php print t('Copyright'); ?> &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?>
        </div> 

        <div class="credits">
          <?php print t('Designed by'); ?>  <a href="http://www.zymphonies.com/">Zymphonies</a>
        </div>

      </div>

    </div>

  </div>

</div>


<!-- end Footer -->