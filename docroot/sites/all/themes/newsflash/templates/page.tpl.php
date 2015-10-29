<?php

/**
 * @file
 * NewsFlash page.tpl.php
 *
 * for Default theme implementation see module/system/page.tpl.php
 *
 */
?>
<!-- page -->
<div id="page">
  <!-- header -->
  <div id="header" class="clearfix">
      <?php if ($logo): ?>
      <div id="logo-title">
      <!-- logo -->
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"> <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" /> </a>
      <!-- /logo -->
      </div>
      <?php endif; ?>
      <div id="name-and-slogan"><!-- name and sloagan -->
        <?php if ($site_name): ?>
        <!-- site-name -->
          <h1 class='site-name'> <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"> <?php print $site_name; ?> </a> </h1>
        <!-- /site-name -->
        <?php endif; ?>
        <?php if ($site_slogan): ?>
          <!-- slogan -->
          <div class='site-slogan'>
          <?php print $site_slogan; ?>
          </div>
          <!-- /slogan -->
        <?php endif; ?>
      <!-- /name and sloagan -->
      </div>
      <?php if ($page['header']): ?>
      <!-- header-region -->
        <div id="clear-header"> </div>
        <?php print render($page['header']); ?>
      <!-- /header-region -->
      <?php endif; ?>
      <?php if($page['search_box']): ?>
      <!-- Searchbox-region-->
        <div id="searchbox">
          <?php print render($page['search_box']); ?>
        </div>
      <!-- /Searchbox-region -->
      <?php endif; ?>
      <?php if ($main_menu || $secondary_menu): ?>
        <!-- navigation main/secondary menu -->
        <div id="primary-secondary-menu">
          <?php if ($main_menu) : ?>
            <div id="primarymenu">
            <!-- main menu -->
                <?php print theme(
                'links__system_main_menu',
                array(
                'links' =>
                $main_menu,
                'attributes' => array(
                'class' => array(
                'links primary-links',
                )),
                ));
                ?>
            <!-- /main menu -->
            </div>
          <?php endif; ?>
          <?php if ($secondary_menu) : ?>
            <div id="secondarymenu">
            <!-- secondary menu -->
              <?php print theme(
                'links__system_secondary_menu',
                array('links' => $secondary_menu,
                'attributes' =>
                array(
                'class' =>
                array(
                'links secondary-links',
                )),
                ));
                ?>
            <!-- secondary menu -->
            </div>
          <?php endif; ?>
        </div>
        <!-- /navigation main/secondary menu -->
      <?php endif; ?>
      <?php if ($page['suckerfish']): ?>
      <!-- suckerfish menu -->
        <div style="clear:both"> </div>
        <div id="suckerfishmenu" class="clearfix">
        <?php print render($page['suckerfish']); ?>
        </div>
      <!-- /suckerfish menu -->
      <?php endif; ?>
  </div>
  <!-- /header -->
  <?php
    $section1count = 0;
    if ($page['user1']): $section1count++; endif;
    if ($page['user2']): $section1count++; endif;
    if ($page['user3']): $section1count++; endif;
  ?>
  <?php if ($section1count): ?>
  <!--user menu 1-3 -->
    <?php $section1width = 'width' . floor(99 / $section1count); ?>
    <div class="clearfix clr" id="section1">
      <div class="sections">
        <?php if ($page['user1']): ?>
          <div class="section <?php echo $section1width ?>"><?php print render($page['user1']); ?></div>
        <?php endif; ?>
        <?php if ($page['user2']): ?>
          <div class="section <?php echo $section1width ?>"><?php print render($page['user2']); ?></div>
        <?php endif; ?>
        <?php if ($page['user3']): ?>
          <div class="section <?php echo $section1width ?>"><?php print render($page['user3']); ?></div>
        <?php endif; ?>
        <div style="clear:both"></div>
      </div>
    </div>
  <!-- /user menu 1-3 -->
  <?php endif; ?>
  <!-- middle-container -->
  <div id="middlecontainer">
    <?php if ($page['sidebar_first']) { ?>
    <!-- sidebar-left -->
      <div id="sidebar-left">
      <?php print render($page['sidebar_first']) ?>
      </div>
    <!-- /sidebar-left -->
    <?php } ?>
    <!-- main -->
    <div id="main">
      <!-- sequeeze -->
      <div id="squeeze">
        <?php if (theme_get_setting('newsflash_breadcrumb')): ?>
          <?php if ($breadcrumb): ?>
          <!-- bredcrumb -->
            <div id="breadcrumb">
            <?php print $breadcrumb; ?>
            </div>
          <!-- /bredcrumb -->
          <?php endif; ?>
        <?php endif; ?>
        <?php if ($page['mission']): ?>
        <!-- mission -->
          <div id="mission">
            <?php print render($page['mission']); ?>
          </div>
        <!-- /mission -->
        <?php endif; ?>
        <!-- sequeeze-content -->
        <div id="squeeze-content">
          <!-- inner-content -->
          <div id="inner-content">
            <?php if ($page['highlighted']): ?>
            <!-- highlighed -->
              <div id="highlighted">
                <?php print render($page['highlighted']); ?>
              </div>
            <!-- /highlighed -->
            <?php endif; ?>
            <?php if ($page['content_top']):?>
            <!-- content top -->
              <div id="content-top">
                <?php print render($page['content_top']); ?>
              </div>
            <!-- /content top -->
            <?php endif; ?>
            <!-- messages -->
            <?php print render($messages); ?>
            <!-- /messages -->
            <?php if ($title): ?>
            <!-- title -->
              <div id="branding" class="clearfix">
                <?php print render($title_prefix); ?>
                <h1 <?php print $title_attributes; ?>><?php print render($title); ?></h1>
                <?php print render($title_suffix); ?>
              </div>
            <!-- /title -->
            <?php endif; ?>
            <?php if ($tabs): ?>
            <!-- tabs -->
              <div class="tabs">
                <?php print  render($tabs) ?>
              </div>
            <!-- /tabs -->
            <?php endif; ?>
            <?php if ($page['help']): ?>
            <!-- Help region -->
              <?php print render($page['help']); ?>
            <!-- /Help region -->
            <?php endif; ?>
            <?php if ($action_links): ?>
            <!-- action links -->
              <ul class="action-links">
                <?php print render($action_links); ?>
              </ul>
            <!-- /action links -->
            <?php endif; ?>
            <!-- content -->
            <?php print render($page['content']); ?>
            <!-- /content -->
            <?php /* print $feed_icons; */ ?>
            <?php if ($page['content_bottom']): ?>
            <!-- content bottom -->
              <div id="content-bottom">
                <?php print render($page['content_bottom']); ?>
              </div>
            <!-- /content bottom-->
            <?php endif; ?>
          </div>
          <!-- /inner-content -->
        </div>
        <!-- /squeeze-content -->
      </div>
      <!-- /squeeze -->
    </div>
    <!-- /main -->
    <?php if ($page['sidebar_second']) { ?>
    <!-- sidebar-right -->
      <div id="sidebar-right">
      <?php print render($page['sidebar_second']) ?>
      </div>
    <!-- /sidebar-right -->
    <?php } ?>
  </div>
  <!-- /middle-container -->
  <div style="clear:both"></div>
  <?php
  $section2count = 0;
    if ($page['user4']): $section2count++; endif;
    if ($page['user5']): $section2count++; endif;
    if ($page['user6']): $section2count++; endif;
  ?>
  <?php if ($section2count): ?>
  <!-- user menu 4-6 -->
    <?php $section2width = 'width' . floor(99 / $section2count); ?>
    <div class="clearfix clr" id="section2">
      <div class="sections">
        <?php if ($page['user4']): ?>
          <div class="section <?php echo $section2width ?>"><?php print render($page['user4']); ?></div>
        <?php endif; ?>
        <?php if ($page['user5']): ?>
          <div class="section <?php echo $section2width ?>"><?php print render($page['user5']); ?></div>
        <?php endif; ?>
        <?php if ($page['user6']): ?>
          <div class="section <?php echo $section2width ?>"><?php print render($page['user6']); ?></div>
        <?php endif; ?>
      </div>
      <div style="clear:both"></div>
    </div>
  <!-- user menu 4-6 -->
  <?php endif; ?>
  <?php if ($page['footer'] || $page['footer_message'] || (theme_get_setting('newsflash_banner'))): ?>
    <div id="footer">
      <?php if ($page['footer']): ?>
        <div id="footer-region">
          <?php print render($page['footer']) ?>
        </div><!-- /footer -->
      <?php endif; ?>
      <?php if ($page['footer_message']): ?>
        <div id="footer-message">
          <?php print render($page['footer_message']) ?>
        </div><!-- /footer message -->
      <?php endif; ?>
      <?php if (theme_get_setting('newsflash_banner')): ?>
        <div class="rooplelogo">
          <?php if (theme_get_setting('newsflash_themelogo')) { ?>
            <?php $logo_path = base_path() . path_to_theme() . "/images/" . get_newsflash_style(); ?>
            <a href="http://www.roopletheme.com" title="RoopleTheme!" target="_blank"><img src="<?php print $logo_path . '/RoopleThemeLogo.png'; ?>" alt="RoopleTheme!"/></a>
          <?php }
          else { ?>
            <a href="http://www.roopletheme.com" title="RoopleTheme!" target="_blank"><img src="<?php print base_path() . path_to_theme() . '/RoopleThemeLogo.png'; ?>" alt="RoopleTheme!"/></a>
          <?php } ?>
        </div><!-- /rooplelogo -->
      <?php endif; ?>
    </div>
  <?php endif; ?>
</div>
<!-- /page NF VER 2.5-->
