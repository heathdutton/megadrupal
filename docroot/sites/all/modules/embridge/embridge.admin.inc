<?php

/**
 * @file
 * Defines admin config. screens and functionality for the embridge module.
 */

/**
 * General admin settings form for embridge.
 */
function embridge_settings() {

  // Add JS for entermedia.
  drupal_add_js(drupal_get_path('module', 'embridge') . '/js/embridge.js');
  drupal_add_css(drupal_get_path('module', 'embridge') . '/css/embridge.css');

  $form = array();
  $default_host = variable_get('embridge_server_url', 'http://entermedia.databasepublish.com');
  $default_port = variable_get('embridge_server_port', '8080');
  $default_user = variable_get('embridge_login', 'admin');
  $default_password = variable_get('embridge_password', 'admin');

  $headers = array(
    array('data' => t('Hostname'), 'colspan' => 2),
    t('Port'),
    t('User'),
    t('Status'),
  );

  $class = _embridge_server_status($default_host, $default_port, $default_user, $default_password) ? 'ok' : 'error';
  $rows[] = array(
    'data' => array(
      // Cells
      array(
        'class' => 'status-icon',
        'data' => '<div title="' . $class . '"><span class="element-invisible">' . $class . '</span></div>',
      ),
      array(
        'class' => 'default-environment',
        'data'  => l($default_host, $default_host . ':' . $default_port, array('attributes' => array('target' => '_blank'))),
      ),
      $default_port,
      $default_user,
      $class,
    ),
    'class' => array(drupal_html_class($class)),
  );
  $form['server_host_status'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#attributes' => array('class' => array('admin-embridge')),
    '#weight' => -50,
  );
  // Server information.
  $form['server_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Connection information'),
    '#description' => t('Information to connect to EnterMedia server.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 0,
  );
  $form['server_info']['embridge_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Hostname'),
    '#size' => 100,
    '#required' => TRUE,
    '#description' => t('EnterMedia Hostname (e.g. http://entermedia.databasepublish.com).'),
    '#default_value' => $default_host,
  );
  $form['server_info']['embridge_server_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#size' => 7,
    '#required' => TRUE,
    '#description' => t('EnterMedia server port (e.g. 8080).'),
    '#default_value' => $default_port,
  );
  $form['server_info']['embridge_login'] = array(
    '#type' => 'textfield',
    '#title' => t('Login'),
    '#required' => TRUE,
    '#description' => t('Login for EnterMedia service.'),
    '#default_value' => $default_user,
  );
  $form['server_info']['embridge_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => TRUE,
    '#description' => t('Password for EnterMedia service.'),
    '#attributes' => array('value' => $default_password),
  );
  $form['save_connection'] = array(
    '#type' => 'submit',
    '#value' => t('Save Connection'),
    '#weight' => 10,
  );
  // User information.
  $form['search_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search information'),
    '#description' => t('Settings for EnterMedia asset search.'),
    '#collapsible' => TRUE,
    '#weight' => 15,
  );
  $form['search_info']['embridge_search_pagesize'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Size'),
    '#description' => t('Page size for EnterMedia search result.'),
    '#default_value' => variable_get('embridge_search_pagesize', 12),
  );
  embridge_catalog_settings($form);
  $form['add_catalog'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Catalog'),
    '#collapsible' => FALSE,
    '#weight' => 25,
  );
  $form['add_catalog']['new_catalog_id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID'),
    '#description' => t('Enter Catalog ID (e.g. media/catalogs/photo).'),
  );
  $form['#validate'][] = '_embridge_settings_validate';
  $form['#submit'][] = '_embridge_settings_submit';

  return system_settings_form($form);
}

/**
 * EnterMedia Catalog Settings.
 */
function embridge_catalog_settings(&$form) {
  global $base_url;
  $catalogs = variable_get('embridge_catalogs', array());
  foreach ($catalogs as $catalog_id => $catalog_name) {
    $form[$catalog_name] = array(
      '#type' => 'fieldset',
      '#title' => $catalog_name . ' Catalog',
      '#description' => t('Search settings in @catalog_name Catalog', array('@catalog_name' => $catalog_name)),
      '#collapsible' => TRUE,
      '#prefix' => '<div id="catalog-' . $catalog_name .'-wrapper" >',
      '#suffix' => '</div>',
      '#weight' => 20,
    );
    $form[$catalog_name]['embridge_' . $catalog_name . '_remove'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove Catalog'),
      '#weight' => 1,
    );
    $form[$catalog_name]['embridge_' . $catalog_name . '_upload_method'] = array(
        '#type' => 'radios',
        '#title' => t('Upload Method'),
        '#options' => array('rest' => 'REST', 'post' => 'POST'),
        '#default_value' => variable_get('embridge_' . $catalog_name . '_upload_method', 'rest'),
        '#weight' => 2,
    );
    $form[$catalog_name]['embridge_' . $catalog_name . '_hot_folder'] = array(
      '#type' => 'textfield',
      '#title' => t('Hot Folder'),
      '#size' => 20,
      '#required' => TRUE,
      '#description' => t('EnterMedia hot folder for the catalog (e.g. assets).'),
      '#default_value' => variable_get('embridge_' . $catalog_name . '_hot_folder', ''),
      '#weight' => 2,
    );
    $form[$catalog_name]['embridge_' . $catalog_name . '_server_mediastore'] = array(
      '#type' => 'textfield',
      '#title' => t('Media store path'),
      '#size' => 100,
      '#required' => TRUE,
      '#description' => t('EnterMedia Media store path for the catalog (e.g. /media/assets).'),
      '#default_value' => variable_get('embridge_' . $catalog_name . '_server_mediastore', ''),
      '#weight' => 2,
    );
    $form[$catalog_name]['embridge_' . $catalog_name . '_application_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Application ID'),
      '#size' => 100,
      '#description' => t('Application ID for the catalog.'),
      '#default_value' => variable_get('embridge_' . $catalog_name . '_application_id', ''),
      '#weight' => 3,
    );
    $fields = variable_get('embridge_' . $catalog_name . '_fields', array());
    $form[$catalog_name]['embridge_' . $catalog_name . '_fields'] = array(
      '#type' => 'item',
      '#title' => t('Fields'),
      '#weight' => 4,
      '#tree' => TRUE,
      '#theme' => 'embridge_fields_table',
    );

    $data_types = array('' => '', 'text' => 'Text', 'number' => 'Number', 'date' => 'Date', 'list' => 'List', 'multi_select_list' => 'List (Multi-Select)');
    foreach ($fields as $field_id => $field) {
      $form[$catalog_name]['embridge_' . $catalog_name . '_fields'][$field_id] = array(
        'label' => array(
          '#markup' => $field_id
         ),
        'id' => array(
          '#type' => 'hidden',
          '#value' => $field_id
         ),
        'name' => array(
          '#type' => 'textfield',
          '#default_value' => empty($field['name']) ? '' : $field['name'],
          '#size' => 20,
         ),
         'datatype' => array(
          '#type' => 'select',
          '#options' => $data_types,
          '#default_value' => empty($field['datatype']) ? '' : $field['datatype'],
         ),
         'include_in_search' => array(
          '#type' => 'checkbox',
          '#default_value' => empty($field['include_in_search']) ? 0 : $field['include_in_search'],
         ),
        'include_in_result' => array(
          '#type' => 'checkbox',
          '#default_value' => empty($field['include_in_result']) ? 0 : $field['include_in_result'],
         ),
        'updatable' => array(
          '#type' => 'checkbox',
          '#default_value' => empty($field['updatable']) ? 0 : $field['updatable'],
         ),
        'selected' => array(
          '#type' => 'checkbox',
          '#default_value' => empty($field['selected']) ? 0 : $field['selected'],
         ),
        'remove' => array(
          '#type' => 'checkbox',
         ),
        'weight' => array(
          '#type' => 'weight',
          '#title' => t('Weight'),
          '#default_value' => empty($field['weight']) ? 0 : $field['weight'],
          '#delta' => 50,
          '#title_display' => 'invisible',
        ),
      );
      if (!empty($field['datatype']) && $field['datatype'] == 'list') {
        $values = _embridge_get_value_list($field_id, $catalog_id);
        $form[$catalog_name]['embridge_' . $catalog_name . '_fields'][$field_id]['value'] = array(
          '#type' => 'select',
          '#default_value' => empty($field['value']) ? '' : $field['value'],
          '#options' => $values,
        );
      }
      else {
        $form[$catalog_name]['embridge_' . $catalog_name . '_fields'][$field_id]['value'] = array(
          '#type' => 'textfield',
          '#default_value' => empty($field['value']) ? '' : $field['value'],
          '#size' => 20,
        );
      }
    }
    $form[$catalog_name]['embridge_' . $catalog_name . '_new_field_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Add Field'),
      '#description' => t('Add EnterMedia Field ID (e.g. name).'),
      '#weight' => 5,
    );

    $renditions = variable_get('embridge_' . $catalog_name . '_renditions', array());
    $form[$catalog_name]['embridge_' . $catalog_name . '_renditions'] = array(
      '#type' => 'item',
      '#title' => t('Conversions'),
      '#weight' => 6,
      '#tree' => TRUE,
      '#theme' => 'embridge_renditions_table',
    );

    foreach ($renditions as $rendition_id => $rendition) {
      $form[$catalog_name]['embridge_' . $catalog_name . '_renditions'][$rendition_id] = array(
        'id' => array(
          '#markup' => $rendition_id
         ),
        'filename' => array(
          '#type' => 'textfield',
          '#default_value' => empty($rendition['filename']) ? '' : $rendition['filename'],
         ),
         'system' => array(
          '#type' => 'checkbox',
          '#default_value' => empty($rendition['system']) ? 0 : $rendition['system'],
         ),
         'wysiwyg' => array(
          '#type' => 'checkbox',
          '#default_value' => empty($rendition['wysiwyg']) ? 0 : $rendition['wysiwyg'],
         ),
         'selected' => array(
          '#type' => 'checkbox',
          '#default_value' => empty($rendition['selected']) ? 0 : $rendition['selected'],
         ),
        'remove' => array(
          '#type' => 'checkbox',
         ),
        'weight' => array(
          '#type' => 'weight',
          '#title' => t('Weight'),
          '#default_value' => empty($rendition['weight']) ? 0 : $rendition['weight'],
          '#delta' => 50,
          '#title_display' => 'invisible',
        ),
      );
    }
    $form[$catalog_name]['embridge_' . $catalog_name . '_new_rendition_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Add Conversion'),
      '#description' => t('Add EnterMedia Conversion ID (e.g. thumb).'),
      '#weight' => 7,
    );
    $form[$catalog_name]['embridge_' . $catalog_name . '_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save ' . $catalog_name . ' Catalog'),
      '#weight' => 15,
    );
  }
}

/**
 * Extra validation for embridge settings.
 */
function _embridge_settings_validate($form, &$form_state) {
  // Validate URL.
  if (!valid_url($form_state['values']['embridge_server_url'], TRUE)) {
    form_set_error('embridge_server_url', t('Invalid URL.'));
  }
  // Validate Port.
  if (!is_numeric($form_state['values']['embridge_server_port'])) {
    form_set_error('embridge_server_port', t('Invalid Port.'));
  }
  if (!empty($form_state['values']['new_catalog_id'])
      && strpos($form_state['values']['new_catalog_id'], 'media/catalogs/') !==0) {
    form_set_error('new_catalog_id', t('Invalid Catalog ID.'));
  }
}

/**
 * Extra validation for embridge settings.
 */
function _embridge_settings_submit($form, &$form_state) {
  $catalogs = variable_get('embridge_catalogs', array());
  foreach ($catalogs as $catalog_id => $catalog_name) {
    // Remove existing catalog.
    if (!empty($form_state['values']['embridge_' . $catalog_name . '_remove'])) {
      unset($catalogs[$catalog_id]);
      // Clean variables related to the catalog.
      db_delete('variable')
        ->condition('name', db_like('embridge_' . $catalog_name . '_') . '%', 'like')
        ->execute();
      foreach($form_state['values'] as $key => $value) {
        if (strpos($key, 'embridge_' . $catalog_name . '_') === 0) {
          unset($form_state['values'][$key]);
        }
      }
    }
    else {
      $weights = array();
      if (!empty($form_state['values']['embridge_' . $catalog_name . '_renditions'])) {
        foreach($form_state['values']['embridge_' . $catalog_name . '_renditions'] as $rendtion_id => $rendtion) {
          $current_weight = empty($rendtion['weight']) ? 0 : $rendtion['weight'];
          $weights[] = $current_weight;
          if (!empty($rendtion['remove'])) {
            unset($form_state['values']['embridge_' . $catalog_name . '_renditions'][$rendtion_id]);
          }
        }
      }
      $max_weight = empty($weights) ? -50 : max($weights) + 1;
      if (!empty($form_state['values']['embridge_' . $catalog_name . '_new_rendition_id'])) {
        $rendtion_id = $form_state['values']['embridge_' . $catalog_name . '_new_rendition_id'];
        $form_state['values']['embridge_' . $catalog_name . '_renditions'][$rendtion_id] = array(
          'selected' => FALSE,
          'filename' => '',
          'remove' => FALSE,
          'weight' => $max_weight,
        );
      }
      if (!empty($form_state['values']['embridge_' . $catalog_name . '_renditions'])) {
      uasort($form_state['values']['embridge_' . $catalog_name . '_renditions'], 'drupal_sort_weight');
      }
      unset($form_state['values']['embridge_' . $catalog_name . '_new_rendition_id']);
      $weights = array();

      if (!empty($form_state['values']['embridge_' . $catalog_name . '_fields'])) {
        foreach($form_state['values']['embridge_' . $catalog_name . '_fields'] as $field_id => &$field) {
          $current_weight = empty($field['weight']) ? 0 : $field['weight'];
          $weights[] = $current_weight;
          if (!empty($field['remove'])) {
            unset($form_state['values']['embridge_' . $catalog_name . '_fields'][$field_id]);
          }
        }
      }
      $max_weight = empty($weights) ? -50 : max($weights) + 1;
      if (!empty($form_state['values']['embridge_' . $catalog_name . '_new_field_id'])) {
        $field_id = $form_state['values']['embridge_' . $catalog_name . '_new_field_id'];
        $form_state['values']['embridge_' . $catalog_name . '_fields'][$field_id] = array(
          'id' => $field_id ,
          'name' => '',
          'value' => '',
          'datatype' => '',
          'include_in_search' => FALSE,
          'include_in_result' => FALSE,
          'updatable' => FALSE,
          'selected' => FALSE,
          'remove' => FALSE,
          'weight' => $max_weight,
        );
      }
      if (!empty($form_state['values']['embridge_' . $catalog_name . '_fields'])) {
      uasort($form_state['values']['embridge_' . $catalog_name . '_fields'], 'drupal_sort_weight');
      }
      unset($form_state['values']['embridge_' . $catalog_name . '_new_field_id']);
    }
  }

  // Add new catalog.
  if (!empty($form_state['values']['new_catalog_id'])) {
    $catalog_name = array_pop(explode('/', $form_state['values']['new_catalog_id']));
    $catalogs[$form_state['values']['new_catalog_id']] = $catalog_name;
  }

  unset($form_state['values']['new_catalog_id']);
  variable_set('embridge_catalogs', $catalogs);
}

function _embridge_catalog_settings_submit_callback($form, &$form_state) {
  $catalog_name = current($form_state['clicked_button']['#array_parents']);
  $form_state['rebuild'] = TRUE;
  $form = drupal_rebuild_form($form['#form_id'], $form_state);
  $form[$catalog_name]['embridge_' . $catalog_name . '_new_rendition_id']['#value'] = '';
  $form[$catalog_name]['embridge_' . $catalog_name . '_new_field_id']['#value'] = ''; 
  return $form[$catalog_name];
}

/**
 * Test connection for the EnterMedia Server.
 *
 * @return string
 *   JSON indicate status of the connection.
 */
function embridge_test_connection() {
  $server_url = filter_xss($_POST['server']);
  $server_port = filter_xss($_POST['port']);
  $login = filter_xss($_POST['login']);
  $password = filter_xss($_POST['password']);
  if (_embridge_server_status($server_url, $server_port, $login, $password)) {
    print drupal_json_output(array('status' => t('Connected to EnterMedia server successfully.')));
  }
  else {
    print drupal_json_output(array('status' => t('Failed to EnterMedia server using above information.')));
  }
  exit();
}

function theme_embridge_renditions_table($variables) {
  $element = $variables['element'];
  $headers = array(t('ID'), t('File Name'), t('System'),  t('WYSIWYG'), t('Enabled'), t('Remove'), '');
  $rows = array();
  foreach (element_children($element) as $rendition_id) {
    $element[$rendition_id]['weight']['#attributes']['class'] = array('renditions-table-item-weight');
    $row = array(
      'data' => array(
        drupal_render($element[$rendition_id]['id']),
        drupal_render($element[$rendition_id]['filename']),
        drupal_render($element[$rendition_id]['system']),
        drupal_render($element[$rendition_id]['wysiwyg']),
        drupal_render($element[$rendition_id]['selected']),
        drupal_render($element[$rendition_id]['remove']),
        drupal_render($element[$rendition_id]['weight']),
      ),
      'class' => array('draggable'),
    );
    $rows[] = $row;
  }
  $output = '';
  if (!empty($rows)) {
    $table_id = 'renditions-table';
    $output .= theme('table', array(
      'header' => $headers,
      'rows' => $rows,
      'attributes' => array('class' => array('renditions-table'), 'id' => $table_id),
    ));
    drupal_add_tabledrag($table_id, 'order', 'sibling', 'renditions-table-item-weight');
  }
  return $output;
}

function theme_embridge_fields_table($variables) {
  $element = $variables['element'];
  $headers = array(t('ID'), t('Field Name'), t('Field Value'), t('Type'), t('Searchable'), t('Include in Result'), t('Editable'), t('Enabled'), t('Remove'), '');
  $rows = array();
  foreach (element_children($element) as $field_id) {
    $element[$field_id]['weight']['#attributes']['class'] = array('fields-table-item-weight');
    $row = array(
      'data' => array(
        drupal_render($element[$field_id]['label']) . drupal_render($element[$field_id]['id']),
        drupal_render($element[$field_id]['name']),
        drupal_render($element[$field_id]['value']),
        drupal_render($element[$field_id]['datatype']),
        drupal_render($element[$field_id]['include_in_search']),
        drupal_render($element[$field_id]['include_in_result']),
        drupal_render($element[$field_id]['updatable']),
        drupal_render($element[$field_id]['selected']),
        drupal_render($element[$field_id]['remove']),
        drupal_render($element[$field_id]['weight']),
      ),
      'class' => array('draggable'),
    );
    $rows[] = $row;
  }
  $output = '';
  if (!empty($rows)) {
    $table_id = 'fields-table';
    $output .= theme('table', array(
      'header' => $headers,
      'rows' => $rows,
      'attributes' => array('class' => array('fields-table'), 'id' => $table_id),
    ));
    drupal_add_tabledrag($table_id, 'order', 'sibling', 'fields-table-item-weight');
  }
  return $output;
}