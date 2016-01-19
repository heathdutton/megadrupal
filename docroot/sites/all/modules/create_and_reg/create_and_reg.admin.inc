<?php
/**
 * @file
 * Create and registration module's admin section.
 */

/**
 * Generates the settings form.
 */
function create_and_reg_settings_form($form, &$form_state) {

  $form['create_and_reg_enabled_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled node types'),
    '#description' => t('Select the node types which you want to use with the module.'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('create_and_reg_enabled_types', array()),
  );

  $form['create_and_reg_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('Redirect the user after the node is created'),
    '#default_value' => variable_get('create_and_reg_redirect', 1),
    '#options' => array(
      t("Don't redirect the user"),
      t('Redirect the user to the login page'),
      t('Redirect the user to the registration page'),
      t('Other url'),
    ),
  );
  $form['create_and_reg_redirect_other_url'] = array(
    '#type' => 'textfield',
    '#title' => t('The url where to redirect'),
    '#default_value' => variable_get('create_and_reg_redirect_other_url'),
    '#states' => array(
      'visible' => array(
       ':input[name="create_and_reg_redirect"]' => array('value' => '3'),
      ),
      'required' => array(
       ':input[name="create_and_reg_redirect"]' => array('value' => '3'),
      ),
    ),
  );

  $form['create_and_reg_disable_status_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable other status messages after the node is created'),
    '#default_value' => variable_get('create_and_reg_disable_status_messages', 0),
    '#description' => t('It will remove all status messages except the added below.')
  );

  $form['create_and_reg_after_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#description' => t('This message will be displayed for the user after he/she created the content.'),
    '#default_value' => variable_get('create_and_reg_after_message'),
  );

  // Token module support.
  if (module_exists('token')) {
    $form['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }

  return system_settings_form($form);
}

/**
 * Validates create_and_reg_settings_form.
 *
 * @param $form
 * @param $form_state
 */
function create_and_reg_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['create_and_reg_redirect'] == 3) {
    if (!trim($form_state['values']['create_and_reg_redirect_other_url'])) {
      form_set_error('create_and_reg_redirect_other_url', t('If you select other url redirect, the url field will be required.'));
    }
    elseif (!valid_url($form_state['values']['create_and_reg_redirect_other_url'])) {
      form_set_error('create_and_reg_redirect_other_url', t('The added url is not valid.'));
    }
    else {
      // Store the normal path instead of the url alias.
      $normal_path = drupal_get_normal_path($form_state['values']['create_and_reg_redirect_other_url']);
      if ($form_state['values']['create_and_reg_redirect_other_url'] != $normal_path) {
        drupal_set_message(t('%link_path has been stored as %normal_path', array('%link_path' => $form_state['values']['create_and_reg_redirect_other_url'], '%normal_path' => $normal_path)));
        $form_state['values']['create_and_reg_redirect_other_url'] = $normal_path;
      }
    }
  }
}
