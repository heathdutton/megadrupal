<?php
/** 
* @file
* This is the node.tpl.php template
*  
* This template controls the output of content for each pieces of information within a page.
* 
*
*
*/
?>

 <?php if (!$page): ?>
      <h2><?php print $title; ?></h2>
 <?php endif; ?>
 
 


<?php if ($display_submitted): ?>
	<p class="meta"><?php print $date; ?> <?php print t("by"); ?> <?php print $name; ?></p>
 <?php endif; ?>
     
	<?php print render($content); ?>
      
      
  


