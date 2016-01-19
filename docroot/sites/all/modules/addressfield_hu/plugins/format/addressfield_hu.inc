<?php

/**
 * @file
 * A specific handler for Hungary.
 */

$plugin = array(
  'title' => t('Address form (Hungary add-on)'),
  'format callback' => 'addressfield_format_address_hu_generate',
  'type' => 'address',
  'weight' => -80,
);

function addressfield_format_address_hu_generate(&$format, $address, $context = array()) {
  if ($address['country'] == 'HU') {

    // Add a special html id need by ajax processes. The standard
    // drupal_html_id() can't works here, because the generated html id's are
    // passed through ajax request, there it is changed on every ajax call.
    $id_count = &drupal_static('address_hu_locality_id_counter', 1);
    $county_id = "address-hu-county-$id_count";
    $locality_html_id = "address-hu-locality-$id_count";
    $postal_code_html_id = "address-hu-postal-code-$id_count";
    $id_count++;

    // In original module the administrative area doesn't exisit. Add this
    // field here.
    $counties = addressfield_hu_get_counties();
    $format['administrative_area'] = array(
      '#title' => t('County'),
      '#options' => $counties,
      '#attributes' => array('class' => array('county')),
    );

    // Customize the address rendering.
    if ($context['mode'] == 'render') {
      $format['administrative_area']['#weight'] = 3;
      $format['locality_block']['#weight'] = 1;
      $format['street_block']['#weight'] = 2;
    }
    if ($context['mode'] == 'form') {
      // Build the options for counties.
      $counties = array_merge(array('' => t('--')), $counties);


      // Extends the administrative area array with form settings.
      $format['administrative_area'] += array(
      // Ajax process to change locality autocomplete path based on this
      // field's value.
        '#ajax' => array(
          'callback' => 'addressfield_hu_rebuild_locality',
          'wrapper' => $locality_html_id,
          'method' => 'replace',
        ),
        '#prefix' => '<div id="' . $county_id . '">',
        '#suffix' => '</div>',
        '#weight' => -9,
        '#size' => 10,
        '#required' => TRUE,
      );

      $format['locality_block']['#weight'] = -1;

      // Overrides of postal code fields.
      $element_postal_code = array(
        '#wrapper_id' => $format['#wrapper_id'],
        '#prefix' => '<div id="' . $postal_code_html_id . '">',
        '#suffix' => '</div>',
        '#weight' => 2,
      );

      // Build the select form element for options it's used to when locality
      // has more than one postal code. - tmp removed
//      $format['locality_block']['postal_code_options'] = $element_postal_code;
//      $element_postal_code_options = array(
//        '#type' => 'select',
//        '#options' => array(),
//        '#weight' => 3,
//      );

      // Choose the form element of postal code by locality. If locality exists
      // and it has more than one postal codes, disable the textfield and vica
      // versa
      $options = addressfield_hu_get_postal_codes_by_locality($address['locality']);

      //-----------------------------------------------------------//
      //          TMP completely remove the options sections       //
      //-----------------------------------------------------------//
//      if ($address['postal_code'] != '') {
//        $element_postal_code['#access'] = TRUE;
//      }
//      else if (!empty($options)) {
//        if (count($options) > 1) {
//          $element_postal_code['#access'] = FALSE;
//          $element_postal_code_options['#options'] = $options;
//          $element_postal_code_options['#default_value'] = $address['postal_code'];
//        }
//        else {
//          $element_postal_code_options['#access'] = FALSE;
//        }
//      }
      // If postal code doesn't exists, disable the postal code field.
//      if (empty($address['postal_code'])) {
//        //$element_postal_code['#access'] = FALSE;
//        $element_postal_code_options['#access'] = FALSE;
//      }

      // Overrides of locality field.
      $autocomplete_path = 'addressfield_hu/autocomplete/';
      $autocomplete_path .= empty($address['administrative_area']) ? 'null' : $address['administrative_area'];
      $element_locality = array(
        '#autocomplete_path' => $autocomplete_path,
        // Add extra class for own js.
        '#attributes' => array('class' => array('addressfield-hu-locality')),
        '#prefix' => '<div id="' . $locality_html_id . '">',
        // If postal_code is exist, no need wrapper to render the hidden postal
        // code field. - tmp disabled the options
        //'#suffix' => !empty($address['postal_code']) ? '</div>' : '</div><div id="' . $postal_code_html_id . '"></div>',
        '#suffix' => '</div>',
        '#size' => '20',
        '#ajax' => array(
          'callback' => 'addressfield_hu_rebuild_postal_code',
          'wrapper' => $postal_code_html_id,
          'method' => 'replace',
        ),
      );


      // Merge overrides with original elements.
      $format['locality_block']['locality'] = array_merge($format['locality_block']['locality'], $element_locality);
      $format['locality_block']['postal_code'] = array_merge($format['locality_block']['postal_code'], $element_postal_code);
      //$format['locality_block']['postal_code_options'] = array_merge($format['locality_block']['postal_code_options'], $element_postal_code_options);

      // Pass some variable to own js, and add own js file. Use a named instead
      // of  __FUNCTION__, if somewhere alteration need.
      $js_settings = &drupal_static('addressfield_hu_js_settings', array());
      global $base_url;
      $js_settings = array(
        'addressfield_hu_fields' => array(
          $context['field']['field_name'] . '[' . $context['langcode'] . ']' . '['. $context['delta'] . ']',
        ),
        'autocomplete_url' => $base_url . '/addressfield_hu/autocomplete/null',
        'counter' => $id_count - 1,
      );
      $format['#attached']['js'][] = array('data' => drupal_get_path('module', 'addressfield_hu') . '/addressfield_hu.js');
      $format['#attached']['js'][] = array(
        'data' => array('addressfield_hu' => $js_settings),
        'type' => 'setting',
      );
      // Add extra class for own js.
      $format['#attributes'] = array('class' => array('addressfield-hu-processed'));
      // Add own validate function, where we will finalize the address array
      // before saving.
      $format['#element_validate'][] = 'addressfield_form_hu_postal_code_validation';
    }
  }
  else {
    if (isset($format['locality_block']['postal_code'])) {
      // Cancel the AJAX for forms we don't control.
      $format['locality_block']['postal_code']['#ajax'] = array();
    }
  }
}

/**
 * Ajax callback if the county field changes.
 *
 * @param type $form
 * @param type $form_state
 * @return array
 *   The $format['locality_block']['locality'] part of
 *   addressfield_format_address_hu_generate() array.
 */
function addressfield_hu_rebuild_locality($form, $form_state) {
  $return = array();
  // $form is not completely built here.
  // The $sender['#parents'][0]][$sender['#parents'][1]][$sender['#parents'][2]
  // is the $form['field_name][language][delta] field form elemenent.
  $sender = $form_state['triggering_element'];
  $field_name = $sender['#parents'][0];
  $field_lang = $sender['#parents'][1];
  $field_delta = $sender['#parents'][2];

  // The addressfield values from $form_state
  $field_element_values = $form_state['values'][$field_name][$field_lang][$field_delta];
  // The locality element within the $form array
  $locality = $form[$field_name][$field_lang][$field_delta]['locality_block']['locality'];

  // Set default value to Budapest
  if ($field_element_values['administrative_area'] == 'BUD') {
    $locality['#autocomplete_path'] = NULL;
    $locality['#default_value'] = 'Budapest';
    $locality['#attributes']['value'] = 'Budapest';
    $locality['#suffix'] = '</div>';
    return $locality;
  }
  // Change the autocomplete path of locality field, and add the original ajax
  // wrapper.
  $path = empty($field_element_values['administrative_area']) ? 'addressfield_hu/autocomplete/null' : 'addressfield_hu/autocomplete/' . $field_element_values['administrative_area'];
  $locality['#default_value'] = '';
  $locality['#attributes']['value'] = '';
  $locality['#autocomplete_path'] = $path;
  // Change the suffix. Originally we add wrapper div to render postal code via
  // ajax. To prevent duplicate postal_code fields, unset it.
  $locality['#suffix'] = '</div>';

  return $locality;

}

/**
 * Ajax callback if the county field changes.
 *
 * @param type $form
 * @param type $form_state
 * @return array
 *   The $format['locality_block']['locality'] part of
 *   addressfield_format_address_hu_generate() array.
 */
function addressfield_hu_rebuild_postal_code($form, $form_state) {
  $return = array();
  // $form is not completely built here.
  // The $sender['#parents'][0]][$sender['#parents'][1]][$sender['#parents'][2]
  // is the $form['field_name][language][delta] field form elemenent.
  $sender = $form_state['triggering_element'];
  $field_name = $sender['#parents'][0];
  $field_lang = $sender['#parents'][1];
  $field_delta = $sender['#parents'][2];

  // The addressfield values from $form_state
  $field_element_values = $form_state['values'][$field_name][$field_lang][$field_delta];

  // The postal code element within the $form array
  $postal_code = $form[$field_name][$field_lang][$field_delta]['locality_block']['postal_code'];
  $locality_block = $form[$field_name][$field_lang][$field_delta]['locality_block'];

  // Get all possible postal codes for the selected locality
  $options = addressfield_hu_get_postal_codes_by_locality($sender['#value']);

  if (!empty($options)) {
    // If more than one possible postal_code exists, show the select widget
    if (count($options) > 1) {
      $postal_code = $form[$field_name][$field_lang][$field_delta]['locality_block']['postal_code_options'];
      $postal_code['#access'] = TRUE;
      $postal_code['#options'] = $options;
    }
    else {
      $postal_code = $form[$field_name][$field_lang][$field_delta]['locality_block']['postal_code'];
      $postal_code['#access'] = TRUE;
      // In ajax callback, only .#default_valuie won't work, need to set
      // attributes too.
      foreach ($options as $key => $value) {
        $postal_code['#default_value'] = $key;
        $postal_code['#attributes']['value'] = $key;
      }
    }
  }
  else {
    // If options doesn't exists, show an empty form.
    $postal_code = $form[$field_name][$field_lang][$field_delta]['locality_block']['postal_code'];
    $postal_code['#access'] = TRUE;
    $postal_code['#default_value'] = '';
    $postal_code['#attributes']['value'] = '';
  }
  $postal_code['#access'] = TRUE;
  return $postal_code;
}

/**
 * Element validation process. This apply all of custumizations to the addresse
 * array.
 */
function addressfield_form_hu_postal_code_validation($element, &$form_state, &$form) {
  // Get the base #parents for this address form.
  $base_parents = array_slice($element['#parents'], 0);
  $address = drupal_array_get_nested_value($form_state['input'], $base_parents);

  // Prepopulate postal_code, if it comes from select.
  if (!empty($address['postal_code_options'])) {
    $address['postal_code'] = $address['postal_code_options'];
    unset($address['postal_code_options']);
  }
  drupal_array_set_nested_value($form_state['values'], array_merge($base_parents, array('postal_code')), $address['postal_code'], TRUE);
}