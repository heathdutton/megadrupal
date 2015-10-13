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

<div id="main-wrapper">
  <div id="gradient-left-right">
    	
    <div class="container-blueprint" id="container-wrapper">
        <div class="span-25 bottom-gradient">
          
          <!--begin header-->
          <div id="header">
            <div id="logo" class="span-6">
              <a title="<?php print $site_name; ?>" href="<?php print $base_path; ?>">
                <img src="/<?php print $directory; ?>/images/logo.png" alt="<?php print $site_name; ?>"/>
              </a>
            </div>

            <div id="top-menu-wrapper" class="span-18 last">
              <?php if ($page['top_menu']): ?>
                <div id="top-menu" style="float:right;">
                  <?php print render($page['top_menu']); ?>
                </div>
              <?php endif; ?>


              </div>
          </div>
          <!--end header-->

          <?php if (!empty($page['top_content_full_width'])): ?>
            <div class="span-24 round-corners-5 white-bg last top-content-full-width">
              <?php print render($page['top_content_full_width']); ?>
            </div>
          <?php endif; ?>

          
          <?php if (!empty($page['left_sidebar'])): ?>
            <!--begin left sidebar-->
            <div class="span-5 left-sidebar round-corners-5 gray-bg">
                <?php print render($page['left_sidebar']); ?>
            </div>
            <!--end left sidebar-->
          <?php endif; ?>
          
          <!--begin middle column-->
          <div class="
            <?php if (empty($page['right_sidebar']) && empty($page['left_sidebar'])): ?>
              span-24 last middle-content-full-width
            <?php elseif (empty($page['right_sidebar']) || empty($page['left_sidebar'])): ?>
              span-18 middle-content
            <?php else: ?>
              span-13 middle-content
            <?php endif; ?>">

            <?php if (!empty($page['search_box'])): ?>
              <!--begin search-->
              <div class="
              <?php if (empty($page['right_sidebar'])): ?>
                span-18 search-700
              <?php else: ?>
                span-13 search-500
              <?php endif; ?> block round-corners-5 white-bg">
                <div class="search-wrapper"><?php print render($page['search_box']); ?></div>
              </div>
              <!--end search-->
            <?php endif; ?>

            <?php if (!empty($page['top_content'])): ?>
              <!--begin banner top-->
              <div class="
                  <?php if (empty($page['right_sidebar'])): ?>
                    span-18
                  <?php else: ?>
                    span-13
                  <?php endif; ?>
                    block banner round-corners-5 white-bg">
                <?php print render($page['top_content']); ?>
              </div>
              <!--end banner top-->
            <?php endif; ?>


             <!--begin content-->
            <div class="
            <?php if (empty($page['right_sidebar']) && empty($page['left_sidebar'])): ?>
              span-24 last content-bottom-full-width
            <?php elseif (empty($page['right_sidebar'])): ?>
              span-18 content-bottom-700
            <?php else: ?>
              span-13 content-bottom-500
            <?php endif; ?>
              round-corners-5 white-bg">

              <h1 class="title" id="page-title"><?php print $title; ?></h1>

              <?php if (!empty($messages)): print $messages; endif; ?>
              <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
              <?php print render($page['help']); ?>
              <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>

              <?php print render($page['content']); ?>

            </div>
            <!--end content-->
          </div>
          <!--end middle column-->
            
          <?php if (!empty($page['right_sidebar'])): ?>
            <!--begin right sidebar-->
            <div class="span-5 last right-sidebar round-corners-5 white-bg">
              <?php print render($page['right_sidebar']); ?>
            </div>
            <!--end right sidebar-->
          <?php endif; ?>
            


          <?php if (!empty($page['footer'])): ?>
            <!--begin footer-->
            <div class="span-24 round-corners-5 white-bg" id="footer">
              <?php print render($page['footer']); ?>
            </div>
            <!--end footer-->
          <?php endif; ?>

          <?php
          /**
           * Please do not remove the credits below.
           * We have worked long and hard at this theme, and now we are offering
           * it for free. We deserve it! ;)
           * So be a sport and leave it the way it is.
           *
           * Thanks.
           */
          ?>
          <div class="span-24" id="credits">
            Designed by <a target="_blank" href="http://d-alex.com/">Dragos Alexandru</a>.
            Developped by <a target="_blank" href="http://www.dicix.ro">dicix</a>.
          </div>

        </div>
    </div>

	</div>
  <div id="gradient-bottom"></div>
</div>
