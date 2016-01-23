<?php

/**
 * @file
 * Theme callbacks for the commerce_price_history module.
 */

/**
 * Theme callback for a price history chart.
 *
 * @param $variables
 *   An associative array containing:
 *   - product_id: The product id.
 *   - chart_data: An array containing price data points.
 *   - chart_title: A string representing the alternative text for our image.
 *   - chart_date_format: A string representing a date format for the X axis
 *     of our image.
 *   - chart_width: An integer represeting our image witdh.
 *   - chart_height: An integer represeting our image height.
 *
 * @ingroup themeable
 */
function theme_commerce_price_history_chart($variables) {
  $output = '';

  if (!empty($variables['chart_data'])) {
    $chart['#title'] = $variables['chart_title'];
    $chart['#attributes']['id'] = 'commerce-price-history-chart-' . $variables['product_id'];
    $chart['#attributes']['class'][] = 'commerce-price-history-chart';

    // Generate data and labels.
    foreach ($variables['chart_data'] as $key => $chart_data) {
      $chart_labels[] = format_date($chart_data['timestamp'], 'custom', $variables['chart_date_format']);
      $chart_points[] = $chart_data['price'];
    }

    // Generate the minimum and maximum scale for the chart.
    $chart_scale = _commerce_price_history_chart_scale($chart_points);

    $chart['#data']['cht'] = 'lc'; // Line chart.
    $chart['#data']['chs'] = $variables['chart_width'] . 'x' . $variables['chart_height'];
    $chart['#data']['chxt'] = 'x,y';
    $chart['#data']['chd'] = 't:' . implode(',', $chart_points);
    $chart['#data']['chds'] = $chart_scale['min'] . ',' . $chart_scale['max'];
    $chart['#data']['chxr'] = '0,' . $chart_scale['min'] . ',' . $chart_scale['max'] . '|1,' . $chart_scale['min'] . ',' . $chart_scale['max'];
    $chart['#data']['chxl'] = '0:|' . implode('|', $chart_labels);
    $chart['#data']['chco'] = '458d15';
    $chart['#data']['chls'] = '3,0,0';
    $chart['#data']['chg'] = (100 / (count($chart_points) - 1)) . ',10,2,3';

    $image_data = array(
      'path' => _commerce_price_history_chart_url($chart['#data']),
      'attributes' => $chart['#attributes'],
      'width' => $variables['chart_width'],
      'height' => $variables['chart_height'],
      'alt' => $chart['#title']
    );

    $output = theme('image', $image_data);
  }

  return $output;
}

/**
 * Returns the min and max values for scaling a chart.
 *
 * @param array $chart_points
 *
 * @return array $chart_scale
 *   - min: The minimum chart value.
 *   - max: The maximum chart value.
 */
function _commerce_price_history_chart_scale($chart_points) {
  $min = min($chart_points);
  $max = max($chart_points);

  $diff = round(($max - $min / 2) / 10);

  $min -= $diff;
  if ($min < 0) {
    $min = 0;
  }
  $max += $diff;

  return array('min' => $min, 'max' => $max);
}

/**
 * Returns a chart URL.
 *
 * @param array $chart_data
 *
 * @return mixed
 *   - Success: Chart image URL
 *   - Failure: FALSE
 */
function _commerce_price_history_chart_url($chart_data = array()) {
  if (!empty($chart_data)) {
    return url('http://chart.apis.google.com/chart', array('query' => $chart_data, 'external' => TRUE));
  }
  return FALSE;
}
