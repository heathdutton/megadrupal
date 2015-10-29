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
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup themeable
 */
?>

<!-- start from drupal body -->
<body>

  <!-- Top menu / toolbar -->
  <div class="topline">
    <div class="row">

      <header id="header" role="banner" class="large-12 columns">

        <!-- Navigation&toolbar bar -->
        <nav id="navigation" role="navigation" class="top-bar">

          <!-- Print main menu  -->
          <section class="top-bar-section">
            <?php if ($main_menu): ?>
              <div id="main-menu" class="navigation">
                <?php print theme('links__system_main_menu'); ?>
              </div>
            <?php endif; ?>
          </section>
          <!-- end main menu  -->

          <!-- Toolbar & mobile menu icon -->
          <ul class="title-area">
            <li class="name">
              <ul id="quick-links">

                <!-- Mundus search  -->
                <?php if (theme_get_setting('mundus_search_top')): ?>
                  <li class="search">
                    <a href="#" data-reveal-id="search-mundus">
                      <?php print t('Search'); ?>
                      <i class="foundicon-search"></i>
                    </a>
                    <div id="search-mundus" class="reveal-modal">
                      <?php print $mundus_search; ?>
                      <a class="close-reveal-modal">
                        <i class="foundicon-remove"></i>
                      </a>
                    </div>
                  </li>
                <?php endif; ?>
                <!-- end Mundus search  -->

                <!-- Mundus social  -->
                <?php if (theme_get_setting('social_profiles_top')): ?>
                  <li class="social">
                    <a href="#" data-reveal-id="social-mundus">
                      <?php print t('Social'); ?>
                      <i class="foundicon-people"></i>
                    </a>
                    <div id="social-mundus" class="reveal-modal">
                      <?php print render($page['social']); ?>
                      <a class="close-reveal-modal">
                        <i class="foundicon-remove"></i>
                      </a>
                    </div>
                  </li>
                <?php endif; ?>
                <!-- end Mundus social  -->

                <!-- Mundus login  -->
                <?php if (theme_get_setting('mundus_login_top')): ?>

                  <?php if ($logged_in): ?>
                    <li class="login"> <?php print l(t('My Account'), 'user'); ?></li>
                  <?php else: ?>

                    <li class="login">
                      <a href="#" data-reveal-id="login-mundus">
                        <?php print t('Log in'); ?>
                        <i class="foundicon-lock"></i>
                      </a>
                      <div id="login-mundus" class="reveal-modal">
                        <div class="large-6 columns">
                          <?php
                          /* @var $mundus_ul type */
                          $mundus_ul = drupal_get_form("user_login");
                          $mundus_ul_form = drupal_render($mundus_ul);
                          echo $mundus_ul_form;
                          ?>
                          <ul class="inline-list">
                            <li> <?php print l(t('Create an account'), 'user/register'); ?></a><li>
                            <li> <?php print l(t('Forgot your password?'), 'user/password'); ?></a><li>
                          </ul>
                        </div>
                        <div class="large-6 columns">
                          <?php print render($page['login_region']); ?>
                        </div>
                        <a class="close-reveal-modal">
                          <i class="foundicon-remove"></i>
                        </a>
                      </div>
                    </li>

                  <?php endif; ?>
                <?php endif; ?>
                <!-- end Mundus login  -->

              </ul>
            </li>

            <!--  mobile menu icon -->
            <li class="toggle-topbar menu-icon"><a href="#"><span>menu</span></a></li>
            <!-- end mobile menu icon -->

          </ul>
          <!-- end toolbar & mobile menu icon -->

        </nav>
        <!-- end navigation toolbar bar-->

      </header>
    </div>
  </div>
  <!-- end of top menu / toolbar -->

  <!-- Additional header region - full width -->
  <div class="row">
    <?php print render($page['header']); ?>
  </div>
  <!-- end of header region -->


  <div class="row">
    <div id="content" class="large-8 columns" role="main">

      <!-- Mundus logo -->
      <div id="logo">
        <h1><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          </a></h1>
      </div>
      <!-- end logo -->

      <!-- Drupal default content  -->
      <?php print $messages; ?>
      <?php print render($page['help']); ?>

      <?php if ($page['highlighted']): ?>
        <div id="highlighted"><?php print render($page['highlighted']); ?></div>
      <?php endif; ?>

      <?php if ($breadcrumb): ?>
        <?php print $breadcrumb; ?>
      <?php endif; ?>

      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>

      <?php if ($tabs): ?>
        <div class="tabs"><?php print render($tabs); ?></div>
      <?php endif; ?>

      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>

      <?php print render($page['content']); ?>
    </div>
    <!-- end Drupal default content  -->

    <!-- Print sidebar region -->
    <?php if ($page['sidebar_first']): ?>
      <aside id="sidebar-first" class="large-4 columns" role="complementary">
        <?php print render($page['sidebar_first']); ?>
      </aside>
    </div>
  <?php endif; ?>
  <!-- end sidebar region -->


  <!-- Three blocks region -->
  <div class="row">

    <div class="large-4 columns">
      <?php print render($page['one']); ?>
    </div>

    <div class="large-4 columns">
      <?php print render($page['two']); ?>
    </div>

    <div class="large-4 columns">
      <?php print render($page['three']); ?>
    </div>

  </div>
  <!-- end three blocks region -->

  <!-- FOOTER REGION -->
  <?php if ($page['footer']): ?>
    <div class="row">
      <div class="large-12 columns">
        <footer id="footer" role="contentinfo">
          <?php print render($page['footer']); ?>
        </footer>
      </div>
    </div>
  <?php endif; ?>
  <!-- end footer -->
