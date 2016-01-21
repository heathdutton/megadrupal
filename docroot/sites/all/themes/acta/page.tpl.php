  <div id='header' class='clear-block'>
    <div id='header-logo'>
            <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
	  <?php endif; ?>
    </div>
    
    <div id='site-info'>  
      <?php if ($site_name): ?>
          <h1 id="logo-text"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
        <?php endif; ?>
      
      <?php if ($site_slogan): ?>
        <p id="slogan"><?php print $site_slogan; ?></p>
      <?php endif; ?>
    </div>    
    
  </div>
  <!-- [ End header area ] -->
  
  <div id='main' class='clear-block'>         <!-- [ holds blue bordered tile ] -->
  <div id='main-top' class='clear-block'>     <!-- [ holds top background with blue borders and sandy gradient ] -->
  
      <div id='main-menu'>
	  <?php if ($main_menu): ?>
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'subnavlist',
            'class' => array('links', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      <?php endif; ?>
      </div>
	  
     <!-- [ Content area ] -->
      
      <!-- [ Check for sidebar - we handle the content section differently if there is none ] -->
      <?php if (!empty($page['sidebar_first'])) { ?>
        <div id='content'>
      <?php } else { ?>
    <div id='content-nosb'>
      <?php } ?>
      
      <?php 
      $acta_mission = theme_get_setting('acta_mission');
      if ($acta_mission && $is_front): ?>
        <div id="mission"><?php print render($acta_mission); ?></div>
      <?php endif; ?>
      <?php if ($breadcrumb) { ?><div id='bc'><?php print $breadcrumb ?></div><?php } ?>
            
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="node-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php if ($tabs): ?>
        <div class="tabs"><?php print render($tabs); ?></div>
      <?php endif; ?>
      <?php if ($show_messages): ?>
        <?php print $messages; ?>
      <?php endif; ?>
          
      <!-- [ Content inner div allows padding without disrupting the containing floating div (content) ] -->
      <div class='content-inner'>
       
      <!-- [ Begin Content ] -->
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
      <!-- [ End Content ] -->  
      
      </div>      
  
    <!-- [ End #content or #content-nosb - from line 56 ] -->  
    </div>
    
    <!-- [ Sidebar area ] -->
	<?php if ($page['sidebar_first']): ?>
    <div id="sidebar">
        <?php print render($page['sidebar_first']); ?>
    </div>
	<?php endif; ?>
    
  <!-- [ End #main, #main-top, and #main-bottom ] -->  
  </div></div>  

  <!-- [ Footer area ] -->
  <div id='footer' class='clear-block'>
    <div id='footer-bottom' class='clear-block'>
      <?php if ($page['footer']): ?>
      <?php print render($page['footer']); ?>
      <?php endif; ?>
	  <p>This site is powered by <a href="http://drupal.org/">Drupal</a>. Theme: <a href="http://itapplication.net/it/category/drupal/acta/">Acta</a>.</p>
    </div>
  </div>
