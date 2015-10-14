<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php if ($block->subject): ?>

    <div class = "icon"></div>

    <?php print render($title_prefix); ?>
    <h3<?php print $title_attributes; ?>><?php print $block->subject; ?></h3>
    <?php print render($title_suffix); ?>

  <?php endif;?>

	<div class="content clearfix"<?php print $content_attributes; ?>>
		<?php print $content; ?>
	</div>

</div>