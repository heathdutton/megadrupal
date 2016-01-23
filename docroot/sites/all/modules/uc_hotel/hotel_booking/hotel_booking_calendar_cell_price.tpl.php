<?php
/*
 * @file hotel_booking_calendar_cell_price.tpl.php
 * Provides price output in calendar cell
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 * variables available
 * $minimum_occupancy the minimum occupancy for this date
 * $minimum_stay the minimum stay for this check in date
 * $no_check_in if check in not allowed on this date
 * $no_check_out if check out not allowed on this date
 * $formatted_price price with currency sign
 * $nid node id
 * $vid node vid
 * $calendar_dt date in ISO format YYYY-MM-DD
 * $rate unformatted price of day
 * $available number of rooms available
 *
 *
 */
?><div class="hotel-booking-calendar-price-outer">
  <?php if ($available > 0 && $rate > 0) : ?>
    <div class="hotel-booking-calendar-price"><?php print $formatted_price; ?></div>
  <?php else: ?>
    <div class="hotel-booking-calendar-no-price">&nbsp;</div>
  <?php endif; ?>
</div>