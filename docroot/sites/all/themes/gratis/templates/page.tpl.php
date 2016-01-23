<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 * least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 * or themes/garland.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 * when linking to the front page. This includes the language domain or
 * prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 * in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 * in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 * site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 * the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 * modules, intended to be displayed in front of the main title tag that
 * appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 * modules, intended to be displayed after the main title tag that appears in
 * the template.
 * - $messages: HTML for status and error messages. Should be displayed
 * prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 * (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 * menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 * associated with the page, and the node ID is the second argument
 * in the page's path (e.g. node/12345 and node/12345/revisions, but not
 * comment/reply/12345).
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

  <div class="l-page-wrapper">
    <div class="l-page">

      <!-- top links-->
      <?php if ($page['top_links']): ?>
        <div id="top-bar" class="">
          <div class="l-top-wrapper l-setwidth" <?php if (!empty($set_width)) : print 'style="max-width:' . $set_width . ';"' ; endif; ?>>
            <div class="top-links s-grid">
              <?php print render($page['top_links']); ?>
            </div>
          </div>
        </div>
      <?php endif; ?>
      <!-- //top links-->

      <!-- header -->
      <div id="header-bar" class="l-header-wrapper" role="banner">
        <header class="l-header l-setwidth" <?php if (!empty($set_width)) : print 'style="max-width:' . $set_width . ';"' ; endif; ?>>

          <?php if ($logo): ?>
          <div class="l-logo">
              <a href="<?php print $front_page; ?>" title="<?php print $site_name; ?> » <?php print $site_slogan; ?>">
                <img id="logo-img" src="<?php print $logo; ?>" alt="<?php print $site_name; ?> » <?php print $site_slogan; ?>"/></a>
            </div><!--// l-logo-->
            <?php endif; ?>

          <?php if ($site_slogan || $site_name) : ?>
          <div class="l-branding">

                <?php if ($site_name) : ?>
                  <h1 class="site-name">
                    <a href="<?php print $front_page; ?>">
                      <?php print $site_name; ?></a>
                  </h1>
              <?php endif; ?>

                  <?php if ($site_slogan) : ?>
                    <h3 class="site-slogan"><?php print $site_slogan; ?></h3>
                <?php endif; ?>

          </div><!--//branding-->
            <?php endif; ?>

        </header>
      </div><!-- // l-header -wrapper-->

      <div id="menu-wrapper" class="l-menu-wrapper main-menu" role="navigation">
        <div class="l-setwidth" <?php if (!empty($set_width)) : print 'style="max-width:' . $set_width . ';"' ; endif; ?>>

          <?php if ($main_menu): ?>
            <a id="off-canvas-left-show" href="#off-canvas" class="l-off-canvas-show l-off-canvas-show--left"><?php print t('Show Navigation'); ?></a>
            <div id="off-canvas-left" class="l-off-canvas l-off-canvas--left">
              <a id="off-canvas-left-hide" href="#" class="l-off-canvas-hide l-off-canvas-hide--left"><?php print t('Hide Navigation'); ?></a>

              <div class="main-menu-wrapper">

                <?php print render($primary_nav); ?>
              </div>
            </div><!-- // off-canvas-left -->
          <?php endif; ?>
          <!-- //main menu -->

          <!-- for third party menu systems or modules-->
          <?php if ($page['thirdparty_menu']): ?>
            <?php print render($page['thirdparty_menu']); ?>
          <?php endif; ?>

        </div>
      </div>

<div class="l-content-wrap">

  <?php if (!empty($page['full_width_highlight'])): ?>
    <div class="l-fullwidth-highlight">
      <?php print render($page['full_width_highlight']); ?>
    </div>
  <?php endif; ?>

      <?php if ($breadcrumb): ?>
        <div id="breadcrumbs-wrapper" class="l-breadcrumbs">
          <div class="breadcrumbs l-setwidth" <?php if (!empty($set_width)) : print 'style="max-width:' . $set_width . ';"' ; endif; ?>>
            <div class="s-grid"><?php print $breadcrumb; ?></div>
          </div>
        </div>
      <?php endif; ?>

      <!-- preface -->
      <?php if ($page['preface_first'] || $page['preface_second'] || $page['preface_third']): ?>

        <div id="preface-wrap" class="l-preface-wrap">
          <div id="preface-container" class="l-preface l-setwidth" <?php if (!empty($set_width)) : print 'style="max-width:' . $set_width . ';"' ; endif; ?>>

            <!--Preface -->
            <?php if (!empty($page['preface_first'])): ?>
              <div class="preface">
                <?php print render($page['preface_first']); ?>
              </div>
            <?php endif; ?>

            <?php if (!empty($page['preface_second'])): ?>
              <div class="preface">
                <?php print render($page['preface_second']); ?>
              </div>
            <?php endif; ?>

            <?php if (!empty($page['preface_third'])): ?>
              <div class="preface">
                <?php print render($page['preface_third']); ?>
              </div>
            <?php endif; ?>

          </div>
        </div>
        <!-- // preface -->

      <?php endif; ?>

      <div class="main" <?php if (!empty($min_height)) : print $min_height; endif; ?>>
        <div class="l-main l-setwidth" role="main" <?php if (!empty($set_width)) : print 'style="max-width:' . $set_width . ';"' ; endif; ?>>

          <div class="l-content">
            <a id="main-content"></a>
            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
              <h1><?php print $title; ?></h1>
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
          </div>

          <?php print render($page['sidebar_first']); ?>
          <?php print render($page['sidebar_second']); ?>
        </div>

      </div>

      <?php
      // Define and divide the postscript page regions.
      if ($page['postscript_first'] || $page['postscript_second'] || $page['postscript_third']): ?>

        <div id="postscript-wrapper">
          <div id="postscript-container" class="l-postscript l-setwidth" <?php if (!empty($set_width)) : print 'style="max-width:' . $set_width . ';"' ; endif; ?>>

            <!--Postscript -->
            <?php if (!empty($page['postscript_first'])): ?>
              <div class="postscript">
                <?php print render($page['postscript_first']); ?>
              </div>
            <?php endif; ?>

            <?php if (!empty($page['postscript_second'])): ?>
              <div class="postscript">
                <?php print render($page['postscript_second']); ?>
              </div>
            <?php endif; ?>

            <?php if (!empty($page['postscript_third'])): ?>
              <div class="postscript">
                <?php print render($page['postscript_third']); ?>
              </div>
            <?php endif; ?>

          </div>
        </div>

      <?php endif; ?>


      <footer id="footer" role="footer" class="l-footer-wrapper">
        <div class="l-setwidth l-footer" <?php if (!empty($set_width)) : print 'style="max-width:' . $set_width . ';"' ; endif; ?>>

          <!--footer -->
          <?php if (!empty($page['footer_first'])): ?>
            <div class="footer">
              <?php print render($page['footer_first']); ?>
            </div>
          <?php endif; ?>

          <?php if (!empty($page['footer_second'])): ?>
            <div class="footer">
              <?php print render($page['footer_second']); ?>
            </div>
          <?php endif; ?>

          <?php if (!empty($page['footer_third'])): ?>
            <div class="footer">
              <?php print render($page['footer_third']); ?>
            </div>
          <?php endif; ?>

        </div>
      </footer>

</div>

    </div>
    <a href="#" class="scrolltop">Scroll to the top</a>
  </div>
