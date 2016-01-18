<?php
/**
 * @file
 * Default theme implementation to display a block.
 */
?>
<<?php print $block_el; ?> id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($block->subject): ?>
    <header>
      <?php print render($title_prefix); ?>
      <h2<?php print $title_attributes; ?>><?php print $block->subject; ?></h2>
      <?php print render($title_suffix); ?>
    </header>
  <?php elseif ($title_suffix): ?>
      <?php print render($title_suffix); ?>
  <?php endif; ?>
  <div class="block-content content">
    <?php print $content ?>
  </div>
</<?php print $block_el; ?>>
