<?php

/**
 * Helper function for og_services_services_resources.
 */
function _og_services_resource_definition() {
  $resources['og'] = array();
  // [GET] {endpoint}/og/{gid}
  $resources['og']['retrieve'] = array(
    'help' => 'Retrieves group information',
    'callback' => '_og_services_resource_retrieve',
    'file' => array('type' => 'inc', 'module' => 'og_services', 'name' => 'og_services.resource'),
    'access callback' => 'user_access',
    'access arguments' => array('administer group'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'gid',
        'type' => 'int',
        'description' => 'The gid of the group to retrieve.',
        'source' => array('path' => 0),
        'optional' => FALSE,
      ),
    ),
  );
  // [PUT] {endpoint}/og/{gid}
  $resources['og']['update'] = array(
  'help' => 'Updates notification settings for a user',
    'callback' => '_og_services_resource_update',
    'file' => array('type' => 'inc', 'module' => 'og_services', 'name' => 'og_services.resource'),
    'access callback' => 'user_access',
    'access arguments' => array('administer group'),
    'access arguments append' => FALSE,
      'args' => array(
        array(
          'name' => 'gid',
          'type' => 'int',
          'description' => 'Group GID',
          'source' => array('path' => 0),
          'optional' => FALSE,
        ),
        array(
          'name' => 'data',
          'type' => 'array',
          'description' => 'The group data with updated information',
          'source' => 'data',
          'optional' => FALSE,
          ),
        ),
  );
  // [POST] {endpoint}/og
  $resources['og']['create'] = array(
    'help' => 'Creates a group',
    'callback' => '_og_services_resource_create',
    'file' => array('type' => 'inc', 'module' => 'og_services', 'name' => 'og_services.resource'),
    'access callback' => 'user_access',
    'access arguments' => array('administer group'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'data',
        'type' => 'array',
        'description' => 'The group data',
        'source' => 'data',
        'optional' => FALSE,
      ),
    ),
  );
  // [DELETE] {endpoint}/og/{gid}
  $resources['og']['delete'] = array(
    'help' => 'Deletes group',
    'callback' => '_og_services_resource_delete',
    'file' => array('type' => 'inc', 'module' => 'og_services', 'name' => 'og_services.resource'),
    'access callback' => 'user_access',
    'access arguments' => array('administer group'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'gid',
        'type' => 'int',
        'description' => 'The GID of the group to delete.',
        'source' => array('path' => 0),
        'optional' => FALSE,
      ),
    ),
  );
  // [GET] {endpoint}/og
  $resources['og']['index'] = array(
    'help' => 'Lists groups',
    'callback' => '_og_services_resource_index',
    'file' => array('type' => 'inc', 'module' => 'og_services', 'name' => 'og_services.resource'),
    'access callback' => 'user_access',
    'access arguments' => array('administer group'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'page',
        'optional' => TRUE,
        'type' => 'int',
        'description' => 'The zero-based index of the page to get, defaults to 0.',
        'default value' => 0,
        'source' => array('param' => 'page'),
      ),
      array(
        'name' => 'fields',
        'optional' => TRUE,
        'type' => 'string',
        'description' => 'The fields to get.',
        'default value' => '*',
        'source' => array('param' => 'fields'),
      ),
      array(
        'name' => 'parameters',
        'optional' => TRUE,
        'type' => 'array',
        'description' => 'Parameters',
        'default value' => array(),
        'source' => array('param' => 'parameters'),
      ),
      array(
        'name' => 'pagesize',
        'optional' => TRUE,
        'type' => 'init',
        'description' => 'Number of records to get per page.',
        'default value' => variable_get('services_user_index_page_size', 20),
        'source' => array('param' => 'pagesize'),
      ),
    ),
  );
  // [POST] {endpoint}/og/{gid}/join
  $resources['og']['actions']['join'] = array(
    'help' => 'Subscribe a user to a group',
    'callback' => '_og_services_resource_user_join',
    'file' => array('type' => 'inc', 'module' => 'og_services', 'name' => 'og_services.resource'),
    'access callback' => 'user_access',
    'access arguments' => array('administer group'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'gid',
        'type' => 'int',
        'description' => 'A gid of a group on the system',
        'source' => array('data' => 'gid'),
        'optional' => FALSE,
      ),
      array(
        'name' => 'uid',
        'type' => 'int',
        'description' => 'A uid of a user on the system',
        'source' => array('data' => 'uid'),
        'optional' => FALSE,
      ),
    ),

  );
  // [POST] {endpoint}/og/{gid}/leave
  $resources['og']['actions']['leave'] = array(
    'help' => 'Unsubscribe a user from a group',
    'callback' => '_og_services_resource_user_leave',
    'file' => array('type' => 'inc', 'module' => 'og_services', 'name' => 'og_services.resource'),
    'access callback' => 'user_access',
    'access arguments' => array('administer group'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'gid',
        'type' => 'int',
        'description' => 'A gid of a group on the system',
        'source' => array('data' => 'gid'),
        'optional' => FALSE,
      ),
      array(
        'name' => 'uid',
        'type' => 'int',
        'description' => 'A uid of a user on the system',
        'source' => array('data' => 'uid'),
        'optional' => FALSE,
      ),
    ),
  );
  // [GET] {endpoint}/og/{gid}/users
  $resources['og']['relationships']['users'] = array(
    'help' => 'Show all users in a group',
    'callback' => '_og_services_resource_users',
    'file' => array('type' => 'inc', 'module' => 'og_services', 'name' => 'og_services.resource'),
    'access callback' => 'user_access',
    'access arguments' => array('administer group'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'gid',
        'description' => 'A gid of a group on the system',
        'optional' => FALSE,
        'type' => 'int',
        'source' => array('path' => 0),
      ),
      array(
        'name' => 'page',
        'optional' => TRUE,
        'type' => 'int',
        'description' => 'The zero-based index of the page to get, defaults to 0.',
        'default value' => 0,
        'source' => array('param' => 'page'),
      ),
      array(
        'name' => 'fields',
        'optional' => TRUE,
        'type' => 'string',
        'description' => 'The fields to get.',
        'default value' => '*',
        'source' => array('param' => 'fields'),
      ),
      array(
        'name' => 'parameters',
        'optional' => TRUE,
        'type' => 'array',
        'description' => 'Parameters',
        'default value' => array(),
        'source' => array('param' => 'parameters'),
      ),
      array(
        'name' => 'pagesize',
        'optional' => TRUE,
        'type' => 'init',
        'description' => 'Number of records to get per page.',
        'default value' => variable_get('services_user_index_page_size', 20),
        'source' => array('param' => 'pagesize'),
      ),
    ),
  );

  // [GET] {endpoint}/user/{uid}/groups
  $resources['user']['relationships']['groups'] = array(
    'help' => 'Show groups user is a member of.',
    'callback' => '_og_services_resource_user_memberships',
    'file' => array('type' => 'inc', 'module' => 'og_services', 'name' => 'og_services.resource'),
    'access callback' => 'user_access',
    'access arguments' => array('administer group'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'uid',
        'optional' => FALSE,
        'type' => 'int',
        'source' => array('path' => 0),
      ),
      array(
        'name' => 'page',
        'optional' => TRUE,
        'type' => 'int',
        'description' => 'The zero-based index of the page to get, defaults to 0.',
        'default value' => 0,
        'source' => array('param' => 'page'),
      ),
      array(
        'name' => 'fields',
        'optional' => TRUE,
        'type' => 'string',
        'description' => 'The fields to get.',
        'default value' => '*',
        'source' => array('param' => 'fields'),
      ),
      array(
        'name' => 'parameters',
        'optional' => TRUE,
        'type' => 'array',
        'description' => 'Parameters',
        'default value' => array(),
        'source' => array('param' => 'parameters'),
      ),
      array(
        'name' => 'pagesize',
        'optional' => TRUE,
        'type' => 'init',
        'description' => 'Number of records to get per page.',
        'default value' => variable_get('services_user_index_page_size', 20),
        'source' => array('param' => 'pagesize'),
      ),
    ),
  );

  return $resources;
}

function _og_services_resource_retrieve($gid) {
  $group = og_load($gid);
  if (!$group) {
    return services_error(t('There is no group with GID @gid.', array('@gid' => $gid)), 404);
  }
  return $group;
}

function _og_services_resource_update($gid, $data) {
  $group = og_load($gid);
  if (!$group) {
    return services_error(t('There is no group with GID @gid.', array('@gid' => $gid)), 404);
  }
  foreach ($data as $key => $value) {
    $group->{$key} = $value;
  }
  $group->save();
  return $group;
}

function _og_services_resource_create($data) {
  if (!$data['entity_type']) {
    return services_error(t('Missing required field "entity_type".'), 404);
  }
  if (!$data['etid']) {
    return services_error(t('Missing required field "etid".'), 404);
  }
  $group = og_create_group($data);
  if ($group) {
    $group->save();
  }
  else {
    return services_error(t('Error creating group.'), 404);
  }
  return $group;
}

function _og_services_resource_delete($gid) {
  $group = og_load($gid);
  if (!$group) {
    return services_error(t('There is no group with GID @gid.', array('@gid' => $gid)), 404);
  }
  entity_delete('group', $group->gid);
  return $group;
}

function _og_services_resource_index($page, $fields, $parameters, $page_size) {
  $sub_select = db_select('og', 't')
    ->orderBy('gid', 'ASC');

  services_resource_build_index_query($sub_select, $page, $fields, $parameters, $page_size, 'group');
  $results = services_resource_execute_index_query($sub_select);
  return services_resource_build_index_list($results, 'group', 'gid');
}

function _og_services_resource_user_join($gid, $uid) {
  if ($existing = og_get_group_membership($gid, 'user', $uid)) {
    $label = og_label($gid);
    return services_error(t('Group membership for user with UID @uid for group ID @gid (@label) already exists.', array('@uid' => $uid, '@gid' => $gid, '@label' => $label)), 404);
  }
  if ($membership = og_membership_create($gid, 'user', $uid)) {
    entity_save('og_membership', $membership);
    return $membership;
  }
  return services_error(t('Error creating membership for user with UID @uid for group ID @gid.', array('@uid' => $uid, '@gid' => $gid)), 404);
}

function _og_services_resource_user_leave($gid, $uid) {
  $existing = og_get_group_membership($gid, 'user', $uid);
  if (!$existing) {
    return services_error(t('Group membership for user with UID @uid for group ID @gid does not exist.', array('@uid' => $uid, '@gid' => $gid)), 404);
  }
  og_membership_delete($existing->id);
  return $existing;
}

function _og_services_resource_users($gid, $page, $fields, $parameters, $page_size) {
  $sub_select = db_select('og_membership', 't')
    ->condition('entity_type', 'user', '=')
    ->condition('gid', $gid, '=')
    ->orderBy('etid', 'ASC');

  services_resource_build_index_query($sub_select, $page, $fields, $parameters, $page_size, 'og_membership');
  $results = services_resource_execute_index_query($sub_select);
  return services_resource_build_index_list($results, 'og_membership', 'gid');
}

function _og_services_resource_user_memberships($uid, $page, $fields, $parameters, $page_size) {
  $sub_select = db_select('og_membership', 't')
    ->condition('entity_type', 'user', '=')
    ->condition('etid', $uid, '=')
    ->orderBy('gid', 'ASC');

  services_resource_build_index_query($sub_select, $page, $fields, $parameters, $page_size, 'og_membership');
  $results = services_resource_execute_index_query($sub_select);
  return services_resource_build_index_list($results, 'og_membership', 'gid');
}

