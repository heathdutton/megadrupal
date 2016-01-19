<?php

/**
 * @file
 * Template for the checkout steps widget.
 *
 * Variables:
 * ----------
 *
 * @var $steps
 *   An array of steps to render, in the proper order. Each step contains:
 *     weight: The order of the step.
 *     path: (Optional) If there is a url for this step, it will link to it.
 *     show_link: (Optional) Which pages to show the link from.
 *     title: Title of the step.
 * @var $current
 *   The number of the current step.
 */
?>

<div class='ms_core_checkout_steps'>
  <?php
  $step_num = 0;
  $past = TRUE;
  foreach ($steps as $step_id => $step) {
    $step_num += 1;
    $classes = array('ms_core_checkout_step');
    if ($step_id == $current && $step_id != 'thankyou') {
      $classes[] = 'ms_core_checkout_step_current';
      $past = FALSE;
    } elseif ($past) {
      $classes[] = 'ms_core_checkout_step_past';
    }
    ?>

    <div class='<?php print implode(' ', $classes); ?>'>
        <span class='ms_core_step_number'>
          <?php print $step_num; ?>
        </span>
        <span class='ms_core_step_title'>
          <?php print !empty($step['path']) && in_array($current, $step['show_link']) ? l($step['title'], $step['path']) : $step['title']; ?>
        </span>
    </div>

  <?php
  }
  ?>
</div>
