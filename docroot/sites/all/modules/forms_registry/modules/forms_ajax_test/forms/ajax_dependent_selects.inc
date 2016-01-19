<?php
/**
 * @file
 * Ajax form example.
 */

/**
 * Creates a dependent dropdown select, with maximum three steps.
 *
 * Initially, there is only one dropdown. After the first is completed, a second
 * one will appear, then a third one and in the end a submit button.
 */
function forms_ajax_test_dependent_select_form($form, $form_state) {
  //get the continents
  $continents = forms_ajax_test_get_continents();
  $form['continents'] = array(
    '#type' => 'select',
    '#options' => $continents,
    '#title' => t('Continents'),
    '#description' => t('Pick one continent'),
    '#ajax' => array(
      'callback' => 'forms_ajax_test_country_ajax',
      'wrapper' => 'geolocations-wrapper',
    )
  );

  //save the continents options for later use
  $form['continents_options'] = array(
    '#type' => 'value',
    '#value' => $continents,
  );

  //ajax wrapper
  $form['ajax_wrapper'] = array(
    '#type' => 'container',
    '#id' => 'geolocations-wrapper',
  );

  if (isset($form_state['values']['continents'])) {
    //get the countries
    $countries = forms_ajax_test_get_countries($form_state['values']['continents']);
    if  (!empty($countries)) {
      $form['ajax_wrapper']['countries'] = array(
        '#type' => 'select',
        '#options' => $countries,
        '#title' => t('Countries'),
        '#description' => t('Pick one country'),
        '#ajax' => array(
          'callback' => 'forms_ajax_test_city_ajax',
          'wrapper' => 'geolocations-wrapper',
        ),
      );

      //save the countries options for later use
      $form['countries_options'] = array(
        '#type' => 'value',
        '#value' => $countries,
      );
      //we display the cities list if we the form has been submitted via AJAX
      //by selecting a country of if the countries list has only one option
      //because then the AJAX submit is never triggered
      if (isset($form_state['values']['countries']) || count($countries) == 1) {
        //get the cities
        if (isset($form_state['values']['countries'])) {
          //get the country from submitted form state values
          $country = $form_state['values']['countries'];
        }
        else {
          //get the country from the list
          $country_codes = array_keys($countries);
          $country = reset($country_codes);
        }
        $cities = forms_ajax_test_get_cities($country);
        if  (!empty($cities)) {
          $form['ajax_wrapper']['cities'] = array(
            '#type' => 'select',
            '#options' => $cities,
            '#title' => t('Cities'),
            '#description' => t('Pick one city'),
          );

          //save the continents options for later use
          $form['cities_options'] = array(
            '#type' => 'value',
            '#value' => $cities,
          );

          $form['ajax_wrapper']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Submit form'),
            '#submit' => array('forms_ajax_test_final_submit'),
            '#validate' => array('forms_ajax_test_final_submit_validate'),
          );
        }
      }
    }
  }

  return $form;
}

/**
 * AJAX handler for countries loading.
 */
function forms_ajax_test_country_ajax($form, &$form_state) {
  return $form['ajax_wrapper'];
}

/**
 * AJAX handler for cities loading.
 */
function forms_ajax_test_city_ajax($form, &$form_state) {
  return $form['ajax_wrapper'];
}

/**
 * Form validation handler.
 *
 * Check to see if any of the options selected are valid
 */
function forms_ajax_test_final_submit_validate($form, $form_state) {
  $continent_code = $form_state['values']['continents'];
  if (!isset($form_state['values']['continents_options'][$continent_code])) {
    form_set_error('continents', t("The selected continent isn't valid"));
  }

  $country_code = $form_state['values']['countries'];
  if (!isset($form_state['values']['countries_options'][$country_code])) {
    form_set_error('countries', t("The selected country isn't valid"));
  }

  $city_code = $form_state['values']['cities'];
  if (!isset($form_state['values']['cities_options'][$city_code])) {
    form_set_error('cities', t("The selected city isn't valid"));
  }
}

/**
 * Form submit hanlder.
 *
 * Sets a message to display in page.
 */
function forms_ajax_test_final_submit($form, &$form_state) {
  $continent_code = $form_state['values']['continents'];
  $continent = $form_state['values']['continents_options'][$continent_code];

  $country_code = $form_state['values']['countries'];
  $country = $form_state['values']['countries_options'][$country_code];

  $city_code = $form_state['values']['cities'];
  $city = $form_state['values']['cities_options'][$city_code];

  drupal_set_message(t('Hello. Your location has been updated to') . ': ' . $city . ', ' . $country . ', ' . $continent);
}