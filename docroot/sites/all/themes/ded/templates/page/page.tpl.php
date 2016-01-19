<?php
/**
 * @file
 * Theme implementation to display a single Drupal page.
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
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 *
 * <?php print render($page['Region_Name']); ?>
 * 
 */
?>

  <div id="page">
  
    <?php if ($main_menu): ?>
      <div id="main-menu" class="navbar">
        <div class="navbar-inner">
          <div class="container">
            <a class="brand" href="<?php print check_url($front_page) ?>">
              <?php print $site_name; ?>
            </a>
            <?php print $main_menu_tree; ?>

            <?php if ($secondary_menu): ?>
              <div id="secondary-menu" class="pull-right">
                <?php print theme('links__system_secondary_menu', array(
                  'links' => $secondary_menu,
                )); ?>
              </div><!-- /secondary_menu -->
            <?php endif; ?>
          </div>
        </div>
      </div><!--/#main-menu-->
    <?php endif; ?>

    <?php $content_top_region = $page['help'] || $messages ?>
    <?php $content_region = $tabs || $title || $page['content']; ?>
    <?php if ($content_top_region || $content_region || $page['sidebar_second']): ?>
      <div id="main-wrapper" class="clearfix">
          
        <?php if ($page['sidebar_first']): ?>
          <div id="sidebar-first" class="sidebar span">
            <?php print render($page['sidebar_first']); ?>
          </div><!-- /sidebar-first -->
        <?php endif; ?>

        <?php if ($content_top_region || $content_region): ?>
          <div id="content-wrapper" class="span">
            
            <?php if ($content_top_region): ?>
              <div id="content-top">
                
                <?php if ($page['help']): ?>
                  <?php print render($page['help']); ?>
                <?php endif; ?>
                    
                <?php if ($messages): ?>
                  <?php print $messages; ?>
                <?php endif; ?>
                        
              </div><!-- /content-top -->
            <?php endif; ?>

            <?php if ($content_region): ?>
              <div id="content">
                <a name="main-content" id="main-content"></a>
                               
                <?php if ($title || $page['content']): ?>
                  <div id="content-inner">
                      
                    <?php print render($title_prefix); ?>
                    <?php if ($title): ?>
                      <div class="page-header">
                        <h1 id="page-title"><?php print $title; ?></h1>
                      </div>
                    <?php endif; ?>
                    <?php print render($title_suffix); ?>
                    <?php if ($breadcrumb): ?>
                      <div id="breadcrumb">
                        <?php print $breadcrumb; ?>
                      </div><!-- /breadcrumb -->
                    <?php endif; ?>

                    <?php if (!empty($tabs['#primary'])): ?>
                      <div id="content-tabs" class="clearfix">
                        <?php print render($tabs); ?>
                      </div>
                    <?php endif; ?>
                      
                    <?php if ($action_links): ?>
                      <ul class="action-links"><?php print render($action_links); ?></ul>
                    <?php endif; ?>
                        
                    <?php if ($page['content']): ?>
                      <?php print render($page['content']); ?>
                    <?php endif; ?>
                      
                  </div><!-- /content-inner -->
                <?php endif; ?>
              </div><!-- /content -->
            <?php endif; ?>
          </div><!-- /content-wrapper -->
        <?php endif; ?>

        <?php if ($page['sidebar_second']): ?>
          <div id="sidebar-second" class="sidebar">
            <?php print render($page['sidebar_second']); ?>
          </div><!-- /sidebar_second -->
        <?php endif; ?>
          
      </div><!-- /main-wrapper -->
    <?php endif; ?>
  
      <div id="footer-wrapper" class="clearfix">
        <?php print render($page['footer']); ?>
      </div><!-- /footer-wrapper -->
   </div><!-- /page -->