<?php
/**
 * @file
 * Zen theme's implementation to display a single Drupal page.
 *
 * @section id_and_css CSS id and class structure
 *
 * - #page
 *   - #header
 *     - #head-banner
 *       - #secondary-menu
 *       - #top
 *         - .region-top-first
 *         - .region-top-second
 *         - .region-top-third
 *     - #identity
 *       - a#logo
 *       - #name-and-slogan
 *         - #site-name
 *         - #site-slogan
 *     - #section
 *       - .region-section-first
 *       - .region-section-second
 *     - #main-menu
 *   - #main
 *     - .region-feature
 *     - #content
 *       - .region-highlighted
 *       - .region-content
 *     - #sidebars
 *       - .region-sidebar-first
 *       - .region-sidebar-second
 *   - #footer
 *     - .region-footer
 *     - #bottom
 *       - .region-bottom-first
 *       - .region-bottom-second
 *     - #tertiary-menu
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
 * - $main_menu: The rendered output of the Main menu links..
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $secondary_menu_heading: The title of the menu used by the secondary links.
 * - $tertiary_menu (array): An array containing the Tertiary menu links for
 *   the site, if they have been configured.
 * - $tertiary_menu_heading: The title of the menu used by the tertiary links.
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
 * - $page['header']: Items for the header region.
 * - $page['navigation']: Items for the navigation region, below the main menu (if any).
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['footer']: Items for the footer region.
 * - $page['bottom']: Items to appear at the bottom of the page below the footer.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see zen_preprocess_page()
 * @see template_process()
 */
?>

<div id="page">

  <?php if ($show_head_banner || $show_identity || $show_section || $main_menu): ?>
    <header id="header" role="banner">

    <?php if ($show_head_banner): ?>
      <div id="head-banner">

        <?php if ($secondary_menu): ?>
          <nav id="secondary-menu" role="navigation">
            <?php print $secondary_menu_rendered; ?>
          </nav><!-- /#secondary-menu -->
        <?php endif; ?>

        <?php if ($show_top): ?>
          <section id="top">
            <?php print $top_first; ?>
            <?php print $top_second; ?>
            <?php print $top_third; ?>
          </section><!-- /#top -->
        <?php endif; ?>

      </div><!-- /#head-banner -->
    <?php endif; ?>

    <?php if ($show_identity): ?>
      <div id="identity">

        <?php if ($site_name || $site_slogan): ?>
          <hgroup id="name-and-slogan">
            <?php if ($site_name): ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
            <?php endif; ?>

            <?php if ($site_slogan): ?>
              <h2 id="site-slogan"><?php print $site_slogan; ?></h2>
            <?php endif; ?>
          </hgroup><!-- /#name-and-slogan -->
        <?php endif; ?>

        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
        <?php endif; ?>

      </div><!-- /#identity -->
    <?php endif; ?>

      <?php if ($show_section): ?>
        <section id="section">
          <?php print $section_first; ?>
          <?php print $section_second; ?>
        </section><!-- /#section -->
      <?php endif; ?>

      <?php if ($main_menu): ?>
        <nav id="main-menu" role="navigation">
          <?php print $main_menu; ?>
        </nav><!-- /#main-menu -->
      <?php endif; ?>

      <?php print render($page['navigation']); ?>

    </header><!-- /#header -->
  <?php endif; ?>

  <div id="main">
    <?php print render($page['feature']); ?>
    <div id="content" class="column" role="main">
      <?php print render($page['highlighted']); ?>
      <?php print $breadcrumb; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <?php print render($tabs); ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div><!-- /#content -->

    <?php if ($show_sidebars): ?>
      <aside class="sidebars">
        <?php print $sidebar_first; ?>
        <?php print $sidebar_second; ?>
      </aside><!-- /#sidebars -->
    <?php endif; ?>

  </div><!-- /#main -->

  <?php
    $footer = render($page['footer']);
  ?>

  <?php if ($footer || $show_bottom || $tertiary_menu): ?>
    <footer id="footer">

      <?php if ($show_bottom): ?>
        <section id="bottom">
          <?php print $bottom_first; ?>
          <?php print $bottom_second; ?>
        </section><!-- /#bottom -->
      <?php endif; ?>

      <?php if ($footer || $tertiary_menu): ?>
        <div id="foot-banner">
          <?php print $footer; ?>

          <?php if ($tertiary_menu): ?>
            <nav id="tertiary-menu" role="navigation">
              <?php print $tertiary_menu_rendered; ?>
            </nav><!-- /#tertiary-menu -->
          <?php endif; ?>

        </div> <!-- /#foot-banner -->
      <?php endif; ?>

    </footer><!-- /#footer -->
  <?php endif; ?>

</div><!-- /#page -->
