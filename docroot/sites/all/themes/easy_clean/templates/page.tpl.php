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
<?php if($page['outside_top']) { ?>
<div class="outside-top"><?php print render($page['outside_top']); ?></div><!-- .outside-top -->
<?php } ?>

<div class="wrapper-container">
  
  <div class="container">
  
    <header class="header-page" role="banner">
      
      <?php if($page['header_top']) { ?>
      <div class="header-top"><?php print render($page['header_top']); ?></div><!--.header-top-->
      <?php } ?>
      
      <?php if($logo || $site_name || $site_slogan || $page['header']) { ?>
      
      <div class="header-middle">
        
        <?php if($logo || $site_name || $site_slogan) { ?>
        
          <?php if($logo) { ?>
          <a class="logo" href="<?php print $front_page; ?>" title="<?php print $site_name; ?>"> <img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" /> </a>
          <?php } ?>

          <?php if($site_name) { ?>
          <h1 title="<?php print $site_name; ?>" class="site-name"><a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>"><?php print $site_name; ?></a></h1>
          <?php } ?>

          <?php if($site_slogan) { ?>         
          <h2 title="<?php print $site_slogan; ?>" class="site-slogan"><?php print $site_slogan; ?></h2>
          <?php } ?>
        
        <?php } ?>
        
        <?php print render($page['header']); ?> 
        
      </div><!--.header-middle-->
      
      <?php } ?>
     
      <?php if($page['header_bottom']) { ?>
      <div class="header-bottom"><?php print render($page['header_bottom']); ?></div><!--.header-bottom-->
      <?php } ?>
      
    </header><!-- .header-page -->
    
    <?php if ($primary_nav) { ?>
    <nav class="primary-navigation" role="navigation">
      <?php print $primary_nav; ?>
    </nav>
    <?php } ?>
    
    <?php if($secondary_nav) { ?>
    <nav class="secondary-navigation" role="navigation">
      <?php print $secondary_nav; ?>
    </nav>
    <?php } ?>

    <?php if($page['preface']) { ?>
    <div class="preface"><?php print render($page['preface']); ?></div><!-- .preface -->
    <?php } ?>
    
    <div class="wrapper-columns clearfix">

      <div class="main" role="main">

        <?php if($page['content_top']) { ?>
        <div class="content-top"> <?php print render($page['content_top']); ?> </div><!-- .content-top -->
        <?php } ?>

        <div class="content">

          <?php if($breadcrumb) { ?>
          <div class="breadcrumb"><?php print $breadcrumb; ?> </div><!-- .breadcrumb -->
          <?php } ?>

          <?php if($page['highlighted']){ ?>
          <div class="highlighted"><?php print render($page['highlighted']); ?></div><!-- .highlighted -->        
          <?php } ?>

          <?php print render($title_prefix); ?>
          <?php if($title) { print '<h2 class="page-title">'. $title .'</h2>'; } ?>
          <?php print render($title_suffix); ?>

          <?php if($tabs) { ?>
          <div class="tabs"><?php print render($tabs); ?></div>
          <?php } ?>

          <?php if($messages) { ?>
          <div class="wrapper-messages"><?php print $messages; ?></div><!-- .messages -->
          <?php } ?>

          <?php if($page['help']) { ?>
          <div class="help"><?php print render($page['help']); ?></div><!-- .help -->
          <?php } ?>

          <?php if($action_links){ ?>
          <div class="action-links">
            <ul><?php print render($action_links); ?></ul>
          </div><!-- .action-links -->
          <?php } ?>

          <?php print render($page['content']); ?> 
          
          <?php if($feed_icons) { ?>
          <div class='rss'><?php print $feed_icons ?></div>
          <?php } ?>
          
        </div><!-- .content -->

        <?php if($page['content_bottom']) { ?>
        <div class="content-bottom"><?php print render($page['content_bottom']); ?></div><!-- .content-bottom -->
        <?php } ?>

      </div><!-- .wrapper-content -->

      <?php if($page['sidebar_first']) { ?>
      <aside class="first_sidebar sidebar" role="complementary"><?php print render($page['sidebar_first']); ?> </aside><!-- .sidebar-first -->
      <?php } ?>
        
      <?php if($page['sidebar_second']){ ?>
      <aside class="second_sidebar sidebar" role="complementary"> <?php print render($page['sidebar_second']) ?> </aside><!-- .sidebar-second -->
      <?php } ?>
      
    </div>
    
    <?php if($page['postscript']) { ?>
    <div class="postscript"><?php print render($page['postscript']); ?></div><!-- .postscript -->
    <?php } ?>
    
    <?php if($page['footer_top'] || $page['footer'] || $page['footer_bottom']) { ?>
    
    <footer class="footer-page" role="contentinfo">
      
      <?php if($page['footer_top']) { ?>
      <div class="footer-top"><?php print render($page['footer_top']); ?></div> <!-- .footer-top -->
      <?php } ?>
      
      <?php if($page['footer']) { ?>
      <div class="footer-middle"><?php print render($page['footer']); ?></div><!-- .footer-middle -->
      <?php } ?>
      
      <?php if($page['footer_bottom']) { ?>
      <div class="footer-bottom"><?php print render($page['footer_bottom']); ?> </div><!-- .footer-bottom -->
      <?php } ?>
       
    </footer><!-- .footer-page -->
    
    <?php } ?>
    
  </div><!-- .container -->   

</div><!-- .wrapper-container -->

<?php if($page['outside_bottom']) { ?>
<div class="outside-bottom"><?php print render($page['outside_bottom']); ?></div><!-- .outside-bottom -->
<?php } ?>
