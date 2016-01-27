<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php if ($block->subject): ?>
    <?php if (theme_get_setting('block_h3')): ?>
  <h3 class="title block-title"<?php print $title_attributes ?>><?php print $block->subject ?></h3>
    <?php else: ?>
  <div class="title block-title"<?php print $title_attributes ?>><?php print $block->subject ?></div>
    <?php endif; ?> 
  <?php endif;?>
  <?php print render($title_suffix); ?>
  <div class="content"<?php print $content_attributes; ?>>
    <?php print $content ?>
  </div>
</div>
