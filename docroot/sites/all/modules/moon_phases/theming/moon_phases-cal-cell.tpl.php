<?php
/**
 * @file
 * Displays a day in the calendar
 * 
 * Available variables:
 * - $first: A string, either "first" or NULL
 * - $week: an array containing phase data and the moon image
 *   - image: The HTML image of the moon phase
 *   - day: A numeric representation of the day of the month
 *   - long_date: A string representation of the day of the month
 *   - phase_data
 *     - link: A link to the phase page
 *     - illum: Percentage of moon that is visible
 *     - days: The number of days until the next New moon
 *     - title: The moon phase
 * 
 * @ingroup themeable
 */
?>
<div class="moon-cell <?php print $first; ?> <?php print $class; ?>">
  <div class="moon-cal-date"><?php echo $week[ 'day' ]?></div>
  <?php echo $week[ 'image' ]?>
  <div class="moon-phase">
    <div class="moon-long"><?php echo $week[ 'long_date' ]; ?><br /></div>
    <?php echo $week[ 'phase_data' ][ 'title' ]?>
  </div>
  <div class="moonclear"></div>
</div>