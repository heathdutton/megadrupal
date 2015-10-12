<?php
/*===================================================================

    Developed By: Samuel Ayela
	Company: Rocom Solutions
	Website: http://www.rocomsolutions.com/
	
\------------------------------------------------------------------*/
?>
<div id="page">
	<div id="header-wrap">
		<div id="header" class="clearfix">
			<?php if ($logo): ?>
			<div id="logo">
				<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
			</div><!-- /site-logo -->
			<?php endif; ?>			

			<?php if ($site_name || $site_slogan): ?>
			<div id="site-details">
				<?php if ($site_name): ?>
				<h1 class="site-name">
					<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
				</h1><!-- /site-name -->
				<?php endif; ?>

				<?php if ($site_slogan): ?>
				<div class="slogan"><?php print $site_slogan; ?></div><!-- /slogan -->
				<?php endif; ?>
			</div><!-- /site-details -->
			<?php endif; ?>

			<?php if ($page['search']): ?>	
			<div id="site-search">
				<?php print render($page['search']); ?>		
			</div><!-- /site-search -->
			<?php endif; ?>
		</div><!-- /header -->
		<div id="menu-wrap">
			<div id="menu">
		<?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu-links',
            'class' => array('main-menu', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
			</div><!-- /menu -->
		</div><!-- /menu-wrap -->
	</div><!-- /header-wrap -->
	
	<?php if ($messages): ?>
	<div id="site-messages"> 
		<?php print $messages; ?>
	</div>
	<?php endif; ?>

	<div id="content-wrap" class="clearfix">	
	<?php if ($page['sidebar_first']): ?>
    <div id="sidebar-first" class="column sidebar"><div class="section">
        <?php print render($page['sidebar_first']); ?>
    </div></div><!-- /sidebar-first -->
    <?php endif; ?>
	
    <div id="content" class="column"><div class="section">
      <?php print render($page['highlight']); ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>	  
      <?php if ($tabs): ?>
        <div class="tabs"><?php print render($tabs); ?></div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div></div><!-- /content -->
    <?php if ($page['sidebar_second']): ?>
    <div id="sidebar-second" class="column sidebar"><div class="section">
        <?php print render($page['sidebar_second']); ?>
    </div></div><!-- /sidebar-second -->
    <?php endif; ?>
  </div><!-- /content-wrap -->
	
	<div id="footer">
		<div id="footer-top" class="clearfix">
			<?php if ($page['footer_1']): ?>		
			<div class="footer-1">
				<?php print render($page['footer_1']); ?>
			</div><!-- /footer_1 -->
			<?php endif; ?>			
			<?php if ($page['footer_2']): ?>			
			<div class="footer-2">
				<?php print render($page['footer_2']); ?>
			</div><!-- /footer_2 -->
			<?php endif; ?>
			<?php if ($page['footer_3']): ?>			
			<div class="footer-3">
				<?php print render($page['footer_3']); ?>
			</div><!-- /footer_3 -->
			<?php endif; ?>			
			<?php if ($page['footer_4']): ?>			
			<div class="footer-4">			
				<?php print render($page['footer_4']); ?>
			</div><!-- /footer_3 -->
			<?php endif; ?>
		</div><!-- /footer-top -->
		<div class="site-credits">
			<p><?php print date('Y') ?> <a href="<?php print $front_page ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a> | Theme by <a href="http://www.rocomsolutions.com" target="_blank">Rocom Solutions</a></p>
			<?php print render($page['footer']); ?>
		</div><!-- /site-credits -->
	</div><!-- /footer -->
	
</div><!-- /page -->

<?php print render($page['bottom']); ?>
