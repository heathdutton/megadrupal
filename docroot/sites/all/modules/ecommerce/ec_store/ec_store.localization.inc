<?php

/**
 * @file
 * All functions that are related to regional settings.
 */

// -------------------------------------------------------------------------
// Settings

/**
 * Set all Location settings, like the used physical
 * dimenstions and currency format
 *
 * @return
 *   Array. The form settings array.
 */
function ec_store_location_settings_form($form, &$form_state) {
  $countries = ec_store_region_country_list();
  $form['ec_country'] = array(
    '#type' => 'select',
    '#title' => t('Store location'),
    '#default_value' => variable_get('ec_country', 'us'),
    '#options' => $countries,
    '#multiple' => FALSE,
    '#description' => 'Default country location.',
  );

  // Physical dimensions
  $form['dimensions'] = array(
    '#collapsible' => TRUE,
    '#title' => t('Physical Dimensions'),
    '#tree' => FALSE,
    '#type' => 'fieldset',
  );
  $form['dimensions']['ec_measure_weight'] = array(
    '#default_value' => variable_get('ec_measure_weight', 'LB'),
    '#description' => t('Default unit measure of weight.'),
    '#options' => _ec_store_location_measures('weight'),
    '#title' => t('Weight'),
    '#type' => 'select',
  );
  $form['dimensions']['ec_measure_length'] = array(
    '#default_value' => variable_get('ec_measure_length', 'IN'),
    '#description' => t('Default unit measure of length.'),
    '#options' => _ec_store_location_measures('length'),
    '#title' => t('Length'),
    '#type' => 'select',
  );
  $form['dimensions']['ec_measure_area'] = array(
    '#default_value' => variable_get('ec_measure_area', 'IN2'),
    '#description' => t('Default unit measure of area.'),
    '#options' => _ec_store_location_measures('area'),
    '#title' => t('Area'),
    '#type' => 'select',
  );
  $form['dimensions']['ec_measure_volume'] = array(
    '#default_value' => variable_get('ec_measure_volume', 'IN3'),
    '#description' => t('Default unit measure of volume.'),
    '#options' => _ec_store_location_measures('volume'),
    '#title' => t('Volume'),
    '#type' => 'select',
  );

  // Price format settings
  $form['price_format'] = array(
    '#collapsible' => TRUE,
    '#title' => t('Price Formatting'),
    '#tree' => FALSE,
    '#type' => 'fieldset',
  );
  $form['price_format']['payment_symbol'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency Symbol'),
    '#default_value' => variable_get('payment_symbol', '$'),
    '#size' => 3,
    '#maxlength' => 5,
    '#description' => t('Enter the currency symbol you wish to associate with your price. This will be displayed in front of the price. Default is the dollar symbol.'),
  );
  $form['price_format']['payment_symbol_position'] = array(
    '#type' => 'radios',
    '#title' => t('Currency Symbol'),
    '#default_value' => variable_get('payment_symbol_position', 1),
    '#options' => array(t('Right'), t('Left')),
    '#description' => t('This option places the currency symbol of the left or right side of the price.'),
  );
  $form['price_format']['payment_thousands'] = array(
    '#type' => 'textfield',
    '#title' => t('Thousands separator'),
    '#default_value' => variable_get('payment_thousands', ','),
    '#size' => 3,
    '#maxlength' => 5,
    '#description' => t('Enter the sign for the thousands separator.'),
  );
  $form['price_format']['payment_decimal'] = array(
    '#type' => 'textfield',
    '#title' => t('Decimal separator'),
    '#default_value' => variable_get('payment_decimal', '.'),
    '#size' => 3,
    '#maxlength' => 5,
    '#description' => t('Enter the sign to seperate real numbers from floating numbers.'),
  );
  $form['price_format']['payment_decimal_places'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of places after the decimal separator'),
    '#default_value' => variable_get('payment_decimal_places', 2),
    '#size' => 3,
    '#maxlength' => 5,
    '#description' => t('How many slots are needed after the decimal?'),
  );

  return system_settings_form($form);
}

// -------------------------------------------------------------------------
// Big nasty arrays

/**
 * List all types of physical measures
 *
 * @return
 *   array keyed on
 *    'name', String, the measure name
 *    'factor', conversion factor to the default (eg. to kilograms or metres)
 */
function _ec_store_location_measures($type = NULL) {
  $measures = array(
    'LB' => array(
      'type' => 'weight',
      'name' => t('Pounds'),
      'factor' => 0.45359237,
    ),
    'G' => array(
      'type' => 'weight',
      'name' => t('Grams'),
      'factor' => 0.001,
    ),
    'KG' => array(
      'type' => 'weight',
      'name' => t('Kilograms'),
      'factor' => 1,
    ),
    'OZ' => array(
      'type' => 'weight',
      'name' => t('Ounces'),
      'factor' => 0.0283495231,
    ),
    'IN' => array(
      'type' => 'length',
      'name' => t('Inches'),
      'factor' => 0.0254,
    ),
    'FT' => array(
      'type' => 'length',
      'name' => t('Feet'),
      'factor' => 0.3048,
    ),
    'CM' => array(
      'type' => 'length',
      'name' => t('Centimetres'),
      'factor' => 0.01,
    ),
    'M' => array(
      'type' => 'length',
      'name' => t('Metres'),
      'factor' => 1,
    ),
    'IN2' => array(
      'type' => 'area',
      'name' => t('Square Inches'),
      'factor' => 0.0254 ^ 2,
    ),
    'FT2' => array(
      'type' => 'area',
      'name' => t('Square Feet'),
      'factor' => 0.3048 ^ 2,
    ),
    'CM2' => array(
      'type' => 'area',
      'name' => t('Square Centimetres'),
      'factor' => 0.01 ^ 2,
    ),
    'M2' => array(
      'type' => 'area',
      'name' => t('Square Metres'),
      'factor' => 1,
    ),
    'IN3' => array(
      'type' => 'volume',
      'name' => t('Cubic Inches'),
      'factor' => 0.0254 ^ 3,
    ),
    'FT3' => array(
      'type' => 'volume',
      'name' => t('Cubic Feet'),
      'factor' => 0.3048 ^ 3,
    ),
    'CM3' => array(
      'type' => 'volume',
      'name' => t('Cubic Centimetres'),
      'factor' => 0.01 ^ 3,
    ),
    'M3' => array(
      'type' => 'volume',
      'name' => t('Cubic Metres'),
      'factor' => 1,
    ),
  );

  if ($type) {
    $list = array();
    foreach ($measures as $measure => $data) {
      if ($data['type'] == $type) {
        $list[$measure] = $data['name'];
      }
    }
    return $list;
  }
  else {
    return $measures;
  }
}

/**
 * Convert some measure units to another. All units are converted
 * to a central unit and them converted again to the destination units.
 *
 * All weight is converted to kilograms and then to the destination.
 * All distance is converted to meters and then to the destination.
 * All area is converted to square meters and then to the destination.
 * All volume is converted to cubic meters and then to the destination.
 *
 * @param $value
 *   Number. The value in the old unit
 * @param $from
 *   String. The unit code that the value is origined
 * @param $to
 *   String. The unit code that the value should be converted to
 * @return
 *   Number. The converted value
 */
function _ec_store_location_measure_convert($value, $from, $to) {
  if ($from == $to) {
    return $value;
  }
  $default = FALSE;
  $measures = _ec_store_location_measures();
  if (!isset($measures[$from]) || !isset($measures[$to]) || $measures[$from]['type'] != $measures[$to]['type']) {
    // Something is screwy.
    watchdog('store', 'e-Commerce failed to convert ' . $measures[$from] . ' to ' . $measures[$to] . '.', WATCHDOG_ERROR);
    return FALSE;
  }
  return $value * $measures[$from]['factor'] / $measures[$to]['factor'];
}
