<div class="clearfix <?php print $classes; ?>"<?php print $attributes; ?> id="<?php print $block_html_id; ?>">
<div class="block_inner">
<?php if (!empty($block->subject)): ?>
  <h2 class="blocktitle"><?php print $block->subject ?></h2>
  
<?php endif;?>

  <div class="content"><?php print $content ?></div>
</div>
</div>