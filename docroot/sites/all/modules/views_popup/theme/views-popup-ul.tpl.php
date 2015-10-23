<?php

/**
 * @file
 * Default simple view popup template to as an HTML ul list
 *
 * @ingroup views_popup_templates
 */
?>
<div class='<?php print $class; ?>'>
 <?php print $header; ?>
 <ul>
  <?php foreach ($items as $item) {?>          
  <li class='views-popup-item'><?php print $item['label'].$item['field']; ?></li>
  <?php } ?>
 </ul>
 <?php print $footer; ?>          
</div>
