<?php

/**
 * @file
 * Addressfield specific handler for Romania.
 */

// Addressfield_ro plugin definition.
$plugin = array(
  'title' => t('Address form (Romania add-on)'),
  'format callback' => 'addressfield_ro_format_address_generate',
  'type' => 'address',
  'weight' => 100,
);

/**
 * Format callback plugin implementation for Ro.
 */
function addressfield_ro_format_address_generate(&$format, $address, $context = array()) {
  // Only add this settings for form mode and RO country.
  if ($address['country'] == 'RO' && $context['mode'] == 'form') {
    // Add County as 'administrative_area', this field does not exist.
    $format['locality_block']['administrative_area'] = array(
      '#title' => t('County'),
      '#options' => addressfield_ro_get_counties(),
      '#weight' => -9,
      '#size' => 10,
      '#required' => TRUE,
      // Add ajax to replace locality by county(administrative_area).
      '#ajax' => array(
        'callback' => 'addressfield_ro_update_locality',
        'wrapper' => 'locality-wrapper',
        'method' => 'replace',
      ),
    );

    // Add a wrapper around locality input.
    $locality = &$format['locality_block']['locality'];
    $locality['#prefix'] = '<div id="locality-wrapper">';
    $locality['#suffix'] = '</div>';
    if (!empty($address['administrative_area'])) {
      $locality['#autocomplete_path'] = 'addressfield_ro/autocomplete/' . $address['administrative_area'];
    }
  }
  elseif ($context['mode'] == 'render') {
    // Add counties to be available on render.
    $format['locality_block']['administrative_area']['#options'] = addressfield_ro_get_counties();
  }
}

/**
 * Ajax callback for 'administrative_area' addressfield input element.
 */
function addressfield_ro_update_locality($form, &$form_state) {
  // Get the triggering_element: administrative_area.
  $parents = $form_state['triggering_element']['#array_parents'];
  $county_exists = NULL;
  // Get the selected country.
  if (isset($form_state['triggering_element']['#value'])) {
    $county = $form_state['triggering_element']['#value'];
  }
  else {
    $county = NULL;
  }

  // Copy the parents from administrative_area.
  $locality_parents = $parents;
  // Move one level up.
  array_pop($locality_parents);
  // Navigate to locality_block as locality.
  $locality_parents[] = 'locality';
  $exists = NULL;
  // Get the locality element.
  $locality = drupal_array_get_nested_value($form, $locality_parents, $exists);
  if (!$exists) {
    return;
  }

  // Update the callback to send the selected county.
  $locality['#autocomplete_path'] = 'addressfield_ro/autocomplete/' . $county;
  // Empty the input, as the old value is out of this county context.
  $locality['#value'] = '';

  // Update the locality-wrapper with the new autocomplete callback.
  $commands = array();
  $commands[] = ajax_command_replace("#locality-wrapper", drupal_render($locality));

  return array('#type' => 'ajax', '#commands' => $commands);
}
