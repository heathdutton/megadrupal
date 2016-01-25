<?php

/**
 * @file
 * A specific handler for NL.
 */

$plugin = array(
  'title' => t('Postcode check and addresslookup (NL add-on)'),
  'format callback' => 'addressfield_nl_postcode_generate',
  'type' => 'address',
  'weight' => -80,
);

function addressfield_nl_postcode_generate(&$format, &$address, $context = array()) {
  if ($address['country'] == 'NL' && $context['mode'] == 'form') {
    // defaults.
    $addition_options = array('-99999' => t('None'));
    if (!isset($address['huisnummer'])) {
      $address['huisnummer'] = '';
    }
    if (!isset($address['huisnummer_addition'])) {
      $address['huisnummer_addition'] = '';
    }

    $html_id = drupal_html_id('addressfield-nl-postcode');
    if (isset($address['data']) && strlen($address['data'])) {
      $data = unserialize($address['data']);
      if (!isset($data['huisnummer'])) {
        $address['huisnummer'] = $data['huisnummer'];
      }
      if (!isset($data['huisnummer_addition'])) {
        $address['huisnummer_addition'] = $data['huisnummer_addition'];
      }
      if(isset($data['houseNumberAdditions']) && count($data['houseNumberAdditions']) && reset($data['houseNumberAdditions'])) {
        $addition_options += array_combine($data['houseNumberAdditions'], $data['houseNumberAdditions']);
      }
    }

    $format['postcode'] = array(
      '#type' => 'addressfield_container',
      '#weight' => -8,
      '#attributes' => array(
        'id' => $html_id,
        'class' => array('addressfield-container-inline', 'addressfield-nl-postcode')
      ),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'addressfield_nl_postcode') . '/addressfield_nl_postcode.js',
        ),
        'library' => array(
          array('system', 'drupal.ajax'),
        ),
      ),
    );
    // copy over postalcode
    $format['postcode']['postal_code'] = $format['locality_block']['postal_code'];
    unset($format['locality_block']['postal_code']);

    // adept postal_code
    $format['postcode']['postal_code']['#element_validate'] = array('addressfield_nl_postcode_postal_code_validate');
    $format['postcode']['postal_code']['#required'] = $context['instance']['required'];
    $format['postcode']['postal_code']['#attributes']['id'] = $html_id . '-postal-code';
    $format['postcode']['postal_code']['#attributes']['class'][] = 'nl-postcode';
    $format['postcode']['postal_code']['#attributes']['class'][] = 'postal-code-nl-postcode';

    // add huisnummer
    $format['postcode']['huisnummer'] = array(
      '#type' =>'textfield',
      '#title' => t('House Number'),
      '#size' => 10,
      '#element_validate' => array('addressfield_nl_postcode_huisnummer_validate'),
      '#default_value' => &$address['huisnummer'],
      '#required' => $context['instance']['required'],
      '#attributes' => array(
        'id' => $html_id . '-huisnummer',
        'class' => array('nl-postcode', 'huisnummer-nl-postcode'),
      )
    );

    // add huisnummer addition
    $format['postcode']['huisnummer_addition'] = array(
      '#type' =>'textfield',
      '#title' => t('House Number Addition'),
      '#size' => 10,
      '#default_value' => &$address['huisnummer_addition'],
      '#attributes' => array(
        'id' => $html_id . '-huisnummer-addition',
        'class' => array('nl-postcode', 'huisnummer-addition-nl-postcode'),
      ),
    );

    // disable values that come from service.
    $format['street_block']['thoroughfare']['#disabled'] =
    $format['locality_block']['locality']['#disabled'] = TRUE;
    // hide second address line
    $format['street_block']['premise']['#access'] = FALSE;

  }
}


function addressfield_nl_postcode_huisnummer_validate($element, &$form_state, &$form) {
  $element['#value'] = trim($element['#value']);
  if (!empty($element['#value']) && !preg_match('/^[1-9][0-9]*$/', $element['#value'])) {
    form_error($element, t('@title must be a positive number.', array('@title' => $element['#title'])));
  }
}

function addressfield_nl_postcode_postal_code_validate($element, &$form_state, &$form) {
  $base_parents = array_slice($element['#parents'], 0, -1);
  $data = array(
    'huisnummer' => trim(drupal_array_get_nested_value($form_state['values'], array_merge($base_parents, array('huisnummer')))),
    'huisnummer_addition' => rtrim(drupal_array_get_nested_value($form_state['values'], array_merge($base_parents, array('huisnummer_addition')))),
  );
  if ($data['huisnummer_addition'] == '-99999') {
    $data['huisnummer_addition'] = '';
  }
  drupal_array_set_nested_value($form_state['values'], array_merge($base_parents, array('huisnummer_addition')), $data['huisnummer_addition'], TRUE);

  if (strlen(trim($element['#value'])) && strlen($data['huisnummer'])) {
    $data = array_merge($data, addressfield_nl_postcode_lookup($element['#value'], $data['huisnummer'], $data['huisnummer_addition']));
    if (isset($data['error']) && $data['error']) {
      $data['street'] = '';
      $data['city'] = '';
      if (!isset($data['error_str'])) {
        $data['error_str'] = 'Could not lookup address.';
      }
      form_error($element, t($data['error_str']));
    }

    $street = $data['street'];
    $city = $data['city'];
    if (strlen($street)) {
      $street .= ' ' . $data['huisnummer'];
      $treet = trim($street);
      $street .= ' ' . $data['huisnummer_addition'];
      $treet = trim($street);
    }

    drupal_array_set_nested_value($form_state['values'], array_merge($base_parents, array('thoroughfare')), $street, TRUE);
    drupal_array_set_nested_value($form_state['values'], array_merge($base_parents, array('locality')), $city, TRUE);
  }
  if (!empty($element['#value']) && !empty($data['huisnummer']) && !empty($data['huisnummer_addition'])) {
    drupal_array_set_nested_value($form_state['values'], array_merge($base_parents, array('data')), serialize($data), TRUE);
  }
}