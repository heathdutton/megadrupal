<?php

/**
 * @file
 * LiteJazz block--blockthme--xx.tpl.php
 * to use this file you need Block Theme Module http://drupal.org/project/blocktheme
 * for Default theme implementation to display a block see modules/block/block.tpl.php.
 */
?>
<div class="<?php print $classes; ?> customframeblock stripebox stripe1-box-page-bg" id="block-<?php print $block->module; ?>-<?php print $block->delta; ?>"<?php print $attributes; ?>>
    <div class="boxborder">
    <div class="bi">
    <div class="bt"><div></div></div>
<div class="custom-inbox">
  <?php print render($title_prefix); ?>
  <h2 <?php print $title_attributes; ?>><?php print $block->subject; ?></h2>
  <?php print render($title_suffix); ?>
  <div <?php print $content_attributes; ?>><?php print $content; ?></div>
</div>
    <div class="bb"><div></div></div>
    </div>
  </div>
</div>




