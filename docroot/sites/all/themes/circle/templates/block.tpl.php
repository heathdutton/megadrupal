<?php
/**
 * @file
 * Returns the HTML for a block.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728246
 */
?>
<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if ($block->subject): ?>
  <header>
    <?php print render($title_prefix); ?>
      <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
    <?php print render($title_suffix); ?>
  </header>
  <?php endif;?>
  <?php print $content ?>
</div>
