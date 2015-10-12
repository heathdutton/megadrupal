<?php

/**
 * Implementation of hook_ext_model_types.
 */
function ext_ext_model_types() {
  $types = array();
  $type_names = array_keys(entity_get_info());
  $type_names[] = 'view';
  foreach ($type_names as $type) {
    $types[$type] = array(
        'user_created' => FALSE,
    );
  }
  return $types;
}

/**
 * Implementation of hook_ext_store_types.
 */
function ext_ext_store_types() {
  return ext_ext_model_types();
}

/**
 * Implementation of hook_ext_models.
 */
function ext_ext_models() {
  $models = array();
  foreach (array_keys(entity_get_info()) as $entity_name) {
    _ext_create_models_from_entity($models, $entity_name);
  }

  // Remove certain fields from the User model.
  $remove_fields = array('pass', 'mode', 'sort', 'threshold', 'theme', 'signature_format', 'data');
  $fields = array();
  foreach ($models['User']['fields'] as $field) {
    if (!in_array($field['name'], $remove_fields)) {
      $fields[] = $field;
    }
  }
  $models['User']['fields'] = $fields;

  // Add custom validations to User model (below regexs for email validation).
  $user = '[a-zA-Z0-9_\-\.\+\^!#\$%&*+\/\=\?\`\|\{\}~\']+';
  $domain = '(?:(?:[a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.?)+';
  $ipv4 = '[0-9]{1,3}(\.[0-9]{1,3}){3}';
  $ipv6 = '[0-9a-fA-F]{1,4}(\:[0-9a-fA-F]{1,4}){7}';
  if (!isset($model['User']['validations'])) $model['User']['validations'] = array();
  $models['User']['validations'][] = array(
      'field' => 'name',
      'type' => 'presence',
  );
  $models['User']['validations'][] = array(
      'field' => 'mail',
      'type' => 'format',
      'matcher' => "^$user@($domain|(\[($ipv4|$ipv6)\]))\$",
  );
  
  // TODO Automatically add associations for all models containing a field named 'uid' to the User model?
  // Or probably better just for some specific entity types such as nodes, comments, files?


  /* OLD CODE, WE NOW USE ENTITY FRAMEWORK TO GENERATE MODELS. See _ext_create_models_from_entity().
   // TODO: Create Model defs for terms.

  // Create Model for node.
  $type = array(
      '#type' => 'node',
      '#db_base_table' => 'node',
      'fields' => array(),
      'idProperty' => 'nid',
  );
  // Get standard fields for nodes.
  $schema = drupal_get_schema('node');
  if( $schema )
  {
  foreach ($schema['fields'] as $field_name => $field) {
  $type['fields'][] = _ext_db_schema_field_to_ext_field($field_name, $field, 'node');
  }
  // Remove unused field(s).
  unset($type['fields']['moderate']);
  }
  // Add some fields from node_revisions.
  $schema = drupal_get_schema('node_revisions');
  if( $schema )
    foreach (array('body', 'teaser') as $field_name) {
  $type['fields'][] = _ext_db_schema_field_to_ext_field($field_name, $schema[$field_name], 'node_revisions');
  }

  // Add standard associations.
  $type['associations'] = array(
      array(
          'type' => 'belongsTo',
          'model' => 'User',
          'primaryKey' => 'uid',
          'foreignKey' => 'uid',
      ),
  );

  $models['Node'] = $type;


  // Create model for each content type.
  // EH: D6 => D7 porting required
  //if (module_exists('content')) {
  // Get info about all CCK fields for all content types.
  //$cck_fields = _content_type_info();
  //}
  foreach (node_type_get_types() as $node_type => $node_type_info) {
  $type = array(
      '#type' => 'node',
      '#node_type' => $node_type,
      '#db_base_table' => 'node',
      '#extends' => 'Node',
      'idProperty' => 'nid',
      'fields' => array(),
      'associations' => array(),
      'validations' => array(),
  );

  // Add standard validations.
  $type['validations'] = array();
  if ($node_type_info->has_title) {
  $type['validations'][] = array(
      'field' => 'title',
      'type' => 'presence',
  );
  }

  // Add fields.
  foreach (field_info_instances('node', $node_type) as $instance) {
  $field = field_info_field_by_id($instance['field_id']);
  $field_name = $instance['field_name'];

  //foreach ($cck_fields['content types'][$node_type]['fields'] as $field_name => $field) {
  if (isset($instance['widget']['default_value'][0]) && is_array($instance['widget']['default_value'][0])) {
  $default_value = $instance['widget']['default_value'][0];
  }
  else {
  $default_value = FALSE;
  }

  // Add field for each (DB) column the CCK field defines.
  foreach ($field['columns'] as $col => $col_def) {
  $field_def = array(
      'name' => $field_name. '_'. $col, //$db_info['columns'][$col]['column'], // Should be $field_name. '_'. $col
      '#db_table' => $field_name, //$db_info['table'],
      '#db_column' => $col, //$db_info['columns'][$col]['column'],
      // Store some info about the CCK field to make things easier later.
      '#content_field_name' => $field_name, // The CCK field name.
      '#content_column' => $col, // The value key in the field on the node.
  );

  // Only specify a type for fields that have a single value and are not
  // serialized, otherwise Ext tries to convert an array of values, or some
  // other structure to a single value.
  $col_type = _ext_db_schema_field_type_to_ext_field_type($col_def['type']);
  if ($field['cardinality']==1 && $col_type)
    $field_def['type'] = $col_type;

  if ($default_value) {
  $field_def['defaultValue'] = $default_value[$col];
  }

  $type['fields'][] = $field_def;
  }

  // Special handling for file type to include extra columns from file_managed table.
  if ($field['type'] == 'file') {
  $db_table = $field['foreign keys']['fid']['table'];
  $schema = drupal_get_schema($db_table);
  foreach ($schema['fields'] as $files_field_name => $files_field) {
  // 'fid' included in CCK file field def.
  if ($files_field_name != 'fid') {
  $field_def = array(
      'name' => $field_name. '_'. $files_field_name,
      '#db_table' => $db_table,
      '#db_column' => $files_field_name,
      '#content_field_name' => $field_name, // The CCK field name.
      '#content_column' => $files_field_name, // The value key in the field on the node.
  );
  if (isset($field['default'])) {
  $field_def['defaultValue'] = $files_field['default'];
  }

  // Only specify a type for columns that  are not serialized, otherwise Ext tries
  // to convert an array of values, or some other structure to a single value.
  $col_type = _ext_db_schema_field_type_to_ext_field_type($files_field['type']);
  if ($col_type)
    $field_def['type'] = $col_type;

  $type['fields'][] = $field_def;
  }
  }
  }

  $columns = array_keys($field['columns']);

  // First field name, used in validations and/or associations.
  $field_col_name = $field_name. '_'. $columns[0];

  // If there is only one column containing a single value, add standard
  // validations.
  // TODO provide validations for multiple column field types and/or multiple value fields?
  if (count($columns) == 1  && $field['cardinality']==1) {
  if ($instance['required']) {
  $type['validations'][] = array(
      'field' => $field_col_name,
      'type' => 'presence',
  );
  }
  if (!empty($field['max_length'])) {
  $type['validations'][] = array(
      'field' => $field_col_name,
      'type' => 'length',
      'max' => $field['max_length'],
  );
  }
  // The "allowed values" validation is determined at run time when it
  // is calculated using a PHP script.
  if (!empty($field['settings']['allowed_values']) && empty($field['allowed_values_php'])) {
  $allowed_values = list_allowed_values($field);
  $type['validations'][] = array(
      'field' => $field_col_name,
      'type' => 'inclusion',
      'list' => array_keys($allowed_values), //content_allowed_values($field)),
  );
  }
  elseif (!empty($field['allowed_values_php'])) {
  $type['server_validate'] = TRUE;
  }
  }

  // Add associations if available.
  // We only support single value fields (mapped as 'belongsTo' association types)
  // at the moment. Conceptually, multiple value fields could be mapped as a 'hasMany'
  // association, however the foreignKey for a hasMany association is the field name
  // on the associated Model linking it to the owner Model, and there is no such field
  // on referenced nodes (one could be added under certain circumstances (ie where it's
      // guaranteed that only one reference will exist to a node from this particular CCK
      // field type), but this is beyond the scope of this module.
      if (($field['type'] == 'entityreference') && ($field['cardinality'] == 1)) {
      if ($field['settings']['target_type'] == 'node') {
      // Create association for every node type that can be referenced.

      foreach (array_filter($field['bundles']) as $bundle)
    foreach( $bundle as $ref_type)
    {
      $ref_model_name = 'Node'. _ext_camelcase($ref_type);
      $assoc = array(
          'type' => 'belongsTo',
          'model' => $ref_model_name,
          'primaryKey' => 'nid', // Primary ID of associated Model.
          'foreignKey' => $field_col_name, // Field name on this Model linking it to associated Model.
          //'#load_nested_max_depth' => 1,
      );
      $type['associations'][] = $assoc;
      }
      }
      elseif ($field['settings']['target_type'] == 'user') { // @TEST: this yet to be tested
      $assoc = array(
          'type' => 'belongsTo',
          'model' => 'User',
          'primaryKey' => 'uid', // Primary ID of associated Model.
          'foreignKey' => $field_col_name, // Field name on this Model linking it to associated Model.
      );
      $type['associations'][] = $assoc;
      }
      }
      }

      $models[_ext_gen_name('node', $node_type)] = $type;
      }


      if (module_exists('profile2')) {
      _ext_create_models_from_entity($models, 'profile2');
      }

      _ext_create_models_from_entity($models, 'user');

      // Create Model for user.
      $type = array(
          '#type' => 'user',
          '#db_base_table' => 'users',
          'fields' => array(),
          'idProperty' => 'uid',
      );
      // Get standard fields for users.
      $schema = drupal_get_schema('users');
      foreach ($schema['fields'] as $field_name => $field) {
      if (!in_array($field_name, array('pass', 'mode', 'sort', 'threshold', 'theme', 'signature_format', 'data'))) {
      $type['fields'][] = _ext_db_schema_field_to_ext_field($field_name, $field, 'users');
      }
      }
      // Add standard validations (below regexs for email validation).
      $user = '[a-zA-Z0-9_\-\.\+\^!#\$%&*+\/\=\?\`\|\{\}~\']+';
      $domain = '(?:(?:[a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.?)+';
      $ipv4 = '[0-9]{1,3}(\.[0-9]{1,3}){3}';
      $ipv6 = '[0-9a-fA-F]{1,4}(\:[0-9a-fA-F]{1,4}){7}';
      $type['validations'] = array(
          array(
              'field' => 'name',
              'type' => 'presence',
          ),
          array(
              'field' => 'mail',
              'type' => 'format',
              'matcher' => "^$user@($domain|(\[($ipv4|$ipv6)\]))\$",
          ),
      );
      if (module_exists('profile2')) {

      // Add profile2 categories as associations.
      foreach (profile2_get_types() as $cat_name => $cat_info) {

      }
      }

      /* D6=>D7: Profile handling to be completed
      if (module_exists('profile')) {
      // Add profile fields.
      foreach (profile_categories() as $cat_info) {
      $profile_fields = _profile_get_fields($cat_info['name']);
      while ($field = db_fetch_array($profile_fields)) {
      $field_name = $field['name'];
      // Add field.
      $type['fields'][] = array(
          'name' => $field_name,
          'type' => 'string', // All profile field values are stored as text.
      );
      // Add validations.
      $type['validations'] = array();
      if ($field['required']) {
      $type['validations'][] = array(
          'field' => $field_name,
          'type' => 'presence',
      );
      }
      if ($field['type'] == 'selection') {
      $type['validations'][] = array(
          'field' => $field_name,
          'type' => 'inclusion',
          'list' => array_filter(split("[\n\r]", $field['options'])),
      );
      }
      }
      }
      }*/
  /*
   $models['User'] = $type;


      // Create Model for files.
      $type = array(
          '#type' => 'file',
          '#db_base_table' => 'files',
          'idProperty' => 'fid',
          'fields' => array(), // Added below from schema.
          'associations' => array(
              array(
                  'type' => 'belongsTo',
                  'model' => 'User',
                  'primaryKey' => 'uid',
                  'foreignKey' => 'uid',
              ),
          ),
      );
      // Get standard fields for files.
      $schema = drupal_get_schema('files');
      if( $schema )
      {
      foreach ($schema['fields'] as $field_name => $field) {
      $type['fields'][] = _ext_db_schema_field_to_ext_field($field_name, $field, 'files');
      }
      $models['File'] = $type;
      }

      if (module_exists('comment')) {
      // Create Model for comments.
      $type = array(
          '#type' => 'comment',
          '#db_base_table' => 'comments',
          'idProperty' => 'cid',
          'fields' => array(), // Added below from schema.
          'associations' => array(
              array(
                  'type' => 'belongsTo',
                  'model' => 'User',
                  'primaryKey' => 'uid',
                  'foreignKey' => 'uid',
              ),
              array(
                  'type' => 'belongsTo',
                  'model' => 'Comment',
                  'primaryKey' => 'uid',
                  'foreignKey' => 'pid',
              ),
              array(
                  'type' => 'belongsTo',
                  'model' => 'Node',
                  'primaryKey' => 'nid',
                  'foreignKey' => 'nid',
              ),
          ),
      );
      // Get standard fields for comments.
      $schema = drupal_get_schema('comments');
      if( $schema )
      {
      foreach ($schema['fields'] as $field_name => $field) {
      $type['fields'][] = _ext_db_schema_field_to_ext_field($field_name, $field, 'files');
      }
      $models['Comment'] = $type;
      }
      }

      */


  if (module_exists('views')) {
    // Create Models for relevant Views displays.
    // These are used for View displays that use a 'fields' row style.
    $views = views_get_all_views();
    foreach ($views as $view_name => $view) {
      if (!$view->disabled) {
        $view->set_display(); // Builds all settings for View displays.

        // Create a Model for each display that has row style set to 'fields'.
        foreach ($view->display as $display_name => $display) {
          if ($display->handler->options['row_plugin'] == 'fields' && isset($display->display_options['fields'])) {
            $type_name = _ext_gen_name('view', $view_name, $display_name);
            $type = array(
                '#type' => 'view',
                '#view_name' => $view_name,
                '#display_name' => $display_name,
                'idProperty' => _ext_table_primary_id($view->base_table),
                'fields' => array(),
            );
            foreach ($display->display_options['fields'] as $field_name => $field) {
              $type['fields'][] = array(
                  'name' => $field['table']. '_'. $field['field'],
              );
            }
            $models[_ext_gen_name('view', $view_name, $display_name)] = $type;
          }
        }
      }
    }
  }

  return $models;
}

function _ext_create_models_from_entity(&$models, $entity_name) {
  $entity_info = entity_get_info($entity_name);

  // Create base model for entity.
  $table = $entity_info['base table'];
  $model = array(
      '#type' => $entity_name,
      '#entity' => $entity_name,
      '#db_base_table' => $table,
      'idProperty' => $entity_info['entity keys']['id'],
      'fields' => array(), // Added below from schema.
      'associations' => array(), // Added below from schema.
  );
  
  // Add fields from DB schema.
  $schema = drupal_get_schema($table);
  foreach ($schema['fields'] as $field_name => $field) {
    $model['fields'][] = _ext_db_schema_field_to_ext_field($field_name, $field, $table);
  }
  // Add User association if available from DB schema.
  // TODO generalise for other referenced entities/bundles.
  if (isset($schema['foreign keys'])) { 
    foreach ($schema['foreign keys'] as $fk_spec) {
      if ($fk_spec['table'] == 'users') {
        // Assume only one column is used (definitely the case for user, don't know about other entities...
        $ak = array_keys($fk_spec['columns']);
        $foreignKey = array_shift($ak); // should work for any entity type (not just user).
		$primaryKey = $fk_spec['columns'][$foreignKey]; // should work for any entity type (not just user).
        $assoc = array(
            'type' => 'belongsTo',
            'model' => 'User',
            'primaryKey' => $primaryKey, // Primary ID of associated Model.
            'foreignKey' => $foreignKey, // Field name on this Model linking it to associated Model.
        );
        $model['associations'][] = $assoc;
      }
    }
  }
  
  $base_model_name = _ext_camelcase($entity_name);
  $models[$base_model_name] = $model;

  // Create model extending base model for each bundle.
  foreach (field_info_instances($entity_name) as $bundle_name => $fields) {
    //if (strtolower($bundle_name) == 'user' || strtolower($entity_name) == 'user') {
    //  wpr($fields, $bundle_name);
    //}
    
    if ($bundle_name == $entity_name) {
      // If the bundle has the same name as the entity then just add fields to existing
      // base model.
      $models[$base_model_name]['#bundle'] = $bundle_name;
      _ext_add_entity_fields_to_model($models[$base_model_name], $fields);
    }
    else {
      $model = array(
          '#type' => $entity_name,
          '#extends' => $base_model_name,
          '#entity' => $entity_name,
          '#bundle' => $bundle_name,
          '#db_base_table' => $table,
          'idProperty' => $entity_info['entity keys']['id'],
          'fields' => array(),
      );
      _ext_add_entity_fields_to_model($model, $fields);
      $models[_ext_camelcase($entity_name. '_'. $bundle_name)] = $model;
    }
  }
}

function _ext_add_entity_fields_to_model(&$model, $fields) {
  foreach ($fields as $field_name => $field_instance) {
    $field = field_info_field_by_id($field_instance['field_id']);
    // Add field for each column the field defines.
    foreach ($field['columns'] as $col => $col_def) {
      $field_def = array(
          'name' => $field_name. '_'. $col,
          // Store some info about the CCK field to make things easier later.
          '#content_field_name' => $field_name, // The CCK field name.
          '#content_column' => $col, // The value key in the field on the node.
      );
      // Is this necessary?
      if ($field['storage']['type'] == 'field_sql_storage') {
      	$temp = array_keys($field['storage']['details']['sql']['FIELD_LOAD_CURRENT']); //remove strict warning
        $field_def['#db_table'] = array_shift($temp);
        $field_def['#db_column'] = $field['storage']['details']['sql']['FIELD_LOAD_CURRENT'][$field_def['#db_table']][$col];
      }

      // Only specify a type for fields that have a single value and are not
      // serialized, otherwise Ext tries to convert an array of values, or some
      // other structure to a single value.
      $col_type = _ext_db_schema_field_type_to_ext_field_type($col_def['type']);
      if ($field['cardinality']==1 && $col_type)
        $field_def['type'] = $col_type;

      if (!empty($field_instance['default_value'][0][$col])) {
        $field_def['defaultValue'] = $field_instance['default_value'][0][$col];
      }

      $model['fields'][] = $field_def;
    }

    if ($field['type'] == 'file') {
      // Add association to File models for file fields.
      // We only support single value file fields (mapped as a 'belongsTo' association type)
      // at the moment. Conceptually, multiple value fields could be mapped as a 'hasMany'
      // association, however the foreignKey for a hasMany association is the field name
      // on the associated Model (File) linking it to the owner Model, and there is no such field
      // on referenced Files (one could be added under certain circumstances (ie where it's
      // guaranteed that only one reference will exist to a File from this particular
      // field type), but this is beyond the scope of this module (conveniently).
      // We use belongsTo, rather than hasOne, so that the ext module will automatically generate a corresponding
      // hasMany association on the associated Model.
      if ($field['cardinality'] == 1) {
        $assoc = array(
            'type' => 'belongsTo',
            'model' => 'File',
            'primaryKey' => 'fid', // Primary ID of associated Model.
            'foreignKey' => $field_name. '_fid', // Field name on this Model linking it to associated Model.
            //'#load_nested_max_depth' => 1,
        );
        $model['associations'][] = $assoc;
      }
    }

    $columns = array_keys($field['columns']);

    // First field name, used in validations and/or associations.
    $field_col_name = $field_name. '_'. $columns[0];

    // If there is only one column containing a single value, add standard
    // validations.
    // TODO provide validations for multiple column field types and/or multiple value fields?
    if (count($columns) == 1 && $field['cardinality']==1) {
      if ($field_instance['required']) {
        $model['validations'][] = array(
            'field' => $field_col_name,
            'type' => 'presence',
        );
      }
      if (!empty($field['settings']['max_length'])) {
        $model['validations'][] = array(
            'field' => $field_col_name,
            'type' => 'length',
            'max' => $field['settings']['max_length'],
        );
      }
      // The "allowed values" validation is determined at run time when it
      // is calculated using a PHP script.
      if (!empty($field['settings']['allowed_values']) && empty($field['allowed_values_php']) && empty($field['allowed_values_function'])) {
        $allowed_values = list_allowed_values($field, $field_instance);
        $model['validations'][] = array(
            'field' => $field_col_name,
            'type' => 'inclusion',
            'list' => array_keys($allowed_values),
        );
      }
      elseif (!empty($field['allowed_values_php'])) {
        $model['server_validate'] = TRUE;
      }
    }

    // Add associations if available.
    // We only support single value fields (mapped as a 'belongsTo' association type)
    // at the moment. Conceptually, multiple value fields could be mapped as a 'hasMany'
    // association, however the foreignKey for a hasMany association is the field name
    // on the associated Model linking it to the owner Model, and there is no such field
    // on referenced entities (one could be added under certain circumstances (ie where it's
    // guaranteed that only one reference will exist to a node from this particular
    // field type), but this is beyond the scope of this module (conveniently).
    // We use belongsTo, rather than hasOne, so that the ext module will automatically generate a corresponding
    // hasMany association on the associated Model.
    if (($field['type'] == 'entityreference') && ($field['cardinality'] == 1)) {
      $target_entity = $field['settings']['target_type'];

      // Determine primary ID field of associated Model.
      $primary_key = NULL;
      foreach(array_values($field['foreign keys']) as $value_fk) {
        if (is_array($value_fk) && isset($value_fk['columns']['target_id'])) {
          $primary_key = $value_fk['columns']['target_id'];
          break;
        }
      }

      // Determine allowed target bundles.
      if (empty($field['settings']['handler_settings']['target_bundles'])) {
        // If this field can reference all bundle types of the target entity.
        $target_entity_info = entity_get_info($target_entity);
        $target_bundles = array_keys($target_entity_info['bundles']);
      }
      else {
        $target_bundles = $field['settings']['handler_settings']['target_bundles'];
      }

      // Create association for every bundle that can be referenced.
      foreach ($target_bundles as $target_bundle) {
        $ref_model_name = _ext_camelcase($field['settings']['target_type']. '_'. $target_bundle);
        $assoc = array(
            'type' => 'belongsTo',
            'model' => $ref_model_name,
            'primaryKey' => $primary_key, // Primary ID of associated Model.
            'foreignKey' => $field_col_name, // Field name on this Model linking it to associated Model.
            //'#load_nested_max_depth' => 1,
        );
        $model['associations'][] = $assoc;
      }
    }
    /* OLD CODE, ABOVE SUPPORTS REFERENCING ALL ENTITY TYPES.
     if ($field['settings']['target_type'] == 'node') {
    // Create association for every node type that can be referenced.

    foreach (array_filter($field['bundles']) as $bundle)
      foreach( $bundle as $ref_type)
      {
    $ref_model_name = 'Node'. _ext_camelcase($ref_type);
    $assoc = array(
        'type' => 'belongsTo',
        'model' => $ref_model_name,
        'primaryKey' => 'nid', // Primary ID of associated Model.
        'foreignKey' => $field_col_name, // Field name on this Model linking it to associated Model.
        //'#load_nested_max_depth' => 1,
    );
    $model['associations'][] = $assoc;
    }
    }
    elseif ($field['settings']['target_type'] == 'user') { // @TEST: this yet to be tested
    $assoc = array(
        'type' => 'belongsTo',
        'model' => 'User',
        'primaryKey' => 'uid', // Primary ID of associated Model.
        'foreignKey' => $field_col_name, // Field name on this Model linking it to associated Model.
    );
    $model['associations'][] = $assoc;
    }
    }*/
  }
}


/**
 * Implementation of hook_ext_stores.
 */
function ext_ext_stores() {
  $stores = array();
  
  $models = ext_ext_models();
  // Create generic Store definition for each base entity type first, then add bundle models.
  for ($entityOrBundle = 0; $entityOrBundle < 2; $entityOrBundle++) {
    foreach ($models as $model_name => $model) {
      // If the model is for an entity and we're adding bundle models now or the Model is for a base entity.
      if (isset($model['#entity']) && ($entityOrBundle == 1 || !isset($model['#bundle']))) {
        $store = array(
            '#type' => $model['#type'],
            '#entity' => $model['#entity'],
            '#generic' => TRUE,
            'model' => $model_name,
            '#filterable_fields' => array(),
            '#sortable_fields' => array(),
        );
        if (isset($model['#bundle'])) {
          $store['#bundle'] = $model['#bundle'];
        }
        
        // Collect filterable and sortable fields from base Model if available.
        if (isset($model['#extends'])) {
          $store['#filterable_fields'] = $stores[$model['#extends']]['#filterable_fields'];
          $store['#sortable_fields'] = $stores[$model['#extends']]['#sortable_fields'];
        }
    
        foreach ($model['fields'] as $field) {
          $store['#filterable_fields'][] = $field['name'];
    
          // If this is an entity field then check if it's sortable.
          if (isset($field['#content_field_name'])) {
            //TODO ...
            //$field = field_info_field($field['#content_field_name']);
            //print_r( $field );
            /*$db_info = content_database_info($field);
             if (!empty($db_info['columns'][$field['#content_column']]['sortable'])) {
            $store['#sortable_fields'][] = $field['name'];
            }*/
          }
          else {
            $store['#sortable_fields'][] = $field['name'];
          }
        }
        $stores[$model_name] = $store;
      }
    }
  }  

  if (module_exists('views')) {
    $views = views_get_all_views();
    foreach ($views as $view_name => $view) {
      if (!$view->disabled) {
        $view->set_display(); // Builds all options for View displays.
        // Create a Store for each display.
        foreach ($view->display as $display_name => $display) {
          $type_name = _ext_gen_name('view', $view_name, $display_name);
          $type = array(
              '#type' => 'view',
          ); // Store def.

          // If the 'fields' row style is not specified.
          if ($display->handler->options['row_plugin'] != 'fields') {
            // This View display can only be used as an instance selector, we use
            // one of the predefined Models.
            if ($view->base_table == 'node' && array_key_exists('filters', $display->display_options )) {
              // Only Display types that filter the type of node to one specific
              // content type can be used.
              $filters = $display->display_options['filters'];
              if (isset($filters['type']) &&
                  $filters['type']['operator'] == 'in' &&
                  count($filters['type']['value']) == 1) {
                // Set the Model to that of the content type.
                $content_type = array_pop($filters['type']['value']);
                $type['model'] = _ext_gen_name('node', $content_type);
              }
            }
            else {
              $model_name = _ext_base_table_to_model_name($view->base_table);
              if ($model_name) {
                $type['model'] = $model_name;
              }
            }
          }
          // If the 'fields' row style is specified and fields have been specified.
          elseif (isset($display->display_options['fields'])) {
            // Use View/display specific Model.
            $type['model'] = $type_name;
            // Record the primary ID field for later use.
            $type['#primaryID'] = _ext_table_primary_id($view->base_table);
          }

          // Only add the Store def if we could determine a Model to use.
          if (isset($type['model'])) {
            $stores[$type_name] = $type;
          }
        }
      }
    }
  }

  return $stores;
}


/**
 * Implementation of hook_ext_access_model_data.
 */
function ext_ext_access_model_data($model_instance_id, $model_name, $op, $account) {
  $model = ext_get_models($model_name);
  
  if (isset($model['#entity'])) {
    if ($op == 'create' || !isset($model_instance_id)) {
      // Special handling for node entities: see https://drupal.org/node/1780646
      if ($model['#entity'] == 'node') {
        return node_access($op, $model['#bundle'], $account);
      }
      return entity_access($op, $model['#entity'], NULL, $account);
    }
    $entity = entity_load_single($model['#entity'], $model_instance_id);
    if ($entity != FALSE) {
       return entity_access($op, $model['#entity'], $entity, $account);
    }
  }
  return FALSE;
  
  /* OLD CODE, CODE ABOVE USES ENTITY FREAMEWORK
  $model = ext_get_models($model_name);
  if ($model['#type'] == 'node') {
    if ($op == 'create') {
      $node_type = substr(_ext_uncamelcase($model_name), 5);
      return node_access($op, $node_type, $account);
    }
    $node = node_load($model_instance_id);
    return node_access($op, $node, $account);
  }
  elseif ($model['#type'] == 'user') {
    if ($op == 'create')
      return user_access('administer users');
    $account = user_load($model_instance_id);
    if ($op == 'update' || $op == 'delete')
      return user_edit_access($account);
    if ($op == 'view')
      return user_view_access($account);
  }
  elseif ($model['#type'] == 'file') {
    // TODO: Creating and updating file model instances isn't supported at the moment, should they be?
    return $op == 'view';
  }
  elseif ($model['#type'] == 'comment') {
    if (user_access('access comments')) {
      if ($op == 'create') {
        // This isn't ideal, we also need to check that they can post the
        // comment to the given node. This is done in ext_ext_save_model_data.
        return user_access('post comments', $account);
      }
      else {
        $comment = _comment_load($model_instance_id);
        $node = node_load($comment->nid);
        // If they have edit access to the comment, they can do anything with it.
        // Logic borrowed from comment_access() function (modified to use arbitrary account).
        if (($account->uid && $account->uid == $comment->uid && comment_num_replies($comment->cid) == 0 && $comment->status == COMMENT_PUBLISHED) || user_access('administer comments', $account)) {
          return TRUE;
        }

        // For 'view', just check they have view access for the relevant node.
        if ($op == 'view' && node_access('view', $node, $account)) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
  */
}


/**
 * Implementation of hook_ext_load_model_data.
 */
function ext_ext_load_model_data($model_instance_ids, $model_name, $options=array()) {
  $model = ext_get_models($model_name);
  if (isset($model['#entity'])) {
    $data = entity_load($model['#entity'], $model_instance_ids);
    foreach ($data as $id => &$entity) {
      $entity = (array) $entity;
      _ext_convert_from_cck_fields($model, $entity);
    }
    return array_values($data);
  }
  return array();
  
  /* OLD CODE, ABOVE USES ENTITY FRAMEWORK.
  // 'view' data is only loaded via a Store so is not handled here.
  if ($model['#type'] == 'node') {
    $data = (array) node_load($model_instance_id);

    // Check node type against model name. In some cases this function can be
    // called for the wrong node type, eg when a CCK nodereference field can
    // reference multiple node types.
    if ($model_name != 'Node' && ($data['type'] != $model['#node_type']))
      return FALSE;

    _ext_convert_from_cck_fields($data);
    return $data;
  }
  elseif ($model['#type'] == 'user') {
    return (array) user_load($model_instance_id);
  }
  elseif ($model['#type'] == 'file') {
    return db_fetch_array(db_query("SELECT * FROM {files} WHERE fid=%d", $model_instance_id));
  }
  elseif ($model['#type'] == 'comment') {
    return array(_comment_load($model_instance_id));
  }
  */
}


/**
 * Implementation of hook_ext_save_model_data.
 */
function ext_ext_save_model_data(&$data, $model_name, $new = FALSE) {
  $model = ext_get_models($model_name);
  if (isset($model['#entity'])) {
    if ($new) {
      unset($data[$model['idProperty']]);
    }
    _ext_convert_to_cck_fields($model, $data);
    $data = (object) $data; // entity_save expects an object.
    $success = entity_save($model['#entity'], $data);
    // Convert data back to our format.
    $data = (array) $data;
    _ext_convert_from_cck_fields($model, $data);
    return $success !== FALSE;
  }
  return FALSE;
  
  /* OLD CODE, ABOVE USES ENTITY FRAMEWORK.
  if ($model['#type'] == 'view') {
    // NOTE: this is not implemented yet, see below TODO.
    return FALSE;
  }

  if ($model['#type'] == 'node') {
    // Prepare and convert data for node_save.
    if ($new) {
      unset($data['nid']);
      unset($data['vid']);
      if (!$data['uid']) {
        global $user;
        $data['uid'] = $user->uid;
      }
    }
    $data['type'] = $model['#node_type']; // Force correct node type.

    _ext_convert_to_cck_fields($data);

    // Do this after _ext_convert_to_cck_fields() as _ext_copy_filefield_files() expects regular CCK field format.
    _ext_copy_filefield_files($data, $new);

    $data = (object) $data; // node_save expects an object.
    $changed_orig = isset($data->changed) ? $data->changed : NULL;

    node_save($data);

    // Convert data back to our format.
    $data = (array) $data;
    _ext_convert_from_cck_fields($data);

    // If the node is new but didn't save.
    if (!$data['nid']) {
      return FALSE;
    }
    $changed_new = db_result(db_query("SELECT changed FROM {node} WHERE nid=%d", $data['nid']));
    // If the node wasn't new and didn't save.
    if ($changed_orig == $changed_new) {
      return FALSE;
    }
    return TRUE;
  }
  elseif ($model['#type'] == 'user') {
    if ($new) {
      unset($data['uid']);
    }
    // user_save expects an object.
    $data = (object) $data;
    $success = user_save($data);
    $data = (array) $data;
    return $success;
  }
  elseif ($model['#type'] == 'file') {
    // TODO: Should this be supported somehow?
    return FALSE;
  }
  elseif ($model['#type'] == 'comment') {
    // Unideally (see comments in ext_ext_access_model_data()) we have to check
    // for permission to post the comment to the specified node here.
    // This is done in the comment_save function.
    if ($new) {
      unset($data['cid']);
    }
    return comment_save($data);
  }
  */
}


/**
 * Implementation of hook_ext_validate_model_data.
 */
function ext_ext_validate_model_data($data, $model_name) {
  $model = ext_get_models($model_name);
  if ($model['#type'] == 'node') {
    // node_validate expects an object.
    $data = (object) $data;
    _ext_convert_from_cck_fields($model, $data);
    node_validate($data);
    return _ext_extract_form_errors();
  }
  elseif ($model['#type'] == 'comment') {
    comment_validate($data);
    return _ext_extract_form_errors();
  }
  return array();
}


/**
 * Implementation of hook_ext_delete_model_data.
 */
function ext_ext_delete_model_data($model_instance_id, $model_name) {
  $model = ext_get_models($model_name);
  if (isset($model['#entity'])) {
    return entity_delete($model['#entity'], $model_instance_id);
    //TODO special support for comment delete (see below)?
  }
  
  /* OLD CODE, ABOVE USES ENTITY FRAMEWORK.
  if ($model['#type'] == 'node') {
    node_delete($model_instance_id);
    return db_result(db_query("SELECT COUNT(*) FROM {node} WHERE nid=%d", $model_instance_id)) == 0;
  }
  elseif ($model['#type'] == 'user') {
    user_delete(array(), $model_instance_id);
    return db_result(db_query("SELECT COUNT(*) FROM {users} WHERE uid=%d", $model_instance_id)) == 0;
  }
  elseif ($model['#type'] == 'comment') {
    // Below taken from comment_confirm_delete_submit().
    // Delete comment and its replies.
    $comment = _comment_load($model_instance_id);
    _comment_delete_thread($comment);
    _comment_update_node_statistics($comment->nid);
    // Clear the cache so an anonymous user sees that his comment was deleted.
    cache_clear_all();
  }
  */
}


/**
 * Implementation of hook_ext_access_store_data.
 */
function ext_ext_access_store_data($store_name, $options, $account) {
  $store = ext_get_stores($store_name);
if ($store['#type'] == 'node') {
    // Individual nodes are checked for access permissions in ext_ext_load_store_data().
    return user_access('access content');
  }
  if ($store['#type'] == 'user') {
    return user_access('administer users');
  }
  if (isset($store['#entity'])) {
    return entity_access('view', $store['#entity'], NULL, $account);
  }
  elseif ($store['#type'] == 'view') {
    list($model, $view_name, $display_name) = _ext_unpack_view_model($store_name);
    if ($view_name) {
      $view = views_get_view($view_name);
      if ($view) {
        return $view->access($display_name, $account);
      }
    }
  }
  return FALSE;
}


/**
 * Implementation of hook_ext_load_store_data.
 */
/*
 function ext_ext_load_store_data($store_name, $options) {
$store = ext_get_stores($store_name);
$limit = isset($options['limit']) ? $options['limit'] : 0;
$start = isset($options['start']) ? $options['start'] : 0;

if ($store['#type'] == 'node' || $store['#type'] == 'file' || $store['#type'] == 'comment') {
$node_store = $store['#type'] == 'node';

$model = ext_get_models($store['model']);
$base_table = $model['#db_base_table'];
$join_tables_type = array();
$join_tables_field = array();
$conditions = array();
$arguments = array();
$sorts = array();

// If we're loading data for a node store for a specific node type.
if ($node_store && $store['model'] != 'Node') {
// Filter on node type for this Model.
$conditions[] = "n.type = '%s'";
$arguments[] = $model['#node_type'];
}

if (isset($options['filter'])) {
// Turn the conditions into a query.
foreach ($options['filter'] as $filter) {
$field_def = $model['fields'][$filter['property']];
$table = $field_def['#db_table'];
$column = $field_def['#db_column'];

if ($table != $base_table) {
// Currently we only have joins for nodes (node_revisions and CCK tables).
$join_tables_type[$table] = $table == 'node_revisions' ? 'LEFT JOIN' : 'INNER JOIN';
$join_tables_field[$table] = 'vid';
}

// Some of below is taken from http://www.levihackwith.com/extjs-4-parsing-filters-from-the-filterfeature-using-php/
// TODO: Some of it doesn't seem to apply and should be checked out
// (eg the 'comparison', 'anyMatch' and 'caseSensitive' properties).
if (isset($filter['comparison'])) {
if ($filter['comparison'] == 'lt')
  $comparator = '<';
elseif ($filter['comparison'] == 'gt')
$comparator = '>';
else
  $comparator = '=';
}
else {
$comparator = '=';
}

$value = $filter['value'];
$table_column = "{{$table}}.$column";
$field_def['type'] = isset($field_def['type']) ? $field_def['type'] : 'string';
switch($field_def['type']) {
case 'boolean':
$place_holder = "%b";
break;
case 'int':
$place_holder = "%d";
break;
case 'float':
$place_holder = "%f";
break;
case 'date':
$place_holder = "'%s'";
break;
case 'string':
default: // Assume string/date
//'anyMatch' not passed through?
//if (!empty($filter['anyMatch'])) {
$place_holder = "'%%%s%%'";
// Perhaps we should be using RegExs here, Ext seems to assume a regex for strings/filters?
$comparator = 'LIKE';
//}
//else {
//  $place_holder = "'%s'";
//}

// Assume case sensitive unless explicitly set otherwise.
//'caseSensitive' not passed through?
//if (isset($filter['caseSensitive']) && !$filter['caseSensitive']) {
//  $table_column = "UPPER($table_column)";
//  $place_holder = "UPPER($place_holder)";
//}
break;
}
$conditions[] = "$table_column $comparator $place_holder";
$arguments[] = $value;
}
}

if (isset($options['sort'])) {
// Turn the conditions into a query.
foreach ($options['sort'] as $sort) {
$field_def = $model['fields'][$sort['property']];
$direction = isset($sort['direction']) ? $sort['direction'] : 'ASC';

$table = $field_def['#db_table'];
$column = $field_def['#db_column'];
if ($table != $base_table) {
$join_tables_type[$table] = $table == 'node_revisions' ? 'LEFT JOIN' : 'INNER JOIN';
}

$sorts[] = "{{$table}}.$column $direction";
}
}

$primary_id = $model['idProperty'];
$base_table_alias = substr($base_table, 0, 1);
$query = "SELECT $base_table_alias.$primary_id AS $primary_id FROM {$base_table} $base_table_alias";
foreach ($join_tables_type as $table => $join) {
$field = $join_tables_field[$table];
$query .= " $join {{$table}} ON $base_table_alias.$field = {{$table}}.$field";
}
if (!empty($conditions)) {
$query .= " WHERE ". implode(" AND ", $conditions);
}
if (!empty($sorts)) {
$query .= " ORDER BY ". implode(', ', $sorts);
}

// Add access filters (and perhaps other stuff).
$query = db_rewrite_sql($query, $base_table_alias, $primary_id);

if ($limit > 0) {
$results = db_query_range($query, $arguments, $start, $limit);
}
else {
$results = db_query($query, $arguments);
}
$ids = array();
while ($data = db_fetch_object($results)) {
$ids[] = $data->$primary_id;
}
return $ids;
}

elseif ($store['#type'] == 'user') {
// TODO: Add user query code. Have to deal with stupid profile fields.
}

elseif ($store['#type'] == 'view') {
// Determine object type of the view.
list($model, $view_name, $display_name) = _ext_unpack_view_model($store_name);

// Unset these params so there's no chance of interfering with the View
// arguments (these params are automatically produced by Ext).
foreach (array('q', '_dc', 'limit', 'start', 'page') as $key) {
unset($options[$key]);
}

$view = views_get_view($view_name);
$view->set_display($display_name);
$view->init_display();
$arguments = array_keys($view->display_handler->get_option('arguments'));
$args = array();
$filters = array();
if (isset($options['filter'])) {
foreach ($options['filter'] as $filter) {
$filters[$filter['property']] = $filter['value'];
}
}
foreach ($arguments as $key) {
$value = 'all';
if (isset($filters[$key])) {
$value = is_array($filters[$key]) ? implode('+', $filters[$key]) : $filters[$key];
}
$args[] = $value;
}
$view->set_arguments($args);
$view->pre_execute();
$view->pager['items_per_page'] = $limit;
$view->pager['offset'] = $start;
$view->pager['use_pager'] = FALSE;
$view->execute();

$return = array();
$data = array();
foreach ($view->result as $row) {
$data[] = (array) $row;
}
unset($view); // Conserve some memory.

// If a View based Model is being used (when display row style is 'fields').
if ($model['#type'] == 'view') {
// Just use raw result data from the View.
$return = $data;
}
else {
// Get the ID of the base object/table type the View.
foreach ($data as $row) {
$return[] = $row[$store['#primaryID']];
}
}
}

return $return;
}
*/

function ext_ext_load_store_data($store_name, $options) {
  $store = ext_get_stores($store_name);
  $limit = isset($options['limit']) ? $options['limit'] : 0;
  $start = isset($options['start']) ? $options['start'] : 0;

  /* Quick Test */
  //$entity_query_2 = new EntityFieldQuery();
  //$entity_query_2->entityCondition('entity_type', 'profile2', '=');
  //$result2 = $entity_query_2->execute();

  if (isset($store['#entity'])) {
    $model = ext_get_models($store['model']);
    
    if( isset($options['#EntityFieldQuery'])) {
      $entity_query = $options['#EntityFieldQuery'];
    }
    else
      $entity_query = new EntityFieldQuery();
    
    $entity_query->entityCondition('entity_type', $store['#entity'], '=');
    if (isset($store['#bundle'])) {
      $entity_query->entityCondition('bundle', $model['#bundle'], '=');
    }
    if (isset($options['filter'])) {
      foreach ($options['filter'] as $filter) {
        $field_def = $model['fields'][$filter['property']];
        if ($field_def['#db_table'] == $model['#db_base_table']) {
          $entity_query->propertyCondition($filter['property'], $filter['value']);
        }
        else {
          $entity_query->fieldCondition($field_def['#content_field_name'], $field_def['#content_column'], $filter['value']);
        }
      }
    }
    /*
      $node_store = $store['#type'] == 'node';

      $model = ext_get_models($store['model']);
      $base_table = $model['#db_base_table'];
      $join_tables_type = array();
      $join_tables_field = array();
      //$conditions = array();
      //$arguments = array();
      $sorts = array();

      // If we're loading data for a node store for a specific node type.
      if ($node_store && $store['model'] != 'Node') {
        // Filter on node type for this Model.
        //$conditions[] = "n.type = '%s'";
        //$arguments[] = $model['#node_type'];
        $entity_query->entityCondition('bundle', $model['#node_type']);
      }
    }
    
    if (isset($options['filter'])) {
      // Turn the conditions into a query.
      foreach ($options['filter'] as $filter) {
        $field_def = $model['fields'][$filter['property']];
        $table = $field_def['#db_table'];
        $column = $field_def['#db_column'];

        if ($table != $base_table) {
          // Currently we only have joins for nodes (node_revisions and CCK tables).
          $join_tables_type[$table] = $table == 'node_revisions' ? 'LEFT JOIN' : 'INNER JOIN';
          $join_tables_field[$table] = 'vid';
        }

        // Some of below is taken from http://www.levihackwith.com/extjs-4-parsing-filters-from-the-filterfeature-using-php/
        // TODO: Some of it doesn't seem to apply and should be checked out
        // (eg the 'comparison', 'anyMatch' and 'caseSensitive' properties).
        if (isset($filter['comparison'])) {
          if ($filter['comparison'] == 'lt')
            $comparator = '<';
          elseif ($filter['comparison'] == 'gt')
          $comparator = '>';
          else
            $comparator = '=';
        }
        else {
          $comparator = '=';
        }

        $value = $filter['value'];

        // treat all filter as fields
        $entity_query->fieldCondition($table, $column, $value, $comparator);
      }
    }
    */

    if (isset($options['sort'])) {
      foreach ($options['sort'] as $sort) {
        $field_def = $model['fields'][$sort['property']];
        $direction = isset($sort['direction']) ? $sort['direction'] : 'ASC';
        if ($field_def['#db_table'] == $model['#db_base_table']) {
          $entity_query->propertyOrderBy($sort['property'], $direction);
        }
        else {
          $entity_query->fieldOrderBy($field_def['#content_field_name'], $field_def['#content_column'], $direction);
        }
      }
    }

    /*$primary_id = $model['idProperty'];
     $base_table_alias = substr($base_table, 0, 1);
    $query = "SELECT $base_table_alias.$primary_id AS $primary_id FROM {$base_table} $base_table_alias";
    foreach ($join_tables_type as $table => $join) {
    $field = $join_tables_field[$table];
    $query .= " $join {{$table}} ON $base_table_alias.$field = {{$table}}.$field";
    }*/


    /*if (!empty($conditions)) {
     $query .= " WHERE ". implode(" AND ", $conditions);
    }
    if (!empty($sorts)) {
    $query .= " ORDER BY ". implode(', ', $sorts);
    }*/

    // Add access filters (and perhaps other stuff).
    //$query = db_rewrite_sql($query, $base_table_alias, $primary_id);

    if ($limit > 0) {
      //$results = db_query_range($query, $arguments, $start, $limit);
      $entity_query->range($start,$limit);
    }
    /*else {
     $results = db_query($query, $arguments);
    }*/
    /*$ids = array();
     while ($data = db_fetch_object($results)) {
    $ids[] = $data->$primary_id;
    //return $ids;

    }*/
    $result = $entity_query->execute();
    if (isset($result[$store['#entity']]))
      return array_keys($result[$store['#entity']]);
    else
      return NULL;
  }

  elseif ($store['#type'] == 'view') {
    // Determine object type of the view.
    list($model, $view_name, $display_name) = _ext_unpack_view_model($store_name);

    // Unset these params so there's no chance of interfering with the View
    // arguments (these params are automatically produced by Ext).
    foreach (array('q', '_dc', 'limit', 'start', 'page') as $key) {
      unset($options[$key]);
    }

    $view = views_get_view($view_name);
    $view->set_display($display_name);
    $view->init_display();
    $arguments = array_keys($view->display_handler->get_option('arguments'));
    $args = array();
    $filters = array();
    if (isset($options['filter'])) {
      foreach ($options['filter'] as $filter) {
        $filters[$filter['property']] = $filter['value'];
      }
    }
    foreach ($arguments as $key) {
      $value = 'all';
      if (isset($filters[$key])) {
        $value = is_array($filters[$key]) ? implode('+', $filters[$key]) : $filters[$key];
      }
      $args[] = $value;
    }
    $view->set_arguments($args);
    $view->pre_execute();
    $view->pager['items_per_page'] = $limit;
    $view->pager['offset'] = $start;
    $view->pager['use_pager'] = FALSE;
    $view->execute();

    $return = array();
    $data = array();
    foreach ($view->result as $row) {
      $data[] = (array) $row;
    }
    unset($view); // Conserve some memory.

    // If a View based Model is being used (when display row style is 'fields').
    if ($model['#type'] == 'view') {
      // Just use raw result data from the View.
      $return = $data;
    }
    else {
      // Get the ID of the base object/table type the View.
      foreach ($data as $row) {
        $return[] = $row[$store['#primaryID']];
      }
    }
  }

  return $return;
}

/**
 * Given Model instance data for a 'node' Model type, convert from the
 * "separate Model field for each column of each CCK field" format to the
 * normal CCK field format as used in a node object (ie from
 * "data[field name_column name][delta] = value" format to
 * "data[field name][delta][column name] = value" format).
 * @param $data The Model instance data.
 *
 * Migration history:
 * D6 => D7: Remove _content_type_info
 */
function _ext_convert_to_cck_fields($model, &$data) {
  if (!isset($model['#entity']) || !isset($model['#bundle'])) return;
  
  $lang = empty($data['language']) ? LANGUAGE_NONE : $data['language'];
  
  // Construct CCK field value columns from separate fields.
  foreach (field_info_instances($model['#entity'], $model['#bundle']) as $instance) {
    $field = field_info_field_by_id($instance['field_id']);
    $field_name = $instance['field_name'];

    $data[$field_name] = array($lang => array());
    $columns = array_keys($field['columns']);

    // Hack to handle the case where no file ID is defined for a file field 
    // (we just remove the field completely from the entity).
    $no_file_fid = FALSE;
    if ($field['type'] == 'file') {
      $no_file_fid = empty($data[$field_name. '_fid']);
    }

    foreach ($columns as $col) {
      // Get values as array.
      $values = ($field['cardinality'] != 1) ? $data[$field_name. '_'. $col] : array($data[$field_name. '_'. $col]);
      // $values can still be not an array in the case that the field allows
      // multiple values, but the Model instance field is empty or not set
      // (eg because the node is new).
      if (is_array($values)) {
        foreach ($values as $delta => $value) {
          if (!$no_file_fid) { // Not relevant for non-file fields, is always true for them.
            $data[$field_name][$lang][$delta][$col] = $value;
          }
        }
      }
      unset($data[$field_name. '_'. $col]);
    }
  }
}


/**
 * Given a node object as an array, convert to the
 * "separate Model field for each column of each CCK field" format from the
 * normal CCK field format as used in a node object (ie to
 * "data[field name_column name][delta] = value" format from
 * "data[field name][delta][column name] = value" format).
 *
 * Migration history:
 * D6 => D7: Remove _content_type_info
 */
function _ext_convert_from_cck_fields($model, &$data) {
  if (!isset($model['#entity']) || !isset($model['#bundle'])) return;
  
  $lang = empty($data['language']) ? LANGUAGE_NONE : $data['language'];
  
  // Extract CCK field value columns into separate fields.
  foreach (field_info_instances($model['#entity'], $model['#bundle']) as $instance) {
    $field = field_info_field_by_id($instance['field_id']);
    $field_name = $instance['field_name'];
    $columns = array_keys($field['columns']);

    $field_data_set = isset($data[$field_name]) && is_array($data[$field_name]) && isset($data[$field_name][$lang]) && is_array($data[$field_name][$lang]);
    foreach ($columns as $col) {
      $values = array();
      if ($field_data_set) {
        $values = _ext_extract_values_from_sub_arrays_by_key($col, $data[$field_name][$lang]);
      }
      if ($field['cardinality'] != 1) {
        $data[$field_name. '_'. $col] = $values;
      }
      elseif (!empty($values)) {
        $data[$field_name. '_'. $col] = $values[0];
      }
      else {
        $data[$field_name. '_'. $col] = NULL;
      }
    }
    unset($data[$field_name]);
  }
}


function _ext_gen_name($type, $part1 = NULL, $part2 = NULL) {
  switch ($type) {
    case 'node':
      return 'Node'. _ext_camelcase($part1);
    case 'view':
      return 'View'. _ext_camelcase($part1. 'VDN_'. $part2);
  }
}


function _ext_unpack_view_model($model_name, $instance_data = NULL) {
  list($view_name, $display_name) = explode('VDN', substr($model_name, 4));
  $view_name = _ext_uncamelcase($view_name);
  $display_name = _ext_uncamelcase($display_name);
  // See if a "custom" Model was defined for this view and display.
  $model = ext_get_models($model_name);
  if (!$model) {
    // Determine "standard" Model from view.
    $view = views_get_view($view_name);
    $model_name = _ext_base_table_to_model_name($view->base_table);
    if ($model_name == 'Node' && $instance_data != NULL) {
      // Determine content type specific Model name.
      $model_name = _ext_gen_name('node', $instance_data->type);
    }
    $model = ext_get_models($model_name);
  }
  return array($model, $view_name, $display_name);
}


function _ext_get_node_model($nid) {
  $name = 'Node';
  $node = node_load($nid);
  if ($node) {
    $name = _ext_gen_name('node', $node->type);
  }
  return ext_get_models($name);
}


function _ext_base_table_to_model_name($table) {
  static $table_to_model = array(
      'node' => 'Node',
      'users' => 'User',
      'comments' => 'Comment',
      'file_managed' => 'File',
      'term_data' => 'Term',
  );
  return $table_to_model[$table];
}

// Determine primary ID field from schema for a table.
function _ext_table_primary_id($table) {
  $schema = drupal_get_schema($table);
  if (!is_array($schema)) {
    return FALSE;
  }
  foreach ($schema['fields'] as $field_name => $field) {
    if ($field['type'] == 'serial') {
      return $field_name;
    }
  }
  return FALSE;
}

function _ext_db_schema_field_to_ext_field($field_name, $field, $db_table=NULL) {
  $ext_field = array(
      'name' => $field_name,
  );

  $type = _ext_db_schema_field_type_to_ext_field_type($field['type']);
  if ($type) $ext_field['type'] = $type;

  if (isset($field['default'])) {
    $ext_field['defaultValue'] = $field['default'];
  }

  if ($db_table != NULL) {
    $ext_field['#db_table'] = $db_table;
    $ext_field['#db_column'] = $field_name;
  }
  return $ext_field;
}

function _ext_db_schema_field_type_to_ext_field_type($type) {
  static $type_map = array(
      'serial' => 'int',
      'int' => 'int',
      'float' => 'float',
      'numeric' => 'float',
      'varchar' => 'string',
      'char' => 'string',
      'text' => 'string',
      'blob' => 'string',
      'datetime' => 'date',
  );
  if (array_key_exists( $type, $type_map))
    return $type_map[$type];
  return FALSE;
}


/**
 * This is used to extract errors set by form validation functions.
 * This is useful when validating model data using such functions.
 */
function _ext_extract_form_errors() {
  foreach (form_get_errors() as $key => $error) {
    $field = array_pop(explode('][', $key));
    $errors[$field] = $error; // Should already be translated.
  }
  form_set_error(NULL, NULL, TRUE); // Clear form errors.
  drupal_get_messages(); // Clear message queue.
  return $errors;
}