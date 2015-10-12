<?php

/**
 * @file
 * Admin page callback file for the node permissions grid pages.
 */

/**
 * Menu callback: role list.
 */
function node_permissions_grid_admin_roles() {
  $header = array(t('Name'), array('data' => t('Operations'), 'colspan' => 2));

  foreach (user_roles() as $rid => $name) {
    $edit_permissions = l(t('edit node permissions'), 'admin/people/permissions/node_permissions/' . $rid);
    switch ($rid) {
      case DRUPAL_ANONYMOUS_RID:
      case DRUPAL_AUTHENTICATED_RID:
        $rows[] = array(
          $name,
          t('locked'),
          $edit_permissions,
        );
        break;
      default:
        $rows[] = array(
          $name,
          l(t('edit role'), 'admin/people/permissions/roles/edit/' . $rid),
          $edit_permissions,
        );
    }
  }

  $output['roles_list'] = array(
    '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
  );

  return $output;
}

/**
 * Form builder: administer permissions.
 *
 * @see theme_user_node_permissions_grid_admin_node_permissions()
 *
 * @param $role
 *  A fully-loaded role object from user_role_load().
 *  @todo: implement monster page of all roles when this is NULL
 */
function node_permissions_grid_admin_node_permissions($form, &$form_state, $role = NULL) {  
  //dsm($role);
  
  // Get node types list so we have human labels.
  $node_types = node_type_get_types();

  if (isset($role)) {
    $rid = $role->rid;
    $role_names = array($rid => $role->name); // old?
  }
  else {
    $role_names = user_roles();
  }
  
  $form['role_names'] = array(
    '#type' => 'value',
    '#value' => $role_names,
  );

  // Node module creates standard node permissions for all applicable node types.
  $node_permissions = array();
  $configured_types = node_permissions_get_configured_types();
  foreach ($configured_types as $type) {
    // Generate the standard permissions for each type.
    $type_permissions = node_list_permissions($type);
    
    // Extract the action verb from each machine permission and
    // add it to the data array.
    foreach ($type_permissions as $perm => $perm_data) {
      $pos = strpos($perm, $type);
      $action = substr($perm, 0, $pos - 1);
      $type_permissions[$perm]['action'] = $action;
    }

    //dsm($type_permissions);
    
    // Build a flat array of all permisions.
    $node_permissions += $type_permissions;
    
    // Build the form data.
    $form['permission'][$type] = array(
      '#markup' => $node_types[$type]->name,
      //'type'   => $type, // store the machine name too?
    );      
    foreach ($type_permissions as $perm => $perm_data) {
      $form['permission'][$type]['perms'][$perm]['#markup'] = $perm_data['title'];
    }
  }
  
  /*
  // @todo Other modules provide permissions if they have
  // variable_get('node_permissions_' . $type, TRUE) == FALSE
  // @see http://drupal.org/node/858398
  This is actually going to be a monster amount of faff to handle,
  as first we need to do module_invoke_all('node_info') to find out
  which module provides which node type, because node_type_get_types()
  gives us the 'base' of the type, not the module.
  Then for each type we have to check the node_permissions_TYPE variable.
  If that is explicitly FALSE, then we can finally invoke
  hook_permissions() on that module and see what permissions it is
  trying to provide for nodes.
  And of course this is assuming it's providing permissions that conform 
  to any kind of pattern!
  The upshot is: as I know of no modules so far that are doing this,
  I'm not going to implement this.
  If you find one, please file an issue!
  */
  
  // Fetch permissions for the one selected role.
  // Gets an array keyed by rid, with an array of permission => TRUE
  $role_permissions = user_role_permissions($role_names);
  //dsm($role_permissions);
  
  // Build the checkboxes.
  //dsm($node_permissions);
  $checkboxes_default = array();
  foreach ($node_permissions as $perm => $perm_data) {
    // Titles come in already translated from node_list_permissions().
    $checkboxes_options[$perm] = $perm_data['title'];
    if (isset($role_permissions[$rid][$perm])) {
      $checkboxes_default[] = $perm;
    }
  }

  // Get permissions for the authenticated user role and remove the options
  // for permissions implicitly granted to the current role. The theme function
  // takes care of adding dummy checkboxes for these permissions.
  $authenticated_perms = array();
  if ($rid != DRUPAL_AUTHENTICATED_RID && $rid != DRUPAL_ANONYMOUS_RID) {
    $authenticated_perms = user_role_permissions(array(DRUPAL_AUTHENTICATED_RID => DRUPAL_AUTHENTICATED_RID));
    foreach (array_keys($authenticated_perms[DRUPAL_AUTHENTICATED_RID]) as $permission) {
      unset($checkboxes_options[$permission]);
    }
  }

  $form['checkboxes'][$rid] = array(
    '#type' => 'checkboxes', 
    '#options' => $checkboxes_options, 
    '#default_value' => $checkboxes_default,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save permissions'));    
  
  //dsm($form);
  return $form;
}


/**
 * Theme the administer permissions form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 */
function theme_node_permissions_grid_admin_node_permissions($variables) {
  $form = $variables['form'];
  //dsm($form);
  
  $output = '';
  foreach ($form['role_names']['#value'] as $rid => $name) {
    // Create the table header.
    $header = array(
      t('Content type'),
      t('Create'),
      t('Edit own'),
      t('Edit any'),
      t('Delete own'),
      t('Delete any'),
    );
  
    foreach (element_children($form['permission']) as $type) {
      // One row for each content type.
      $row = array();
      $row[] = drupal_render($form['permission'][$type]);
      foreach (element_children($form['permission'][$type]['perms']) as $permission) {
        // Render the checkboxes for the permissions for this type row.
        if (isset($form['checkboxes'][$rid][$permission])) {
          // Create labels with element-invisible for accessibility.
          $form['checkboxes'][$rid][$permission]['#title'] = $name . ': ' . $form['checkboxes'][$rid][$permission]['#title'];//. t($key);
          $form['checkboxes'][$rid][$permission]['#title_display'] = 'invisible';

          // One cell for each permission.
          $row[] = array(
            'data' => drupal_render($form['checkboxes'][$rid][$permission]),
            'class' => array('checkbox'),
          );
        }
        else {
          // If the permission doesn't exist in the form array, it's because
          // it was removed due to the authenticated user having the it too.
          // Show a dummy greyed out selected checkbox.
          $row[] = array(
            'data' => '<input type="checkbox" class="form-checkbox" checked="checked" disabled="disabled">',
            'class' => array('checkbox'),
          );
        }
      }
    
      $rows[] = $row;
    }
    //dsm($rows);

    $output .= '<h3>Node permissions for ' . check_plain($name) . '</h3>';

    $output .= theme('table', array(
      'rows'    => $rows,
      'header'  => $header,
      //'caption' => check_plain($name),
      'attributes' => array('id' => 'permissions'),
      'sticky' => TRUE,
    ));
  }
  $output .= drupal_render_children($form);

  return $output;
} 


/**
 * Submit the form.
 */
function node_permissions_grid_admin_node_permissions_submit($form, &$form_state) {
  drupal_set_message(t('The changes have been saved.'));

  // Save the changed permissions.
  foreach ($form_state['values']['role_names'] as $rid => $name) {
    user_role_change_permissions($rid, $form_state['values'][$rid]);
  }
}
