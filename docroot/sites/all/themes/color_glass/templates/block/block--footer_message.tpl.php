<?php
/**
 * @file
 * Override the footer message block regions.
 */
?>
<div class = "container">
<?php if ($block->subject) : ?><h2 class = "title"><?php print $block->subject; ?></h2><?php endif; ?>
<div class = "block-content text-center">
<?php print $content; ?>
</div>
</div>
