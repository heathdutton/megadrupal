<?php

/**
 * @file
 * LiteJazz block--blockthme--xx.tpl.php
 * to use this file you need Block Theme Module http://drupal.org/project/blocktheme
 * for Default theme implementation to display a block see modules/block/block.tpl.php.
 */
?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="<?php print $classes; ?> blk-solid blk-solid2"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>

<?php if ($block->subject): ?>
  <h2 <?php print $title_attributes; ?>><?php print $block->subject ?></h2>
<?php endif;?>
  <?php print render($title_suffix); ?>
  <div <?php print $content_attributes; ?>><?php print $content ?></div>
</div>
