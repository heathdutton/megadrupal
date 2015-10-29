<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 */ 
?> 
  <div id="page" class="<?php if ($page['sidebar_first'] || $page['sidebar_second']) { print "one-sidebar"; } if ($page['sidebar_first'] && $page['sidebar_second']) { print " two-sidebars"; }?>">
  
    <div id="header" class="clearfix">
    
      <div id="logo-title">
       
      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>
        
          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <div id="site-name"><strong>
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </strong></div>
            <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
            <?php endif; ?>
        
          <?php if ($site_slogan): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        
      </div>
      
      <div class="menu <?php 
	        if($main_menu){ print 'withprimary ';}
			if($secondary_menu){ print 'withsecondary';}
			endif; ?>">
          <?php if ($main_menu): ?>
            <div id="primary" class="clear-block">
			  <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'clearfix', 'primary-links')), 'heading' => t('Main menu'))); ?>              
            </div>
          <?php endif; ?>
          
          <?php if ($secondary_menu): ?>
          <div id="secondary" class="clear-block">
		   <?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary-menu', 'class' => array('links', 'inline', 'clearfix', 'links secondary-links')), 'heading' => t('Secondary menu'))); ?>      
           </div>
          <?php endif; ?>
      </div>
      
      <?php if ($page['header']): ?>
        <div id="header-region">
          <?php print render($page['header']); ?>
        </div>
      <?php endif; ?>
      
    </div>

    <div id="container" class="<?php if ($page['sidebar_first']) { print "withleft"; } if ($page['sidebar_second']) { print " withright"; }?> clear-block">
      
      <div id="main-wrapper">
      <div id="main" class="clear-block">
        <?php if ($breadcrumb): ?>
        <div id="breadcrumb"><?php print $breadcrumb; ?></div>
        <?php endif; ?>
        <?php print $messages ?>
        <?php if ($page['content_top']):?><div id="content-top"><?php print render($page['content_top']); ?></div><?php endif; ?>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>        
        <?php print render($page['content']); ?>
        <?php if ($page['content_top']): ?><div id="content-bottom"><?php print render($page['content_top']) ?></div><?php endif; ?>
      </div>
      </div>
      
      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-left" class="sidebar">
          <?php print render($page['sidebar_first']); ?>
        </div>
      <?php endif; ?>

      <?php if ($page['sidebar_second']): ?>
        <div id="sidebar-right" class="sidebar">
          <?php print render($page['sidebar_second']); ?>
        </div>
      <?php endif; ?>

    </div>

    <div id="footer">
	  <div id="footerblocks"><?php if ($page['footer']): print render($page['footer']); endif; ?></div>      
    </div>

  </div>
