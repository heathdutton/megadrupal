<?php
/**
 * @file
 *  Roles manager forms.
 */

/**
 * This function lists all rules, and let the admin to do certain actions with them
 *
 * @returns
 *  HTML table containing the number of rule, attribute, RegExp, role and the actions which can be done with each role.
 */
function _shib_auth_list_rules() {
  $output = NULL;
  $rows = array();

  // if the admin is not logged in through shibboleth, rolename cache has to be generated
  shib_auth_generate_rolenames(TRUE);

  //create rows
  $rules = db_select('shib_auth', 'role')
              ->fields('role')
              ->execute();

  while ($rule = $rules->fetchAssoc()) {
    $roles = unserialize($rule['role']);
    $roles_list = '';

    foreach ($roles as $role) {
      if (!empty($role)) {
        $roles_list .= shib_auth_get_rolename($role) . ', ';
      }
    }
    //cut off the last ', ' charaters
    $roles_list = drupal_substr($roles_list, 0, -2);

    $rule['sticky'] == 1 ? $sticky = 'Yes' : $sticky = 'No';
    $rows[] = array(
      $rule['field'],
      urldecode($rule['regexpression']),
      $roles_list,
      $sticky,
      l(t('Clone'), 'admin/config/people/shib_auth/clone/' . $rule['id']) . ' | ' .
      l(t('Edit'), 'admin/config/people/shib_auth/edit/' . $rule['id']) . ' | ' .
      l(t('Delete'), 'admin/config/people/shib_auth/delete/' . $rule['id']),
    );
  }

  //create the rule list in HTML table
  $attributes = array('class' => 'shib_auth_role_table');
  $header = array(t('Attribute'), t('RegExp'), t('Roles'), t('Sticky'), t('Actions'));
  $output = theme_table(
              array(
                'header'     => $header,
                'rows'       => $rows,
                'attributes' => $attributes,
                'empty'      => t('There is no rule in the database'),
                //beacuse the theme_table hastly implemented
                'caption'    => NULL,
                'colgroups'  => NULL,
                'sticky'     => NULL,
              )
            );
  $output .= l(t('Add new rule'), 'admin/config/people/shib_auth/new');

  return $output;
} //function _shib_auth_list_rules

/**
 * This is the confirmation form for deleting a rule
 */
function _shib_auth_rule_delete_confirm_form($form, &$form_state) {
  $id = $form_state['build_info']['args'][0];
  $desc = t("The rule with id %id will be deleted permanently!", array('%id' => $id));

  // Make sure the form redirects in the end
  $form['destination'] = array('#type' => 'hidden', '#value' => 'admin/config/people/shib_auth/rules');

  return confirm_form($form,
                      filter_xss($desc),
                      'admin/config/people/shib_auth/rules');
} //_shib_auth_rule_delete_confirm_form

/**
 * This function deletes an existing rule
 */
function _shib_auth_rule_delete_confirm_form_submit($form, &$form_state) {
  $id = $form_state['build_info']['args'][0];
  if ($id = intval($id)) {
    $ret = db_delete('shib_auth')
    ->condition('id', $id)
    ->execute();
    if ($ret)
      drupal_set_message(t('Rule <strong>#@id</strong> has been deleted.', array('@id' => $id)));
    else
      drupal_set_message(t('Failed to delete rule.'), 'error');
  }
  else
    drupal_set_message(t("Invalid rule id."), 'error');

  drupal_goto('admin/config/people/shib_auth/rules');
} //_shib_auth_rule_delete_confirm_form

/**
 * This is the confirmation form for cloning a rule
 */
function _shib_auth_rule_clone_confirm_form($form, &$form_state) {
  $id = $form_state['build_info']['args'][0];
  $desc = t("The rule with id %id will be cloned", array('%id' => $id));

  // Make sure the form redirects in the end
  $form['destination'] = array('#type' => 'hidden', '#value' => 'admin/config/people/shib_auth/rules');

  return confirm_form($form,
                      filter_xss($desc),
                      'admin/config/people/shib_auth/rules');
} //_shib_auth_rule_clone_confirm_form

/**
 * This function enables the administrator to clone an existing rule, this is useful,
 * when we want to create a rule, which is simiral to another one
 */
function _shib_auth_rule_clone_confirm_form_submit($form, &$form_state) {
  $id = $form_state['build_info']['args'][0];
  if($id = intval($id)) {
    $rule = db_select('shib_auth', 'c')
            ->fields('c')
            ->condition('id', $id, '=')
            ->execute()
            ->fetchAssoc();
    unset($rule['id']);

    $ret = drupal_write_record('shib_auth', $rule);
    if ($ret === SAVED_NEW)
      drupal_set_message(t('The rule has been successfully cloned.'));
    else
      drupal_set_message(t('Unexpected error has been detected.'));
  }
  else
    drupal_set_message(t("Invalid rule id."), 'error');

  drupal_goto('admin/config/people/shib_auth/rules');
} //function _shib_auth_rule_clone_confirm_form_submit

/**
 * Generate the shibboleth rule adding form
 *
 * @param array $options
 *  Contains the data, we want to fill the form with
 *
 * @returns
 *  The edit form, with the fields already filled in with the elements of the options array.
 */
function shib_auth_edit_form($options) {
  $form = array();

  $form['shib_auth_new_id'] = array(
    '#title'          => t('Entry id'),
    '#type'           => 'hidden',
    '#default_value'  => $options[0],
  );

  $form['shib_auth_new_attrib'] = array(
    '#title'          => t('Shibboleth attribute name'),
    '#type'           => 'textfield',
    '#default_value'  => $options[1],
    '#require'        => TRUE,
    '#description'    => t('More properly: <b>@server</b> field name; enable DEBUG mode to list available fields. <br/>Note that it might differ from your users\' fields.', array('@server' => '$_SERVER')),
  );

  $form['shib_auth_new_regexp'] = array(
    '#title'          => t('Value (regexp)'),
    '#type'           => 'textfield',
    '#default_value'  => $options[2],
    '#require'        => TRUE,
  );

  $roles = user_roles(TRUE);

  $form['shib_auth_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => count($options[3]) > 1 || (count($options[3]) == 1 && $options[3] != "") ? $options[3] : array(),
    '#options' => $roles,
  );

  $form['sticky_markup'] = array(
    '#value' => '<b>Role type:</b>',
  );

  $form['shib_auth_new_sticky'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sticky'),
    '#default_value' => $options[5],
    '#description'    => t('Set the rule to be sticky if you want to save the role(s) permanently to the user\'s profile.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t($options[4]),
  );

  return $form;
} //function shib_auth_edit_form

/**
 * Creates a new rule by calling universal create/edit form
 */
function shib_auth_new_rule($form, &$form_state) {
  return shib_auth_edit_form(array(0, '', '', '', 'Add rule', 0));
} //function shib_auth_new_rule

/**
 * Validates a new rule form
 *
 */
function shib_auth_new_rule_validate($form, &$form_state) {
  if (empty($form_state['values']['shib_auth_new_attrib'])) {
    form_set_error('shib_auth_new_attrib', t('This element must not be empty'));
  }
  if (empty($form_state['values']['shib_auth_new_regexp'])) {
    form_set_error('shib_auth_new_regexp', t('This element must not be empty'));
  }
} //function shib_auth_new_rule_validate

/**
 * Submit the new rule from
 */
function shib_auth_new_rule_submit($form, &$form_state) {
  shib_auth_save_rule($form_state, array());
} //function shib_auth_new_rule_submit

/**
 * Cunstructor of the rule edit form
 */
function shib_auth_edit_rule($form, &$form_state) {
  $id = $form_state['build_info']['args'][0];
  $form = array();
  // calls the edit form, with the fields of the existing rule
  if (is_int((int) $id)) {
    $rule = db_select('shib_auth', 'c')
              ->fields('c')
              ->condition('id', $id, '=')
              ->execute()
              ->fetchAssoc();
    return shib_auth_edit_form(
      array($rule['id'], $rule['field'], urldecode($rule['regexpression']), unserialize($rule['role']), 'Apply', $rule['sticky'])
    );
  }
} //function shib_auth_edit_rule

/**
 * Validates rule edit form
 *
 */
function shib_auth_edit_rule_validate($form, &$form_state) {
  if (empty($form_state['values']['shib_auth_new_attrib'])) {
    form_set_error('shib_auth_new_attrib', t('This element must not be empty'));
  }
  if (empty($form_state['values']['shib_auth_new_regexp'])) {
    form_set_error('shib_auth_new_regexp', t('This element must not be empty'));
  }
} //function shib_auth_edit_rule_validate

/**
 * Submit rule edit form
 */
function shib_auth_edit_rule_submit($form, &$form_state) {
  shib_auth_save_rule($form_state, "id");
} //function shib_auth_edit_rule_submit

/**
 * Saves a new rule into database
 *
 * @param integer $update
 *  Decides if it is a new rule (NULL), or we're just modifing one rule id.
 *
 * @param array $form_state
 *  The state of the form, which we have just received, including all of the variables.
 */
function shib_auth_save_rule($form_state, $update) {
  $values = $form_state['values'];
  $new_id = $values['shib_auth_new_id'] == '0' ? NULL : (int) $values['shib_auth_new_id'];

  // collect ther roles into an array
  $roles = array();
  if (is_array($values['shib_auth_roles'])) {
    foreach ($values['shib_auth_roles'] as $role_id) {
      if (!empty($role_id))
        $roles[] = $role_id;
    }
  }

  //save the new element into an array
  $new_element = array(
      'id'            => $new_id,
      'field'         => urlencode($values['shib_auth_new_attrib']),
      'regexpression' => urlencode($values['shib_auth_new_regexp']),
      'role'          => serialize($roles),
      'sticky'        => urlencode($values['shib_auth_new_sticky']),
  );

  //write it in a record
  $ret = drupal_write_record('shib_auth', $new_element, $update);

  // if it wasn't an error
  if (empty($update)) {
    if ($ret === SAVED_NEW) {
      drupal_set_message(t('New rule has been stored.'));
    }
    else {
      drupal_set_message(t('Unexpected error has been detected.'));
    }
  }
  //an existing rule was updated
  else {
    if ($ret === SAVED_UPDATED) {
      drupal_set_message(t('The rule has been modified.'));
    }
    else {
      drupal_set_message(t('Unexpected error has been detected.'));
    }
  }

  drupal_goto('admin/config/people/shib_auth/rules');
} //function shib_auth_save_rule
