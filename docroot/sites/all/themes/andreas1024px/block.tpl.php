<?php
?>
<div id="block-<?php print $block->module . '-' . $block->delta; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if(isset($block->subject)): ?>
    <?php print render($title_prefix); ?>
    <?php if ($block->subject): ?>
      <h2<?php print $title_attributes; ?>><?php print $block->subject; ?></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print $content; ?>
  </div>

</div> 
