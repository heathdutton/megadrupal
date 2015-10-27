<div id="page">
  <div id="header">
   <div class="inner">
    <div id="headerimg">
	  <?php if ($logo): ?>
        <a href="<?php print $front_page; ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
	  <?php endif; ?>      
        <?php if ($site_name): ?>
          <h1 id="logo-text"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
        <?php endif; ?>
	  <div class="description">
	  <?php if ($site_slogan): ?>
        <?php print $site_slogan; ?>
      <?php endif; ?></div>
	</div>
	
      <div  id="nav">
	  <?php if ($main_menu): ?>
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu-links',
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
   </div><!-- .inner -->	  
  </div><!-- #header -->
  <div id="content">
    <?php if ($breadcrumb): print $breadcrumb; endif; ?>
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
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
      
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    
	<div class="navigation">
	  <span class="previous-entries"></span> <span class="next-entries"></span>
	</div>	
		
  </div><!--/content -->
  
<div id="sidebar-right">
    <?php if ($page['sidebar_right']): ?>
      <?php print render($page['sidebar_right']); ?>
    <?php endif; ?>

</div><!--/sidebar -->

  <hr class="clear" />
  <!--/header -->
</div>
<div id="credits">
<div class="alignleft"><a href="http://itapplication.net">Theme develop by iTApplication.net</a><br /><?php print render($page['footer']); ?></div> 
<div class="alignright"><?php print $feed_icons ?></div>

</div>
