<div id="outer-wrapper">  
  <div id="header">  
    <?php if ($is_front && $site_name): ?>
      <h1 id="site-name"><a href="<?php print $front_page; ?>" title="<?php print check_plain($site_name); ?>">
								<?php print strip_tags($site_name); ?>
       </a></h1>
				<?php else: ?>
						<div id="site-name"><a href="<?php print $front_page; ?>" title="<?php print check_plain($site_name); ?>">
								<?php print strip_tags($site_name); ?>
       </a></div>
    <?php endif; ?>
				
    <?php if ($secondary_menu): ?>  
      <?php print theme('links__system_secondary_menu', array(
        'links' => $secondary_menu,
        'attributes' => array(
          'id' => 'secondary-menu',
          'class' => array('links', 'secondary-links'),
        ),
        'heading' => array(
            'text' => t('Secondary menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
    <?php endif; ?>
				
				<?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print $site_name; ?>">
								<img src="<?php print $logo; ?>" alt="<?php print $site_name; ?>" title="<?php print $site_name; ?>" />
      </a>
    <?php endif; ?>
  </div> <!-- #header -->
		
		<div id="main">
    <?php print render($page['help']); ?>
				<?php if ($messages): ?>
						<?php print $messages; ?>
    <?php endif; ?>
				<?php if ($action_links): ?>
						<ul class="action-links">
								<?php print render($action_links); ?>
						</ul>
				<?php endif; ?>
				
				<?php if ($main_menu): ?>
      <?php print theme('links__system_main_menu', array(
        'links' => $main_menu,
        'attributes' => array(
          'id' => 'main-nav',
          'class' => array('tabs', 'primary-links'),
        ),
        'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
    <?php endif; ?>
				
				<?php if ($page['highlighted']): ?>
      <div id="mission">
        <?php print render($page['highlighted']); ?>
      </div>
    <?php endif; ?>
    
    <div id="main-inner">
						<?php if ($tabs): ?><?php print render($tabs); ?><?php endif; ?>
						<?php if (!empty($tabs2)): print render($tabs2); endif; ?>
						<a name="main-content"></a>
						<?php print render($title_prefix); ?>
      <?php if ($title && !$is_front): ?>
        <h1 class="title"><?php print $title; ?></h1>
      <?php endif; ?>
						<?php print render($title_suffix); ?>		
						
						<?php print render($page['content']); ?>
						<?php if ($breadcrumb): print $breadcrumb; endif; ?>
    </div><!-- #main-inner -->
				
  </div><!-- #main -->
  
  <?php if ($page['primary_box']): ?>
    <div id="primary-box">
						<?php print render($page['primary_box']); ?>
				</div><!-- #primary-box -->
  <?php endif; ?>
  
  <?php if ($page['secondary_box']): ?>
    <div id="secondary-box">
						<?php print render($page['secondary_box']); ?>
				</div><!-- #secondary-box -->
  <?php endif; ?>
  
  <?php if ($page['tertiary_box']): ?>
    <div id="tertiary-box" >
						<?php print render($page['tertiary_box']); ?>
				</div><!-- #tertiary-box -->
  <?php endif; ?>		

  <?php if ($page['footer']): ?>
    <div id="footer"><?php print render($page['footer']); ?></div><!-- #footer -->
  <?php endif; ?>
  
</div> <!-- #outer-wrapper -->