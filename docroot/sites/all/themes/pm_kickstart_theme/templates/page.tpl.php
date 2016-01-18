<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
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
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see bootstrap_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see bootstrap_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>
<div id="wrapper" class="clearfix">
  <header id="navbar" role="banner" class="navbar navbar-default navbar-fixed-top">
    <div class="navbar-header">
      <?php if ($logo): ?>
      <a class="logo navbar-btn pull-left" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
      <?php endif; ?>

      <?php if (!empty($site_name)): ?>
      <a class="name navbar-brand" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
      <?php endif; ?>
    </div>
    <?php if (!empty($page['header'])): ?>
      <div class="pull-left header">
        <?php print render($page['header']); ?>
      </div>
    <?php endif; ?>
    <div class="pull-right secondary-nav">
      <?php if (!empty($secondary_nav)): ?>
          <nav role="navigation">
            <?php if (!empty($secondary_nav)): ?>
              <?php print render($secondary_nav); ?>
            <?php endif; ?>
          </nav>
      <?php endif; ?>
      <?php if (!empty($page['navigation'])): ?>
        <?php print render($page['navigation']); ?>
      <?php endif; ?>
    </div>
  </header>
        <!-- Sidebar -->
        <div id="sidebar-wrapper" class="sidebar-wrapper">
            <div class="navbar-default sidebar clearfix" role="navigation">
              <div class="sidebar-nav clearfix">
                <?php if (!empty($primary_nav)): ?>
                    <nav role="navigation">
                      <?php if (!empty($primary_nav)): ?>
                        <?php print render($primary_nav); ?>
                      <?php endif; ?>
                    </nav>
                <?php endif; ?>
              </div>
              <?php if (!empty($page['sidebar_first'])): ?>
                <div class="sidebar-first-wrapper">
                  <?php print render($page['sidebar_first']); ?>
                </div>
              <?php endif; ?>
            </div>
            <button id="sidebar-toggle-button" class="btn"><i class="fa fa-chevron-left pull-left"></i></button>
        </div>
        <!-- /#sidebar-wrapper -->

        <!-- Page Content -->
        <div id="page-content-wrapper" class="main-content-wrapper">
            <div class="container-fluid">
                <div class="row">
                    <div class="col-lg-12">
                      <div class="row">
                        <header role="banner" id="page-header">
                          <?php if (!empty($site_slogan)): ?>
                            <span class="lead"><?php print $site_slogan; ?></span>
                          <?php endif; ?>
                        </header> <!-- /#page-header -->

                        <section>
                          <?php if (!empty($page['highlighted'])): ?>
                            <div class="highlighted jumbotron"><?php print render($page['highlighted']); ?></div>
                          <?php endif; ?>
                          <?php if (!empty($breadcrumb)): print $breadcrumb; endif;?>
                          <a id="main-content"></a>
                          <?php print render($title_prefix); ?>
                          <?php if (!empty($title)): ?>
                            <h1 class="page-header"><?php print $title; ?></h1>
                          <?php endif; ?>
                          <?php print render($title_suffix); ?>
                          <?php print $messages; ?>
                          <?php if (!empty($tabs)): ?>
                            <?php print render($tabs); ?>
                          <?php endif; ?>
                          <?php if (!empty($page['help'])): ?>
                            <?php print render($page['help']); ?>
                          <?php endif; ?>
                          <?php if (!empty($action_links)): ?>
                            <ul class="action-links"><?php print render($action_links); ?></ul>
                          <?php endif; ?>
                          <?php print render($page['content']); ?>
                        </section>

                        <footer class="footer container">
                          <?php print render($page['footer']); ?>
                        </footer>
                      </div>
                    </div>
                </div>
            </div>
        </div>
        <!-- /#page-content-wrapper -->

    </div>