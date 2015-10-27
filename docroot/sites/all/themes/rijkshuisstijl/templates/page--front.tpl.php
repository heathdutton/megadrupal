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
 * Main bar:
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
 * - $page['main_bar']: The main bar for the main menu and search form.
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

<?php if ($page['highlighted']): ?>
  <div id="highlighted">
    <?php print render($page['highlighted']); ?>
  </div>
<?php endif; ?>

  <div id="centerbox" role="document">
    <?php if ($page['header_menu']): ?>
    <div id="header-menu" role="contentinfo">
      <?php print render($page['header_menu']); ?>
    </div>
    <?php endif; ?>
    <div id="mainbox">
      <header id="header" role="banner" class="clearfix">
        <?php if ($projectlogo && $branding_type == "neutral"): ?>
          <div id="projectlogo" >
          <?php print $projectlogo; ?>
          </div>
        <?php endif; ?>

        <?php if ($rijkslogo && $branding_type == "rijkshuisstijl"): ?>
          <div id="rijkslogo" >
          <?php print $rijkslogo; ?>
          </div>
        <?php endif; ?>

        <div class="skiplinks element-invisible">
          <p>
            <span class="assistive"><?php print t('Go to'); ?></span>
            <a title="<?php print t('Go to the content'); ?>" href="#content"><?php print t('Content'); ?></a>
            <span class="assistive"><?php print t('or'); ?></span>
            <a id="skip-to-menu" class="active" title="<?php print t('Go to the main menu'); ?>" href="#main-bar"><?php print t('Menu'); ?></a>
          </p>
        </div>
        <?php print render($page['header']); ?>
        <?php if ($site_name): ?>
          <div id="title-bar">
              <?php print $site_name; ?>
          </div>
        <?php endif; ?>
      </header>

      <?php if ($page['main_bar']): ?>
        <div id="main-bar" class="clearfix">
          <?php print render($page['main_bar']); ?>
        </div>
      <?php endif; ?>


      <div id="main" role="main" class="clearfix">
        <div id="content" role="article" class="column article">
          <?php print render($title_prefix); ?>
          <?php if ($title): ?><h1<?php print $title_attributes; ?>><?php print $title; ?></h1><?php endif; ?>
          <?php print render($title_suffix); ?>

          <?php if ($messages || $page['help']): ?>
            <div id="messages" role="alertdialog">
              <?php print $messages; ?>
              <?php print render($page['help']); ?>
            </div>
          <?php endif; ?>

          <?php if ($tabs): ?>
            <div class="tabs"><?php print render($tabs); ?></div>
          <?php endif; ?>

          <?php if ($action_links): ?>
            <ul class="action-links"><?php print render($action_links); ?></ul>
          <?php endif; ?>

          <?php print render($page['content']); ?>

        </div>

        <?php if ($page['sidebar_first']): ?>
          <aside id="sidebar-first" role="complementary" class="column sidebar">
            <?php print render($page['sidebar_first']); ?>
          </aside>
        <?php endif; ?>

        <?php if ($page['sidebar_second']): ?>
          <aside id="sidebar-second" role="complementary" class="column sidebar">
            <?php print render($page['sidebar_second']); ?>
          </aside>
        <?php endif; ?>
      </div>

      <?php if ($site_slogan && $branding_type == "rijkshuisstijl"): ?>
        <p id="payoff"><span><?php print $site_slogan; ?></span></p>
      <?php endif; ?>
    </div>

    <footer id="footer" role="contentinfo">
      <?php print render($page['footer']); ?>
      <?php if ($page['footer_menu']): ?>
      <div id="footer-menu" role="contentinfo">
        <?php print render($page['footer_menu']); ?>
      </div>
      <?php endif; ?>
    </footer>
  </div>
