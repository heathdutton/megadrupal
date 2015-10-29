<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clearfix block block-<?php print $block->module ?>">

<?php if (!empty($block->subject)): ?>
  <div class="titlecontainer"><h2 class="blocktitle"><?php print $block->subject ?></h2></div>
<?php endif;?>

  <div class="content"><?php print $content ?></div>
</div>