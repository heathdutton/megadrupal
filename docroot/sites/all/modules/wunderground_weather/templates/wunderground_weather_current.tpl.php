<?php
/**
 * @file
 * Theme for displaying current weather conditions.
 *
 * Available variables:
 * - $image: An icon image to represent current weather conditions.
 * - $summary: An unordered list describing the current weather conditions.
 *
 * @see template_preprocess()
 * @see template_preprocess_wunderground_weather_current()
 * @see template_process()
 */
?>

<div class="current-weather-block-content">

  <?php if ($image): ?>
    <?php print $image; ?>
  <?php endif; ?>

  <?php if ($summary): ?>
    <?php print $summary; ?>
  <?php endif; ?>
</div>
