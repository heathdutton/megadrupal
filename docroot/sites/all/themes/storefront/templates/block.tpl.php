<?php
/**
 * @file
 * Default Block Template.
 *
 * @param $block_html_id
 *   the block-specific id.
 * @param $block->subject
 *   the block title
 * @param $content
 *   the block content.
 */

?>
<?php $tag = $block->subject ? 'section' : 'div'; ?>
<<?php print $tag; ?> id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
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
</<?php print $tag; ?>>
