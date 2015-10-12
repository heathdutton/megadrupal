<?php
/**
 * @file
 * Insurance theme's implementation to display a single Drupal page.
 */
 global $base_url;
 $header_style = '';
 $header_bg_file = theme_get_setting('header_bg_file');  
 if ($header_bg_file) {
  $header_style .= 'filter:;background: url(' . $header_bg_file . ') repeat ';
  $header_style .= theme_get_setting('header_bg_alignment') . ';';
 }
 drupal_add_js(drupal_get_path('theme', 'insurance') .'/js/jquery.bxslider.min.js');
 drupal_add_css(drupal_get_path('theme', 'insurance') . '/css/jquery.bxslider.css'); 
?>
<div id="page" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <!-- ______________________ HEADER _______________________ -->
  
  <header id="header" style="">

    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/>
      </a>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <hgroup id="name-and-slogan">

        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
            </div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>

      </hgroup>
    <?php endif; ?>

    <?php if ($page['header']): ?>
      <div id="header-region">
        <?php print render($page['header']); ?>
      </div>
    <?php endif; ?>

      <!-- customize social links code -->
      <?php if (theme_get_setting('socialicon_display', 'insurance')): ?>
      <?php 
      $twitter_url = check_plain(theme_get_setting('twitter_url', 'insurance')); 
      $facebook_url = check_plain(theme_get_setting('facebook_url', 'insurance')); 
      $linkedin_url = check_plain(theme_get_setting('linkedin_url', 'insurance')); 
      $theme_path_social = base_path() . drupal_get_path('theme', 'insurance');
      ?>
      <div id="socialbar">
        <ul class="social">
      <?php if ($facebook_url): ?><li> <a href="<?php print $facebook_url; ?>" target="_blank"> <img src="<?php print $theme_path_social; ?>/images/facebook.png"> </a> </li> <?php endif; ?>
      <?php if ($twitter_url): ?><li> <a href="<?php print $twitter_url; ?>" target="_blank"> <img src="<?php print $theme_path_social; ?>/images/twitter.png"> </a> </li> <?php endif; ?>
      <?php if ($linkedin_url): ?><li> <a href="<?php print $linkedin_url; ?>" target="_blank"> <img src="<?php print $theme_path_social; ?>/images/in.png"> </a> </li> <?php endif; ?>
      <li> <a href="<?php print $front_page; ?>rss.xml"> <img src="<?php print $theme_path_social; ?>/images/rss.png"> </a> </li>
        </ul>
      </div>
      <?php endif; ?>
      <!-- customize social links code -->  

  </header> <!-- /header -->

  <?php if ($main_menu || $secondary_menu): ?>
    <nav id="navigation" class="menu <?php if (!empty($main_menu))
     {print "with-primary";}
      if (!empty($secondary_menu))
     {print " with-secondary";} ?>">
      <?php print theme('links', array('links' => $main_menu, 'attributes' => array('id' => 'primary', 'class' => array('links', 'clearfix', 'main-menu')))); ?>
      <?php print theme('links', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary', 'class' => array('links', 'clearfix', 'sub-menu')))); ?>
    </nav> <!-- /navigation -->
  <?php endif; ?>
 <!--code to print the image in region-->
   <!-- ______________________ MAIN _______________________ -->

  <div id="main" class="clearfix">

    <section id="content">

        <?php if ($breadcrumb || $title|| $messages || $tabs || $action_links): ?>
          <div id="content-header">       
            <?php print $breadcrumb; ?>             
             <div id="banner_img">
              <ul class="bxslider">
              <?php
              $no_of_images = theme_get_setting('slider_images_count')+1;
              for($i=1;$i<=$no_of_images;$i++){
               $image = theme_get_setting('slider_image_'.$i , 'insurance') ;
               $title = (theme_get_setting('slider_image_title_'.$i , 'insurance')) ? theme_get_setting('slider_image_title_'.$i , 'insurance') : '';
               $url = theme_get_setting('slider_image_file_'.$i , 'insurance');
               if($image){
                if($url)
                 echo '<li><a href ="'.$url.'"><img title = "'.$title.'" width="600" height="200" src= "'.$base_url."/".$image.'"></a></li>';
                else
                 echo '<li><img title = "'.$title.'" width="600" height="200" src= "'.$base_url."/".$image.'"></li>';
               }
              }            
              ?>
             </ul>
            </div>
            <?php if ($page['highlighted']): ?>
              <div id="highlighted"><?php print render($page['highlighted']) ?></div>
             

            <?php endif; ?>

            <?php print render($title_prefix); ?>

            <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?><img src="<?php print $base_path; ?>sites/all/themes/insurance/images/bullet.png"></h1>
            <?php endif; ?>

            <?php print render($title_suffix); ?>
            <?php print $messages; ?>
            <?php print render($page['help']); ?>

            <?php if ($tabs): ?>
              <div class="tabs"><?php print render($tabs); ?></div>
            <?php endif; ?>

            <?php if ($action_links): ?>
              <ul class="action-links"><?php print render($action_links); ?></ul>
            <?php endif; ?>

          </div> <!-- /#content-header -->
          <?php endif; ?>
           <div id="content-area">
          
          <?php print render($page['content']) ?>
         
        </div>

        <?php print $feed_icons; ?>

    </section> <!-- /content-inner /content -->

    <?php if ($page['sidebar_first']): ?>
      <aside id="sidebar-first" class="column sidebar first">
        <?php print render($page['sidebar_first']); ?>
      </aside>
    <?php endif; ?> <!-- /sidebar-first -->
    
    <?php if ($page['sidebar_second']): ?>
      <aside id="sidebar-second" class="column sidebar second">
        <?php print render($page['sidebar_second']); ?>
      </aside>
    <?php endif; ?> <!-- /sidebar-second -->
  </div> <!-- /main -->
    <div style="clear: both;"></div>
      <div id="content-bottom">
        <?php print render($page['content_bottom']) ?>
    </div>
    <!-- Custom Bottom Columns -->
      <?php if ($page['bottom_column_first'] | $page['bottom_column_second'] |
                $page['bottom_column_third'] ) { ?>
        <div id="bottom-columns">
          <?php print insurance_build_columns( array(
              render($page['bottom_column_first']),
              render($page['bottom_column_second']),
              render($page['bottom_column_third']),
            ));
          ?>
        </div> <!--/bottom-columns -->
      <?php } ?>    
      
  <!-- ______________________ FOOTER _______________________ -->

  <?php if ($page['footer']): ?>
    <footer id="footer">
      <?php print render($page['footer']); ?>
    </footer> <!-- /footer -->
  <?php endif; ?>

</div> <!-- /page -->
