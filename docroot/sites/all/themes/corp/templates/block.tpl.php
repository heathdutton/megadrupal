<?php
/**
 * @file
 * Corp theme's implementation to display a block.
 */
?>
<div id="block-<?php print $block->module .'-'. $block->delta ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="block-inner">
<?php global $base_url; ?>
    <?php print render($title_prefix); ?>
    <?php if ($block->subject): ?>
      <h2 class="block-title"<?php print $title_attributes; ?>><?php print $block->subject ?><img src="<?php print $base_url; ?>/sites/all/themes/corp/images/bullet.png"></h2>
    <?php endif;?>
    <?php print render($title_suffix); ?>
 <div style="clear:both;"></div>
  </div>

    <div class="content" <?php print $content_attributes; ?>>
      <?php print $content; ?>
      
    </div>
 
</div> <!-- /block-inner /block -->