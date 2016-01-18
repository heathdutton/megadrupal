<div id="header-top">
	<div class="container" class="clearfix">
		<?php if ($logo): ?>
		  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo">
			<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
		  </a>
		<?php endif; ?>
		<div id="social">
			<?php if (theme_get_setting('social_icons')): ?>
				<li><a href="<?php echo theme_get_setting('linkedin_username'); ?>" class="linkedin icon" target="_blank"></a></li>
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
	<div class="container" class="clearfix">
		<?php 
		$main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu')); 
		print drupal_render($main_menu_tree);
		?>
	</div>
</nav> <!-- /#navigation -->
<?php if (theme_get_setting('breadcrumb_link')): ?>
<div id="breadcrumb">
	<div id="container" class="clearfix">
		<?php if ($breadcrumb): print $breadcrumb; endif;?>
	</div>
</div> <!-- /#breadcrumb -->
<?php endif; ?>
<div id="container" class="clearfix">
  <section id="main" role="main" class="clearfix">
    <?php print $messages; ?>
    <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if (!empty($tabs['#primary'])): ?><div class="tabs-wrapper clearfix"><?php print render($tabs); ?></div><?php endif; ?>
    <?php print render($page['help']); ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    <?php print render($page['content']); ?>
  </section> <!-- /#main -->
  
  <?php if ($page['sidebar_first']): ?>
    <aside id="sidebar-first" role="complementary" class="sidebar clearfix">
      <?php print render($page['sidebar_first']); ?>
    </aside>  <!-- /#sidebar-first -->
  <?php endif; ?>

  <?php if ($page['sidebar_second']): ?>
    <aside id="sidebar-second" role="complementary" class="sidebar clearfix">
      <?php print render($page['sidebar_second']); ?>
    </aside>  <!-- /#sidebar-second -->
  <?php endif; ?>
</div> <!-- /.container -->
<footer id="footer" role="contentinfo" class="clearfix">
	<div class="container" class="clearfix">	
		<?php if ($page['footer_one']): ?>
			<div class="footer-one">
				<?php print render($page['footer_one']); ?>
			</div>
		<?php endif; ?>
		
		<?php if ($page['footer_two']): ?>
			<div class="footer-two">
				<?php print render($page['footer_two']); ?>
			</div>
		<?php endif; ?>
		
		<?php if ($page['footer_three']): ?>	
			<div class="footer-three">
				<?php print render($page['footer_three']); ?>
			</div>
		<?php endif; ?>
	</div> <!-- /.container -->
</footer> <!-- /#footer -->
<div class="clear"></div>
<div id="footer-bottom">
	<div class="container" class="clearfix">	
		<?php if (theme_get_setting('footer_copyright')): ?>
			<div id="copyright">Copyright &copy; <?php echo date("Y"); ?>, <?php print $site_name; ?></div>
		<?php endif; ?>
		<?php print render($page['footer']) ?>
	</div> <!-- /.container -->
</div>
