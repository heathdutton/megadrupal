<?php

/**
 * @defgroup curlypage_hooks Curlypage hook functions
 *
 * Hooks for the Curlypage module.
 */

/**
 * @file
 * API documentation file.
 *
 * @ingroup curlypage_hooks
 */

/**
 * Allows other modules to add elements to the default Curlypage settings page.
 *
 * @param &$form
 *  The $form array for Curlypage settings passed by reference.
 *  Normally, you should include your form elements inside a new fieldset.
 * @param $form_state
 *  The state of the form.
 *
 * @return
 *  No return value. The form is modified by reference.
 *
 * @ingroup curlypage_hooks
 */
function hook_curlypage_form(&$form, $form_state) {

  // Add the form element to the main screen.
  $form['curlypage_mymodule'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mymodule settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
    );

  $form['curlypage_mymodule']['mymodule_setting'] = array(
    '#type' => 'textfield',
    '#title' => t('Mymodule setting variable'),
    '#default_value' => $mymodule_setting_default,
    '#description' => t('Your description goes here.'),
    '#maxlength' => 3,
    '#size' => 8,
  );

}

/**
 * Allows other modules to validate their added settings.
 *
 * @param $form
 *  The Curlypage $form settings array.
 * @param &$form_state
 *  The state of the form.
 *
 * @return
 *  No return value.
 *
 * @ingroup curlypage_hooks
 */
function hook_curlypage_form_validate($form, &$form_state) {

  // mymodule setting validation
  if (!is_numeric($form_state['values']['mymodule_setting']) || $form_state['values']['mymodule_setting'] < 0) {
    form_set_error('mymodule_setting', t('My module Setting must be a number between 0 and 999'));
  }

}

/**
 * Allows other modules to submit their added settings.
 *
 * @param $form
 *  The Curlypage $form settings array.
 * @param &$form_state
 *  The state of the form.
 *
 * @return
 *  No return value.
 *
 * @ingroup curlypage_hooks
 */
function hook_curlypage_form_submit($form, &$form_state) {

  $cpid = $form_state['values']['cpid'];

  db_insert('curlypage_mymodule')
    ->fields(array(
      'cpid' => $cpid,
      'mymodule_setting' => $form_state['values']['mymodule_setting'],
    ))
    ->execute();

}

/**
 * Notify other modules that a curlypage has been removed.
 *
 * @param $curlypage
 *  The curlypage that has been removed.
 *
 * @return
 *  No return value.
 *
 * @ingroup curlypage_hooks
 */
function hook_curlypage_delete($curlypage) {

  db_delete('curlypage_mymodule')
    ->condition('cpid', $curlypage->cpid)
    ->execute();

}

/**
 * Notify other modules that the curlypage has been shown.
 *
 * @param $curlypage
 *  The curlypage that has been shown.
 * @param $status
 *  The status of the curlypages:
 *   - "close": The curlypage has been viewed closed.
 *   - "open": The curlypage has been viewed totally opened.
 *
 * @return
 *  No return value.
 *
 * @ingroup curlypage_hooks
 */
function hook_curlypage_view($curlypage, $status) {

  switch ($status) {
    case 'close':
      watchdog('curlypage', 'The curlypage %curlypage has been shown.', array('%curlypage' => $curlypage->name), WATCHDOG_INFO);
      break;
    case 'open':
      watchdog('curlypage', 'The curlypage %curlypage has been shown totally opened.', array('%curlypage' => $curlypage->name), WATCHDOG_INFO);
      break;
  }

}

/**
 * Notify other modules that the curlypage has been clicked.
 *
 * @param $curlypage
 *  The curlypage clicked.
 *
 * @return
 *  No return value.
 *
 * @ingroup curlypage_hooks
 */
function hook_curlypage_click($curlypage) {

  watchdog('curlypage', 'The curlypage %curlypage has been clicked.', array('%curlypage' => $curlypage->name), WATCHDOG_INFO);

}

/**
 * Enable a curlypage
 *
 * @param $cpid
 *  The curlypage unique identifier
 *
 * @return
 *  No return value.
 */
function curlypage_curlypage_enable($cpid) {
  db_update('curlypages')
    ->fields(array(
      'status' => 1,
    ))
    ->condition('cpid', $cpid)
    ->execute();
  variable_set('num_enabled_curlypages', variable_get('num_enabled_curlypages', 0) + 1);
}

/**
 * Disable a curlypage
 *
 * @param $cpid
 *  The curlypage unique identifier
 *
 * @return
 *  No return value.
 */
function curlypage_curlypage_disable($cpid) {
  db_update('curlypages')
    ->fields(array(
      'status' => 0,
    ))
    ->condition('cpid', $cpid)
    ->execute();
  variable_set('num_enabled_curlypages', variable_get('num_enabled_curlypages', 1) - 1);
}

/**
 * Add parameters to curlypages
 *
 * @param &$curlypages
 *   Array of curlypage objects.
 *
 * @return
 *   No return value.
 */
function hook_curlypage_parameters(&$curlypages) {

  // Add tracking parameter to each curlypage.
  $result = db_query('SELECT cpid, my_parameter FROM {curlypage_mymodule}');
  foreach ($result as $curlypage) {
    $curlypages[$curlypage->cpid]->my_parameter = $curlypage->my_parameter;
  }

}
