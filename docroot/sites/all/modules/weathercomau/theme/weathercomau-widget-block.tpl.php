<?php
/**
 * @file
 * This template handles the layout of the Weathercomau widget block.
 *
 * Variables available:
 * - $display_current_conditions: (bool) Whether to display current conditions.
 * - $current_conditions: (string) Rendered current conditions.
 * - $display_forecast: (bool) Whether to display forecast.
 * - $forecast: (string) Rendered forecast.
 *
 * @ingroup weathercomau_templates
 */
?>
<?php if ($display_current_conditions): ?>
  <?php print $current_conditions; ?>
<?php endif; ?>

<?php if ($display_forecast): ?>
  <?php print $forecast; ?>
<?php endif; ?>

<div class="weathercomau-credits">
  <?php print $credits; ?>
</div>
