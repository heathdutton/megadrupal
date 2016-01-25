<?php
/**
 * @file
 * Drupal forms.
 */

/**
 * Form definition.
 */
function giantbomb_job_add_form($form, &$form_state) {
  $type_options = array();
  $endpoints = giantbomb_api_endpoints();
  $form['#type_options'] = array();

  foreach ($endpoints as $endpoint => $endpoint_info) {
    $jobs = giantbomb_jobs_all($endpoint);
    $objects = giantbomb_types_all($endpoint, 'id');
    foreach ($objects as $object) {
      $key = $endpoint . '|' . $object->id;
      $form['#type_options'][$key] = array($endpoint, $object->id);
      $type_options[$endpoint_info['title']][$key] = $object->title;
    }
  }

  asort($type_options);
  foreach ($type_options as &$type_option) {
    asort($type_option);
  }

  $default_type = !empty($_GET['type']) ? check_plain($_GET['type']) : '';
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => $type_options,
    '#required' => TRUE,
    '#empty_option' => t('- Select -'),
    '#default_value' => $default_type,
  );

  $default_id = !empty($_GET['id']) ? check_plain($_GET['id']) : '';
  $form['id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID'),
    '#description' => 'Fill only for objects. Leave blank for list jobs.',
    '#size' => 7,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $default_id,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create Job'),
  );

  return $form;
}

/**
 * Form submission callback for giantbomb_job_add_form().
 */
function giantbomb_job_add_form_submit(&$form, &$form_state) {
  $type_raw = $form_state['values']['type'];
  list($endpoint, $type_id) = $form['#type_options'][$type_raw];

  $id = $form_state['values']['id'];
  $resource  = empty($id) ? 'list' : 'object';
  $object_id = empty($id) ? NULL : $id;

  $fields = array(
    'endpoint' => $endpoint,
    'resource' => $resource,
    'type' => $type_id,
    'object_id' => $object_id,
    'settings' => serialize(array()),
  );

  $job_id = db_insert('giantbomb_jobs')
    ->fields($fields)
    ->execute();

  if ($job_id) {
    drupal_set_message(t('Job created.'));
    $form_state['redirect'] = 'admin/config/services/giantbomb/scheduler/job/' . $job_id . '/edit';
  }
}

/**
 * Edit an existing job.
 */
function giantbomb_job_edit_form($form, &$form_state, $job) {
  $form['#job'] = $job;
  $types = giantbomb_types_all($job->endpoint, 'id');
  $job_type = &$types[$job->type];

  $object_options_types = array();
  $update_frequency_options = array(
    43200 => t('12 Hours'),
    86400 => t('1 Day'),
    172800 => t('2 Days'),
    604800 => t('1 Week'),
    1209600 => t('2 Weeks'),
    2419200 => t('1 Month'),
  );

  foreach ($types as $type) {
    $object_options_types[$type->id] = $type->title;
  }
  asort($object_options_types);
  $list_options_types[$job->type] = $job_type->title;

  if ($job->resource == 'object') {
    $form[] = array(
      '#prefix' => '<p>',
      '#markup' => t('The next update field for the entity associated with this object will be ignored.'),
      '#suffix' => '</p>',
    );
  }

  $form['active'] = array(
      '#type' => 'checkbox',
      '#title' => t('Job is active'),
      '#description' => t('Whether this job should run.'),
      '#default_value' => $job->active,
  );

  $form['update_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Update Frequency'),
    '#options' => $update_frequency_options,
    '#description' => t('How often this job should update.'),
    '#default_value' => $job->update_frequency,
  );

  // Lists
  if ($job->resource == 'list') {
    $form['list'] = array(
      '#type' => 'fieldset',
      '#title' => t('Lists'),
      '#tree' => TRUE, 
    );

    $form['list']['related_create'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Create objects in list'),
      '#options' => $list_options_types,
      '#description' => t('Create objects if they do not exist locally.'),
      '#default_value' => isset($job->settings['request']['list_related_create']) ? $job->settings['request']['list_related_create'] : array(),
      '#multiple' => TRUE,
      '#size' => 10,
    );

    $form['list']['related_update'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Update objects in list'),
      '#options' => $list_options_types,
      '#description' => t('Update objects if they exist locally.'),
      '#default_value' => isset($job->settings['request']['list_related_update']) ? $job->settings['request']['list_related_update'] : array(),
      '#multiple' => TRUE,
      '#size' => 10,
    );
  }

  // Objects
  $form['object'] = array(
    '#type' => 'fieldset',
    '#title' => t('Objects'),
    '#tree' => TRUE, 
  );

  if ($job->resource == 'list') {
    $form['object'][]['#markup'] = t('For each object in the list:');
  }

  $form['object']['related_create'] = array(
    '#type' => 'select',
    '#title' => t('Create related objects'),
    '#options' => $object_options_types,
    '#description' => t('Create related objects if they do not exist locally.'),
    '#default_value' => isset($job->settings['request']['object_related_create']) ? $job->settings['request']['object_related_create'] : array(),
    '#multiple' => TRUE,
    '#size' => 10,
  );
  
  $form['object']['related_update'] = array(
    '#type' => 'select',
    '#title' => t('Update related objects'),
    '#options' => $object_options_types,
    '#description' => t('Update related objects if they exist locally.'),
    '#default_value' => isset($job->settings['request']['object_related_update']) ? $job->settings['request']['object_related_update'] : array(),
    '#multiple' => TRUE,
    '#size' => 10,
  );

  $form['object']['relationships_create'] = array(
    '#type' => 'select',
    '#title' => t('Create relationships'),
    '#options' => $object_options_types,
    '#description' => t('Create relationships to these types. Relationships will be created only if the related object exists locally.'),
    '#default_value' => isset($job->settings['request']['object_relationships_create']) ? $job->settings['request']['object_relationships_create'] : array(),
    '#multiple' => TRUE,
    '#size' => 10,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Job'),
  );

  return $form;
}

/**
 * Form submission callback for giantbomb_job_edit_form().
 */
function giantbomb_job_edit_form_submit(&$form, &$form_state) {
  $job = $form['#job'];
  $fields = array();
  $settings = array();

  $fields['update_frequency'] = $form_state['values']['update_frequency'];
  $fields['active'] = (int)$form_state['values']['active'];

  if ($job->resource == 'list') {
    $settings['request']['list_related_create'] = $form_state['values']['list']['related_create'];
    $settings['request']['list_related_update'] = $form_state['values']['list']['related_update'];
  }

  $settings['request']['object_related_create'] = $form_state['values']['object']['related_create'];
  $settings['request']['object_related_update'] = $form_state['values']['object']['related_update'];
  $settings['request']['object_relationships_create'] = $form_state['values']['object']['relationships_create'];

  $fields['settings'] = serialize($settings);

  $result = db_update('giantbomb_jobs')
    ->fields($fields)
    ->condition('id', $job->id)
    ->execute();

  if ($result) {
    drupal_set_message(t('Configuration saved.'));
    $form_state['redirect'] = 'admin/config/services/giantbomb/scheduler';
  }
}

/**
 * Form callback.
 */
function giantbomb_job_delete_form($form, &$form_state, $job) {
  $form['#job'] = $job;
  return confirm_form(
    $form,
    t('Are you sure you want to delete this job?'), 'admin/config/services/giantbomb/scheduler'
  );
}

/**
 * Form submission callback for giantbomb_job_delete_form().
 */
function giantbomb_job_delete_form_submit(&$form, &$form_state) {
  $job = $form['#job'];

  $result = db_delete('giantbomb_jobs')
    ->condition('id', $job->id)
    ->execute();

  if ($result) {
    drupal_set_message(t('Deleted job.'));
  }
  else {
    drupal_set_message(t('Error deleting job.'));
  }

  $form_state['redirect'] = 'admin/config/services/giantbomb/scheduler';
}

/**
 * Search a Giant Bomb compatible endpoint for an object.
 * 
 * Form callback.
 */
function giantbomb_search_form($form, &$form_state) {
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#prefix' => '<div class="container-inline form-wrapper">',
    '#suffix' => '</div>',
  );

  $form['search']['query'] = array(
    '#title' => t('Query'),
    '#type' => 'textfield',
    '#title_display' => 'invisible',
  );

  $endpoints = giantbomb_api_endpoints();
  $endpoint_options = array();
  foreach ($endpoints as $name => $endpoint) {
    $endpoint_options[$name] = $endpoint['title'];
  }

  $form['search']['endpoint'] = array(
    '#options' => $endpoint_options,
    '#type' => 'select',
  );

  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search')
  );

  if (isset($form_state['storage']['query'])) {
    module_load_include('inc', 'giantbomb', 'includes/giantbomb.pages');

    $form['search_result'] = giantbomb_search_result(
      $form_state['storage']['query'],
      $form_state['storage']['endpoint']
    );
  }

  return $form;
}

/**
 * Form submission callback for giantbomb_search_form().
 */
function giantbomb_search_form_submit(&$form, &$form_state) {
  // see drupal_get_form().
  $form_state['rebuild'] = TRUE;
  $form_state['storage']['query']    = $form_state['values']['query'];
  $form_state['storage']['endpoint'] = $form_state['values']['endpoint'];
}

/**
 * Form callback.
 */
function giantbomb_type_edit_settings_form($form, &$form_state, $endpoint_definition, $type_definition) {
  $form['#giantbomb_type'] = $type_definition;
  $form['#giantbomb_endpoint'] = $endpoint_definition['name'];

  $update_frequency_options = array(
    43200 => t('12 Hours'),
    86400 => t('1 Day'),
    172800 => t('2 Days'),
    604800 => t('1 Week'),
    1209600 => t('2 Weeks'),
    2419200 => t('1 Month'),
  );

  $form['update_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Update Frequency'),
    '#options' => $update_frequency_options,
    '#description' => t('After an entity has been created or updated, how long to wait before updating again.'),
    '#default_value' => $type_definition->update_frequency,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission callback for giantbomb_type_edit_settings_form().
 */
function giantbomb_type_edit_settings_form_submit(&$form, &$form_state) {
  $endpoint = $form['#giantbomb_endpoint'];
  $type_definition = $form['#giantbomb_type'];

  $fields = array(
    'update_frequency' => $form_state['values']['update_frequency'],
  );

  $result = db_update('giantbomb_types')
    ->fields($fields)
    ->condition('endpoint', $endpoint)
    ->condition('id', $type_definition->id)
    ->execute();

  if ($result) {
    drupal_set_message(t('Configuration saved.'));
  }
}
/**
 * Form callback.
 * 
 * @param array $form
 * @param array $form_state
 * @param array $endpoint_definition
 *   A fully loaded endpoint definition.
 * @param object $type_definition
 *   A fully loaded Giant Bomb type definition. 
 */
function giantbomb_type_edit_entity_form($form, &$form_state, $endpoint_definition, $type_definition) {
  $form['#giantbomb_type'] = $type_definition;
  $endpoint = $form['#giantbomb_endpoint'] = $endpoint_definition['name'];

  $endpoint_info = giantbomb_api_endpoint_load($endpoint);

  $object = $type_definition->object;
  $entity_type = $type_definition->entity_type;
  $bundle = $type_definition->bundle;

  $options = array();
  $default_value = ($entity_type && $bundle) ? $entity_type . '|' . $bundle : '';

  // Show warning message if bundle is already set.
  $message = '';
  if (!empty($default_value)) {
    // Show this message if not submitted.
    if (empty($form_state['input'])) {
      $existing_object_count = (int)giantbomb_local_entity_count($endpoint, $object, $entity_type, $bundle);
  
      if ($existing_object_count > 0) {
        $message = format_plural(
          $existing_object_count,
          'Changing this will orphan @count existing object, and reset field mappings for this type.',
          'Changing this will orphan @count existing objects, and reset field mappings for this type.'
        );
      }
      // Entities may already be orphaned, or fields have never been set.
      elseif (!empty($type_definition->settings)) {
        $message = t('Changing this will reset field mappings for this type.');
      }
  
      if ($message) {
        drupal_set_message($message, 'warning');
      }
  }
  }

  $entities = entity_get_info();
  foreach ($entities as $entity_type => $entity_info) {
    if ($entity_info['fieldable'] && isset($entity_info['bundles'])) {
      $entity_label = $entity_info['label'];
      foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
        $value = $entity_type . '|' . $bundle;
        $options[$entity_label][$value] = $bundle_info['label'];
      }
    }
  }

  $form['entity_bundle'] = array(
    '#title' => t('Bundle'),
    '#type' => 'select',
    '#options' => $options,
    '#empty_option' => t('-- None --'),
    '#default_value' => $default_value,
    '#description' => t('Object data from @endpoint will be stored in.', array(
      '@endpoint' => isset($endpoint_info['title']) ? $endpoint_info['title'] : '',
    )),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission callback for giantbomb_type_edit_entity_form().
 */
function giantbomb_type_edit_entity_form_submit(&$form, &$form_state) {
  $endpoint = $form['#giantbomb_endpoint'];
  $type = $form['#giantbomb_type'];

  $fields = array(
    'entity_type' => NULL,
    'bundle' => NULL,
  );
  
  $entity_bundle = $form_state['values']['entity_bundle'];

  if (!empty($entity_bundle)) {
    list($fields['entity_type'], $fields['bundle']) = explode('|', $entity_bundle);
  }

  if ($type->entity_type != $fields['entity_type'] || $type->bundle != $fields['bundle']) {
    $fields['settings'] = serialize(array());
  }

  $result = db_update('giantbomb_types')
    ->fields($fields)
    ->condition('endpoint', $endpoint)
    ->condition('id', $type->id)
    ->execute();

  if ($result) {
    drupal_set_message(t('Configuration saved.'));
  }
}

/**
 * Form callback.
 * 
 * @param array $endpoint_definition
 *   A fully loaded endpoint definition.
 * @param object $type_definition
 *   A fully loaded Giant Bomb type definition. 
 */
function giantbomb_type_edit_fields_form($form, &$form_state, $endpoint_definition, $type_definition) {
  $form['#giantbomb_type'] = $type_definition;
  $endpoint = $form['#giantbomb_endpoint'] = $endpoint_definition['name'];
  $type = $type_definition->object;

  $entity_type = $type_definition->entity_type;
  $bundle = $type_definition->bundle;

  if (!$entity_type || !$bundle) {
    return FALSE;
  }

  $instances = field_info_instances($entity_type, $bundle);

  $properties = giantbomb_object_properties($endpoint, $type);
  $property_options = array();
  foreach ($properties as $property_name => $property) {
    $option_text = $property['title'];
    if (!empty($property['required'])) {
      $option_text = t('@option (Required)', array('@option' => $option_text));
    }
    $property_options[$property_name] = $option_text;
  }

  $property_element = array(
    '#type' => 'select',
    '#options' => $property_options,
    '#empty_option' => t('-- None --'),
  );

  $form['entity'] = array(
    '#prefix' => '<h3>' . t('Entity') . '</h3>',
    '#tree' => TRUE,
  );
  
  $entity_properties = array('label' => t('Label'));
  foreach ($entity_properties as $entity_property => $entity_property_label) {
    $form['entity']['label'] = array(
      '#title' => check_plain($entity_property_label),
      '#default_value' => isset($type_definition->settings['entity_mapping'][$entity_property]) ? $type_definition->settings['entity_mapping'][$entity_property] : '',
    ) + $property_element;
  }

  $form['fields'] = array(
    '#prefix' => '<h3>' . t('Fields') . '</h3>',
    '#tree' => TRUE,
  );

  foreach ($instances as $instance) {
    $field_info = field_info_field($instance['field_name']);
    $id = $instance['id'];

    $form['fields'][$id] = array(
      '#title' => t('@label (@type)', array(
        '@label' => $instance['label'],
        '@type' => $field_info['type'],
      )),
      '#type' => 'fieldset',
    );

    foreach (array_keys($field_info['columns']) as $column) {
      $form_id = 'giantbomb_property_' . $id . '_' . $column;
  
      if (isset($form_state['values']['fields'][$id][$column]['property'])) {
        $property_default = $form_state['values']['fields'][$id][$column]['property'];
      }
      else {
        $property_default = isset($type_definition->settings['field_mapping'][$id][$column]['property']) ? $type_definition->settings['field_mapping'][$id][$column]['property'] : '';
      }

      $form['fields'][$id][$column]['#type'] = 'fieldset';
      $form['fields'][$id][$column]['#title'] = check_plain($column);
      $form['fields'][$id][$column]['property'] = array(
        '#title' => t('Property'),
        '#default_value' => $property_default,
        '#ajax' => array(
          'event' => 'change',
          'method' => 'replace',
          'wrapper' => $form_id,
          'callback' => 'giantbomb_type_edit_fields_ajax_property_form',
        ),
      ) + $property_element;

      if ($property_default) {
        $property_class = giantbomb_property_type($properties[$property_default]['data type']);
        if ($property_class) {
          $default_settings = isset($type_definition->settings['field_mapping'][$id][$column]['settings']) ? $type_definition->settings['field_mapping'][$id][$column]['settings'] : array();
          if ($property_form = $property_class::form($form, $form_state, $default_settings)) {
            $form['fields'][$id][$column]['form'] = $property_form;
            $form['fields'][$id][$column]['form']['#tree'] = TRUE;
            $form['fields'][$id][$column]['#title'] = check_plain($column);
          }
        }
      }

      $form['fields'][$id][$column]['form']['#prefix'] = '<div id="' . $form_id . '">';
      $form['fields'][$id][$column]['form']['#suffix'] = '</div>';
    }
  }

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

function giantbomb_type_edit_fields_ajax_property_form(&$form, &$form_state) {
  if (isset($form_state['triggering_element']['#parents']) && count($form_state['triggering_element']['#parents']) >= 3) {
    $parents = $form_state['triggering_element']['#parents'];
    if (isset($form['fields'][$parents[1]][$parents[2]]['form'])) {
      return $form['fields'][$parents[1]][$parents[2]]['form'];
    }
  }
}

/**
 * Form validation callback for giantbomb_type_edit_form().
 */
function giantbomb_type_edit_fields_form_validate(&$form, &$form_state) {
  // ensure endpoint name, type, and object id are set!
  // @todo: offer DO IT FOR ME checkbox. module_exists(field, int, string)
}

/**
 * Form submission callback for giantbomb_type_edit_form().
 */
function giantbomb_type_edit_fields_form_submit(&$form, &$form_state) {
  $type_info = $form['#giantbomb_type'];
  $endpoint = $form['#giantbomb_endpoint'];

  $settings = is_array($type_info->settings) ? $type_info->settings : array();

  // Entity properties
  $settings['entity_mapping'] = array();
  if (isset($form_state['values']['entity']['label'])) {
    $settings['entity_mapping']['label'] = $form_state['values']['entity']['label'];
  }

  // Fields
  $settings['field_mapping'] = array();
  foreach ($form_state['values']['fields'] as $id => $columns) {
    foreach ($columns as $column => $property) {
      if (!empty($property)) {
        $settings['field_mapping'][$id][$column]['property'] = $property['property'];
        if (!empty($property['form'])) {
          $settings['field_mapping'][$id][$column]['settings'] = $property['form'];
        }
      }
    }
  }

  $result = db_update('giantbomb_types')
    ->fields(array('settings' => serialize($settings)))
    ->condition('endpoint', $endpoint)
    ->condition('id', $type_info->id)
    ->execute();

  if ($result) {
    drupal_set_message(t('Configuration saved.'));
  }
}

/**
 * Form callback.
 *
 * @param array $endpoint_definition
 *   A fully loaded endpoint definition.
 * @param object $type_definition
 *   A fully loaded Giant Bomb type definition.
 */
function giantbomb_type_edit_relationships_form($form, &$form_state, $endpoint_definition, $type_info) {
  $form['#giantbomb_type'] = $type_info;
  $endpoint = $form['#giantbomb_endpoint'] = $endpoint_definition['name'];

  $types = giantbomb_types_all($endpoint);

  $relation = array();

  $form['relation'] = array(
    '#title' => t('Relation'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  if (module_exists('relation')) {
    $type_entity_bundle = $type_info->entity_type . ':' . $type_info->bundle;
    $options_relation = array(
    );
    $relation_types = relation_get_types();
    foreach ($types as $a_type) {
      if ($a_type->entity_type && $a_type->bundle) {
        $options = array(':create:' => t('- Create new relation type -'), );
        $a_type_entity_bundle = $a_type->entity_type . ':' . $a_type->bundle;

        foreach ($relation_types as $relation_type) {
          if (in_array($a_type_entity_bundle, $relation_type->source_bundles) && in_array($type_entity_bundle, $relation_type->source_bundles)) {
            $options[$relation_type->relation_type] = $relation_type->label;
          }
        }

        $form['relation'][$a_type->id] = array(
          '#type' => 'select',
          '#title' => check_plain($a_type->title),
          '#options' => $options,
          '#empty_option' => t('- None -'),
          '#default_value' => isset($type_info->settings['relationships']['relation'][$a_type->id]) ? $type_info->settings['relationships']['relation'][$a_type->id] : '',
        );
      }
    }
  }
  else {
    $form['relation'][]['#markup'] = t('You must install and enable Relation module to use its relationships.');
  }

  $form['entityreference'] = array(
    '#title' => t('Entity Reference'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#access' => FALSE, // @todo: remove when ER is supported.
  );
  
  if (module_exists('entityreference')) {
    /*
    foreach ($types as $a_type) {
      if ($a_type->entity_type && $a_type->bundle) {
        $options = array();
        $form['entityreference'][$a_type->id] = array(
          '#type' => 'select',
          '#title' => check_plain($a_type->title),
          '#options' => $options,
          '#empty_option' => t('- None -'),
          '#default_value' => isset($type_info->settings['entityreference']['relation'][$a_type->id]) ? $type_info->settings['entityreference']['relation'][$a_type->id] : '',
        );
      }
    }*/
  }
  else {
    $form['entityreference'][]['#markup'] = t('You must install and enable Entity Reference module to use its relationships.');
  }

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submission callback for giantbomb_type_edit_relationships_form().
 */
function giantbomb_type_edit_relationships_form_submit(&$form, &$form_state) {
  $type_info = $form['#giantbomb_type'];
  $endpoint = $form['#giantbomb_endpoint'];
  $types = giantbomb_types_all($endpoint, 'id');
  $this_entity_bundle = $type_info->entity_type . ':' . $type_info->bundle;

  $settings = is_array($type_info->settings) ? $type_info->settings : array();
  $settings['relationships'] = array();

  if (isset($form_state['values']['relation'])) {
    foreach ($form_state['values']['relation'] as $type_id => $relation_type) {
      $a_type = $types[$type_id];
      if ($relation_type == ':create:') {
        // Create new Relation type
        $relation_type = '';
        $relation_type_translatable = array(
          '@object_source' => $type_info->title,
          '@object_destination' => $a_type->title,
        );
        $new_relation_type = array(
          'relation_type' => 'giantbomb_' . $type_info->object . '_to_' . $a_type->object,
          'label' => t('Giant Bomb: @object_source to @object_destination' , $relation_type_translatable),
          'source_bundles' => array(
            $this_entity_bundle,
            $a_type->entity_type . ':' . $a_type->bundle,
          )
        );
        $relation = relation_type_create($new_relation_type);
        if ($relation) {
          relation_type_save($relation);
          $relation_type = $relation->relation_type;
          drupal_set_message(t('Relation type between @object_source and @object_destination created.', $relation_type_translatable));
        }
        else {
          continue;
        }
      }
      if (!empty($relation_type)) {
        // Prepare settings for this type, to save later.
        $settings['relationships']['relation'][$type_id] = $relation_type;

        // Save settings for $a_type to this type.
        $a_type_settings = $a_type->settings;
        $a_type_settings['relationships']['relation'][$type_info->id] = $relation_type;

        $result = db_update('giantbomb_types')
          ->fields(array('settings' => serialize($a_type_settings)))
          ->condition('endpoint', $endpoint)
          ->condition('id', $a_type->id)
          ->execute();
      }
    }
  }

  $result = db_update('giantbomb_types')
    ->fields(array('settings' => serialize($settings)))
    ->condition('endpoint', $endpoint)
    ->condition('id', $type_info->id)
    ->execute();
  
  if ($result) {
    drupal_set_message(t('Configuration saved.'));
  }
}

/**
 * Form callback.
 */
function giantbomb_type_delete_form($form, &$form_state, $endpoint_definition, $type_definition) {
  $endpoint = $form['#giantbomb_endpoint'] = $endpoint_definition['name'];
  $form['#giantbomb_type_id'] = $type_definition->id;
  $endpoint_info = giantbomb_api_endpoint_load($endpoint);
  $tvars = array(
    '@endpoint' => $endpoint_info['title'],
  );
  return confirm_form(
    $form,
    t('Are you sure you want to delete this type?'), 'admin/config/services/giantbomb/types/manage/' . $endpoint,
    t('Entity and field mappings for this type will be deleted. This type, but not its settings, may be restored next time @endpoint types are refreshed.', $tvars)
  );
}


/**
 * Form submission callback for giantbomb_type_delete_form().
 */
function giantbomb_type_delete_form_submit(&$form, &$form_state) {
  $endpoint = $form['#giantbomb_endpoint'];
  $type_id = $form['#giantbomb_type_id'];
  $endpoint_info = giantbomb_api_endpoint_load($endpoint);

  $result = db_delete('giantbomb_types')
    ->condition('endpoint', $endpoint)
    ->condition('id', $type_id)
    ->execute();

  if ($result) {
    drupal_set_message(t('Deleted @endpoint type.', array(
      '@endpoint' => $endpoint_info['title'],
    )));
  }
  else {
    drupal_set_message(t('Error deleting @endpoint type.', array(
      '@endpoint' => $endpoint_info['title'],
    )));
  }

  $form_state['redirect'] = 'admin/config/services/giantbomb/types/manage/' . $endpoint;
}

/**
 * Form callback.
 */
function giantbomb_type_reload_form($form, $form_state) {
  return confirm_form(
    $form,
    t('Are you sure you want to update all types?'), 'admin/config/services/giantbomb/types/',
    t('All available endpoints will be queried for types. New types are created automatically.'),
    t('Refresh')
  );
}

/**
 * Form submission callback for giantbomb_type_edit_form().
 */
function giantbomb_type_reload_form_submit(&$form, &$form_state) {
  $endpoints = giantbomb_api_endpoints();
  $endpoint_options = array();
  foreach ($endpoints as $name => $endpoint) {
    $result = giantbomb_types_update($name);
    $t = array(
      '@endpoint' => $endpoint['title'],
      '@error' => isset($result->error) ? $result->error : 'Fatal error',
    );
    if ($result->status_code == GIANTBOMB_API_CODE_OK) {
      drupal_set_message(t('Updated @endpoint types.', $t));
    }
    else {
      drupal_set_message(t('Failed to update @endpoint types: @error', $t), 'error');
    }
  }

  $form_state['redirect'] = 'admin/config/services/giantbomb/types/';
}