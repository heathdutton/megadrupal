<?php

/**
 * @file
 * Implements the list administration interface, which can be used to manage the
 * different Campaign Monitor lists.
 */

/**
 * Builds an overview table with basic information about the list and adds
 * action links that can be used to update/delete lists.
 *
 * @return string $html
 */
function campaignmonitor_admin_settings_lists() {
  $account = variable_get('campaignmonitor_account', FALSE);
  if (!$account) {
    drupal_set_message(t('You have not entered your account information yet, hence lists from Campaign Monitor can not be downloaded.'), 'error');
    return '';
  }

  // Download list information from Campaign Monitor.
  $cm = CampaignMonitor::getConnector();
  $lists = $cm->getLists();
  $error = $cm->getLatestError();
  if ($error['code'] != 1) {
    drupal_set_message($error['message'], 'error');
  }

  $header = array(
    array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
    array('data' => t('List ID'), 'field' => 'id'),
    array('data' => t('Subscribed / Unsubscribed'), 'field' => 'status'),
    array('data' => t('Operations'), 'field' => 'Operations'),
  );

  $rows = array();
  if ($lists) {
    foreach ($lists as $id => $list) {
      // Define supported operations.
      $operations = array(
        'Edit' => l(t('Edit'), 'admin/config/services/campaignmonitor/lists/' . $id . '/edit', array('query' => array('token' => drupal_get_token()))),
        'Delete' => l(t('Delete'), 'admin/config/services/campaignmonitor/lists/' . $id . '/delete', array('query' => array('token' => drupal_get_token()))),
      );

      // Load local list options.
      $list_options = variable_get('campaignmonitor_list_' . $id, array());
      if (isset($list_options['status']['enabled']) && !$list_options['status']['enabled']) {
        // Add enable operation.
        $class = 'campaignmonitor-list-disabled';
        $operations['enable'] = l(t('Enable'), 'admin/config/services/campaignmonitor/lists/' . $id . '/enable', array('query' => array('token' => drupal_get_token())));
      }
      else {
        // Add disable operation.
        $class = 'campaignmonitor-list-enabled';
        $operations['disable'] = l(t('Disable'), 'admin/config/services/campaignmonitor/lists/' . $id . '/disable', array('query' => array('token' => drupal_get_token())));
      }

      // Allow other modules to add more operations.
      drupal_alter('campaignmonitor_operations', $operations);

      $stats = $cm->getListStats($id);
      $rows[] = array(
        'data' => array(
          $list['name'],
          $id,
          $stats['TotalActiveSubscribers'] . ' / ' . $stats['TotalUnsubscribes'],
          implode(' ', $operations),
        ),
        'class' => array('class' => $class),
      );
    }
  }

  // Theme the information as a table.
  $html = theme('table',
    array(
      'header' => $header,
      'rows' => $rows,
      'sticky' => TRUE,
      'empty' => t('Lists not found or not yet created...'),
    )
  );

  // Add a pager to the table, for sites that have a great many lists.
  $html .= theme('pager', array('tags' => array()));

  // Give it some styling.
  drupal_add_css(drupal_get_path('module', 'campaignmonitor') . '/css/campaignmonitor.admin.css');

  return ($html);
}

/**
 * List edit callback that returns a form with information about a given list.
 *
 * @param array $form
 * @param array $form_state
 * @param string $list_id
 *   The unique Campaign Monitor list ID.
 *
 * @return array $form
 */
function campaignmonitor_admin_settings_list_edit($form, &$form_state, $list_id) {

  // Confirm that this form has a valid security token present.
  _campaignmonitor_admin_verify_access();

  $form = array('#tree' => TRUE);

  // Get campaign monitor connection.
  $cm = CampaignMonitor::getConnector();

  // Load extended information about the list.
  $list = $cm->getExtendedList($list_id);

  // Add list id to the form.
  $form['listId'] = array(
    '#type' => 'hidden',
    '#value' => $list_id,
  );

  // Set this form name (index).
  $form_key = 'campaignmonitor_list_' . $list_id;

  // Get previously saved list information.
  $defaults = variable_get($form_key, array());

  $form[$form_key]['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enable list'),
    '#description' => t('Enable the list to configure it and use it on the site.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form[$form_key]['status']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => isset($defaults['status']['enabled']) ? $defaults['status']['enabled'] : 1,
    '#attributes' => array('class' => array('enabled-list-checkbox')),
  );

  $form[$form_key]['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('List options'),
    '#description' => t('Changing the values will result in an update of the values on the Campaign Monitor homepage.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
         '.enabled-list-checkbox' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$form_key]['options']['listname'] = array(
    '#type' => 'textfield',
    '#title' => t('List name'),
    '#default_value' => $list['name'],
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="status[enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$form_key]['options']['UnsubscribePage'] = array(
    '#type' => 'textfield',
    '#title' => t('Unsubscribe page'),
    '#default_value' => $list['details']['UnsubscribePage'],
  );

  $form[$form_key]['options']['ConfirmationSuccessPage'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmation success page'),
    '#default_value' => $list['details']['ConfirmationSuccessPage'],
  );

  $form[$form_key]['options']['ConfirmedOptIn'] = array(
    '#type' => 'checkbox',
    '#title' => t('Confirmed Opt In'),
    '#default_value' => $list['details']['ConfirmedOptIn'],
  );

  $form[$form_key]['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        '.enabled-list-checkbox' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$form_key]['display']['name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Name field'),
    '#description' => t('Whether the Name field should be displayed when subscribing.'),
    '#default_value' => isset($defaults['display']['name']) ? $defaults['display']['name'] : 0,
    '#attributes' => array('class' => array('tokenable', 'tokenable-name')),
  );

  // List custom fields.
  if (!empty($list['CustomFields'])) {
    $options = array();
    foreach ($list['CustomFields'] as $key => $field) {
      // Form API can't handle keys with [] in all cases.
      $token_form_key = str_replace(array('[', ']'), '', $key);
      $options[$token_form_key] = $field['FieldName'];
    }

    $form[$form_key]['CustomFields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Custom fields'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#attributes' => array('class' => array('tokenable', 'tokenable-custom-fields')),
      '#states' => array(
        'visible' => array(
          '.enabled-list-checkbox' => array('checked' => TRUE),
        ),
      ),
    );

    $form[$form_key]['CustomFields']['selected'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Available fields'),
      '#description' => t('Select the fields that should be displayed on subscription forms.'),
      '#options' => $options,
      '#default_value' => isset($defaults['CustomFields']['selected']) ? $defaults['CustomFields']['selected'] : array(),
    );
  }

  if (module_exists('token')) {
    $form[$form_key]['tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field tokens'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form[$form_key]['tokens']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name field'),
      '#default_value' => isset($defaults['tokens']['name']) ? $defaults['tokens']['name'] : '[current-user:name]',
      '#states' => array(
        'visible' => array(
          '.tokenable-name' => array('checked' => TRUE),
        ),
      ),
    );

    if (!empty($list['CustomFields'])) {
      foreach ($list['CustomFields'] as $key => $field) {
        if ($field['DataType'] == 'MultiSelectMany') {
          // We can't handle this type of custom field (with tokens).
          continue;
        }

        // Form API can't handle keys with [] in all cases.
        $token_form_key = str_replace(array('[', ']'), '', $key);
        $form[$form_key]['tokens'][$token_form_key] = array(
          '#type' => 'textfield',
          '#title' => t('Custom field (@name)', array('@name' => $field['FieldName'])),
          '#default_value' => isset($defaults['tokens'][$token_form_key]) ? $defaults['tokens'][$token_form_key] : '',
          '#states' => array(
            'visible' => array(
              ':input[name="' . $form_key . '[CustomFields][selected][' . $token_form_key . ']' . '"]' => array('checked' => TRUE),
            ),
          ),
        );
      }
    }

    $form[$form_key]['tokens']['token_tree'] = array(
      '#theme' => 'token_tree',
    );
  }

  // Give the form system look and feel.
  $form = system_settings_form($form);

  // Add validation function.
  $form['#validate'][] = 'campaignmonitor_admin_settings_list_edit_validate';
  return $form;
}

/**
 * Edit form validation handler which calls the API to save the information that
 * was entered. This is done in the validation function so we can give better
 * feedback to the user and to prevent the user from having to enter the
 * information once more on failure.
 *
 * @param array $form
 * @param array $form_state
 * @return boolean FALSE on failure, to prevent submission.
 */
function campaignmonitor_admin_settings_list_edit_validate($form, &$form_state) {
  // Build array with basic information.
  $values = $form_state['values']['campaignmonitor_list_' . $form_state['values']['listId']];
  $options = array(
    'Title' => check_plain($values['options']['listname']),
    'UnsubscribePage' => check_plain($values['options']['UnsubscribePage']),
    'ConfirmedOptIn' => $values['options']['ConfirmedOptIn'] ? TRUE : FALSE,
    'ConfirmationSuccessPage' => check_plain($values['options']['ConfirmationSuccessPage']),
  );

  // Get connected.
  $cm = CampaignMonitor::getConnector();

  // Update the information.
  if (!$cm->updateList($form_state['values']['listId'], $options)) {
    $error = $cm->getLatestError();
    form_set_error('', $error['message']);
    drupal_set_message(t('The list options were not updated correctly at Campaign Monitor.'), 'error');
    return FALSE;
  }

  // Remove list options.
  unset($form_state['values']['campaignmonitor_list_' . $form_state['values']['listId']]['options']);

  // Redirect to list overview.
  $form_state['redirect'] = 'admin/config/services/campaignmonitor/lists';

  // Save display options and custom field selection.
  system_settings_form_submit($form, $form_state);
}

/**
 * List deletion confirmation callback.
 *
 * @param array $form
 * @param array $form_state
 * @param string $list_id
 *   The unique Campaign Monitor list ID.
 *
 * @return array
 *   The confirmation form.
 */
function campaignmonitor_admin_settings_list_delete($form, &$form_state, $list_id) {

  // Confirm that this form has a valid security token present.
  _campaignmonitor_admin_verify_access();

  // Get Campaign Monitor connection.
  $lists = CampaignMonitor::getConnector()->getLists();

  $str = t('You are about to delete the list "@list", which may still have users subscribed to it.<br /><br />This action cannot be undone.', array('@list' => $lists[$list_id]['name']));

  // Build confirmation form.
  $form['list_id'] = array('#type' => 'hidden', '#value' => $list_id);
  $form['#submit'][] = 'campaignmonitor_admin_settings_list_delete_submit';
  $confirm_question = 'Are you sure you want to delete this list ?';
  return confirm_form($form, $confirm_question, 'admin/config/services/campaignmonitor/lists', $str, t('Delete'), t('Cancel'));
}

/**
 * Confirmation form delete callback.
 *
 * @param array $form
 * @param array $form_state
 */
function campaignmonitor_admin_settings_list_delete_submit($form, &$form_state) {
  // Connect to Campaign Monitor.
  $cm = CampaignMonitor::getConnector();

  // Delete the list at Campaign Monitor.
  if ($cm->deleteList($form_state['values']['list_id'])) {
    drupal_set_message(t('The list have been deleted.'), 'status');
  }
  else {
    drupal_set_message(t('The list could not be deleted.'), 'error');
  }

  // Rebuild the blocks cache.
  _block_rehash();

  // Set message and redirect to list overview.
  $form_state['redirect'] = 'admin/config/services/campaignmonitor/lists';
}

/**
 * Build list creation form, which is used to create new lists.
 *
 * @return array $form
 */
function campaignmonitor_admin_settings_list_create_form() {
  $form = array();

  $form['listname'] = array(
    '#type' => 'textfield',
    '#title' => t('List name'),
    '#default_value' => '',
    '#required' => TRUE,
  );

  $form['UnsubscribePage'] = array(
    '#type' => 'textfield',
    '#title' => t('Unsubscribe page'),
    '#default_value' => '',
  );

  $form['ConfirmationSuccessPage'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirmation success page'),
    '#default_value' => '',
  );

  $form['ConfirmedOptIn'] = array(
    '#type' => 'checkbox',
    '#title' => t('Confirmed Opt In'),
    '#default_value' => FALSE,
  );

  $form = system_settings_form($form);
  $form['#submit'] = array('campaignmonitor_admin_settings_list_create_form_submit');
  $form['#validate'][] = 'campaignmonitor_admin_settings_list_create_form_validate';

  return $form;
}

/**
 * Create list validation form handler, which calls the API to create the list.
 * This is done here to ensure better user feedback on failure.
 *
 * @param array $form
 * @param array $form_state
 * @return boolean FALSE on failure
 */
function campaignmonitor_admin_settings_list_create_form_validate($form, &$form_state) {
  $cm = CampaignMonitor::getConnector();
  $result = $cm->createList($form_state['values']['listname'],
                            $form_state['values']['UnsubscribePage'],
                            $form_state['values']['ConfirmedOptIn'],
                            $form_state['values']['ConfirmationSuccessPage']);
  if (!$result) {
    $error = $cm->getLatestError();
    form_set_error('listname', $error['message']);
    return FALSE;
  }
}

/**
 * List creation submit handler, used to set a positive feedback message and
 * rehash the block table, to ensure that the new list subscribe block is
 * available.
 *
 * @param array $form
 * @param array $form_state
 */
function campaignmonitor_admin_settings_list_create_form_submit($form, &$form_state) {
  _block_rehash();
  drupal_set_message(t('List has been created at Campaign monitor.'), 'status');
}

/**
 * Enables the list locally. The function is used as a menu callback in the list
 * tables, when the list have been enabled the user is redirected to the list
 * page.
 *
 * @param string $list_id
 *   The Campaign Monitor list ID.
 */
function campaignmonitor_admin_settings_list_enable($list_id) {

  // Confirm that this form has a valid security token present.
  _campaignmonitor_admin_verify_access();

  // Toggle list enable status.
  campaignmonitor_admin_settings_list_toggle_enable($list_id);

  // Redirect to list overview.
  drupal_goto('admin/config/services/campaignmonitor/lists');
}

/**
 * Disables the list locally. The function is used as a menu callback in the
 * list tables. When the list has been disabled the user is redirected to the
 * list page.
 *
 * @param string $list_id
 *   The Campaign Monitor list ID.
 */
function campaignmonitor_admin_settings_list_disable($list_id) {

  // Confirm that this form has a valid security token present.
  _campaignmonitor_admin_verify_access();

  // Toggle list enable status.
  campaignmonitor_admin_settings_list_toggle_enable($list_id);

  // Redirect to list overview.
  drupal_goto('admin/config/services/campaignmonitor/lists');
}

/**
 * Helper function that enables/disables a given list.
 *
 * @param string $list_id
 *   The Campaign Monitor list ID.
 */
function campaignmonitor_admin_settings_list_toggle_enable($list_id) {
  $list_key = 'campaignmonitor_list_' . $list_id;

  // Get local list information and update enabled state.
  $list_options = variable_get($list_key, array());
  $enable = 0;
  if (isset($list_options['status']['enabled'])) {
    $enable = $list_options['status']['enabled'] == 1 ? 0 : 1;
  }
  $list_options['status']['enabled'] = $enable;
  variable_set($list_key, $list_options);

  // Clear blocks cache.
  _block_rehash();
}

/**
 * Verify that a token is set for CSRF protection.
 */
function _campaignmonitor_admin_verify_access() {

  if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'])) {
    drupal_not_found();
    module_invoke_all('exit');
    exit();
  }
}
