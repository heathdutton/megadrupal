<?php

/**
 * @file
 * Administration pages for configuring services client module.
 */

/**
 * Settings page callback
 */
function services_client_settings() {
  $form = array();

  $form['services_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID'),
    '#description' => t('Services client id of current site'),
    '#default_value' => variable_get('services_client_id', drupal_get_token('services_client')),
    '#size' => "70",
  );

  $form['services_client_use_queue'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Queue'),
    '#default_value' => variable_get('services_client_use_queue', FALSE),
    '#description' => t('Use queue for processing new objects coming from remote sites.'),
  );

  $form['services_client_process_queue_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Process Queue items in cron'),
    '#default_value' => variable_get('services_client_process_queue_cron', FALSE),
    '#description' => t('Process queue items in cron'),
    '#states' => array(
      'visible' => array(
        ':input[name="services_client_use_queue"]' => array('checked' => TRUE),
      )
    ),
  );

  $form['services_client_exclude_users'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude users'),
    '#default_value' => variable_get('services_client_exclude_users', '1'),
    '#description' => t('Enter which users should not be synced, comma separated i.e. 1,3,475,9949')
  );

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 98,
  );

  $form['#pre_render'][] = 'vertical_tabs_form_pre_render';

  return system_settings_form($form);
}

/**
 * Create new mapping wizard.
 */
function services_client_wizard($step = NULL) {
  $form_info = array(
    'id' => 'services_client_wizard',
    'path' => "admin/structure/services_client/wizard/%step",
    'show trail' => TRUE,
    'show back' => TRUE,
    'show return' => FALSE,
    'finish callback' => 'services_client_wizard_form_finish',
    'cancel callback' => 'services_client_wizard_form_cancel',
    'order' => array(
      'connection' => t('Connection'),
      'entity' => t('Entity selector'),
      'mapping' => t('Field mapping'),
    ),
    'forms' => array(
      'connection' => array(
        'form id' => 'services_client_wizard_form_details'
      ),
      'entity' => array(
        'form id' => 'services_client_wizard_form_entity'
      ),
      'mapping' => array(
        'form id' => 'services_client_wizard_form_mapping'
      ),
    ),
  );

  if (empty($step)) {
    $step = 'connection';
  }

  ctools_include('wizard');
  $form_state = array();
  $form = ctools_wizard_multistep_form($form_info, $step, $form_state);
  $output = drupal_render($form);

  return $output;
}

/**
 * Cancel wizard action. Clear selected values.
 */
function services_client_wizard_form_cancel($form, &$form_state) {
  unset($_SESSION['services_client_wizard']);
}

/**
 * Finish form button.
 */
function services_client_wizard_form_finish(&$form_state) {
  unset($_SESSION['services_client_wizard']);
}

/**
 * Form step 1; Initial event details.
 */
function services_client_wizard_form_details($form, &$form_state) {
  // Initialize empty object.
  $_SESSION['services_client_wizard'] = new stdClass;

  $form['connection'] = array(
    '#type' => 'select',
    '#title' => t('Connection'),
    '#options' => array_map(function ($item) {
      return $item->name;
    }, services_client_connection_load_all()),
    '#description' => t('Select remote connection where will be new object mapped. !add_connection', array(
      '!add_connection' => l(t('Add connection'), 'admin/structure/services_client/connection/add', array('query' => drupal_get_destination())),
    )),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Enter event administrative name.'),
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#machine_name' => array(
      'exists' => function ($name) {
        ctools_include('export');
        return ctools_export_crud_load('services_client_connection_event', $name);
      },
      'source' => array('title'),
    ),
  );

  $form['handler'] = array(
    '#type' => 'select',
    '#title' => t('Handler'),
    '#options' => services_client_get_plugins('event_handler', TRUE, function ($item) {
      return $item['type'] == 'save';
    }),
  );

  return $form;
}

/**
 * Try to retrieve remote entities definition.
 */
function services_client_wizard_form_details_validate($form, &$form_state) {
  try {
    $connection = services_client_connection_get($form_state['values']['connection']);
    $entities = $connection->action('services_client', 'entity_info');
    // Store info in global object.
    $object = &$_SESSION['services_client_wizard'];
    $object->connection = $form_state['values']['connection'];
    $object->entities = $entities;
    $object->handler = $form_state['values']['handler'];
    $object->title = $form_state['values']['title'];
    $object->name = $form_state['values']['name'];
  }
  catch (ServicesClientConnectionResponseException $e) {
    form_set_error('', 'Error response from remote connection when trying to retrieve list of remote entities');
    drupal_set_message(t('You have to install and enable services_client_services on remote endpoint.'), 'error');
    drupal_set_message($e->getMessage(), 'error');
  }
}

/**
 * Form step 2; Entity selector.
 */
function services_client_wizard_form_entity($form, &$form_state) {
  // Make shortcut to have more readable code.
  $object = &$_SESSION['services_client_wizard'];

  // General ajax wrapper
  $form['#prefix'] = '<div id="wizard-entity-wrapper">';
  $form['#suffix'] = '</div>';

  $local_entities = entity_get_info();
  // Local entity selector
  $form['local_entity'] = array(
    '#type' => 'select',
    '#title' => t('Local entity'),
    '#options' => array_map(function ($item) {
      return $item['label'];
    }, $local_entities),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'services_client_wizard_form_ajax',
      'wrapper' => 'wizard-entity-wrapper',
    ),
  );

  if (!empty($form_state['values']['local_entity']) && !empty($local_entities[$form_state['values']['local_entity']]['bundle keys'])) {
    $form['local_bundle'] = array(
      '#type' => 'select',
      '#title' => t('Local bundle'),
      '#options' => array_map(function ($item) {
        return $item['label'];
      }, $local_entities[$form_state['values']['local_entity']]['bundles']),
    );
  }

  // Remote entity selector
  $form['remote_entity'] = array(
    '#type' => 'select',
    '#title' => t('Remote entity'),
    '#options' => array_map(function ($item) {
      return $item['name'];
    }, $object->entities),
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'services_client_wizard_form_ajax',
      'wrapper' => 'wizard-entity-wrapper',
    ),
  );

  if (!empty($form_state['values']['remote_entity']) && !empty($object->entities[$form_state['values']['remote_entity']]['bundles'])) {
    $form['remote_bundle'] = array(
      '#type' => 'select',
      '#title' => t('Remote bundle'),
      '#options' => $object->entities[$form_state['values']['remote_entity']]['bundles'],
    );
  }

  return $form;
}

/**
 * Form step 2; Submit handler;
 */
function services_client_wizard_form_entity_submit($form, &$form_state) {
  $object = &$_SESSION['services_client_wizard'];
  $object->local_entity = $form_state['values']['local_entity'];
  $object->local_bundle = !empty($form_state['values']['local_bundle']) ? $form_state['values']['local_bundle'] : NULL;
  $object->remote_entity = $form_state['values']['remote_entity'];
  $object->remote_bundle = !empty($form_state['values']['remote_bundle']) ? $form_state['values']['remote_bundle'] : NULL;
}

/**
 * Form step 3; Mapping.
 */
function services_client_wizard_form_mapping($form, &$form_state) {
  // General ajax wrapper
  $form['#prefix'] = '<div id="wizard-entity-wrapper">';
  $form['#suffix'] = '</div>';

  // Determine no of avaialble rows
  $form_state['rows'] = isset($form_state['rows']) ? $form_state['rows'] : 1;

  // Display selector in table
  $form['mapping'] = array(
    '#theme' => 'services_client_mapping_rows',
    '#tree' => TRUE,
  );

  for ($i = 0; $i < $form_state['rows']; $i++) {
    $form['mapping'][$i] = services_client_wizard_form_mapping_row($form, $form_state, $i);
  }

  $form['add_row'] = array(
    '#type' => 'submit',
    '#value' => t('Add mapping'),
    '#submit' => array('services_client_wizard_form_mapping_add_row'),
    '#ajax' => array(
      'callback' => 'services_client_wizard_form_ajax',
      'wrapper' => 'wizard-entity-wrapper',
    ),
  );

  return $form;
}

/**
 * Build mapping row form entry.
 *
 * @param int $row
 *   Row id.
 *
 * @return array
 */
function services_client_wizard_form_mapping_row($form, &$form_state, $row) {
  // Make shortcut to have more readable code.
  $object = &$_SESSION['services_client_wizard'];

  // Build local select box options.
  $local_options = array('' => '- ' . t('None') . ' -');
  if (module_exists('entity')) {
    $properties = entity_get_property_info($object->local_entity);
    foreach ($properties['properties'] as $name => $info) {
      $local_options['property:' . $name] = t('Property: @label', array('@label' => $info['label']));
    }
  }
  else {
    drupal_set_message(t('Please install entity module to get list of available properties.'), 'warning', FALSE);
  }
  // Filter available entity fields.
  $fields = array_filter(field_info_fields(), function ($item) use ($object) {
    return isset($item['bundles'][$object->local_entity]);
  });
  if (!empty($object->local_bundle)) {
    $fields = array_filter($fields, function ($item) use ($object) {
      return in_array($object->local_bundle, $item['bundles'][$object->local_entity]);
    });
  }
  foreach ($fields as $field_name => $field_info) {
    foreach (array_keys($field_info['columns']) as $column) {
      $local_options['field:' . $field_name . ':' . $column] = t('Field: @name:@column', array('@name' => $field_name, '@column' => $column));
    }
  }

  // Build remote select box options.
  $remote_options = array('' => '- ' . t('None') . ' -');
  foreach ($object->entities[$object->remote_entity]['properties'] as $name => $label) {
    $remote_options['property:' . $name] = t('Property: @label', array('@label' => $label));
  }
  foreach ($object->entities[$object->remote_entity]['fields'] as $name => $info) {
    $show_field = TRUE;
    if (!empty($object->remote_bundle) && !in_array($object->remote_bundle, $info['bundles'])) {
      $show_field = FALSE;
    }

    if ($show_field) {
      foreach ($info['columns'] as $column) {
        $remote_options['field:' . $name . ':' . $column] = t('Field: @field:@column', array('@field' => $name, '@column' => $column));
      }
    }
  }

  return array(
    '#tree' => TRUE,
    'local' => array(
      '#type' => 'select',
      '#title' => t('Local field'),
      '#options' => $local_options,
    ),
    'remote' => array(
      '#type' => 'select',
      '#title' => t('Remote field'),
      '#options' => $remote_options,
    ),
  );
}

/**
 * Submit handler; Build event and save it to DB.
 */
function services_client_wizard_form_mapping_submit($form, &$form_state) {
  // Make shortcut to have more readable code.
  $object = &$_SESSION['services_client_wizard'];

  // Create new event object.
  ctools_include('export');
  $event = ctools_export_crud_new('services_client_connection_event');
  $event->connection = $object->connection;
  $event->name = $object->name;
  $event->title = $object->title;
  $event->entity_type = $object->local_entity;
  $event->event = 'save';
  $event->plugin = $object->handler;

  $handler = $event->getHandler();

  // Add property condition
  if (!empty($object->local_bundle)) {
    $entity_info = entity_get_info($object->local_entity);
    $uuid = $handler->addPlugin('condition', 'ServicesClientPropertyCondition');
    $handler->setPluginConfig('condition', $uuid, array(
      'property' => $entity_info['bundle keys']['bundle'],
      'value' => $object->local_bundle,
      'condition' => 'equals',
    ));
  }

  // Add mapping
  foreach ($form_state['values']['mapping'] as $row) {
    $local = explode(':', $row['local']);
    $remote = explode(':', $row['remote']);

    $reader = $formatter = '';
    $reader_config = $formatter_config = array();

    // Field reader
    if ($local[0] == 'field') {
      $reader = 'ServicesClientFieldReader';
      $reader_config = array('field' => $local[1], 'property' => $local[2], 'language' => LANGUAGE_NONE, 'all_values' => TRUE);
    }
    // Property reader
    elseif ($local[0] == 'property') {
      $reader = 'ServicesClientPropertyReader';
      $reader_config = array('property' => $local[1]);
    }

    // Field formatter
    if ($remote[0] == 'field') {
      $formatter = 'ServicesClientFieldFormatter';
      $formatter_config = array(
        'field' => $remote[1],
        'language' => LANGUAGE_NONE,
        'property' => $remote[2],
        'multivalue' => 'all_values',
        'empty' => 'null_field',
        'default_value' => '',
      );
    }
    // Property formatter
    elseif ($remote[0] == 'property') {
      $formatter = 'ServicesClientPropertyFormatter';
      $formatter_config = array(
        'property' => $remote[1],
        'multivalue' => 'force_single',
        'empty' => 'no_field',
        'default_value' => '',
      );
    }

    // If both are configured.
    if (!empty($reader) && !empty($formatter)) {
      $uuid = $handler->addPlugin('mapping', 'ServicesClientMappingPlugin');
      $handler->setPluginConfig('mapping', $uuid, array(
        'reader' => $reader, 'reader_config' => $reader_config,
        'formatter' => $formatter, 'formatter_config' => $formatter_config,
      ));
    }
  }

  // Save event and put to disabled state.
  $event = $handler->save();
  $event = ctools_export_crud_load('services_client_connection_event', $event->name);
  $event->table = 'services_client_connection_event';
  ctools_export_crud_set_status($event->table, $event, TRUE);

  // Go to configuratin page.
  drupal_set_message(t('Event was saved, please review, finish configuration and enable event.'), 'status');
  $form_state['redirect'] = $handler->getUrl('configure');
}

/**
 * Submit handler; Add new row.
 */
function services_client_wizard_form_mapping_add_row($form, &$form_state) {
  $form_state['rows']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX handler; Rebuild form.
 */
function services_client_wizard_form_ajax($form, &$form_state) {
  return $form;
}
