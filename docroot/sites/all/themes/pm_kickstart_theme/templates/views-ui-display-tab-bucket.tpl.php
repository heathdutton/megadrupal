<?php

/**
 * @file
 * Template for each "box" on the display query edit screen.
 */
?>
<div class="<?php print $classes; ?>" <?php print $attributes; ?>>
  <div class="btn-group btn-group-xs" style="clearfix display:block; width:100%;">
    <?php if (!empty($title)) : ?>
      <button class="btn btn-info disabled"><?php print $title; ?></button>
    <?php endif; ?>
    <?php print $item_help_icon; ?>
    <?php if(!empty($actions)) : ?>
      <?php print $actions; ?>
    <?php endif; ?>
  </div>
  <?php print $content; ?>
</div>
