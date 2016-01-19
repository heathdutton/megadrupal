<?php
/**
 * @file
 * This template handles the layout of the Weathercomau widget block.
 *
 * Variables available:
 * - $forecast: Each forecast contains:
 * - $forecast->day: (string) Day of the week.
 * - $forecast->icon: (string) Icon as an <img />.
 * - $forecast->description: (string) Brief description.
 * - $forecast->min: (string) Minimum temperature.
 * - $forecast->max: (string) Maximum temperature.
 *
 * @ingroup weathercomau_templates
 */
?>
<div class="weathercomau-forecast-day">
  <strong><?php print $forecast->day; ?></strong>
</div>

<div class="weathercomau-forecast-icon">
  <?php print $forecast->icon; ?>
</div>

<div class="weathercomau-forecast-temperature">
  <?php print $forecast->min; ?> &dash; <?php print $forecast->max; ?>
</div>

<div class="weathercomau-forecast-description">
  <?php print $forecast->description; ?>
</div>
