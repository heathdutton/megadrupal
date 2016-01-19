<?php

/* 
*  @file block--menu.bar.tpl.php 
*/

?>
<?php $tag = $block->subject ? 'nav' : 'div'; ?>
<<?php print $tag; ?> >
  <?php print render($title_prefix); ?>
  <?php if ($block->subject): ?>
    <h2<?php print $title_attributes; ?>><?php print $block->subject; ?></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
 <div class="pn"> <?php print $content ?></div>
</<?php print $tag; ?>>