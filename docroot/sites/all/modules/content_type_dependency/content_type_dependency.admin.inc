<?php
/**
 * @file
 * content_type_dependency.admin.inc
 * Admin configuration forms to create, listing, edit, delete & enable/disable
 * of content type dependency rules.
 */

/**
 * Custom form for adding & editing content type dependency rule.
 */
function content_type_dependency_modify($form, &$form_state) {
  // Rule edit form & new rule form.
  $node_type_get_types = node_type_get_types();
  $content_types = array();
  $content_types[''] = t('- SELECT -');
  foreach ($node_type_get_types as $mechine_name => $human_read) {
    $content_types[$mechine_name] = $human_read->name;
  }
  if (isset($_GET['edit']) && isset($_GET['cd_id']) && $_GET['edit'] == 1) {
    unset($content_types['']);
    $result = db_query('SELECT * FROM {content_type_dependency} WHERE cd_id = :cd_id', array(':cd_id' => $_GET['cd_id']));
    $record = $result->fetchObject();
  }
  $form['to_create_modify'] = array(
    '#title' => t('To create'),
    '#type' => 'select',
    '#description' => t('The content type that the following content creation dependency rules apply to'),
    '#options' => $content_types,
    '#weight' => 1,
    '#required' => TRUE,
    '#default_value' => (isset($_GET['edit']) && $_GET['edit']) ? $record->to_create : '',
  );
  $form['must_have_fields_modify'] = array(
    '#type' => 'fieldset',
    '#title' => t('Condition'),
    '#weight' => 2,
  );
  $form['must_have_fields_modify']['must_have_modify'] = array(
    '#title' => t('Must have'),
    '#type' => 'select',
    '#description' => t('This content type has to be created before being able to create the "To Create" content type'),
    '#options' => $content_types,
    '#required' => TRUE,
    '#weight' => 3,
    '#default_value' => (isset($_GET['edit']) && $_GET['edit']) ? $record->must_have : 'select',
  );
  $form['must_have_fields_modify']['no_of_modify'] = array(
    '#title' => t('No of'),
    '#description' => t('The total number of contents the user has to create of this content type'),
    '#type' => 'textfield',
    '#default_value' => (isset($_GET['edit']) && $_GET['edit']) ? $record->no_of : '',
    '#size' => 6,
    '#required' => TRUE,
    '#maxlength' => 3,
    '#weight' => 4,
  );
  $form['enable_modify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#weight' => 6,
    '#default_value' => (isset($_GET['edit']) && $_GET['edit']) ? $record->status : 0,
  );
  $form['display_message_modify'] = array(
    '#title' => t('Dispaly message'),
    '#description' => t("The drupal message to be displayed when the user tries to create the 'Must have' content type, and still has to create more of the prerequisite content type.<br />Leave empty for Default. Example: You must have 4 Articles created to continue."),
    '#type' => 'textarea',
    '#default_value' => (isset($_GET['edit']) && $_GET['edit']) ? ($record->default_message ? '' : $record->message) : '',
    '#rows' => 4,
    '#cols' => 20,
    '#weight' => 5,
  );
  $form['Submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('content_type_dependency_modify_submit'),
    '#validate' => array('content_type_dependency_modify_validate'),
    '#weight' => 10,
  );
  return $form;
}

/**
 * Validate content_type_dependency_modify form.
 */
function content_type_dependency_modify_validate($form, &$form_state) {
  // User selects the same content types on each dropdown list.
  if ($form_state['values']['to_create_modify'] == $form_state['values']['must_have_modify']) {
    form_set_error('cd_both', t('You should select different content types'));
  }
  // Checking rule exists in rule edit form.
  if (isset($_GET['edit']) && isset($_GET['cd_id']) && $_GET['edit'] == 1) {
    $result = db_select('content_type_dependency', 'c')
      ->fields('c')
      ->condition('to_create', $form_state['values']['to_create_modify'], '=')
      ->condition('must_have', $form_state['values']['must_have_modify'], '=')
      ->condition('cd_id', $_GET['cd_id'], '!=')
      ->execute();
    // Redirect URL.
    $url = url('admin/config/content/content_type_dependency/list');
    // If rule already created with this content types.
    if ($result->rowCount() >= 1) {
      form_set_error('cd_exists', t('The selected Content Dependency already exists. View the <a href="@url">List</a> of already existing dependences', array('@url' => $url)));
    }
  }
  // Checking rule exists in new rule form.
  else {
    $result = db_select('content_type_dependency', 'c')
      ->fields('c')
      ->condition('to_create', $form_state['values']['to_create_modify'], '=')
      ->condition('must_have', $form_state['values']['must_have_modify'], '=')
      ->execute();
    // Redirect url.
    $url = url('admin/config/content/content_type_dependency/list');
    // If rule already created with this content types.
    if ($result->rowCount() >= 1) {
      form_set_error('cd_exists', t('The selected Content Dependency already exists. View the <a href="@url">List</a> of already existing dependences', array('@url' => $url)));
    }
  }
  // Restrict user input to minimum 1.
  if ($form_state['values']['no_of_modify'] < 1) {
    form_set_error('cd_no_of', t('The number of contents to create should be minimum 1'));
  }
  // Restrict user input type to number.
  if (!is_numeric($form_state['values']['no_of_modify'])) {
    form_set_error('cd_no_of_numeric', t('The value of the "No of" field should be a number'));
  }
}

/**
 * Submit content_type_dependency_modify form.
 */
function content_type_dependency_modify_submit($form, &$form_state) {
  // Store form_state values in variables.
  $no_of     = $form_state['values']['no_of_modify'];
  $must_have = $form_state['values']['must_have_modify'];
  $to_create = $form_state['values']['to_create_modify'];
  $status    = $form_state['values']['enable_modify'];
  // Setting default message if text area is empty.
  if ($form_state['values']['display_message_modify'] == '') {
    $message = 'You must create ' . $no_of . ' ' . $must_have . ' to continue ';
    $default_message = 1;
  }
  else {
    $message = $form_state['values']['display_message_modify'];
    $default_message = 0;
  }
  // Display enabled or disabled on message status.
  if ($status == 1) {
    $msg = t('Enabled');
  }
  else {
    $msg = t('Disabled');
  }
  // Form submition for editing.
  if (isset($_GET['edit']) && $_GET['edit'] == 1) {
    db_update('content_type_dependency')->fields(
      array(
        'to_create' => $to_create,
        'must_have' => $must_have,
        'no_of' => $no_of,
        'message' => $message,
        'status' => $status,
        'default_message' => trim($default_message),
      ))->condition('cd_id', $_GET['cd_id'])
        ->execute();
    drupal_set_message(t('content dependency has been updated.'));
  }
  // New rule form submtion.
  else {
    db_insert('content_type_dependency')
      ->fields(array(
        'to_create' => $to_create,
        'must_have' => $must_have,
        'no_of'     => $no_of,
        'message'   => $message,
        'status'    => $status,
        'default_message' => trim($default_message)))->execute();
    drupal_set_message(t('New content dependency created & @msg', array('@msg' => $msg)));
  }
  $form_state['redirect'] = 'admin/config/content/content_type_dependency/list';
}

/**
 * Confirm form for deleting selected rule.
 */
function content_type_dependency_delete_confirm($form, &$form_state, $id) {
  $form['cd_id'] = array('#type' => 'value', '#value' => $id);
  return confirm_form($form,
    t('Are you sure you want to delete ?'),
    'admin/config/content/content_type_dependency/list',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submitting configure form & Deleting selected rule.
 */
function content_type_dependency_delete_confirm_submit($form, &$form_state) {
  drupal_set_message(t('Rule deleted successfully!'));
  db_delete('content_type_dependency')->condition('cd_id', $form_state['values']['cd_id'])->execute();
  drupal_goto('admin/config/content/content_type_dependency/list');
}

/**
 * Enabling selected rule.
 */
function content_type_dependency_enable($cd_id) {
  if (!drupal_valid_token($_GET['token'], $cd_id . 'en')) {
    drupal_access_denied();
  }
  else {
    drupal_set_message(t('Enabled!'));
    db_update('content_type_dependency')->fields(array('status' => 1))->condition('cd_id', $cd_id)->execute();
    drupal_goto('admin/config/content/content_type_dependency/list');
  }
}

/**
 * Disabling selected rule.
 */
function content_type_dependency_disable($cd_id) {
  if (!drupal_valid_token($_GET['token'], $cd_id . 'dis')) {
    drupal_access_denied();
  }
  else {
    drupal_set_message(t('Disabled!'));
    db_update('content_type_dependency')->fields(array('status' => 0))->condition('cd_id', $cd_id)->execute();
    drupal_goto('admin/config/content/content_type_dependency/list');
  }
}

/**
 * Displays the list of rules created by admin.
 */
function content_type_dependency_list($content = NULL) {
  // Adding css file.
  drupal_add_css(drupal_get_path('module', 'content_type_dependency') . '/table_caption.css');
  $query = db_select('content_type_dependency', 'd');
  $query->fields('d',
    array('to_create', 'must_have', 'no_of', 'message', 'cd_id'));
  $table_sort = $query->extend('TableSort');
  $pager = $table_sort->extend('PagerDefault')
                      ->orderby('d.cd_id', 'DESC')
                      ->condition('d.status', 1);
  $result = $pager->execute();
  $row = array();
  // Table header for enabled rules.
  $header_table = array(
    t('To create'),
    t('Must have'),
    t('No of'),
    t('Message'),
    '',
    '');
  while ($rows = $result->fetchAssoc()) {
    $token = drupal_get_token($rows['cd_id'] . 'dis');
    $opt = array('query' => array('token' => $token));
    $opt_edit = array('query' => array('cd_id' => $rows['cd_id'], 'edit' => 1));
    $row[] = array($rows['to_create'],
      $rows['must_have'],
      $rows['no_of'],
      check_plain($rows['message']),
      l(t('Disable'), 'admin/config/content/content_type_dependency/' . $rows['cd_id'] . '/disable', $opt),
      l(t('Delete'), 'admin/config/content/content_type_dependency/' . $rows['cd_id'] . '/delete') . ' | ' . l(t('Edit'), 'admin/config/content/content_type_dependency/list/modify', $opt_edit));
  }
  $query1 = db_select('content_type_dependency', 'd');
  $query1->fields('d',
    array('to_create', 'must_have', 'no_of', 'message', 'cd_id'));
  $table_sort1 = $query1->extend('TableSort');
  $pager1 = $table_sort1->extend('PagerDefault')
    ->orderby('d.cd_id', 'DESC')
    ->condition('d.status', 0);
  $result1 = $pager1->execute();
  $row1 = array();
  // Table header for disabled rules.
  $header_table1 = array(
    t('To create'),
    t('Must have'),
    t('No of'),
    t('Message'),
    '',
    '');
  while ($rows1 = $result1->fetchAssoc()) {
    $token = drupal_get_token($rows1['cd_id'] . 'en');
    $opt = array('query' => array('token' => $token));
    $opt_edit = array('query' => array('cd_id' => $rows1['cd_id'], 'edit' => 1));
    $row1[] = array(
      $rows1['to_create'],
      $rows1['must_have'],
      $rows1['no_of'],
      check_plain($rows1['message']),
      l(t('Enable'), 'admin/config/content/content_type_dependency/' . $rows1['cd_id'] . '/enable', $opt),
      l(t('Delete'), 'admin/config/content/content_type_dependency/' . $rows1['cd_id'] . '/delete') . ' | ' . l(t('Edit'), 'admin/config/content/content_type_dependency/list/modify', $opt_edit));
  }
  // Table list for enabled rules.
  $table_enable = array(
    '#theme' => 'table',
    '#header' => $header_table,
    '#rows' => $row,
    '#caption' => '<span class=\'content_type_dependency_caption\'>' . t('Enabled') . '</span>',
  );
  // Table list for Disabled rules.
  $table_disable = array(
    '#theme' => 'table',
    '#header' => $header_table1,
    '#rows' => $row1,
    '#caption' => '<span class=\'content_type_dependency_caption\'>' . t('Disabled') . '</span>',
  );
  return array($table_enable, $table_disable);
}
