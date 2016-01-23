<div id="page-header">
	<div class="container clearfix">
		<?php if (theme_get_setting('breadcrumb_link')): ?>
			<?php if ($breadcrumb): print $breadcrumb; endif;?>
		<?php endif; ?>
		<?php print render($title_prefix); ?>
		<?php if ($title): ?>
			<h1 id="page-title"><?php print $title; ?></h1>
		<?php endif; ?>
		<?php print render($title_suffix); ?>
	</div><!--.container-->
</div><!--/#page-header-->