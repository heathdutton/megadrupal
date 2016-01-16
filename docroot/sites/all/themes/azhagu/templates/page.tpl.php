<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 */
?>
<div id="wrapper"> <!-- wrapper starts here -->

  <div id="header-wrapper"> <!-- Header-wrapper starts here-->
    <!-- Branding -->
    <div id="branding">
      <div id="site-logo">
        <?php if ($logo): ?>
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>
      </div>
      
       <?php if ($site_name || $site_slogan): ?>
        <div id="name-and-slogan">
            <?php if ($site_name): ?>
            <?php if ($title): ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
             <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
            <?php endif; ?>
            <?php endif; ?>

             <?php if ($site_slogan): ?>     
              <span id="site-slogan"><?php print $site_slogan; ?></span>
            <?php endif; ?>            
    
        </div>
    
      <?php endif; ?>

       <?php print render($page['header']); ?>
    </div>      

    <!-- Navigation -->
      <div id="main-menu" class="navigation">
        <div id="navigation">
          <?php  print render($main_menu_expanded); ?>  
        </div>
      </div>
   </div> <!-- Header-wrapper ends here -->

  <?php if (!empty($messages)): print $messages; endif; ?>

  <div id="content-wrapper"> <!-- content-wrapper starts here -->
    <div id="breadcrumb" class="breadcrumb"><?php print $breadcrumb; ?></div>
    <div id="primary">
      <!-- content -->
      <div id="content-section">
        <div class="content">
        
        <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
        <a id="main-content"></a>
        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        <div id="header-meta">
          <?php if (isset($node->type) &&  $node->type != 'article'): ?>
          <div class="title-meta">
            <?php print render($title_prefix); ?>
          
            <?php if ($title): ?><h2 class="title" id="page-title"><?php print $title; ?></h2><?php endif; ?>
            <?php print render($title_suffix); ?>

        </div>
        <?php endif; ?>
        </div>
        <?php print render($page['content']); ?>

        </div>
      </div>
      <!-- sidebar first -->
      <?php if ($page['sidebar_first']): ?>
      <div id="sidebar-first">
        <?php print render($page['sidebar_first']); ?>
      </div>
      <?php endif; ?>
    </div>
  </div><!-- content-wrapper ends here -->

  <div id="footer-wrapper"> <!-- footer-wrapper starts here -->
    <div class='footer-region-first'>
      <div class="footer-first footer-columns">
         <?php if (theme_get_setting('social_block', 'azhagu')): ?>
            <h2 class="title">
              <?php if($social_block_title): ?>
              <?php print $social_block_title; ?>
              <?php elseif(!$social_block_title): ?>
              <?php print $default_title; ?>
              <?php endif; ?>
            </h2>
            <ul class="social">
                <?php if ($facebook_url): ?><li>
                  <a target="_blank" title="<?php print $site_name; ?> in Facebook" href="<?php print $facebook_url; ?>"><img alt="Facebook" src="<?php print $path . '/images/icons/icon-facebook.png'; ?>" /> </a>
                </li><?php endif; ?>
                <?php if ($twitter_url): ?><li>
                  <a target="_blank" title="<?php print $site_name; ?> in Twitter" href="<?php print $twitter_url; ?>"><img alt="Twitter" src="<?php print $path . '/images/icons/icon-twitter.png'; ?>" /> </a>
                </li><?php endif; ?>
                <?php if ($gplus_url): ?><li>
                  <a target="_blank" title="<?php print $site_name; ?> in Google+" href="<?php print $gplus_url; ?>"><img alt="Google+" src="<?php print $path . '/images/icons/icon-google.png'; ?>" /> </a>
                </li><?php endif; ?>
                <?php if ($pinterest_url): ?><li>
                  <a target="_blank" title="<?php print $site_name; ?> in Pinterest" href="<?php print $pinterest_url; ?>"><img alt="Pinterest" src="<?php print $path . '/images/icons/icon-pinterest.png'; ?>" /> </a>
                </li><?php endif; ?>
                <li>
                  <a target="_blank" title="<?php print $site_name; ?> in RSS" href="<?php print $front_page; ?>rss.xml"><img alt="RSS" src="<?php print $path . '/images/icons/icon-rss.png'; ?>" /> </a>
                </li>             
            </ul>
        <?php endif; ?>
             <?php print render($page['footer_firstcolumn']); ?>
      </div>
      <?php if ($page['footer_secondcolumn']): ?>
        <div class="footer-second footer-columns">
          <?php print render($page['footer_secondcolumn']); ?>
        </div>
      <?php endif; ?>
      <?php if ($page['footer_thirdcolumn']): ?>
        <div class="footer-third footer-columns">
          <?php print render($page['footer_thirdcolumn']); ?>       
        </div>
      <?php endif; ?>
    </div>
    <div class='footer-region-second'>
      <p>
        <?php print t('Copyright'); ?> &copy; <?php echo date("Y"); ?>, <a href="<?php print $front_page; ?>"><?php print $site_name; ?></a>. 
        <?php print t('Theme by'); ?>  <a href="http://www.drupal-responsive.com" target="_blank">Drupal Responsive</a>. 
        <?php print t('Icons by'); ?>  <a href="http://www.glyphicons.com" target="_blank">GLYPHICONS</a>. 
      </p>
    </div>
  </div><!-- footer-wrapper ends here -->
</div> <!-- wrapper ends here -->
