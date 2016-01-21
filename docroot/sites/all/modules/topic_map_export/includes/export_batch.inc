<?php

/**
 * @file
 * The Topic Map Export Batch. There only one batch in this file and multiple
 * processing functions along with their helper functions.
 *
 * Based on the settings given on the export form, the batch
 * loads, parses and exports every type of information(Entities, Bundles,
 * Fields) that is available and supported in Drupal. Basically most of the
 * module's work is done here.
 *
 * See more info for functions here, see Batch API documentation.
 *
 * @see forms.inc
 */

/**
 * Creates and starts topic map export process.
 *
 * Declares and starts the batch process. Also parses the form settings and
 * does some initial configuration to the batch based on the received settings.
 *
 * @param array $raw_settings
 *   An associative array containing:
 *   - entities: An array selected of entity types.
 *   - bundles: An associative array of selected bundles of the selected
 *   entities.
 *   - associations: An array selected associations that are described between
 *   entities.
 *   Values are generated from topic_map_export_export_form().
 *
 * @ingroup batch
 */
function topic_map_export_topic_map($raw_settings = NULL) {
  $settings = array(
    'entities' => (isset($raw_settings['entities'])) ? $raw_settings['entities'] : array(),
    'bundles' => array(),
    'associations' => (isset($raw_settings['associations'])) ? $raw_settings['associations'] : array(),
  );

  foreach ($settings['entities'] as $key => $value) {
    if ($value && isset($raw_settings['bundle_' . $key])) {
      $settings['bundles'][$key] = $raw_settings['bundle_' . $key];
    }
  }

  variable_set('topic_map_export_batch_settings', $settings);

  // Batch declaration.
  $batch = array(
    'finished' => 'topic_map_export_finished',
    'title' => t('Exporting topic map'),
    'operations' => array(),
    'init_message' => t('Initializing topic map export.'),
    'progress_message' => t('Processed @current of @total.'),
    'error_message' => t('A error has occurred during export.'),
    'file' => drupal_get_path('module', 'topic_map_export') . '/includes/export_batch.inc',
  );

  $file = file_create_filename('topic_map_' . time() . '.xtm', 'public://');
  $batch['operations'][] = array('topic_map_export_prepare_processing', array($file));

  foreach ($settings['entities'] as $type => $enabled) {
    if ($enabled) {
      $batch['operations'][] = array('topic_map_export_process_entities', array($type));
    }
  }

  _topic_map_export_include_hook_files();
  $associations = module_invoke_all('topic_map_export_association_info');
  drupal_alter('topic_map_export_association_info', $associations);

  $available_associations = array();

  foreach ($associations as $info) {
    $key = _topic_map_export_create_key_from_association($info);
    $available_associations[$key] = $info;
  }

  foreach ($settings['associations'] as $association_key => $enabled) {
    if ($enabled) {
      $batch['operations'][] = array('topic_map_export_process_associations', array($available_associations[$association_key]));
    }
  }


  $batch['operations'][] = array('topic_map_export_process_create_topic_map', array());

  batch_set($batch);
  batch_process();
}

/**
 * Initial processing function of the export.
 *
 * Creates XTM file and adds header, content types, entities(not instances),
 * association types and occurrence types to it.
 *
 * @param string $filepath
 *   Path to the XTM file where Topic Map will be exported.
 *
 * @ingroup batch
 */
function topic_map_export_prepare_processing($filepath, &$context) {
  $settings = variable_get('topic_map_export_batch_settings');
  $context['results'] = array('topics' => array(), 'associations' => array());
  $context['results']['topic_map'] = $filepath;

  $file = fopen($filepath, 'w');

  $sitename = variable_get('site_name', 'Drupal');

  $entities = entity_get_info();

  _topic_map_export_include_hook_files();
  $associations = module_invoke_all('topic_map_export_association_info');
  drupal_alter('topic_map_export_association_info', $associations);

  $association_definitions = array();

  foreach ($associations as $info) {
    $as_key = _topic_map_export_create_key_from_association($info);
    if (!$settings['associations'][$as_key]) {
      continue;
    }
    foreach ($info['definitions'] as $type => $group) {
      foreach ($group as $key => $value) {
        $association_definitions[$key] = $value;
      }
    }
  }

  // Variant languages.
  $variant_types = array();

  $variant_types[] = array(
    'topic_id' => TOPIC_MAP_EXPORT_DISPLAY_TOPIC_ID,
    'name' => 'Display',
    'si' => array(TOPIC_MAP_EXPORT_DISPLAY_TOPIC_SI),
    'instance_of' => array(),
  );

  $variant_types[] = array(
    'topic_id' => TOPIC_MAP_EXPORT_VARIANT_LANG_ID,
    'name' => 'Language',
    'si' => array(TOPIC_MAP_EXPORT_VARIANT_LANG_SI),
    'instance_of' => array(),
  );
  $languages = language_list();
  foreach ($languages as $key => $cur_lang) {
    $variant_types[] = array(
      'topic_id' => TOPIC_MAP_EXPORT_VARIANT_LANG_ID . '_' . $key,
      'name' => $cur_lang->name,
      'si' => array(TOPIC_MAP_EXPORT_VARIANT_LANG_SI . '#' . $key),
      'instance_of' => array(TOPIC_MAP_EXPORT_VARIANT_LANG_ID),
    );
  }

  // Occurence types.
  $occurrence_sets = array();

  foreach ($entities as $type => $info) {
    $bundles = field_info_bundles($type);
    foreach ($bundles as $bundle_key => $data) {
      $occurrence_sets[] = field_info_instances($type, $bundle_key);
    }
  }

  foreach ($entities as $key => $value) {
    if (!isset($settings['entities'][$key]) || !$settings['entities'][$key]) {
      unset($entities[$key]);
    }
  }

  foreach ($settings['bundles'] as $key => $bundle_set) {
    if (isset($entities[$key])) {
      foreach ($entities[$key]['bundles'] as $bundle_key => $bundle_value) {
        if (!isset($bundle_set[$bundle_key]) || !$bundle_set[$bundle_key]) {
          unset($entities[$key]['bundles'][$bundle_key]);
        }
      }
    }
  }

  fwrite($file, theme('topic_map_header', array('site_name' => $sitename)));
  fwrite($file, theme('topic_map_entities_and_bundles', array('entities' => $entities)));

  foreach ($variant_types as $key => $variant) {
    fwrite($file, theme('topic_map_topic', array(
      'topic_id' => $variant['topic_id'],
      'name' => $variant['name'],
      'si' => $variant['si'],
      'instance_of' => $variant['instance_of'],
    )));
  }

  foreach ($occurrence_sets as $bundle => $occurrence_set) {
    fwrite($file, theme('topic_map_occurence_types', array('occurrences' => $occurrence_set)));
  }

  fwrite($file, theme('topic_map_roles_and_association_types', array('association_definitions' => $association_definitions)));

  fclose($file);
}

/**
 * Goes trough all Drupal entities and creates topics out of them.
 *
 * @param string $type
 *   Current entity type being handled by processing function.
 *
 * @ingroup batch
 */
function topic_map_export_process_entities($type, &$context) {

  $settings = variable_get('topic_map_export_batch_settings');
  $entity_info = entity_get_info($type);


  if (!isset($context['sandbox']['current_entity'])) {

    $context['sandbox']['current_entity'] = 0;

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $type);
    $result = $query->execute();

    if (empty($result)) {
      $context['finished'] = 1;
      return;
    }

    $context['sandbox']['entities'] = array_keys($result[$type]);

    $context['sandbox']['max'] = count($context['sandbox']['entities']);
    $context['finished'] = 0;

    return;
  }

  $limit = 10;

  $max = 0;

  $identifiers = $context['sandbox']['entities'];

  $entities = array();

  try {
    if ($context['sandbox']['max'] > ($context['sandbox']['current_entity'] + $limit)) {
      $max = $context['sandbox']['current_entity'] + $limit;
      $entities = entity_load($type, array_slice($identifiers, $context['sandbox']['current_entity'], $limit));
    }
    else {
      $entities = entity_load($type, array_slice($identifiers, $context['sandbox']['current_entity'],
        $context['sandbox']['max'] - $context['sandbox']['current_entity']));
    }
  }
  catch (Exception $e) {
    watchdog_exception('topic_map_export', $e);
  }

  if (empty($entities)) {
    $context['finished'] = 1;
    return;
  }

  $context['sandbox']['current_entity'] = min(array($context['sandbox']['max'], $context['sandbox']['current_entity'] + $limit));

  foreach ($entities as $key => $entity) {
    $bundle_key = $entity_info['entity keys']['bundle'];


    if (!empty($bundle_key)) {
      $entity_bundle = $entity->$bundle_key;

      // If the bundle of the current entity was not selected then skip its
      // processing.
      if (!$settings['bundles'][$type][$entity_bundle]) {
        continue;
      }
    }

    $topic = _topic_map_export_create_topic($entity, $type);
    $context['results']['topics'][$topic->topic_id] = $topic;

  }

  if ($context['sandbox']['max'] != $context['sandbox']['current_entity']) {
    $context['finished'] = $context['sandbox']['current_entity'] / $context['sandbox']['max'];
  }

}

/**
 * Goes trough the created topics and creates associations between them.
 *
 * @param array $association_info
 *   An associative array containing the definition for an association type.
 *
 * @ingroup batch
 */
function topic_map_export_process_associations($association_info, &$context) {

  _topic_map_export_include_hook_files();

  if (!isset($context['sandbox']['current_topic'])) {

    $context['sandbox']['current_topic'] = 0;
    $context['sandbox']['keys'] = array_keys($context['results']['topics']);
    $context['sandbox']['max'] = count($context['results']['topics']);

    if (!isset($context['results']['associations'])) {
      $context['results']['associations'] = array();
    }

    if (isset($association_info['requirements']['field'])) {

      $context['sandbox']['fields'] = array();
      $fields_info = field_info_fields();
      foreach ($fields_info as $field_name => $field) {

        if ($field['module'] == $association_info['requirements']['field']['module']
        && $field['type'] == $association_info['requirements']['field']['type']) {
          $context['sandbox']['fields'][$field_name] = $field;
        }
      }

    }

    $context['finished'] = 0;

    return;
  }


  $limit = 5;

  $topic_ilength = min(array($context['sandbox']['max'], $context['sandbox']['current_topic'] + $limit));

  for ($i = $context['sandbox']['current_topic']; $i < $topic_ilength; $i++) {
    $key = $context['sandbox']['keys'][$i];
    $topic = $context['results']['topics'][$key];

    try {
      $entity = entity_load($topic->type, array($topic->entity_id));
    }
    catch (Exception $e) {
      watchdog_exception('topic_map_export', $e);
      throw $e;
    }
    $entity = reset($entity);
    $association_handler_args = array($topic, $entity);

    if (isset($context['sandbox']['fields'])) {

      $field_found = FALSE;

      foreach ($context['sandbox']['fields'] as $field_name => $field) {
        if (isset($entity->$field_name)) {
          $field_items = field_get_items($topic->type, $entity, $field_name);
          if ($field_items) {
            $association_handler_args[] = $field_items;
            $field_found = TRUE;
          }

        }
      }

      if (!$field_found) {
        continue;
      }
    }

    if (!isset($association_info['handler_entity_type']) ||
      isset($association_info['handler_entity_type']) &&
      $association_info['handler_entity_type'] == $topic->type) {

      $associations = call_user_func_array($association_info['handler'], $association_handler_args);

      if (!empty($associations)) {

        foreach ($associations as $data) {

          $used_roles = array();
          $members = array();
          foreach ($data as $member_data) {
            $member = new stdClass();

            try {
              $entity = entity_load($member_data['type'], array($member_data['id']));
              $entity = reset($entity);
              list($id, $vid, $bundle) = entity_extract_ids($member_data['type'], $entity);
            }
            catch (Exception $e) {
              watchdog_exception('topic_map_export', $e);
            }

            if (!isset($entity) || empty($entity)) {
              continue;
            }

            $member->role = 'entity_' . $member_data['type'] . '-bundle_' . $bundle;
            if (isset($used_roles[$member->role])) {
              $used_roles[$member->role]++;
              $member->role .= '#' . $used_roles[$member->role];
            }
            else {
              $used_roles[$member->role] = 1;
            }

            $member->actor = $member_data['type'] . '-' . $id;
            $members[] = $member;
          }
          $context['results']['associations'][] = _topic_map_export_create_association($members, key($association_info['definitions']['class']));
        }
      }

    }

  }

  $context['sandbox']['current_topic'] = $topic_ilength;

  if ($context['sandbox']['max'] != $context['sandbox']['current_topic']) {
    $context['finished'] = $context['sandbox']['current_topic'] / $context['sandbox']['max'];
  }

}

/**
 * Finalizes the export and writes results into a XTM file.
 *
 * Last batch processing function. Goes through every created topic and
 * association and writes them into the XTM file.
 *
 * @ingroup batch
 */
function topic_map_export_process_create_topic_map(&$context) {
  if (!isset($context['sandbox']['current_topic'])) {
    $context['sandbox']['current_topic'] = 0;
    $context['sandbox']['current_association'] = 0;

    $context['finished'] = 0;

    $context['sandbox']['topic_keys'] = array_keys($context['results']['topics']);

    $context['sandbox']['max_topic'] = count($context['sandbox']['topic_keys']);
    $context['sandbox']['max_association'] = count($context['results']['associations']);

    $context['finished'] = 0;
    return;
  }

  $limit = 10;

  // Begin top insertion.
  $topic_ilength = min(array($context['sandbox']['max_topic'], $context['sandbox']['current_topic'] + $limit));

  $file = fopen($context['results']['topic_map'], 'a+');

  for ($i = $context['sandbox']['current_topic']; $i < $topic_ilength; $i++) {
    $key = $context['sandbox']['topic_keys'][$i];

    $topic = $context['results']['topics'][$context['sandbox']['topic_keys'][$i]];

    try {
      $topic_render = theme('topic_map_topic_entity', array('topic' => $topic));
      fwrite($file, $topic_render);
    }
    catch (Exception $e) {
      watchdog_exception('topic_map_export', $e);
    }
  }

  fclose($file);

  $context['sandbox']['current_topic'] = $topic_ilength;

  // Begin association insertion.
  if ($context['sandbox']['current_topic'] == $context['sandbox']['max_topic']) {
    $asso_ilength = min(array($context['sandbox']['max_association'], $context['sandbox']['current_association'] + $limit));

    $file = fopen($context['results']['topic_map'], 'a');

    for ($i = $context['sandbox']['current_association']; $i < $asso_ilength; $i++) {
      if (!$context['results']['associations'][$i]) {
        continue;
      }

      $asso = $context['results']['associations'][$i];

      try {
        $association_render = theme('topic_map_association', array(
         'type' => $asso->type,
         'roles' => $asso->members,
        ));
        fwrite($file, $association_render);
      }
      catch (Exception $e) {
        watchdog_exception('topic_map_export', $e);
      }
    }

    fclose($file);

    $context['sandbox']['current_association'] = $asso_ilength;
  }

  // End association insertion.
  $totalmax = $context['sandbox']['max_topic'] + $context['sandbox']['max_association'];
  $totalcurrent = $context['sandbox']['current_topic'] + $context['sandbox']['current_association'];

  if ($totalcurrent != $totalmax) {
    $context['finished'] = $totalcurrent / $totalmax;
  }
}

/**
 * Finishes the topic map export batch.
 *
 * @ingroup batch
 */
function topic_map_export_finished($success, $results, $operations) {

  global $user;

  $file = fopen($results['topic_map'], 'a');
  fwrite($file, '</topicMap>');
  fclose($file);

  // Make a file entry to Drupal's database.
  $file = new stdClass();
  $file->uid = $user->uid;
  $file->filename = basename($results['topic_map']);
  $file->uri = $results['topic_map'];
  $file->filemime = file_get_mimetype($results['topic_map']);
  $file->filesize = @filesize($results['topic_map']);
  $file->timestamp = REQUEST_TIME;
  $file->status = 0;

  file_save($file);

  $msg = t('The exported topic map can be found !link_starthere!link_end.', array(
    '!link_start' => '<a href="' . file_create_url($results['topic_map']) . '">',
    '!link_end' => '</a>',
  ));

  drupal_set_message($msg);
  variable_del('topic_map_export_batch_settings');
}
