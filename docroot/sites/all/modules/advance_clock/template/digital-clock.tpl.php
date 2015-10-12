<?php
/**
 * @file
 * Default simple digital clock template to display a list of rows.
 *
 * - $clock : array which holds the clock id and the clock format informatioin.
 * @ingroup advance_clock > templates
 */
?>
<?php $clock = $variables['clock']; ?>
<div class="container">
  <span class="clock-icon"></span>
  <div class="clock">
    <div class="advance-clock-location" id="location-<?php print $clock['clock_id']; ?>"><?php print $clock['location']; ?></div>
    <?php if ($clock['show_date'] == TRUE) : ?>
      <div class="advance-clock-date" id="date-<?php print $clock['clock_id']; ?>"></div>
    <?php endif; ?>
    <ul class="clock-wrap">
      <li class="hours" id="hours-<?php print $clock['clock_id']; ?>">
      <li id="point">:</li>  
      <li class="min" id="min-<?php print $clock['clock_id']; ?>">
        <?php if ($clock['show_secs'] == TRUE) : ?>        
        <li id="point">:</li>
        <li class="sec" id="sec-<?php print $clock['clock_id']; ?>">
        <?php endif; ?>
        <?php if ($clock['format'] == 0) : ?>
        <li class="format" id="format-<?php print $clock['clock_id']; ?>">
        <?php endif; ?>
    </ul>
  </div>
</div>
