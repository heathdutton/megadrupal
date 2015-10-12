<?php
/**
 * @file
 * Admin forms to define and select states.
 */

/**
 * Form constructor for the Define States form.
 *
 * @see context_states_define_states_submit()
 *
 * @ingroup forms
 */
function context_states_define_states_form() {
  $form = array();

  $form['context_states_defined_states'] = array(
    '#type' => 'textarea',
    '#description' => t('Define the available states, one per line.'),
    '#default_value' => variable_get('context_states_defined_states', ''),
    '#title' => t('Defined States'),
  );

  return system_settings_form($form);
}

/**
 * Form constructor for the Edit States form.
 *
 * @see context_states_submit()
 *
 * @ingroup forms
 */
function context_states_form() {
  // Get Defined States and parse into array.
  $states = preg_split('/[\r\n]+/', variable_get('context_states_defined_states', ''), -1, PREG_SPLIT_NO_EMPTY);
  $form = array();

  $form['states'] = array(
    '#type' => 'fieldset',
    '#title' => t('Apply the following state(s):'),
  );

  // If states are defined.
  if (!empty($states[0])) {
    foreach ($states as $key => &$value) {
      $form['states']['states_' . $key . ''] = array(
        '#type' => 'checkbox',
        '#default_value' => variable_get('context_states_states_' . $key),
        '#title' => t("@value", array('@value' => format_string($value))),
      );
    }

    $form['#submit'][] = 'context_states_submit';

    return system_settings_form($form);
  }
  else {
    if (user_access('define states')) {
      $empty_text = t('No states have been defined. <a href="@define_url">Add
    new states</a>.', array('@define_url' => '/admin/structure/states/define'));
    }
    else {
      $empty_text = t('No states have been defined.');
    }
    $form['states']['no_state'] = array(
      '#type' => 'item',
      '#markup' => $empty_text,
    );

    return $form;
  }

}

/**
 * Form submission handler for context_states_form().
 */
function context_states_submit($form, &$form_state) {
  $i = 0;
  while (isset($form_state['values']['states_' . $i])) {
    variable_set('context_states_states_' . $i, $form_state['values']['states_' . $i]);
    $i++;
  }
}
