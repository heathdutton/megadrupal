<?php
/**
 * @file
 * This file define a style of output.
 */

drupal_add_css(drupal_get_path('module', 'google_weather') . '/google_weather.css');
?>

<div class="<?php print $classes; ?>">
  <div class="weather current">
    <div class="location"><?php print $google_weather_info['location']; ?></div>
    <div class="weather-icon">
      <?php print $google_weather_current['icon']; ?>
    </div>
    <div class="weather-info forecast-info ">
      <div class="day"><?php print t("Now"); ?></div>
      <div class="temp"><?php print $google_weather_current['temp']; ?></div>
      <div class="condition"><?php print $google_weather_current['condition']; ?></div>
      <div class="wind"><?php print $google_weather_current['wind_condition']; ?></div>
    </div>
    <div class="clear-block"></div>
  </div>
  
  <?php if ($google_weather_forecast) : ?>
    <div class="separator">
      <?php print t("Weather forecast"); ?>
    </div>
  <?php endif; ?>

  <?php foreach ($google_weather_forecast as $id => $forecast): ?>
    <div class="weather forecast forecast-<?php print $id; ?>">
      <div class="weather-icon float-left">
        <?php print $forecast['icon']; ?>
      </div>
      <div class="weather-info forecast-info">
        <div class="day"><?php print $forecast['day']; ?></div>
        <div class="temp"><?php print $forecast['high_temp']; ?> | <?php print $forecast['low_temp']; ?></div>
        <div class="condition"><?php print $forecast['condition']; ?></div>
      </div>
      <div class="clear-block"></div>
    </div>
  <?php endforeach; ?>
</div>
