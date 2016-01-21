<?php
/**
 * @file
 * Functions for full record display options
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Full record display options form
 */
function xc_search_full_record_display_options_form($form, &$form_state) {
  $form['#prefix'] = '<p>' . t('With these options the site dministrator can tweek the loading time of the full record display by moving some parts of the page after the bulk loaded. These parts can work with Ajax calls, and they will not displayed first, gradually but later as usual with Ajax technology.') . '</p>';

  $record = xc_search_get_full_record_display_options();
  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $record->id,
  );

  $form['show_search_form'] = array(
    '#title' => t('Display the search form above the content'),
    '#type' => 'radios',
    '#options' => array(
      '0' => t('Do not show search form above the the content'),
      '1' => t('Show search form above the the content where it is possible.'),
    ),
    '#default_value' => $record->show_search_form,
  );

  $form['show_browse_form'] = array(
    '#title' => t('Display the browse form above the content'),
    '#type' => 'radios',
    '#options' => array(
      '0' => t('Do not show browse form above the the content'),
      '1' => t('Show browse form above the the content where it is possible.'),
    ),
    '#default_value' => $record->show_browse_form,
  );

  $form['show_navigation'] = array(
    '#title' => t('Display the navigation bar above the content'),
    '#type' => 'radios',
    '#options' => array(
      '0' => t('Do not show the navigation bar above the the content'),
      '1' => t('Show the navigation bar above the the content where it is possible.'),
    ),
    '#default_value' => $record->show_navigation,
  );

  $form['mlt_ajax'] = array(
    '#title' => t('Select More Like This display option'),
    '#type' => 'radios',
    '#options' => array(
      'normal' => t('Load it normally with the other parts of the page.'),
      'ajax' => t('Load it after the page were served (Ajax method).'),
    ),
    '#default_value' => $record->mlt_ajax,
  );

  $form['holdings_use_table'] = array(
    '#title' => t('Use table-like display for textual holdings.'),
    '#type' => 'radios',
    '#options' => array(
      '1' => t('True'),
      '0' => t('False'),
    ),
    '#default_value' => $record->holdings_use_table,
  );

  $form['holdings_list_all'] = array(
    '#title' => t('How many textual holdings to be display.'),
    '#type' => 'radios',
    '#options' => array(
      '1' => t('List all'),
      '0' => t('Only the first'),
    ),
    '#default_value' => $record->holdings_list_all,
  );

  $form['xc_search_show_timing_results'] = array(
    '#title' => t('Do display the timing results at the bottom of page (only for admin user).'),
    '#type' => 'radios',
    '#options' => array(
      '1' => t('Display'),
      '0' => t('Do not display'),
    ),
    '#default_value' => variable_get('xc_search_show_timing_results', 0),
  );

  $form['enhancements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enhancements'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Handles full record display options form submit
 *
 * @param $form
 *   The FAPI form definition
 * @param $form_state (array)
 *   The FAPI form values
 */
function xc_search_full_record_display_options_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $keys = array('show_search_form', 'show_browse_form', 'show_navigation', 'mlt_ajax', 'holdings_use_table', 'holdings_list_all');
  $record = new stdClass();
  foreach ($keys as $key) {
    $record->$key = $values[$key];
  }
  if ($values['id'] == 0) {
    drupal_write_record('xc_search_full_record_display', $record);
  }
  else {
    // TODO: we can change it in the future, but now it is a singleton by design
    $record->id = 1;
    drupal_write_record('xc_search_full_record_display', $record, 'id');
  }

  // xc_search_show_timing_results is read in shutdown function, we don't want
  // to add unnecessary plus DB request, so we put it into a variable
  variable_set('xc_search_show_timing_results', $form_state['values']['xc_search_show_timing_results']);
}

