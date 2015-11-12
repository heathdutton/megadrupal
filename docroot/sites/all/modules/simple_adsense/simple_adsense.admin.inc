<?php

/**
 * @file
 * Configuration form.
 */

/**
 * Form_simple_adsense.
 */
function simple_adsense_settings($form, &$form_state) {

  $form['#tree'] = TRUE;

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Settings for simple adsense'),
  );

  $form['client'] = array(
    '#type' => 'textfield',
    '#title' => t('Client'),
    '#required' => TRUE,
    '#default_value' => variable_get("simple_adsense_client", NULL),
  );

  $slots = variable_get("simple_adsense_slots", NULL);

  if (empty($form_state['num_slots'])) {
    if (isset($slots)) {
      $form_state['num_slots'] = count($slots);
    }
    else {
      $form_state['num_slots'] = 1;
    }
  }

  for ($i = 1; $i <= $form_state['num_slots']; $i++) {
    $form['slot'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Slot #@num', array('@num' => $i)),
      '#required' => TRUE,
      '#default_value' => isset($slots[$i]) ? $slots[$i] : NULL,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  $form['add_slot'] = array(
    '#type' => 'submit',
    '#value' => t('Add another slot'),
    '#submit' => array('simple_adsense_settings_add_slot'),
  );

  if ($form_state['num_slots'] > 1) {
    $form['remove_slot'] = array(
      '#type' => 'submit',
      '#value' => t('Remove latest slot'),
      '#submit' => array('simple_adsense_settings_remove_slot'),
      '#limit_validation_errors' => array(),
    );
  }

  return $form;
}

/**
 * Add form_simple_adsense_add_slot.
 */
function simple_adsense_settings_add_slot($form, &$form_state) {
  $form_state['num_slots']++;
  $form_state['rebuild'] = TRUE;
}


/**
 * Remove slot.
 */
function simple_adsense_settings_remove_slot($form, &$form_state) {
  if ($form_state['num_slots'] > 1) {
    $form_state['num_slots']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Form validate.
 */
function simple_adsense_settings_validate($form, &$form_state) {

  for ($i = 1; $i <= $form_state['num_slots']; $i++) {
    $slot = $form_state['values']['slot'][$i];
    if (!preg_match('/^\d{10}$/', $slot)) {
      form_set_error("slot][$i", t("slot @i must be 10 digits", array('@i' => $i)));
    }
  }

  $client = $form_state['values']['client'];
  if (!preg_match('/^ca-pub-\d+$/', $client)) {
    form_set_error("client", t('client string must be like this ca-pub-9513614146655499'));
  }
}

/**
 * Form submit.
 */
function simple_adsense_settings_submit($form, &$form_state) {

  $slots = array();

  for ($i = 1; $i <= $form_state['num_slots']; $i++) {
    $slots[$i] = $form_state['values']['slot'][$i];
  }

  $client = $form_state['values']['client'];

  variable_set('simple_adsense_slots', $slots);
  variable_set('simple_adsense_client', $client);

  $output = t("Settings have been saved successfully, go to !blocks next.",
    array(
      "!blocks" => l(
      t("Blocks administration Page"),
      "admin/structure/block"),
    )
    );

  drupal_set_message($output);

}
