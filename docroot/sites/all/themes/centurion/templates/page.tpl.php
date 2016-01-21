<?php
/**
 * @file
 * page.tpl.php file for theme
 */
?>

<!-- header -->

<header id="pagetop" class="clearfix">
  <div class="container">
    <div class="grid-8 alpha">
      <?php if ($linked_logo_img): ?>
      <span id="logo"><?php print $linked_logo_img; ?></span> 
      <!-- end of logo -->
      <?php endif; ?>
      <?php if ($linked_site_name): ?>
      <h1 id="site-name"><?php print $linked_site_name; ?></h1>
      <!-- end of sitename -->
      <?php endif; ?>
      <?php if ($site_slogan): ?>
      <div id="site-slogan"><?php print $site_slogan; ?></div>
      <!-- site slogan -->
      <?php endif; ?>
    </div>
    <div class="grid-4 omega">
      <?php if ($secondary_menu): ?>
      <div id="secondary-menu"> <?php print $secondary_menu_links; ?> </div>
      <!-- end of secondary menu -->
      <?php endif; ?>
      <?php if ($page['search_box']): ?>
      <!-- search form -->
      <div id="search-box"><?php print render($page['search_box']); ?></div>
      <!-- end of search -->
      <?php endif; ?>
      <div class="clear"></div>
    </div>
  </div>
  <div class="clear"></div>
  <br />
</header>
<!-- end of header --> 

<!-- Subheader -->
<?php if ($page['highlighted']): ?>
<div class="container">

<!-- Messages and Help section --> 
<?php print $messages; ?> <?php print render($page['help']); ?> 
<!-- End ofMessages and Help section --> 

<!-- Site subheader -->
<div id="site-subheader" class="clearfix">
  <?php if ($page['highlighted']): ?>
  <div id="highlighted" class="<?php print centurion_chclass('grid-12', $page['header'], 6); ?>"> <?php print render($page['highlighted']); ?> </div>
  <?php endif; ?>
  <?php if ($page['header']): ?>
  <div id="header-region" class="<?php print centurion_chclass('grid-12', $page['highlighted'], 6); ?> clearfix"> <?php print render($page['header']); ?> </div>
  <?php endif; ?>
  <!-- End of Site subheader --> 
  
</div>
<?php endif; ?>
<!-- End of Subheader --> 

<!-- Navigation -->
<nav id="main" class="clearfix">
  <div class="container">
    <?php if ($main_menu): ?>
    <?php print $main_menu_links; ?>
    <?php endif; ?>
  </div>
</nav>
<!-- End of Navigation --> 

<!-- content -->
<div class="container">
  <div id="content" class="clearfix"> 
    
    <!-- main content area -->
    <section> <?php print $breadcrumb; ?>
      <section id="contentArea" class="clearfix">
        <?php if ($page['sidebar_left']): ?>
        <!-- left bar -->
        <div class="alpha <?php print centurion_chclass('grid-3', $page['sidebar_right'], 3) . ' ' . centurion_chclass('grid-3', !$page['sidebar_right'], 3); ?>">
          <aside class="leftBar"> <?php print render($page['sidebar_left']); ?> </aside>
        </div>
        <!-- end of left bar -->
        <?php endif; ?>
        
        <!-- page -->
        <div class="<?php print centurion_chclass('grid-12', $page['sidebar_left'], 3, $page['sidebar_right'], 3); ?>">
          <article id="mainContent"> <a id="main-content"></a> <?php print render($title_prefix); ?>
            <?php if ($title): ?>
            <h1 class="title"> <?php print $title; ?> </h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php if ($tabs): ?>
            <div class="tabs"> <?php print render($tabs); ?> </div>
            <div class="clear"></div>
            <?php endif; ?>
            <?php print render($page['content']); ?> <?php print $feed_icons; ?> </article>
        </div>
        <!--end of page --> 
        <!-- right bar -->
        <?php if ($page['sidebar_right']): ?>
        <div class="grid-3 omega">
          <aside id="rightBar"> <?php print render($page['sidebar_right']); ?> </aside>
        </div>
        <?php endif; ?>
        <!-- end of right bar --> 
      </section>
    </section>
    <!-- main content area -->
    <div class="clear"></div>
  </div>
  <!-- end of content --> 
</div>
<!-- end of container --> 

<!-- footer -->
<footer>
  <div class="container">
    <section id="bottomFooter">
      <div class="grid-4 alpha"> 
        <!-- footer area 1 -->
        <article> <?php print render($page['footer_block_1']); ?> </article>
        <!-- end of footer area 1 --> 
      </div>
      <div class="grid-4"> 
        <!-- footer area 2 -->
        <article> <?php print render($page['footer_block_2']); ?> </article>
        <!-- end of footer area 2 --> 
      </div>
      <div class="grid-4 omega"> 
        <!-- footer area 3 -->
        <article> <?php print render($page['footer_block_3']); ?> </article>
        <!-- end of footer area 3 --> 
      </div>
    </section>
    <div class="clear"></div>
    <!-- footer bottom -->
    <?php if ($page['footer_bottom']): ?>
    <div class="grid-12 alpha"> <?php print render($page['footer_bottom']); ?> </div>
    <?php endif; ?>
    <!-- end of footer bottom --> 
    
  </div>
</footer>
<!-- end of footer -->
