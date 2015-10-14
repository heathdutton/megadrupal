<?php
/**
 * @file
 * league_extras_owner.inc
 *
 * Game owner management
 */

function league_extras_get_team_owners($nid) {
  $owners = array();
  $node = node_load($nid);
  $field_items = field_get_items('node', $node, 'field_owners');
  foreach ($field_items as $field_item) {
    $owners[] = $field_item['target_id'];
  }
  return $owners;
}

function league_extras_get_owner_teams($uid = NULL) {
  $team_ids = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'team')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_owners', 'target_id', $uid, '=')
    ->addMetaData('account', user_load(1)); // Run the query as user 1.
  $result = $query->execute();
  if (isset($result['node'])) {
    $teams_nids = array_keys($result['node']);
    $teams = entity_load('node', $teams_nids); 
  }
  foreach ($teams as $team) {
    $team_ids[$team->nid] = $team->title;
  }
  return $team_ids;
}

function league_extras_field_owners() {
  // Check if our field is not already created.
  if (!field_info_field('field_owners')) {

    // Create the field base.
    $field = array(
      'active' => 1,
      'cardinality' => -1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_owners',
      'foreign keys' => array(
          'users' => array(
          'columns' => array(
            'target_id' => 'uid',
          ),
          'table' => 'users',
        ),
      ),
      'indexes' => array(
        'target_id' => array(
          0 => 'target_id',
        ),
      ),
     'locked' => 0,
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'behaviors' => array(
            'views-select-list' => array(
              'status' => 0,
            ),
          ),
          'sort' => array(
            'type' => 'none',
          ),
          'target_bundles' => array(),
        ),
        'target_type' => 'user',
      ),
      'translatable' => 0,
      'type' => 'entityreference',
    );
    field_create_field($field);

    // Create the field instance on the bundle.
    $instance = array(
      'bundle' => 'team',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'link' => FALSE,
          ),
          'type' => 'entityreference_label',
          'weight' => 6,
        ),
        'jquery_ajax_load' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_owners',
      'label' => 'Owners',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'entityreference',
        'settings' => array(
          'match_operator' => 'CONTAINS',
          'path' => '',
          'size' => 60,
        ),
        'type' => 'entityreference_autocomplete_tags',
        'weight' => 45,
      ),
    );
    field_create_instance($instance);
  }
}

