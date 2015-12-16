<?php

/**
 * @file
 * tapestry page.tpl.php
 * for Default theme implementation to display a block see modules/system/page.tpl.php.
 */
?>

  <div id="leaderboard">
    <?php if ($page['leaderboard']) { ?><?php print render($page['leaderboard']) ?><?php } ?>
  </div>

  <div id="header">
   <div id="headercontainer" class="clearfix">
    <?php if ($logo) { ?>
      <div class="site-logo">
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"> <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" /> </a>
      </div>
    <?php } ?>
      <?php if($page['search_box']): ?>
      <!-- Searchbox-region-->
        <div id="search">
        <?php print render($page['search_box']); ?>
        </div> 
      <!-- /Searchbox-region -->
      <?php endif; ?>
    <?php if ($site_name) { ?>
      <h1 class='site-name'><a href="<?php print $front_page; ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1>
    <?php } ?>
    <?php if ($site_slogan) { ?>
      <div class='site-slogan'><?php print $site_slogan ?></div>
  <?php } ?>
  <?php
    $headerregioncount = 0;
    if ($page['header_left']) { $headerregioncount++; }
    if ($page['header_center']) { $headerregioncount++; }
    if ($page['header_right']) { $headerregioncount++; }
  ?>
  <?php if ($headerregioncount): ?>
  <?php $headerregionwidth = 'width' . floor(99 / $headerregioncount); ?>
  <div class="clearfix" style="line-height:0;"></div>
  <div id="header-region-container" class="clearfix">
    <div id="header-region" class="clearfix">
    <?php if ($page['header_left']): ?>
    <div id="header-left" class="<?php echo $headerregionwidth ?>"><?php print render($page['header_left']); ?></div>
    <?php endif; ?>
    <?php if ($page['header_center']): ?>
    <?php if ($headerregioncount == 3) { ?>
    <div id="header-center" class="width34"><?php print render($page['header_center']); ?></div>
    <?php } else { ?>
    <div id="header-center" class="<?php echo $headerregionwidth ?>"><?php print render($page['header_center']); ?></div>
    <?php } ?>
    <?php endif; ?>
    <?php if ($page['header_right']): ?>
    <div id="header-right" class="<?php echo $headerregionwidth ?>"><?php print render($page['header_right']); ?></div>
    <?php endif; ?>
  </div>
    <!-- /header-region -->
  </div>
  <!-- /header-region-container -->
  <?php endif; ?>


  <?php if ($page['suckerfish']) { ?>
  <div class="clearfix" style="line-height:0;"></div>
  <div id="suckerfish-container"><div id="suckerfishmenu"><?php print render($page['suckerfish']) ?></div></div>
  <?php } ?>
   </div>
  </div>
  <div id="header-bottom" class="clearfix"></div>


<div id="outer-container">
<div id="page-right">
<div id="page-left">

  <?php if ($page['banner']): ?>
        <div id="banner">
               <?php print render($page['banner']); ?>
       </div>
  <?php endif; ?>

  <div id="container">

      <?php if (theme_get_setting('tapestry_breadcrumb')): ?>
         <?php if ($breadcrumb): ?>
             <div id="breadcrumb">
       
               <?php print ($breadcrumb); ?>
       </div>
         <?php endif; ?>
      <?php endif; ?>

  <?php if ($main_menu) {
    $pid = variable_get('menu_main_links_source', 'main-menu');
    $tree = menu_tree($pid);
    $tree = str_replace(' class="menu"','', $tree);
    $main_menu = drupal_render($tree); 
  ?>
  <div id="primary">
    <?php print $main_menu; ?>
  </div>
  <?php }else{$main_menu = FALSE;} ?>

<div class="clearfix"></div>
<div id="inside-content">
  <?php
         $region1count = 0;
         if ($page['user1']) { $region1count++; }
         if ($page['user2']) { $region1count++; }
         if ($page['user3']) { $region1count++; }
      ?>
  <?php if ($region1count): ?>
  <?php $region1width = 'width' . floor(99 / $region1count); ?>
  <div id="region1-container" class="clearfix">
    <div id="region1">
      <?php if ($page['user1']): ?>
      <div id="user1" class="<?php echo $region1width ?>"><?php print render($page['user1']); ?></div>
      <?php endif; ?>
      <?php if ($page['user2']): ?>
      <?php if ($region1count == 3) { ?>
      <div id="user2" class="width34"><?php print render($page['user2']); ?></div>
      <?php } else { ?>
      <div id="user2" class="<?php echo $region1width ?>"><?php print render($page['user2']); ?></div>
      <?php } ?>
      <?php endif; ?>
      <?php if ($page['user3']): ?>
      <div id="user3" class="<?php echo $region1width ?>"><?php print render($page['user3']); ?></div>
      <?php endif; ?>
    </div>
    <!-- /region1 -->
  </div>
  <!-- /region1-container -->
  <?php endif; ?>
  <?php
         $region2count = 0;
         if ($page['user4']) { $region2count++; }
         if ($page['user5']) { $region2count++; }
         if ($page['user6']) { $region2count++; }
      ?>
  <?php if ($region2count): ?>
  <?php $region2width = 'width' . floor(99 / $region2count); ?>
  <div id="region2-container" class="clearfix">
    <div id="region2">
      <?php if ($page['user4']): ?>
      <div id="user4" class="<?php echo $region2width ?>"><?php print render($page['user4']); ?></div>
      <?php endif; ?>
      <?php if ($page['user5']): ?>
      <?php if ($region2count == 3) { ?>
      <div id="user5" class="width34"><?php print render($page['user5']); ?></div>
      <?php } else { ?>
      <div id="user5" class="<?php echo $region2width ?>"><?php print render($page['user5']); ?></div>
      <?php } ?>
      <?php endif; ?>
      <?php if ($page['user6']): ?>
      <div id="user6" class="<?php echo $region2width ?>"><?php print render($page['user6']); ?></div>
      <?php endif; ?>
    </div>
    <!-- /region2 -->
  </div>
  <!-- /region2-container -->
  <?php endif; ?>
  <?php
         $region3count = 0;
         if ($page['user7']) { $region3count++; }
         if ($page['user8']) { $region3count++; }
         if ($page['user9']) { $region3count++; }
      ?>
  <?php if ($region3count): ?>
  <?php $region3width = 'width' . floor(99 / $region3count); ?>
  <div id="region3-container" class="clearfix">
    <div id="region3">
      <?php if ($page['user7']): ?>
      <div id="user7" class="<?php echo $region3width ?>"><?php print render($page['user7']); ?></div>
      <?php endif; ?>
      <?php if ($page['user8']): ?>
      <?php if ($region3count == 3) { ?>
      <div id="user8" class="width34"><?php print render($page['user8']); ?></div>
      <?php } else { ?>
      <div id="user8" class="<?php echo $region3width ?>"><?php print render($page['user8']); ?></div>
      <?php } ?>
      <?php endif; ?>
      <?php if ($page['user9']): ?>
      <div id="user9" class="<?php echo $region3width ?>"><?php print render($page['user9']); ?></div>
      <?php endif; ?>
    </div>
    <!-- /region3 -->
  </div>
  <!-- /region3-container -->
  <?php endif; ?>
  <div class="clearfix">
  <?php
    $sidebar_mode = theme_get_setting('tapestry_sidebarmode');
    if ($sidebar_mode == 'right') {
      if ($page['sidebar_second']) { ?><div id="sidebar-right"><?php print render($page['sidebar_second']) ?></div><?php }
      if ($page['sidebar_first']) { ?><div id="sidebar-left"><?php print render($page['sidebar_first']) ?></div><?php }
    } else {
      if ($page['sidebar_first']) { ?><div id="sidebar-left"><?php print render($page['sidebar_first']) ?></div><?php }
      if ($page['sidebar_second']) { ?><div id="sidebar-right"><?php print render($page['sidebar_second']) ?></div><?php }
    }
  ?>
  <div id="mainContent">
      <?php if ($page['mission']) { ?><div id="mission"><?php print render($page['mission']) ?></div><?php } ?>


<?php	$contenttopcountcount = 0;
    if ($page['content_top_left']) { $contenttopcountcount++; }
    if ($page['content_top_right']) { $contenttopcountcount++; }
    ?>
<?php if ($contenttopcountcount ) { ?>
  <?php $contenttopwidth = 'width' . floor(99 / $contenttopcountcount); ?>
  <div id="content-top" class="clearfix">
  <?php if ($page['content_top_left']) { ?><div id="content-top-left" class="<?php echo $contenttopwidth ?>"><?php print render($page['content_top_left']) ?></div><?php } ?>
  <?php if ($page['content_top_right']) { ?><div id="content-top-right" class="<?php echo $contenttopwidth ?>"><?php print render($page['content_top_right']) ?></div><?php } ?>
  </div>
<?php } ?>

            <?php if ($title): ?>
            <!-- title -->
              <div id="branding" class="clearfix">
                <?php print render($title_prefix); ?>
                <h1 class="title"><?php print render($title); ?></h1>
                <?php print render($title_suffix); ?>
              </div>
            <!-- /title -->
            <?php endif; ?>
        <div class="tabs"><?php print render($tabs) ?></div>
        <?php print render($page['help']) ?>
            <?php if ($action_links): ?>
            <!-- action links -->
              <ul class="action-links">
                <?php print render($action_links); ?>
              </ul>
            <!-- /action links -->
            <?php endif; ?>
        <?php if ($show_messages) { print $messages; } ?>
        <?php print render($page['content']); ?>

<?php	$contentbottomcountcount = 0;
    if ($page['content_bottom_left']) { $contentbottomcountcount++; }
    if ($page['content_bottom_right']) { $contentbottomcountcount++; }
    ?>
<?php if ($contentbottomcountcount ) { ?>
  <?php $contentbottomwidth = 'width' . floor(99 / $contentbottomcountcount); ?>
  <div id="content-bottom" class="clearfix">
  <?php if ($page['content_bottom_left']) { ?><div id="content-bottom-left" class="<?php echo $contentbottomwidth ?>"><?php print render($page['content_bottom_left']) ?></div><?php } ?>
  <?php if ($page['content_bottom_right']) { ?><div id="content-bottom-right" class="<?php echo $contentbottomwidth ?>"><?php print render($page['content_bottom_right']) ?></div><?php } ?>
  </div>
<?php } ?>

  <!-- end #mainContent --></div>
</div>


<?php
         $region4count = 0;
         if ($page['user10']) { $region4count++; }
         if ($page['user11']) { $region4count++; }
         if ($page['user12']) { $region4count++; }
      ?>
<?php if ($region4count): ?>
<?php $region4width = 'width' . floor(99 / $region4count); ?>
<div id="region4-container" class="clearfix">
  <div id="region4">
    <?php if ($page['user10']): ?>
    <div id="user10" class="<?php echo $region4width ?>"><?php print render($page['user10']); ?></div>
    <?php endif; ?>
    <?php if ($page['user11']): ?>
    <?php if ($region4count == 3) { ?>
    <div id="user11" class="width34"><?php print render($page['user11']); ?></div>
    <?php } else { ?>
    <div id="user11" class="<?php echo $region4width ?>"><?php print render($page['user11']); ?></div>
    <?php } ?>
    <?php endif; ?>
    <?php if ($page['user12']): ?>
    <div id="user12" class="<?php echo $region4width ?>"><?php print render($page['user12']); ?></div>
    <?php endif; ?>
  </div>
  <!-- /region4 -->
</div>
<!-- /region4-container -->
<?php endif; ?>

</div> <!-- /inside-content -->
<?php if ($page['sidebar_outside']) { ?>
<div id="sidebar-outside"><?php print render($page['sidebar_outside']) ?></div>
<?php } ?>
<div class="clearfix" style="clear: both;"></div>
<div class="page-bottom clearfix"></div>

<?php
         $region5count = 0;
         if ($page['user13']) { $region5count++; }
         if ($page['user14']) { $region5count++; }
         if ($page['user15']) { $region5count++; }
      ?>
<?php if ($region5count): ?>
  <div id="mastfoot" class="clearfix">
<?php $region5width = 'width' . floor(99 / $region5count); ?>
<div id="region5-container" class="clearfix">
  <div id="region5">
    <?php if ($page['user13']): ?>
    <div id="user13" class="<?php echo $region5width ?>"><?php print render($page['user13']); ?></div>
    <?php endif; ?>
    <?php if ($page['user14']): ?>
    <?php if ($region5count == 3) { ?>
    <div id="user14" class="width34"><?php print render($page['user14']); ?></div>
    <?php } else { ?>
    <div id="user14" class="<?php echo $region5width ?>"><?php print render($page['user14']); ?></div>
    <?php } ?>
    <?php endif; ?>
    <?php if ($page['user15']): ?>
    <div id="user15" class="<?php echo $region5width ?>"><?php print render($page['user15']); ?></div>
    <?php endif; ?>
  </div>
  <!-- /region5 -->
</div>
<!-- /region5-container -->
  </div>
  <div id="mastfoot-bottom" class="clearfix"></div>
<?php endif; ?>

  <div id="trailerboard">
  <div id="footer">
 <?php
    $footerregioncount = 0;
    if ($page['footer_left']) { $footerregioncount++; }
    if ($page['footer_center']) { $footerregioncount++; }
    if ($page['footer_right']) { $footerregioncount++; }
?>
        <?php if ($footerregioncount): ?>
        <?php $footerregionwidth = 'width' . floor(99 / $footerregioncount); ?>
        <div id="footer-region-container" class="clearfix">
    <div id="footer-region" class="clearfix">
                <?php if ($page['footer_left']): ?>
                <div id="footer-left" class="<?php echo $footerregionwidth ?>"><?php print render($page['footer_left']); ?></div>
                <?php endif; ?>
                <?php if ($page['footer_center']): ?>
                <?php if ($footerregioncount == 3) { ?>
                <div id="footer-center" class="width34"><?php print render($page['footer_center']); ?></div>
                <?php } else { ?>
                <div id="footer-center" class="<?php echo $footerregionwidth ?>"><?php print render($page['footer_center']); ?></div>
                <?php } ?>
                <?php endif; ?>
                <?php if ($page['footer_right']): ?>
                <div id="footer-right" class="<?php echo $footerregionwidth ?>"><?php print render($page['footer_right']); ?></div>
                <?php endif; ?>
        </div>
    <!-- /footer-region -->
        </div>
        <!-- /footer-region-container -->
        <?php endif; ?>
  <?php if ($page['footer_message']) { ?>
  <div id="footer-message">
  <?php print render($page['footer_message']) ?>
  </div>
  <?php } ?>
  <!-- end #footer --></div>

<?php if (theme_get_setting('tapestry_footerlogo')): ?>
<script type="text/javascript">
<!-- Hide Script
  function move_in(img_name,img_src) {
  document[img_name].src=img_src;
  }

  function move_out(img_name,img_src) {
  document[img_name].src=img_src;
  }
//End Hide Script-->
</script><div class="rooplelogo">
<?php
  $logopath = base_path() . path_to_theme() . '/images/' . get_tapestry_style() . '/roopletheme.png';
  $logorolloverpath = base_path() . path_to_theme() . '/images/' . get_tapestry_style() . '/roopletheme-rollover.png'; 
?>
<a href="http://www.roopletheme.com" onmouseover="move_in('rtlogo','<?php print $logorolloverpath ?>')" onmouseout="move_out('rtlogo','<?php print $logopath ?>')" title="RoopleTheme!" target="_blank"><img class="rtlogo" src="<?php print $logopath ?>" name="rtlogo" alt="RoopleTheme"/></a>
</div>
<?php endif; ?>
</div>

<!-- end #container --></div>
</div></div>

<div id="round-right">
<div id="round-left">
<div id="round-container">
</div></div></div>

</div>
<!-- RoopleTheme Tapestry Drupal 7/8 Dev Version by Alyx Vance -->
