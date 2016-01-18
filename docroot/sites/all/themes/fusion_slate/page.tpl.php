<?php

/**
 * @file
 * Fusion theme implementation to display a page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation.
 *   At the very least, this will always default to /.
 * - $directory: The directory the template is located in,
 *   e.g. modules/system or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been
 *   disabled in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been
 *   disabled in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the site,
 *   if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links
 *   for the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears
 *   in the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node associated
 *   with the page, and the node ID is the second argument in the page's path
 *   (e.g. node/12345 and node/12345/revisions, but not comment/reply/12345).
 *
 * Regions:
 * - $page['header_top']: Items for the header top region.
 * - $page['header']: Items for the header region.
 * - $page['main_menu']: Main menu placement.
 * - $page['preface_top']: Items for the preface top region.
 * - $page['preface_bottom']: Items for the preface bottom region.
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['postcript_top']: Items for the postcript top region.
 * - $page['postscript_bottom']: Items for the postscript bottom region.
 * - $page['footer']: Items for the footer region.
 */
?>

  <div id="page" class="page">
    <div id="page-inner" class="page-inner">

      <!-- header-top row: width = grid_width -->
      <?php print render($page['header_top']); ?>

      <!-- header-group row: width = grid_width -->
      <div id="header-group-wrapper" class="header-group-wrapper <?php if ($page['preface_top']): ?><?php echo "with-preface-top"; ?><?php else : ?><?php echo "without-preface-top"; ?><?php endif; ?> full-width">
        <div id="header-group" class="header-group region <?php print $grid_width; ?>">
          <div id="header-group-inner" class="header-group-inner inner clearfix">
        		<?php print render($page['main_menu']); ?>
        		<?php if ($logo || $site_name || $site_slogan || ($page['header'])): ?>
            <div id="header-site-info" class="header-site-info <?php if ($page['preface_top']): ?><?php echo "with-preface-top"; ?><?php else : ?><?php echo "without-preface-top"; ?><?php endif; ?> block">
              <div id="header-site-info-inner" class="header-site-info-inner inner clearfix">
                <?php if ($logo): ?>
                <div id="logo">
                  <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
                </div>
                <?php endif; ?>
                <?php if ($site_name || $site_slogan): ?>
                <div id="site-name-wrapper" class="clearfix">
                  <?php if ($site_name): ?>
                  <span id="site-name"<?php if ($site_slogan): ?> class="with-slogan"<?php endif; ?>><a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></span>
                  <?php endif; ?>
                  <?php if ($site_slogan): ?>
                  <span id="slogan"><?php print $site_slogan; ?></span>
                  <?php endif; ?>
                </div><!-- /site-name-wrapper -->
                <?php endif; ?>
                <?php if ($page['header']): ?>
                <div id="header-wrapper" class="header-wrapper">
                  <?php print render($page['header']); ?>
                </div>
                <?php endif; ?>
              </div><!-- /header-site-info-inner -->
            </div><!-- /header-site-info -->
            <?php endif; ?>
          </div><!-- /header-group-inner -->

          <!-- preface-top row: width = grid_block -->
          <?php print render($page['preface_top']); ?>

        </div><!-- /header-group -->
      </div><!-- /header-group-wrapper -->

      <!-- main row: width = grid_width -->
      <div id="main-wrapper" class="main-wrapper full-width clearfix">
        <div id="main" class="main region <?php print $grid_width; ?>">
          <div id="main-inner" class="main-inner inner clearfix">
            <?php print render($page['sidebar_first']); ?>

            <!-- main group: width = grid_width - sidebar_first_width -->
            <div id="main-group" class="main-group region nested <?php print $main_group_width; ?>">
              <div id="main-group-inner" class="main-group-inner inner clearfix">
                <?php print render($page['preface_bottom']); ?>

                <div id="main-content" class="main-content region nested">
                  <div id="main-content-inner" class="main-content-inner inner clearfix">
                    <!-- content group: width = grid_width - (sidebar_first_width + sidebar_last_width) -->
                    <div id="content-group" class="content-group region nested <?php print $content_group_width; ?>">
                      <div id="content-group-inner" class="content-group-inner inner clearfix">
	                    <?php print theme('grid_block', array('content' => $breadcrumb, 'id' => 'breadcrumbs')); ?>
                        <?php print theme('grid_block', array('content' => $messages, 'id' => 'content-messages')); ?>

                        <div id="content-region" class="content-region region nested">
                          <div id="content-region-inner" class="content-region-inner inner clearfix">
                            <a name="main-content-area" id="main-content-area"></a>
	                          <?php if ($tabs): ?>
			                    <div id="content-tabs" class="clear">
			                      <?php print render($tabs); ?>
			                    </div>
	                          <?php endif; ?>
                            <?php print theme('grid_block', array('content' => render($page['help']), 'id' => 'content-help')); ?>
                            <div id="content-inner" class="content-inner block">
                              <div id="content-inner-inner" class="content-inner-inner inner clearfix">
                                <?php print render($title_prefix); ?>
                                <?php if ($title): ?>
                                <h1 class="title gutter"><?php print $title; ?></h1>
                                <?php endif; ?>
                                <?php print render($title_suffix); ?>
                                <?php if ($page['content']): ?>
                                <div id="content-content" class="content-content">
				                  <?php print render($page['content']); ?>
                                </div><!-- /content-content -->
                                <?php endif; ?>
                              </div><!-- /content-inner-inner -->
                            </div><!-- /content-inner -->
                          </div><!-- /content-region-inner -->
                        </div><!-- /content-region -->

                        <?php print render($page['content_bottom']); ?>
                      </div><!-- /content-group-inner -->
                    </div><!-- /content-group -->

                    <?php print render($page['sidebar_second']); ?>
                  </div><!-- /main-content-inner -->
                </div><!-- /main-content -->

                <?php print render($page['postscript_top']); ?>
              </div><!-- /main-group-inner -->
            </div><!-- /main-group -->
          </div><!-- /main-inner -->
        </div><!-- /main -->
      </div><!-- /main-wrapper -->

      <!-- postscript-bottom row: width = grid_width -->
      <?php print render($page['postscript_bottom']); ?>

      <!-- footer row: width = grid_width -->
      <?php print render($page['footer']); ?>
    </div><!-- /page-inner -->
  </div><!-- /page -->
