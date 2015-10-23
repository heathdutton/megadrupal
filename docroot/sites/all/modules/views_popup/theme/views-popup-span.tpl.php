<?php

/**
 * @file
 * Default simple view popup template to as a list of spans
 *
 * @ingroup views_popup_templates
 */
?>
<div class='<?php print $class; ?>'>
 <?php print $header; ?> 
 <?php foreach ($items as $item) {?>          
 <span class='views-popup-item'><?php print $item['label'].$item['field']; ?></span>
 <?php } ?>
 <?php print $footer; ?>          
</div>
