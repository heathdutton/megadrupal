<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 */
?>

  <div id="page-wrapper"><div id="page">

    <div id="header"><div class="section clearfix">

      <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      <?php endif; ?>
      
        <?php if ($site_name || $site_slogan || $bio_name): ?>
        <div id="name-and-slogan">
	    <?php if($bio_name): ?>
		<div id="bio-name"><h1><strong><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $bio_name; ?></a></strong></h1></div>
		<?php endif; ?>          

          <?php if ($site_slogan): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
		  
		  <?php if ($site_name): ?>
            <?php if ($title): ?>
              <div id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </div>
            <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
              </h1>
            <?php endif; ?>
			
          <?php endif; ?>
        </div> <!-- /#name-and-slogan -->
      <?php endif; ?>

      <?php print render($page['header']); ?>

    </div></div> <!-- /.section, /#header -->

    <?php if ($main_menu || $secondary_menu): ?>
      <div id="navigation"><div class="section">
        <?php print theme('links__system_main_menu', array('links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'inline', 'clearfix')))); ?>        
      </div></div> <!-- /.section, /#navigation -->
    <?php endif; ?>

    <?php if ($breadcrumb): ?>
      <div id="breadcrumb"><?php print $breadcrumb; ?></div>
    <?php endif; ?>

    <?php print $messages; ?>

    <div id="main-wrapper"><div id="main" class="clearfix">

      <div id="content" class="column1">
	  <?php if ($page['content_top']): ?>
        <div id="content-top" class="column content"><div class="section">
          <?php print render($page['content_top']); ?>
        </div></div> <!-- /.section, /#content-bottom -->
      <?php endif; ?>	
	  <div class="section">
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        <?php print render($page['content']); ?>
        <?php print $feed_icons; ?>
      </div><!-- /.section -->
	  <?php if ($page['content_bottom']): ?>
        <div id="content-bottom" class="column content"><div class="section">
          <?php print render($page['content_bottom']); ?>
        </div></div> <!-- /.section, /#content-bottom -->
      <?php endif; ?>	  
	  </div> <!-- /#content -->

      <?php if ($page['sidebar_second']): ?>
        <div id="sidebar-second" class="column2 sidebar"><div class="section">
          <?php print render($page['sidebar_second']); ?>
        </div></div> <!-- /.section, /#sidebar-second -->
      <?php endif; ?>

    </div></div> <!-- /#main, /#main-wrapper -->
    <hr class="footer" />
    <div id="footer" class="clearfix">
	
      <div class="column1"><div class="section"><?php print render($page['footer']); ?> </div></div>
	  <div class="sidebar column2 <?php if(theme_get_setting('biography_attribute')==0): ?>nocredit<?php endif; ?>"><div class="section">Theme by <a href="http://itapplication.net">itapplication</a>.</div></div>
   
	</div> <!-- /#footer -->

  </div></div> <!-- /#page, /#page-wrapper -->
