<?php

/**
 * @file
 * Callbacks for administrator forms in Permission watchdog.
 */

/**
 * Main configuration form.
 *
 * @return array
 */
function permission_watchdog_admin_settings() {
  $roles = array_merge(array('all' => t('All roles')),  user_roles());

  $form['permission_watchdog_roles'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Track changes to permissions on the following roles.'),
    '#options' => $roles,
    '#default_value' => variable_get('permission_watchdog_roles', array()),
  );

  return system_settings_form($form);
}

/**
 * Permission change report.
 *
 * Display a log of changes to role permissions.
 *
 * @return array
 */
function permission_watchdog_report($form, $form_state) {
  $form = array();
  $headers = array(t('User'), t('Role'), t('Timestamp'), t('Changes'));
  $module_info = system_get_info('module');
  $permission_labels = array();
  foreach (module_implements('permission') as $module) {
    $modules[$module] = $module_info[$module]['name'];
  }
  foreach ($modules as $module => $display_name) {
    $permissions = module_invoke($module, 'permission');
    if ($permissions) {
      foreach ($permissions as $k => $permission) {
        $permissions[$k]['module'] = $display_name;
      }
      $permission_labels = array_merge($permission_labels, $permissions);
    }
  }
  $query = db_select('permission_watchdog', 'w');
  $count_query = clone $query;
  $count_query->addExpression('COUNT(w.change_id)');

  $session = (!empty($_SESSION['permission_watchdog_filter'])) ? $_SESSION['permission_watchdog_filter'] : array();

  if (!empty($session['roles'])) {
    $query->condition('rid', $session['roles'], 'IN');
  }

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('w', array('uid', 'rid', 'stamp', 'data'))
    ->limit(30)
    ->setCountQuery($count_query);
  $result = $query->execute();

  $rows = array();

  foreach ($result as $row) {
    $changes = unserialize($row->data);
    $role = user_role_load($row->rid);
    $items = array();
    foreach ($changes as $permission => $change) {
      $change = ($change) ? t('Added') : t('Removed');
      $items[] = t('%change <strong>@module:</strong> !permission', array('%change' => $change,
                    '@module' => $permission_labels[$permission]['module'],
                    '!permission' => $permission_labels[$permission]['title']));
    }
    $rows[] = array(theme('username', array('account' => user_load($row->uid))),
                    $role->name,
                    format_date($row->stamp),
                    theme('item_list', array('items' => $items)));
  }

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => 'Filters',
  );

  $form['filters']['roles'] = array(
    '#type' => 'select',
    '#multiple' => 'multple',
    '#title' => t('Roles'),
    '#options' => user_roles(),
    '#default_value' => isset($session['roles']) ? $session['roles'] : FALSE,
  );

  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $form['table'] = array('#markup' => theme('table', array('header' => $headers, 'rows' => $rows)));

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * Magic form submission handler.
 */
function permission_watchdog_report_submit($form, $form_state) {
  $_SESSION['permission_watchdog_filter'] = array();
  foreach ($form_state['values']['roles'] as $rid => $role) {
    if ($role) {
      $_SESSION['permission_watchdog_filter']['roles'][$rid] = $rid;
    }
  }
}
