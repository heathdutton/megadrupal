<?php
/**
 * @file
 * Callback functions for the administrative ui of Shared Content.
 */

function sharedcontent_status_view() {
  module_load_include('inc', 'views', 'includes/admin');
  views_ui_add_admin_css();

  $content = array();

  // Get names of Shared Content enabled connections
  $active_connections = variable_get('sharedcontent_endpoint_connections', array());

  $rows = array();
  foreach (services_client_connection_load_all() as $config) {
    $data = array();
    $class = array();

    $is_active = in_array($config->name, $active_connections);

    $queue_name = 'sharedcontent_sync_' . $config->name;
    $pending_count = DrupalQueue::get($queue_name)->numberOfItems();

    $operations = module_invoke_all('sharedcontent_connection_operations', array(
      'is_active' => $is_active,
      'pending_count' => $pending_count,
      'connection' => $config,
    ));
    drupal_alter('sharedcontent_connection_operations', $operations, $config);

    // Column: Name
    $name = t('<h2>@title (@name)</h2><p class="endpoint">!endpoint</p><p class="description">@description</p>', array(
      '@title' => $config->admin_title,
      '@name' => $config->name,
      '!endpoint' => l($config->endpoint, $config->endpoint),
      '@description' => $config->admin_description,
    ));
    $data[] = array('data' => $name, 'class' => array('name'));
    // Column: Status
    $data[] = array(
      'data' => $is_active ? t('Active') : t('Disabled'),
      'class' => array('status')
    );
    $class[] = $is_active ? 'active' : 'disabled';

    // Column: Last Sync
    $date = variable_get('sharedcontent_last_sync_' . $config->name);
    if ($date) {
      $date = format_date(strtotime($date));
    }
    else {
      $date = '-';
    }
    $data[] = array('data' => $date, 'class' => array('last-sync'));

    // Column: Pending tasks
    $data[] = array(
      'data' => $pending_count,
      'class' => array('pending')
    );

    // Column: Operations
    // Extra actions for the display
    $data[] = array(
      'data' => array(
        '#theme' => 'links__ctools_dropbutton',
        '#attributes' => array(
          'class' => array('horizontal', 'right', 'links', 'actions'),
        ),
        '#links' => $operations,
      ),
      'class' => array('operations'),
    );

    $rows[] = array('data' => $data, 'class' => $class);
  }

  $header = array(
    array('data' => t('Name'), 'class' => 'name'),
    array('data' => t('Status'), 'class' => 'status'),
    array('data' => t('Last sync'), 'class' => 'last-sync'),
    array('data' => t('Pending tasks'), 'class' => 'pending'),
    array('data' => t('Operations'), 'class' => 'operations'),
  );

  $content['connections_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No connections'),
    '#attributes' => array(
      'class' => array('connections-table'),
    ),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'sharedcontent') . '/css/sharedcontent.css',
      ),
    ),
  );

  $singular_msg = 'A task is defined as a package of 1 index record that will be synchronized by cron.';
  $plural_msg = 'A task is defined as a package of @count index records that will be synchronized by cron.';
  $batch_size = variable_get('sharedcontent_sync_batch_size', SHAREDCONTENT_CLIENT_DEFAULT_SYNC_BATCH_SIZE);
  $content['from'] = array(
    '#type' => 'item',
    '#title' => t('Pending Tasks'),
    '#markup' => format_plural($batch_size, $singular_msg, $plural_msg),
  );
  return $content;
}

/**
 * Indexed entities config form
 *
 * Configuration form allowing to select which entity will be indexed.
 */
function sharedcontent_configure_indexed($form, &$form_state) {
  $indexed = variable_get('sharedcontent_indexed_entities', array());
  foreach (entity_get_info() as $entity_type => $entity_values) {
    $fieldset = $entity_type . '_fieldset';
    $form[$fieldset] = array(
      '#type' => 'fieldset',
      '#title' => $entity_values['label'],
    );
    $types = array();
    $defaults = array();

    foreach ($entity_values['bundles'] as $bundle => $values) {
      $defaults[$bundle] = $values['label'];
      $types[$bundle] = $values['label'];
    }

    if (isset($indexed[$entity_type])) {
      $defaults = array_keys(array_filter($indexed[$entity_type]));
    }

    $form[$fieldset][$entity_type] = array(
      '#type' => 'checkboxes',
      '#options' => $types,
      '#default_value' => $defaults,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update indexed entities')
  );

  return $form;
}

/**
 * Submit callback for indexed entities config form
 *
 * @see sharedcontent_configure_indexed()
 */
function sharedcontent_configure_indexed_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $values = array();
  foreach (entity_get_info() as $entity_type => $entity_values) {
    $value = isset($form_state['values'][$entity_type]) ? array_filter($form_state['values'][$entity_type]) : array();
    if (!empty($value)) {
      $values[$entity_type] = $value;
    }
  }
  variable_set('sharedcontent_indexed_entities', $values);
}
