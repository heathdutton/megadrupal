<?php

/**
 * @file
 * A specific handler for CH.
 */

$plugin = array(
  'title' => t('Address form (CN add-on from China Address Field)'),
  'format callback' => 'addressfield_format_address_cn_generate',
  'type' => 'address',
  'weight' => -80,
);

/**
 * Plugin for Address Field.
 */
function addressfield_format_address_cn_generate(&$format, $address, $context = array()) {
  if ($address['country'] == 'CN' && $context['mode'] == 'form') {

    $province = !empty($address['administrative_area']) ? $address['administrative_area'] : '北京';
    $provinces = china_address_provinces();
    $provinces_flip = array_flip($provinces);
    $province_id = isset($provinces_flip[$province]) ? $provinces_flip[$province] : '北京';
    $province_options = array();
    foreach ($provinces as $value) {
      $province_options[$value] = $value;
    }

    // $city = isset($address['city']) ? $address['city'] : '北京';
    $city = !empty($address['locality']) ? $address['locality'] : '北京';
    $cities = china_address_cities($province_id);
    $cities_flip = array_flip($cities);
    $city_options = array();
    foreach ($cities as $value) {
      $city_options[$value] = $value;
    }
    // $city_keys = array_keys($cities);dpm($city_keys);
    if (!in_array($city, $city_options)) {
      // $city = $city_keys[0];
      $city = key($city_options);
    }
    $city_id = isset($cities_flip[$city]) ? $cities_flip[$city] : key($cities);

    // $county = isset($address['county']) ? $address['county'] : '东城区';
    $county = !empty($address['thoroughfare']) ? $address['thoroughfare'] : '东城区';
    $counties = china_address_counties($city_id);
    $counties_flip = array_flip($counties);
    $county_options = array();
    foreach ($counties as $value) {
      $county_options[$value] = $value;
    }
    $county_keys = array_keys($counties);
    if (!in_array($county, $county_keys)) {
      // $county = $county_keys[0];
      $county = key($county_keys);
    }

    $format['locality_block'] = array(
      '#type' => 'addressfield_container',
      '#attributes' => array(
        'class' => array(
          'addressfield-container-inline',
          'locality-block',
          'country-' . $address['country'],
        ),
      ),
      '#weight' => 0,
    );
    $format['locality_block']['administrative_area'] = array(
      '#type' => 'select',
      '#title' => t('Province'),
      '#options' => $province_options,
      '#prefix' => '<div class="china-address province">',
      '#suffix' => '</div>',
      '#attached' => array(
        'css' => array(
          array(
            'data' => '.china-address .form-item {float:left;margin-right:10px;}',
            'type' => 'inline',
          ),
        ),
      ),
    );
    $format['locality_block']['locality'] = array(
      '#type' => 'select',
      '#title' => t('City'),
      '#default_value' => $city,
      '#options' => $city_options,
      '#prefix' => '<div class="china-address city">',
      '#suffix' => '</div>',
    );
    unset($format['street_block']['thoroughfare']);
    $format['locality_block']['thoroughfare'] = array(
      '#type' => 'select',
      '#title' => t('County'),
      '#options' => $county_options,
      '#default_value' => $county,
      '#prefix' => '<div class="china-address county">',
      '#suffix' => '</div>',
    );

    $format['street_block']['premise'] = array(
      '#type' => 'textfield',
      '#title' => t('Detail'),
      '#size' => 30,
    );

    unset($format['locality_block']['postal_code']);
    $format['street_block']['postal_code'] = array(
      '#type' => 'textfield',
      '#title' => t('Postal Code'),
      '#size' => 10,
    );

    $format['locality_block']['administrative_area']['#wrapper_id'] = $format['#wrapper_id'];
    $format['locality_block']['administrative_area']['#process'][] = 'ajax_process_form';
    $format['locality_block']['administrative_area']['#process'][] = 'addressfield_format_address_cn_province_process';
    $format['locality_block']['administrative_area']['#ajax'] = array(
      'callback' => 'addressfield_standard_widget_refresh',
      'wrapper' => $format['#wrapper_id'],
      'method' => 'replace',
    );

    $format['locality_block']['locality']['#wrapper_id'] = $format['#wrapper_id'];
    $format['locality_block']['locality']['#process'][] = 'ajax_process_form';
    $format['locality_block']['locality']['#process'][] = 'addressfield_format_address_cn_province_process';
    $format['locality_block']['locality']['#ajax'] = array(
      'callback' => 'addressfield_standard_widget_refresh',
      'wrapper' => $format['#wrapper_id'],
      'method' => 'replace',
    );
  }
  else {
    if (isset($format['locality_block']['postal_code'])) {
      // Cancel the AJAX for forms we don't control.
      $format['locality_block']['postal_code']['#ajax'] = array();
    }
  }
}

/**
 * Something.
 */
function addressfield_format_address_cn_postal_code_process($element) {
  $element['#limit_validation_errors'] = array($element['#parents']);

  return $element;
}

/**
 * Helper function.
 */
function addressfield_format_address_cn_province_process($element) {
  $element['#limit_validation_errors'] = array($element['#parents']);

  return $element;
}

/**
 * Helper function.
 */
function addressfield_form_cn_postal_code_validation($element, &$form_state, &$form) {
  $data = array(
    '1000' => array('town' => 'Lausanne0', 'canton' => 'VD'),
    '1001' => array('town' => 'Lausanne1', 'canton' => 'VD'),
    '1002' => array('town' => 'Lausanne2', 'canton' => 'VD'),
  );

  // Check if theres something for autocomplete.
  if (!empty($element['#value']) && (isset($data[$element['#value']]))) {
    // Get the base #parents for this address form.
    $base_parents = array_slice($element['#parents'], 0, -1);
    $base_array_parents = array_slice($element['#array_parents'], 0, -2);

    $city = $data[$element['#value']];

    // Set the new values in the form.
    drupal_array_set_nested_value($form_state['values'], array_merge($base_parents, array('locality')), $city['town'], TRUE);
    drupal_array_set_nested_value($form_state['values'], array_merge($base_parents, array('administrative_area')), $city['canton'], TRUE);

    // Discard value the user has already entered there.
    drupal_array_set_nested_value($form_state['input'], array_merge($base_parents, array('locality')), NULL, TRUE);
    drupal_array_set_nested_value($form_state['input'], array_merge($base_parents, array('administrative_area')), NULL, TRUE);
  }
}
