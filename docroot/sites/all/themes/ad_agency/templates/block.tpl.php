<?php
/**
 * @file block.tpl.php
 *
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $block->subject: Block title.
 * - $content: Block content.
 * - $block->module: Module that generated the block.
 * - $block->delta: This is a numeric id connected to each module.
 * - $block->region: The block region embedding the current block.
 */
?>
<div class="block block-<?php print $block->module; ?>" id="block-<?php print $block->module; ?>-<?php print $block->delta; ?>">
  <div class="blockinner">

    <?php if (!empty($block->subject)): ?>
      <h2 class="title"><?php print $block->subject; ?></h2>
    <?php endif; ?>

    <div class="content">
      <?php print $content; ?>
    </div>
    
  </div>
</div>

