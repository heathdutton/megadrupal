<?php

/**
 * @file
 * Default simple view popup template to as an HTML ol list
 *
 * @ingroup views_popup_templates
 */
?>
<div class='<?php print $class; ?>'>
 <?php print $header; ?>
 <ol>
  <?php foreach ($items as $item) {?>          
  <li class='views-popup-item'><?php print $item['label'].$item['field']; ?></li>
  <?php } ?>
 </ol>
 <?php print $footer; ?>          
</div>
