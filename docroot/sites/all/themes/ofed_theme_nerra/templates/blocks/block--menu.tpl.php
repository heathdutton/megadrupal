<?php
/**
 * @file
 * Needs to be documented.
 */
?>

<div class="block-menu">
  <?php print render($title_prefix); ?>
  <?php if ($block->subject): ?>
    <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php print $content ?>
</div>
