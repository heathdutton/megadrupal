<?php
/**
 * @file
 * Admin page callback file for the uc_order_access module.
 */

/**
 * Gets standard uc_order permission form.
 *
 * This function is a clone of user_admin_permissions() but only for uc_order
 * module.
 *
 * @see user_admin_permissions()
 */
function uc_order_access_uc_order_permissions($rid = NULL) {

  // Retrieve role names for columns.
  $role_names = user_roles();
  if (is_numeric($rid)) {
    $role_names = array($rid => $role_names[$rid]);
  }
  // Fetch permissions for all roles or the one selected role.
  $role_permissions = user_role_permissions($role_names);

  // Store $role_names for use when saving the data.
  $form['role_names'] = array(
    '#type' => 'value',
    '#value' => $role_names,
  );
  // Render role/permission overview:
  $options = array();
  $hide_descriptions = system_admin_compact_mode();

  $modules = array(
    'uc_order' => t('Standard Order Permissions'),
  );

  foreach ($modules as $module => $display_name) {
    if ($permissions = module_invoke($module, 'permission')) {
      $form['permission'][] = array(
        '#markup' => 'Order',
        '#id' => $module,
      );
      foreach ($permissions as $perm => $perm_item) {
        // Fill in default values for the permission.
        $perm_item += array(
          'description' => '',
          'restrict access' => FALSE,
          'warning' => !empty($perm_item['restrict access']) ? t('Warning: Give to trusted roles only; this permission has security implications.') : '',
        );
        $options[$perm] = '';
        $form['permission'][$perm] = array(
          '#type' => 'item',
          '#markup' => $perm_item['title'],
          '#description' => theme('user_permission_description', array('permission_item' => $perm_item, 'hide' => $hide_descriptions)),
        );
        foreach ($role_names as $rid => $name) {
          // Builds arrays for checked boxes for each role.
          if (isset($role_permissions[$rid][$perm])) {
            $status[$rid][] = $perm;
          }
        }
      }
    }
  }

  // Have to build checkboxes here after checkbox arrays are built.
  foreach ($role_names as $rid => $name) {
    $form['checkboxes'][$rid] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => isset($status[$rid]) ? $status[$rid] : array(),
      '#attributes' => array('class' => array('rid-' . $rid)),
    );
    $form['role_names'][$rid] = array('#markup' => check_plain($name), '#tree' => TRUE);
  }

  $form['#attached']['js'][] = drupal_get_path('module', 'user') . '/user.permissions.js';

  $form['#theme'] = 'user_admin_permissions';

  return $form;
}

/**
 * Permission settings form.
 */
function uc_order_access_perm_form($form, &$form_state) {
  $permissions = array(
    'view order' => array(
      'title' => t('View order'),
    ),
    'edit order' => array(
      'title' => t('Edit order'),
    ),
    'delete order' => array(
      'title' => t('Delete order'),
    ),
    'change status to' => array(
      'title' => t('Change order to this status'),
    ),
    'change status from' => array(
      'title' => t('Change order from this status'),
    ),
  );

  $form['standard_perms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Standard Order Permissions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['standard_perms']['order_permissions'] = uc_order_access_uc_order_permissions();
  $form['standard_perms']['order_permissions']['#tree'] = TRUE;

  // Get and store $role_names for use when saving the data.
  $role_names = user_roles();
  $role_permissions = user_role_permissions($role_names);

  $form['role_names'] = array(
    '#type' => 'value',
    '#value' => $role_names,
  );

  // Get statuses and order permissions.
  $statuses = uc_order_status_list();
  $order_permissions = uc_order_access_get_permissions($statuses, $role_names);

  foreach ($statuses as $status) {
    $form['permission'][$status['id']][] = array(
      '#markup' => $status['title'],
      '#id' => $status['id'],
    );
    foreach ($permissions as $perm => $perm_item) {
      $perm_item += array(
        'description' => '',
      );
      $form['permission'][$status['id']][$perm] = array(
        '#type' => 'item',
        '#markup' => $perm_item['title'],
      );
    }
    foreach ($role_names as $rid => $name) {
      foreach ($permissions as $key => $permission) {
        $checkbox = &$form['checkboxes'][$status['id']][$rid][$key];
        $checkbox = array(
          '#type' => 'checkbox',
          '#title' => $role_names[$rid] . ': ' . $form['permission'][$status['id']][$key]['#markup'],
          '#title_display' => 'invisible',
          '#default_value' => isset($order_permissions[$status['id']][$rid][$key]) ? TRUE : FALSE,
          '#attributes' => array(
            'class' => array(
              'rid-' . $rid,
              str_replace(' ', '-', $key),
            ),
          ),
        );
      }
    }
  }
  foreach ($role_names as $rid => $name) {
    $form['role_names'][$rid] = array('#markup' => check_plain($name), '#tree' => TRUE);
  }
  $form['checkboxes']['#tree'] = TRUE;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save permissions'));

  $form['#attached']['js'][] = drupal_get_path('module', 'uc_order_access') . '/uc_order_access.permissions.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'uc_order_access') . '/uc_order_access.css';

  return $form;
}

/**
 * Returns HTML for the order permissions page.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see theme_user_admin_permissions()
 */
function theme_uc_order_access_perm_form($variables) {
  $form = $variables['form'];
  $roles = user_roles();

  foreach (element_children($form['permission']) as $status) {
    foreach (element_children($form['permission'][$status]) as $key) {
      $row = array();
      if (is_numeric($key)) {
        $row[] = array(
          'data' => drupal_render($form['permission'][$status][$key]),
          'class' => array('status'),
          'id' => 'status-' . $form['permission'][$status][$key]['#id'],
          'colspan' => count($form['role_names']['#value']) + 1,
        );
      }
      else {
        // Permission row.
        $row[] = array(
          'data' => drupal_render($form['permission'][$status][$key]),
          'class' => array('permission'),
        );
        foreach (element_children($form['checkboxes'][$status]) as $rid) {
          $row[] = array(
            'data' => drupal_render($form['checkboxes'][$status][$rid][$key]),
            'class' => array('checkbox'),
          );
        }
      }
      $rows[] = $row;
    }
  }
  $header[] = (t('Permission'));
  foreach (element_children($form['role_names']) as $rid) {
    $header[] = array(
      'data' => drupal_render($form['role_names'][$rid]),
      'class' => array('checkbox'),
    );
  }
  $output = drupal_render($form['standard_perms']);
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'order-permissions')));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Save permissions selected on the orders permissions page.
 *
 * @see uc_order_access_perm_form()
 */
function uc_order_access_perm_form_submit($form, &$form_state) {
  // Update standard order permissions.
  $order_permissions = &$form_state['values']['order_permissions'];
  foreach ($order_permissions['role_names'] as $rid => $name) {
    user_role_change_permissions($rid, $order_permissions['checkboxes'][$rid]);
  }

  // Update order permissions.
  foreach ($form_state['values']['checkboxes'] as $status_id => $roles) {
    foreach ($roles as $rid => $permissions) {
      uc_order_access_change_permissions($status_id, $rid, $permissions);
    }
  }

  drupal_set_message(t('The changes have been saved.'));

  // Clear the cached pages and blocks.
  cache_clear_all();
}
