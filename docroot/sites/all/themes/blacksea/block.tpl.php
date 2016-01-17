<div id="block-<?php print $block->module . '-' . $block->delta; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>><div class="block-inner clearfix">

  <?php print render($title_prefix); ?> 
  <?php if ($block->subject): ?>
      <h3 class="title block-title"<?php print $title_attributes; ?>><span><?php print $block->subject ?></span></h3>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  
  <div class="content"<?php print $content_attributes; ?>><div class="content-inner">
    <?php print $content; ?>
  </div></div>

</div></div> <!-- /block-inner, /block -->