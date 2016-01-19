<?php
/**
 * @file
 * Describe hooks provided by the BOM Weather Lite module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Perform alterations before a weather forecast is themed.
 *
 * @param $forecast
 *   A keyed array containing the weather forecast.
 */
function hook_bom_weather_lite_forecast_alter(&$forecast) {
  // The temperature should not dip below absolute zero.
  if ($forecast['maximum'] < -273) {
    $forecast['maximum'] = -273;
  }

  // UV warnings.
  if ($forecast['uv_index'] == 11) {
    $forecast['uv_text'] = t('Crispy');
  }
}

/**
 * @} End of "addtogroup hooks".
 */
