<?php

/**
 * @file
 * TODO.
 */

/* ################ */
/* # Global Admin # */
/* ################ */

/* Overview */

/**
 * Page callback.
 *
 * This page display a summary table of the configuration for
 * every bundle of every entity type.
 */
function gcc_admin_overview() {

  $entity_info = entity_get_info();
  $features_info = gcc_features_get_info();

  $gcc_entity = FALSE;
  foreach ($entity_info as $info) {
    if (isset($info['gcc'])) {

      $gcc_entity = TRUE;
      break;
    }
  }

  if (!$gcc_entity) {
    return t('To start using group, You must enable at least one of the <strong>GCC Entity</strong> modules !link', array('!link' => l(t('here'), 'admin/modules')));
  }

  $headers = array(

    t('Entity Type'),
    t('Bundle'),
    t('Configuration'),
    t('Operations'),
  );

  $rows = array();

  foreach ($entity_info as $entity_type => $info) {

    // Skip non GCC Entity.
    if (!isset($info['gcc'])) {
      continue;
    }

    foreach ($info['bundles'] as $bundle => $bundle_info) {

      $row = array();
      $row[] = $info['label'];
      $row[] = $bundle_info['label'];

      $features = gcc_features_get_bundle_info($entity_type, $bundle);

      $configuration = array();
      foreach ($features as $key => $feature) {
        if ($feature['enabled']) {
          $configuration[] = $features_info[$key]['explaination'];
        }
      }
      if (empty($configuration)) {
        $configuration[] = t('No group features enabled');
      }

      $row[] = theme('item_list', array('items' => $configuration));

      $path = $bundle_info['admin']['path'];
      if (isset($bundle_info['admin']['real path'])) {
        $path = $bundle_info['admin']['real path'];
      }

      $row[] = l(t('Edit Configuration'), $path . '/gcc', array(

        'query' => array('destination' => 'admin/config/gcc'),
      ));

      $rows[] = $row;
    }
  }

  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/* Bundle Administration */

/**
 * Form builder.
 */
function gcc_admin_bundle($form, $form_state, $entity_type, $bundle) {

  $form = array();
  $bundle = field_extract_bundle($entity_type, $bundle);

  $form['#entity_type'] = $entity_type;
  $form['#bundle'] = $bundle;

  $features_info = gcc_features_get_info();
  $features = gcc_features_get_bundle_info($entity_type, $bundle);

  $form['features'] = array('#tree' => TRUE);
  foreach ($features as $key => $info) {

    $form['features'][$key] = array(

      '#type' => 'checkbox',
      '#title' => $features_info[$key]['label'],
      '#description' => $info['description'],
      '#default_value' => $info['enabled'],
    );
  }

  $form['submit'] = array(

    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );

  return $form;
}

/**
 * TODO.
 */
function gcc_admin_bundle_submit($form, $form_state) {

  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];

  $features_info = gcc_features_get_info();
  $features = gcc_features_get_bundle_info($entity_type, $bundle);

  foreach ($features as $key => $info) {

    if ($form_state['values']['features'][$key] && !$info['enabled']) {
      module_invoke($features_info[$key]['module'], 'gcc_features_enable', $key, $entity_type, $bundle);
    }
    elseif (!$form_state['values']['features'][$key] && $info['enabled']) {
      module_invoke($features_info[$key]['module'], 'gcc_features_disable', $key, $entity_type, $bundle);
    }
  }

  drupal_set_message(t('Configuration saved.'));
}

/* ################ */
/* # Common Admin # */
/* ################ */

/* Role Administration */

/**
 * Form builder for the group global role administration page.
 */
function gcc_admin_role($form, $form_state, $entity_type, $entity, $base_path) {

  $form = array();

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $roles = gcc_role_get_list($entity_type, $entity_id);

  $contexts = array(
    GCC_GLOBAL_TYPE . ':' . GCC_GLOBAL_BUNDLE => t('Every bundle of every entity'),
  );

  $list = gcc_features_get_enabled_bundles('group');
  foreach ($list as $type => $bundles) {
    foreach ($bundles as $bundle => $name) {
      $contexts[$type . ':' . $bundle] = t('Group of type !type', array('!type' => $type . ' : ' . $name));
    }
  }

  $headers = array(

    'name' => t('Name'),
    'context' => t('Context'),
    'edit' => t('Operations'),
  );

  $rows = array();
  foreach ($roles as $role) {

    $links = '';
    if (!gcc_role_is_default($role->rid)) {
      if ($role->entity_type == $entity_type && $role->entity_id == $entity_id ||
          $entity_type == GCC_GLOBAL_TYPE && $role->entity_id == 0) {
        $links = l(t('Edit'), $base_path . '/gcc/role/' . $role->rid . '/edit') . ' | ' . l(t('Delete'), $base_path . '/gcc/role/' . $role->rid . '/delete');
      }
    }

    $row = array();
    $row[] = check_plain($role->name);

    if (gcc_role_is_global($role)) {
      $row[] = $contexts[$role->entity_type . ':' . $role->bundle];
    }
    else {
      $row[] = t('This group');
    }

    $row[] = $links;

    $rows[] = $row;
  }

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#base_path'] = $base_path;

  $form['roles'] = array(

    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
  );

  $form['add'] = array(

    '#type' => 'fieldset',
    '#title' => t('Add a new role'),
  );

  $form['add']['name'] = array(

    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
  );

  $form['add']['rid'] = array(

    '#type' => 'machine_name',
    '#required' => TRUE,
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'gcc_admin_role_exists',
      'source' => array('add', 'name'),
    ),
  );

  if (gcc_is_global_group($entity_type, $entity_id)) {

    $form['add']['entity_type'] = array(

      '#type' => 'select',
      '#title' => t('Context'),
      '#description' => t('Select in which context this role will be available'),
      '#options' => $contexts,
      '#default_value' => GCC_GLOBAL_TYPE . ':' . GCC_GLOBAL_BUNDLE,
      '#required' => TRUE,
    );
  }
  else {

    $form['add']['entity_type'] = array(
      '#type' => 'value',
      '#value' => $entity_type,
    );

    list(,,$bundle) = entity_extract_ids($entity_type, $entity);
    $form['add']['bundle'] = array(
      '#type' => 'value',
      '#value' => $bundle,
    );
  }

  $form['add']['submit'] = array(

    '#type' => 'submit',
    '#submit' => array('gcc_admin_role_add'),
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * TODO.
 */
function gcc_admin_role_add($form, &$form_state) {

  $entity_type = $form['#entity_type'];
  $entity_id = $form['#entity_id'];

  if (gcc_is_global_group($entity_type, $entity_id)) {
    list($form_state['values']['entity_type'], $form_state['values']['bundle']) = explode(':', $form_state['values']['entity_type']);
  }
  else {
    $form_state['values']['rid'] = $entity_type . '-' . $entity_id . '-' . $form_state['values']['rid'];
  }

  gcc_role_add($form_state['values']['rid'], $form_state['values']['name'], $form_state['values']['entity_type'], $form_state['values']['bundle'], $form['#entity_id']);
  drupal_set_message(t('Role saved.'));
}

/**
 * Exists callback for the role machine name.
 */
function gcc_admin_role_exists($rid) {

  return gcc_role_is_default($rid) || gcc_role_load($rid);
}

/**
 * Return the edit form.
 */
function gcc_admin_role_edit($form, $form_state, $role, $entity_type, $entity, $base_path) {

  $form = array();

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#base_path'] = $base_path;
  $form['#role'] = $role;

  $form['name'] = array(

    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#default_value' => $role->name,
  );

  $form['rid'] = array(

    '#type' => 'machine_name',
    '#required' => TRUE,
    '#maxlength' => 32,
    '#default_value' => $role->rid,
    '#disabled' => TRUE,
    '#machine_name' => array(
      'exists' => 'gcc_admin_role_exists',
    ),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(

    '#type' => 'submit',
    '#value' => t('Save role'),
  );
  $form['actions']['delete'] = array(

    '#type' => 'submit',
    '#value' => t('Delete role'),
    '#submit' => array('gcc_admin_role_delete'),
  );

  return $form;
}

/**
 * TODO.
 */
function gcc_admin_role_edit_submit($form, &$form_state) {

  $role = $form['#role'];
  $role->name = $form_state['values']['name'];
  gcc_role_save($role);

  drupal_set_message(t('Role saved.'));

  $form_state['redirect'] = $form['#base_path'] . '/gcc/role';
}

/**
 * TODO.
 */
function gcc_admin_role_delete($form, &$form_state) {

  $form_state['redirect'] = $form['#base_path'] . '/gcc/role/' . $form['#role']->rid . '/delete';
}

/**
 * Return the edit form.
 */
function gcc_admin_role_delete_confirm($form, $form_state, $role, $entity_type, $entity, $base_path) {

  $form = array();

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#base_path'] = $base_path;
  $form['#role'] = $role;

  return confirm_form(
    $form,
    t('Are you sure you want to delete the role %name ?', array('%name' => $role->name)),
    $base_path . '/gcc/role',
    t('This action cannot be undone.'),
    t('Delete')
  );
}

/**
 * TODO.
 */
function gcc_admin_role_delete_confirm_submit($form, &$form_state) {

  gcc_role_delete($form['#role']->rid);
  drupal_set_message(t('Role deleted.'));

  $form_state['redirect'] = $form['#base_path'] . '/gcc/role';
}

/* Permission Administration */

/**
 * TODO.
 */
function gcc_admin_permissions($form, $form_state, $entity_type, $entity) {

  $form = array();
  $form['#attached']['css'] = array(drupal_get_path('module', 'gcc') . '/gcc-admin.css');

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $roles = gcc_role_get_list($entity_type, $entity_id);
  $permissions = gcc_permission_get_list();

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#roles'] = $roles;

  $form['#permissions'] = array();
  foreach ($permissions as $key => $permission) {
    $form['#permissions'][$permission['group']][$key] = $permission;
  }

  // Allow to override a default role.
  $form['overrides'] = array('#tree' => TRUE);
  foreach ($roles as $role) {
    if (gcc_role_is_global($role) && !gcc_is_global_group($entity_type, $entity_id)) {
      $form['overrides'][$role->rid] = array(

        '#type' => 'checkbox',
        '#title' => '',
        '#default_value' => gcc_role_is_overriden($role->rid, $entity_type, $entity_id),
        '#id' => 'gcc-override-' . $role->rid,
      );
    }
  }

  $form['permissions'] = array('#tree' => TRUE);
  foreach ($roles as $role) {

    $current_permissions = gcc_permission_get_list_by_role($role->rid, $entity_type, $entity_id);
    $form['permissions'][$role->rid] = array('#tree' => TRUE);

    foreach ($permissions as $key => $permission) {

      $form['permissions'][$role->rid][$key] = array(

        '#type' => 'checkbox',
        '#title' => '',
        '#default_value' => $current_permissions[$key],
      );

      if ($role->entity_type != $entity_type || $role->entity_id != $entity_id) {

        $form['permissions'][$role->rid][$key]['#states'] = array(

          'enabled' => array('#gcc-override-' . $role->rid => array('checked' => TRUE)),
          'disabled' => array('#gcc-override-' . $role->rid => array('unchecked' => TRUE)),
        );
      }

      // Fixed exceptions.
      if (in_array($key, array('subscribe', 'skip approval')) && $role->rid != GCC_NON_MEMBER) {

        unset($form['permissions'][$role->rid][$key]['#states']);
        $form['permissions'][$role->rid][$key]['#disabled'] = TRUE;
      }

      if ($key == 'unsubscribe' && $role->rid == GCC_NON_MEMBER) {

        unset($form['permissions'][$role->rid][$key]['#states']);
        $form['permissions'][$role->rid][$key]['#disabled'] = TRUE;
      }

      if ($key == 'administer group' && $role->rid == GCC_ADMIN) {

        unset($form['permissions'][$role->rid][$key]['#states']);
        $form['permissions'][$role->rid][$key]['#disabled'] = TRUE;
        $form['permissions'][$role->rid][$key]['#value'] = TRUE;
      }
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
 * Theme the permission form.
 */
function theme_gcc_admin_permissions($variables) {

  $form = $variables['form'];

  $rows = array();

  if (count(element_children($form['overrides']))) {

    $row = array();
    $row[] = array(

      'data' => t('Override the default permissions'),
      'class' => array('text'),
    );
    foreach ($form['#roles'] as $role) {
      if (isset($form['overrides'][$role->rid])) {
        $row[] = drupal_render($form['overrides'][$role->rid]);
      }
      else {
        $row[] = '';
      }
    }

    $rows[] = array(

      'data' => $row,
      'class' => array('override'),
    );
  }

  foreach ($form['#permissions'] as $group => $permissions) {

    $row = array();
    $row[] = array(

      'data' => $group,
      'colspan' => count($form['#roles']) + 1,
      'class' => array('text'),
    );

    $rows[] = array(

      'data' => $row,
      'class' => array('group-name'),
    );

    foreach ($permissions as $key => $permission) {

      $row = array();
      $row[] = array(

        'data' => $permission['title'],
        'class' => array('text'),
      );

      foreach ($form['#roles'] as $role) {
        $row[] = drupal_render($form['permissions'][$role->rid][$key]);
      }

      $rows[] = array(

        'data' => $row,
        'class' => array('group-perm'),
      );
    }
  }

  $header = array('');
  foreach ($form['#roles'] as $role) {
    $header[] = $role->name;
  }

  $output = theme('table', array(

    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'gcc-permission'),
  ));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Submit handle for the permission form.
 */
function gcc_admin_permissions_submit($form, &$form_state) {

  $entity_type = $form['#entity_type'];
  $entity_id = $form['#entity_id'];

  db_delete('gcc_role_permission')->condition('entity_type', $entity_type)->condition('entity_id', $entity_id)->execute();

  foreach ($form['#roles'] as $role) {

    // If it's a default role, and it's not overriden, skip it.
    if (gcc_role_is_global($role) && !gcc_is_global_group($entity_type, $entity_id)) {
      if (!$form_state['values']['overrides'][$role->rid]) {
        continue;
      }
    }

    $record = new stdClass();
    $record->rid = $role->rid;
    $record->entity_type = $entity_type;
    $record->entity_id = $entity_id;
    foreach (array_filter($form_state['values']['permissions'][$role->rid]) as $key => $value) {
      if ($value) {

        $record->permission = $key;
        drupal_write_record('gcc_role_permission', $record);
      }
    }

    // If it's a default role, and it's overriden, add an extra marker.
    if (gcc_role_is_global($role) && !gcc_is_global_group($entity_type, $entity_id)) {

      $record->permission = '__overriden';
      drupal_write_record('gcc_role_permission', $record);
    }
  }

  drupal_set_message(t('Permissions saved'));
}

/* ############### */
/* # Group Admin # */
/* ############### */

/* Overview */

/**
 * TODO.
 */
function gcc_admin_group_overview($entity_type, $entity) {

  $entity_id = gcc_get_entity_id($entity_type, $entity);

  drupal_set_title(t('Administration of group %group', array('%group' => entity_label($entity_type, $entity))), PASS_THROUGH);

  $content = array();

  $content['info'] = array(

    '#type' => 'fieldset',
    '#title' => t('Group Informations'),
  );

  $owner = user_load(gcc_group_get_owner($entity));

  $text = $owner ? format_username($owner) : t('The owner does not exist.');
  $content['info']['owner'] = array(

    '#type' => 'item',
    '#title' => t('Group Owner'),
    '#markup' => $text,
  );

  $has_field = count(field_read_instances(array('field_name' => GCC_FIELD_AUDIENCE)));

  if ($has_field) {

    $query = new EntityFieldQuery();
    $query->fieldCondition(GCC_FIELD_AUDIENCE, 'entity_type', $entity_type);
    $query->fieldCondition(GCC_FIELD_AUDIENCE, 'entity_id', $entity_id);
    $query->count();
    $count = $query->execute();

    $content['info']['content'] = array(

      '#type' => 'item',
      '#title' => t('Group Content Number'),
      '#markup' => ($count ? format_plural($count, '1 content', '@count contents') : t('0 contents')),
    );
  }
  else {

    $content['info']['content'] = array(

      '#type' => 'item',
      '#title' => t('Group Content Number'),
      '#markup' => t('You need to enable a group content type to have content.'),
    );
  }

  $query = db_select('gcc_membership');
  $query->condition('entity_type', $entity_type);
  $query->condition('entity_id', $entity_id);
  $query = $query->countQuery();
  $count = $query->execute()->fetchField();

  $content['info']['user'] = array(

    '#type' => 'item',
    '#title' => t('Group Member Number'),
    '#markup' => ($count ? format_plural($count, '1 member', '@count members') : t('0 members')),
  );

  return $content;
}

/* Content Administration */

/**
 * Form builder for the group members administration page.
 */
function gcc_admin_content($form, &$form_state, $entity_type, $entity, $base_path) {

  $form = array();

  $type = gcc_features_get_enabled_bundles('group_content');

  if (empty($type)) {

    $form['empty'] = array(
      '#markup' => t('No content currently available.'),
    );

    return $form;
  }

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#entity'] = $entity;
  $form['#base_path'] = $base_path;

  $default_values = isset($_SESSION['gcc_admin_content'][$form['#entity_type'] . ':' . $form['#entity_id']]) ? $_SESSION['gcc_admin_content'][$form['#entity_type'] . ':' . $form['#entity_id']] : array();
  $default_values += array(
    'type' => array(),
  );

  $form['#attached']['css'] = array(drupal_get_path('module', 'gcc') . '/gcc-admin.css');

  $form['filters'] = array(

    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#tree' => TRUE,
    '#weight' => 1,
  );

  $form['filters']['type'] = array(

    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Content type'),
    '#options' => isset($type['node']) ? $type['node'] : array(),
    '#default_value' => $default_values['type'],
  );

  $form['filters']['actions'] = array('#type' => 'actions');

  $form['filters']['actions']['submit'] = array(

    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('gcc_admin_content_filter'),
  );

  $form['filters']['actions']['reset'] = array(

    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('gcc_admin_content_reset'),
  );

  $header = array(

    'created' => array('data' => t('Created'), 'field' => 'n.created', 'sort' => 'desc'),
    'name' => array('data' => t('Title'), 'field' => 'n.title'),
    'type' => array('data' => t('Type'), 'field' => 'n.type'),
    'status' => array('data' => t('Status'), 'field' => 'n.status'),
    'author' => array('data' => t('Author'), 'field' => 'u.name'),
  );

  $query = db_select('node', 'n');
  $query = $query->extend('PagerDefault');
  $query->limit(20);
  $query = $query->extend('TableSort');
  $query->orderByHeader($header);
  $query->join('users', 'u', 'u.uid = n.uid');
  $query->fields('n');
  $query->fields('u', array('name'));
  $query->join('field_data_field_gcc_audience', 'f', '%alias.entity_type = :entity_type AND %alias.entity_id = n.nid', array(':entity_type' => 'node'));
  $query->condition('f.field_gcc_audience_entity_type', $entity_type);
  $query->condition('f.field_gcc_audience_entity_id', $entity_id);

  if (!empty($default_values['type'])) {
    $query->condition('n.type', $default_values['type']);
  }

  $result = $query->execute();

  $form['results'] = array(

    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array(),
    '#empty' => t('No members'),
    '#weight' => 4,
  );

  while ($item = $result->fetchAssoc()) {

    $row = array();
    $row[] = format_date($item['created'], 'short');
    $row[] = l($item['title'], 'node/' . $item['nid']);
    $row[] = check_plain(node_type_get_name((object) $item));
    $row[] = $item['status'] ? t('published') : t('not published');
    $row[] = theme('username', array('account' => (object) $item));

    $form['results']['#rows'][] = $row;
  }

  $form['pager'] = array(

    '#markup' => theme('pager'),
    '#weight' => 5,
  );

  return $form;
}

/**
 * Submit handler.
 *
 * Set the filter values.
 */
function gcc_admin_content_filter($form, &$form_state) {

  $_SESSION['gcc_admin_content'][$form['#entity_type'] . ':' . $form['#entity_id']] = $form_state['values']['filters'];
}

/**
 * Submit handler.
 *
 * Reset the filter values.
 */
function gcc_admin_content_reset($form, &$form_state) {

  $_SESSION['gcc_admin_content'][$form['#entity_type'] . ':' . $form['#entity_id']] = array();
}


/* Members Administration */

/**
 * Form builder for the group members administration page.
 */
function gcc_admin_member($form, &$form_state, $entity_type, $entity, $base_path) {

  $form = array();

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#entity'] = $entity;
  $form['#base_path'] = $base_path;

  $default_values = isset($_SESSION['gcc_admin_member'][$form['#entity_type'] . ':' . $form['#entity_id']]) ? $_SESSION['gcc_admin_member'][$form['#entity_type'] . ':' . $form['#entity_id']] : array();
  $default_values += array(
    'state' => array(),
    'roles' => array(),
    'mail' => '',
    'name' => '',
  );

  $form['#attached']['css'] = array(drupal_get_path('module', 'gcc') . '/gcc-admin.css');

  if (!isset($form_state['storage']['op'])) {
    $form_state['storage']['op'] = 'default';
  }

  $function = 'gcc_admin_member_' . $form_state['storage']['op'];

  if (function_exists($function)) {
    $function($form, $form_state, $entity_type, $entity_id, $default_values);
  }

  return $form;
}

/**
 * Build the members administration form.
 */
function gcc_admin_member_default(&$form, &$form_state, $entity_type, $entity_id, $default_values) {

  $form['filters'] = array(

    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#tree' => TRUE,
    '#theme' => 'gcc_admin_member_filters',
    '#weight' => 1,
  );

  $form['filters']['state'] = array(

    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Membership status'),
    '#options' => array(

      GCC_PENDING => t('Pending'),
      GCC_ACTIVE => t('Active'),
      GCC_BLOCKED => t('Blocked'),
    ),
    '#default_value' => $default_values['state'],
  );

  $roles = gcc_role_get_list($entity_type, $entity_id);
  foreach ($roles as &$role) {
    $role = $role->name;
  }
  unset($roles[GCC_NON_MEMBER]);
  unset($roles[GCC_MEMBER]);
  $form['filters']['roles'] = array(

    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Roles'),
    '#options' => $roles,
    '#default_value' => $default_values['roles'],
  );

  $form['filters']['mail'] = array(

    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => $default_values['mail'],
    '#size' => 30,
  );

  $form['filters']['name'] = array(

    '#type' => 'textfield',
    '#title' => t('Login'),
    '#default_value' => $default_values['name'],
    '#size' => 30,
  );

  $form['filters']['actions'] = array('#type' => 'actions');

  $form['filters']['actions']['submit'] = array(

    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('gcc_admin_member_filter'),
  );

  $form['filters']['actions']['reset'] = array(

    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('gcc_admin_member_reset'),
  );

  $header = array(

    'name' => array('data' => t('Login'), 'field' => 'u.name', 'sort' => 'asc'),
    'mail' => array('data' => t('Email'), 'field' => 'u.mail'),
    'role' => array('data' => t('Roles')),
    'date' => array('data' => t('Member since'), 'field' => 'm.date'),
    'state' => array('data' => t('Status'), 'field' => 'm.state'),
  );

  $query = db_select('gcc_membership', 'm');
  $query = $query->extend('PagerDefault');
  $query->limit(20);
  $query = $query->extend('TableSort');
  $query->orderByHeader($header);
  $query->leftJoin('gcc_users_roles', 'ur', 'ur.uid = m.uid AND ur.entity_type = m.entity_type AND ur.entity_id = m.entity_id');
  $query->join('users', 'u', 'u.uid = m.uid');
  $query->fields('m');
  $query->fields('u', array('name', 'mail'));
  $query->condition('m.entity_type', $entity_type);
  $query->condition('m.entity_id', $entity_id);
  $query->groupBy('m.uid');

  if (!empty($default_values['state'])) {
    $query->condition('m.state', $default_values['state']);
  }

  if (!empty($default_values['roles'])) {
    $query->condition('ur.rid', $default_values['roles']);
  }

  if (!empty($default_values['mail'])) {
    $query->condition('u.mail', '%' . db_like($default_values['mail']) . '%', 'LIKE');
  }

  if (!empty($default_values['name'])) {
    $query->condition('u.name', '%' . $default_values['name'] . '%', 'LIKE');
  }

  $total_item = $query->countQuery()->execute()->fetchField();
  $result = $query->execute();

  $options = array();
  while ($item = $result->fetchAssoc()) {

    $state = '';
    switch ($item['state']) {

      case GCC_PENDING:
        $state = t('Pending');
        break;

      case GCC_ACTIVE:
        $state = t('Active');
        break;

      case GCC_BLOCKED:
        $state = t('Blocked');
        break;
    }

    $roles = gcc_membership_get_roles($entity_type, $entity_id, $item['uid']);
    unset($roles[GCC_MEMBER]);

    $options[$item['uid']] = array(

      'name' => theme('username', array('account' => (object) $item)),
      'mail' => $item['mail'],
      'role' => implode(',', $roles),
      'date' => format_interval(time() - $item['date']),
      'state' => $state,
    );
  }

  $form['results'] = array(

    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No members'),
    '#multiple' => TRUE,
    '#weight' => 4,
  );

  $form['pager'] = array(

    '#markup' => theme('pager'),
    '#weight' => 5,
  );

  $form['operations'] = array(

    '#type' => 'fieldset',
    '#title' => t('Operations'),
    '#weight' => 2,
    '#theme' => 'gcc_admin_member_operations',
  );

  $operations = array();
  foreach (gcc_operation_get_list($entity_type, $entity_id) as $key => $item) {
    $operations[$item['group']][$key] = $item['title'];
  }
  $form['operations']['operation'] = array(

    '#type' => 'select',
    '#title' => t('Operation'),
    '#options' => $operations,
    '#weight' => 10,
    '#collapsed' => TRUE,
  );

  $form['operations']['actions'] = array('#type' => 'actions', '#weight' => 11);

  $form['operations']['actions']['selection'] = array(

    '#type' => 'submit',
    '#value' => t('Apply to the selected members'),
    '#submit' => array('gcc_admin_member_operation_selection'),
  );

  $form['operations']['actions']['everyone'] = array(

    '#type' => 'submit',
    '#value' => t('Apply to the @count matched members', array('@count' => $total_item)),
    '#submit' => array('gcc_admin_member_operation_all'),
  );

  $form['operations']['member_name'] = array(

    '#type' => 'textfield',
    '#title' => t('User Login'),
    '#autocomplete_path' => 'user/autocomplete',
    '#weight' => 1,
  );

  $form['operations']['add_member'] = array(

    '#type' => 'submit',
    '#value' => t('Add a new member'),
    '#submit' => array('gcc_admin_member_add_member'),
    '#weight' => 3,
  );
}

/**
 * Build the operation configuration form.
 */
function gcc_admin_member_operation(&$form, &$form_state, $entity_type, $entity_id, $default_values) {

  $operations = gcc_operation_get_list($form['#entity_type'], $form['#entity_id']);

  $form['count'] = array(

    '#markup' => '<p>' . t('Number of selected members : @count', array('@count' => count($form_state['storage']['list']))) . '</p>',
  );

  $form['operation'] = array(

    '#type' => 'fieldset',
    '#title' => $operations[$form_state['storage']['values']['operation']]['title'],
    '#tree' => TRUE,
  );

  if ($operations[$form_state['storage']['values']['operation']]['file']) {
    require_once DRUPAL_ROOT . '/' . $operations[$form_state['storage']['values']['operation']]['file'];
  }

  $form['operation'] += $operations[$form_state['storage']['values']['operation']]['form callback']($form['#entity_type'], $form['#entity_id']);

  $form = confirm_form($form, t('Configure the operation'), $form['#base_path'] . '/gcc/members', NULL, t('Launch'));

  $form['actions']['submit']['#submit'][] = 'gcc_admin_member_operation_submit';
}

/**
 * TODO.
 */
function theme_gcc_admin_member_filters($variables) {

  $form = $variables['form'];

  $rows = array(
    array(

      drupal_render($form['state']),
      drupal_render($form['roles']),
      drupal_render($form['mail']),
      drupal_render($form['name']),
    ),
  );

  return theme('table', array('header' => array(), 'rows' => $rows)) . drupal_render($form['actions']);
}

/**
 * TODO.
 */
function theme_gcc_admin_member_operations($variables) {

  $form = $variables['form'];

  $rows = array(
    array(

      'data' => array(
        drupal_render($form['operation']),
        drupal_render($form['member_name']),
      ),
      'no_striping' => TRUE,
    ),
    array(

      'data' => array(
        drupal_render($form['actions']),
        drupal_render($form['add_member']),
      ),
      'no_striping' => TRUE,
    ),
  );

  return theme('table', array('header' => array(), 'rows' => $rows)) . drupal_render($form['actions']);
}

/**
 * Submit handler.
 *
 * Set the filter values.
 */
function gcc_admin_member_filter($form, &$form_state) {

  $_SESSION['gcc_admin_member'][$form['#entity_type'] . ':' . $form['#entity_id']] = $form_state['values']['filters'];
}

/**
 * Submit handler.
 *
 * Reset the filter values.
 */
function gcc_admin_member_reset($form, &$form_state) {

  $_SESSION['gcc_admin_member'][$form['#entity_type'] . ':' . $form['#entity_id']] = array();
}

/**
 * Submit handler.
 *
 * Apply the selected operation to the selected members
 */
function gcc_admin_member_operation_selection($form, &$form_state) {

  $list = array_keys(array_filter($form_state['values']['results']));

  if (empty($list)) {

    drupal_set_message(t('You must select at least one member before applying an operation.'), 'error');
    return;
  }

  $operations = gcc_operation_get_list($form['#entity_type'], $form['#entity_id']);

  if (isset($operations[$form_state['values']['operation']]['form callback'])) {

    $form_state['storage']['values'] = $form_state['values'];
    $form_state['storage']['list'] = $list;
    $form_state['storage']['op'] = 'operation';
    $form_state['rebuild'] = TRUE;
  }
  else {
    gcc_operation_mass_execute($form['#entity_type'], $form['#entity_id'], $list, $form_state['values']['operation']);
  }
}

/**
 * Submit handler.
 *
 * Apply the selected operation to all the matched members
 */
function gcc_admin_member_operation_all($form, &$form_state) {

  $default_values = isset($_SESSION['gcc_admin_member'][$form['#entity_type'] . ':' . $form['#entity_id']]) ? $_SESSION['gcc_admin_member'][$form['#entity_type'] . ':' . $form['#entity_id']] : array();
  $default_values += array(

    'state' => array(),
    'roles' => array(),
    'mail' => '',
    'name' => '',
  );

  $query = db_select('gcc_membership', 'm');
  $query = $query->extend('PagerDefault');
  $query->limit(20);
  $query->leftJoin('gcc_users_roles', 'ur', 'ur.uid = m.uid AND ur.entity_type = m.entity_type AND ur.entity_id = m.entity_id');
  $query->join('users', 'u', 'u.uid = m.uid');
  $query->fields('m');
  $query->fields('u', array('name', 'mail'));
  $query->condition('m.entity_type', $form['#entity_type']);
  $query->condition('m.entity_id', $form['#entity_id']);
  $query->groupBy('m.uid');

  if (!empty($default_values['state'])) {
    $query->condition('m.state', $default_values['state']);
  }

  if (!empty($default_values['roles'])) {
    $query->condition('ur.rid', $default_values['roles']);
  }

  if (!empty($default_values['mail'])) {
    $query->condition('u.mail', '%' . db_like($default_values['mail']) . '%', 'LIKE');
  }

  if (!empty($default_values['name'])) {
    $query->condition('u.name', '%' . $default_values['name'] . '%', 'LIKE');
  }

  $list = array();
  $result = $query->execute();
  while ($item = $result->fetchAssoc()) {
    $list[] = $item['uid'];
  }

  if (empty($list)) {

    drupal_set_message(t('You must select at least one member before applying an operation.'), 'error');
    return;
  }

  $operations = gcc_operation_get_list($form['#entity_type'], $form['#entity_id']);

  if (isset($operations[$form_state['values']['operation']]['form callback'])) {

    $form_state['storage']['values'] = $form_state['values'];
    $form_state['storage']['list'] = $list;
    $form_state['storage']['op'] = 'operation';
    $form_state['rebuild'] = TRUE;
  }
  else {
    gcc_operation_mass_execute($form['#entity_type'], $form['#entity_id'], $list, $form_state['values']['operation']);
  }
}

/**
 * Submit handler.
 *
 * Add a new member.
 */
function gcc_admin_member_add_member($form, &$form_state) {

  $user = FALSE;
  if (!empty($form_state['values']['member_name'])) {
    $user = user_load_by_name($form_state['values']['member_name']);
  }

  if (!$user) {

    drupal_set_message(t('Invalide username'), 'error');
    return;
  }

  if (gcc_membership_load($form['#entity_type'], $form['#entity_id'], $user->uid)) {

    drupal_set_message(t('User %username is already a member of this group.', array('%username' => $user->name)), 'error');
    return;
  }

  drupal_goto($form['#base_path'] . '/gcc/subscribe/' . $user->uid, array(

    'query' => array(
      'destination' => $form['#base_path'] . '/gcc/members',
    ),
  ));
}

/**
 * Submit handler.
 *
 * Apply the selected operation to the selected members.
 */
function gcc_admin_member_operation_submit($form, &$form_state) {

  $list = $form_state['storage']['list'];
  $operation = $form_state['storage']['values']['operation'];
  $config = array();

  if (isset($form_state['values']['operation'])) {
    $config = $form_state['values']['operation'];
  }

  gcc_operation_mass_execute($form['#entity_type'], $form['#entity_id'], $list, $operation, $config);
}

/* Subscription */

/**
 * Form Buidler.
 */
function gcc_admin_subscribe($form, &$form_state, $account, $entity_type, $entity, $base_path) {

  global $user;

  $label = entity_label($entity_type, $entity);
  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form = array();
  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#entity'] = $entity;
  $form['#base_path'] = $base_path;
  $form['#user'] = $account;

  if ($user->uid == $account->uid) {
    $text = t('<p>Your are about to subscribe to %label', array('%label' => $label));
  }
  else {
    $text = t('<p>Your are about to subscribe the user %username to %label', array('%username' => $account->name, '%label' => $label));
  }

  $form['basic'] = array(

    '#type' => 'fieldset',
    '#title' => t('Informations'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );

  $form['basic']['information'] = array('#markup' => $text);

  if (gcc_user_access('administer group members', $entity_type, $entity)) {
    $form['basic']['member_state'] = array(

      '#type' => 'select',
      '#title' => t('Membership State'),
      '#options' => array(

        GCC_ACTIVE => t('Active'),
        GCC_PENDING => t('Pending'),
      ),
    );
  }
  else {

    $form['basic']['member_state'] = array(

      '#type' => 'value',
      '#value' => GCC_PENDING,
    );

    if (gcc_user_access('skip approval', $entity_type, $entity)) {
      $form['basic']['member_state']['#value'] = GCC_ACTIVE;
    }
  }

  foreach (module_implements('gcc_membership_add_form') as $module) {

    $function = $module . '_gcc_membership_add_form';
    $function($account, $entity_type, $entity, $form, $form_state);
  }

  return confirm_form(
    $form,
    t('Subscription to %label', array('%label' => $label)),
    $base_path,
    '',
    t('Subscribe')
  );
}

/**
 * Form validation handler.
 */
function gcc_admin_subscribe_validate($form, &$form_state) {

  $entity_type = $form['#entity_type'];
  $entity = $form['#entity'];
  $account = $form['#user'];

  foreach (module_implements('gcc_membership_add_form_validate') as $module) {

    $function = $module . '_gcc_membership_add_form_validate';
    $function($account, $entity_type, $entity, $form, $form_state);
  }
}

/**
 * Form submit handler.
 *
 * Save a new membership.
 */
function gcc_admin_subscribe_submit($form, &$form_state) {

  global $user;

  $entity_type = $form['#entity_type'];
  $entity = $form['#entity'];
  $entity_id = $form['#entity_id'];
  $account = $form['#user'];
  $label = entity_label($entity_type, $entity);

  $membership = gcc_membership_create($entity_type, $entity_id, $account->uid, $form_state['values']['basic']['member_state']);

  if ($membership) {

    foreach (module_implements('gcc_membership_add_form_submit') as $module) {

      $function = $module . '_gcc_membership_add_form_submit';
      $function($account, $entity_type, $entity, $form, $form_state, $membership);
    }

    if ($account->uid != $user->uid) {
      drupal_set_message(t('@user correctly added to the group @group.', array('@user' => $account->name, '@group' => $label)));
    }
    else {

      if ($form_state['values']['basic']['member_state'] == GCC_PENDING) {
        drupal_set_message(t('Your subscription has been correctly saved. It will be actived soon by an administrator.'));
      }
      else {
        drupal_set_message(t('Your subscription has been correctly saved.'));
      }
    }
  }

  $form_state['redirect'] = $form['#base_path'];
}

/* Unsubscription */

/**
 * Form Builder.
 */
function gcc_admin_unsubscribe($form, &$form_state, $account, $entity_type, $entity, $base_path) {

  global $user;

  $label = entity_label($entity_type, $entity);
  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form = array();
  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#entity'] = $entity;
  $form['#base_path'] = $base_path;
  $form['#user'] = $account;

  if ($user->uid == $account->uid) {
    $text = t('<p>Your are about to unsubscribe from %label', array('%label' => $label));
  }
  else {
    $text = t('<p>Your are about to unsubscribe the user %username from %label', array('%username' => $account->name, '%label' => $label));
  }

  return confirm_form(
    $form,
    t('Unsubscription from %label', array('%label' => $label)),
    $base_path,
    '',
    t('Unsubscribe')
  );
}

/**
 * Submit handler.
 */
function gcc_admin_unsubscribe_submit($form, &$form_state) {

  global $user;

  $entity_type = $form['#entity_type'];
  $entity = $form['#entity'];
  $entity_id = $form['#entity_id'];
  $account = $form['#user'];
  $label = entity_label($entity_type, $entity);

  gcc_membership_delete($entity_type, $entity_id, $account->uid);

  if ($account->uid != $user->uid) {
    drupal_set_message(t('@user correctly unsubscribed from the group @group.', array('@user' => $account->name, '@group' => $label)));
  }
  else {
    drupal_set_message(t('Your subscription has been correctly removed.'));
  }

  $form_state['redirect'] = $form['#base_path'];
}
