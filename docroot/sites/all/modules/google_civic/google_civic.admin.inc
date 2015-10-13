<?php

/**
 * @file Google Civic settings
 * Responsible for the Google Civic Drupal admin interface.
 */

/**
 * Creates the admin form.
 *
 * @return Form
 *   An array containing form items to place on the module settings page.
 */
function google_civic_settings_form($form, &$form_state) {
  $form = array();

  $form['google_civic_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Civic API Key'),
    '#default_value' => variable_get('google_civic_key', ''),
    '#description' => t('Enter your Google Civic API Key. Obtain one from the'
                    . ' !google_api_console',
                    array('!google_api_console' =>
                      l('Google API Console',
                        'https://code.google.com/apis/console/')
                    )
    ),
  );

  $form['google_civic_election_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Civic Election ID'),
    '#default_value' => variable_get('google_civic_election_id', '4000'),
    '#description' => t('Enter the Google Civic election ID to use '
                      . 'when issuing Google Civic requests. Defaults to 4000,'
                      . 'the default key for the U.S. 2012 General Election.'),
  );

  $form['google_civic_election_date'] = array(
    '#type' => 'date_select',
    '#title' => t('Election Day'),
    '#default_value' => variable_get('google_civic_election_date', '2012-11-06 00:00'),
    '#description' => t('Enter the election date (for display).'),
  );

  $form['google_civic_default_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Address'),
    '#default_value' => variable_get('google_civic_default_address', ''),
    '#description' => t('An optional default address for pre-populating the Address search field.
                         Should be a full address, e.g. "123 Main St, Anytown, VA 12345"'),
  );
 
  $form['google_civic_show_contest_info'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show contest information for the provided address. Includes candidates and referendums.'),
    '#return_value' => TRUE,
    '#default_value' => variable_get('google_civic_show_contest_info', FALSE),
  );

  return system_settings_form($form);
}
