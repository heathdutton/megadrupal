<div class="clearfix <?php print $classes; ?>"<?php print $attributes; ?> id="<?php print $block_html_id; ?>">
<img class="dark_fold" src="<?php print base_path() . path_to_theme() ?>/images/dark_fold.png" alt="" />
<div class="right_dark_block_inner">
<?php if (!empty($block->subject)): ?>
  <h2 class="blocktitle"><?php print $block->subject ?></h2>
  
<?php endif;?>

  <div class="content"><?php print $content ?></div>
</div>
<img class="right_dark_bottom" src="<?php print base_path() . path_to_theme() ?>/images/right_dark_bottom.jpg" alt="" />

</div>