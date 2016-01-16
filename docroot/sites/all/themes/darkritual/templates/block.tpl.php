<?php
/**
* @file
* This the block template. 
* 
* This template outputs the content of blocks
* 
*/
?>

<?php print render($title_prefix); ?>
 <?php if ($block->subject): ?>
  <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
<?php endif;?>
     <?php print render($title_suffix); ?> 
     <?php print $content_attributes; ?>
     <?php print $content; ?>
	      


