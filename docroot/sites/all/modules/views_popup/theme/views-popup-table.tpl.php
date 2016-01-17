<?php

/**
 * @file
 * Default simple view popup template to as a vertical table
 *
 * @ingroup views_popup_templates
 */
?>
<div class='<?php print $class; ?>'>
 <?php print $header; ?>
 <table>
  <?php foreach ($items as $item) {?>          
  <tr class='views-popup-item'><td><?php print $item['label'] ?></td><td><?php print $item['field']; ?></td></tr>
  <?php } ?>
 </table>
 <?php print $footer; ?>          
</div>
