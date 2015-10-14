<?php

/**
 * @file
 * Admin settings form and OAuth authentication integration
 */

/**
 * Parse administrative settings form.
 */
function parse_admin_settings($form, &$form_state) {
  $form['parse'] = array(
    '#type' => 'fieldset',
    '#title' => t('Parse Application API Settings'),
  );
  $form['parse']['parse_application_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#required' => TRUE,
    '#default_value' => variable_get('parse_application_id', NULL),
  );
  $form['parse']['parse_masterkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Master Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('parse_masterkey', NULL),
  );
  $form['parse']['parse_restkey'] = array(
    '#type' => 'textfield',
    '#title' => t('REST API Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('parse_restkey', NULL),
  );
  $form['parse']['parse_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API URL'),
    '#required' => TRUE,
    '#default_value' => variable_get('parse_url', 'https://api.parse.com/1/'),
  );

  $form['#validate'][] = 'parse_admin_settings_validate';
  return system_settings_form($form);
}

/**
 * Validation callback for the administrative settings form.
 */
function parse_admin_settings_validate($form, &$form_state) {
  $form_state['values']['parse_application_id'] = trim($form_state['values']['parse_application_id']);
  $form_state['values']['parse_masterkey'] = trim($form_state['values']['parse_masterkey']);
  $form_state['values']['parse_restkey'] = trim($form_state['values']['parse_restkey']);
  $form_state['values']['parse_url'] = trim($form_state['values']['parse_url']);
}

/**
 * List of configured rules for sending out post notifications. Doing it this way
 * because I don't want to make "Rules" a dependency.
 */
function parse_rules() {
  // Set up the right breadcrumbs
  $breadcrumbs = array();
  $breadcrumbs[] = l('Home', '<front>');
  $breadcrumbs[] = l('Administration', 'admin');
  $breadcrumbs[] = l('Configuration', 'admin/config');
  $breadcrumbs[] = l('Web Services', 'admin/config/services');
  $breadcrumbs[] = l('Parse', 'admin/config/services/parse');
  $breadcrumbs[] = l('Content Based Rules', 'admin/config/services/parse/rules');
  drupal_set_breadcrumb($breadcrumbs);

  $rows = array();
  $header = array(
    'rule_name' => array('data' => t('Rule Name'), 'style' => 'text-align: center;'),
    'edit' => array('data' => t('Edit'), 'style' => 'text-align: center;'),
    'delete' => array('data' => t('Delete'), 'style' => 'text-align: center;'),
  );
  $result = db_select('parse_rules', 'p')
              ->fields('p')
              ->orderBy('p.rule_name', 'ASC')
              ->execute();
  while ($data = $result->fetchObject()) {
    $row = array();
    $row[] = $data->rule_name;
    $row[] = (array('data' => l(t('Edit'), 'admin/config/services/parse/rules/edit/' . $data->rid), 'align' => 'center'));
    $row[] = (array('data' => l(t('Delete'), 'admin/config/services/parse/rules/delete/' . $data->rid), 'align' => 'center'));
    $rows[] = $row;
  }

  if (count($rows) == 0) {
    $rows = array(
      array(
        'data' => array(array('align' => 'center', 'colspan' => 3, 'data' => t('THERE ARE CURRENTLY NO CONFIGURED CONTENT RULE PUSH NOTIFICATIONS.')))
      ),
    );
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Form for adding or modifying an existing rule.
 *
 * @param array $form
 *   Our instantiated $form associative array.
 * @param array &$form_state
 *   Any values associated with this form.
 * @param int $rid
 *   Optional parameter. Passed when modifying an existing rule. The ID of the rule to
 *   be modified.
 * @return array $form
 *   An associative array with all of our configured form fields.
 */
function parse_add_rule_form($form, &$form_state, $rid = 0) {
  // If we were passed a rule id then we are editing, so we need to put the rid in a
  // hidden field.
  if (!empty($rid)) {
    $form['rid'] = array(
      '#type' => 'hidden',
      '#value' => $rid,
    );

    // Load the form field default values. If, for some reason, there are none, then just
    // initialize the object.
    $result = db_select('parse_rules', 'r')
      ->fields('r')
      ->condition('rid', $rid)
      ->execute();
    if ($result->rowCount() > 0) {
      $rdata = $result->fetchObject();
      $button_value = t('Update Rule');
    }
    else {
      unset($form['rid']);
      $rdata = new stdClass();
      $button_value = t('Create Rule');
    }
  }
  else {
    $rdata = new stdClass();
    $button_value = t('Create Rule');
  }

  $form['rule_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Rule Name'),
    '#default_value' => !empty($rdata->rule_name) ? $rdata->rule_name : NULL,
    '#maxlength' => 240,
    '#size' => 60,
  );

  // Get a list of all of our available content types so that the user can select which
  // ones to post notifications for when they get added to or modified.
  $types = array();
  $node_types = node_type_get_types();
  foreach ($node_types as $type => $object) {
    if (empty($object->disabled)) {
      $types[$type] = $object->name;
    }
  }
  
  // Get a list of our available roles so we can allow the user which roles can be used
  // for notification sending.
  $roles = user_roles(TRUE);
  $new_roles = array();
  $rdata->roles = (!empty($rdata->roles)) ? $rdata->roles = unserialize($rdata->roles) : array();
  
  // Convert any stored roles back into an array Drupal can use to automatically check
  // boxes on their form since we need them one way for our form here and another way
  // for our array checking at the time of posting.
  foreach ($rdata->roles as $key => $role) {
    if (!empty($role)) {
      $new_roles[$key] = $key;
    }
  }
  $rdata->roles = $new_roles;
  
  // We need to do the same thing with node_create_types and node_update_types. Love that.
  
  $form['roles_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Notification Roles',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['roles_fieldset']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Which roles are allowed to send notifications with this rule?'),
    '#options' => $roles,
    '#default_value' => !empty($rdata->roles) ? $rdata->roles : array(),
    '#required' => TRUE,
  );
  
  $form['create_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Insert Notification Node Types',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['create_fieldset']['node_create_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types to send notifications for when created.'),
    '#options' => $types,
    '#default_value' => !empty($rdata->node_create_types) ? unserialize($rdata->node_create_types) : array(),
  );
  $form['update_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Update Notification Node Types',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['update_fieldset']['node_update_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types to send notifications for when updated.'),
    '#options' => $types,
    '#default_value' => !empty($rdata->node_update_types) ? unserialize($rdata->node_update_types) : array(),
  );
  $form['comments_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Comments Notifications',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['comments_fieldset']['comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send notifications when a comment to a piece of content is posted.'),
    '#default_value' => !empty($rdata->comments) ? $rdata->comments : NULL,
  );
  $form['message_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Notification Message',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['message_fieldset']['notification_message'] = array(
    '#type' => 'textarea',
    '#title' => NULL,
    '#description' => t('You can use replacement tokens if you have the tokens module enabled. (Which you should!). Maximum of 250 characters. Please respect the possible values of your tokens when taking this into consideration. Module will truncate messages over 140 characters.'),
    '#default_value' => !empty($rdata->notification_message) ? $rdata->notification_message : NULL,
    '#required' => TRUE,
    '#maxlength' => 250,
  );
  $form['message_fieldset']['badge_type'] = array(
    '#type' => 'select',
    '#title' => 'Badge Type',
    '#options' => array(
      'None' => t('None'),
      'Increment' => t('Increment'),
    ),
    '#default_value' => !empty($rdata->badge_type) ? $rdata->badge_type : 'Increment',
    '#required' => TRUE,
    '#maxlength' => 250,
  );
  
  // Display the interface for replacement tokens.
  if (module_exists('token')) {
    $form['message_fieldset']['parse_tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#theme' => 'token_tree',
      '#token_types' => array('node', 'user'),
      '#recursion_limit' => min(variable_get('token_tree_recursion_limit', 3), variable_get('colorbox_token_recursion_limit', 3)),
      '#dialog' => TRUE,
    );
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $button_value,
  );
  return $form; 
}

/**
 * Rules form submission callback
 *
 * Processes the form fields and stores them in our database for future use.
*/
function parse_add_rule_form_submit($form, &$form_state) {
  // For roles and content types we need to parse out the titles of the roles and content
  // types for those that are set so that PHP will find a match between our variables
  // and the ones that Drupal provides.
  foreach ($form_state['values']['roles'] AS $key => $role) {
    $mew_roles = array();
    if ($key == $role) {
      $r = user_role_load($key);
      $new_roles[$key] = $r->name;
    }
  }
  
  // Assign the form values to their respective database columns
  $rule = array(
    'rule_name' => $form_state['values']['rule_name'],
    'roles' => $new_roles,
    'node_create_types' => $form_state['values']['node_create_types'],
    'node_update_types' => $form_state['values']['node_update_types'],
    'comments' => $form_state['values']['comments'],
    'notification_message' => $form_state['values']['notification_message'],
    'badge_type' => $form_state['values']['badge_type'],
  );

  // If we are updating, then assign the primary key, otherwise we are assigning a new
  // rule.
  if (!empty($form_state['values']['rid'])) {
    $rule['rid'] = $form_state['values']['rid'];
    drupal_write_record('parse_rules', $rule, array('rid'));
    drupal_set_message('The configured rule has been successfully updated.');
  }
  else {
    drupal_write_record('parse_rules', $rule);
    drupal_set_message('The new rule has been successfully saved.');
  }

  // Clear our rules cache
  cache_clear_all('parse:rules', 'cache');

  // Redirect back to the main form rules list page/
  drupal_goto('admin/config/services/parse/rules'); 
}

/**
 * Our confirmation of delete form
 *
 * We need to make sure our user knows this will delete the rule and it cannot be undone.
 * No matter how badly they may want to do so.
 *
 * @param int $rid
 *   The rule id of the rule we are confirming for deletion
 * @return array $form
 *   The array of the form to be rendered.
 */
function parse_rule_delete_confirm_form($form, &$form_state, $rid) {
  $form['rid'] = array(
    '#type' => 'hidden',
    '#value' => $rid,
  );
  return confirm_form($form, 
                      'Are you sure you wish to delete this rule? There is no going back!!',
                      'admin/config/services/parse/rules',
                      t('You cannot undo this. It will be a permanent mark on your record and the only way to fix it is to re-create the rule from scratch. I JUST want you to know what you are getting yourself into.'),
                      t('Confirm Delete'),
                      t('Don\'t Do It')
                     );
}

/**
 * The callback to our delete form
 *
 * This is where we take the confirmed form and delete the rule.
 */
function parse_rule_delete_confirm_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['rid'])) {
    // Delete the rule and notify the admin when it is completed.
    $rid = $form_state['values']['rid'];
    $number_deleted = db_delete('parse_rules')
      ->condition('rid', $rid)
      ->execute();
    drupal_set_message($number_deleted . ' rule has been successfully deleted.');

    // Clear the cache for rules so they will be repopulated on the next use.
    cache_clear_all('parse:rules', 'cache', FALSE);
  }
  else {
    // Set the error message so the admin knows the rule could not be deleted.
    drupal_set_message('No rule could be deleted.', 'error');
  }
  // Go back to the list of rules. 
  drupal_goto('admin/config/services/parse/rules'); 
}
    
