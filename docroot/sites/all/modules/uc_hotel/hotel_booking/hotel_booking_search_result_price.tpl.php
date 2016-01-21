<?php
/*
 * @file hotel_booking_calendar_cell_price.tpl.php
 * Provides price output in calendar cell
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 * variables available
 * $caption the caption for the price
 * $price the price
 * $class class of price
 *
 *
 */
?><div class="hotel-booking-search-price-outer <?php print $class?>">
  <?php if ($caption) : ?>
    <div class="hotel-booking-search-price-caption"><?php print $caption; ?></div>
  <?php endif; ?>
  <div class="hotel-booking-search-price-price"><?php print $price; ?></div>
</div>