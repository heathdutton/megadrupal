<?php
// $Id$

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
 * - $page['highlighted']: Items for the highlighted content region.
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
<div id="page-wrapper">
  <div id="page">

    <header role="banner">
      <div id="header-wrapper" class="header">
        <div id="header-inner" class="inner clearfix">
          <section>
  
            <?php if ($logo || $page['secondary_navigation']): ?>
            <header class="clearfix">
  
              <?php if ($logo): ?>
              <div id="logo">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
                  <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
                </a>
              </div><!-- /logo -->
              <?php endif; ?>

              <?php if ($page['secondary_navigation']): ?>
              <div id="secondary-menu-wrapper" class="clearfix">
                <?php print render($page['secondary_navigation']); ?>
              </div>
              <?php endif; ?>

            </header>
            <?php endif; ?>
  
          </section><!-- /section -->
        </div><!-- /header -->
      </div><!-- /header-wrapper -->
    </header><!-- /header[banner] -->

    <?php if ($page['primary_navigation'] || $page['search']): ?>
    <div id="nav-wrapper">
      <div id="nav-inner" class="inner clearfix">
        <?php if ($main_menu): ?>
        <nav role="navigation">
          <section>
            <header>
              <div id="primary-menu" class="clearfix">
                <?php print render($page['primary_navigation']); ?>
              </div><!-- /primary-menu -->
            </header><!-- /header --> 
          </section><!-- /section -->
        </nav><!-- /navigation -->
        <?php endif; ?>

        <?php print render($page['search']); ?>

      </div><!-- /nav-inner -->
    </div><!-- /nav-wrapper -->
    <?php endif; ?>
    
    <?php if ($breadcrumb): ?>
    <div id="breadcrumb-wrapper" class="breadcrumb">
      <div id="breadcrumb-inner" class="inner">
        <?php print $breadcrumb; ?>
      </div>
    </div>
    <?php endif; ?>

    <?php if ($page['preface_first'] || $page['preface_second'] || $page['preface_third']): ?>
    <div id="preface-wrapper" role="complementary" class="preface <?php print " " . $prefaces; ?>">
      <div id="preface-inner" class="inner clearfix">
        <section>

          <?php if ($page['preface_first']): ?>
          <div id="preface-first" class="column">
            <?php print render($page['preface_first']); ?>
          </div><!-- /preface-first -->
          <?php endif; ?>
          
          <?php if ($page['preface_second']): ?>
          <div id="preface-second" class="column">
            <?php print render($page['preface_second']); ?>
          </div><!-- /preface-second -->
          <?php endif; ?>
          
          <?php if ($page['preface_third']): ?>
          <div id="preface-last" class="column">
            <?php print render($page['preface_third']); ?>
          </div><!-- /preface-last -->
          <?php endif; ?>

        </section><!-- /section -->
      </div><!-- /preface-inner -->
    </div><!-- /preface-wrapper -->
    <?php endif; ?>

    <div id="main-wrapper">
      <div id="main-inner" class="inner clearfix">
        <?php if ($page['sidebar_first']): ?>
        <aside id="sidebar-first" class="column sidebar" role="complementary">
          <section>
            <div id="sidebar-first-inner" class="sidebar-inner">
              <?php print render($page['sidebar_first']); ?>
            </div><!-- /section -->
          </section><!-- /section -->
        </aside><!-- /sidebar-first -->
        <?php endif; ?>
          <div id="main" class="clearfix">
          <div id="content" class="column" role="main">
            <section>
              <?php if ($page['highlighted']): ?>
              <div id="highlighted"><?php print render($page['highlighted']); ?></div>
              <?php endif; ?>
              <?php print $messages; ?>
              <?php print render($title_prefix); ?>
              <?php if ($title): ?>
              <header>
                <h1 class="title" id="page-title"><?php print t($title); ?></h1>
              </header>
              <?php endif; ?>
              <?php if ($page['page_node_top']): ?>
              <section>
                <div id="page-node-top" class="page-node-top">
                  <?php print render($page['page_node_top']); ?>
                </div><!-- /page-node-top -->
              </section><!-- /section -->
              <?php endif; ?>
              <?php print render($title_suffix); ?>
              <?php if (render($tabs)): ?>
              <div class="tabs" role="tab"><?php print render($tabs); ?></div>
              <?php endif; ?>
              <?php print render($page['help']); ?>
              <?php if (render($action_links)): ?>
                <ul class="action-links"><?php print render($action_links); ?></ul>
              <?php endif; ?>
              <?php print render($page['content']); ?>
              <?php if ($page['page_node_bottom']): ?>
              <section>
                <div id="page-node-bottom" class="page-node-bottom">
                  <?php print render($page['page_node_bottom']); ?>
                </div><!-- /page-node-top -->
              </section><!-- /section -->
              <?php endif; ?>
              <?php print $feed_icons; ?>
            </section><!-- /section -->
          </div><!-- /content -->
        </div><!-- /main -->
        <?php if ($page['sidebar_second']): ?>
        <aside id="sidebar-second" class="column sidebar" role="complementary">
          <section>
            <div id="sidebar-second-inner" class="sidebar-inner">
              <?php print render($page['sidebar_second']); ?>
            </div><!-- /sidebar-second-inner -->
          </section><!-- /section -->
        </aside><!-- /sidebar-second -->
        <?php endif; ?>
      </div><!-- /main-inner -->
    </div><!-- /main-wrapper -->

    <?php if ($page['postscript_top']): ?>
    <div id="postscript-top-wrapper" class="postscript-top">
      <div id="postscript-top-inner" class="inner clearfix">
        <?php if ($page['postscript_top']): ?>
        <div id="postscript-top-region">
          <?php print render($page['postscript_top']); ?>
        </div><!-- /postscript-top-region -->
        <?php endif; ?>
      </div><!-- /postscript-top-inner -->
    </div><!-- /postscript-top-wrapper -->
    <?php endif; ?>

    <?php if ($page['postscript_first'] || $page['postscript_second'] || $page['postscript_third']): ?>
    <div id="postscript-wrapper" class="postscript<?php print " " . $postscripts; ?>">
      <div id="postscript-inner" class="inner clearfix">
        <?php if ($page['postscript_first']): ?>
        <div id="postscript-first" class="column">
          <?php print render($page['postscript_first']); ?>
        </div><!-- /postscript-first -->
        <?php endif; ?>
  
        <?php if ($page['postscript_second']): ?>
        <div id="postscript-second" class="column">
          <?php print render($page['postscript_second']); ?>
        </div><!-- /postscript-second -->
        <?php endif; ?>
  
        <?php if ($page['postscript_third']): ?>
        <div id="postscript-last" class="column">
          <?php print render($page['postscript_third']); ?>
        </div><!-- /postscript-third -->
        <?php endif; ?>
      </div><!-- /postscript-inner -->
    </div><!-- /postscript-wrapper -->
    <?php endif; ?>

    <?php if ($page['footer_first'] || $page['footer_second'] || $page['footer_third']): ?>
    <footer role="contentinfo">
      <div id="footer-wrapper" class="footer<?php print " " . $footers; ?>">
        <div id="footer-inner" class="inner clearfix">
          <div id="footer-inner-inner" class="clearfix">
            <?php if ($page['footer_first']): ?>
            <div id="footer-first" class="column">
              <?php print render($page['footer_first']); ?>
            </div><!-- /footer-first -->
            <?php endif; ?>
      
            <?php if ($page['footer_second']): ?>
            <div id="footer-second" class="column">
              <?php print render($page['footer_second']); ?>
            </div><!-- /footer-second -->
            <?php endif; ?>
      
            <?php if ($page['footer_third']): ?>
            <div id="footer-last" class="column">
              <?php print render($page['footer_third']); ?>
            </div><!-- /footer-third -->
            <?php endif; ?>
          </div><!-- /footer-inner-inner -->
        </div><!-- /footer-inner -->
      </div><!-- /footer-wrapper -->
    </footer><!-- /footer -->
    <?php endif; ?>

  </div><!-- /page -->
</div><!-- /page-wrapper -->