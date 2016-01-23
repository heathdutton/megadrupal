<?php
/**
 * @file
 *originally the calendar-item.tpl.php rewritten to display
 * agreservations in a booking calendar showing a matrix of rooms/resources
 * and days / times.
 */
$index = 0;
?>
<!--<pre><?php print_r($continues) ?></pre>-->
<div class="<?php print !empty($item->class) ? $item->class : 'item'; ?>">
  <div class="view-item view-item-<?php print $view->name ?>">
  <div class="<?php print !empty($itemclass) ? $itemclass : 'agreservations-inner'; ?> <?php print $item->granularity; ?>view">
    <?php print theme('agres_availability_stripe_stripe', array('item' => $item)); ?>
    <div class="<?php print $item->date_id ?> contents">
               <?php if (isset($continuation) && $continuation == 1) : ?>
             <span style="text-align:left">&laquo;&nbsp;&nbsp;&nbsp;</span>
        <?php endif;?>
      <?php foreach ($rendered_fields as $field): ?>
        <?php print $field; ?>
      <?php endforeach; ?>
    <?php if (isset($continues) && $continues == 1) : ?>
             
             <span style="text-align:right">&nbsp;&nbsp;&nbsp;&raquo;</span>    
    <?php else : ?>
    &nbsp;
    <?php endif;?>        
    </div>  

  </div> 
  </div>   
</div>