<?php
/**
 * @file
 * Name day block
 *
 * Variables:
 * $date string The preformatted date string
 * nameday string The current nameday
 * joliday string The current holiday
 */

?>
<?php if ($date): ?>
  <span class="nameday-date">
    <?php print $date; ?>,
  </span>
<?php endif; ?>
<span id="nameday"><?php print $name; ?></span>
<?php if ($holiday): ?>
  <span id="holiday"><?php print $holiday; ?></span>
<?php endif; ?>