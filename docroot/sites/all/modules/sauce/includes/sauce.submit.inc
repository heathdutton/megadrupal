<?php

/**
 * @file
 * Handles the form submission of the customer survey form
 */


/**
 * Handles what to do with the submitted form depending on what stage has been
 * complated.
 *
 * @see sauce_form()
 * @see sauce_form_validate()
 *
 * @param type $form
 * @param type $form_state
 */
function sauce_form_submit($form, &$form_state) {
 // dsm($form_state['stage']);
  switch ($form_state['stage']) {
    case 'sauce_install':
      $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
      if ($form_state['triggering_element']['#value'] != 'Back') {
        sauce_install_submit($form, $form_state);
        $form_state['new_stage'] = sauce_move_to_next_stage($form, $form_state);
      }
      break;

    case 'sauce_configure':
      $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
      if ($form_state['triggering_element']['#value'] != 'Back') {
        sauce_configure_submit($form, $form_state);
        $form_state['complete'] = TRUE;
      }
      break;

    default:
      $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
      $form_state['new_stage'] = sauce_move_to_next_stage($form, $form_state);
      break;

  }

  if (isset($form_state['complete'])) {
    drupal_set_message("Completed. Continue adding more Sauce to your site.");
    drupal_goto('admin/sauce');
  }

  if ($form_state['triggering_element']['#value'] == 'Back') {
    $form_state['new_stage'] = sauce_move_to_previous_stage($form, $form_state);
  }

  if (isset($form_state['multistep_values']['form_build_id'])) {
    $form_state['values']['form_build_id'] = $form_state['multistep_values']['form_build_id'];
  }
  $form_state['multistep_values']['form_build_id'] = $form_state['values']['form_build_id'];
  $form_state['stage'] = $form_state['new_stage'];
  $form_state['rebuild'] = TRUE;

}
    
function sauce_install_submit($form, &$form_state) {
  // dsm("installing");
  $download_id = $form_state['values']['sauce_installed_item_id'];
  // dsm($download_id);
  $result = xmlrpc('http://drupalsauce.technikh.com/xmlrpc.php', array(
      'sauce.process_download_request' => array($download_id),
  ));
  //dsm($result);
  $sauce_config_markup = sauce_install_from_json($result);
  $form_state['values']['sauce_config_markup_hidden'] = $sauce_config_markup;
}

/**
 * Handles the submission of the final stage
 *
 * Sends an email to the user confirming their entry
 *
 * @param type $form
 * @param type $form_state
 */
function sauce_configure_submit($form, &$form_state) {

  $multstep_values = $form_state['multistep_values'];

  $module = 'sauce';
  $key = 'sauce_complete';

}
