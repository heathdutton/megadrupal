<div id="page">

  <div class="user-menu-wrapper"><div class="user-menu-wrapper-inner <?php echo $grid_size ?>">  
    <nav id="user-menu" class="<?php echo $grid_full_width ?> clearfix">
      <?php print render($page['user_menu']); ?>
    </nav>
  </div></div>

  <div class="header-wrapper"><div class="header-wrapper-inner"><div class="header-wrapper-inner-innner <?php echo $grid_size ?>">
    <header class="<?php echo $grid_full_width ?> clearfix">
            
      <hgroup>
      
		<?php if ($logo): ?>
          <div class="site-logo">
            <a href="<?php print check_url($front_page); ?>"><img src="<?php print $logo ?>" alt="<?php print $site_name; ?>" /></a>
          </div>
        <?php endif; ?>	      
      
        <?php if ($site_name): ?>
            <?php if ($is_front) { ?>
              <h1 class="site-name"><a href="<?php print check_url($front_page); ?>"><?php print $site_name; ?></a></h1>
            <?php } else { ?>
              <div class="site-name"><a href="<?php print check_url($front_page); ?>"><?php print $site_name; ?></a></div>
            <?php } ?>
        <?php endif; ?>
        
        <?php if ($site_slogan): ?>
            <div class="site-slogan"><?php print $site_slogan; ?></div>            
        <?php endif; ?>  
      
      </hgroup>
      
      <?php print render($page['search_box']); ?>                   
       
    </header>
  </div></div></div>

  <?php if ($page['main_menu']): ?>    
    <div class="main-menu-wrapper"><div class="main-menu-wrapper-inner <?php echo $grid_size ?>">  
      <nav id="main-menu" class="<?php echo $grid_full_width ?> clearfix">
        <?php print render($page['main_menu']); ?>
      </nav>
    </div></div> 
  <?php endif; ?>  
  
  <?php if ($page['slideshow']): ?>    
    <div class="slideshow-wrapper"><div class="slideshow-wrapper-inner <?php echo $grid_size ?>">  
      <nav id="slideshow" class="<?php echo $grid_full_width ?> clearfix">
        <?php print render($page['slideshow']); ?>
      </nav>
    </div></div> 
  <?php endif; ?>      
  
  <?php if ($page['preface_1'] || $page['preface_2'] || $page['preface_3'] || $page['preface_4']): ?>
    <div class="preface-wrapper"><div class="preface-wrapper-inner <?php echo $grid_size ?>">  
      <section id="preface" class="clearfix">
        <div class="<?php echo $preface_1_grid_width ?>"><?php print render($page['preface_1']); ?></div>
        <div class="<?php echo $preface_2_grid_width ?>"><?php print render($page['preface_2']); ?></div>
        <div class="<?php echo $preface_3_grid_width ?>"><?php print render($page['preface_3']); ?></div>
        <div class="<?php echo $preface_4_grid_width ?>"><?php print render($page['preface_4']); ?></div>
      </section>
    </div></div> 
  <?php endif; ?>   
   
<!-- Main Content -->  
  <div class="main-content-wrapper"><div class="main-content-wrapper-inner <?php echo $grid_size ?>">
    <section id="main-content" class="clearfix"> 	   

      <?php if ($page['sidebar_first']): ?>
      <aside class="sidebar first-sidebar <?php print $sidebar_first_grid_width ?>">
          <?php print render($page['sidebar_first']); ?>
      </aside>
      <?php endif; ?>    
    
      <div class="main">
        <div class="main-inner  <?php print $main_content_grid_width ?>">
          <?php if ($breadcrumb): ?> <?php print $breadcrumb; ?><?php endif; ?>
          <?php print render($page['content_top']); ?>
          <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
          <?php print render($tabs); ?>
          <?php if (!isset($node)): ?>
            <?php print render($title_prefix); ?>
              <?php if ($title): ?><h1 class="title" id="page-title"><span><?php print $title; ?></span></h1><?php endif; ?>
            <?php print render($title_suffix); ?>
          <?php endif; ?>
          <?php print render($page['content']); ?>          
          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
          <?php print render($page['content_bottom']); ?>
        </div> 
        <?php if ($page['sidebar_second']): ?>
        <aside class="sidebar second-sidebar <?php print $sidebar_second_grid_width ?>">
            <?php print render($page['sidebar_second']); ?>                
        </aside>
        <?php endif; ?>     
      </div>            
  
    </section>
  </div></div>
  
  <?php if ($page['postscript_1'] || $page['postscript_2'] || $page['postscript_3'] || $page['postscript_4']): ?>
    <div class="postscript-wrapper"><div class="postscript-wrapper-inner <?php echo $grid_size ?>">  
      <section id="postscript" class="clearfix">
        <div class="<?php echo $postscript_1_grid_width ?>"><?php print render($page['postscript_1']); ?></div>
        <div class="<?php echo $postscript_2_grid_width ?>"><?php print render($page['postscript_2']); ?></div>
        <div class="<?php echo $postscript_3_grid_width ?>"><?php print render($page['postscript_3']); ?></div>
        <div class="<?php echo $postscript_4_grid_width ?>"><?php print render($page['postscript_4']); ?></div>
      </section>
    </div></div>
  <?php endif; ?>    
    
<!-- All Hail the Footer -->
  <div class="footer-wrapper"><div class="footer-wrapper-inner <?php echo $grid_size ?>">
    <footer id="footer" class="<?php echo $grid_full_width ?> clearfix">
      <?php print render($page['footer']) ?>
      <?php if($is_front): ?><span style="font-size: 0.9em;"><a href="http://www.themeshark.com">Responsive Premium Drupal Templates and Themes</a></span> by ThemeShark<?php endif; ?>
    </footer><!-- /footer -->
  </div></div>
 
</div><!-- page -->