<div class="block-wrapper <?php print $block_zebra .' block_'. $block_id ?>">
  <div id="block-<?php print $block->module .'-'. $block->delta ?>" class="<?php print $classes ?>"<?php print $attributes ?>>
    <?php print render($title_prefix) ?>
    <div class="block-icon"></div>
  <?php if ($block->subject): ?>
    <h3 class="title block-title"<?php print $title_attributes ?>><?php print $block->subject ?></h3>
  <?php endif ?>
    <?php print render($title_suffix) ?>
    <div class="content"<?php print $content_attributes ?>>
      <?php print $content ?>
    </div>
  </div>
</div>
