<div id="header-top">
<div class="container">
	<!-- Start logo -->
	<div id="logo">
	<?php if (theme_get_setting('site_logo') == 'logo_image') { ?>
		<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
		<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
		</a>
	<?php } elseif (theme_get_setting('site_logo') == 'logo_text') { ?>
		<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><h1><?php print $site_name; ?></h1></a>
		<?php if ($site_slogan): ?><div id="site-slogan"><?php print $site_slogan; ?></div><?php endif; ?>
	<?php } ?>
	</div><!--/#logo -->
	<!-- start main menu -->
<nav id="nav">
<?php 
	$main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu')); 
	print drupal_render($main_menu_tree);
?>
</nav> <!-- /#navigation -->
</div><!--/.container -->
</div><!--/#header-top -->