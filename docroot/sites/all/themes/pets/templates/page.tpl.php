<?php
/**
 * @file
 * pets theme's implementation to display a single Drupal page.
 */
?>
<?php
  global $base_url;
    $header_style = '';
    $header_bg_file = theme_get_setting('header_bg_file');
    $header_path = $base_url."/".$header_bg_file;
     if ($header_bg_file) {
      $header_style .= 'filter:;background: url(' . $header_path . ')repeat';
      $header_style .= theme_get_setting('header_bg_alignment') . ';';
    }
   $background_style = '';
   $bg_file = theme_get_setting('bg_file');  
  if ($bg_file) {
    $background_style .= 'filter:;background: url(' . $bg_file . ') repeat ';
  }

?>
<div id="page" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <!-- ______________________ HEADER _______________________ -->
  
  <header id="header" style="<?php echo $background_style;?>">

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
      <?php if (theme_get_setting('socialicon_display', 'pets')): ?>
      <?php 
      $twitter_url = check_plain(theme_get_setting('twitter_url', 'pets')); 
      $facebook_url = check_plain(theme_get_setting('facebook_url', 'pets')); 
      $linkedin_url = check_plain(theme_get_setting('linkedin_url', 'pets')); 
      $theme_path_social = base_path() . drupal_get_path('theme', 'pets');
      ?>
      <div id="socialbar">
        <ul class="social">
      <?php if ($twitter_url): ?><li> <a href="<?php print $twitter_url; ?>" target="_blank"> <img src="<?php print $theme_path_social; ?>/images/twitter.png"> </a> </li> <?php endif; ?>
      <?php if ($facebook_url): ?><li> <a href="<?php print $facebook_url; ?>" target="_blank"> <img src="<?php print $theme_path_social; ?>/images/facebook.png"> </a> </li> <?php endif; ?>
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
  <?php global $base_url;?>
   <!-- ______________________ MAIN _______________________ -->
  
  <div id="main" class="clearfix">

    <section id="content">
        <div id="contentimg" style="<?php echo $header_style;?>"></div> 
        <?php if ($breadcrumb || $title|| $messages || $tabs || $action_links): ?>
      
           <div id="content-header">
            <?php print $breadcrumb; ?>

            <?php if ($page['highlighted']): ?>
            <div id="highlighted"><?php print render($page['highlighted']) ?></div>
            <?php endif; ?>
            <?php print render($title_prefix); ?>
           
                <?php if ($page['content_column_first'] | $page['content_column_second'] |
                  $page['content_column_third'] ) { ?>
                  <div id="bottom-columns">
                  <?php print pets_build_columns( array(
                   render($page['content_column_first']),
                   render($page['content_column_second']),
                   render($page['content_column_third']),
                   ));
                  ?>
                 </div> <!--/bottom-columns -->
                <?php } ?>

            <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
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
            <div class="clear"></div>
          </div> <!-- /#content-header -->
        <?php endif; ?>
        <div class="clear"></div>
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

  <!-- ______________________ FOOTER _______________________ -->

  <?php if ($page['footer']): ?>
    <footer id="footer">
      <?php print render($page['footer']); ?>
    </footer> <!-- /footer -->
  <?php endif; ?>

</div> <!-- /page -->
