<?php

/**
 * @file
 * This file contains all the functions for manipulating GCC.
 */

/* ################# */
/* # GCC Functions # */
/* ################# */

/* Utility Functions */

/**
 * Determine if an entity is a group.
 */
function gcc_is_group($entity) {

  return isset($entity->{GCC_FIELD_ENABLE}[LANGUAGE_NONE][0]['value']) && $entity->{GCC_FIELD_ENABLE}[LANGUAGE_NONE][0]['value'];
}

/**
 * Determine if an entity is a group content.
 */
function gcc_is_group_content($entity) {

  if (isset($entity->{GCC_FIELD_AUDIENCE}[LANGUAGE_NONE][0]['entity_type'], $entity->{GCC_FIELD_AUDIENCE}[LANGUAGE_NONE][0]['entity_id'])) {
    return $entity->{GCC_FIELD_AUDIENCE}[LANGUAGE_NONE][0]['entity_id'] != 0;
  }

  return FALSE;
}

/**
 * Get the entity id from an entity.
 */
function gcc_get_entity_id($entity_type, $entity) {

  if ($entity_type == GCC_GLOBAL_TYPE) {
    return GCC_GLOBAL_ID;
  }

  list($id) = entity_extract_ids($entity_type, $entity);

  return $id;
}

/**
 * Translate a path to replace a place holder with an id.
 */
function gcc_translate_path($entity_type, $entity_id, $base_path) {

  if ($entity_type == GCC_GLOBAL_TYPE) {
    return $base_path;
  }

  $info = entity_get_info();
  $map = explode('/', $base_path);
  $map[$info[$entity_type]['gcc']['entity position']] = $entity_id;

  return implode('/', $map);
}

/**
 * Determine if the current group is the global one.
 */
function gcc_is_global_group($entity_type, $entity_id) {

  return $entity_type == GCC_GLOBAL_TYPE && $entity_id == GCC_GLOBAL_ID;
}

/* Group Functions */

/**
 * Return a list of all the groups.
 */
function gcc_group_get_list() {

  $query = new EntityFieldQuery();
  $query->fieldCondition(GCC_FIELD_ENABLE, 'value', 1);
  $query->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');
  $result = $query->execute();

  $list = array();
  foreach ($result as $entity_type => $entities) {
    foreach ($entities as $entity_id => $entity) {

      $list[] = (object) array(

        'entity_type' => $entity_type,
        'entity_id' => $entity_id,
      );
    }
  }

  return $list;
}

/**
 * Return the group owner of the given group.
 */
function gcc_group_get_owner($entity) {

  if (!gcc_is_group($entity)) {
    return FALSE;
  }

  return $entity->{GCC_FIELD_ENABLE}[LANGUAGE_NONE][0]['uid'];
}

/**
 * Return the list of member of the group $entity_type/$entity_id.
 *
 * Optionnaly filtered by $states.
 */
function gcc_group_get_members($entity_type, $entity_id, $states = array(GCC_ACTIVE)) {

  $list = &drupal_static(__FUNCTION__, array());

  asort($states);
  $cid = $entity_type . ':' . $entity_id . ':' . implode(':', $states);

  if (!isset($list[$cid])) {

    $list[$cid] = array();

    // Select the membership and filter by states (really easy).
    $query = db_select('gcc_membership', 'm');
    $query->fields('m', array('uid'));
    $query->condition('m.entity_type', $entity_type);
    $query->condition('m.entity_id', $entity_id);
    $query->condition('m.state', $states);

    $result = $query->execute();
    $result->setFetchMode(PDO::FETCH_OBJ);
    foreach ($result as $item) {
      $list[$cid][$item->uid] = $item;
    }
  }

  return $list[$cid];
}

/**
 * Return the list of member of the group $entity_type/$entity_id.
 *
 * Filtered by $permission.
 */
function gcc_group_get_members_by_permission($entity_type, $entity_id, $permission, $include_owner = TRUE, $include_global_permission = FALSE) {

  $list = &drupal_static(__FUNCTION__, array());

  $cid = $entity_type . ':' . $entity_id . ':' . $permission . ':' . $include_owner . ':' . $include_global_permission;

  if (!isset($list[$cid])) {

    $list[$cid] = array();

    // Add all the users with the right global permission as members.
    if ($include_global_permission) {

      // Get all the user with the corresponding global permission.
      $permissions = gcc_permission_get_list();
      if ($permissions[$permission]['global']) {

        $perms = array('0');
        foreach ($permissions[$permission]['global'] as $perm) {
          $perms[] = $perm;
        }

        $query = db_select('users', 'u');
        $query->fields('u', arrau('uid'));
        $query->join('users_roles', 'r', '%alias.uid = u.uid');
        $query->join('role_permission', 'p', '%alias.rid = r.rid');
        $query->condition('p.permission', $perms);

        $result = $query->execute();
        $result->setFetchMode(PDO::FETCH_OBJ);
        foreach ($result as $item) {
          $list[$cid][$item->uid] = $item;
        }
      }
    }

    // Add the owner to the member list.
    if ($include_owner) {

      $entity = entity_load($entity_type, array($entity_id));
      $entity = reset($entity);
      if ($entity) {

        $item = (object) array('uid' => gcc_group_get_owner($entity));
        $list[$cid][$item->uid] = $item;
      }
    }

    // Next get all the the members with $permission.
    $query = db_select('gcc_membership', 'm');
    $query->fields('m', array('uid'));
    $query->condition('m.entity_type', $entity_type);
    $query->condition('m.entity_id', $entity_id);
    $query->condition('m.state', GCC_ACTIVE);

    // Retrieve all the roles given to the members.
    $role_query = db_select('gcc_users_roles', 'user_role');
    $role_query->distinct();
    $role_query->addField('user_role', 'uid');
    $role_query->addField('user_role', 'entity_type');
    $role_query->addField('user_role', 'entity_id');
    $role_query->addField('user_role', 'rid');
    $role_query->condition('user_role.entity_type', $entity_type);
    $role_query->condition('user_role.entity_id', $entity_id);

    // Retrieve all the members of the group.
    // and give them the GCC_MEMBER role.
    $member_query = db_select('gcc_membership');
    $member_query->addField('gcc_membership', 'uid');
    $member_query->addField('gcc_membership', 'entity_type');
    $member_query->addField('gcc_membership', 'entity_id');
    $member_query->addExpression("'" . GCC_MEMBER . "'", 'rid');
    $member_query->condition('gcc_membership.entity_type', $entity_type);
    $member_query->condition('gcc_membership.entity_id', $entity_id);

    // Join the roles corresponding to the groups list.
    $query->join($role_query->union($member_query), 'user_role', '%alias.uid = m.uid AND %alias.entity_type = m.entity_type AND %alias.entity_id = m.entity_id');

    // Join the permissions corresponding to roles list.
    $query->join('gcc_role_permission', 'role_perm', '%alias.rid = user_role.rid AND ((%alias.entity_type = m.entity_type AND %alias.entity_id = m.entity_id) OR (%alias.entity_type = :type AND %alias.entity_id = :id))', array(':type' => GCC_GLOBAL_TYPE, ':id' => GCC_GLOBAL_ID));

    // Magic thing to filter out overriden permissions.
    $query->addJoin('LEFT', 'gcc_role_permission', 'role_perm_double', '%alias.rid = role_perm.rid AND role_perm.entity_type = :type AND %alias.entity_type != :type AND %alias.entity_type = m.entity_type AND %alias.entity_id = m.entity_id', array(':type' => GCC_GLOBAL_TYPE));
    $query->condition('role_perm_double.rid', NULL);

    // Finally filter the result set by the given permission.
    $query->condition('role_perm.permission', $permission);

    $result = $query->execute();
    $result->setFetchMode(PDO::FETCH_OBJ);
    foreach ($result as $item) {
      $list[$cid][$item->uid] = $item;
    }
  }

  return $list[$cid];
}

/* User Access Functions */

/**
 * Check if a user has a gcc related permission.
 *
 * Pending membership are treated like non members.
 * Blocked membership are treated like nothing (not members nor non members)
 */
function gcc_user_access($permission, $entity_type, $entity, $account = NULL) {

  global $user;

  if (!isset($account)) {
    $account = $user;
  }

  // If not a group, always return FALSE.
  if (!gcc_is_group($entity) && $entity_type != GCC_GLOBAL_TYPE) {
    return FALSE;
  }

  // The group owner always has every permissions.
  if ($account->uid == gcc_group_get_owner($entity)) {
    return TRUE;
  }

  // If the user has the corresponding global gcc permission.
  $permissions = gcc_permission_get_list();
  if ($permissions[$permission]['global']) {
    foreach ($permissions[$permission]['global'] as $perm) {
      if (user_access($perm, $account)) {
        return TRUE;
      }
    }
  }

  // To reduce the number of SQL queries, we cache the user's permissions
  // in a static variable.
  // Use the advanced drupal_static() pattern, since this is called very often.
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast['perm'] = &drupal_static(__FUNCTION__);
  }
  $perm = &$drupal_static_fast['perm'];
  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $cid = $account->uid . ':' . $entity_type . ':' . $entity_id;

  if (!isset($perm[$cid])) {

    $perm[$cid] = array();
    $membership = gcc_membership_load($entity_type, $entity_id, $account->uid);

    // Take care only of non member or non blocked member.
    if (($membership && $membership->state != GCC_BLOCKED) || $membership === FALSE) {

      $roles = gcc_membership_get_roles($entity_type, $entity_id, $account->uid);

      foreach (array_keys($roles) as $rid) {
        $perm[$cid] += array_filter(gcc_permission_get_list_by_role($rid, $entity_type, $entity_id));
      }
    }
  }

  return isset($perm[$cid][$permission]) && $perm[$cid][$permission];
}

/* Entity Functions */

/**
 * Get the list of entity type usable with GCC.
 *
 * DEPRECATED.
 */
function gcc_entity_get_info() {

  throw new Exception('The use of this function is now depracated. Please use the entity_get_info() function instead.');

  $info = &drupal_static(__FUNCTION__, NULL);

  if (!isset($info)) {

    $info = array();

    foreach (module_implements('gcc_entity_info') as $module) {

      $list = module_invoke($module, 'gcc_entity_info');

      foreach ($list as &$config) {
        $config += array(

          'module' => $module,
          'group' => FALSE,
          'group content' => FALSE,
          'display callback' => FALSE,
        );
      }

      $info += $list;
    }
  }

  return $info;
}

/* Features Functions */

/**
 * Retrive the list of GCC features.
 */
function gcc_features_get_info() {

  $info = &drupal_static(__FUNCTION__, NULL);

  if (!isset($info)) {

    $info = array();

    foreach (module_implements('gcc_features_info') as $module) {

      $list = module_invoke($module, 'gcc_features_info');

      foreach ($list as &$config) {
        $config += array(

          'module' => $module,
          'label' => '',
          'explaination' => '',
        );
      }

      $info += $list;
    }
  }

  return $info;
}

/**
 * Retrive the list of available features and their status.
 *
 * The list if for a given entity_type/bundle.
 */
function gcc_features_get_bundle_info($entity_type, $bundle) {

  $features = &drupal_static(__FUNCTION__, array());
  $cid = $entity_type . ':' . $bundle;

  if (!isset($features[$cid])) {

    $features[$cid] = module_invoke_all('gcc_entity_features', $entity_type, $bundle);

    $features_info = gcc_features_get_info();
    foreach ($features[$cid] as $key => $info) {

      if (!isset($features_info[$key])) {
        unset($features[$cid][$key]);
      }
      else {
        $features[$cid][$key]['enabled'] = module_invoke($features_info[$key]['module'], 'gcc_features_is_enabled', $key, $entity_type, $bundle);
      }
    }
  }

  return $features[$cid];
}

/**
 * Return a list of bundles with a specific features enabled.
 *
 * @param string $feature
 *   The machine name of a gcc feature (example : group).
 *
 * @return array
 *   Return the list of bundles for which $feature is enabled.
 *   The list is an array of bundles arrays keyed by entity type.
 *   Each bundles arrays is keyed by bundle machine name and
 *   contain the bundles labels.
 */
function gcc_features_get_enabled_bundles($feature) {

  $bundles = &drupal_static(__FUNCTION__, array());

  if (!isset($bundles[$feature])) {

    $bundles[$feature] = array();

    $entity_info = entity_get_info();
    foreach ($entity_info as $entity_type => $info) {

      // Skip non GCC Entity.
      if (!isset($info['gcc'])) {
        continue;
      }

      foreach ($info['bundles'] as $bundle => $bundle_info) {

        $features = gcc_features_get_bundle_info($entity_type, $bundle);
        if (isset($features[$feature]['enabled']) && $features[$feature]['enabled']) {
          $bundles[$feature][$entity_type][$bundle] = $bundle_info['label'];
        }
      }
    }
  }

  return $bundles[$feature];
}

/* Roles Functions */

/**
 * Add a new gcc role.
 */
function gcc_role_add($rid, $name, $entity_type, $bundle, $entity_id) {

  $role = new stdClass();
  $role->rid = $rid;
  $role->name = $name;
  $role->entity_type = $entity_type;
  $role->entity_id = $entity_id;
  $role->bundle = $bundle;

  drupal_write_record('gcc_role', $role);

  module_invoke_all('gcc_role_add', $entity_type, $entity_id, $role);

  return $role;
}

/**
 * Save a gcc role.
 */
function gcc_role_save($role) {

  drupal_write_record('gcc_role', $role, array('rid'));

  module_invoke_all('gcc_role_save', $role);

  return $role;
}

/**
 * Delete a gcc role.
 */
function gcc_role_delete($rid) {

  db_delete('gcc_role')->condition('rid', $rid)->execute();
  db_delete('gcc_role_permission')->condition('rid', $rid)->execute();
  db_delete('gcc_users_roles')->condition('rid', $rid)->execute();

  module_invoke_all('gcc_role_delete', $rid);
}

/**
 * Load a group role.
 */
function gcc_role_load($rid) {

  $role = db_select('gcc_role', 'r')->fields('r')->condition('rid', $rid)->execute()->fetch(PDO::FETCH_OBJ);

  return $role;
}

/**
 * Load a group role.
 */
function gcc_role_load_by_name($name, $entity_type = NULL, $entity_id = NULL) {

  $query = db_select('gcc_role', 'r')->fields('r')->condition('name', $name);

  if ($entity_type) {
    $query->condition('entity_type', $entity_type);
  }

  if ($entity_id) {
    $query->condition('entity_id', $entity_id);
  }

  $role = $query->execute()->fetch(PDO::FETCH_OBJ);

  return $role;
}

/**
 * Return the list of roles for the given entity_type/entity_id.
 *
 * This list include the default GCC_NON_MEMBER, GCC_MEMBER, ... entries.
 */
function gcc_role_get_list($entity_type, $entity_id) {

  $list = &drupal_static(__FUNCTION__, array());
  $cid = $entity_type . '_' . $entity_id;

  if (!isset($list[$cid])) {

    $list[$cid] = array(

      GCC_NON_MEMBER => (object) array(

        'rid' => GCC_NON_MEMBER,
        'entity_type' => GCC_GLOBAL_TYPE,
        'entity_id' => GCC_GLOBAL_ID,
        'bundle' => GCC_GLOBAL_BUNDLE,
        'name' => t('Non Member'),
      ),
      GCC_MEMBER => (object) array(

        'rid' => GCC_MEMBER,
        'entity_type' => GCC_GLOBAL_TYPE,
        'entity_id' => GCC_GLOBAL_ID,
        'bundle' => GCC_GLOBAL_BUNDLE,
        'name' => t('Member'),
      ),
      GCC_ADMIN => (object) array(

        'rid' => GCC_ADMIN,
        'entity_type' => GCC_GLOBAL_TYPE,
        'entity_id' => GCC_GLOBAL_ID,
        'bundle' => GCC_GLOBAL_BUNDLE,
        'name' => t('Administrator'),
      ),
    );

    $bundle = GCC_GLOBAL_BUNDLE;
    if ($entity_type != GCC_GLOBAL_TYPE) {

      $entity = entity_load($entity_type, array($entity_id));
      if (isset($entity[$entity_id])) {
        list(,,$bundle) = entity_extract_ids($entity_type, $entity[$entity_id]);
      }
    }

    $query = db_select('gcc_role', 'r');
    $query->fields('r');

    if ($entity_type != GCC_GLOBAL_TYPE) {

      $query->condition(db_or()
        ->condition(db_and()
          ->condition('entity_type', GCC_GLOBAL_TYPE)
          ->condition('bundle', GCC_GLOBAL_BUNDLE)
          ->condition('entity_id', GCC_GLOBAL_ID)
        )
        ->condition(db_and()
          ->condition('entity_type', $entity_type)
          ->condition('bundle', $bundle)
          ->condition('entity_id', 0)
        )
        ->condition(db_and()
          ->condition('entity_type', $entity_type)
          ->condition('bundle', $bundle)
          ->condition('entity_id', $entity_id)
        )
      );
    }
    else {

      $query->condition(db_or()
        ->condition(db_and()
          ->condition('entity_type', GCC_GLOBAL_TYPE)
          ->condition('bundle', GCC_GLOBAL_BUNDLE)
          ->condition('entity_id', GCC_GLOBAL_ID)
        )
        ->condition(db_and()
          ->condition(db_or()
            ->condition('entity_type', GCC_GLOBAL_TYPE, '!=')
            ->condition('bundle', GCC_GLOBAL_BUNDLE, '!=')
          )
          ->condition('entity_id', 0)
        )
      );
    }
    $query->orderBy('entity_id');

    $result = $query->execute();
    $result->setFetchMode(PDO::FETCH_OBJ);
    foreach ($result as $role) {
      $list[$cid][$role->rid] = $role;
    }
  }

  return $list[$cid];
}

/**
 * Tell if the permissions are overriden for a given role.
 */
function gcc_role_is_overriden($rid, $entity_type, $entity_id) {

  // We cannot override roles in the global configuration.
  if (gcc_is_global_group($entity_type, $entity_id)) {
    return FALSE;
  }

  $query = db_select('gcc_role_permission');
  $query->addExpression('1');
  $query->condition('rid', $rid);
  $query->condition('entity_type', $entity_type);
  $query->condition('entity_id', $entity_id);

  return $query->execute()->fetchField();
}

/**
 * Check of the role is a default one.
 */
function gcc_role_is_default($rid) {

  return in_array($rid, array(GCC_NON_MEMBER, GCC_MEMBER, GCC_ADMIN));
}

/**
 * Determine if a role is a global one.
 */
function gcc_role_is_global($role) {

  if ($role->entity_id == 0) {
    return TRUE;
  }

  return FALSE;
}

/* Permissions Functions */

/**
 * Retrive a list of all GCC Permissions.
 */
function gcc_permission_get_list() {

  $list = &drupal_static(__FUNCTION__, NULL);

  if (!isset($list)) {

    $list = module_invoke_all('gcc_permission');
    foreach ($list as &$item) {
      $item += array(

        'group' => t('Default'),
        'global' => FALSE,
      );
    }
  }

  return $list;
}

/**
 * Return the list of permission for the given role/entity_type/entity_id.
 *
 * If the permission are overriden, return the overriden values.
 * If not, return the default values.
 */
function gcc_permission_get_list_by_role($rid, $entity_type, $entity_id) {

  $permissions = &drupal_static(__FUNCTION__, array());
  $cid = $rid . ':' . $entity_type . ':' . $entity_id;

  if (!isset($permissions[$cid])) {

    $permissions[$cid] = array();
    $roles = gcc_role_get_list($entity_type, $entity_id);

    // If the role doesn't belongs to the group and is not overriden.
    // Load the default config.
    if ($roles[$rid]->entity_type != $entity_type || $roles[$rid]->entity_id != $entity_id) {
      if (!gcc_role_is_overriden($rid, $entity_type, $entity_id)) {

        $entity_type = GCC_GLOBAL_TYPE;
        $entity_id = GCC_GLOBAL_ID;
      }
    }

    // Load the permissions.
    $query = db_select('gcc_role_permission', 'p')->fields('p', array('permission'));
    $query->condition('rid', $rid);
    $query->condition('entity_type', $entity_type);
    $query->condition('entity_id', $entity_id);
    $list = $query->execute();

    $permissions[$cid] = array();
    while ($key = $list->fetchField()) {
      $permissions[$cid][$key] = TRUE;
    }

    foreach (array_keys(gcc_permission_get_list()) as $key) {
      if (!isset($permissions[$cid][$key])) {
        $permissions[$cid][$key] = FALSE;
      }
    }
  }

  return $permissions[$cid];
}

/* Membership Functions */

/**
 * Create a membership.
 */
function gcc_membership_create($entity_type, $entity_id, $uid, $state) {

  $record = new stdClass();
  $record->entity_type = $entity_type;
  $record->entity_id = $entity_id;
  $record->uid = $uid;
  $record->state = $state;
  $record->date = time();

  $result = drupal_write_record('gcc_membership', $record);

  module_invoke_all('gcc_memberhsip_create', $record);

  return $result == SAVED_NEW;
}

/**
 * Load a membership.
 */
function gcc_membership_load($entity_type, $entity_id, $uid) {

  $membership = db_select('gcc_membership', 'm')
  ->fields('m')
  ->condition('entity_type', $entity_type)
  ->condition('entity_id', $entity_id)
  ->condition('uid', $uid)
  ->execute()
  ->fetchObject();

  return $membership;
}

/**
 * Activate a membership.
 */
function gcc_membership_activate($entity_type, $entity_id, $uid) {

  db_update('gcc_membership')
  ->fields(array('state' => GCC_ACTIVE))
  ->condition('entity_type', $entity_type)
  ->condition('entity_id', $entity_id)
  ->condition('uid', $uid)
  ->execute();

  module_invoke_all('gcc_membership_activate', $entity_type, $entity_id, $uid);
}

/**
 * Block a membership.
 */
function gcc_membership_block($entity_type, $entity_id, $uid) {

  db_update('gcc_membership')
  ->fields(array('state' => GCC_BLOCKED))
  ->condition('entity_type', $entity_type)
  ->condition('entity_id', $entity_id)
  ->condition('uid', $uid)
  ->execute();

  module_invoke_all('gcc_membership_block', $entity_type, $entity_id, $uid);
}

/**
 * Delete a membership.
 */
function gcc_membership_delete($entity_type, $entity_id, $uid) {

  db_delete('gcc_membership')
  ->condition('entity_type', $entity_type)
  ->condition('entity_id', $entity_id)
  ->condition('uid', $uid)
  ->execute();

  db_delete('gcc_users_roles')
  ->condition('entity_type', $entity_type)
  ->condition('entity_id', $entity_id)
  ->condition('uid', $uid)
  ->execute();

  module_invoke_all('gcc_membership_delete', $entity_type, $entity_id, $uid);
}

/**
 * Add a role to a member.
 */
function gcc_membership_add_role($entity_type, $entity_id, $uid, $rid) {

  db_merge('gcc_users_roles')->key(array(

    'uid' => $uid,
    'rid' => $rid,
    'entity_type' => $entity_type,
    'entity_id' => $entity_id,
  ))->execute();

  module_invoke_all('gcc_membership_add_role', $entity_type, $entity_id, $uid, $rid);
}

/**
 * Remove a role from a member.
 */
function gcc_membership_remove_role($entity_type, $entity_id, $uid, $rid) {

  db_delete('gcc_users_roles')
  ->condition('entity_type', $entity_type)
  ->condition('entity_id', $entity_id)
  ->condition('uid', $uid)
  ->condition('rid', $rid)
  ->execute();

  module_invoke_all('gcc_membership_remove_role', $entity_type, $entity_id, $uid, $rid);
}

/**
 * Get the roles associated with a membership.
 */
function gcc_membership_get_roles($entity_type, $entity_id, $uid) {

  $roles = &drupal_static(__FUNCTION__, array());
  $cid = $entity_type . ':' . $entity_id . ':' . $uid;

  if (!isset($roles[$cid])) {

    $roles[$cid] = array();
    $membership = gcc_membership_load($entity_type, $entity_id, $uid);

    // Active members.
    if ($membership && $membership->state == GCC_ACTIVE) {

      $roles[$cid][GCC_MEMBER] = t('Member');

      $roles_list = gcc_role_get_list($entity_type, $entity_id);

      $query = db_select('gcc_users_roles', 'ur');
      $query->fields('ur', array('rid'));
      $query->condition('entity_type', $entity_type);
      $query->condition('entity_id', $entity_id);
      $query->condition('uid', $uid);
      $result = $query->execute();
      while ($rid = $result->fetchField()) {
        if (isset($roles_list[$rid])) {
          $roles[$cid][$rid] = $roles_list[$rid]->name;
        }
      }
    }
    // Non members (Pending are treated like non members).
    elseif ($membership === FALSE || $membership->state == GCC_PENDING) {
      $roles[$cid][GCC_NON_MEMBER] = t('Non Member');
    }

    // Blocked are treated like nothing.
  }

  return $roles[$cid];
}

/**
 * Retrieve all the membership of a user filtered by state.
 */
function gcc_membership_get_user_memberships($uid, $states = array(GCC_ACTIVE)) {

  $list = &drupal_static(__FUNCTION__, array());

  asort($states);
  $cid = $uid . ':' . implode(':', $states);

  if (!isset($list[$cid])) {

    $list[$cid] = array();

    // Select the membership and filter by states.
    $query = db_select('gcc_membership', 'm');
    $query->fields('m', array('entity_type', 'entity_id'));
    $query->condition('m.uid', $uid);
    $query->condition('m.state', $states);

    $result = $query->execute();
    $result->setFetchMode(PDO::FETCH_OBJ);
    foreach ($result as $item) {
      $list[$cid][$item->entity_type . ':' . $item->entity_id] = $item;
    }
  }

  return $list[$cid];
}

/**
 * Retrieve a list of group where $uid has $permission.
 *
 * Pending membership are treated like non members.
 * Blocked membership are treated like nothing (not members nor non members).
 *
 * @param int $uid
 *   The $uid for which you want the groups.
 *
 * @param string $permission
 *   The permission $uid must have.
 *
 * @param bool $include_owner
 *   If set to TRUE, the result will include all the groups where $uid
 *   is the owner.
 *
 * @param bool $include_non_member
 *   If set to TRUE, the result will include all the groups where $uid is not
 *   a member and NON_MEMBER have the $permission.
 *
 * @param bool $include_global_permission
 *   If set to TRUE and $uid as a global permission corresponding
 *   to $permission, then $uid will have access to all the group.
 */
function gcc_membership_get_by_permission($uid, $permission, $include_owner = TRUE, $include_non_member = TRUE, $include_global_permission = FALSE) {

  $list = &drupal_static(__FUNCTION__, array());

  $cid = $uid . ':' . $permission . ':' . $include_owner . ':' . $include_non_member . ':' . $include_global_permission;

  if (!isset($list[$cid])) {

    $list[$cid] = array();

    if (!gcc_features_get_enabled_bundles('group')) {
      return $list[$cid];
    }

    if ($include_global_permission) {

      $account = user_load($uid);

      // If the user has the corresponding global gcc permission.
      $permissions = gcc_permission_get_list();
      if ($permissions[$permission]['global']) {
        foreach ($permissions[$permission]['global'] as $perm) {
          if (user_access($perm, $account)) {

            // Get all the groups where $uid is the owner.
            foreach (gcc_group_get_list() as $item) {
              $list[$cid][$item->entity_type . ':' . $item->entity_id] = $item;
            }

            return $list[$cid];
          }
        }
      }
    }

    if ($include_owner) {

      // Get all the groups where $uid is the owner.
      $query = new EntityFieldQuery();
      $query->fieldCondition(GCC_FIELD_ENABLE, 'value', 1);
      $query->fieldCondition(GCC_FIELD_ENABLE, 'uid', $uid);
      $query->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');
      $groups = $query->execute();

      foreach ($groups as $entity_type => $entities) {
        foreach ($entities as $entity) {

          $entity_id = gcc_get_entity_id($entity_type, $entity);

          $item = (object) array(

            'entity_type' => $entity_type,
            'entity_id' => $entity_id,
          );

          $list[$cid][$item->entity_type . ':' . $item->entity_id] = $item;
        }
      }
    }

    // Next get all the groups where $uid is a member and has $permission.
    $query = db_select('gcc_membership', 'm');
    $query->fields('m', array('entity_type', 'entity_id'));
    $query->condition('m.uid', $uid);
    $query->condition('m.state', GCC_ACTIVE);

    // Retrieve all the roles given to $uid and the group coming with them.
    $role_query = db_select('gcc_users_roles', 'user_role');
    $role_query->distinct();
    $role_query->addField('user_role', 'uid');
    $role_query->addField('user_role', 'entity_type');
    $role_query->addField('user_role', 'entity_id');
    $role_query->addField('user_role', 'rid');
    $role_query->condition('user_role.uid', $uid);

    // Retrieve all the group $uid is a member of.
    // and give him the GCC_MEMBER role.
    $member_query = db_select('gcc_membership');
    $member_query->addField('gcc_membership', 'uid');
    $member_query->addField('gcc_membership', 'entity_type');
    $member_query->addField('gcc_membership', 'entity_id');
    $member_query->addExpression("'" . GCC_MEMBER . "'", 'rid');
    $member_query->condition('gcc_membership.uid', $uid);

    // Join the roles corresponding to the groups list.
    $query->join($role_query->union($member_query), 'user_role', '%alias.uid = m.uid AND %alias.entity_type = m.entity_type AND %alias.entity_id = m.entity_id');

    // Join the permissions corresponding to roles list.
    $query->join('gcc_role_permission', 'role_perm', '%alias.rid = user_role.rid AND ((%alias.entity_type = m.entity_type AND %alias.entity_id = m.entity_id) OR (%alias.entity_type = :type AND %alias.entity_id = :id))', array(':type' => GCC_GLOBAL_TYPE, ':id' => GCC_GLOBAL_ID));

    // Magic thing to filter out overriden permissions.
    $query->addJoin('LEFT', 'gcc_role_permission', 'role_perm_double', '%alias.rid = role_perm.rid AND role_perm.entity_type = :type AND %alias.entity_type != :type AND %alias.entity_type = m.entity_type AND %alias.entity_id = m.entity_id', array(':type' => GCC_GLOBAL_TYPE));
    $query->condition('role_perm_double.rid', NULL);

    // Finally filter the result set by the given permission.
    $query->condition('role_perm.permission', $permission);

    $result = $query->execute();
    $result->setFetchMode(PDO::FETCH_OBJ);
    foreach ($result as $item) {
      $list[$cid][$item->entity_type . ':' . $item->entity_id] = $item;
    }

    if ($include_non_member) {

      // Selecting all the enabled groups.
      $query = db_select('field_data_field_gcc_enable', 'm');
      $query->distinct();
      $query->fields('m', array('entity_type', 'entity_id'));
      $query->condition('m.field_gcc_enable_value', 1);

      // Filter only group where is not a member or still pending.
      // (so i can have the group he is a GCC_NON_MEMBER of).
      $query->addJoin('LEFT', 'gcc_membership', 'gcc_membership', '%alias.entity_type = m.entity_type AND %alias.entity_id = m.entity_id AND %alias.uid = :uid', array(':uid' => $uid));
      $condition = db_or();
      $condition->condition('gcc_membership.uid', NULL);
      $condition->condition('gcc_membership.state', GCC_PENDING);
      $query->condition($condition);

      // Join the permissions corresponding to his roles.
      $query->join('gcc_role_permission', 'role_perm', '%alias.rid = :non_member AND ((%alias.entity_type = m.entity_type AND %alias.entity_id = m.entity_id) OR (%alias.entity_type = :type AND %alias.entity_id = :id))', array(
        ':non_member' => GCC_NON_MEMBER,
        ':type' => GCC_GLOBAL_TYPE,
        ':id' => GCC_GLOBAL_ID,
      ));

      // Magic thing to filter out overriden permissions.
      $query->addJoin('LEFT', 'gcc_role_permission', 'role_perm_double', '%alias.rid = role_perm.rid AND role_perm.entity_type = :type AND %alias.entity_type != :type AND %alias.entity_type = m.entity_type AND %alias.entity_id = m.entity_id', array(':type' => GCC_GLOBAL_TYPE));
      $query->condition('role_perm_double.rid', NULL);

      // Finally filter the result set by the given permission.
      $query->condition('role_perm.permission', $permission);

      $result = $query->execute();
      $result->setFetchMode(PDO::FETCH_OBJ);
      foreach ($result as $item) {
        $list[$cid][$item->entity_type . ':' . $item->entity_id] = $item;
      }
    }
  }

  return $list[$cid];
}

/* Operations Functions */

/**
 * Return the list a available operations.
 */
function gcc_operation_get_list($entity_type, $entity_id) {

  $list = &drupal_static(__FUNCTION__, array());
  $cid = $entity_type . ':' . $entity_id;

  if (!isset($list[$cid])) {

    $list[$cid] = module_invoke_all('gcc_operation', $entity_type, $entity_id);

    drupal_alter('gcc_operation_list', $list[$cid], $entity_type, $entity_id);

    foreach ($list[$cid] as &$item) {
      $item += array(

        'file' => FALSE,
        'group' => t('Default'),
        'params' => array(),
      );
    }
  }

  return $list[$cid];
}

/**
 * Execute a mass operation.
 */
function gcc_operation_mass_execute($entity_type, $entity_id, $uids, $operation, $operation_params = array()) {

  $operations = gcc_operation_get_list($entity_type, $entity_id);

  if (!isset($operations[$operation])) {
    return;
  }
  $operation = $operations[$operation];
  $params = array(

    $entity_type,
    $entity_id,
    $uids,
    $operation,
    $operation_params,
  );

  $batch = array(

    'operations' => array(

      array('_gcc_operation_mass_execute', $params),
    ),
    'finished' => '_gcc_operation_mass_finished',
    'file' => drupal_get_path('module', 'gcc') . '/gcc.inc',
    'title' => t('Working'),
  );

  batch_set($batch);
}

/**
 * Internal functions.
 *
 * TODO.
 */
function _gcc_operation_mass_execute($entity_type, $entity_id, $uids, $operation, $operation_params, &$context) {

  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = count($uids);
    $context['sandbox']['current'] = 0;
    $context['results']['results'] = 0;
  }

  if ($operation['file']) {
    require_once DRUPAL_ROOT . '/' . $operation['file'];
  }

  if ($operation['callback']($entity_type, $entity_id, $uids[$context['sandbox']['current']], $operation['params'], $operation_params)) {
    ++$context['results']['results'];
  }
  ++$context['sandbox']['current'];

  $context['finished'] = $context['sandbox']['current'] / $context['sandbox']['total'];
}

/**
 * Internal functions.
 *
 * TODO.
 */
function _gcc_operation_mass_finished($success, $results) {

  if ($success) {
    drupal_set_message(t('Operation successfully applied on @count items', array('@count' => $results['results'])));
  }
}

/* Field Functions */

/**
 * Create an instance for the given entity type/bundle and the given field.
 */
function gcc_field_create_field_instance($field_type, $field_name, $label, $entity_type, $bundle, $field_definition = array(), $instance_definition = array()) {

  if (!field_read_field($field_name)) {

    $field = array(

      'field_name' => $field_name,
      'type' => $field_type,
    );
    $field += $field_definition;

    field_create_field($field);
  }

  $instance = field_read_instance($entity_type, $field_name, $bundle);

  if (!$instance) {

    $instance = array(

      'label' => $label,
      'field_name' => $field_name,
      'entity_type' => $entity_type,
      'bundle' => $bundle,
    );

    $instance += $instance_definition;

    field_create_instance($instance);
  }
}

/**
 * Delete the instance for the given entity type/bundle and the given field.
 */
function gcc_field_delete_field_instance($field_name, $entity_type, $bundle) {

  $instance = field_read_instance($entity_type, $field_name, $bundle);
  if ($instance) {

    field_delete_instance($instance);
    field_purge_batch(10);
  }
}
