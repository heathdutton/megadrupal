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
 * $formatted_price price with currency sign (blank if not shown see admin screens)
 * $nid node id
 * $vid node vid
 * $calendar_dt date in ISO format YYYY-MM-DD
 * $rate unformatted price of day
 * $available number of rooms available
 * $restrictions html for restrictions
 * $price html for price (blank if not shown see admin screens)
 * $class css class
 * $id css id
 * $day number of day
 *
 */
?><div class="hotel-booking-calendar-view <?php print $class; ?>" id="<?php print $id; ?>">
  <div class="calendar-day-number"><?php print $day; ?></div>
  <?php if ($price) : ?>
    <?php print $price; ?>
  <?php endif; ?>
</div>