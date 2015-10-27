<?php

/**
 * @file
 * Administration functionality for the PowerTagging.
 */

/**
 * List all saved PowerTagging configurations.
 *
 * @return string
 *   The rendered HTML of the list of configurations.
 */
function powertagging_list() {
  $conf_sets = powertagging_config_load();

  $rows = array();
  foreach ($conf_sets as $conf) {
    $actions = array(
      l(t('Edit'), 'admin/config/services/powertagging/' . $conf->powertagging_id . '/edit'),
      l(t('Delete'), 'admin/config/services/powertagging/' . $conf->powertagging_id . '/delete'),
    );
    $rows[] = array(
      $conf->title,
      $conf->connection->getURL(),
      '<div class="pp-led" data-id="' . $conf->powertagging_id . '" title="Checking service"></div>',
      implode(' | ', $actions),
    );
  }

  $table = array(
    'header' => array(
      t('Name'),
      t('URL to PoolParty server'),
      t('Available'),
      t('Actions'),
    ),
    'rows' => $rows,
  );

  return theme('table', $table);
}

/**
 * Ajax callback function for checking if a new PoolParty server is available.
 */
function powertagging_new_available($form, $form_state) {
  $available = '<div id="health_info" class="available">' . t('The server is available.') . '</div>';
  $not_available = '<div id="health_info" class="not-available">' . t('The server is not available or the credentials are incorrect.') . '</div>';

  if (isset($form_state['values']['url']) && valid_url($form_state['values']['url'], TRUE)) {
    // Create a new connection (without saving) with the current form data.
    $connection = SemanticConnector::getConnection('pp_server');
    $connection->setURL($form_state['values']['url']);
    $connection->setCredentials(array(
      'username' => $form_state['values']['name'],
      'password' => $form_state['values']['pass'],
    ));
    $pp_api = $connection->getAPI('PPX');

    $availability = $pp_api->available();
    if (isset($availability['message']) && !empty($availability['message'])) {
      return '<div id="health_info" class="not-available">' . $availability['message'] . '</div>';
    }
    else {
      return $availability['success'] ? $available : $not_available;
    }
  }

  return $not_available;
}

/**
 * Ajax callback function for checking if a PowerTagging is available.
 */
function powertagging_available($powertagging_id) {
  $pp_api = powertagging_getPoolPartyApi($powertagging_id);
  $availability = $pp_api->available();
  echo $availability['success'] ? 1 : 0;
  exit();
}

/**
 * The form for a PowerTagging configuration.
 */
function powertagging_form($form, &$form_state, $conf = NULL) {
  if (!is_null($conf) && !empty($conf)) {
    $is_configuration_new = FALSE;
    $form['powertagging_id'] = array(
      '#type' => 'hidden',
      '#value' => $conf->powertagging_id,
    );
  }
  else {
    $is_configuration_new = TRUE;
    $conf = powertagging_default_config_load();
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name of the PowerTagging configuration.'),
    '#size' => 35,
    '#maxlength' => 60,
    '#default_value' => $conf->title,
    '#required' => TRUE,
  );

  // Define the container for the vertical tabs.
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // Tab: Server settings.
  $form['server_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PoolParty server settings'),
    '#group' => 'settings',
  );

  // Display any unfulfilled module requirement.
  $requirement_errors = powertagging_check_requirements();
  if (!empty($requirement_errors)) {
    $requirement_errors_markup = '';
    foreach ($requirement_errors as $requirement_error) {
      $requirement_errors_markup .= '<div class="messages error">' . $requirement_error . '</div>';
    }
    $form['server_settings']['requirements'] = array(
      '#type' => 'item',
      '#markup' => $requirement_errors_markup,
    );
  }

  $connections = SemanticConnector::getConnectionsByType('pp_server');
  if (!empty($connections)) {
    $connection_options = array();
    foreach ($connections as $connection) {
      $credentials = $connection->getCredentials();
      $connection_options[implode('|', array($connection->getTitle(), $connection->getURL(), $credentials['username'], $credentials['password']))] = $connection->getTitle();
    }
    $form['server_settings']['load_connection'] = array(
      '#type' => 'select',
      '#title' => t('Load an available PoolParty server'),
      '#options' => $connection_options,
      '#empty_option' => '',
      '#default_value' => '',
    );

    $form['server_settings']['load_connection_button'] = array(
      '#type' => 'button',
      '#value' => t('Load'),
      '#attributes' => array(
        'onclick' => '
        var connection_value = (jQuery(\'#edit-load-connection\').val());
        if (connection_value.length > 0) {
          var connection_details = connection_value.split(\'|\');
          jQuery(\'#edit-server-title\').val(connection_details[0]);
          jQuery(\'#edit-url\').val(connection_details[1]);
          jQuery(\'#edit-name\').val(connection_details[2]);
          jQuery(\'#edit-pass\').val(connection_details[3]);
        }
        return false;',
      ),
    );
  }

  $form['server_settings']['connection_id'] = array(
    '#type' => 'hidden',
    '#value' => $conf->connection->getID(),
  );

  $form['server_settings']['server_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Server title'),
    '#description' => t('A short title for the server below.'),
    '#size' => 35,
    '#maxlength' => 60,
    '#default_value' => $conf->connection->getTitle(),
    '#required' => TRUE,
  );

  $form['server_settings']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('URL, where the PoolParty server runs.'),
    '#size' => 35,
    '#maxlength' => 255,
    '#default_value' => $conf->connection->getURL(),
    '#required' => TRUE,
  );

  $credentials = $conf->connection->getCredentials();
  $form['server_settings']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Name of a user for the credentials.'),
    '#size' => 35,
    '#maxlength' => 60,
    '#default_value' => $credentials['username'],
  );

  $form['server_settings']['pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('Password of a user for the credentials.'),
    '#size' => 35,
    '#maxlength' => 128,
    '#default_value' => $credentials['password'],
  );

  $form['server_settings']['health_check'] = array(
    '#type' => 'button',
    '#value' => t('Health check'),
    '#ajax' => array(
      'callback' => 'powertagging_new_available',
      'wrapper' => 'health_info',
      'method' => 'replace',
      'effect' => 'slide',
    ),
  );

  if ($is_configuration_new) {
    $form['server_settings']['health_info'] = array(
      '#markup' => '<div id="health_info">' . t('Click to check if the server is available.') . '</div>',
    );
  }
  else {
    $available = '<div id="health_info" class="available">' . t('The server is available.') . '</div>';
    $not_available = '<div id="health_info" class="not-available">' . t('The server is not available or the credentials are incorrect.') . '</div>';

    $pp_api = powertagging_getPoolPartyApi($conf->powertagging_id);
    $availability = $pp_api->available();
    if (isset($availability['message']) && !empty($availability['message'])) {
      $availability_markup = '<div id="health_info" class="not-available">' . $availability['message'] . '</div>';
    }
    else {
      $availability_markup = $availability['success'] ? $available : $not_available;
    }

    $form['server_settings']['health_info'] = array(
      '#markup' => $availability_markup,
    );
  }

  // Tab: Project settings.
  $form['project_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available projects'),
    '#group' => 'settings',

  );

  $form['project_settings']['projects'] = array(
    '#type' => 'item',
    '#title' => t('Available PoolParty Projects'),
    '#tree' => TRUE,
  );

  if ($is_configuration_new) {
    $form['project_settings']['projects']['#description'] = t('Please enter server settings and save the configuration before selecting the projects.');
  }
  else {
    $available_languages = array(
      LANGUAGE_NONE => (object) array(
        'language' => LANGUAGE_NONE,
        'name' => 'Language neutral',
        'enabled' => TRUE,
      ),
    );
    $available_languages = array_merge($available_languages, language_list());

    $projects = $pp_api->getProjects();
    if (!empty($projects)) {
      foreach ($projects as $project) {
        $form['project_settings']['projects'][$project->uuid] = array(
          '#type' => 'fieldset',
          '#title' => $project->label,
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
          '#description' => t('Please map the PoolParty project languages to the Drupal languages:'),
        );

        $form['project_settings']['projects'][$project->uuid]['label'] = array(
          '#type' => 'hidden',
          '#value' => $project->label,
        );

        // Language mapping.
        $project_language_options = array();
        foreach ($project->languages as $project_language) {
          $project_language_options[$project_language] = $project_language;
        }

        $states = array();
        foreach ($available_languages as $lang) {
          if ($lang->enabled) {
            $form['project_settings']['projects'][$project->uuid]['languages'][$lang->language] = array(
              '#type' => 'select',
              '#title' => $lang->name,
              '#options' => $project_language_options,
              '#empty_option' => '',
              '#default_value' => ((isset($conf->config['projects']) && isset($conf->config['projects'][$project->uuid]) && isset($conf->config['projects'][$project->uuid]['languages'][$lang->language])) ? $conf->config['projects'][$project->uuid]['languages'][$lang->language] : ''),
            );
            $states['#edit-projects-' . strtolower($project->uuid) . '-languages-' . $lang->language] = array('value' => '');
          }
        }

        // Vocabulary selection.
        // Hidden field for the selecting the vocabulary.
        // It checks the availability of a language.
        $form['project_settings']['projects'][$project->uuid]['no_language_selected'] = array(
          '#type' => 'checkbox',
          '#default_value' => TRUE,
          '#attributes' => array('class' => array('element-hidden')),
          '#states' => array(
            'checked' => $states,
          ),
        );
        $form['project_settings']['projects'][$project->uuid]['taxonomy_name'] = array(
          '#title' => t('Select or enter a new vocabulary'),
          '#type' => 'textfield',
          '#default_value' => ((isset($conf->config['projects']) && isset($conf->config['projects'][$project->uuid]) && isset($conf->config['projects'][$project->uuid]['taxonomy_name'])) ? $conf->config['projects'][$project->uuid]['taxonomy_name'] : ''),
          '#autocomplete_path' => 'powertagging/autocomplete_taxonomies',
          '#states' => array(
            'required' => array(
              '#edit-projects-' . strtolower($project->uuid) . '-no-language-selected' => array('checked' => FALSE),
            ),
            'disabled' => $states,
          ),
        );

        // Ask if the vocabulary should be removed also if no language is
        // selected.
        if (isset($conf->config['projects'][$project->uuid]['taxonomy_name']) && !empty($conf->config['projects'][$project->uuid]['taxonomy_name'])) {
          $form['project_settings']['projects'][$project->uuid]['remove_taxonomy'] = array(
            '#type' => 'checkbox',
            '#title' => t('Remove the appropriate vocabulary. All terms and relations of this vocabulary will be removed also.'),
            '#states' => array(
              'visible' => $states,
            ),
          );
        }
      }
    }
    else {
      $form['project_settings']['errors'] = array(
        '#type' => 'item',
        '#markup' => '<div class="messages warning">' . t('Either no connection can be established or there are no projects available for the given credentials.') . '</div>',
      );
    }
  }

  // Tab: Global limits.
  $form['global_limits_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global limits'),
    '#group' => 'settings',
  );
  powertagging_global_limits_form($form['global_limits_settings'], $conf->config['concepts_per_extraction'], $conf->config['concepts_threshold'], $conf->config['freeterms_per_extraction'], $conf->config['freeterms_threshold']);

  // Tab: Batch Jobs.
  $form['batch_jobs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Batch jobs'),
    '#group' => 'settings',
    '#tree' => TRUE,
  );

  $bulk_projects = array();
  if (isset($conf->config['projects'])) {
    $project_rows = array();

    foreach ($projects as $project) {
      if (isset($conf->config['projects'][$project->uuid])) {
        $project_languages = array();
        foreach ($conf->config['projects'][$project->uuid]['languages'] as $pp_language) {
          if (!empty($pp_language)) {
            $project_languages[] = $pp_language;
          }
        }

        if (!empty($project_languages)) {
          $bulk_projects[] = $project->uuid;
          $project_rows[$project->uuid] = array(
            'label' => $project->label . ' (' . implode(', ', $project_languages) . ')',
            'bulk_tagging' => l(t('Index'), 'admin/config/services/powertagging/' . $conf->powertagging_id . '/index_project/' . $project->uuid),
          );

          if (module_exists('skos_importer')) {
            $project_rows[$project->uuid]['skos_import'] = l(t('Synchronize'), 'admin/config/services/powertagging/' . $conf->powertagging_id . '/synchronize_project/' . $project->uuid);
          }
        }
      }
    }
    if (!empty($project_rows)) {
      $header = array(
        'label' => t('Project'),
        'bulk_tagging' => 'Operations',
      );

      if (module_exists('skos_importer')) {
        $header['skos_import'] = '';
      }

      $form['batch_jobs']['projects'] = array(
        '#type' => 'item',
        '#markup' => theme('table', array(
          'header' => $header,
          'rows' => $project_rows,
          'attributes' => array(),
          'empty' => t('No content available.'),
          'sticky' => FALSE,
          'colgroups' => array(),
          'caption' => NULL,
        )),
      );
    }
  }

  if (empty($bulk_projects)) {
    $form['batch_jobs']['errors'] = array(
      '#type' => 'item',
      '#markup' => t('Projects need to be correctly configured before bulk operations can be run for them.'),
    );
  }

  /*if (module_exists('smart_glossary')) {
    // Tab: Semantic module connection.
    $form['semantic_connection'] = array(
      '#type' => 'fieldset',
      '#title' => t('Semantic module connection'),
      '#group' => 'settings',
      '#tree' => TRUE,
    );
  }*/

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/services/powertagging',
  );

  return $form;
}

/**
 * Validate handler for powertagging_form.
 */
function powertagging_form_validate($form, &$form_state) {
  // If there are any requirement errors, do not save the configuration.
  $requirement_errors = powertagging_check_requirements();
  if (!empty($requirement_errors)) {
    $requirement_errors_markup = t('The configuration could not be saved due to unfulfilled module requirements:');
    foreach ($requirement_errors as $requirement_error) {
      $requirement_errors_markup .= '<br />&nbsp;- ' . $requirement_error;
    }

    form_set_error('', $requirement_errors_markup);
    return FALSE;
  }

  $values = $form_state['values'];
  $default_language = language_default('language');

  if (!valid_url($values['url'], TRUE)) {
    form_set_error('url', t('The field URL must be a valid URL.'));
  }

  if (isset($values['projects']) && !empty($values['projects'])) {
    foreach ($values['projects'] as $key => $project) {
      if (!$project['no_language_selected'] && empty($project['taxonomy_name'])) {
        form_set_error('projects][' . $key . '][taxonomy_name', t('"Select or enter a new vocabulary" field is required.'));
      }

      if (!powertagging_translation_possible()) {
        foreach ($project['languages'] as $language) {
          if (!empty($language) && $language != LANGUAGE_NONE && $language != $default_language) {
            $drupal_languages = language_list();
            form_set_error('projects][' . $key . '][languages][' . $language, t('Language "%language" requires localization of taxonomies as it is not your default site language.<br /> Install and enable module "!internationalizationlink" and its sub-module "Taxonomy translation" to make multilingual tagging possible.', array(
              '%language' => $drupal_languages[$language]->name,
              '!internationalizationlink' => l(t('Internationalization'), 'https://www.drupal.org/project/i18n', array('attributes' => array('target' => '_blank'))),
            )));
          }
        }
      }
    }
  }
}

/**
 * Submit handler for powertagging_form.
 */
function powertagging_form_submit($form, &$form_state) {
  // Get taxonomy id if a taxonomy name is given for each project.
  if (isset($form_state['values']['projects'])) {
    // Get all existing taxonomies.
    $taxonomy_names = taxonomy_vocabulary_get_names();
    $vids = array();
    foreach ($taxonomy_names as $taxonomy_data) {
      $vids[$taxonomy_data->name] = $taxonomy_data->vid;
    }
    // Go through all projects.
    foreach ($form_state['values']['projects'] as &$project) {
      if (!empty($project['taxonomy_name'])) {
        $taxonomy_name = trim(check_plain($project['taxonomy_name']));
        if (isset($vids[$taxonomy_name])) {
          if (isset($project['remove_taxonomy']) && $project['remove_taxonomy']) {
            taxonomy_vocabulary_delete($vids[$taxonomy_name]);
            drupal_set_message(t('Vocabulary %name has been removed.', array('%name' => $project['taxonomy_name'])));
            unset($project['taxonomy_id']);
            unset($project['taxonomy_name']);
          }
          else {
            // Make sure the already existing vocabulary is able to store
            // PowerTagging tags.
            powertagging_modify_vocabulary($vids[$taxonomy_name]);
            $project['taxonomy_id'] = $vids[$taxonomy_name];
          }
        }
        else {
          // If taxonomy does not exists, it will be created.
          $taxonomy = powertagging_create_vocabulary($taxonomy_name);
          $project['taxonomy_id'] = $taxonomy->vid;
        }
      }
    }
  }

  // Serialize the configuration data.
  $serialized_config = serialize(array(
    'concepts_per_extraction' => $form_state['values']['concepts_per_extraction']['value'],
    'concepts_threshold' => $form_state['values']['concepts_threshold']['value'],
    'freeterms_per_extraction' => $form_state['values']['freeterms_per_extraction']['value'],
    'freeterms_threshold' => $form_state['values']['freeterms_threshold']['value'],
    'projects' => (isset($form_state['values']['projects']) ? $form_state['values']['projects'] : array()),
  ));

  // Always create a new connection, if URL and type are the same the old one
  // will be used anyway.
  $connection = SemanticConnector::createConnection('pp_server', $form_state['values']['url'], $form_state['values']['server_title'], array(
    'username' => $form_state['values']['name'],
    'password' => $form_state['values']['pass'],
  ));

  // Save configuration into database.
  if (isset($form_state['values']['powertagging_id'])) {
    db_update('powertagging')
      ->fields(array(
          'title' => $form_state['values']['title'],
          'connection_id' => $connection->getID(),
          'config' => $serialized_config,
        ))
      ->condition('powertagging_id', $form_state['values']['powertagging_id'])
      ->execute();
  }
  else {
    db_insert('powertagging')
      ->fields(array(
          'title' => $form_state['values']['title'],
          'connection_id' => $connection->getID(),
          'config' => $serialized_config,
        ))
      ->execute();
  }

  // Insert/update skos importer configuration set.
  if (module_exists('skos_importer')) {
    powertagging_skos_importer_config_save($form_state['values']);
  }

  drupal_set_message(t('PowerTagging configuration %title has been saved.', array('%title' => $form_state['values']['title'])));
  $form_state['redirect'] = 'admin/config/services/powertagging';
}

/**
 * List deletion form.
 */
function powertagging_delete_form($form, &$form_state, $conf) {
  $form_state['conf'] = $conf;
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $conf->title)),
    'admin/config/services/powertagging',
    t('This action cannot be undone.'),
    t('Delete configuration'));
}

/**
 * Submit handler for powertagging_delete_form().
 */
function powertagging_delete_form_submit($form, &$form_state) {
  $conf = $form_state['conf'];
  powertagging_conf_delete($conf->powertagging_id);
  drupal_set_message(t('%title has been deleted.', array('%title' => $conf->title)));
  $form_state['redirect'] = 'admin/config/services/powertagging';
}

/**
 * Create a new taxonomy for PowerTagging tags.
 *
 * @param string $name
 *   The human readable name of the new taxonomy.
 *
 * @return object
 *   The created vocabulary object.
 */
function powertagging_create_vocabulary($name) {
  // Create the new Drupal taxonomy.
  $machine_name = powertagging_machine_name($name);
  $vocabulary = (object) array(
    'name' => $name,
    'machine_name' => $machine_name,
    'description' => t('Automatically created by PowerTagging Module'),
    'hierarchy' => 2,
  );
  taxonomy_vocabulary_save($vocabulary);

  // Add required fields for PowerTagging.
  powertagging_modify_vocabulary($vocabulary->vid);

  drupal_set_message(t('Vocabulary %name has been created.', array('%name' => $vocabulary->name)));

  return $vocabulary;
}

/**
 * Modify a taxonomy in a way that it can be used to store PowerTagging tags.
 *
 * @param int $vid
 *   The vocabulary id of the taxonomy to modify.
 */
function powertagging_modify_vocabulary($vid) {
  $taxonomy = taxonomy_vocabulary_load($vid);
  $machine_name = $taxonomy->machine_name;

  // Create an URI field if it does not exist yet.
  $field_name = 'field_uri';
  $field      = field_info_field($field_name);
  if (is_null($field)) {
    $field = array(
      'field_name'  => $field_name,
      'type'        => 'text',
      'label'       => t('URI'),
    );
    field_create_field($field);
  }

  // Attach the URI field to the taxonomy if not yet attached.
  $instance = field_info_instance('taxonomy_term', $field_name, $machine_name);
  if (is_null($instance)) {
    $instance = array(
      'field_name'  => $field_name,
      'entity_type' => 'taxonomy_term',
      'bundle'      => $machine_name,
      'label'       => t('URI'),
      'description' => t('The URI from the concept'),
      'required'    => FALSE,
      'widget'      => array(
        'type'      => 'text_textfield',
        'weight'    => 3,
      ),
    );
    field_create_instance($instance);
  }

  // Create an "alt label" field if it does not exist yet.
  $field_name = 'field_alt_labels';
  $field      = field_info_field($field_name);
  if (is_null($field)) {
    $field = array(
      'field_name'  => $field_name,
      'type'        => 'text',
      'label'       => t('Alternative labels'),
      'description' => t('A comma separated list of synonyms'),
      'settings'    => array(
        'max_length' => 8191,
      ),
    );
    field_create_field($field);
  }

  // Attach the "alt label" field to the taxonomy if not yet attached.
  $instance = field_info_instance('taxonomy_term', $field_name, $machine_name);
  if (is_null($instance)) {
    $instance = array(
      'field_name'  => $field_name,
      'entity_type' => 'taxonomy_term',
      'bundle'      => $machine_name,
      'label'       => t('Alternative labels'),
      'description' => t('A comma separated list of synonyms'),
      'required'    => FALSE,
      'widget'      => array(
        'type'      => 'text_textfield',
        'weight'    => 4,
      ),
    );
    field_create_instance($instance);
  }

  // Make the vocabulary localized for concept translations.
  if (powertagging_translation_possible() && (!isset($taxonomy->i18n_mode) || $taxonomy->i18n_mode != I18N_MODE_LOCALIZE)) {
    db_update('taxonomy_vocabulary')
      ->fields(array('i18n_mode' => I18N_MODE_LOCALIZE))
      ->condition('vid', $taxonomy->vid)
      ->execute();
  }
}

/**
 * Start the indexing-process for a PoolParty project.
 */
function powertagging_indexProject_form($form, &$form_state, $powertagging_conf, $project_id) {
  $field_instances = powertagging_get_tagging_field_instances($powertagging_conf->powertagging_id, $project_id);

  if (!empty($field_instances)) {
    $node_type_names = node_type_get_names();
    $taxonomy_names = taxonomy_vocabulary_get_names();

    $content_type_options = array();
    foreach ($field_instances as $field_instance) {
      // If the entity type is not supported, throw an error and continue.
      if (!in_array($field_instance['entity_type'], array('node', 'user', 'taxonomy_term'))) {
        drupal_set_message(t('Entity type "%entitytype" is not supported in Bulk tagging.', array('%entitytype' => $field_instance['entity_type'])), 'warning');
        continue;
      }

      // Build the title of the option.
      switch ($field_instance['entity_type']) {
        case 'node':
          $option_title = 'Nodes of content type "' . $node_type_names[$field_instance['bundle']] . '" (' . $field_instance['field_name'] . ')';
          break;

        case 'user':
          $option_title = 'Users (' . $field_instance['field_name'] . ')';
          break;

        case 'taxonomy_term':
          $option_title = 'Taxonomy terms of vocabulary "' . $taxonomy_names[$field_instance['bundle']]->name . '" (' . $field_instance['field_name'] . ')';
          break;
      }

      // Add the option.
      $content_type_options[$field_instance['entity_type'] . ' ' . $field_instance['bundle'] . ' ' . $field_instance['field_name']] = $option_title;
    }

    // Fixed values for the formatter.
    $form['powertagging_conf'] = array(
      '#type' => 'value',
      '#value' => $powertagging_conf,
    );
    $form['project_id'] = array(
      '#type' => 'value',
      '#value' => $project_id,
    );

    $form['content_types'] = array(
      '#title' => t('Entity types to be included in the batch process.'),
      '#type' => 'checkboxes',
      '#options' => $content_type_options,
      '#required' => TRUE,
    );

    $form['skip_tagged_content'] = array(
      '#title' => t('Skip already tagged content'),
      '#type' => 'radios',
      '#options' => array(
        '1' => t('Yes'),
        '0' => t('No'),
      ),
      '#default_value' => TRUE,
    );

    $form['entities_per_request'] = array(
      '#type' => 'textfield',
      '#title' => t('Entities per request'),
      '#description' => t('The number of entities, that get processed during one HTTP request. (Allowed value range: 1 - 100)') . '<br />' . t('The higher this number is, the less HTTP requests have to be sent to the server until the batch finished tagging ALL your entities, what results in a shorter duration of the bulk tagging process.') . '<br />' . t('Numbers too high can result in a timeout, which will break the whole bulk tagging process.') . '<br />' . t('If entities are configured to get tagged with uploaded files, a value of 5 or below is recommended.'),
      '#required' => TRUE,
      '#default_value' => '10',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Index',
    );
  }
  else {
    $form['error'] = array(
      '#type' => 'item',
      '#markup' => '<div class="messages error">' . t('No taggable content types found for project "%projectid" in PowerTagging configuration "%ptconfname".', array('%projectid' => $project_id, '%ptconfname' => $powertagging_conf->name)) . '</div>',
    );
  }

  return $form;
}

/**
 * Validate-Handler of the form to index a PoolParty project.
 */
function powertagging_indexProject_form_validate($form, &$form_state) {
  $entities_per_request = $form_state['values']['entities_per_request'];
  if (empty($entities_per_request) || !ctype_digit($entities_per_request) || (int) $entities_per_request == 0 || (int) $entities_per_request > 100) {
    form_set_error('entities_per_request', t('Only values in the range of 1 - 100 are allowed for field "Entities per request"'));
  }
}

/**
 * Submit-Handler of the form to index a PoolParty project.
 */
function powertagging_indexProject_form_submit($form, &$form_state) {
  $content_types = array();
  foreach ($form_state['values']['content_types'] as $content_type_id => $value) {
    if (!empty($value)) {
      $content_types[] = $content_type_id;
    }
  }

  powertagging_indexProject($form_state['values']['powertagging_conf'], $form_state['values']['project_id'], array(
    'content_types' => $content_types,
    'skip_tagged_content' => $form_state['values']['skip_tagged_content'],
    'entities_per_request' => $form_state['values']['entities_per_request'],
  ));
}

/**
 * Start the indexing-process for a PoolParty project.
 */
function powertagging_synchronizeProject_form($form, &$form_state, $powertagging_conf, $project_id) {
  if (module_exists('skos_importer')) {
    // Save source and import.
    $source = skos_importer_source($project_id);

    // Create the config if it does not exist yet.
    if (is_null($source)) {
      powertagging_skos_importer_config_save($form_state['values']);
      $source = skos_importer_source($project_id);
    }

    $ppt_api = $powertagging_conf->connection->getAPI('PPT');
    $file_url = $ppt_api->storeProject($project_id);

    if (!empty($file_url)) {
      $importer = $source->importer();
      $fetcher_conf = $importer->fetcher->getConfig();

      if ($fetcher_conf['source'] != $file_url) {
        // Update the config of the current fetcher instance.
        $fetcher_conf['source'] = $file_url;
        $importer->fetcher->setConfig($fetcher_conf);

        // Save the URL for further import-processes.
        $importer_config = $importer->getConfig();
        $importer_config['fetcher']['fetchers']['SKOSImporterHTTPFetcher']['source'] = $file_url;
        $importer->setConfig($importer_config);
        $importer->save();
      }
    }

    // Start the import.
    $source->startImport();

    batch_process('admin/config/services/powertagging/' . $powertagging_conf->powertagging_id);
  }
  else {
    drupal_set_message(t('The SKOS Importer module is required for taxonomy synchronization.'), 'error');
  }
}

/**
 * Creates the configuration set for the skos importer for each project.
 *
 * @param array $pt_config
 *   The powertagging configuration set.
 *
 * @return bool
 *   TRUE on success, FALSE on fail.
 */
function powertagging_skos_importer_config_save(array $pt_config) {
  if (empty($pt_config['projects'])) {
    return FALSE;
  }

  // Create the RDF/XML URL for the import.
  $http_source = $pt_config['url'];
  if (substr($http_source, -1) == '/') {
    $http_source = substr($http_source, 0, -1);
  }
  $http_source .= '/PoolParty/OpenConfigPathResourceProvider/backuproot?';

  // Create a skos importer configuration set for each project.
  foreach ($pt_config['projects'] as $uuid => $project) {

    // Get selected languages.
    $languages = array();
    if (!$project['no_language_selected']) {
      foreach ($project['languages'] as $d_lang => $pp_lang) {
        if (!empty($pp_lang)) {
          $languages[$d_lang] = $d_lang;
        }
      }
      $languages = array_unique($languages);
    }

    // Create a skos importer object.
    $rdf_filename = preg_replace('/\W+/', '', strtolower($project['label']));
    $rdf_filename = 'pp_project_' . $rdf_filename . '.rdf';
    $importer = skos_importer_importer($uuid);

    // Disable or remove skos importer configuration set if no language is
    // given.
    if (empty($languages)) {
      // Delete importer configuration set and the appropriate taxonomy.
      if ($pt_config['remove_taxonomy']) {
        // $importer->delete();
      }
      // Disable the importer configuration set.
      else {
        /* if ($importer->export_type) {
          $importer->disabled = TRUE;
          $importer->save();
        } */
      }
      continue;
    }

    $si_config = array(
      'name' => $project['label'] . ' (' . $pt_config['title'] . ')',
      'description' => t('Automatically generated by the PowerTagging module'),
      'fetcher' => array(
        'plugin_key' => 'SKOSImporterHTTPFetcher',
        'fetchers' => array(
          'SKOSImporterFileFetcher' => array(
            'fid' => 0,
            'source' => '',
            'upload' => '',
          ),
          'SKOSImporterHTTPFetcher' => array(
            'source' => $http_source . 'fileName=' . $uuid . '/' . $rdf_filename,
          ),
        ),
      ),
      'parser' => array(
        'plugin_key' => 'SKOSImporterTermParser',
        'additional_data' => array(
          'altLabel' => 'altLabel',
          'hiddenLabel' => 0,
          'scopeNote' => 0,
          'related' => 0,
        ),
        // Import all ConceptSchemes and Concepts in one vocabulary.
        'import_type' => 1,
        'vocabulary_name' => $project['taxonomy_name'],
        'multi_language' => (count($languages) <= 1 ? 0 : 1),
        'languages' => $languages,
        'language_mapping' => $project['languages'],
      ),
      'processor' => array(
        'plugin_key' => 'SKOSImporterTermProcessor',
        // Update existing terms: TRUE.
        'update_existing' => 2,
      ),
      // Periodic import: Off.
      'import_period' => -1,
      // Process in background: FALSE.
      'process_in_background' => 0,
    );
    $importer->addConfig($si_config);
    $importer->save();
  }

  return TRUE;
}

/**
 * Create a machine name from a given name.
 *
 * @param string $name
 *   Name of the skos importer.
 *
 * @return string
 *   The machine name.
 */
function powertagging_machine_name($name) {
  return substr(preg_replace('/[^a-z0-9]+/', '_', strtolower($name)), 0, 255);
}
