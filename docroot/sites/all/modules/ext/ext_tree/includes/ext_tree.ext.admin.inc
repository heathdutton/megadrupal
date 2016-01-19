<?php


/**
 * Implementation of hook_ext_model_addedit_form().
 */
function ext_tree_ext_model_addedit_form(&$form_state, $model_type, $model_name=NULL) {
  $form = array();
  $form['settings'] = array(
    '#tree' => TRUE,
  );

  // If a new tree node Model is being added, choose the Model to base it on.
  if ($model_name == NULL) {
    $options = array();
    $models = ext_get_models();
    foreach ($models as $base_model_name => $base_model) {
      if ($base_model['#type'] != 'treenode' && !empty($base_model['associations'])) {
        // To be used as a Tree node Model the Model must be able to reference
        // itself in order to form a hierarchy, using a 'belongsTo' association
        // type that also has a corresponding 'hasMany' association.
        $self_reference = FALSE;
        foreach ($base_model['associations'] as $assoc) {
          if ($assoc['model'] == $base_model_name && $assoc['type'] == 'belongsTo' && !empty($assoc['#reverse_of'])) {
            $self_reference = TRUE;
            break;
          }
        }
        if ($self_reference) {
          $options[$base_model_name] = $base_model_name;
        }
      }
    }
    $form['settings']['base_model'] = array(
      '#type' => 'select',
      '#title' => t("Base Model"),
      '#description' => t("A Tree node Model is based on an existing Model. Once you have selected a Model to base it on you can configure the Tree node Model settings. The list of Models that can be used contains only existing non-Tree node Models, and Models that contain an association referencing the same Model (ie the base Model must have a field which can reference another instance of the base Model, using a belongsTo (parent) association, in order to construct a tree hierarchy)."),
      '#options' => $options,
    );
  }
  // Otherwise edit the existing tree node Model.
  else {
    $settings = ext_tree_get_model_settings($model_name);
    $base_model = ext_get_models($settings['base_model']);

    $form['settings']['base_model'] = array(
      '#type' => 'value',
      '#value' => $settings['base_model'],
    );

    $associations = array();
    foreach ($base_model['associations'] as $assoc) {
      // If self-referential association of type 'belongsTo' that has a reverse 'hasMany' association.
      if ($assoc['type'] == 'belongsTo' && $assoc['model'] == $settings['base_model'] && !empty($assoc['#reverse_of'])) {
        $associations[$assoc['associationKey']] = $assoc['associationKey'];
      }
    }
    $form['settings']['association'] = array(
      '#type' => 'select',
      '#title' => t("Parent association"),
      '#description' => t("Select the association that is used to specify the parent of a tree node. This association will be used to construct the tree hierarchy. Only associations that reference the base Model type and which are of the type 'belongsTo' (indicating a 'parent' type relationship), and that have a reverse 'hasMany' association (in order to generate lists of children) are shown."),
      '#options' => $associations,
      '#default_value' => ($settings && isset($settings['association']) ? $settings['association'] : NULL),
    );

    // Collect all fields from the base Model.
    $field_names = _ext_extract_values_from_sub_arrays_by_key('name', $base_model['fields'], 'value');
    array_unshift($field_names, '<'. t('None'). '>');

    // Get configurable Ext tree node options.
    $options = ext_tree_node_options();
    $form['settings']['field_options'] = array(
      '#type' => 'fieldset',
      '#title' => t("Field to tree node option mapping"),
      '#description' => t("You can select which fields from the base Model to use for various properties of the tree node. When an instance of the tree node Model is loaded the mapped values from the base Model will be used to populate the tree node fields, and when an instance of the tree node Model is saved the (possibly modified) values from the tree node Model will be copied back into the mapped base Model fields. It is recommended that at least the 'text' option be specified. Refer to the Ext documentation for details for the tree node fields: !link", array('!link' => l('http://docs.sencha.com/ext-js/4-0/#!/api/Ext.data.NodeInterface', 'http://docs.sencha.com/ext-js/4-0/#!/api/Ext.data.NodeInterface'))),
    );
    foreach ($options as $option) {
      $form['settings']['field_options'][$option] = array(
        '#type' => 'select',
        '#title' => $option, // Don't localise/translate as this is the config property for an Ext class.
        '#options' => $field_names,
        '#default_value' => ($settings && isset($settings['field_options'][$option])) ? $settings['field_options'][$option] : NULL,
        // Don't allow mapping tree node fields that are already defined in
        // the base Model.
        '#enabled' => !isset($field_names[$option]),
      );
    }
  }

  return $form;
}

/**
 * Implementation of hook_ext_model_addedit_form_validate().
 */
function ext_tree_ext_model_addedit_form_validate($form, &$form_state, $model_type, $model_name=NULL) {

}

/**
 * Implementation of hook_ext_model_addedit_form_submit().
 */
function ext_tree_ext_model_addedit_form_submit($form, &$form_state, $model_type, $model_name) {
  $settings = $form_state['values']['settings'];
  // Get existing tree node Model settings.
  $tree_models = ext_tree_get_model_settings();

  // If this is a new tree node Model redirect to the edit page so the user
  // can configure it.
  if (!isset($tree_models[$model_name])) {
    $form_state['redirect'] = "admin/build/ext/models/edit/treenode/$model_name";
  }
  else {
    // Generate parent association definition from selected association.
    $base_model = ext_get_models($settings['base_model']);
    $associationKey = $settings['association'];
    $base_assoc = _ext_get_sub_array_by_value_for_member($base_model['associations'], 'associationKey', $settings['association']);
    $assoc = array(
      'model' => $model_name,
      'foreignKey' => $base_assoc['foreignKey'],
      'primaryKey' => $base_assoc['primaryKey'],
      'type' => $base_assoc['type'],
      '#tree_node_parent' => TRUE, // Mark it as ours.
    );
    $settings['association'] = $assoc;
  }

  $tree_models[$model_name] = $settings;
  variable_set('ext_tree_models', $tree_models);
}


/**
 * Implementation of hook_ext_model_delete().
 */
function ext_tree_ext_model_delete($model_type, $model_name) {
  $tree_models = ext_tree_get_model_settings();
  unset($tree_models[$model_name]);
  variable_set('ext_tree_models', $tree_models);
}


/**
 * Implementation of hook_ext_store_addedit_form().
 */
function ext_tree_ext_store_addedit_form(&$form_state, $store_type, $store_name=NULL) {
  $settings = FALSE;
  if ($store_name != NULL) {
    $settings = ext_tree_get_store_settings($store_name);
  }

  $form = array();
  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $options = array();
  $models = ext_get_models();
  foreach ($models as $model_name => $model) {
    if ($model['#type'] == 'treenode') {
      $options[$model_name] = $model_name;
    }
  }
  $form['settings']['model'] = array(
    '#type' => 'select',
    '#title' => t("Select the tree node Model to use"),
    '#options' => $options,
    '#default_value' => $settings ? $settings['model'] : NULL,
  );

  $form['settings']['root_id'] = array(
    '#type' => 'textarea',
    '#title' => "Root Model ID",
    '#description' => t("Enter the ID of the Model instance to use as the root node of the tree."),
    '#default_value' => $settings ? $settings['root_id'] : NULL,
  );
  $form['settings']['root_id_format'] = array(
    '#type' => 'select',
    '#title' => "Root Model ID format",
    '#description' => t("You can enter either an ID in plain text or enter PHP code to determine the ID of the root node Model. Do not incude &lt;?php ?&gt; delimeters for the PHP option."),
    '#options' => array(
      'plain' => t("Plain"),
      'php' => t("PHP"),
    ),
    '#default_value' => $settings ? $settings['root_id_format'] : 'plain',
  );

  return $form;
}

/**
 * Implementation of hook_ext_store_addedit_form_validate().
 */
function ext_tree_ext_store_addedit_form_validate($form, &$form_state, $store_type, $store_name=NULL) {

}

/**
 * Implementation of hook_ext_store_addedit_form_submit().
 */
function ext_tree_ext_store_addedit_form_submit($form, &$form_state, $store_type, $store_name) {
  $tree_stores = ext_tree_get_store_settings();
  $tree_stores[$store_name] = $form_state['values']['settings'];
  variable_set('ext_tree_stores', $tree_stores);
}


/**
 * Implementation of hook_ext_store_delete().
 */
function ext_tree_ext_store_delete($store_type, $store_name) {
  $tree_stores = ext_tree_get_store_settings();
  unset($tree_stores[$store_name]);
  variable_set('ext_tree_stores', $tree_stores);
}




/*
function theme_ext_tree_admin_model_addedit_form_models($form) {
  $header = array(t('Model'));
  $rows = array();
  $first_row = TRUE;
  foreach (element_children($form) as $key_model) {
    $row = array($form[$key_model]['#title']);
    foreach (element_children($form[$key_model]) as $key_el) {
      if ($first_row) {
        $header[] = $form[$key_model][$key_el]['#title'];
      }
      unset($form[$key_model][$key_el]['#title']);
      unset($form[$key_model][$key_el]['#description']);
      $row[] = drupal_render($form[$key_model][$key_el]);
    }
    $rows[] = $row;
    $first_row = FALSE;
  }

  return theme('table', $header, $rows);
}
*/
