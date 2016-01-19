<?php
/**
 * @file
 * Functions related to Solr indexing
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * For multiple value sortable field index the first instance
 * @var int
 */
define('XC_SOLR_SORT_STRATEGY_FIRST', 2);

/**
 * For multiple value sortable field index the first minimal and the maximal values
 * @var int
 */
define('XC_SOLR_SORT_STRATEGY_MINMAX', 1);

/**
 * Metadata process strategy: merge all metadata first then call indexing (1)
 * @var int
 */
define('XC_SOLR_METADATA_PROCESS_STRATEGY_MERGE_FIRST', 1);

/**
 * Metadata process strategy: index metadata as soon as possible (2)
 * @var int
 */
define('XC_SOLR_METADATA_PROCESS_STRATEGY_INDEX_FIRST', 2);

/**
 * Displays the schema field to Solr fields mapping
 */
function xc_solr_show_fields2index_list() {
  variable_del('xc_solr_fields2index_list');
  $list = xc_solr_fields2index_list();

  $rows = array();
  foreach ($list as $schema_field => $targets) {
    $cells = array();
    $cells[] = $schema_field;
    $output = array();
    foreach ($targets as $type => $solr_fields) {
      if ($type == 'value_as_key') {
        // array ('value_as_key' => array('type' => array('values' => array('@gtt ' => array('xc__subject_gtt _t')))),
        foreach ($solr_fields as $attribute => $properties) {
          foreach ($properties['values'] as $property => $solr_fields2) {
            $output[] = '@' . $attribute . '=' . $property . ': ' . join(', ', $solr_fields2);
          }
        }
      }
      else {
        $output[] = $type . ': ' . join(', ', $solr_fields);
      }
    }
    $cells[] = join('<br />', $output);

    $rows[] = $cells;
  }
  return theme('table', array('header' => array(t('Schema field'), t('Solr fields')), 'rows' => $rows));
}

/**
 * Displays a confirmation form for launching the one-stop Solr indexer
 *
 * @param $form_state (Array)
 *   The FAPI form state array
 */
function xc_solr_onestop_indexer_batch_launcher_form($form, &$form_state, $xc_location = NULL) {
  // TODO: check whether Solr is runing
  $form['xc_location'] = array(
    '#type' => 'value',
    '#value' => serialize($xc_location),
  );

  return confirm_form(
    array($form),
    t('Are you sure, that you would like to launch indexing metadata for search?'),
    'admin/xc/solr', // path to go if user click on 'cancel'
    t('This action cannot be undone.'),
    t('Index metadata'),
    t('Cancel')
  );
}

/**
 * Submission handler for the confirmation form for launching the one-stop Solr indexer.
 *
 * @param $form (Array)
 *   The FAPI form array
 * @param $form_state (Array)
 *   The FAPI form state array
 */
function xc_solr_onestop_indexer_batch_launcher_form_submit($form, &$form_state) {
  global $_oaiharvester_statistics;

  $xc_location = unserialize($form_state['values']['xc_location']);
  variable_del('oaiharvester_statistics');
  variable_del('xc_solr_fields2index_list');
  variable_del('xc_solr_sortable_fields');
  $_oaiharvester_statistics = array();

  xc_solr_onestop_indexer_batch_launcher();
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_solr_onestop_indexer_batch_launcher() {
  drupal_set_message('xc_solr_onestop_indexer_batch_launcher');
  if (!function_exists('xc_location_get')) {
    include_once drupal_get_path('module', 'xc_metadata') . '/xc_metadata.location.inc';
  }
  variable_set('xc_solr_fields2index_list_applied', xc_solr_fields2index_list('simple'));
  $locations = xc_location_get(1);
  $batch = array(
    'operations' => array(array('xc_solr_onestop_indexer_batch', array(NULL, array($locations), -1, -1))),
    'title' => t('Indexing Solr'),
    'init_message' => t('Initialize reindexing process'),
    'progress_message' => t('Keep on running'),
    'finished' => 'xc_solr_onestop_indexer_batch_finished',
  );
  batch_set($batch);
  batch_process('admin/xc/solr'); // index?
}

/**
 * Creates Solr document from an entity, and its relationships
 *
 * @param $metadata_id (int)
 *   The metadata_id of an entity
 * @param $clear_cache (boolean)
 *   Whether the metadata is the first one in a row (needs for emtying static variables)
 *
 * @return (object)
 *   An Apache_Solr_DocumentXc object
 */
function xc_solr_onestop_indexer($metadata_id, $clear_cache = FALSE) {
  global $_oaiharvester_statistics;
  static $commons;

  if (!isset($commons)) {
    $commons = new stdClass();
  }

  if (!isset($commons->metadata_process_strategy)) {
    $commons->metadata_process_strategy = xc_solr_get_metadata_process_strategy();
  }

  if (!isset($commons->conflicting_fields)) {
    $commons->conflicting_fields = xc_solr_get_conflicting_fields();
    $commons->indexes = xc_solr_fields2index_list();
    $commons->bibid = array('@type' => xc_solr_get_bibid());
  }
  $conflicting_fields_cache = array();

  $conditional_facets = array();
  // iterate manifestations
  $manifestation = xc_solr_get_metadata($metadata_id);

  $metadata = $manifestation->metadata;
  $manifestation->bibid = xc_util_get_metadata_value($metadata['xc:recordID'], $commons->bibid);

  if ($commons->metadata_process_strategy == XC_SOLR_METADATA_PROCESS_STRATEGY_INDEX_FIRST) {
    $document = xc_solr_metadata_to_solr_iterate_indexes($manifestation, $conditional_facets);
  }
  elseif (isset($commons->conflicting_fields['manifestation'])) {
    foreach ($commons->conflicting_fields['manifestation'] as $schema_field => $solr_field) {
      if ($commons->indexes[$schema_field]['value_as_key']) {
        $conflicting_fields_cache[$solr_field]
          = xc_solr_index_general_value_as_key($metadata, $schema_field, $commons->indexes[$schema_field]);
      }
    }
  }

  // iterate holdings
  $holdings_ids = xc_entity_get_child_identifiers($manifestation->identifier_int);
  foreach ($holdings_ids as $holdings_identifier_int) {
    $holdings = xc_solr_get_metadata_by_identifier_int($holdings_identifier_int);

    if (is_array($holdings->metadata)) {
      $ft1 = microtime(TRUE);
      $metadata = array_merge_recursive($metadata, $holdings->metadata);
      $ft2 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/01 metadata merge', $ft2 - $ft1);
    }

    if ($commons->metadata_process_strategy == XC_SOLR_METADATA_PROCESS_STRATEGY_INDEX_FIRST) {
      xc_solr_metadata_to_solr_iterate_indexes($holdings, $conditional_facets, $document);
    }
    elseif (isset($commons->conflicting_fields['holdings'])) {
      foreach ($commons->conflicting_fields['holdings'] as $schema_field => $solr_field) {
        if ($commons->indexes[$schema_field]['value_as_key']) {
          $conflicting_fields_cache[$solr_field]
            = xc_solr_index_general_value_as_key($holdings->metadata, $schema_field, $commons->indexes[$schema_field]);
        }
      }
    }
  }

  // $expression_ids = xc_solr_get_parent($manifestation);
  $expressions = xc_solr_get_parent($manifestation, $clear_cache);

  foreach ($expressions as $expression) {
    if ($expression === FALSE) {
      xc_log_error('solr', 'empty expression in: ' . var_export($expressions, TRUE) . ', ' . var_export($manifestation->metadata, TRUE));
      continue;
    }

    if (is_array($expression->metadata)) {
      $ft1 = microtime(TRUE);
      $metadata = array_merge_recursive($metadata, $expression->metadata);
      $ft2 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/01 metadata merge', $ft2 - $ft1);
    }

    if ($commons->metadata_process_strategy == XC_SOLR_METADATA_PROCESS_STRATEGY_INDEX_FIRST) {
      xc_solr_metadata_to_solr_iterate_indexes($expression, $conditional_facets, $document);
    }
    elseif (isset($commons->conflicting_fields['expression'])) {
      foreach ($commons->conflicting_fields['expression'] as $schema_field => $solr_field) {
        if ($commons->indexes[$schema_field]['value_as_key']) {
          $conflicting_fields_cache[$solr_field]
            = xc_solr_index_general_value_as_key($expression->metadata, $schema_field, $commons->indexes[$schema_field]);
        }
      }
    }
    $works = xc_solr_get_parent($expression, $clear_cache);

    foreach ($works as $work) {
      if ($work === FALSE) {
        xc_log_error('solr', 'empty work in: ' . var_export($works, TRUE));
        continue;
      }

      if (is_array($work->metadata)) {
        $ft1 = microtime(TRUE);
        $metadata = array_merge_recursive($metadata, $work->metadata);
        $ft2 = microtime(TRUE);
        xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/01 metadata merge', $ft2 - $ft1);
      }

      if ($commons->metadata_process_strategy == XC_SOLR_METADATA_PROCESS_STRATEGY_INDEX_FIRST) {
        xc_solr_metadata_to_solr_iterate_indexes($work, $conditional_facets, $document);
      }
      elseif (isset($commons->conflicting_fields['work'])) {
        foreach ($commons->conflicting_fields['work'] as $schema_field => $solr_field) {
          if ($commons->indexes[$schema_field]['value_as_key']) {
            $conflicting_fields_cache[$solr_field]
              = xc_solr_index_general_value_as_key($work->metadata, $schema_field, $commons->indexes[$schema_field]);
          }
        }
      }
    }
  }

  if ($commons->metadata_process_strategy == XC_SOLR_METADATA_PROCESS_STRATEGY_MERGE_FIRST) {
    $manifestation->metadata = $metadata;
    $document = xc_solr_metadata_to_solr_iterate_indexes($manifestation, $conditional_facets);

    // adding conflicting fields
    if (!empty($conflicting_fields_cache)) {
      foreach ($conflicting_fields_cache as $solr_field => $values) {
        $document->addField($solr_field, $values, FALSE, TRUE);
      }
    }
  }

  $document->addField('metadata_tsn', serialize($metadata), FALSE, TRUE);
  $document->setField('text', join(' ', $document->text));
  $document->setField('highlight_txt', join(' ', $document->highlight_txt));

  $ft1 = microtime(TRUE);

  // format facet (it is a special case of facets)
  // TODO: convert this to normal hook or DB based conditions
  xc_index_format_facet($document, $metadata);
  $ft2 = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/02 format facet', $ft2 - $ft1);

  // general case for facet hooks (hook_xc_process_facet_[facetname])
  if (!empty($conditional_facets)) {
    foreach ($conditional_facets as $facet_name => $schema_field_names) {
      // retrieve saved hooks
      if (!isset($commons->field_hooks)) {
        $commons->field_hooks = variable_get('xc_solr_field_hooks', array());
      }

      // register new hooks
      if (!isset($commons->field_hooks[$facet_name])) {
        $commons->field_hooks[$facet_name] = array();
        $hook = 'xc_process_facet_' . $facet_name;
        foreach (module_implements($hook) as $module) {
          $commons->field_hooks[$facet_name][] = $module . '_' . $hook;
        }
        variable_set('xc_solr_field_hooks', $commons->field_hooks);
      }

      // run hooks
      foreach ($commons->field_hooks[$facet_name] as $function) {
        $function($document, $metadata, array_keys($schema_field_names));
      }
    }
  }
  $ft3 = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/03 conditional facets 1 - hooks', $ft3 - $ft2);

  // general case for conditional facet existing in database
  if (!isset($commons->conditional_facets)) {
    require_once drupal_get_path('module', 'xc_index') . '/xc_index_facet.inc';
    $commons->conditional_facets = xc_index_facet_list_conditionals(TRUE);
    $commons->conditional_facets_file_required = FALSE;
  }

  if (!empty($commons->conditional_facets)) {
    foreach ($commons->conditional_facets as $facet) {
      if ($facet->fn !== FALSE) {
        // require necessary file
        if (!$commons->conditional_facets_file_required) {
          require_once drupal_get_path('module', 'xc_index') . '/xc_index_facet_mapping.inc';
          $commons->conditional_facets_file_required = TRUE;
        }

        $mapping = xc_index_facet_for_edit_load($facet->name);
        $function = $facet->fn;
        //TODO from Tom
        //$values = $function(&$document, $metadata, $mapping->fields);
        $values = $function($document, $metadata, $mapping->fields);
      }
    }
  }
  $ft4 = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/04 conditional facets 2 - forms', $ft4 - $ft3);

  if (!isset($commons->sortable_fields)) {
    $commons->sortable_fields = variable_get('xc_solr_sortable_fields', NULL);
    if (is_null($commons->sortable_fields)) {
      if (!function_exists('xc_index_sortables_get_all')) {
        require_once drupal_get_path('module', 'xc_index') . '/xc_index_sortables.inc';
      }
      $fields = xc_index_sortables_get_all();
      if (!empty($fields)) {
        $suffixes = array_diff(array_keys(xc_solr_field_type_get_types()), array('n/a'));
        $regex = '/^(.*?)(' . join('|', $suffixes) . ')$/';
        $commons->sortable_fields = array();
        foreach ($fields as $field) {
          if (preg_match($regex, $field, $matches)) {
            $commons->sortable_fields[$field] = array(
              'name' => $matches[1],
              'suffix' => $matches[2],
            );
          }
          else {
            $commons->sortable_fields[$field] = array(
              'name' => $field,
              'suffix' => '',
            );
          }
        }
      }
      else {
        $commons->sortable_fields = array();
      }
      variable_set('xc_solr_sortable_fields', $commons->sortable_fields);
    }
  }

  if (!empty($commons->sortable_fields)) {
    $sort_strategy = xc_solr_get_sortstrategy();
    foreach ($commons->sortable_fields as $field => $properties) {
      if (isset($document->$field)) {
        $instances = $document->$field;
        if (!empty($instances)) {
          $suffix = ($properties['suffix'] == '_dt' || $properties['suffix'] == '_df') ? '_sdt' : '_ss';
          $len = count($instances) - 1;
          if ($sort_strategy == XC_SOLR_SORT_STRATEGY_MINMAX) {
            sort($instances);
            for ($i = 0; $i <= $len; $i++) {
              if ($instances[$i] != '') {
                $document->addField($properties['name'] . '_min' . $suffix, $instances[$i]);
                break;
              }
            }
            for ($i = $len; $i >= 0; $i--) {
              if ($instances[$i] != '') {
                $document->addField($properties['name'] . '_max' . $suffix, $instances[$i]);
                break;
              }
            }
          }
          elseif ($sort_strategy == XC_SOLR_SORT_STRATEGY_FIRST) {
            for ($i = 0; $i <= $len; $i++) {
              if ($instances[$i] != '') {
                $document->addField($properties['name'] . $suffix, $instances[$i]);
                break;
              }
            }
          }
        }
      }
    }
  }
  $ft5 = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/05 sortable fields', $ft5 - $ft4);

  // cleanups
  unset($manifestation);
  if (isset($works) && is_array($works)) {
    foreach ($works as $work) {
      unset($work);
    }
  }
  if (isset($expressions) && is_array($expressions)) {
    foreach ($expressions as $expression) {
      unset($expression);
    }
  }
  /*
  //this $holdings is out of scope.
  if (is_array($holdings)) {
    foreach ($holdings as $holding) {
      unset($holding);
    }
  }*/

  return $document;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_solr_get_metadata($metadata_id) {
  global $_oaiharvester_statistics;
  static $sql;

  $t = microtime(TRUE);
  if (empty($metadata_id)) {
    return FALSE;
  }
  // xc_log_info('solr', 'get #' . $metadata_id);

  $entity = db_query('SELECT e.identifier, e.identifier_int, e.node_id, e.node_type, e.metadata_id, e.metadata_type, e.source_id,
            m.metadata AS metadata
            FROM {xc_entity_properties} AS e
            LEFT JOIN {xc_sql_metadata} AS m ON (e.metadata_id = m.metadata_id)
            WHERE e.metadata_id = ' . $metadata_id)->fetchObject();
  $metadata = unserialize($entity->metadata);
  if ($metadata == FALSE) {
    xc_log_error('solr', 'Wrong serialization #' . $metadata_id . ': ' . $entity->metadata);
  }
  $entity->metadata = $metadata;
  $t2 = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/01 xc_solr_get_metadata', ($t2 - $t));

  //drupal_set_message(var_export($entity, TRUE));
  return $entity; // $source_id,
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_solr_get_metadata_by_identifier($identifier, $identifier_prefix) {
  $identifier_int = xc_util_get_identifier_int($identifier, $identifier_prefix);
  return xc_solr_get_metadata_by_identifier_int($identifier_int);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_solr_get_metadata_by_identifier_int($identifier_int) {
  global $_oaiharvester_statistics, $_xc_solr_indexing_statistics;
  static $queries;

  $t = microtime(TRUE);
  if (empty($identifier_int)) {
    $_xc_solr_indexing_statistics['empty']++;
    return FALSE;
  }
  // xc_log_info('solr', 'get #' . $metadata_id);
  $ts1 = microtime(TRUE);
  if (!isset($queries)) {
    $queries = (object) array(
      'strategy' => 'join', // or it may be 'distinct'
      // SQL for JOIN
      'join' => 'SELECT e.identifier, e.identifier_int, e.node_id, e.node_type, e.metadata_id, e.metadata_type, e.source_id,
                     m.metadata AS metadata
                     FROM {xc_entity_properties} AS e
                     LEFT JOIN {xc_sql_metadata} AS m ON (e.metadata_id = m.metadata_id)
                     WHERE e.identifier_int = :identifier',
      // SQL for getting entity properties
      'entity' => 'SELECT identifier, identifier_int, node_id, node_type, metadata_id, metadata_type, source_id
                     FROM {xc_entity_properties}
                     WHERE identifier_int = :identifier',
      // SQL for getting metadata
      'metadata' => 'SELECT metadata FROM {xc_sql_metadata} WHERE metadata_id = :metadata_id',
    );
  }

  if ($queries->strategy == 'join') {
    // TODO Please convert this statement to the D7 database API syntax.
    // TODO from Tom
    $entity = db_query($queries->join,array(':identifier'=>$identifier_int))->fetchObject();
    $ts2 = microtime(TRUE);
    xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_solr_get_metadata_by_identifier/sql', ($ts2 - $ts1));
    if ($entity != FALSE) {
      $tu1 = microtime(TRUE);
      $metadata = unserialize($entity->metadata);
      $tu2 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_solr_get_metadata_by_identifier/unserialize', ($tu2 - $tu1));
      if ($metadata == FALSE) {
        $_xc_solr_indexing_statistics['metadata']++;
        xc_log_error('solr', 'Wrong serialization #' . $identifier . ' (' . $identifier_int . '): ' . $entity->metadata);
      }
      else {
        $_xc_solr_indexing_statistics['success']++;
      }
      $entity->metadata = $metadata;
    }
    else {
      $_xc_solr_indexing_statistics['entity']++;
    }
  }
  elseif ($queries->strategy == 2) { // 'distinct'
    // TODO Please convert this statement to the D7 database API syntax.
    $entity = db_query($queries->entity, array(':identifier'=>$identifier_int))->fetchObject();
    $ts2 = microtime(TRUE);
    xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_solr_get_metadata_by_identifier/01 get entity', ($ts2 - $ts1));

    if ($entity != FALSE) {
      // TODO Please convert this statement to the D7 database API syntax.
      // TODO from Tom, need to convert this query
      //$entity->metadata = db_query('SELECT metadata FROM {xc_sql_metadata} WHERE metadata_id = ' . $entity->metadata_id)->fetchField();
      $entity->metadata = db_query($queries->metadata, array(':metadata_id' => $entity->metadata_id))->fetchField();
      $ts3 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_solr_get_metadata_by_identifier/02 get metadata', ($ts3 - $ts2));

      $tu1 = microtime(TRUE);
      $metadata = unserialize($entity->metadata);
      $tu2 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_solr_get_metadata_by_identifier/03 unserialize', ($tu2 - $tu1));
      if ($metadata == FALSE) {
        $_xc_solr_indexing_statistics['metadata']++;
        xc_log_error('solr', 'Wrong serialization #' . $identifier . ' (' . $identifier_int . '): ' . $entity->metadata);
      }
      else {
        $_xc_solr_indexing_statistics['success']++;
      }
      $entity->metadata = $metadata;
    }
    else {
      $_xc_solr_indexing_statistics['entity']++;
    }
  }
  $t2 = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_solr_get_metadata_by_identifier', ($t2 - $t));

  return $entity; // $source_id,
}

/**
 * Gets the parent of an entiry
 *
 * @param $entity (object)
 *   An XCEntity object
 * @param $clear_cache (boolean)
 *   Whether to clear the static cache
 */
function xc_solr_get_parent($entity, $clear_cache = FALSE) {
  global $_oaiharvester_statistics;
  $t_start = microtime(TRUE);
  $entities = array();

  $t_id_start = microtime(TRUE);
  $parents = xc_entity_get_parent_identifiers($entity->identifier_int, $clear_cache);
  $t_id_end = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent/xc_entity_get_parent_identifiers', ($t_id_end - $t_id_start));
  if ($parents) {
    foreach ($parents as $identifier_int) {
      // $metadata = xc_solr_get_metadata_by_identifier_int($identifier, $identifier_prefix);
      $parent_entity = xc_solr_get_metadata_by_identifier_int($identifier_int);
      if ($parent_entity === FALSE || empty($parent_entity)) {
        xc_log_error('solr', 'No entity for ' . $identifier_int . ' (parent of a ' . $entity->metadata_type . ')');
      }
      else {
        $entities[] = $parent_entity;
      }
    }
  }
  else {
    if ($entity->metadata_type == 'manifestation') {
      $parents = $entity->metadata['xc:expressionManifested'];
    }
    elseif ($entity->metadata_type == 'expression') {
      $parents = $entity->metadata['xc:workExpressed'];
    }
    elseif ($entity->metadata_type == 'holdings') {
      $parents = $entity->metadata['xc:manifestationHeld'];
    }
    elseif ($entity->metadata_type == 'item') {
      $parents = $entity->metadata['xc:holdingsExemplified'];
    }
    if (!empty($parents)) {
      $identifier_prefix = _source_id_to_identifier_prefix($entity->source_id);
      foreach ($parents as $identifier) {
        // $metadata_id = _identifier_to_metadata_id($identifier['#value']);
        $metadata = xc_solr_get_metadata_by_identifier($identifier['#value'], $identifier_prefix);
        $t2 = microtime(TRUE);
        if (empty($metadata)) {
          xc_log_error('solr', 'No metadata for ' . $identifier['#value'] . ' (' . $entity->metadata_type . ')');
        }
        else {
          $entities[] = $metadata;
        }
      }
    }
  }
  $t_end = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/xc_solr_get_parent', ($t_end - $t_start));
  return $entities;
}

/**
 *
 * @param $type
 * @return unknown_type
 */
function xc_solr_metadata_to_solr_factory(&$object, $type, &$document = NULL) {
  $conditional_facets = array();
  return xc_solr_metadata_to_solr_iterate_indexes($object, $conditional_facets, $document);
}

/**
 * Converts a metadata object to Solr document
 *
 * @param $object (Object)
 *   A metadata object.
 *   an example:
 *   identifier => 'oai:URMST:Transformation Service/10002'
 *   (optional) 'node_id' => ...
 *   (optional) 'node_type' => ...
 *   metadata_id => ...,
 *   metadata_type => ...,
 *   source_id => ...
 *   metadata => array(
 *     "@type" => "expression",
 *     "@id"   => "oai:mst.rochester.edu:MST/MARCToXCTransformation/14999",
 *     "xc:titleOfExpression" = array(array("#value" => "Philosophy of art history.")),
 *     "dcterms:type" => array(array("@type" => "dcterms:DCMIType", "#value" => "Text")),
 *     ...
 *   );
 * @param $conditional_facets (array)
 *   Pairs of facet name and array of schema field names used in the record.
 * @param $document (Apache_Solr_DocumentXc)
 *   Reference to an Apache Solr document or NULL
 *
 * @return Apache_Solr_DocumentXc
 *   An Apache Solr document
 */
function xc_solr_metadata_to_solr_iterate_indexes(&$object, &$conditional_facets, &$document = NULL) {
  global $_oaiharvester_statistics;
  static $indexes, $cache;

  //xc_log_info('solr', var_export($object, TRUE));
  if (!isset($cache)) {
    $cache = array(
      'unique_fieldnames' => array('serialized_tsn'),
      'subject_topic_facet_field' => array(
        array(
          '#name' => 'subject_fc',
          '#type' => 'facet',
        ),
        array(
          '#name' => 'topic_fc',
          '#type' => 'facet',
        ),
        array(
          '#name' => 'subject_topic_fc',
          '#type' => 'facet',
        ),
      ),
      'exclude_from_highlight' => array(
        '@id' => 1,
        'xc:workExpressed' => 1,
        'xc:expressionManifested' => 1,
        'xc:manifestationHeld' => 1,
      ),
    );
  }

  $t_new = microtime(TRUE);
  if (is_null($document)) {
    $new_document = TRUE;
    $document = new Apache_Solr_DocumentXc();
    $document->setUniqueFieldnames($cache['unique_fieldnames']);
  }
  else {
    $new_document = FALSE;
  }

  $t_new2 = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/01 new', ($t_new2 - $t_new));

  $debug = FALSE;
  //$debug = ($object->identifier == 'oai:URMST:Transformation Service/10002') ? TRUE : FALSE;
  if ($new_document) { // $object->metadata_type == 'manifestation') {
    $document->addField('id',               $object->identifier, FALSE, TRUE);

    if (isset($object->node_id)) {
      $document->addField('node_id_i',      $object->node_id, FALSE, TRUE);
    }

    if (isset($object->node_type)) {
      $document->addField('node_type_s',    $object->node_type, FALSE, TRUE);
    }

    $document->addField('metadata_id_s',    $object->metadata_id, FALSE, TRUE);
    $document->addField('metadata_type_s', $object->metadata_type, FALSE, TRUE);
    $document->addField('source_id_s',      $object->source_id, FALSE, TRUE);
    // $document->addField('serialized_tsn',  serialize($object), FALSE, TRUE);
    $document->addField('type',             $object->metadata_type, FALSE, TRUE); // $object->metadata['@type']

    $document->addField('identifier_int_i', $object->identifier_int, FALSE, TRUE);

    if (isset($object->bibid)) {
      $document->addField('bibid_s', $object->bibid, FALSE, TRUE);
    }
  }
  $t_new3 = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/02 common fields', ($t_new3 - $t_new2));
  if (!isset($indexes)) {
    $indexes = xc_solr_fields2index_list();
  }

  if (is_array($indexes)) {
    foreach ($indexes as $schema_field_name => $field_properties) {
      if (!isset($object->metadata[$schema_field_name])) {
        continue;
      }
      /*
       if (isset($field_properties['value_as_key']) || isset($field_properties['value_as_value'])) {
       drupal_set_message($schema_field_name . ': ' . var_export($field_properties, TRUE));
       }
       */

      // main field value
      $tm = microtime(TRUE);
      if (isset($field_properties['general_fields'])) {
        $values = array();
        foreach ($object->metadata[$schema_field_name] as $instance) {
          if (isset($instance['#value'])) {
            $values[] = $instance['#value'];
          }
        }
        xc_solr_add_fields($document, $field_properties['general_fields'], $values);
      }

      // date fields
      if (isset($field_properties['date_fields'])) {
        $values = array();
        foreach ($object->metadata[$schema_field_name] as $instance) {
          $temp = xc_solr_to_date($instance['#value'], $schema_field_name, $object->bibid);
          if (strlen($temp) == 20) {
            $values[] = $temp;
          }
        }
        xc_solr_add_fields($document, $field_properties['date_fields'], $values);
      }
      $tm2 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/03 main value', ($tm2 - $tm));

      // use attribute value as field value
      if (isset($field_properties['value_as_value'])) {
        foreach ($field_properties['value_as_value'] as $attribute) {
          if (isset($object->metadata[$schema_field_name][$attribute])) {
            xc_solr_add_fields($document, $field_properties['value_as_value'][$attribute], $object->metadata[$schema_field_name][$attribute]);
            xc_solr_add_fields($document, $field_properties['general_fields'], $object->metadata[$schema_field_name][$attribute]);
          }
        }
      }
      // }
      $tm3 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/04 value_as_value', ($tm3 - $tm2));

      // use attribute value as part of field name
      if (isset($field_properties['value_as_key'])) {
        foreach ($field_properties['value_as_key'] as $attribute => $attribute_properties) {
          for ($len = count($object->metadata[$schema_field_name]), $i = 0; $i < $len; $i++) {
            if (isset($object->metadata[$schema_field_name][$i]['@' . $attribute])) {
              $key = $object->metadata[$schema_field_name][$i]['@' . $attribute];
              if (isset($attribute_properties['values'][$key])) {
                xc_solr_add_fields($document, $attribute_properties['values'][$key], $object->metadata[$schema_field_name][$i]['#value']);
              }
            }
          }
        }
      }
      $tm4 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/05 value_as_key', ($tm4 - $tm3));

      // use attribute value as part of field name and do not use main field
      // TODO: revision
      if (isset($field_properties['as_key_exclusively'])) {
        foreach ($field_properties['as_key_exclusively'] as $attribute) {
          $values = array();
          foreach ($object->metadata[$schema_field_name] as $instance) {
            if (isset($instance[$attribute])) {
              $values[] = $instance[$attribute];
            }
          }
          if (!empty($values)) {
            xc_solr_add_fields($document, $field_properties['as value'][$attribute], $values);
          }
        }
      }
      $tm5 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/06 as_key_exclusively', ($tm5 - $tm4));

      // date facet
      if (isset($field_properties['date_facets'])) {
        $values = array();
        foreach ($object->metadata[$schema_field_name] as $instance) {
          $temp = xc_solr_to_date($instance['#value'], $schema_field_name, $object->bibid);
          if (strlen($temp) == 20) {
            $values[] = $temp;
          }
        }
        xc_solr_add_fields($document, $field_properties['date_facets'], $values);
      }

      // facet
      $tm6 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/07 date_facet', ($tm6 - $tm5));
      if (isset($field_properties['facets'])) {
        $values = array();
        $tm61 = microtime(TRUE);
        foreach ($object->metadata[$schema_field_name] as $instance) {
          $values[] = _xc_solr_clean($instance['#value']);
        }
        $tm62 = microtime(TRUE);
        xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/08 facet/01 get values', ($tm62 - $tm61));
        xc_solr_add_fields($document, $field_properties['facets'], $values);
        $tm63 = microtime(TRUE);
        xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/08 facet/02 add fields', ($tm63 - $tm62));
      }
      $tm7 = microtime(TRUE);
      xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/08 facet', ($tm7 - $tm6));

      // collect conditional facets. We will run all this at the end of processing
      if (isset($field_properties['conditional_facets'])) {
        foreach ($field_properties['conditional_facets'] as $facet_name) {
          if (!isset($conditional_facets[$facet_name])) {
            $conditional_facets[$facet_name] = array();
          }
          if (!isset($conditional_facets[$facet_name][$schema_field_name])) {
            $conditional_facets[$facet_name][$schema_field_name] = 1;
          }
        }
      }
    } // foreach ($indexes as $schema_field_name => $field_properties)
  }

  // text field, which contains everything
  $tm1 = microtime(TRUE);
  $highlight = $all_fields = '';
  if (is_array($object->metadata)) {
    foreach ($object->metadata as $field => $values) {
      if (!isset($values) || empty($values) || !is_array($values)) {
        continue;
      }
      foreach ($values as $element_instance) {
        if (is_array($element_instance)) {
          foreach ($element_instance as $key => $value) {
            if ($key == '#value') {
              $all_fields .= $value . ' ';
              if (!isset($cache['exclude_from_highlight'][$field])) {
                $highlight .= $value . ' ';
              }
            } // foreach element_instances
          }
        } // foreach values
        else { // '@id', '@type'
          if (isset($value) && !empty($value)) {
            $all_fields .= $value . ' ';
            if (!isset($cache['exclude_from_highlight'][$field])) {
              $highlight .= $value . ' ';
            }
          }
        }
      } // processing arrays
    }
  } // processing metadata
  $document->addField('text', $all_fields, FALSE, TRUE);
  $document->addField('highlight_txt', $highlight, FALSE, TRUE);
  $t2 = microtime(TRUE);
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr/09 text', ($t2 - $tm1));
  xc_oaiharvester_statistics_set('03 step3/02 iterator/01 xc_solr_onestop_indexer/toSolr', ($t2 - $t_new));
  return $document;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_solr_index_general_value_as_key($metadata, $field_name, $field_properties, $document = NULL) {
  if (is_null($document)) {
    $values = array();
  }

  foreach ($field_properties['value_as_key'] as $attribute => $attribute_properties) {
    for ($len = count($metadata[$field_name]), $i = 0; $i < $len; $i++) {
      if (isset($metadata[$field_name][$i]['@' . $attribute])) {
        $key = $metadata[$field_name][$i]['@' . $attribute];
        if (isset($attribute_properties['values'][$key])) {
          if (!is_null($document)) {
            xc_solr_add_fields($document, $attribute_properties['values'][$key], $metadata[$field_name][$i]['#value']);
          }
          else {
            $values[] = $metadata[$field_name][$i]['#value'];
          }
        }
      }
    }
  }

  if (is_null($document)) {
    return $values;
  }
}

/**
 * Adds field to Solr Document
 *
 * @param $document (Object)
 *   Reference to Solr Document
 * @param $fields (array)
 *   The list of fields
 * @param $values (array)
 *   The list of values
 * @param $function (function)
 *   An optional function which will be applied to all values
 */
function xc_solr_add_fields(&$document, $fields, $values, $function = NULL) {

  // run function if there is
  if (!is_null($function)) {
    if (is_array($values)) {
      array_walk($values, $function);
    }
    else {
      $values = $function($values);
    }
  }

  if (is_array($fields)) {
    foreach ($fields as $field) {
      xc_solr_add_field($document, $field, $values);
    }
  }
  else {
    xc_solr_add_field($document, $fields, $values);
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_solr_add_field(&$document, &$field, $values) {
  // check whether the field values has to be fieltered
  if (xc_solr_is_field_with_excludable_values($field)) {
    if (!is_array($values)) {
      $values = (array) $values;
    }
    $values = xc_solr_exclude_values($field, $values);
  }

  $document->addField($field, $values, FALSE, TRUE);
}

/**
 * Prepares the Solr onestop indexer's run.
 *
 * It is called at the first iteration of the batch process
 *
 * @param $locations (array)
 *   List of location objects (see XC Metadata module's xc_location table,
 *   and Storage locations at admin/xc/metadata/location)
 * @param $saved_batch_id (int)
 *   The identifier of the saved batch object (see oaiharvester_batch table)
 * @param $context (array)
 *   Batch API's internal use container
 */
function xc_solr_onestop_indexer_batch_prepare($locations = array(), $saved_batch_id = -1, &$context) {
  global $_xc_solr_indexing_statistics, $_xc_solr_batch_remove;

  $t0 = microtime(TRUE);
  $context['sandbox']['start_time'] = microtime(TRUE);
  xc_log_info('onestop', 'first iteration');
  drupal_set_message('xc_solr_onestop_indexer_batch');
  variable_del('xc_solr_field_hooks');
  variable_del('xc_solr_version_numbers');

  // set up iteration strategies
  $iteration_strategies = variable_get('xc_solr_iteration_strategies', array());
  $allowed_strategies = array_flip(array(XC_SOLR_PROCESS_ALL, XC_SOLR_PROCESS_CHANGES_ONLY));
  if (isset($iteration_strategies[$saved_batch_id]) // $saved_batch_id
       && isset($allowed_strategies[$iteration_strategies[$saved_batch_id]])) {
    $iteration_strategy = $iteration_strategies[$saved_batch_id];
  }
  else {
    $iteration_strategy = XC_SOLR_PROCESS_ALL; // XC_SOLR_PROCESS_CHANGES_ONLY
  }
  $context['sandbox']['iteration_strategy'] = $iteration_strategy;

  if ($iteration_strategy == XC_SOLR_PROCESS_ALL) {
    // get implemented schemas
    $metadata_types = array();
    $schemas = module_invoke_all('xc_schema');
    foreach ($schemas as $key => $schema) {
      if (!isset($schema['group']) || empty($schema['group'])) {
        $metadata_types[] = $key;
      }
      // handling xc schemas
      elseif ($schema['group'] == 'xc_schema') {
        if ($key == 'manifestation') {
          $metadata_types[] = $key;
        }
        // else do nothing, process only this type
      }
      else {
        // there is a group, but at time of writing this code there is no example for that.
      }
    }
    // TODO: check the node types instead!!!
    $metadata_types_conditions = array();
    foreach ($metadata_types as $metadata_type) {
      $metadata_types_conditions[] = "node_type = 'xc_" . $metadata_type . "'";
    }
    $metadata_types_condition = '(' . join(' OR ', $metadata_types_conditions) . ')';
    $context['sandbox']['metadata_types_condition'] = $metadata_types_condition;
    $count_sql = 'SELECT count(*) FROM {xc_entity_properties} WHERE ' . $metadata_types_condition; //source_id = %d';
    xc_log_info('onestop', $count_sql);
    // TODO: remove this! 
    $context['sandbox']['max'] = db_query($count_sql)->fetchField(); //, $source_id

    $stored_offset_id = variable_get('xc_solr_reindex_offset_id', -1);
    if ($stored_offset_id == -1) {
      // TODO Please convert this statement to the D7 database API syntax. 
      $offset_id = db_query('SELECT min(metadata_id) FROM {xc_entity_properties} WHERE ' . $metadata_types_condition)->fetchField() - 1;
    }
    else {
      $offset_id = (int) $stored_offset_id;
    }
  } // XC_SOLR_PROCESS_ALL ends
  elseif ($iteration_strategy == XC_SOLR_PROCESS_CHANGES_ONLY) { 
    $context['sandbox']['max'] = db_query('SELECT count(*) FROM {xc_oaiharvester_bridge_changes} WHERE type != :type', array(':type' => XC_DELETED))->fetchField();
    $stored_offset_id = variable_get('xc_solr_reindex_offset_id', -1);
    if ($stored_offset_id == -1) {
      $offset_id = db_query('SELECT min(metadata_id) FROM {xc_oaiharvester_bridge_changes} WHERE type != :type', array(':type' => XC_DELETED))->fetchField() - 1;
    }
    else {
      $offset_id = (int) $stored_offset_id;
    }
    $context['sandbox']['metadata_types_condition'] = '';

    // get deleted
    $deleted_count = db_query('SELECT count(*) AS count FROM {xc_oaiharvester_bridge_changes} WHERE type = :type', array(':type' => XC_DELETED))->fetchField();
    if ($deleted_count) { 
      $result = db_query('SELECT metadata_id FROM {xc_oaiharvester_bridge_changes} WHERE type = :type', array(':type' => XC_DELETED));
      $deleted = 0;
      $object = array();
      $_xc_solr_batch_remove = array();
      while ($metadata_id = $result->fetchField()) {
        drupal_set_message('delete from Solr: ' . $metadata_id);
        $deleted++;
        $_xc_solr_batch_remove[] = $metadata_id;
        if ($deleted == XC_BATCH_SIZE) {
          xc_solr_xc_remove($object, $locations, array('solr servers' => $solrs));
          $deleted = 0;
          $_xc_solr_batch_remove = array();
        }
      }
      if ($deleted != 0) {
        xc_solr_xc_remove($object, $locations, array('do_check_size' => FALSE, 'solr servers' => $solrs));
      }
    }
  } // PROCESS_CHANGES_ONLY

  $context['sandbox']['offset_id'] = $offset_id;

  // with a variable (xc_solr_max_records_to_index) we can set the maximum number or records to index
  $max_records_to_index = variable_get('xc_solr_max_records_to_index', 0);
  if ($max_records_to_index > 0 && $context['sandbox']['max'] > $max_records_to_index) {
    $context['sandbox']['max'] = $max_records_to_index;
  }

  $context['results']['max'] = $context['sandbox']['max'];
  xc_log_info('reindex', 'count: ' . $context['sandbox']['max']);
  $_xc_solr_indexing_statistics = array(
    'empty' => 0,
    'metadata' => 0,
    'success' => 0,
    'entity' => 0,
  );

  $stored_offset = variable_get('xc_solr_reindex_offset', -1);
  if ($stored_offset == -1) {
    $offset = 0;
  }
  else {
    $offset = (int) $stored_offset;
  }
  $context['sandbox']['offset'] = $offset;
  xc_log_info('reindex', sprintf('Start indexing from offset: %s (offset_id: %s) of %s', $offset, $offset_id, $context['sandbox']['max']));

  // whether Solr is updated with JSON (TRUE) or XML (FALSE)
  $context['sandbox']['asJSON'] = variable_get('xc_solr_index_as_json', 1);

  // setup cache if needed. xc_solr_target variable's possible values are 'create_dump' and 'index'
  $context['sandbox']['solr_target'] = variable_get('xc_solr_target', 'index');
  if ($context['sandbox']['solr_target'] == 'create_dump') {
    $context['sandbox']['upload']['cache_dir'] = xc_solr_create_input_cache();
    xc_solr_purge_input_cache($context['sandbox']['upload']['cache_dir']);
  }
  $context['sandbox']['documents_added'] = 0;
  xc_oaiharvester_statistics_set('03 step3/00 preparation', (microtime(TRUE) - $t0));
  $context['sandbox']['do_next'] = TRUE;
  $context['finished'] = (1 / $context['sandbox']['max']);
} // xc_solr_onestop_indexer_batch_firstiteration

/**
 * Create a cache directory for the Solr input (XML or JSON) files
 *
 * @return (String)
 *   The cache directory's name
 */
function xc_solr_create_input_cache() {

  $cache_dir = file_directory_path() . '/xc_solr_input_cache/';
  if (!file_prepare_directory($cache_dir, FILE_CREATE_DIRECTORY)) {
    xc_log_error('harvester', t('The Drupal default "file" directory is not writable. Check out your Drupal settings, or turn off the caching of harvested responses. Your current file directory is %file.', array('%file' => $file_dir)));
    $cache_dir = '';
  }

  return $cache_dir;
}

/**
 * Remove all JSON files from the cache directory
 *
 * @param $dir (String)
 *   The name of directory
 */
function xc_solr_purge_input_cache($dir) {
  $mask = $dir . '/upload.*.json';
  array_map('unlink', glob($mask));
}

/**
 * Updates the context message in Batch API
 *
 * @param $context (array)
 *   Reference to the Batch API context array.
 * @param $preparation_time (int)
 *   The time the preparation process takes (in ms).
 */
function xc_solr_onestop_indexer_update_context_message(&$context, $preparation_time) {

  if ($context['sandbox']['max'] == 0) {
    $percent = 1.0;
  }
  else {
    $percent = ($context['sandbox']['offset'] / $context['sandbox']['max']);
  }
  $now = microtime(TRUE);
  $time_elapsed = ((int) $now - (int) $context['sandbox']['start_time']);
  $time_iteration = $time_elapsed - $preparation_time;
  $time_remained = ((int) ($time_iteration / $percent) - $time_iteration);
  $time_total = $time_elapsed + $time_remained;

  $context['message'] = t('%offset / %max. Elapsed time: %time_elapsed, remained appr.: %time_remained, total appr.: %time_total.',
    array(
      '%offset' => $context['sandbox']['offset'],
      '%max' => $context['sandbox']['max'],
      '%time_elapsed' => oaiharvester_sec_to_time($time_elapsed),
      '%time_remained' => oaiharvester_sec_to_time($time_remained),
      '%time_total' => oaiharvester_sec_to_time($time_total),
    )
  );
  $context['message'] .= '<br />' . t('Warning! Do not close this window untill reindex is complete!') . '<br />';
  $current_batch = batch_get();
  $current_batch_id = isset($current_batch['id']) ? $current_batch['id'] : 0;
  $link = l(t('Stop harvesting'), 'batch', array('query' => array('op' => 'finished', 'id' => $current_batch_id)));
  $context['message'] .= ' ' . $link . ' ' . t('(You will be able to restart or start over.)');
}

/**
 * Sets the Solr dump file name
 *
 * It manipulates the context array, and sets $context['sandbox']['upload']['file'].
 *
 * @param $context (Array)
 *   Reference to the Batch API's $context array.
 */
function xc_solr_create_input_cache_file_name(&$context) {
  $ext = $context['sandbox']['asJSON'] ? 'json' : 'xml';
  $context['sandbox']['upload']['file'] = sprintf('%s/upload.%06d.%s',
    $context['sandbox']['upload']['cache_dir'],
    $context['sandbox']['upload']['file_counter'],
    $ext
  );
  return $context['sandbox']['upload']['file'];
}

/**
 * Get the actual Solr strategy for indexing sortable fields.
 *
 * @return (int)
 *   The constans defined for the appropriate strategy. Possible values:
 *   - XC_SOLR_SORT_STRATEGY_MINMAX: for creating two fields for minimum and maximum values
 *   - XC_SOLR_SORT_STRATEGY_FIRST: for creating field from the first value
 */
function xc_solr_get_sortstrategy() {
  return XC_SOLR_SORT_STRATEGY_MINMAX;
}

/**
 * Returns the actual metadata process strategy, actually XC_SOLR_METADATA_PROCESS_STRATEGY_MERGE_FIRST
 *
 * @return int
 *   The actual strategy as constant
 */
function xc_solr_get_metadata_process_strategy() {
  return XC_SOLR_METADATA_PROCESS_STRATEGY_MERGE_FIRST; // the other option is XC_SOLR_METADATA_PROCESS_STRATEGY_INDEX_FIRST
}

/**
 * Get those fields, which are in multiple entities, but we need them separated
 *
 * <code>
 * array(
 'manifestation' => array('xc:recordID' => 'bibid_s'),
 'holdings' => array('xc:recordID' => 'holdingsid_i'),
 );
 </code>

 * @return (Array)
 *   schema field, Solr field pairs grouped by entities
 */
function xc_solr_get_conflicting_fields() {
  static $cache;

  if (!isset($cache)) {
    $cache = array();
    /*
     $cache = array(
     'manifestation' => array('xc:recordID' => 'bibid_s'),
     'holdings' => array('xc:recordID' => 'holdingsid_i'),
     );
     */
  }

  return $cache;
}

/**
 * Returns the list of library codes of bibIds. These library codes
 * can help us to extract the bib IDs from the metadata record.
 * See the following XML fragment: 
 * <xc:recordID type="[library code]">[bib id]</xc:recordID>.
 * The library codes exist as the value of type attribute of xc:recordID element.
 * The XC ILS module manages the registration and storage of usable library codes.
 */
function xc_solr_get_bibid() {
  static $cache;

  if (!isset($cache)) {
    $cache = array();
    $ils_mappings = xc_ils_get_all_mappings();
    if (isset($ils_mappings['xc:recordID'])) {
      foreach ($ils_mappings['xc:recordID'] as $mapping) {
        if (isset($mapping['filter']['@type'])) {
          $cache[] = $mapping['filter']['@type'];
        }
      }
    }
    // $cache = array('NRU');
  }

  return $cache;
}

/**
 * Get the list of excludable values.
 *
 * @return (Array)
 *   An associative array in which the keys are the Solr field names, and the values
 *   are the array of excludable values.
 */
function xc_solr_get_excludable_values() {
  $values = array(
    'subject_fc' => array('Text'),
  );
  return $values;
}

/**
 * Checks whether the field has excludable values
 *
 * @param $field (String)
 *   The name of the field to check
 *
 * @return (Boolean)
 *   TRUE if the field has excludable value, FALSE otherwise
 */
function xc_solr_is_field_with_excludable_values($field) {
  static $cache, $excludable_values;

  if (!isset($cache[$field])) {
    if (!isset($excludable_values)) {
      $excludable_values = xc_solr_get_excludable_values();
    }
    $cache[$field] = isset($excludable_values[$field]);
  }

  return $cache[$field];
}

/**
 * Filters out the excludable values among a field's values
 *
 * @param $field (String)
 *   The name of Solr field
 * @param $values (Array)
 *   The field's original values
 *
 * @return (Array)
 *   The fields filtered values
 */
function xc_solr_exclude_values($field, $values) {
  static $settings;

  if (!isset($settings)) {
    $settings = xc_solr_get_excludable_values();
  }

  return array_diff($values, $settings[$field]);
}
