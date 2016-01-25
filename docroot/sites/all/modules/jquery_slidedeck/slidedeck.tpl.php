<?php

/**
 * @file
 * Main slidedeck template
 *
 * Variables available:
 * - $content: An array of block content with block content and block title
 */
?>
<div class="<?php print $skin; ?> slidedeck_frame">
   <dl class="slidedeck">
     <?php foreach ($content as $block_key => $block) : ?>
	<dt>
	<?php if($block->title) : ?>
	<?php print check_plain($block->title); ?>
	<?php endif; ?>
	</dt>
	<dd><?php print render($block->content);?></dd>
     <?php endforeach; ?>
  </dl>
</div>
