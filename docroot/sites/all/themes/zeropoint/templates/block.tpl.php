<div class="block-wrapper <?php print $block_zebra .' block_'. $block_id ?>">
  <div id="block-<?php print $block->module .'-'. $block->delta ?>" class="<?php print $classes ?> <?php if ($themed_block): ?>themed-block<?php endif ?>"<?php print $attributes ?>>
    <?php print render($title_prefix) ?>
    <?php if ($block->subject): ?>
      <?php if ($themed_block): ?>
    <div class="block-icon"></div>
      <?php endif ?>
      <?php if (theme_get_setting('block_h3')): ?>
    <h3 class="title block-title"<?php print $title_attributes ?>><?php print $block->subject ?></h3>
      <?php else: ?>
    <div class="title block-title"<?php print $title_attributes ?>><?php print $block->subject ?></div>
      <?php endif; ?>
    <?php endif ?>
    <?php print render($title_suffix) ?>
    <div class="content"<?php print $content_attributes ?>>
      <?php print $content ?>
    </div>
  </div>
</div>
