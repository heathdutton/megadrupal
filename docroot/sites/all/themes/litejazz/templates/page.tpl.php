<?php

/**
 * @file
 * LiteJazz page.tpl.php
 * 
 * for Default theme implementation to display a block see modules/system/page.tpl.php.
 */
?>
<div id="page">
  <div id="masthead">
    <div id="header" class="clearfix">
      <div class="header-right">
        <div class="header-left">
      <?php if($page['search_box']): ?>
      <!-- Searchbox-region-->
        <div id="searchbox">
        <?php print render($page['search_box']); ?>
        </div> 
      <!-- /Searchbox-region -->
      <?php endif; ?>
          <?php if ($logo): ?>
            <div id="logo-title">
                <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>"> 
                <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" /> 
                </a>
            </div>
          <?php endif; ?>
          <!-- /logo-title -->
          <?php if ($site_name || $site_slogan): ?>
            <div id="name-and-slogan">
              <?php if ($site_name): ?>
                <h1 id='site-name'> 
                  <a href="<?php print $front_page ?>" title="<?php print t('Home'); ?>"> 
                    <?php print $site_name; ?> 
                  </a> 
                  </h1>
              <?php endif; ?>
              <?php if ($site_slogan): ?>
                <div id='site-slogan'> 
                  <?php print $site_slogan; ?> 
                </div>
              <?php endif; ?>
            </div>
         <?php endif; ?>
         <!-- /name-and-slogan -->
            <?php if ($page['header']): ?>
              <div style="clear:both"></div>
              <?php print render($page['header']); ?>
            <?php endif; ?>
          </div>
          <!-- /header-left -->
        </div>
        <!-- /header-right -->
      </div>
      <!-- /header -->
    </div>
    <div id="navigation" class="menu <?php if ($main_menu) { print "withprimary"; } if ($secondary_menu) { print " withsecondary"; } ?> ">
      <?php if ($main_menu || $secondary_menu): ?>
        <!-- navigation main/secondary menu -->
        <div id="primarymenu" class="clearfix">
          <?php if ($main_menu) : ?>
            <!-- main menu -->
            <?php print theme(
              'links__system_main_menu',
              array(
                'links' => 
                $main_menu,
                'attributes' => array(
                  'id' => array('primary'), 
                  //'primary-links',
                  'class' => array(
                  'menu',
                  //'links primary-links',
                  //'inline',
                  'clearfix'
                  )),
                //'heading' => 
                //t('Main menu')
              ));
            ?>
            <!-- /main menu -->
          <?php endif; ?>
        </div>
            
      <div id="secondarymenu" class="clearfix"> 
        <?php if ($secondary_menu) : ?>
          <!-- secondary menu -->
          <?php print theme(
          'links__system_secondary_menu',
          array('links' => $secondary_menu,
          'attributes' => 
          array(
          'id' => array('secondary'),
          //'secondary-links',
          'class' => 
          array(
          'links secondary-links',
          //'inline', 
          'clearfix'
          )),
          //'heading' => 
          //t('Secondary menu')
          ));
          ?>
          <!-- secondary menu -->
        <?php endif; ?>
      </div>
            
      <?php endif; ?>
    </div>
    <?php if ($page['suckerfish']): ?>
      <div id="suckerfishmenu" class="clearfix">
        <?php print render($page['suckerfish']); ?>
      </div>
    <?php endif; ?>
    <!-- /navigation -->
    <?php
      $section1count = 0;
      if ($page['user1'])  { $section1count++; }
      if ($page['user2'])  { $section1count++; }
      if ($page['user3'])  { $section1count++; }
    ?>
    <?php if ($section1count): ?>
      <?php $section1width = 'width' . floor(99 / $section1count); ?>
      <div class="clearfix clr" id="section1">
        <div class="sections">
          <?php if ($page['user1']): ?>
            <div class="section <?php echo $section1width ?>">
              <?php print render($page['user1']); ?>
            </div>
          <?php endif; ?>
          <?php if ($page['user2']): ?>
            <div class="section <?php echo $section1width ?>">
              <?php print render($page['user2']); ?>
            </div>
          <?php endif; ?>
          <?php if ($page['user3']): ?>
            <div class="section <?php echo $section1width ?>">
              <?php print render($page['user3']); ?>
            </div>
          <?php endif; ?>
        </div>
        <div style="clear:both">
        </div>
      </div>
      <!-- /section1 -->
    <?php endif; ?>
    <div class="clearfix" id="middlecontainer">
      <?php if ($page['sidebar_first']) { ?>
        <div id="sidebar-left"><?php print render($page['sidebar_first']) ?> </div>
      <?php } ?>
      <div id="main">
        <div id="squeeze">
          <?php if (theme_get_setting('litejazz_breadcrumb')): ?>
            <?php if ($breadcrumb): ?>
              <div id="breadcrumb">
                <?php print $breadcrumb; ?> 
              </div>
            <?php endif; ?>
          <?php  endif; ?>
        <?php  if ($page['highlighted']) { ?>
          <div id="mission">
            <?php  print render($page['highlighted']) ?>
          </div>
        <?php } ?>

        <?php if ($page['content_top']):?>
          <div id="content-top">
            <?php print render($page['content_top']); ?>
          </div>
        <?php endif; ?>

        <?php  if ($show_messages) { print render($messages); } ?> 
        <?php if ($title): ?>
        <!-- title -->
          <div id="branding" class="clearfix">
            <?php print render($title_prefix); ?>
            <h1 class="title"><?php print render($title); ?></h1>
            <?php print render($title_suffix); ?>
          </div>
        <!-- /title -->
        <?php endif; ?>
        <?php if ($tabs): ?>
        <div class="tabs"><?php print render($tabs) ?></div>
        <?php endif; ?>
        <?php print render($page['help']); ?>

        <?php if ($action_links): ?>
          <!-- action links -->
          <ul class="action-links">
            <?php print render($action_links); ?>
          </ul>
          <!-- /action links -->
        <?php endif; ?>
        <?php print render($page['content']); ?> 
        <?php print $feed_icons; ?>
        <?php if ($page['content_bottom']): ?>
          <div id="content-bottom">
            <?php print render($page['content_bottom']); ?>
          </div>
        <?php endif; ?>
      </div>
    </div>
    <?php if ($page['sidebar_second']) { ?>
        <div id="sidebar-right"><?php print render($page['sidebar_second']); ?> </div>
    <?php } ?>
  </div>
  <div style="clear:both">
</div>

<?php
  $section2count = 0;
  if ($page['user4'])  { $section2count++; }
  if ($page['user5'])  { $section2count++; }
  if ($page['user6'])  { $section2count++; }
?>
<?php if ($section2count): ?>
  <?php $section2width = 'width' . floor(99 / $section2count); ?>
  <div class="clearfix clr" id="section2">
    <div class="sections">
      <?php if ($page['user4']): ?>
        <div class="section <?php echo $section2width ?>">
          <?php print render($page['user4']); ?>
        </div>
      <?php endif; ?>
      <?php if ($page['user5']): ?>
        <div class="section <?php echo $section2width ?>">
          <?php print render($page['user5']); ?>
        </div>
      <?php endif; ?>
        <?php if ($page['user6']): ?>
        <div class="section <?php echo $section2width ?>">
          <?php print render($page['user6']); ?>
        </div>
      <?php endif; ?>
    </div>
    <div style="clear:both">
    </div>
  </div>
<!-- /section2 -->
<?php endif; ?>

<?php if ($page['footer'] || $page['footer_message'] || theme_get_setting('litejazz_banner')): ?>
  <div id="footer"> 
    <?php if ($page['footer']) { ?>
      <div id="footer-region">
        <?php print render($page['footer']);?>
      </div>
    <?php } ?>
      <?php if ($page['footer_message']) { ?>
    <div id="footer-message">
        <?php print render($page['footer_message']) ?>
    </div><!-- /footer message -->
      <?php  } ?>
      <?php if (theme_get_setting('litejazz_banner')): ?>
      <div class="rooplelogo">
      <?php if (theme_get_setting('litejazz_themelogo')) { ?>
      <a href="http://www.roopletheme.com" title="RoopleTheme!" target="_blank">
        <img src="<?php print base_path() . path_to_theme() . '/images/' . get_litejazz_style() . '/roopletheme.png'; ?>" alt="RoopleTheme!"/>
      </a> 
      <?php } else { ?>
      <a href="http://www.roopletheme.com" title="RoopleTheme!" target="_blank">
        <img src="<?php print base_path() . path_to_theme() . '/roopletheme.png'; ?>" alt="RoopleTheme!"/>
      </a> 
      <?php } ?>
      </div><!-- /rooplelogo -->
      <?php endif; ?>
    </div>
<?php endif; ?>
    <div id="footer-wrapper" class="clearfix">
      <div class="footer-right">
        <div class="footer-left"> 
        </div>
        <!-- /footer-left -->
      </div>
      <!-- /footer-right -->
    </div>
<!-- /footer-wrapper -->
<?php // print $closure ?> </div>

