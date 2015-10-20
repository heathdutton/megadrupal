<?php
?>
<div id="container">
	<div id="site-holder">
		<div id="header">     
			<?php if (!empty($secondary_nav)): print $secondary_nav; endif; ?>
			<div id="logo">
				<a href="<?php print $front_page ?>">
					<?php if ($logo): ?>
						<img src="<?php print $logo ?>" alt="<?php print $site_name ?>" title="<?php print $site_name ?>" />
					<?php endif; ?>
				</a>
				<?php if ($site_name): ?>
					<h1 class="site-name">
						<a title="<?php print t('Home'); ?>" rel="home" href="<?php print $front_page; ?>">
							<?php print $site_name; ?>
						</a>
					</h1>
        <?php endif; ?>
				<?php if ($site_slogan): ?>
          <div class="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
			</div>
			<?php print render($page['header']); ?> 
		</div> <!-- /#header -->
		
		<?php if ($page['precontent']): ?>          
			<div id="precontent">
				<?php print render($page['precontent']); ?>
			</div>
    <?php endif; ?>
		
		<?php if ($page['mission']): ?>          
			<div id="mission">
				<?php print render($page['mission']); ?>
			</div>
    <?php endif; ?>

		<div id="content">
			<div id="center">
				<?php if ($tabs): ?>
					<div id="tabs-wrapper">
						<?php print render($tabs); ?>
					</div>
				<?php endif; ?>					
				<?php if ($title): ?>
					<h1><?php print $title ?></h1>
				<?php endif; ?>
				<?php print render($tabs_secondary); ?>
				<?php print $messages; ?>
				<?php if ($action_links): ?>
					<ul class="action-links">
						<?php print render($action_links); ?>
					</ul>
				<?php endif; ?>
				<div class="content-wrapper">
					<?php print render($page['content']); ?>
				</div>              
			</div> <!--#center -->

			<?php if ($page['sidebar_second']): ?>
				<div id="sidebar-second" class="sidebar">
					<?php print render($page['sidebar_second']); ?>
				</div>
			<?php endif; ?>
			
			<?php if ($page['aftercontent']): ?>          
				<div id="aftercontent">
					<?php print render($page['aftercontent']); ?>
				</div>
			<?php endif; ?>
		</div>
			
		<div id="footer">
			<?php print render($page['footer']); ?>
		</div>
	</div>
	<div id="copyright">
		<strong><a href="http://www.adciserver.com" title="Go to adciserver.com">Drupal theme</a></strong> by <a href="http://www.adciserver.com" title="Go to adciserver.com">www.adciserver.com</a>
	</div>
</div> <!-- /#container -->
