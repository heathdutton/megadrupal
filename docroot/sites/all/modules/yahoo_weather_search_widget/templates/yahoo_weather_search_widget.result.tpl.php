<?php
/**
 * @file
 * Template that renders the weather block.
 *
 * Variable list
 * $location[0]['city'] - City
 * $location[0]['region'] - Region
 * $location[0]['country'] - Country
 *
 * $current['date'] - Current Date
 * $current['temp'] - Current Temperature
 * $current['text'] - Current Condition
 *
 * This get the weather of the next day. Just replace the x with a number,
 * start with 0
 * $forecast[x]['day']
 * $forecast[x]['text']
 * $forecast[x]['high']
 * $forecast[x]['low']
 *
 * @see https://developer.yahoo.com/weather/
 */?>
<div id="yahoo-weather-form" class="results">
  <?php  echo $link; ?>
  <?php foreach($items as $item):
    $current = $item->xpath('yweather:condition');
    $forecast = $item->xpath('yweather:forecast');
    $current = $current[0];
    ?>
    <div class="location">
      <div class="location-item city">
        <?php echo $location[0]['city'] . ','; ?>
        <?php if (isset($location[0]['region']) && !empty($location[0]['region'])):?>
          <span class="location-item region"><?php echo $location[0]['region'] . ','; ?></span>
        <?php endif; ?>
        <span class="location-item country"><?php echo $location[0]['country']; ?></span>
      </div>
    </div>
    <div class="conditions">

      <span class="temp">
        <span class="fahrenheit tempscale">
          <?php echo $current['temp']; ?>&deg;
        </span>
        <span class="celsius tempscale" style="display: none">
          <?php echo yahoo_weather_search_widget_fahrenheit_to_celsius($current['temp']); ?>&deg;
        </span>

      </span>
      <div class="tempscale-buttons">
        <span class="tempscale-button fahrenheit active">F</span>
        <span class="tempscale-button celsius">C</span>
      </div>

      <?php echo yahoo_weather_search_widget_weather_icon($current['code']); ?>

      <span class="condition"><?php echo $current['text']; ?></span>
    </div>
    <div class="weather-buttons result">
      <?php echo l(t('View details'), $item->link, array(
        'attributes' => array(
          'target' => '_blank',
          'class' => array('button'),
        ),
      ));?>
    </div>
  <?php endforeach ?>
</div>
