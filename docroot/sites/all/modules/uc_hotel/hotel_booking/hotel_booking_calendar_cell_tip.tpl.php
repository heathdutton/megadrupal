<?php

/*
 * @file hotel_booking_calendar_cell_tip.tpl.php
 * Provides template for theming calendar cell tips (rendered using beauty_tips)
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 * variables available
 * $minimum_occupancy the minimum occupancy for this date
 * $minimum_stay the minimum stay for this check in date
 * $no_check_in if check in not allowed on this date
 * $no_check_out if check out not allowed on this date
 * $nid node id
 * $vid node vid
 * $calendar_dt date in ISO format YYYY-MM-DD
 * $available number of rooms available
 *
 *
 */
?>
<div class="hotel-booking-restrictions">
  <?php if ($minimum_occupancy > 1) : ?>
    <div class="hotel-booking-restriction">
      <?php print t('Minimum guests: !min', array('!min' => $minimum_occupancy)); ?>
    </div>
  <?php endif ?>
  <?php if ($minimum_stay > 1) : ?>
    <div class="hotel-booking-restriction">
      <?php print t('Minimum Stay: !min nights', array('!min' => $minimum_stay)); ?>
    </div>
  <?php endif ?>
  <?php if ($no_check_in) : ?>
    <div class="hotel-booking-restriction">
      <?php print t('No Arrivals'); ?>
    </div>
  <?php endif ?>
  <?php if ($no_check_out) : ?>
    <div class="hotel-booking-restriction">
      <?php print t('No Departures'); ?>
    </div>
  <?php endif ?>
</div>