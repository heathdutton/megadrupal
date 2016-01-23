<div id="content">
	<?php include ($directory."/includes/header.php"); ?>
	<?php if (theme_get_setting('breadcrumb_link')): ?>
	<div id="breadcrumb">
		<div class="container" class="clearfix">
			<?php if ($breadcrumb): print $breadcrumb; endif;?>
		</div>
	</div> <!-- /#breadcrumb -->
	<?php endif; ?>
	<div id="container" class="clearfix">
	  <section id="main" role="main" class="clearfix">
		<?php print $messages; ?>
		<?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
		<?php print render($title_prefix); ?>
		<?php if ($title): ?><h1 class="title"><?php print $title; ?></h1><?php endif; ?>
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
	</div> <!-- /#container -->
</div> <!-- /#content -->
<?php include ($directory."/includes/footer.php"); ?>
