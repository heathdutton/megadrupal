<?php

/**
 * @file
 * Settings forms
 */

/**
 * Implements hook_drupal_sync_prepare_types().
 */
function drupal_sync_drupal_sync_prepare_types($types) {
  $node_types = node_type_get_types();
  foreach ($node_types as $key => $value) {
    $types['node'][$key] = array(
      'type' => $value->type,
      'name' => $value->name
    );
  }

  $taxonomy_voc = taxonomy_get_vocabularies();
  foreach ($taxonomy_voc as $key => $value) {
    $types['taxonomy_term'][$value->machine_name] = array(
      'vid' => $value->vid,
      'name' => $value->name
    );
  }
  return $types;
}

/**
 * Implements hook_drupal_sync_default_fields_info().
 */
function drupal_sync_drupal_sync_default_fields_info($fields) {
  $fields['node'] = array(
    'title' => array(
      'label' => t('Title'),
    ),
    'status' => array(
      'label' => t('Published'),
    ),
    'promote' => array(
      'label' => t('Promoted to front page'),
    ),
    'comment' => array(
      'label' => t('Comments on/off'),
    ),
  );

  $fields['taxonomy_term'] = array(
    'name' => array(
      'label' => t('Name'),
    ),
    'description' => array(
      'label' => t('Description'),
    ),
    'parent' => array(
      'label' => t('Parent'),
    ),
  );

  return $fields;
}

/**
 * Implements hook_drupal_sync_types_build_form_alter().
 */
function drupal_sync_drupal_sync_types_build_form_alter(&$form, $types, $defaults = array()) {
  foreach ($types as $type => $value) {
    switch ($type) {
      case 'node':
        $options = array();
        foreach ($value as $option) {
          $options[$option['type']] = $option['name'];
        }

        $form['drupal_sync_settings']['drupal_sync_entities']['drupal_sync_node_settings'] = array(
          '#type' => 'fieldset',
          '#title' => t('Available node types'),
          '#collapsible' => TRUE,
        );

        $form['drupal_sync_settings']['drupal_sync_entities']['drupal_sync_node_settings']['drupal_sync_node_types'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Choose node types for sync'),
          '#default_value' => (isset($defaults['node'])) ? array_keys($defaults['node']) : array(),
          '#options' => $options,
        );

        break;
      case 'taxonomy_term':
        $options = array();
        foreach ($value as $option) {
          $options[$option['vid']] = $option['name'];
        }
        $form['drupal_sync_settings']['drupal_sync_entities']['drupal_sync_taxonomy_settings'] = array(
          '#type' => 'fieldset',
          '#title' => t('Available taxonomy vocabularies'),
          '#collapsible' => TRUE,
        );

        $form['drupal_sync_settings']['drupal_sync_entities']['drupal_sync_taxonomy_settings']['drupal_sync_taxonomy_vocabularies'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Choose vocabularies for sync'),
          '#default_value' => (isset($defaults['taxonomy_term'])) ? array_keys($defaults['taxonomy_term']) : array(),
          '#options' => $options,
        );
        break;
    }
  }
  return $form;
}

/**
 * Implements hook_drupal_sync_settings_submit_form_alter().
 */
function drupal_sync_drupal_sync_settings_submit_form_alter(&$data, $form_state) {
  $settings = (isset($form_state['values']['drupal_sync_settings'])) ? $form_state['values']['drupal_sync_settings'] : array();

  $node_settings = isset($settings['drupal_sync_entities']['drupal_sync_node_settings']['drupal_sync_node_types']) ? $settings['drupal_sync_entities']['drupal_sync_node_settings']['drupal_sync_node_types'] : array();
  $node_settings = array_filter($node_settings);

  foreach ($node_settings as $key => $value) {
    $node_settings[$key] = (isset($form_state['original_entity_types']['node'][$key]['name'])) ? $form_state['original_entity_types']['node'][$key]['name'] : $value;
  }

  $taxonomy_settings = isset($settings['drupal_sync_entities']['drupal_sync_taxonomy_settings']['drupal_sync_taxonomy_vocabularies']) ? $settings['drupal_sync_entities']['drupal_sync_taxonomy_settings']['drupal_sync_taxonomy_vocabularies'] : array();
  $taxonomy_settings = array_filter($taxonomy_settings);

  $original_taxonomy = (isset($form_state['original_entity_types']['taxonomy_term'])) ? $form_state['original_entity_types']['taxonomy_term'] : array();
  $tmp_taxonomy = array();
  foreach ($original_taxonomy as $machine_name => $value) {
    $tmp_taxonomy[$value['vid']] = array(
      'name' => $value['name'],
      'machine_name' => $machine_name
    );
  }
  $original_taxonomy = $tmp_taxonomy;

  foreach ($taxonomy_settings as $key => $value) {
    $taxonomy_settings[$key] = (isset($original_taxonomy[$key]['name'])) ? $original_taxonomy[$key]['name'] : $value;
  }

  $data['drupal_sync_entities']['node'] = $node_settings;
  $data['drupal_sync_entities']['taxonomy_term'] = $taxonomy_settings;

  return $data;
}

/**
 * Create settings form in drupal configuration setings section for module
 */
function drupal_sync_admin_settings_form($form, &$form_state) {
  $request_limit = drupal_map_assoc(array(1, 3, 10, 20, 30, 50, 100));
  $data = variable_get('drupal_sync_settings', array());
  $types = array();
  $types = module_invoke_all('drupal_sync_prepare_types', $types);
  $form_state['original_entity_types'] = $types;

  $queue_update_frequency = drupal_map_assoc(array(60, 300, 600, 1800, 3600), 'format_interval');
  $queue_update_count = drupal_map_assoc(array(1, 3, 5, 10, 20, 50, 100, 200, 300, 500, 1000));

  /*   * ***************************************************************
   * Base section
   * *************************************************************** */
  $form['#tree'] = TRUE;
  $form['#attached']['css'][] = drupal_get_path('module', 'drupal_sync') . '/css/drupal_sync_admin.css';

  $form['drupal_sync_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync settings'),
  );



  /*   * ***************************************************
   * Site domains
   * **************************************************** */

  $domains = (isset($data['drupal_sync_domains']['drupal_sync_domain'])) ? $data['drupal_sync_domains']['drupal_sync_domain'] : array();

  $form['drupal_sync_settings']['drupal_sync_domains'] = array(
    '#type' => 'fieldset',
    '#title' => t('Current site domains'),
    '#description' => t('Current site domains withour protocol and slash. E.g. site.com, m.site.com'),
    '#prefix' => '<div id="drupal-sync-domain-wrapper">',
    '#suffix' => '</div>',
  );

  if (!isset($form_state['drupal_sync_domains_num']) || empty($form_state['drupal_sync_domains_num'])) {
    $form_state['drupal_sync_domains_num'] = (isset($data['drupal_sync_domains_num'])) ? $data['drupal_sync_domains_num'] : 1;
  }

  for ($i = 0; $i < $form_state['drupal_sync_domains_num']; $i++) {
    $form['drupal_sync_settings']['drupal_sync_domains']['drupal_sync_domain'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('Domain @count', array('@count' => ($i + 1))),
      '#default_value' => (isset($domains[$i])) ? $domains[$i] : '',
      '#required' => empty($i) ? TRUE : FALSE
    );
  }

  $form['drupal_sync_settings']['drupal_sync_domains']['drupal_sync_add_domain'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more domain'),
    '#submit' => array('drupal_sync_domain_add_one'),
    '#ajax' => array(
      'callback' => 'drupal_sync_domain_add_more_callback',
      'wrapper' => 'drupal-sync-domain-wrapper',
    ),
  );
  if ($form_state['drupal_sync_domains_num'] > 1) {
    $form['drupal_sync_settings']['drupal_sync_domains']['drupal_sync_remove_domain'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one last domain'),
      '#submit' => array('drupal_sync_domain_remove_one'),
      '#ajax' => array(
        'callback' => 'drupal_sync_domain_add_more_callback',
        'wrapper' => 'drupal-sync-domain-wrapper',
      ),
    );
  }

  $form['drupal_sync_settings']['drupal_sync_request_limit'] = array(
    '#type' => 'select',
    '#title' => t('Count http requests (if receive fail)'),
    '#default_value' => (isset($data['drupal_sync_request_limit'])) ? $data['drupal_sync_request_limit'] : 3,
    '#options' => $request_limit,
    '#required' => TRUE
  );

  $form['drupal_sync_settings']['drupal_sync_queue_update_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Frequency of updates'),
    '#default_value' => (isset($data['drupal_sync_queue_update_frequency'])) ? $data['drupal_sync_queue_update_frequency'] : 60,
    '#options' => $queue_update_frequency,
    '#required' => TRUE
  );

  $form['drupal_sync_settings']['drupal_sync_queue_update_count'] = array(
    '#type' => 'select',
    '#title' => t('Could of updates per request'),
    '#default_value' => (isset($data['drupal_sync_queue_update_count'])) ? $data['drupal_sync_queue_update_count'] : 10,
    '#options' => $queue_update_count,
    '#required' => TRUE
  );

  $form['drupal_sync_settings']['drupal_sync_notification_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email for notifications'),
    '#default_value' => (isset($data['drupal_sync_notification_mail'])) ? $data['drupal_sync_notification_mail'] : '',
  );

  $form['drupal_sync_settings']['drupal_sync_entities'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync entities'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );


  /*   * ***************************************************************
   * Entities section
   * *************************************************************** */
  $defaults = isset($data['drupal_sync_entities']) ? $data['drupal_sync_entities'] : array();
  drupal_alter('drupal_sync_types_build_form', $form, $types, $defaults);

  /*   * ***************************************************************
   * Sync remote section
   * *************************************************************** */

  $form['drupal_sync_settings']['drupal_sync_remote'] = array(
    '#type' => 'fieldset',
    '#title' => t('Remote sites'),
    '#description' => t('Send updates from current site to remote sites.'),
    '#prefix' => '<div id="drupal-sync-remote-wrapper">',
    '#suffix' => '</div>',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (empty($form_state['drupal_sync_remote_num'])) {
    $form_state['drupal_sync_remote_num'] = (isset($data['drupal_sync_remote_count'])) ? $data['drupal_sync_remote_count'] : 1;
  }

  for ($i = 0; $i < $form_state['drupal_sync_remote_num']; $i++) {
    $form['drupal_sync_settings']['drupal_sync_remote'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Site @count', array('@count' => ($i + 1))),
      '#collapsible' => TRUE,
    );

    $form['drupal_sync_settings']['drupal_sync_remote'][$i]['drupal_sync_remote_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Site name'),
      '#description' => t('Site name must be unique, only lowercase letters, numbers, and underscores'),
      '#default_value' => (isset($data['drupal_sync_remote'][$i]['name'])) ? $data['drupal_sync_remote'][$i]['name'] : '',
    );

    $form['drupal_sync_settings']['drupal_sync_remote'][$i]['drupal_sync_remote_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Url'),
      '#description' => t('Site Url must be unique. Site url must be with protocol but without close slash. E.g. http://site.com'),
      '#default_value' => (isset($data['drupal_sync_remote'][$i]['url'])) ? $data['drupal_sync_remote'][$i]['url'] : '',
    );

    $form['drupal_sync_settings']['drupal_sync_remote'][$i]['drupal_sync_remote_login'] = array(
      '#type' => 'textfield',
      '#title' => t('Login'),
      '#default_value' => (isset($data['drupal_sync_remote'][$i]['login'])) ? $data['drupal_sync_remote'][$i]['login'] : '',
    );

    $form['drupal_sync_settings']['drupal_sync_remote'][$i]['drupal_sync_remote_pass'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#default_value' => (isset($data['drupal_sync_remote'][$i]['pass'])) ? $data['drupal_sync_remote'][$i]['pass'] : '',
    );
  }

  $form['drupal_sync_settings']['drupal_sync_remote']['drupal_sync_add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#submit' => array('drupal_sync_remote_add_one'),
    '#ajax' => array(
      'callback' => 'drupal_sync_remote_add_more_callback',
      'wrapper' => 'drupal-sync-remote-wrapper',
    ),
  );
  if ($form_state['drupal_sync_remote_num'] > 1) {
    $form['drupal_sync_settings']['drupal_sync_remote']['drupal_sync_remove_name'] = array(
      '#type' => 'submit',
      '#value' => t('Remove one'),
      '#submit' => array('drupal_sync_remote_remove_one'),
      '#ajax' => array(
        'callback' => 'drupal_sync_remote_add_more_callback',
        'wrapper' => 'drupal-sync-remote-wrapper',
      ),
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  $form['#submit'][] = 'drupal_sync_admin_settings_form_submit';
  $form['#validate'][] = 'drupal_sync_admin_settings_form_validate';
  return $form;
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function drupal_sync_remote_add_one($form, &$form_state) {
  $form_state['drupal_sync_remote_num']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function drupal_sync_remote_add_more_callback($form, $form_state) {
  return $form['drupal_sync_settings']['drupal_sync_remote'];
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function drupal_sync_remote_remove_one($form, &$form_state) {
  if ($form_state['drupal_sync_remote_num'] > 1) {
    $form_state['drupal_sync_remote_num']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function drupal_sync_domain_add_one($form, &$form_state) {
  $form_state['drupal_sync_domains_num']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function drupal_sync_domain_add_more_callback($form, $form_state) {
  return $form['drupal_sync_settings']['drupal_sync_domains'];
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function drupal_sync_domain_remove_one($form, &$form_state) {
  if ($form_state['drupal_sync_domains_num'] > 1) {
    $form_state['drupal_sync_domains_num']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Form drupal_sync_admin_settings_form Validation
 */
function drupal_sync_admin_settings_form_validate($form, &$form_state) {
  $url_values = array();
  $name_values = array();

  foreach ($form_state['values']['drupal_sync_settings']['drupal_sync_remote'] as $key => $value) {
    if (is_numeric($key)) {
      $url_values[$key] = $value['drupal_sync_remote_url'];
      $name_values[$key] = $value['drupal_sync_remote_name'];
    }
  }

  //check url duplicates
  $duplicate_url_values = _drupal_sync_get_duplicates_values($url_values);
  if (!empty($duplicate_url_values)) {
    //search url duplicates
    foreach ($url_values as $i => $value) {
      if (in_array($value, $duplicate_url_values)) {
        form_set_error('drupal_sync_settings][drupal_sync_remote][' . $i . '][drupal_sync_remote_url', 'Error, site URLs must be unique');
      }
    }
  }

  //check allowed name symbols
  foreach ($name_values as $i => $value) {
    if (preg_match('@[^a-z0-9_]@', $value)) {
      form_set_error('drupal_sync_settings][drupal_sync_remote][' . $i . '][drupal_sync_remote_name', t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
    }
  }

  //check names duplicates
  $duplicate_name_values = _drupal_sync_get_duplicates_values($name_values);
  if (!empty($duplicate_name_values)) {
    //search url duplicates
    foreach ($name_values as $i => $value) {
      if (in_array($value, $duplicate_name_values)) {
        form_set_error('drupal_sync_settings][drupal_sync_remote][' . $i . '][drupal_sync_remote_name', 'Error, site Name must be unique');
      }
    }
  }
}

/**
 * Takes an array and returns an array of duplicate items
 *
 * @param array $array
 *
 * @return array
 */
function _drupal_sync_get_duplicates_values($array) {
  return array_diff_assoc($array, array_unique($array));
}

/**
 * Form drupal_sync_admin_settings_form submit function
 */
function drupal_sync_admin_settings_form_submit($form, &$form_state) {
  $data = array();
  $settings = (isset($form_state['values']['drupal_sync_settings'])) ? $form_state['values']['drupal_sync_settings'] : array();

  $data['drupal_sync_request_limit'] = $settings['drupal_sync_request_limit'];
  $data['drupal_sync_queue_update_frequency'] = $settings['drupal_sync_queue_update_frequency'];
  $data['drupal_sync_queue_update_count'] = $settings['drupal_sync_queue_update_count'];
  $data['drupal_sync_notification_mail'] = $settings['drupal_sync_notification_mail'];


  $data['drupal_sync_remote_count'] = (isset($form_state['drupal_sync_remote_num'])) ? $form_state['drupal_sync_remote_num'] : 1;
  $data['drupal_sync_domains_num'] = (isset($form_state['drupal_sync_domains_num'])) ? $form_state['drupal_sync_domains_num'] : 1;
  $data['drupal_sync_domains'] = $settings['drupal_sync_domains'];


  $remote = (isset($settings['drupal_sync_remote'])) ? $settings['drupal_sync_remote'] : array();
  for ($i = 0; $i < $data['drupal_sync_remote_count']; $i++) {
    if (isset($remote[$i])) {
      $data['drupal_sync_remote'][$i] = array(
        'name' => (isset($remote[$i]['drupal_sync_remote_name'])) ? $remote[$i]['drupal_sync_remote_name'] : '',
        'url' => (isset($remote[$i]['drupal_sync_remote_url'])) ? $remote[$i]['drupal_sync_remote_url'] : '',
        'login' => (isset($remote[$i]['drupal_sync_remote_login'])) ? $remote[$i]['drupal_sync_remote_login'] : '',
        'pass' => (isset($remote[$i]['drupal_sync_remote_pass'])) ? $remote[$i]['drupal_sync_remote_pass'] : '',
      );
    }
  }

  drupal_alter('drupal_sync_settings_submit_form', $data, $form_state);

  variable_set('drupal_sync_settings', $data);
  // clear auth cache
  variable_del('drupal_sync_xmlrpc_auth_options');

  // clear menu cache
  menu_cache_clear_all();
  drupal_set_message(t('The settings are saved.'), 'status');
}

/**
 * Drupal sync backup/restore functionality
 *
 * @return string
 */
function drupal_sync_settings_backup() {
  $action = arg(5);
  $domain_name = str_replace('www.', '', $_SERVER['HTTP_HOST']);
  $domain_name = str_replace(array('.', '-'), array('_', '_'), $domain_name);
  if (!drupal_realpath('private://')) {
    drupal_set_message(t('Please, set up Private file system path !link', array('!link' => l(t('here'), 'admin/config/media/file-system'))), 'error');
    return '';
  }

  $filedir = 'private://drupal_sync';
  file_prepare_directory($filedir, FILE_CREATE_DIRECTORY);

  $file_real_name = drupal_realpath($filedir) . "/" . $domain_name . ".dat";

  if (!file_exists($file_real_name)) {
    $fh = fopen($file_real_name, "x+");
    fclose($fh);
    chmod($file_real_name, 0440);
  }

  if ($action == 'restore') {
    $data = file_get_contents($file_real_name);
    $data = unserialize($data);
    if (isset($data['settings']) && isset($data['relations'])) {
      variable_set('drupal_sync_settings', $data['settings']);
      variable_set('drupal_sync_remote_relation_settings', $data['relations']);
      drupal_set_message(t("Drupal sync settings have been restored"));
    }
    else {
      drupal_set_message(t("Error! Drupal sync settings have not been restored. Wrong format of backup file!"), 'error');
    }
    drupal_goto("admin/config/drupal_sync/settings/backup");
  }
  elseif ($action == 'backup') {
    $data = array(
      'settings' => variable_get('drupal_sync_settings', array()),
      'relations' => variable_get('drupal_sync_remote_relation_settings', array())
    );

    if (function_exists('posix_getpwuid') && function_exists('posix_geteuid')) {
      $apache_user = posix_getpwuid(posix_geteuid());
      $file_owner = posix_getpwuid(fileowner($file_real_name));

      if ($apache_user != $file_owner) {
        drupal_set_message(t("Drupal sync settings have not been saved.") . " <br>" . t("Check backup file owner permissions!"), 'error');
        drupal_goto("admin/config/drupal_sync/settings/backup");
      }
    }
    
    $perms = chmod($file_real_name, 0774);

    if ($perms) {
      $result = file_put_contents($file_real_name, serialize($data));
      chmod($file_real_name, 0440);
      drupal_set_message(t('Drupal sync settings have been saved'));
    }
    else {
      $result = file_put_contents($file_real_name, serialize($data));
      chmod($file_real_name, 0440);
      drupal_set_message(t('Drupal sync settings have not been saved. Check backup file permissions!'), 'error');
    }
    drupal_goto("admin/config/drupal_sync/settings/backup");
  }


  $content = l("[ " . t("Create backup") . " ]", "admin/config/drupal_sync/settings/backup/backup") . "<br><br>" .
      l("[ " . t("Restore backup") . " ]", "admin/config/drupal_sync/settings/backup/restore");

  return $content;
}