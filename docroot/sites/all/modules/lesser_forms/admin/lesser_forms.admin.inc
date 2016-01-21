<?php

/**
 * @file
 *
 * Admin settings.
 */

/**
 * Configuration form.
 */
function lesser_forms_configuration_form($form, $form_state) {
  $form = array();
  $settings = variable_get('lesser_forms_config', array());

  $form['lesser_forms_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lesser Forms system settings'),
    '#description' => t('By checking below checkboxes you will customize the UI for following roles:'),
  );

  $form['lesser_forms_config']['applies_to'] = array(
    '#type' => 'checkboxes',
    '#title' => t('User roles'),
    '#description' => t('Apply the Lesser Forms to these user roles.'),
    '#options' => drupal_map_assoc(user_roles()),
    '#default_value' => isset($settings['applies_to']) ? $settings['applies_to'] : array(0),
  );

  $form['lesser_forms_config']['options'] = array(
    '#type' => 'fieldset',
    '#title' => 'Disable these options by checking the checkbox.',
  );

  $form['lesser_forms_config']['options']['promote'] = array(
    '#type' => 'checkbox',
    '#title' => t('Promoted to front page'),
    '#default_value' => isset($settings['promote']) ? $settings['promote'] : 0,
    '#description' => t('Promoted to front page'),
  );

  $form['lesser_forms_config']['options']['sticky'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sticky at top of lists'),
    '#default_value' => isset($settings['sticky']) ? $settings['sticky'] : 0,
    '#description' => t('Sticky at top of lists'),
  );

  $form['lesser_forms_config']['options']['preview'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preview'),
    '#default_value' => isset($settings['preview']) ? $settings['preview'] : 0,
    '#description' => t('Preview'),
  );

  $form['lesser_forms_config']['options']['author'] = array(
    '#type' => 'checkbox',
    '#title' => t('Authoring information'),
    '#default_value' => isset($settings['author']) ? $settings['author'] : 0,
    '#description' => t('Authoring information'),
  );

  $form['lesser_forms_config']['options']['revisioninfo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Revision information'),
    '#default_value' => isset($settings['revisioninfo']) ? $settings['revisioninfo'] : 0,
    '#description' => t('Revision information'),
  );

  $form['lesser_forms_config']['options']['pathauto'] = array(
    '#type' => 'checkbox',
    '#title' => t('URL path settings'),
    '#default_value' => isset($settings['pathauto']) ? $settings['pathauto'] : 0,
    '#description' => t('Generate automatic URL alias'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    // Define our submit handler.
    '#submit' => array('lesser_forms_configuration_form_submit'),
  );

  return $form;

}

/**
 * Custom submit handler.
 */
function lesser_forms_configuration_form_submit(&$form, &$form_state) {

  $settings = variable_get('lesser_forms_config', array());
  $settings = array_merge($settings, $form_state['values']);
  variable_set('lesser_forms_config', $settings);

  drupal_set_message(t('System preferences saved!'));
}
