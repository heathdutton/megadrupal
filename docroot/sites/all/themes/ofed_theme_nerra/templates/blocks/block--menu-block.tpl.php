<?php
/**
 * @file
 * Needs to be documented.
 */
?>

<div  class="subnavigation">
  <?php print render($title_prefix); ?>
  <?php if ($block->subject): ?>
    <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <nav <?php print $content_attributes; ?>>
    <?php print $content ?>
  </nav>
</div>
