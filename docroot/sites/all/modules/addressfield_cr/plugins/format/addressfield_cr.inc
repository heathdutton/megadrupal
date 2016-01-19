<?php

/**
 * @file
 * The default format for Costa Rican adresses.
 */

$plugin = array(
  'title' => t('Costa Rican Address form'),
  'format callback' => 'addressfield_cr_format_address_generate',
  'type' => 'address',
);

/**
 * Format callback for Costa Rican address.
 *
 * @param array $format
 *   Format address.
 * @param array $address
 *   Data of address.
 * @param array $context
 *   Context.
 */
function addressfield_cr_format_address_generate(&$format = array(), $address = array(), $context = array()) {
  if ($address['country'] == 'CR') {

    $id_suffix = '';

    // Get id suffix for tags html.
    if (!empty($format['#wrapper_id'])) {
      $id_suffix = get_suffix_id($format['#wrapper_id']);
    }

    // Remove address fields.
    unset($format['street_block']['premise']);
    unset($format['street_block']['thoroughfare']);
    unset($format['locality_block']['administrative_area']);
    unset($format['locality_block']['locality']);
    unset($format['locality_block']['thoroughfare']);
    unset($format['locality_block']['postal_code']);
    unset($format['locality_block']['name_line']);

    // Form to select the way to register the address.
    $format['locality_block']['name_line'] = array(
      '#id' => 'name_line' . $id_suffix,
      '#title' => t('Enter Address'),
      '#empty_option' => t('- Select an option -'),
      '#options' => array('1' => t('By District'), '2' => t('By ZIP Code')),
      '#required' => TRUE,
      '#attributes' => array('class' => array('name_line' . $id_suffix)),
      '#weight' => -2,
      '#render_option_value' => TRUE,
    );
    if (!empty($address['name_line'])) {

      // If the option "postal code" is selected.
      if ($address['name_line'] === '2') {
        // Field for search postal code.
        $format['locality_block']['postal_code'] = array(
          '#title' => t('ZIP Code'),
          '#size' => 15,
          '#required' => TRUE,
          '#attributes' => array('class' => array('postal_code')),
          '#weight' => -1,
          '#render_option_value' => TRUE,
        );
        // Costa Rican provinces.
        $format['locality_block']['administrative_area'] = array(
          '#title' => t('Province'),
          '#size' => 20,
          '#required' => TRUE,
          '#attributes' => array('class' => array('province')),
          '#weight' => 2,
          '#render_option_value' => TRUE,
          '#disabled' => TRUE,
        );
        // Costa Rican cantons.
        $format['locality_block']['locality'] = array(
          '#title' => t('Canton'),
          '#size' => 20,
          '#required' => TRUE,
          '#attributes' => array('class' => array('canton')),
          '#weight' => 3,
          '#render_option_value' => TRUE,
          '#disabled' => TRUE,
        );
        // Costa Rican districts.
        $format['locality_block']['thoroughfare'] = array(
          '#title' => t('District'),
          '#size' => 25,
          '#required' => TRUE,
          '#attributes' => array('class' => array('canton')),
          '#weight' => 4,
          '#render_option_value' => TRUE,
          '#disabled' => TRUE,
        );
        // Field for other signs of the address.
        $format['locality_block']['premise'] = array(
          '#id' => 'txt-signs-' . $id_suffix,
          '#title' => t('Additional Information'),
          '#weight' => 10,
          '#attributes' => array(
            'class' => array('other'),
          ),
          '#size' => 40,
          '#required' => TRUE,
        );
        // If an postal code has been entered.
        if (!empty($address['postal_code'])) {
          $postal_code = $address['postal_code'];
          // Obtain the province, canton and province of the postal code.
          $address_result = address_cr_addresses(5, '', '', '', $postal_code);
          list($province, $canton, $district) = explode("-", $address_result);
          $format['locality_block']['thoroughfare']['#value'] = $district;
          $format['locality_block']['locality']['#value'] = $canton;
          $format['locality_block']['administrative_area']['#value'] = $province;
        }
        // If render address add a suffix.
        if ($context['mode'] == 'render') {
          $format['locality_block']['postal_code']['#suffix'] = ', ';
        }

        // Check ajax form.
        if ($context['mode'] == 'form') {
          $format['locality_block']['postal_code']['#ajax'] = array(
            'callback' => 'addressfield_standard_widget_refresh',
            'wrapper' => $format['#wrapper_id'],
            'method' => 'replace',
          );
        }
        else {
          if (isset($format['locality_block']['postal_code'])) {
            $format['locality_block']['postal_code']['#ajax'] = array();
          }
        }
      }
      else {
        // Costa Rican provinces.
        $format['locality_block']['administrative_area'] = array(
          '#id' => 'province' . $id_suffix,
          '#title' => t('Province'),
          '#empty_option' => t('- Seleccione una provincia -'),
          '#required' => TRUE,
          '#options' => address_cr_addresses(1, '', '', '', ''),
          '#weight' => 1,
          '#render_option_value' => TRUE,
        );

        // Costa Rican cantons.
        $format['locality_block']['locality'] = array(
          '#id' => 'canton' . $id_suffix,
          '#title' => t('Canton'),
          '#empty_option' => t('- Seleccione un cantón -'),
          '#options' => array(),
          '#required' => TRUE,
          '#disabled' => TRUE,
          '#attributes' => array('class' => array('locality')),
          '#weight' => 2,
          '#render_option_value' => TRUE,
        );
        // Costa Rican districts.
        $format['locality_block']['thoroughfare'] = array(
          '#id' => 'district' . $id_suffix,
          '#title' => t('District'),
          '#empty_option' => t('- Select a District -'),
          '#options' => array(),
          '#required' => TRUE,
          '#disabled' => TRUE,
          '#attributes' => array('class' => array('district')),
          '#weight' => 3,
          '#render_option_value' => TRUE,
        );

        // Costa Rican postal codes.
        $format['locality_block']['postal_code'] = array(
          '#title' => t('ZIP Code'),
          '#size' => 15,
          '#required' => TRUE,
          '#attributes' => array('class' => array('thoroughfare')),
          '#weight' => 4,
          '#render_option_value' => TRUE,
          '#value' => '',
          '#disabled' => TRUE,
        );
        // Field for other signs of the address.
        $format['locality_block']['premise'] = array(
          '#title' => t('Additional Information'),
          '#weight' => 10,
          '#attributes' => array(
            'class' => array('other'),
          ),
          '#size' => 40,
          '#required' => TRUE,
        );

        // If a province has been selected then enable locality and load canton.
        if (!empty($address['administrative_area'])) {
          $format['locality_block']['locality']['#disabled'] = FALSE;
          $format['locality_block']['locality']['#options'] = address_cr_addresses(2, $address['administrative_area'], '', '', '');
          $format['locality_block']['thoroughfare']['#disabled'] = TRUE;
        }
        // If a canton has been selected then enable locality and load districts
        // of this canton.
        if (!empty($address['locality'])) {
          $format['locality_block']['thoroughfare']['#disabled'] = FALSE;
          $format['locality_block']['thoroughfare']['#options'] = address_cr_addresses(3, '', $address['locality'], '', '');
        }
        // If a district has been selected then get the postal code of this
        // district.
        if (!empty($address['thoroughfare'])) {
          $format['locality_block']['postal_code']['#value'] = address_cr_addresses(4, '', $address['locality'], $address['thoroughfare'], '');
        }

        // If render address add a suffix.
        if ($context['mode'] == 'render') {
          $format['locality_block']['administrative_area']['#suffix'] = ', ';
        }
        // Check ajax form.
        if ($context['mode'] == 'form') {
          $format['locality_block']['administrative_area']['#ajax'] = array(
            'callback' => 'addressfield_standard_widget_refresh',
            'wrapper' => $format['#wrapper_id'],
            'method' => 'replace',
          );
          $format['locality_block']['locality']['#ajax'] = array(
            'callback' => 'addressfield_standard_widget_refresh',
            'wrapper' => $format['#wrapper_id'],
            'method' => 'replace',
          );
          $format['locality_block']['thoroughfare']['#ajax'] = array(
            'callback' => 'addressfield_standard_widget_refresh',
            'wrapper' => $format['#wrapper_id'],
            'method' => 'replace',
          );
        }
        else {

          if (isset($format['locality_block']['administrative_area'])) {
            $format['locality_block']['administrative_area']['#ajax'] = array();
          }
          if (isset($format['locality_block']['locality'])) {
            $format['locality_block']['locality']['#ajax'] = array();
          }
          if (isset($format['locality_block']['thoroughfare'])) {
            $format['locality_block']['thoroughfare']['#ajax'] = array();
          }
        }
      }
    }
    // Check ajax form.
    if ($context['mode'] == 'form') {
      $format['locality_block']['name_line']['#ajax'] = array(
        'callback' => 'addressfield_standard_widget_refresh',
        'wrapper' => $format['#wrapper_id'],
        'method' => 'replace',
      );
    }
    else {
      if (isset($format['locality_block']['name_line'])) {
        $format['locality_block']['name_line']['#ajax'] = array();
      }
    }
  }
}

/**
 * Get suffix id for HTML tag.
 *
 * @param string $wrapper_id
 *    Wrapper id of div tag.
 *
 * @return string
 *    Id suffix number of wrapper id.
 */
function get_suffix_id($wrapper_id) {

  $suffix_id = array();

  if ($wrapper_id === 'addressfield-wrapper') {
    $suffix_id[1] = '1';
  }
  else {
    $suffix_id = explode("--", $wrapper_id);
  }

  return $suffix_id[1];
}
