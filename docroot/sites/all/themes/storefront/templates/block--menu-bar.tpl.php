<?php
/**
 * @file
 * Menu Bar Block
 *
 * @param $block_html_id
 *   the block-specific id.
 * @param $block->subject
 *   the block title.
 * @param $content
 *   the block content.
 */

?>
<?php if ($content): ?>
  <nav id="<?php print $block_html_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
    <?php print render($title_prefix); ?>
    <?php if ($block->subject): ?>
      <h2<?php print $title_attributes; ?>><?php print $block->subject; ?></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <div class="menu-wrapper clearfix"><?php print $content ?></div>
  </nav>
<?php endif; ?>
