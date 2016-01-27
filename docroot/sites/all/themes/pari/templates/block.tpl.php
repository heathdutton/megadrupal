<section id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
	<?php print render($title_prefix); ?>
	<?php if ($title): ?>
		<h3 class="block-title"<?php print $title_attributes; ?>><?php print $title; ?></h3>
	<?php endif;?>
	<?php print render($title_suffix); ?>
	<div class="content"<?php print $content_attributes; ?>>
		<?php print $content ?>
	</div>
</section> <!-- /.block -->
