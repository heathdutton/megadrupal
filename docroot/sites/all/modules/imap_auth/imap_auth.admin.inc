<?php

/**
 * @file
 *   Admin callbacks and helper function for imap_auth.
 */

/**
 * Admin settings form.
 */
function imap_auth_settings_form($form, &$form_state) {
  $form['imap_auth_enabled'] = array(
    '#type' => 'radios',
    '#title' => t('Enable IMAP Auth'),
    '#description' => t('Enable to allow user to login using an outside email account'),
    '#options' => array('Disabled', 'Enabled'),
    '#default_value' => variable_get('imap_auth_enabled', 0),
  );

  $form['imap_auth_allow_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow all servers'),
    '#description' => t('Allow any server to validate the users login'),
    '#default_value' => variable_get('imap_auth_allow_all', 0),
  );

  // get available roles
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);

  $form['imap_auth_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#description' => t('Select the role(s) you would like to assign to new users created by IMAP Auth.'),
    '#options' => $roles,
    '#default_value' => variable_get('imap_auth_roles', array()),
  );

  // get an array of all servers
  $servers = _imap_auth_get_servers();

  // create a form row for each server
  foreach ($servers as $server) {
    $form['servers'][$server->name]['name'] = array(
      '#type' => 'item',
      '#markup' => check_plain($server->name),
    );

    $form['servers'][$server->name]['address'] = array(
      '#type' => 'item',
      '#markup' => check_plain($server->address),
    );

    $form['servers'][$server->name]['port'] = array(
      '#type' => 'item',
      '#markup' => check_plain($server->port),
    );

    $form['servers'][$server->name]['usessl'] = array(
      '#type' => 'item',
      '#markup' => ($server->usessl) ? t('Yes') : t('No'),
    );

    $form['servers'][$server->name]['enabled'] = array(
      '#type' => 'item',
      '#markup' => ($server->enabled) ? t('Yes') : t('No'),
    );

    $form['servers'][$server->name]['edit'] = array(
      '#type' => 'item',
      '#markup' => l(t('edit'), 'admin/config/people/imap_auth/server/' . $server->sid . '/edit'),
    );

    $form['servers'][$server->name]['delete'] = array(
      '#type' => 'item',
      '#markup' => l(t('delete'), 'admin/config/people/imap_auth/server/' . $server->sid . '/delete'),
    );
  }

  $form['add'] = array(
    '#type' => 'item',
    '#markup' => l(t('Add new server'), 'admin/config/people/imap_auth/server/add'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Submit the settings form
 */
function imap_auth_settings_form_submit($form, &$form_state) {
  variable_set('imap_auth_enabled', $form_state['values']['imap_auth_enabled']);
  variable_set('imap_auth_allow_all', $form_state['values']['imap_auth_allow_all']);
  variable_set('imap_auth_roles', $form_state['values']['imap_auth_roles']);

  drupal_set_message(t('Settings saved.'));
}

/**
 * Theme the settings form.
 */
function theme_imap_auth_settings_form($variables) {
  $form = $variables['form'];
  $rows = array();
  $output = '';

  $output .= drupal_render($form['imap_auth_enabled']);
  $output .= drupal_render($form['imap_auth_allow_all']);
  $output .= drupal_render($form['imap_auth_roles']);

  // unset form #block element
  unset($form['#block']);

  // add each servers elements to the table rows array
  if (array_key_exists('servers', $form)) {
    foreach ($form['servers'] as $name => $element) {
      if (strpos($name, '#') === FALSE) {
        $rows[] = array(
          drupal_render($element['name']),
          drupal_render($element['address']),
          drupal_render($element['port']),
          drupal_render($element['usessl']),
          drupal_render($element['enabled']),
          drupal_render($element['edit']),
          drupal_render($element['delete']),
        );
      }
    }
    unset($form['servers']);

    $header = array(t('Server Name'), t('Server Address'), t('Port'), t('Use SSL?'), t('Enabled'), '', '');
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }

  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Add/edit server form.
 */
function imap_auth_server_form($form, &$form_state, $action, $sid = NULL) {
  // set some default variables
  $server = new stdClass();
  $server->mbox_name = t('INBOX');
  $btn_text = 'Add Server';

  // load the server that is being edited
  if ($action == 'edit') {
    $server = imap_auth_server_load($sid);
    $btn_text = 'Edit Server';
  }

  $form['type'] = array(
    '#type' => 'radios',
    '#title' => t('Server type'),
    '#description' => t('The type of server.'),
    '#options' => array(
      'imap' => 'IMAP',
      'pop' => 'POP',
      'nntp' => 'NNTP',
    ),
    '#required' => TRUE,
    '#default_value' => isset($server->type) ? $server->type : '',
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Server name'),
    '#description' => t('Server name is the part of the email address that comes
      after the @. For example, if your email address is you@example.com, the
      server name is example.com'),
    '#size' => 20,
    '#required' => TRUE,
    '#default_value' => isset($server->name) ? $server->name : '',
  );

  $form['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Server address'),
    '#description' => t('Server address is the location of the server that
      corresponds to the server name above. For example, imap.example.com.'),
    '#size' => 20,
    '#required' => TRUE,
    '#default_value' => isset($server->address) ? $server->address : '',
  );

  $form['port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#description' => t('Optional. The port that the server runs on. Leave
      blank if you\'re not sure.'),
    '#size' => 5,
    '#default_value' => isset($server->port) ? $server->port : '',
  );

  $form['mbox_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailbox name'),
    '#description' => t('The name of the mailbox or newsgroup to connect to.
      If you\'re unsure what this should be, keep the default.'),
    '#size' => 20,
    '#default_value' => $server->mbox_name,
  );

  $form['usessl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use SSL?'),
    '#description' => t('Check if your server requires a secure connection.'),
    '#default_value' => isset($server->usessl) ? $server->usessl : 0,
  );

  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Check to enable authentication with this server.'),
    '#default_value' => isset($server->enabled) ? $server->enabled : 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('@btn_text', array('@btn_text' => $btn_text)),
  );

  $form['sid'] = array(
    '#type' => 'value',
    '#value' => isset($server->sid) ? $server->sid : '',
  );

  $form['action'] = array(
    '#type' => 'value',
    '#value' => $action,
  );

  return $form;
}

/**
 * Add/edit server form submit handler.
 */
function imap_auth_server_form_submit($form, &$form_state) {
  if ($form_state['values']['action'] == 'add') {
    $nid = db_insert('imap_auth_servers')
      ->fields(array(
        'type' => $form_state['values']['type'],
        'name' => $form_state['values']['name'],
        'address' => $form_state['values']['address'],
        'port' => $form_state['values']['port'],
        'mbox_name' => $form_state['values']['mbox_name'],
        'usessl' => $form_state['values']['usessl'],
        'enabled' => $form_state['values']['enabled'],
      ))
      ->execute();
    if ($nid) {
      drupal_set_message(t('The server was sucessfully added.'));
      $form_state['redirect'] = 'admin/config/people/imap_auth';
    }
    else {
      drupal_set_message(t('There was an error adding the new server.', 'error'));
    }
  }
  elseif ($form_state['values']['action'] == 'edit') {
    $num = db_update('imap_auth_servers')
      ->fields(array(
        'sid' => $form_state['values']['sid'],
        'type' => $form_state['values']['type'],
        'name' => $form_state['values']['name'],
        'address' => $form_state['values']['address'],
        'port' => $form_state['values']['port'],
        'mbox_name' => $form_state['values']['mbox_name'],
        'usessl' => $form_state['values']['usessl'],
        'enabled' => $form_state['values']['enabled'],
      ))
      ->execute();
    if ($num > 0) {
      drupal_set_message(t('The server was sucessfully edited.'));
      $form_state['redirect'] = 'admin/config/people/imap_auth';
    }
    else {
      drupal_set_message(t('There was an error editing the server.', 'error'));
    }
  }
}

/**
 * Server delete form.
 */
function imap_auth_delete_server($form, &$form_state, $sid) {
  $server = imap_auth_server_load($sid);

  $form = array();
  $form['#server'] = $server;
  return confirm_form(
    $form,
    t('Are you sure you want to delete the server %name?', array('%name' => $server->name)),
    'admin/config/people/imap_auth/overview',
    t('All data associated with this server will be lost. This action cannot be undone.'),
    t('Delete'),
    t('Cancel'),
    'imap_auth_server_confirm_delete');
}

/**
 * Server delete form submit handler.
 */
function imap_auth_delete_server_submit($form, &$form_state) {
  db_delete('imap_auth_servers')
  ->condition('sid', $form['#server']->sid)
  ->execute();
  $form_state['redirect'] = 'admin/config/people/imap_auth';
}
