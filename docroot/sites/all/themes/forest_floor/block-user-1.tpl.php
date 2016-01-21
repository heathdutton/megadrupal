<?php

/**
 * @file
 * Custom template file for the standard navigation block
 * 
 * Used to rename the block title as 'Menu' (translated if appropriate)
 */
?>

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?>">
  <div class="block-inner">
    <h2><?php print t('Menu') ?></h2>
    <div class="content"><?php print $block->content ?></div>
  </div>
</div>
