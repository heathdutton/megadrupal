<?php

/**
 * @file
 * This file gives a timeline representation of product status.
 */

  global $base_url;
  $module_path = drupal_get_path('module', 'product_case_tracker');
 ?>
<link rel="stylesheet" type="text/css" href="<?php echo $base_url . '/' . $module_path; ?>/product_case_tracker.css">

<div class="timeline">
<?php
  $step = 0;
  $values = product_case_tracker_status_info($node);
  $dates = product_case_tracker_get_all_dates($node);
?>

<div class="pct_outer">
  <div class="pct_step">
    STEPS
  </div>
  <?php
    foreach ($values['status_values'][0] as $key => $status):
  ?>
  <div class="pct_inner">
   <div class="pct_rotate_status">
     <?php echo $status; ?>
   </div>
   <div class="<?php $last = product_case_tracker_class_name($dates, $key); echo product_case_tracker_class_name($dates, $key); ?>"></div>
      <div class="pct_steps">
        <?php echo ++$step; ?>
      </div>
      <div class="pct_rotate_date">
        <?php echo date('jS M Y', $values['date_values'][$key]); ?>
      </div>
    </div>
    <?php
    endforeach;
  ?>
  <div class="pct_circle" style="background-color:<?php echo $last; ?>"></div>
  <div id="pct_infobar">
	  <div id='pct_red'></div> <div class="info"> Task delayed</div>
  	<div id='pct_yellow'></div> <div class="info"> Task completed with delays</div>
	  <div id='pct_green'></div> <div class="info"> Task successfully completed</div>
  </div>
</div>
