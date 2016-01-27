<?php

/**
 * @file
 * Manifest module page callbacks and forms.
 */


/**
 * Form builder function for manifest admin form.
 */
function manifest_admin_list() {

  $output = array();
  $manifests = manifest_load_multiple();

  if (!$manifests) {
    $output['no-manifests'] = t('There are currently no manifests configured.');
  }
  else {
    $rows = array();
    foreach ($manifests as $manifest) {
      $rows[] = array(
        array(
          'data' => '<h3>' . manifest_title($manifest) . '</h3><p>' . check_plain($manifest->settings['description']) . '</p>',
          'class' => 'manifest-name',
        ),
        array(
          'data' =>
            l(t('List users'), 'admin/people/manifest/' . $manifest->name . '/list-users')
            . ' | ' .
            l(t('Roles'), 'admin/people/manifest/' . $manifest->name . '/role')
            . ' | ' .
            l(t('IP addresses'), 'admin/people/manifest/' . $manifest->name . '/ip')
            . ' | ' .
            l(t('Edit'), 'admin/people/manifest/' . $manifest->name . '/edit')
            . ' | ' .
            l(t('Delete'), 'admin/people/manifest/' . $manifest->name . '/delete'),
          'class' => 'manifest-ops',
          'align' => 'right',
        ),
      );
    }
    $output['manifest-table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#attributes' => array(
        'class' => 'manifest-table',
      ),
    );
  }

  return $output;

}

/**
 * List manifest users.
 */
function manifest_admin_list_users($manifest) {
  drupal_set_title(t('List of users in !manifest', array('!manifest' => manifest_title($manifest))));

  $users = manifest_users(NULL, array($manifest->name), NULL, NULL, TRUE);
  $items = array();
  if (!empty($users)) {
    foreach ($users as $user) {
      if ($account = user_load($user->uid)) {
        $item = theme('username', array('account' => $account));
        $item .= ' (' . t('reason') . ': ';
        $reasons = array();
        if ($user->by_user) {
          $reasons[] = t('account');
        }
        if ($user->by_role) {
          $reasons[] = t('role');
        }
        $item .= implode(', ', $reasons) . ')';
        $items[] = $item;
      }
    }
  }
  $out[] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#title' => manifest_title($manifest),
  );
  $out[] = array(
    '#prefix' => '<div class="description">',
    '#markup' => t('Note: Users affected by IP Address only are not listed.'),
    '#suffix' => '</div>',
  );
  return $out;
}

/**
 * Admin manifest add/edit page.
 */
function manifest_admin_edit($form, $form_state, $manifest = NULL) {
  $form_state['storage']['manifest'] = $manifest;

  $form['#tree'] = TRUE;
  $machine_name = '';
  if (!isset($manifest->name)) {
    $form['new'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Machine name'),
      '#default_value' => '',
      '#maxlength' => 32,
      '#size' => 32,
      '#required' => TRUE,
      '#description' => t('The machine-readable name of this manifest. This name must contain only lowercase letters, numbers, and underscores. This name must be unique.'),
    );
  }
  else {
    $form['name'] = array(
      '#type' => 'value',
      '#value' => $manifest->name,
    );
    $machine_name = ' ' . t('(machine name: %name)', array('%name' => $manifest->name));
  }
  $form['settings']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Display title'),
    '#default_value' => isset($manifest->settings['title']) ? $manifest->settings['title'] : '',
    '#maxlength' => 32,
    '#size' => 32,
    '#description' => t('The human-readable title of this manifest.') . $machine_name,
  );
  $form['settings']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => isset($manifest->settings['description']) ? $manifest->settings['description'] : '',
    '#description' => t('This will appear on the manifest administration page.'),
  );
  $form['settings']['user'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Add user accounts'),
    '#default_value' => isset($manifest->settings['user']) ? $manifest->settings['user'] : array('profile', 'operations'),
    '#options' => array(
      'profile' => t('Through the user profile page'),
      'operations' => t('Through the user update options'),
    ),
    '#description' => t('How to add user accounts to this manifest.'),
  );

  $form['config'] = module_invoke_all('manifest_config', 'form', $manifest);
  $form['config']['#tree'] = FALSE;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save manifest'),
  );
  if ($manifest) {
    $form['submit']['#suffix'] = l(t('Delete'), 'admin/people/manifest/' . $manifest->name . '/delete');
  }
  else {
    $form['submit']['#suffix'] = l(t('Cancel'), 'admin/people/manifest');
  }
  return $form;
}

/**
 * Validate function for admin manifest add/edit page.
 */
function manifest_admin_edit_validate($form, &$form_state) {
  $values = &$form_state['values'];
  if (!empty($values['new'])) {
    $manifest_name = trim($values['name']);
    if (manifest_load($manifest_name) !== FALSE) {
      form_set_error('name', t('The machine-readable name %manifest is already taken.', array('%manifest' => $manifest_name)));
    }
    if (!preg_match('!^[a-z0-9_]+$!', $manifest_name)) {
      form_set_error('name', t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
    }
  }
}

/**
 * Submit function for admin manifest add/edit page.
 */
function manifest_admin_edit_submit($form, &$form_state) {
  $values = &$form_state['values'];
  $manifest = (object)array(
    'name' => trim($values['name']),
    'settings' => $values['settings'],
  );
  $fields = module_invoke_all('manifest_config', 'fields', $manifest);
  manifest_save($manifest);
  manifest_config_delete($manifest->name);
  if (!empty($fields)) {
    foreach ($fields as $field) {
      manifest_config_save($manifest->name, $field, $values[$field]);
    }
  }
  drupal_set_message(t("The manifest was saved."));
  drupal_goto("admin/people/manifest");
}

/**
 * Admin finder delete page.
 */
function manifest_admin_delete($form, $form_state, $manifest) {
  $form['#tree'] = TRUE;
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $manifest->name,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the %manifest manifest?', array('%manifest' => manifest_title($manifest))),
    $_GET['destination'] ? $_GET['destination'] : 'admin/people/manifest',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel'));
}

/**
 * Submit function for admin manifest delete page.
 */
function manifest_admin_delete_submit($form, &$form_state) {
  $manifest_name = &$form_state['values']['name'];
  manifest_delete($manifest_name);
  drupal_set_message(t("The manifest was deleted."));
  drupal_goto("admin/people/manifest");
}

/**
 * Admin manifest role page.
 */
function manifest_admin_role($form, $form_state, $manifest = NULL) {
  drupal_set_title(t('!manifest roles', array('!manifest' => manifest_title($manifest))));
  $role_entries = manifest_role_load(NULL, $manifest->name);
  $roles = array();
  if (is_array($role_entries)) {
    foreach ($role_entries as $role_entry) {
      $roles[$role_entry->rid] = $role_entry->rid;
    }
  }

  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('User roles'),
    '#default_value' => $roles,
    '#options' => user_roles(),
    '#description' => t('Users in these roles will be added to this Manifest by default.'),
  );
  $form['manifest'] = array(
    '#type' => 'value',
    '#value' => $manifest->name,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save roles'),
    '#suffix' => l(t('Cancel'), 'admin/people/manifest'),
  );
  return $form;
}

/**
 * Submit function for admin manifest role page.
 */
function manifest_admin_role_submit($form, &$form_state) {
  $values = &$form_state['values'];
  $roles = $values['roles'];
  foreach ($roles as $key => $value) {
    if ($value) {
      manifest_role_save($key, $values['manifest']);
    }
    else {
      manifest_role_delete($key, $values['manifest']);
    }
  }
  drupal_set_message(t("The manifest roles were saved."));
  drupal_goto("admin/people/manifest");
}

/**
 * Admin manifest ip page.
 */
function manifest_admin_ip($form, $form_state, $manifest = NULL) {
  drupal_set_title(t('!manifest IP addresses', array('!manifest' => manifest_title($manifest))));
  $ip_entries = manifest_ip_load(NULL, NULL, $manifest->name);
  $rows = array();
  $ips = '';
  if (is_array($ip_entries)) {
    foreach ($ip_entries as $ip_entry) {
      $ip = $ip_entry->ip1;
      if ($ip_entry->ip2 && $ip_entry->ip2 != $ip_entry->ip1) {
        $ip .= ' - ' . $ip_entry->ip2;
      }
      $rows[] = array(
        $ip,
        l(t('Delete'), 'admin/people/manifest/' . $manifest->name . '/ip/' . $ip . '/delete'),
      );
    }
    $ips .= theme('table', array(t('IP addresses'), t('Operations')), $rows);
  }
  $form['ips'] = array(
    '#value' => $ips,
  );
  $form['ip1'] = array(
    '#title' => t('IP address, or IP range start'),
    '#type' => 'textfield',
    '#size' => 15,
    '#maxlength' => 15,
  );
  $form['ip2'] = array(
    '#title' => t('IP range end'),
    '#type' => 'textfield',
    '#size' => 15,
    '#maxlength' => 15,
    '#description' => t('Leave blank if specifying a single IP address.'),
  );
  $form['manifest'] = array(
    '#type' => 'value',
    '#value' => $manifest->name,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save IP addresses'),
    '#suffix' => l(t('Cancel'), 'admin/people/manifest'),
  );
  return $form;
}

/**
 * Submit function for admin manifest ip page.
 */
function manifest_admin_ip_submit($form, &$form_state) {
  $values = &$form_state['values'];
  $ip1 = $values['ip1'];
  $ip2 = $values['ip2'];
  manifest_ip_save($ip1, $ip2, $values['manifest']);
  drupal_set_message(t("The manifest IP addresses were saved."));
  drupal_goto('admin/people/manifest/' . $values['manifest'] . '/ip');
}

/**
 * Admin finder delete page.
 */
function manifest_admin_ip_delete($form, $form_state, $manifest, $ip) {
  $form['#tree'] = TRUE;
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $manifest->name,
  );
  $form['ip'] = array(
    '#type' => 'value',
    '#value' => $ip,
  );
  return confirm_form($form,
    t('Are you sure you want to delete %ip from the %manifest manifest?', array('%ip' => $ip, '%manifest' => manifest_title($manifest))),
    $_GET['destination'] ? $_GET['destination'] : 'admin/people/manifest',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel'));
}

/**
 * Submit function for admin manifest delete page.
 */
function manifest_admin_ip_delete_submit($form, &$form_state) {
  $manifest_name = &$form_state['values']['name'];
  $manifest_ip = &$form_state['values']['ip'];
  $ips = explode(' - ', $manifest_ip);
  manifest_ip_delete($ips[0], $ips[1], $manifest_name);
  drupal_set_message(t("The IP address was deleted."));
  drupal_goto('admin/people/manifest/' . $manifest_name . '/ip');
}