<?php

/**
 * @file
 * Administration part of Form default button.
 */

/**
 * Form constructor to configure form default buttons.
 */
function form_default_button_settings($form, &$form_state) {
  $form['forms']['#tree'] = TRUE;
  $form['forms']['#theme'] = 'form_default_button_forms_table';

  // Configuration options for the individual form.
  $button_default_action_options = array(
    'browser_default' => t('Browser default'),
    'none' => t('Do nothing'),
    'click' => t('Click element'),
  );

  $button_conf_all = db_query('SELECT * FROM {form_default_buttons} ORDER BY form_id ASC');
  foreach ($button_conf_all as $button_conf) {
    $existing_form = 'existing-' . $button_conf->form_id;
    $configuration = unserialize($button_conf->configuration);

    $form['forms'][$existing_form]['form_id_value'] = array(
      '#type' => 'value',
      '#value' => $button_conf->form_id,
    );

    $form['forms'][$existing_form]['form_id'] = array(
      '#type' => 'markup',
      '#markup' => $button_conf->form_id,
    );

    $form['forms'][$existing_form]['default_action'] = array(
      '#type' => 'select',
      '#default_value' => $button_conf->default_action,
      '#options' => $button_default_action_options,
    );

    $form['forms'][$existing_form]['click_selector'] = array(
      '#type' => 'textarea',
      '#default_value' => isset($configuration['click_selector']) ? $configuration['click_selector'] : '',
      '#rows' => 2,
      '#states' => array(
        'visible' => array(
          ":input[name='forms[existing-$button_conf->form_id][default_action]']" => array('value' => 'click'),
        ),
      ),
      '#description' => t('jQuery selector for the element that should be clicked.<br />
        It must be in the same form, and only the first match will be used.'),
    );

    $form['forms'][$existing_form]['delete'] = array(
      '#type' => 'link',
      '#title' => t('delete'),
      '#href' => "admin/config/user-interface/form-default-button/$button_conf->form_id/delete",
    );
  }

  $form['forms']['new_form']['form_id'] = array(
    '#type' => 'textfield',
    '#size' => 16,
  );

  $form['forms']['new_form']['default_action'] = array(
    '#type' => 'select',
    '#default_value' => 'browser_default',
    '#options' => $button_default_action_options,
  );

  $form['forms']['new_form']['click_selector'] = array(
    '#type' => 'textarea',
    '#default_value' => '',
    '#rows' => 2,
    '#states' => array(
      'visible' => array(
        ":input[name='forms[new_form][default_action]']" => array('value' => 'click'),
      ),
    ),
    '#description' => t('jQuery selector for the element that should be clicked.
      It must be in the same form, and only the first match will be used.'),
  );

  $form['forms']['new_form']['delete'] = array();

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form validate callback for configuring form default buttons.
 */
function form_default_button_settings_validate($form, &$form_state) {
  $new_form = $form_state['values']['forms']['new_form'];

  if (!empty($new_form['form_id'])) {
    // Validate the FORM_ID of the new form.
    if (preg_match('/[^a-z0-9_]+/', $new_form['form_id'])) {
      $error_message = t('FORM_ID must contain only alphanumeric characters and underscores.');
      form_set_error('forms][new_form][form_id', $error_message);
    }
  }
}

/**
 * Form submit callback for configuring form default buttons.
 */
function form_default_button_settings_submit($form, &$form_state) {
  $forms = $form_state['values']['forms'];
  $new_form = $forms['new_form'];
  unset($forms['new_form']);

  foreach ($forms as $form_settings) {
    // Update existing.
    $configuration = form_default_button_configuration_serialize($form_settings);
    db_merge('form_default_buttons')
      ->key(array('form_id' => $form_settings['form_id_value']))
      ->fields(array('default_action' => $form_settings['default_action'], 'configuration' => $configuration))
      ->execute();
  }

  if (!empty($new_form['form_id'])) {
    // Create new Id.
    $configuration = form_default_button_configuration_serialize($new_form);
    db_merge('form_default_buttons')
      ->key(array('form_id' => $new_form['form_id']))
      ->fields(array('default_action' => $form_settings['default_action'], 'configuration' => $configuration))
      ->execute();
  }
}

/**
 * Helper function to get serialized configurations.
 */
function form_default_button_configuration_serialize($form_settings) {
  $configuration = array();
  if ($form_settings['default_action'] == 'click') {
    $configuration['click_selector'] = $form_settings['click_selector'];
  }
  return serialize($configuration);
}

/**
 * Form constructor to delete a form default button configuration.
 */
function form_default_button_delete($form, &$form_state, $button_form_id) {
  $form['button_form_id'] = array(
    '#type' => 'value',
    '#value' => $button_form_id,
  );
  $question = t('Are you sure you want to remove %button_form_id default button?', array('%button_form_id' => $button_form_id));
  $path = 'admin/config/user-interface/form-default-button';
  return confirm_form($form, $question, $path);
}

/**
 * Form submit callback to delete a form default button configuration.
 */
function form_default_button_delete_submit($form, &$form_state) {
  $button_form_id = $form_state['values']['button_form_id'];
  db_delete('form_default_buttons')->condition('form_id', $button_form_id)->execute();
  $form_state['redirect'] = 'admin/config/user-interface/form-default-button';
}

/**
 * Theme function to wrap the configuration form in a single table.
 */
function theme_form_default_button_forms_table($variables) {
  $forms = $variables['form'];
  $header = array('FORM_ID', t('Action'), t('Configuration'), t('Delete'));
  $rows = array();
  foreach (element_children($forms) as $form) {
    $row = array();
    $row[] = drupal_render($forms[$form]['form_id']);
    $row[] = drupal_render($forms[$form]['default_action']);
    $row[] = drupal_render($forms[$form]['click_selector']);
    $row[] = drupal_render($forms[$form]['delete']);
    $rows[] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}
