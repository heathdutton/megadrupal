<?php

/**
 * @file
 * Administrative page callbacks for the Flexi Access module.
 */


/**
 * @todo:
 * - Theme
 */

/**
 * Form builder to display a listing of content types and their ACL status.
 */
function flexiaccess_admin_content($form) {
  $names = node_type_get_names();
  $flexiaccess_types = variable_get('flexiaccess_types', array());

  $form['flexiaccess']['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types:'),
    '#default_value' => variable_get('flexiaccess_types', array()),
    '#options' => $names,
    '#description' => t('Check the content types you want to assign ACLs using Flexi Access.  Then press &#8220;Save&#8221;.'),
  );

  $flexiaccess_types_enabled = array_filter($flexiaccess_types);
  $form['flexiaccess']['typesettings'] = array(
    '#type'        => 'fieldset',
    '#tree'        => TRUE,
    '#title'       => 'Per-type settings',
    '#description' => 'To configure flexiaccess settings for a specific content type, enable it first, above.',
  );
  foreach($flexiaccess_types_enabled as $type) {
    $form['flexiaccess']['typesettings'][$type] = array(
      '#type'          => 'fieldset',
      '#title'         => $names[$type],
      '#collapsible'   => TRUE,
      '#collapsed'     => TRUE,
    );
    $form['flexiaccess']['typesettings'][$type]['autoACL'] = array(
      '#type'          => 'checkbox',
      '#title'         => 'Automatic ACL creation',
      '#description'   => 'Enable this for content types which should have restricted access by default. This will create an empty ACL for every new node created, thereby restricting access to the node by all users. This does not affect existing nodes.',
      '#default_value' => variable_get('flexiaccess_typesettings_'.$type.'_autoACL', 0),
    );
  }

  $form['flexiaccess']['priority'] = array(
    '#type' => 'textfield',
    '#size' => 2,
    '#title' => t('Default priority for new ACL entries'),
    '#default_value' => variable_get('flexiaccess_priority', 0),
    '#description' => t('With every ACL entry (which translate to entries in the node_access table) there is an associated priority.  Read !url for more information.  You only need to change this if you understand what it does, and you want to integrate Flexi Access with other access control modules such as TAC.  Note that changing this value does not change previously created ACL entries.',
array('!url' => l(t('this page'), 'https://drupal.org/node/765832', array('fragment' => 'priority','external' => TRUE,'attributes' => array('title' => t('Grant priority.')),)))));

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}


/**
 * Callback for the submit button.
 */
function flexiaccess_admin_content_submit($form, &$form_state) {
  if (isset($form_state['values']['types'])) {
    variable_set('flexiaccess_types', $form_state['values']['types']);
    variable_set('flexiaccess_priority', $form_state['values']['priority']);
    if (isset($form_state['values']['typesettings'])) {
      foreach($form_state['values']['typesettings'] as $type => $ts) {
        variable_set('flexiaccess_typesettings_'.$type.'_autoACL', $ts['autoACL']);
      }
    }
  }

  drupal_set_message(t('Flexi Access changes have been updated.'));
  cache_clear_all();
}


/**
 * Form builder to display a listing of content types and their ACL status.
 */
function flexiaccess_admin_anon($form) {
  $anon = user_load(0);
  if ($anon != FALSE) {
    $uname = $anon->name;
  }
  else {
    $uname = '';
  }
  $form = array();
  $form['flexiacceess_anon']['intro'] = array(
    '#markup' => '<p>' . t('This admin-page lets you inspect the name of the Anonymous user as it is represented in the {users} table in the database, and change it if it is required.') . '</p>',
  );
  if (!empty($uname)) {
    $nname = $uname;
    $form['flexiacceess_anon']['anonprofile'] = array(
      '#markup' => '<p>' . t('The Anonymous user is currently known to Flexi Access as &#8220;!anon&#8221;.', array('!anon' => $uname)) . '</p>',
    );
    $default = 0;
  }
  else {
    $uid = 1;
    $ii = 0;
    while ($uid) {
      $iii = sprintf('%d', $ii++);
      $nname = 'anon' . $iii;
      $uid = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $nname))->fetchAssoc();
      //drupal_set_message("[$nname | $uid]");
    }
    //drupal_set_message("[$nname]");

    $form['flexiacceess_anon']['anonprofile'] = array(
      '#markup' => '<p>' . t('The current name of the Anonymous user in the {users} table is the empty string.  This the default for Drupal 7, but it rules out having the Anonymous user in an ACL.') . '</p>',
    );
    $default = 1;
  }

  $form['flexiacceess_anon']['nname'] = array(
    //'#title' => 'ID',
    '#value' => $nname,
    '#type' => 'hidden',
  );
  $form['flexiacceess_anon']['howshall'] = array(
    '#type' => 'radios',
    '#title' => t('How shall the Anonymous user\'s name be represented in the {users} table in the database?'),
    '#default_value' => $default,
    '#options' => array(
      t('Store &#8220;@name&#8221; as the name the Anonymous user (pick this option to use have the Anonymous user in ACLs).', array('@name' => $nname)),
      t('Make the name of the Anonymous user an empty string (pick this option to reset this to Drupal 7 default).'),
    ),
    '#description' => t('Choose how you want the name of the Anonymous user represented in the database and press <em>Execute</em> to execute.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Execute'));

  return $form;
}

/**
 * Callback for the submit button.
 */
function flexiaccess_admin_anon_submit($form, &$form_state) {
  if ($form_state['values']['howshall']) {
    $nname = '';
    $target = 'an empty string';
  }
  else {
    $nname = $form_state['values']['nname'];
    $target = $nname;
  }
  $query = db_update('users')
    ->fields(array(
      'name' => $nname,
    ))
    ->condition('uid', 0)
    ->execute();
  drupal_set_message(t('Anonym user\'s name set to !target.', array('!target' => $target)));
}


/**
 * Form builder to display available bulk operations.
 */
function flexiaccess_admin_bulkop($form, &$form_state) {
  $options = array(
    0 => t('Reset all ACL priorities'),
    1 => t('Delete all ACLs'),
  );
  $descriptions = array(
    0 => t('Sets the priority of every ACL entry created by Flexi Access to the value specified on the configuration page.'),
    1 => t('Remove all ACLs created by Flexi Access.'),
  );

  if (isset($form_state['values']['action_list'])) {
    // build confirmation form
    $form['action_chosen'] = array(
      '#type'  => 'value',
      '#value' => $form_state['values']['action_list']
    );
    $form = confirm_form($form, t('Are you sure?'), current_path(), t('You are about to perform the following bulk operation: %operation  This cannot be reversed, and could affect many nodes and users.  Use with caution.',array('%operation' => $options[$form_state['input']['action_list']])));
  } else {
    // build form for first time

    $form['action_list'] = array(
      '#type'          => 'radios',
      '#required'      => TRUE,
      '#options'       => $options,
    );
    // add description to each radio option
    foreach ($form['action_list']['#options'] as $key => $label) {
      $form['action_list'][$key]['#description'] = $descriptions[$key];
    }

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit','#value' => t('Do it!'));
  }

  return $form;
}


/**
 * Callbacks for the bulk operations
 */
function flexiaccess_admin_bulkop_submit($form, &$form_state) {
  if (isset($form_state['values']['action_chosen'])) {
    // user has confirmed
    switch($form_state['values']['action_chosen']) {
      case 0: // Reset ACL priorities
        flexiaccess_reset_priorities();
        break;
      case 1: // Delete all ACLs
        flexiaccess_delete_all();
        break;
    }
    return TRUE;
  }
  // Otherwise prepare the confirmation dialog
  $form_state['rebuild'] = TRUE;
}
