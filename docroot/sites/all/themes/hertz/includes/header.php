<div id="header">
	<div class="container" class="clearfix">
		<?php if ($logo): ?>
		  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo">
			<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
		  </a>
		<?php endif; ?>
		<div id="social">
			<?php if (theme_get_setting('social_icons')): ?>
				<li><a href="<?php echo theme_get_setting('googleplus_username'); ?>" class="googleplus icon" target="_blank"></a></li>
				<li><a href="<?php echo theme_get_setting('twitter_username'); ?>" class="twitter icon" target="_blank"></a></li>
				<li><a href="<?php echo theme_get_setting('facebook_username'); ?>" class="facebook icon" target="_blank"></a></li>
			<?php endif; ?>
				<li>
					<?php if ($page['search_box']): ?><!-- start search box region -->
						<div class="search-box">
							<?php print render($page['search_box']); ?>
						</div> <!-- end search box region -->
					<?php endif; ?>
				</li>
		</div> <!-- end #social -->
	</div> <!-- /.container -->
</div> <!-- /#header-top -->
<nav id="navigation" class="navigation">
	<div class="container clearfix">
		<?php 
		$main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu')); 
		print drupal_render($main_menu_tree);
		?>
	</div>
</nav> <!-- /#navigation -->