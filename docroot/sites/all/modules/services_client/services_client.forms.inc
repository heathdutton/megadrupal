<?php

/**
 * Event common configuration form.
 *
 * @see EntityHandler::configForm.
 *
 * @param EventHandler $event
 *   Event handler that is configured.
 */
function services_client_event_config($form, &$form_state, $event) {
  $form_state['event'] = $event;

  if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    // Show warning that configuration isn't saved.
    if ($event->isChanged()) {
      drupal_set_message(t('This event has been changed. To store changes click on "Save" button on this screen.'), 'warning');
    }

    // Show edit lock warning if exists
    if (($lock = $event->getEditLock())) {
      $account = user_load($lock->uid);
      drupal_set_message(t('This event is being edited by user !user, and is therefore locked from editing by others. This lock is !age old. Click here to <a href="!break">break this lock</a>.', array(
        '!user' => theme('username', array('account' => $account)),
        '!age' => format_interval(REQUEST_TIME - $lock->updated),
        '!break' => url($event->getUrl('break-lock')),
      )), 'warning');

      $form['actions']['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#action' => 'cancel',
      );

      return $form;
    }
  }

  // Run config form function
  $event->configForm($form, $form_state);

  // Add actions.
  $form['actions'] = array(
    '#weight' => 300,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#action' => 'save',
  );

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#action' => 'cancel',
  );

  return $form;
}

/**
 * Submit handler for the plugin config form
 */
function services_client_event_config_submit($form, &$form_state) {
  // Clear the object cache.
  $form_state['event']->clearObjectCache();

  // If "Cancel" was clicked do nothing.
  if ($form_state['clicked_button']['#action'] == 'cancel') {
    drupal_set_message(t('Configuration has been cancelled.'));
  }
  // Store updated event configuration.
  else {
    $form_state['event']->configFormSubmit($form, $form_state);
    $result = $form_state['event']->save();
    drupal_set_message(t('Configuration has been saved.'));
  }

  // To base URL.it
  $form_state['redirect'] = 'admin/structure/services_client';
}

/**
 * Form; Add new plugin.
 *
 * @param EventHandler $event
 *   Event handler that is configured.
 *
 * @param string $type
 *   Type of added plugin. (condition, mapping)
 */
function services_client_plugin_add($form, &$form_state, $event, $type) {
  $form_state += array(
    'event' => $event,
    'type' => $type,
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Plugin'),
    '#options' => services_client_get_plugins('condition', TRUE),
    '#required' => TRUE,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#action' => 'save',
  );

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#action' => 'cancel',
  );

  return $form;
}

/**
 * Submit handler; Add plugin and redirect to plugin config page.
 */
function services_client_plugin_add_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#action'] == 'cancel') {
    $form_state['redirect'] = $form_state['event']->getUrl('configure');
  }
  else {
    $uuid = $form_state['event']->addPlugin($form_state['type'], $form_state['values']['type']);
    $form_state['event']->setObjectCache();
    $form_state['redirect'] = $form_state['event']->getUrl('plugin/' . $form_state['type'] . '/' . $uuid . '/edit');
  }
}

/**
 * Form; Configure plugin form.
 *
 * @param EventHandler $event
 *   Event handler that is configured.
 *
 * @param string $type
 *   Type of added plugin. (condition, mapping)
 *
 * @param string $uuid
 *   UUID identifier.
 *
 * @param ServicesClientConfiguragleInterface $plugin
 *   Plugin that is configured.
 */
function services_client_plugin_edit($form, &$form_state, $event, $type, $uuid, $plugin) {
  // Set plugins
  $form_state += array(
    'type' => $type,
    'event' => $event,
    'plugin' => $plugin,
    'uuid' => $uuid,
  );

  $plugin->configForm($form, $form_state);

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#action' => 'submit',
  );

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#action' => 'cancel',
  );

  return $form;
}

/**
 * Validation handler;
 */
function services_client_plugin_edit_validate($form, &$form_state) {
  $form_state['plugin']->configFormValidate($form, $form_state);
}

/**
 * Submit handler; Save plugin configuration.
 */
function services_client_plugin_edit_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#action'] == 'cancel') {
    // Do nothing here.
  }
  else {
    $form_state['plugin']->configFormSubmit($form, $form_state);
    $form_state['event']->setPluginConfig($form_state['type'], $form_state['uuid'], $form_state['plugin']->getConfiguration());
    $form_state['event']->setObjectCache();
  }
  $form_state['redirect'] = $form_state['event']->getUrl('configure');
}

/**
 * Define the preset add/edit form.
 */
function services_client_ctools_export_ui_form(&$form, &$form_state) {
  $event = &$form_state['item'];

  // Get all available connections
  $connections = array_map(function ($item) {
    return !empty($item->admin_title) ? $item->admin_title : $item->name;
  }, services_client_connection_load_all());

  $form['connection'] = array(
    '#type' => 'select',
    '#title' => t('Connection'),
    '#description' => t('Choose the Connection for which to this hook is valid.'),
    '#options' => $connections,
    '#default_value' => !empty($event->connection) ? $event->connection : NULL,
    '#required' => TRUE,
  );

  // Get all entity types
  $entity_types = array_map(function ($item) {
    return $item['label'];
  }, entity_get_info());

  $form['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity'),
    '#description' => t('Choose the entity type on which should react.'),
    '#options' => $entity_types,
    '#default_value' => $event->entity_type,
    '#required' => TRUE,
  );

  $event->event = !empty($event->event) ? $event->event : 'save';

  $form['event'] = array(
    '#type' => 'select',
    '#title' => t('Event'),
    '#options' => array(
      'save' => t('Save'),
      'delete' => t('Delete'),
    ),
    '#default_value' => $event->event,
    '#description' => t('What event should plugin react to.'),
    '#ajax' => array(
      'callback' => 'services_client_ctools_export_ui_form_ajax',
      'wrapper' => 'plugin-wrapper'
    ),
  );

  // Filter handler plugins by event type
  $event_type = isset($form_state['values']['event']) ? $form_state['values']['event'] : $event->event;
  $form['plugin_wrapper'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array('id' => 'plugin-wrapper'),
  );

  $form['plugin_wrapper']['plugin'] = array(
    '#type' => 'select',
    '#title' => t('Handler'),
    '#options' => services_client_get_plugins('event_handler', TRUE, function ($item) use ($event_type) {
      return $item['type'] == $event_type;
    }),
    '#description' => t('Select which plugin will be processing local data and sending to remote connection.'),
    '#required' => TRUE,
    '#default_value' => $event->plugin
  );
}

/**
 * Ajax callback.
 */
function services_client_ctools_export_ui_form_ajax($form, &$form_state) {
  return $form['plugin_wrapper'];
}

/**
* Submit handler for the preset edit form.
*/
function services_client_ctools_export_ui_form_submit($form, &$form_state) {
  $item = &$form_state['item'];
  $item->entity_type = $form_state['values']['entity_type'];
  $item->event = $form_state['values']['event'];

  // Set new events disabled by default
  if (empty($item->eid)) {
    // Transfer data from the form to the $item based upon schema values.
    $schema = ctools_export_get_schema($form_state['plugin']['schema']);
    foreach (array_keys($schema['fields']) as $key) {
      if(isset($form_state['values'][$key])) {
        $item->{$key} = $form_state['values'][$key];
      }
    }
    $item->table = $form_state['plugin']['schema'];
    ctools_export_crud_set_status($item->table, $item, TRUE);
    drupal_set_message(t('New event was added as disabled. Please configure event before enabling it.'), 'status');
  }
}

/**
 * General ajax handler to use ajax in plugin config forms.
 *
 * @see ServicesClient
 */
function services_client_plugin_mapping_ajax($form, &$form_state) {
  return $form['wrapper'];
}

/**
 * AJAX submit handler; Add new row to user roles configuration.
 */
function services_client_plugin_mapping_role_add_row($form, &$form_state) {
  $form_state['user_roles_count']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * AJAX callback; User configuration.
 */
function services_client_plugin_mapping_role_ajax($form, &$form_state) {
  return $form['user_config'];
}

/**
 * AJAX submit handler; Refresh remote roles.
 */
function services_client_plugin_mapping_roles_refresh($form, &$form_state) {
  $form_state['event']->refreshRemoteRoles();
  $form_state['rebuild'] = TRUE;
}

/**
 * Form; Confirm breaking edit lock.
 */
function services_client_event_break_lock($form, &$form_state, $event) {
  $form_state['event'] = &$event;
  $form = array();

  if (!($lock = $event->getEditLock())) {
    $form['message']['#markup'] = t('There is no lock on event %name to break.', array('%name' => $event->getEvent()->name));
    return $form;
  }

  $cancel = $event->getUrl('configure');
  if (!empty($_REQUEST['cancel'])) {
    $cancel = $_REQUEST['cancel'];
  }

  $account = user_load($lock->uid);
  return confirm_form($form,
    t('Are you sure you want to break the lock on event %name?',
    array('%name' => $event->getEvent()->name)),
    $cancel,
    t('By breaking this lock, any unsaved changes made by !user will be lost!', array('!user' => theme('username', array('account' => $account)))),
    t('Break lock'),
    t('Cancel'));
}

/**
 * Submit handler; Break edit lock.
 */
function services_client_event_break_lock_submit($form, &$form_state) {
  $event = $form_state['event'];
  $event->breakEditLock();
  $form_state['redirect'] = $event->getUrl('configure');
  drupal_set_message(t('The lock has been broken and you may now edit this event.'));
}
