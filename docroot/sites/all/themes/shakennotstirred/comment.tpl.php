<?php
?>
<div class="<?php print $classes . ' ' . $zebra; ?>"<?php print $attributes; ?>>

  <?php print $picture ?>

	<?php print render($title_prefix); ?>
	<h3<?php print $title_attributes; ?>><?php print $title ?></h3>
	<?php print render($title_suffix); ?>

	<div class="content"<?php print $content_attributes; ?>>
		<?php hide($content['links']); print render($content); ?>
		<div class="submitted"><?php print $submitted ?></div>
		<?php if ($signature): ?>
		<div class="clearfix">
			<div>—</div>
			<?php print $signature ?>
		</div>
		<?php endif; ?>
	</div>
	<?php print render($content['links']) ?>

</div>
