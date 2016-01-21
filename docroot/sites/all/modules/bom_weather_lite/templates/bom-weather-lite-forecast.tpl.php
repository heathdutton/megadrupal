<?php
/**
 * Template for a BOM Forecast.
 *
 * To customise output, copy this template to your theme's templates directory
 * and change as you see fit.
 *
 * Available variables are:
 * - $identifier: The title of the forecast page, includes the location name.
 * - $maximum:    The predicted maximum temperature in celsius.
 * - $summary:    The text summary of the predicted weather.
 * - $uv_index:   The predicted UV index.
 * - $uv_text:    The text description of the UV index.
 * - $fire:       The predicted fire danger rating.
 * - $date:       A string containing the date this prediction was made.
 * - $icon:       A URL to the icon representation of the summary.
 * - $url:        The BOM website URL used to fetch the data.
 */

?>
<?php if ($maximum): ?>
  <p class="bwl-maximum">Predicted maximum: <a href="<?php print $url; ?>"><?php print $maximum; ?>&degC</a></p>
<?php endif; ?>
<?php if ($icon): ?>
  <img class="bwl-icon" src="<?php print $icon; ?>" <?php if ($summary): ?>alt="<?php print $summary; ?>" title="<?php print $summary; ?>"<?php endif; ?> />
<?php endif; ?>
<?php if ($fire): ?>
  <p class="bwl-fire">Fire danger: <?php print $fire; ?></p>
<?php endif; ?>
<?php if ($uv_index): ?>
  <p class="bwl-uv-index">UV Index: <?php print $uv_index; ?> (<?php print $uv_text; ?>)</p>
<?php endif; ?>
<?php if ($date): ?>
  <p class="bwl-date"><?php print $date; ?></p>
<?php endif; ?>
