<?php
/**
 * @file
 * Detamo's theme implementation to display a single Drupal page.
 *
 */
?>
 <div id="page" class="full clearfix">

  <div id="site-header" class="clearfix">
    <div id="branding" class="desktop-12 tablet-8 mobile-4 clearfix">
    <?php if ($linked_logo_img): ?>
      <span id="logo" class="desktop-3 tablet-3 mobile-4 "><?php print $linked_logo_img; ?></span>
    <?php endif; ?>
    <?php if ($linked_site_name): ?>
      <h1 id="site-name" class="desktop-9 tablet-5 mobile-4 "><?php print $linked_site_name; ?></h1>
    <?php endif; ?>
    <?php if ($site_slogan): ?>
      <div id="site-slogan" class="desktop-9 tablet-5 mobile-4 "><?php print $site_slogan; ?></div>
    <?php endif; ?>
  </div>

  <?php if ($main_menu_links || $secondary_menu_links): ?>
    <div id="site-menu" class="desktop-12 tablet-8 mobile-4">
      <?php print $main_menu_links; ?>
      <?php print $secondary_menu_links; ?>
    </div>
  <?php endif; ?>

  <?php if ($page['search_box']): ?>
    <div id="search-box" class="desktop-12 tablet-8 mobile-4 "><?php print render($page['search_box']); ?></div>
  <?php endif; ?>
  </div>

  <div id="site-subheader" class="clearfix">
  <?php if ($page['highlighted']): ?>
    <div id="highlighted" class="desktop-5 tablet-8 mobile-4">
      <?php print render($page['highlighted']); ?>
    </div>
  <?php endif; ?>

  <?php if ($page['header']): ?>
    <div id="header-region" class="region desktop-7 tablet-8 mobile-4 clearfix">
      <?php print render($page['header']); ?>
    </div>
  <?php endif; ?>
  </div>

  <div id="content-wrapper" class="clearfix">
  <div id="main" class="column desktop-push-3 desktop-7 tablet-8 mobile-4">
    <?php print $breadcrumb; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h1 class="title" id="page-title"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>      
    <?php if ($tabs): ?>
      <div class="tabs"><?php print render($tabs); ?></div>
    <?php endif; ?>
    <?php print $messages; ?>
    <?php print render($page['help']); ?>

    <div id="main-content" class="region clearfix">
      <?php print render($page['content']); ?>
    </div>

    <?php print $feed_icons; ?>
  </div>

  <?php if ($page['sidebar_first']): ?>
    <div id="sidebar-left" class="region desktop-pull-7 desktop-3 tablet-4 mobile-4">
      <?php print render($page['sidebar_first']); ?>
    </div>
  <?php endif; ?>

  <?php if ($page['sidebar_second']): ?>
    <div id="sidebar-right" class="region desktop-2 tablet-4 mobile-4">
      <?php print render($page['sidebar_second']); ?>
    </div>
  <?php endif; ?>
  </div>

  <?php if ($page['triple_one'] || $page['triple_two'] || $page['triple_three']): ?>
    <div id="triple-wrapper"><div id="triple" class="clearfix">
      <div id="triple-one" class="desktop-4 tablet-prefix-1 tablet-6 tablet-suffix-1 mobile-4 "> <?php print render($page['triple_one']); ?></div>
      <div id="triple-two" class="desktop-4 tablet-push-4 tablet-4 mobile-4 "><?php print render($page['triple_two']); ?> </div>
      <div id="triple-three" class="desktop-4 tablet-pull-4 tablet-4 mobile-4 "><?php print render($page['triple_three']); ?> </div>
    </div></div> <!-- /#triple, /#triple-wrapper -->
  <?php endif; ?>

  <?php if ($page['quad_one'] || $page['quad_two'] || $page['quad_three'] || $page['quad_four']): ?>
    <div id="quad-wrapper"><div id="quad" >
    <div id="quad-one" class="desktop-3 tablet-4 mobile-4 "> <?php print render($page['quad_one']); ?></div>
    <div id="quad-two" class="desktop-3 tablet-4 mobile-4 ">  <?php print render($page['quad_two']); ?></div>
    <div id="quad-three" class="desktop-3 tablet-4 tablet-clear mobile-4 "> <?php print render($page['quad_three']); ?></div>
    <div id="quad-four" class="desktop-3 tablet-4 mobile-4"> <?php print render($page['quad_four']); ?></div>
    </div></div> <!-- /#quad, /#quad-wrapper -->
  <?php endif; ?>

  <div id="footer" >
    <?php if ($page['footer']): ?>
      <div id="footer-region" class="desktop-12 tablet-8 mobile-4 clearfix">
        <?php print render($page['footer']); ?>
      </div>
    <?php endif; ?>
  </div>

  </div>
