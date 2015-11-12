<?php

// TODO: Add child/parent association functionality (using which ever
// association type was selected by user).

// TODO: Provide Tree model for Taxonomy Terms (once we have a basic term
// Model, see todo in ../includes/ext.ext.inc).


/**
 * Implementation of hook_ext_model_types.
 */
function ext_tree_ext_model_types() {
  $types = array();
  $types['treenode'] = array(
    'user_created' => TRUE,
  );
  return $types;
}


/**
 * Implementation of hook_ext_models.
 */
function ext_tree_ext_models() {
  $models = array();

  $tree_models = ext_tree_get_model_settings();
  foreach ($tree_models as $model_name => $settings) {
    // Get base Model definition.
    $base_model = ext_get_models($settings['base_model']);

    // Make sure base Model still exists and the parent association is configured.
    if ($base_model && isset($settings['association'])) {
      $model = array(
        '#type' => 'treenode',
        '#extends' => $settings['base_model'],
        'idProperty' => $base_model['idProperty'],
        'fields' => array(),
        'associations' => array(),
        'validations' => array(),
      );

      // Add parent association definition.
      $model['associations'][] = $settings['association'];

      // If any field mappings have been configured.
      if (isset($settings['field_options'])) {
        $base_field_names = _ext_extract_values_from_sub_arrays_by_key('name', $base_model['fields'], 'value');
        $model['fieldSyncMap'] = array(); // Defined in model.DrupalModel.
        // Set-up client-side synchronisation between mapped fields.
        foreach ($settings['field_options'] as $node_field => $model_field) {
          // If a mapping has been set and the base Model doesn't define a field
          // of the same name as the node field.
          if ($model_field !== '0' && !isset($base_field_names[$node_field])) {
            // Add the tree node field to the Model.
            $model['fields'][] = array(
              'name' => $node_field,
            );

            // Synchronise fields in both directions.
            $model['fieldSyncMap'][$node_field] = $model_field;
            $model['fieldSyncMap'][$model_field] = $node_field;
          }
        }
      }

      $models[$model_name] = $model;
    }
  }

  return $models;
}


/**
 * Implementation of hook_ext_access_model_data.
 */
function ext_tree_ext_access_model_data($model_instance_id, $model_name, $op, $account) {
  $settings = ext_tree_get_model_settings($model_name);
  // Access is determined from the base Model.
  return ext_access_model_data($model_instance_id, $settings['base_model'], $op, $account);
}


/**
 * Implementation of hook_ext_load_model_data.
 */
function ext_tree_ext_load_model_data($model_instance_id, $model_name, $options) {
  $settings = ext_tree_get_model_settings($model_name);
  $data = ext_load_model_data($model_instance_id, $settings['base_model'], $options);
  return $data;
}


/**
 * Implementation of hook_ext_save_model_data.
 */
function ext_tree_ext_save_model_data(&$data, $model_name, $new = FALSE) {
  $settings = ext_tree_get_model_settings($model_name);
  return ext_save_model_data($data, $settings['base_model'], $new);
}


/**
 * Implementation of hook_ext_validate_model_data.
 */
function ext_tree_ext_validate_model_data(&$data, $model_name) {
  $settings = ext_tree_get_model_settings($model_name);
  return ext_validate_model_data($data, $settings['base_model']);
}


/**
 * Implementation of hook_ext_delete_model_data.
 */
function ext_tree_ext_delete_model_data($model_instance_id, $model_name) {
  $settings = ext_tree_get_model_settings($model_name);
  return ext_delete_model_data($model_instance_id, $settings['base_model']);
}



/**
 * Implementation of hook_ext_store_types.
 */
function ext_tree_ext_store_types() {
  $types = array();
  $types['tree'] = array(
    'user_created' => TRUE,
  );
  $types['tree_base_model_store'] = array(
    'user_created' => FALSE,
  );
  return $types;
}


/**
 * Implementation of hook_ext_stores.
 */
function ext_tree_ext_stores() {
  $stores = array();

  // Create Tree Stores.
  $tree_stores = ext_tree_get_store_settings();
  foreach ($tree_stores as $store_name => $settings) {
    $model = ext_get_models($settings['model']);
    // Use parent association that was generated from base Model parent association.
    $assoc = _ext_get_sub_array_by_value_for_member($model['associations'], '#tree_node_parent', TRUE);

    $store = array(
      '#type' => 'tree',
      '#class' => 'Ext.data.TreeStore',
      '#proxy_reader_root' => $assoc['#reverse_of'],
      'model' => $settings['model'],
      'nodeParam' => $model['idProperty'],
      '#proxy_reader_root' => $assoc['#reverse_of'],
    );
    $stores[$store_name] = $store;
  }

  // Create normal Stores based on existing Stores for Models that tree node Models extend.
  // This allows loading tree node models for inherited associations and the like.
  $tree_model_settings = ext_tree_get_model_settings();
  // Get all Models that tree node Models extend (use as a base Model),
  // generate mapping from extended Model name to tree node Model names.
  $extended_model_to_tree_models = array();
  foreach ($tree_model_settings as $tree_model_name => $tree_model_settings) {
    $base_model = $tree_model_settings['base_model'];
    if (!isset($extended_model_to_tree_models[$base_model])) {
      $extended_model_to_tree_models[$base_model] = array();
    }
    $extended_model_to_tree_models[$base_model][] = $tree_model_name;
  }

  //We can not call _ext_get_stores_for_model() here as it may result in an infinite loop.
  //Thus we must invoke hook_ext_stores to get all the defined Stores from other modules.
  foreach (module_implements('ext_stores') as $module) {
    if ($module != 'ext_tree') {
      $result = call_user_func($module .'_ext_stores');
      if (isset($result) && is_array($result)) {
        foreach ($result as $store_name => $store) {
          $store_model = $store['model'];
          // If the store is for a model that a tree node model extends.
          if (isset($extended_model_to_tree_models[$store_model])) {
            foreach ($extended_model_to_tree_models[$store_model] as $tree_model_name) {
              // If a store for this tree node model has not been found yet, or this is a generic store (which we prefer).
              if (!isset($stores[$tree_model_name. 'Store']) || (isset($store['#generic']) &&  $store['#generic'])) {
                $treestore = array(
                  'model' => $tree_model_name,
                  '#type' => 'tree_base_model_store',
                  '#backing_store' => $store_name,
                  '#generic' => isset($store['#generic']) ? $store['#generic'] : FALSE,
                  '#filterable_fields' => isset($store['#filterable_fields']) ? $store['#filterable_fields'] : array(),
                  '#sortable_fields' => isset($store['#sortable_fields']) ? $store['#sortable_fields'] : array(),
                );
                $stores[$tree_model_name. 'Store'] = $treestore;
              }
            }
          }
        }
      }
    }
  }

  return $stores;
}


/**
 * Implementation of hook_ext_access_store_data.
 */
function ext_tree_ext_access_store_data($store_name, $options, $account) {
  // Access is determined by whether the account has access to the root node Model.
  $root_id = _ext_tree_get_store_root_node_id($store_name);
  $settings = ext_tree_get_store_settings($store_name);
  return ext_access_model_data($root_id, $settings['model'], 'view', $account);


  $store = ext_get_stores($store_name);
  if ($store['#type'] == 'tree') {
    // Access is determined by whether the account has access to the root node Model.
    $root_id = _ext_tree_get_store_root_node_id($store_name);
    if ($root_id !== FALSE) {
      $settings = ext_tree_get_store_settings($store_name);
      return ext_access_model_data($root_id, $settings['model'], 'view', $account);
    }
  }
  elseif ($store['#type'] == 'tree_base_model_store') {
    // Pass request on to backing Store.
    return ext_access_store_data($store['#backing_store'], $options, $account);
  }
  return FALSE;
}


/**
 * Implementation of hook_ext_load_store_data.
 */
function ext_tree_ext_load_store_data($store_name, $options) {
  $store = ext_get_stores($store_name);
  if ($store['#type'] == 'tree') {
    $model = ext_get_models($store['model']);

    // Use parent association that was generated from base Model parent association.
    $assoc = _ext_get_sub_array_by_value_for_member($model['associations'], '#tree_node_parent', TRUE);
    // #reverse_of gives the associationKey of the reverse association.
    $children_assoc = $assoc['#reverse_of'];

    // This request could be coming from a recursive ext_load_model_data call
    // (to load nested associated Model data), so check for the max depth(s)
    // (ignore if not an array (not ideal, really)).
    $options['load_nested_max_depth'] = (isset($options['load_nested_max_depth']) && is_array($options['load_nested_max_depth'])) ? $options['load_nested_max_depth'] : array();

    $parent_id = $options[$model['idProperty']] ;
    if ($parent_id == 0) {
      // Ext sends id=0 when loading children for the auto-generated root
      // node (which we would generally make invisible in the tree panel).
      // So we just load the root tree node and send that as the child.
      $root_id = _ext_tree_get_store_root_node_id($store_name);
      if ($root_id) {
        // Set depth of children to include.
        $options['load_nested_max_depth'][$children_assoc] = 1;
        return array(ext_load_model_data($root_id, $store['model'], $options));
      }
    }
    else {
      // Set depth of children to include (get grand-children).
      $options['load_nested_max_depth'][$children_assoc] = 2;
      // Load parent model data, which will children model data from the children assocation.
      $parent_data = ext_load_model_data($parent_id, $store['model'], $options);
      $children = $parent_data[$children_assoc];
      return $children;
    }
  }
  elseif ($store['#type'] == 'tree_base_model_store') {
    // Pass request on to Store for the Model that the tree node Model extends.
    // This just loads ids so we don't need to worry about Model-specific fields being populated.
    return ext_load_store_data($store['#backing_store'], $options, TRUE);
  }
}
