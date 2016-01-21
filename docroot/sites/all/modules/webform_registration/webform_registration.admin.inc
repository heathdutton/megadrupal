<?php
/**
 * @file
 * Webform Registration Include File.
 *
 * This file holds all the webform administration functionality
 */

function webform_registration_settings_form($form, $form_state, $node) {

  $components = $node->webform['components'];

  if (empty($components)) {
    $form['enabled']['#value'] = FALSE;
    $form['enabled']['#disabled'] = TRUE;
    $form['instructions'] = array(
      '#type' => 'item',
      '#title' => 'No available components',
      '#description' => filter_xss(t('You must first !add_components. Then you can enable registration via this webform', array('!add_components' => l(t('Add  Components'), 'node/' . $node->nid . '/webform')))),
    );
    return $form;
  }

  $form_settings = $node->webform_registration;

  $form['node'] = array(
    '#type' => 'value',
    '#default_value' => $node,
  );

  $form['webform_registration'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#title' => t('Registration Settings'),
  );

  $form['webform_registration']['nid'] = array(
    '#type' => 'value',
    '#default_value' => $node->nid,
  );

  $form['webform_registration']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to register using this webform'),
    '#default_value' => ($form_settings['enabled'] ? TRUE : FALSE),
  );

  // We do have components in this form, so let's process them into select options.
  $options = array();
  $fieldset = array(); // Holds form_key references to map pid to fieldset cids.
  $auto = array('<auto>' => t('Generate field automatically'));
  $none = array('<none>' => t('Omit field'));

  foreach ($components as $component) {
    // Available options are nid, cid, form_key, name, type, value, extra,
    // mandatory, email, pid, weight, page_num
    if ($component['type'] == 'fieldset') {
      $fieldset[$component['cid']] = array('name' => $component['name'], 'form_key' => $component['form_key']);
      $options[$component['name']] = array();
    }
    elseif ($component['pid'] != 0) {
      $options[$fieldset[$component['pid']]['name']][$fieldset[$component['pid']]['form_key'] . '][' . $component['form_key']] = $component['name'];
    }
    else {
      $options[$component['form_key']] = $component['name'];
    }
  }

  $form['webform_registration']['name'] = array(
    '#type' => 'select',
    '#title' => t('Username field'),
    '#description' => t('Select the field on this webform that will be used
      for the username.'),
    '#default_value' => $form_settings['name'],
    '#options' => $options,
    // @ToDo: Optionally implement automatic username generation.
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['webform_registration']['pass'] = array(
    '#type' => 'select',
    '#title' => t('Password field'),
    '#description' => t('Select the field on this webform that will be used
      for the user\'s password. Select &lt;auto&gt; to have Drupal generate a
      random password. This field will be altered in the webform to hide the text entered where applicable.'),
    '#default_value' => $form_settings['pass'],
    '#options' => $auto + $options,
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['webform_registration']['pass_confirm'] = array(
    '#type' => 'select',
    '#title' => t('Password Confirmation field'),
    '#default_value' => $form_settings['pass_confirm'],
    '#options' => $none + $options,
    '#description' => t('Selecting the same field selected above renders a second verification only dummy field. Select &lt;none&gt; to show only one email field.'),
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['webform_registration']['mail'] = array(
    '#type' => 'select',
    '#title' => t('Email Address field'),
    '#default_value' => $form_settings['mail'],
    '#options' => $options,
    '#description' => t('Select the field on this webform that will be used for the user\'s email.'),
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['webform_registration']['mail_confirm'] = array(
    '#type' => 'select',
    '#title' => t('Email Confirmation field'),
    '#default_value' => $form_settings['mail_confirm'],
    '#options' => $none + $options,
    '#description' => t('Selecting the same field selected above renders a second verification only dummy field. Select &lt;none&gt; to show only one email field.'),
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Default status
  $form['webform_registration']['status'] = array(
    '#type' => 'radios',
    '#title' => t('Default status'),
    '#default_value' => empty($form_settings['status']) ? 1 : $form_settings['status'],
    '#options' => array(1 => 'Active', 0 => 'Blocked'),
    '#description' => t('Select the default status for users created through webform registration'),
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Opt in.
  $form['webform_registration']['opt_in'] = array(
    '#type' => 'select',
    '#title' => t('Opt In field'),
    '#default_value' => $form_settings['opt_in'],
    '#options' => $none + $options,
    '#description' => t('Select the field on this webform that will be used to indicate that user opts in for registration. Select &lt;none&gt; to register all users.'),
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['webform_registration']['opt_in_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Opt In value'),
    '#default_value' => $form_settings['opt_in_value'],
    '#description' => t('Specify the value for Opt In field where user opts in for registration.'),
    '#required' => FALSE,
    '#maxlength' => 255,
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Show list of user roles.
  $roles = user_roles(TRUE);
  $form['webform_registration']['roles'] = array(
    '#type' => 'fieldset',
    '#description' => t('Select the roles that users should be assigned to.'),
    '#title' => t('User roles'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );
  foreach ($roles as $rid => $role) {
    $form['webform_registration']['roles'][$rid] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($role),
      '#default_value' => (isset($form_settings['roles'][$rid]) ? TRUE : FALSE),
      '#states' => array(
        'invisible' => array(
         'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
        ),
      ),
    );
    // They're always authenticated users.
    if ($rid == 2) {
      $form['webform_registration']['roles'][$rid]['#default_value'] = TRUE;
      $form['webform_registration']['roles'][$rid]['#disabled'] = TRUE;
    }
  }

  $form['webform_registration']['theme_override'] = array(
    '#type' => 'select',
    '#title' => t('Override Page Template'),
    '#default_value' => $form_settings['theme_override'],
    '#options' => array(FALSE => t('Disabled'), TRUE => t('Enabled')),
    '#description' => t('Select %enabled to use page-registration.tpl.php instead of page.tpl.php', array('%enabled' => t('Enabled'))),
    '#states' => array(
      'invisible' => array(
       'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $user_field_info_instances = field_info_instances('user', 'user');
  if (!empty($user_field_info_instances)) {
    $form['webform_registration']['account_fields'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#tree' => TRUE,
      '#title' => t('Account Fields'),
       '#states' => array(
          'invisible' => array(
           'input[name="webform_registration[enabled]"]' => array('checked' => FALSE),
          ),
        ),
    );
    //Iterate around instances of user field
    foreach ($user_field_info_instances as $field_name => $instance) {
      $form['webform_registration']['account_fields'][$instance['field_name']] = array(
        '#type' => 'select',
        '#title' => check_plain($instance['label']),
        '#description' => t('Select the field on this webform that will be used
          for @field_name.', array("@field_name" => $instance['label'])),
        '#default_value' => (isset($form_settings['account_fields'][$instance['field_name']]) ? $form_settings['account_fields'][$instance['field_name']] : ''),
        '#options' => $none + $options,
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for webform_registration_settings_form
 */
function webform_registration_settings_form_submit($form, &$form_state) {
  $form_values = $form_state['values']['webform_registration'];
  if (isset($form_values['account_fields'])) {
    $form_values['account_fields'] = serialize($form_values['account_fields']);
  }
  // Store only required roles.
  foreach ($form_values['roles'] as $rid => $selected) {
    if (!$selected) {
      unset($form_values['roles'][$rid]);
    }
  }
  $form_values['roles'] = serialize($form_values['roles']);

  //Check for existing record
  $result = db_query("SELECT * FROM {webform_registration} WHERE nid = :nid", array(":nid" => $form_state['values']['node']->nid));
  if ($result->rowCount() < 1) {
    $result = drupal_write_record('webform_registration', $form_values);
  }
  else {
    $result = drupal_write_record('webform_registration', $form_values, 'nid');
  }
  drupal_set_message(t('The registration settings have been updated.'));
  return $result;
}

/**
 * Validation handler for webform_registration_settings_form
 */
function webform_registration_settings_form_validate($form, &$form_state) {
  $form_values = $form_state['values']['webform_registration'];

  if ($form_values['enabled'] && ($form_values['opt_in'] != '<none>' && $form_values['opt_in_value'] == '')) {
    form_set_error('webform_registration][opt_in_value', t('Opt In value is required'));
  }

}
