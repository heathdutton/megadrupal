<?php

/**
 * Implements hook_views_data().
 */
function recommender_views_data() {
  $views_data = array();
  $recommender_data = recommender_fetch_data();
  $tables = array();

  // NOTE: if different recommender applications use the same tables in hook_recommender_data(),
  // then these will cause the later tables to override the first few.
  // We'll just rely on hook_recommender_data() define recommender applications correctly.

  foreach ($recommender_data as $name => $def) {
    $raw_structure = $def['data structure'];
    $default_structure = recommender_prepare_data_structure($raw_structure);
    $item_entity_info = entity_get_info(@$default_structure['item entity type']);
    $user_entity_info = entity_get_info(@$default_structure['user entity type']);

    // handle preference table.
    if (isset($raw_structure['preference']) && $default_structure['preference']['type'] == 'table' && !@$raw_structure['preference']['no views']) {
      $table_structure = $default_structure['preference'];
      $tables[$table_structure['name']][] = $def['title'];
      $views_data[$table_structure['name']] = _recommender_views_template_table($table_structure, 'preference', implode(', ', $tables[$table_structure['name']]), $user_entity_info, $item_entity_info);
    }

    // handle user similarity table
    if (isset($raw_structure['user similarity']) && $default_structure['user similarity']['type'] == 'table' && !@$raw_structure['user similarity']['no views']) {
      $table_structure = $default_structure['user similarity'];
      $tables[$table_structure['name']][] = $def['title'];
      $views_data[$table_structure['name']] = _recommender_views_template_table($table_structure, 'user similarity', implode(', ', $tables[$table_structure['name']]), $user_entity_info, $item_entity_info);
    }

    // handle item similarity table
    if (isset($raw_structure['item similarity']) && $default_structure['item similarity']['type'] == 'table' && !@$raw_structure['item similarity']['no views']) {
      $table_structure = $default_structure['item similarity'];
      $tables[$table_structure['name']][] = $def['title'];
      $views_data[$table_structure['name']] = _recommender_views_template_table($table_structure, 'item similarity', implode(', ', $tables[$table_structure['name']]), $user_entity_info, $item_entity_info);
    }

    // handler prediction table
    if (isset($raw_structure['prediction']) && $default_structure['prediction']['type'] == 'table' && !@$raw_structure['prediction']['no views']) {
      $table_structure = $default_structure['prediction'];
      $tables[$table_structure['name']][] = $def['title'];
      $views_data[$table_structure['name']] = _recommender_views_template_table($table_structure, 'prediction', implode(', ', $tables[$table_structure['name']]), $user_entity_info, $item_entity_info);
    }

  } // end of foreach

  return $views_data;
}


function _recommender_views_template_table($table_structure, $table_type, $table_human_readable_name, $user_entity_info, $item_entity_info) {
  $data = array();
  $data['table'] = array(
    'group' => t('Recommender'),
    'help' => "$table_human_readable_name: $table_type",
    'base' => array(
      // this field doesn't matter when showing a list of db rows.
      // Perhaps will be used for table joins, but apparently we don't use this table for that purpose.
      'field' => 'id',
      'title' => "$table_human_readable_name: $table_type",
    ),

    // join is tricky because 2 fields in this table might need to be joined to the same 'users' table.
    // but the 2nd user field will override the first one if they join to the same 'users' table.
    // the following 'join' definition doesn't work: the keys of 'join' array have to be base table names.

//          'join' => array(
//            'user1_table' => array(
//              'left_table' => 'users',
//              'left_field' => 'uid',
//              'field' => $prediction_structure['user1 field'],
//              //'type' => 'left', // by default is 'left join'.
//            ),
//            'user2_table' => array(
//              'left_table' => 'users',
//              'left_field' => 'uid',
//              'field' => $prediction_structure['user2 field'],
//            ),
//          ),

//          'join' => array(
//            'users' => array(
//              //'left_table' => 'users',
//              'left_field' => 'uid',
//              'field' => $prediction_structure['user field'],
//              //'type' => 'left', // by default is 'left join'.
//            ),
//            $item_entity_info['base table'] => array(
//              //'left_table' => $item_entity_info['base table'],
//              'left_field' => $item_entity_info['entity keys']['id'],
//              'field' => $prediction_structure['item field'],
//            ),
//          ),
  );

  // first field:
  switch ($table_type) {
    case 'preference':
    case 'prediction':
      $data[$table_structure['user field']] = _recommender_views_template_user_field(
        t('!name: user in !type table', array('!name' => $table_human_readable_name, '!type' => $table_type)),
        t('The user field in !type table', array('!type' => $table_type)),
        $user_entity_info
      );
      break;
    case 'user similarity':
      $data[$table_structure['user1 field']] = _recommender_views_template_user_field(
        t('!name: user 1 in user similarity table', array('!name' => $table_human_readable_name)),
        t('The user 1 field in user similarity table'),
        $user_entity_info
      );
      break;
    case 'item similarity':
      $data[$table_structure['item1 field']] = _recommender_views_template_item_field(
        t('!name: item 1 in item similarity table', array('!name' => $table_human_readable_name)),
        t('The item 1 field in item similarity table'),
        $item_entity_info
      );
      break;
  }

  // second field
  switch ($table_type) {
    case 'preference':
    case 'prediction':
      $data[$table_structure['item field']] = _recommender_views_template_item_field(
        t('!name: item in !type table', array('!name' => $table_human_readable_name, '!type' => $table_type)),
        t('The item field in !type table', array('!type' => $table_type)),
        $item_entity_info
      );
      break;
    case 'user similarity':
      $data[$table_structure['user2 field']] = _recommender_views_template_user_field(
        t('!name: user 2 in user similarity table', array('!name' => $table_human_readable_name)),
        t('The user 2 field in user similarity table'),
        $user_entity_info
      );
      break;
    case 'item similarity':
      $data[$table_structure['item2 field']] = _recommender_views_template_item_field(
        t('!name: item 2 in item similarity table', array('!name' => $table_human_readable_name)),
        t('The item 2 field in item similarity table'),
        $item_entity_info
      );
      break;
  }

  // third field, always 'score field'
  $data[$table_structure['score field']] = _recommender_views_template_score_field(
    t('!name: score in !type table', array('!name' => $table_human_readable_name, '!type' => $table_type)),
    t('The score field in !type table', array('!type' => $table_type))
  );

  // forth field, always 'timestamp'.
  $data[$table_structure['timestamp field']] = _recommender_views_template_timestamp_field(
    t('!name: timestamp in !type table', array('!name' => $table_human_readable_name, '!type' => $table_type)),
    t('The timestamp field in !type table', array('!type' => $table_type))
  );

  return $data;
}


function _recommender_views_template_user_field($title, $help, $user_entity_info) {
  $return = array(
    'title' => $title,
    'help' => $help,
    'field' => array(
      // see [#2369101]
      //'handler' => 'views_handler_field_user',
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // this is only valid in "base table". use "add relationship" to add users table.
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => $user_entity_info['base table'],
      'base field' => $user_entity_info['entity keys']['id'],
      'label' => $title,
    ),
  );
  if ($user_entity_info['base table'] == 'users') {
    $return['filter'] =  array(
      'handler' => 'views_handler_filter_user_name',
    );
    $return['argument'] = array(
      'handler' => 'views_handler_argument_user_uid',
    );
  }
  else {
    $return['filter'] =  array(
      'handler' => 'views_handler_filter_numeric',
    );
    $return['argument'] = array(
      'handler' => 'views_handler_argument_numeric',
    );
  }
  return $return;
}


// TODO: try to use entity handler instead of "views_handler_field_numeric".
function _recommender_views_template_item_field($title, $help, $item_entity_info) {
  $return = array(
    'title' => $title,
    'help' => $help,
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => $item_entity_info['base table'],
      'base field' => $item_entity_info['entity keys']['id'],
      'label' => $title,
    ),
  );
  if ($item_entity_info['base table'] == 'node') {
    $return['field'] = array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    );
    $return['argument'] = array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid'
    );
  }
  else {
    $return['field'] = array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    );
    $return['argument'] = array(
      'handler' => 'views_handler_argument_numeric',
    );
  }
  return $return;
}


function _recommender_views_template_score_field($title, $help) {
  return array(
    'title' => $title,
    'help' => $help,
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
}


function _recommender_views_template_timestamp_field($title, $help) {
  return array(
    'title' => $title,
    'help' => $help,
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );
}