<?php
/**
 * @file
 * Extend template implementation to display a badge block.
 */

?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?> block-badge"<?php print $attributes; ?>>
  <span class="badge">&nbsp;</span>
  <div class="block-inner clearfix">
    <?php print render($title_prefix); ?>
    <?php if ($block->subject): ?>
      <h2<?php print $title_attributes; ?>><?php print $block->subject; ?></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <div<?php print $content_attributes; ?>>
      <?php print $content ?>
    </div>
  </div>
</div>
