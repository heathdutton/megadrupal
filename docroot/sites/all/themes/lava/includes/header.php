<?php if ($logo): ?>
	<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" id="logo">
		<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
	</a>
<?php endif; ?>
<nav id="navigation" class="navigation">
	<?php 
	$main_menu_tree = menu_tree(variable_get('menu_main_links_source', 'main-menu')); 
	print drupal_render($main_menu_tree);
	?>
</nav> <!-- /#navigation -->
