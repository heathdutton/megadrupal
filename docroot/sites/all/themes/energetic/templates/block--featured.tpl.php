<?php // $Id: $ ?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?>">
  <?php if ($block->subject): ?>
    <h2><?php print $block->subject ?></h2>
  <?php endif;?>
  <div class="content"<?php print $content_attributes; ?>>
    <?php print $content ?>
  </div>
</div>
