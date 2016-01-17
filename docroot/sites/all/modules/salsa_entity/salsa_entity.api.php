<?php

/**
 * @file
 * Hooks provided by the Salsa Entity module.
 */


/**
 * Provide information about salsa object types.
 */
function hook_salsa_object_type_info() {
  return array(
    'supporter' => array(
      'label' => t('Salsa Supporter'),
    ),
    'supporter_groups' => array(
      'label' => t('Salsa Supporter Groups'),
    ),
    'supporter_event' => array(
      'label' => t('Salsa Supporter Event'),
    ),
    'event' => array(
      'label' => t('Salsa Event'),
    ),
    'distributed_event' => array(
      'label' => t('Salsa Distributed Event'),
    ),
    'donation' => array(
      'label' => t('Salsa Donation'),
    ),
    'groups' => array(
      'label' => t('Salsa Groups'),
    ),
    'donate_page' => array(
      'label' => t('Donate Page'),
      'entity class' => 'SalsaEntityDonatePage',
    ),
  );
}

/**
 * Alter salsa object types information.
 *
 * @param $info
 *   The defined salsa object types information.
 *
 * @see hook_salsa_object_type_info()
 */
function hook_salsa_object_type_info_alter(&$info) {
  $info['supporter']['label'] = t('Updated Salsa Supporter');
  $info['supporter_groups']['label'] = t('Updated Salsa Supporter Groups');
  $info['supporter_event']['label'] = t('Updated Salsa Supporter Eventr');
  $info['event']['label'] = t('Updated Salsa Event');
  $info['distributed_event']['label'] = t('Updated Salsa Distributed Event');
  $info['donation']['label'] = t('Updated Salsa Donation');
  $info['groups']['label'] = t('Updated Salsa Groups');
  $info['donate_page']['label'] = t('Updated Donate Page');
}

/**
 * Hook that allows other modules to suggest a current supporter.
 *
 * @param string $mail
 *   E-mail address
 * @return object $supporter
 *   Salsa Supporter
 */
function hook_salsa_entity_current_supporter($mail = NULL) {
  if ($mail == 'example@example.com') {
    return reset(entity_load('salsa_supporter', FALSE, array('Email' => $mail)));
  }
}

/**
 * Hook that allows other modules to set supporter if user is logged in.
 *
 * @param object $supporter
 *   Salsa Supporter
 */
function hook_salsa_entity_supporter_set($supporter) {
  salsa_profile_set_supporter($supporter);
}

/**
 * Allow other modules to alter supporter fieldset.
 *
 * @param $form
 *   The form array.
 * @param $form_state
 *   The  standard associative array containing the current state of the form.
 * @param $form_id
 *   The form ID to find the callback for.
 * @return
 *   Form array.
 */
function hook_salsa_entity_supporter_fieldset_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'example_form') {
    // Add gender.
    $form['gender'] = array(
      '#type' => 'select',
      '#title' => t('Gender'),
      '#options' => array(
        'M' => t('Male'),
        'F' => t('Female'),
      ),
    );
  }
}

/**
 * Alter the formatted currency amount.
 *
 * @param float &$amount
 *   The currency amount as a float.
 */
function hook_salsa_amount_alter(&$amount) {
  $amount .= ' USD';
}
