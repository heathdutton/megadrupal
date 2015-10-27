<div id="block-<?php print $block->module.'-'.$block->delta; ?>" class="clear-block block block-<?php print $block->module . ' ' . $zebra . ' ' . $classes . ' ' . $attributes; ?>">
  <?php print render($title_prefix); ?>
	<?php if (!empty($block->subject)): ?>
    <h2<?php print $title_attributes; ?>><?php print $block->subject; ?></h2>
  <?php endif;?>
	<?php print render($title_suffix); ?>
  <div class="content<?php if($block->subject): print ' with-subject'; endif; ?><?php print $content_attributes; ?>">
    <?php print $content; ?>
  </div>
</div>
