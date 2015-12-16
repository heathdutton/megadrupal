<?php

/**
 * @file
 * WhiteJazz D7.x page.tpl.php  side calculatings in template.php
 *
 * for Default theme implementation see module/system/page.tpl.php
 *
 */
?>
<div id="page">
  <div id="masthead">
    <div id="header" class="clearfix">
      <div class="header-right">
        <div class="header-left">
          <?php if($page['search_box']): ?>
            <!-- Searchbox-region-->
            <div id="search">
              <?php print render($page['search_box']); ?>
            </div> 
            <!-- /Searchbox-region -->
          <?php endif; ?>
          <div id="logo-title">
            <?php if ($logo): ?>
              <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>"> <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" /> </a>
            <?php endif; ?>
          </div><!-- /logo-title -->
          <div id="name-and-slogan">
            <?php if ($site_name): ?>
              <h1 id='site-name'> <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>"> <?php print $site_name; ?> </a> </h1>
            <?php endif; ?>
            <?php if ($site_slogan): ?>
              <div id='site-slogan'> <?php print $site_slogan; ?> </div>
            <?php endif; ?>
          </div><!-- /name-and-slogan -->
          <?php if ($page['header']): ?>
            <div style="clear:both"></div>
            <?php print render($page['header']); ?>
          <?php endif; ?>
        </div><!-- /header-left -->
      </div><!-- /header-right -->
    </div><!-- /header -->
  </div>
  <?php if ($main_menu): ?>
    <div id="primary" class="clearfix"> <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('class' => array('links primary-links')))); ?> </div>
  <?php endif; ?>
  <?php if ($secondary_menu): ?>
    <div id="secondary" class="clearfix"> <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('class' => array('links secondary-links')))); ?> </div>
  <?php endif; ?>
  <?php if ($page['suckerfish']): ?>
    <div id="suckerfishmenu" class="clearfix">
      <?php print render($page['suckerfish']); ?>
    </div>
  <?php endif; ?>
  <?php
  $section1count = 0;
  if ($page['user1']) { 
    $section1count++; 
  }
  if ($page['user2']) { 
    $section1count++; 
  }
  if ($page['user3']) {
    $section1count++; 
  }
  ?>
  <?php if ($section1count): ?>
    <?php $section1width = 'width'. floor(99 / $section1count); ?>
    <div class="clearfix clr" id="section1">
      <div class="sections">
        <?php if ($page['user1']): ?>
          <div class="section user1 <?php echo $section1width ?>"><?php print render($page['user1']); ?></div>
        <?php endif; ?>
        <?php if ($page['user2']): ?>
          <div class="section user2 <?php echo $section1width ?>"><?php print render($page['user2']); ?></div>
        <?php endif; ?>
        <?php if ($page['user3']): ?>
          <div class="section user3 <?php echo $section1width ?>"><?php print render($page['user3']); ?></div>
        <?php endif; ?>
      </div>
      <div style="clear:both"></div>
    </div><!-- /section1 -->
  <?php endif; ?>
  <div id="middlecontainer">
    <?php if ($page['sidebar_first']) { ?>
      <div id="sidebar-left">
        <?php print render($page['sidebar_first']) ?>
      </div>
    <?php } ?>
    <div id="main">
      <div id="squeeze">
        <?php if (theme_get_setting('whitejazz_breadcrumb')): ?>
          <?php if ($breadcrumb): ?>
            <div id="breadcrumb"> <?php print $breadcrumb; ?> </div>
          <?php endif; ?>
        <?php endif; ?>
        <?php if ($page['mission']) { ?>
          <div id="mission"><?php print render($page['mission']) ?></div>
        <?php } ?>
        <?php if ($page['content_top']):?>
          <div id="content-top"><?php print render($page['content_top']); ?></div>
        <?php endif; ?>
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
        <?php if ($show_messages) { 
        print $messages; 
      } ?> 
        <?php print render($page['content']); ?> 
        <?php print $feed_icons; ?>
        <?php if ($page['content_bottom']): ?>
          <div id="content-bottom">
            <?php print render($page['content_bottom']); ?>
          </div>
        <?php endif; ?>
      </div><!--/squeeze-->
    </div><!--/main-->
    <?php if ($page['sidebar_second']) { ?>
      <div id="sidebar-right">
        <?php print render($page['sidebar_second']) ?>
      </div>
    <?php } ?>
  </div><!--middle-container-->
  <div style="clear:both"></div>
  <?php
  $section2count = 0;
  if ($page['user4']) {
    $section2count++; 
  }
  if ($page['user5']) {
    $section2count++; 
  }
  if ($page['user6']) {
    $section2count++; 
  }
  ?>
  <?php if ($section2count): ?>
    <?php $section2width = 'width'. floor(99 / $section2count); ?>
    <div class="clearfix clr" id="section2">
      <div class="sections">
        <?php if ($page['user4']): ?>
          <div class="section user4 <?php echo $section2width ?>"><?php print render($page['user4']); ?></div>
        <?php endif; ?>
          <?php if ($page['user5']): ?>
        <div class="section user5 <?php echo $section2width ?>"><?php print render($page['user5']); ?></div>
          <?php endif; ?>
        <?php if ($page['user6']): ?>
          <div class="section user6 <?php echo $section2width ?>"><?php print render($page['user6']); ?></div>
        <?php endif; ?>
      </div><!--sections-->
      <div style="clear:both"></div>
    </div><!-- /section2 -->
  <?php endif; ?>
  <div id="footer">
    <?php if ($page['footer_region']) { ?>
      <div id="footer-region">
      <?php print render($page['footer_region']) ?>
      </div>
    <?php } ?>
    <div id="footer-message">
      <?php if ($page['footer_message']) { ?>
        <?php print render($page['footer_message']) ?>
      <?php } ?>
      <?php if (theme_get_setting('whitejazz_rooplefooterlogo')): ?>
        <div class="rooplelogo"><a href="http://www.roopletheme.com" title="RoopleTheme!"><img src="<?php print base_path() . path_to_theme() ."/roopletheme.png"; ?>" alt="RoopleTheme!"/></a> <div>
      <?php endif; ?>
    </div><!--footer-message-->
  </div><!--footer-->
   <div id="footer-wrapper" class="clearfix">
    <div class="footer-right">
      <div class="footer-left"></div><!-- /footer-left -->
    </div><!-- /footer-right -->
  </div><!-- /footer-wrapper -->
</div>
