<?php

/**
 * @file page.tpl.php
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>

  <!--[if IE]>
    <link rel="stylesheet" href="<?php print $base_path . $directory; ?>/css/ie.css" type="text/css" />
  <![endif]-->
  <!--[if IE 6]>
    <link rel="stylesheet" href="<?php print $base_path . $directory; ?>/css/ie6.css" type="text/css" />
    <script type="text/javascript" src="<?php print $base_path . $directory; ?>/js/supersleight.js"></script>
  <![endif]-->
  <!--[if IE 7]>
    <link rel="stylesheet" href="<?php print $base_path . $directory; ?>/css/ie7.css" type="text/css" />
  <![endif]-->
</head>

<body id="bd">
  <!-- Header -->
  <div id="st_wrapper">
    <div class="container_16 clear-block">
      <div id="st_header" >
        <div id="st_logo_header" class="grid_4">
          <?php
          // Prepare header
          $site_fields = array();
          if ($site_name) {
            $site_fields[] = check_plain($site_name);
          }
          if ($site_slogan) {
            $site_fields[] = check_plain($site_slogan);
          }
          $site_title = implode(' ', $site_fields);

          if ($logo || $site_title) {
            print '<h1><a href="' . check_url($front_page) . '" title="' . $site_title . '">';
            if ($logo) {
              print '<img src="' . check_url($logo) . '" alt="' . $site_title . '" id="logo" />';
            }
            else {
              $logo_path = $base_path . $directory . '/images/logo.png';
              print '<img src="' . check_url($logo_path) . '" alt="' . $site_title . '" id="logo" />';
            }
            print '</a></h1>';
          }
        ?>
        </div>

        <!-- Main menu -->
        <div id="st_main_menu" class="grid_12">
          <?php if (isset($primary)) :
           print $primary;
          endif; ?>
        </div>
        <!-- End main menu -->
      </div> <!-- End header -->

      <!-- Slogan -->
      <?php if ($slider): ?>
      <div id="st_slogan" class="grid_16">
          <?php print $slider; ?>
      </div>
      <?php endif; ?>
      <!-- End slogan -->

      <!-- Main -->
      <div id="st_main">
        <?php $count_grid = 16;
          if ($left) $count_grid -= 3;
          if ($right) $count_grid -= 4;
        ?>
  
        <!-- Left -->
        <?php if ($left): ?>
        <div id="st_sidebar_left" class="grid_3">
          <?php print $left; ?>
        </div>
        <?php endif; ?>
        <!-- End left -->
                
        <!-- Main Content -->
        <div id="st_center" class="grid_<?php print $count_grid;?>">
          <div id="st_center_inner">
            <!-- Breadcumb -->
            <?php if (theme_get_setting('show_breadcrumb') and $breadcrumb and !$is_front) { ?>
            <div id="st_breadcrumb">
              <?php print($breadcrumb); ?>
            </div>
            <?php } ?>
            
            <?php if ($mission): print '<div id="mission">' . $mission . '</div>'; endif; ?>
            <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
            <?php if ($title): print '<h2' . ($tabs ? ' class="with-tabs"' : ' class="node-title"') . '>' . $title . '</h2>'; endif; ?>
            <?php if ($tabs): print $tabs . '</div>'; endif; ?>
            <?php if ($tabs2): print $tabs2; endif; ?>
            <?php if ($show_messages && $messages): print $messages; endif; ?>
            <?php print $help; ?>
    
            <div class="clear-block">
              <?php print $content; ?>
            </div>
            <?php print $feed_icons; ?>
          </div>
        </div>
        <!-- End content -->
  
        <!-- Right -->
        <?php if ($right): ?>
        <div id="st_sidebar_right" class="grid_4 omega">
          <?php print $right; ?>
        </div>
        <?php endif; ?>
        <!-- End right -->
      </div> 
      <!-- End Main -->
    </div>
  </div>
  <div id="st_wrapper_news">    
    <div class="container_16 clear-block">
      <!-- News -->
      <?php if ($news1 or $news2 or $news3 or $news4): ?>
      <?php 
      $count_news = 0;
      if ($news1) $count_news++;
      if ($news2) $count_news++;
      if ($news3) $count_news++;
      if ($news4) $count_news++;
      switch ($count_news) {
        case 1: 
          $grid_news_class = "grid_16";
          break;

        case 2:
          $grid_news_class = "grid_8";
          break;

        case 3:
        case 4:
          $grid_news_class = "grid_4";
          break;

        default:
          $grid_news_class = "grid_4";
          break;
      }
      ?>
      <div id="st_news">
        <!-- News1 -->
        <?php if ($news1): ?>
          <div id="st_news1" class="<?php print $grid_news_class;?>">
            <?php print $news1; ?>
          </div>
        <?php endif; ?>
        <!-- End News1-->
  
        <!-- News2 -->
        <?php if ($news2): ?>
          <div id="st_news2" class="<?php print $grid_news_class;?>">
            <?php print $news2; ?>
          </div>
        <?php endif; ?>
        <!-- End News2-->
  
        <!-- News3 -->
        <?php if ($news3): ?>
          <div id="st_news3" class="<?php if ($count_news == 3 and !$news4) print "grid_8 omega"; else print $grid_news_class; ?>">
            <?php print $news3; ?>
          </div>
        <?php endif; ?>
        <!-- End News3-->
  
        <!-- News4 -->
        <?php if ($news4): ?>
          <div id="st_news4" class="<?php if ($count_news == 3) print "grid_8"; else print $grid_news_class; ?> omega">
            <?php print $news4; ?>
          </div>
        <?php endif; ?>
        <!-- End News4-->
      </div>
      <?php endif; ?>
      <!-- End News -->

    </div>
  </div>
  <div id="st_wrapper_footer">    
    <div class="container_16 clear-block">
      <!-- Footer -->
      <div id="st_footer" class="grid_16">
        <div id="st_footer_logo">
          <?php
          // Prepare footer
          if ($site_name) {
            $site_fields[] = check_plain($site_name);
          }
        
          $logo_footer = theme_get_setting('show_logo_footer');
          if ($logo_footer || $site_title) {
            print '<h1><a href="' . check_url($front_page) . '" title="' . $site_title . '">';
            if ($logo_footer) {
              $logo_path = $base_path . $directory . '/images/logo-footer.png';
              print '<img src="' . check_url($logo_path) . '" alt="' . $site_title . '" id="logo" />';
            }
            print '</a></h1>';
          }
          ?>
        </div>
      
        <!-- Main menu -->
        <div id="st_menu_footer">
          <?php if (isset($footer_menu)): ?>
            <?php print $footer_menu; ?>
          <?php endif; ?>
        </div>
        <!-- End main menu -->
        <?php if ($copyright): ?>
        <div id="st_copyright" class="grid_16 alpha">
          <?php print $copyright; ?>
        </div>
        <?php endif; ?>
          </div>
          <!-- End main menu -->
        </div>
        <!-- End Footer -->
        
    </div>
    <!-- End container -->
  </div>  
  <!-- End st_wrapper_footer -->
  
  <?php print $closure; ?>

  </body>
</html>

