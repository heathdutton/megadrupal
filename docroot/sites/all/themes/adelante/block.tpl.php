  <div class="block block-<?php print $block->module; ?>" id="block-<?php print $block->module; ?>-<?php print $block->delta; ?>">
  <?php if($block->subject) { ?>
  	<div class="block-title">
    	<h2 class="title"><?php print $block->subject; ?></h2>
	</div>
	<?php } ?>
    <div class="block-content"><?php print $content; ?></div>
 </div>