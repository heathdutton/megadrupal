<?php

/**
 * @file
 * Hooks provided by the GCC Core module.
 */

/* Entity Hooks */

/**
 * Declare an entity type to be used with group.
 *
 * This function is used to declare an entity type being useable with group.
 * This will add a "Group" tab on the configuration page of every bundle of this
 * entity type with the group configuration options.
 * It will also add a "Group" tab for every entity of this type with the
 * internal group administration when the group features is enable.
 *
 * @return array
 *   An array of entity type. Each item has a key corresponding to the entity
 *   type. The corresponding array value is an associative array that may
 *   contain the following key-value pairs :
 *   - "path" : Required. The base path for viewing entity of this type. The
 *     path must contain a valid auto-loader wildcard for the entity.
 *   - "entity position" : Required. The index of the auto-loader wildcard in
 *     the path.
 *   - "group" : Optionnal. A boolean value indicating if bundle of this entity
 *     type can be used as groups. Default to FALSE.
 *   - "group content" : Optionnal. A boolean value indicating if bundle of this
 *     entity type can be used as group contents. Default to FALSE.
 */
function hook_gcc_entity_info() {

  $entity_types = array();

  $entity_types['node'] = array(

    'path' => 'node/%node',
    'entity position' => 1,
    'group' => TRUE,
    'group content' => TRUE,
  );

  $entity_types['user'] = array(

    'path' => 'user/%user',
    'entity position' => 1,
    'group' => TRUE,
    'group content' => FALSE,
  );

  return $entity_types;
}

/* Features Hooks */

/**
 * Declare a features that can be used by GCC.
 *
 * This function is used to declare features.
 * TODO : complete
 *
 * @return array
 *   An array of features. Each item has a key corresponding to the features
 *   machine anme. The corresponding array value is an associative array that
 *   must contain the following key-value pairs :
 *   - "label" : Required. The label of the feature. This label will be used
 *     on the configuration form of each bundle.
 *   - "explaination" : Required. This explaination will be display on the
 *     bundle overview page for every bundle with this feature activated.
 */
function hook_gcc_features_info() {

  $features = array();

  $features['group'] = array(

    'label' => t('Enable the group features'),
    'explaination' => t('Can be used as a group'),
  );

  $features['group_content'] = array(

    'label' => t('Enable the group content features'),
    'explaination' => t('Can be used as a group content'),
  );

  return $features;
}

/**
 * Declare what feature is usable with $entity_type/$bundle.
 *
 * This function is used to declare the feature that are allowed to be use
 * with the $bundle of $entity_type. It can specify
 * an accurate description that will be display on the bundle configuration
 * form.
 * TODO : complete
 *
 * @return array
 *   An array of features that is allowed. Each item has a key corresponding to
 *   the features machine anme. The corresponding array value is an associative
 *   array that must contain the following key-value pairs :
 *   - "description" : TODO
 */
function hook_gcc_entity_features($entity_type, $bundle) {

  $features = array();

  if ($entity_type == 'node') {

    $features['group'] = array(

      'enabled' => is_array(field_read_instance($entity_type, GCC_FIELD_ENABLE, $bundle)),
      'description' => t('When enabled, %entity_type : %bundle can be used as group.', array('%entity_type' => $entity_type, '%bundle' => $bundle)),
    );

    $features['group_content'] = array(

      'enabled' => is_array(field_read_instance($entity_type, GCC_FIELD_AUDIENCE, $bundle)),
      'description' => t('When enabled, %entity_type : %bundle can be associated to one or more existing groups as group content', array('%entity_type' => $entity_type, '%bundle' => $bundle)),
    );
  }

  return $features;
}

/**
 * TODO.
 */
function hook_gcc_features_is_enabled($feature, $entity_type, $bundle) {

  switch ($feature) {

    case 'group':
      return is_array(field_read_instance($entity_type, GCC_FIELD_ENABLE, $bundle));

    case 'group_content':
      return is_array(field_read_instance($entity_type, GCC_FIELD_AUDIENCE, $bundle));
  }
}

/**
 * TODO.
 */
function hook_gcc_features_enable($feature, $entity_type, $bundle) {

  switch ($feature) {

    case 'group':
      gcc_field_create_field_instance(GCC_FIELD_TYPE_ENABLE, GCC_FIELD_ENABLE, t('Enable the group'), $entity_type, $bundle);
      break;

    case 'group_content':
      gcc_field_create_field_instance(GCC_FIELD_TYPE_AUDIENCE, GCC_FIELD_AUDIENCE, t('Associate with the following groups'), $entity_type, $bundle);
      break;
  }
}

/**
 * TODO.
 */
function hook_gcc_features_disable($feature, $entity_type, $bundle) {

  switch ($feature) {

    case 'group':
      gcc_field_delete_field_instance(GCC_FIELD_ENABLE, $entity_type, $bundle);
      break;

    case 'group_content':
      gcc_field_delete_field_instance(GCC_FIELD_AUDIENCE, $entity_type, $bundle);
      break;
  }
}

/* Core Hooks */

/**
 * TODO.
 *
 * the key '__overriden' is reserved by the system.
 */
function hook_gcc_permission() {

  $perm = array();

  $perm['subscribe'] = array(

    'title' => t('Subscribe'),
    'group' => t('Subscription'),
  );

  $perm['skip approval'] = array(

    'title' => t('Skip approval when subscribing'),
    'group' => t('Subscription'),
  );

  $perm['unsubscribe'] = array(

    'title' => t('Unsubscribe'),
    'group' => t('Subscription'),
  );

  $perm['administer group'] = array(

    'title' => t('Administer the group'),
    'group' => t('Administration'),
    'global' => array(
      'administer all groups',
    ),
  );

  $perm['administer group members'] = array(

    'title' => t('Administer the group members'),
    'group' => t('Administration'),
    'global' => array(
      'administer all groups',
    ),
  );

  return $perm;
}

/**
 * TODO.
 */
function hook_gcc_audience_allowed_value_alter(&$list, $context) {

}

/* Roles Hooks */

/**
 * TODO.
 */
function hook_gcc_role_add($entity_type, $entity_id, $role) {

}

/**
 * TODO.
 */
function hook_gcc_role_save($role) {

}

/**
 * TODO.
 */
function hook_gcc_role_delete($rid) {

}

/* Membership Hooks */

/**
 * TODO.
 */
function hook_gcc_memberhsip_create($membership) {

}

/**
 * TODO.
 */
function hook_gcc_membership_activate($entity_type, $entity_id, $uid) {

}

/**
 * TODO.
 */
function hook_gcc_membership_block($entity_type, $entity_id, $uid) {

}

/**
 * TODO.
 */
function hook_gcc_membership_delete($entity_type, $entity_id, $uid) {

}

/**
 * TODO.
 */
function hook_gcc_membership_add_role($entity_type, $entity_id, $uid, $rid) {

}

/**
 * TODO.
 */
function hook_gcc_membership_remove_role($entity_type, $entity_id, $uid, $rid) {

}
