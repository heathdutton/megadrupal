  <?php require_once ('includes/region_layout.php');?>
  <?php $show_breadcrumb = theme_get_setting('show_breadcrumb');?>    
  <?php $twitter_url = theme_get_setting('twitter_url');?>  
  <?php $facebook_url = theme_get_setting('facebook_url');?>       

  <div id="page-wrapper"><div id="page-wrapper-inner">

<!-- User Menu -->
     <?php if ($page['user_menu']): ?>
        <div id="user-menu"><div class="width-wrapper">	
					<div id="search-box">
					  <?php print render($page['search_box']); ?>
          </div>
          <div id="social">
            <ul class="social-links">
              <li><a class="rss" href="<?php print $base_path ?>rss.xml"></a></li>
              <li><a class="twitter" href="<?php print $twitter_url ?>"></a></li>
              <li><a class="facebook" href="<?php print $facebook_url ?>"></a></li>
            </ul>
          </div> 
          <?php print render($page['user_menu']); ?>
        </div></div>
      <?php endif; ?>       

<div id="header-slideshow-wrapper">

<!-- Header --> 
    <div id="header-wrapper"><div class="width-wrapper"><div class="header-wrapper-inner clearfix">
      
      <?php if ($logo): ?>
        <div id="site-logo">
          <a href="<?php print check_url($front_page); ?>"><img src="<?php print $logo ?>" alt="<?php print $site_name; ?>" /></a>
        </div>
      <?php endif; ?>	      
      
      <?php if ($site_name): ?>
        <div id="site-name">
          <?php if ($is_front) { ?>
            <h1><a href="<?php print check_url($front_page); ?>"><?php print $site_name; ?></a></h1>
          <?php } else { ?>
            <span><a href="<?php print check_url($front_page); ?>"><?php print $site_name; ?></a></span>
          <?php } ?>
        </div>
      <?php endif; ?>
      
      <?php if ($site_slogan): ?>
        <div id="site-slogan">
          <h3><?php print $site_slogan; ?></h3>            
        </div>
      <?php endif; ?>    
      
<!-- Logo/Primary-Links/Search -->	
			<?php if ($page['main_menu']): ?>     
        <div id="main-menu">
          <?php print render($page['main_menu']); ?>
        </div>
      <?php endif; ?> 
    
    </div></div></div><!-- /header-wrapper -->
    
<!-- Slideshow Region -->
		<?php if ($page['slideshow']): ?>
      <div id="slideshow"><div class="width-wrapper"><div class="slideshow-inner clearfix">	
        <?php print render($page['slideshow']); ?>
      </div></div></div>
    <?php endif; ?>       
   
</div>   
    
<!-- Top User Regions -->
		<?php if ($page['user1'] || $page['user2'] || $page['user3'] || $page['user4']): ?>
      <div id="top-user-regions"><div class="top-user-regions-inner"><div class="width-wrapper clearfix">
      <?php if ($page['user1']): ?>
          <div class="user-region <?php echo $topBlocks; ?> user1">
            <div class="user-region-inner">
              <?php print render($page['user1']); ?>
            </div>
          </div>
      <?php endif; ?>
      <?php if ($page['user2']): ?>
        <div class="user-region <?php echo $topBlocks; ?> user2">
          <div class="user-region-inner">
            <?php print render($page['user2']); ?>
          </div>
        </div>
      <?php endif; ?>
      <?php if ($page['user3']): ?>
        <div class="user-region <?php echo $topBlocks; ?> user3">
          <div class="user-region-inner">
            <?php print render($page['user3']); ?>
          </div>
        </div>
      <?php endif; ?>
      <?php if ($page['user4']): ?>
        <div class="user-region <?php echo $topBlocks; ?> user4">
          <div class="user-region-inner">
            <?php print render($page['user4']); ?>
          </div>
        </div>
      <?php endif; ?>
      </div></div></div><!-- End of Top User Regions -->
    <?php endif; ?>            
   
<!-- Middle Wrapper -->  
    <div id="middle-wrapper"><div class="width-wrapper clearfix">	  
    
<!-- Navigation Menu -->
			<?php if ($page['navigation_menu']): ?>
        <div class="navigation-left-cap"><div class="navigation-right-cap"><div id="navigation-menu">
          <?php print render($page['navigation_menu']); ?>
        </div></div></div>
      <?php endif; ?>      
  	 
      <div id="main-content" class="clearfix"><div id="main-content-inner">
        <?php if  ($show_breadcrumb == 1): ?> <?php print $breadcrumb; ?><?php endif; ?>
        <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
       	<?php if ($page['content_top']): ?><div class="content-top"><?php print render($page['content_top']); ?></div><?php endif; ?>
        <?php print render($tabs); ?>
        <?php if (!isset($node)): ?>
					<?php print render($title_prefix); ?>
						<?php if ($title): ?><h1 class="title" id="page-title"><span><?php print $title; ?></span></h1><?php endif; ?>
          <?php print render($title_suffix); ?>
        <?php endif; ?>
        <?php print render($page['content']); ?>          
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        <?php if ($page['content_bottom']): ?><div class="content-bottom"><?php print render($page['content_bottom']); ?></div><?php endif; ?>
      </div></div>
      
      <?php if ($page['sidebar_first']): ?>
	    <div class="sidebar-first clearfix"><div class="sidebar-first-inner">
          <?php print render($page['sidebar_first']); ?>
	    </div></div>
      <?php endif; ?>
	  
      <?php if ($page['sidebar_second']): ?>
	    <div class="sidebar-second clearfix"><div class="sidebar-second-inner">
          <?php print render($page['sidebar_second']); ?>                
	    </div></div>
      <?php endif; ?>
  
    </div></div><!-- /middle-wrapper --> 
    
<!-- Bottom User Regions -->
    <?php if ($page['user5'] || $page['user6'] || $page['user7'] || $page['user8']): ?>
    
      <div id="bottom-user-regions"><div class="bottom-user-regions-inner"><div class="width-wrapper clearfix">
		<?php if ($page['user5']): ?>
		  <div class="user-region <?php echo $bottomBlocks; ?> user5">
		    <div class="user-region-inner bottom-equal">
              <?php print render($page['user5']); ?>
		    </div>
		  </div>
        <?php endif; ?>
        <?php if ($page['user6']): ?>
		  <div class="user-region <?php echo $bottomBlocks; ?> user6">
		    <div class="user-region-inner bottom-equal">
              <?php print render($page['user6']); ?>
	        </div>
		  </div>
        <?php endif; ?>
        <?php if ($page['user7']): ?>
		  <div class="user-region <?php echo $bottomBlocks; ?> user7">
		    <div class="user-region-inner bottom-equal">
              <?php print render($page['user7']); ?>
		    </div>
		  </div>
        <?php endif; ?>
		<?php if ($page['user8']): ?>
		  <div class="user-region <?php echo $bottomBlocks; ?> user8">
		    <div class="user-region-inner bottom-equal">
              <?php print render($page['user8']); ?>
		    </div>
		  </div>
        <?php endif; ?>
      </div></div></div><!-- End of Bottom User Regions -->
    <?php endif; ?>  
    
<!-- The All Knowing All Seeing Footer Block -->
	  <div id="footer"><div class="width-wrapper clearfix">
      <?php print render($page['footer']) ?>
      Designed by <a href="http://www.neptunethemes.com">NeptuneThemes.com</a>
	  </div></div><!-- /footer -->
 
  </div></div><!-- page-wrapper -->