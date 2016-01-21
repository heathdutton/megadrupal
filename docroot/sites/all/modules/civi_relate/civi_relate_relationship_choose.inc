<?php

/* Function to define simple form for jQuery modal dialog or fall back html in dynamic mode.
 */

function civi_relate_relationship_choose_form($form, &$form_state) {
  $form['civi_relate_dynamic_choice'] = array(
    '#type' => 'select',
    '#options' => variable_get('civi_relate_dynamic_relationship_select', array()),
    '#field_prefix' => t('You are the'),
    '#field_suffix' => t('the contact you are creating.'),
  );
  $form['civi_relate_dynamic_source'] = array(
    '#type' => 'hidden',
    '#default_value' => 'no_js',
    '#size' => 5,
  );  // Submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set Relationship'),
    '#submit' => array('civi_relate_relationship_choose_form_submit'),
    '#ajax' => array(
      'callback' => 'civi_relate_relationship_choose_ajax_reply',
      'wrapper' => 'crm-container',
      'method' => 'prepend',
    ),
  );
  return $form;
}

function civi_relate_relationship_choose_form_submit($form, &$form_state) {
  civi_relate_prep();
  $session = &CRM_Core_Session::singleton();
  /* The default value for civi_relate_dynamic_source is no_js. If civi_relate_dynamic_source
   * is js then the form submit is from the jQuery dialog & we just need to stash the value.
   */
  if ($form_state['values']['civi_relate_dynamic_source'] == 'js') {
    $session->set('relationship_type_id', $form_state['values']['civi_relate_dynamic_choice'], 'civi_relate');
    }
  else { // No jQuery dialog - we are post profile submission & need to create the relationship
    /* Pull the params from session
     */
    $selected_profile = variable_get('civi_relate_profile', NULL);
    $params = $session->get('params', 'civi_relate');
    $params['relationship_type_id'] = $form_state['values']['civi_relate_dynamic_choice'];
    civi_relate_create_relationship($params);
//    drupal_goto('civicrm/profile/create?gid=' . $selected_profile . '&reset=1');
  }
}

function civi_relate_relationship_choose_ajax_reply($form, &$form_state) {
  /* Not using a customized confirmation message at this point. 
   */
//  if ($form_state['values']['civi_relate_dynamic_source'] == 'js') {
//    $choices = variable_get('civi_relate_dynamic_relationship_select', array());
//    $display_choice = '<div>' .
//        t('Relationship Set: You are the @choice of this contact', array('@choice' => $choices[$form_state['values']['civi_relate_dynamic_choice']]))
//        . '</div>';
//    return $display_choice;
//  }
}