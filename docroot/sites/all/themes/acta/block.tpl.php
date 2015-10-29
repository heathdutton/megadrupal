<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?>">
  <div class='block-top'>
    <?php if ($block->subject): ?>
      <h2><?php print $block->subject ?></h2>
    <?php endif;?>
  </div>
  
  <div class='block-content'>
    <div class="content"><?php print $block->content ?></div>
  </div>
  
  <div class='block-bottom'>
    <!-- [ IE fix... this div does not display consistently otherwise SAY NO TO IE ] -->
    <span>&nbsp;</span>
  </div>
</div>  
