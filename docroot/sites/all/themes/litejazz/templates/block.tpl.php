<?php

/**
 * @file
 * LiteJazz block.tpl.php
 *
 * for Default theme implementation to display a block see modules/block/block.tpl.php.
 */
?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="<?php print $classes; ?> unstyled-block"<?php print $attributes; ?>>
<?php print render($title_prefix); ?>
<?php if ($block->subject): ?>
  <h2 <?php print $title_attributes; ?>><?php print $block->subject ?></h2>
<?php endif;?>
<?php print render($title_suffix); ?>
  <div <?php print $content_attributes; ?>><?php print $content ?></div>
</div>
