<?php
/**
 * @file
 * XC Metadata statistics functions
 *
 * It provides information about the number of records in the Drupal Toolkit in three
 * different time: at the pre-harvest state, during harvesting, and in the current state.
 *
 * The statistics of current state is created on demand. The previous statistics
 * are saved in Drupal variables 'xc_metadata_stat_previous', and 'xc_metadata_stat_harvested'.
 * They are collected during harvest in xc_oaiharvester_bridge module's implementation
 * of different OAI Harvester hooks. The statistics of harvesting is collected into
 * the global variable $_xc_metadata_statistics.
 *
 * @see xc_oaiharvester_bridge_oaiharvester_harvest_starting
 * @see xc_oaiharvester_bridge_oaiharvester_request_started
 * @see xc_oaiharvester_bridge_oaiharvester_process_record
 * @see xc_oaiharvester_bridge_oaiharvester_request_processed
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * View the metadata statistics.
 *
 * It provides information about pre-harvest state, the harvested records, and the current state
 * of metadata in Drupal Toolkit
 */
function xc_metadata_statistics_view() {
  $states = array(
    'previous' => xc_metadata_statistics('previous'),
    'harvested' => xc_metadata_statistics('harvested'),
    'current' => xc_metadata_statistics('current'),
  );

  $properties = xc_metadata_statistics_get_properties();
  $metadata_types = array_flip(array('work', 'expression', 'manifestation', 'holdings', 'oai_dc'));
  $record_types = array('new', 'updated', 'deleted');
  $array_property_in_harvested = array_flip(array('metadata', 'nodes', 'relations', 'new', 'updated', 'deleted', 'types'));

  $rows = array();
  $line = 0;
  foreach ($properties as $property) {
    $line++;
    $label = xc_metadata_statistics_get_label($property);
    if (!$label) {
      $label = $property;
    }

    $row = array('<a id="line-' . $line . '" />' . $line, $label);
    $link = l('#' . $line, 'admin/xc/metadata/statistics', array('fragment' => 'line-' . $line));
    foreach ($states as $state_name => $state) {
      if ($state_name == 'harvested' && isset($array_property_in_harvested[$property])) {
        // TODO: check it on creation side!
        $number = isset($state->{$property}['total']) ? $state->{$property}['total'] : 0;
      }
      elseif ($state_name == 'harvested' && isset($metadata_types[$property])) {
        $number = ($state->new[$property] ? $state->new[$property] : 0) + ($state->updated[$property] ? $state->updated[$property] : 0);
      }
      else {
        // TODO: check it on creation side!
        $number = isset($state->$property) ? $state->$property : 0;
      }
      $cell = isset($state->$property) ? number_format($number, 0, ',', ' ') : t('n/a');

      // calculate style
      $style = FALSE;
      if ($property == 'entities' && isset($state->work)) {
        $style = 'color: green';
        if ($state_name == 'harvested') {
          $n = ($state->metadata['work'] + $state->metadata['expression'] + $state->metadata['manifestation'] + $state->metadata['holdings'] + $state->deleted['work'] + $state->deleted['expression'] + $state->deleted['manifestation'] + $state->deleted['holdings']);
          if ($state->entities != $n) {
            $style = 'color: red';
            drupal_set_message(sprintf('%s - %s: entities (%d) != work + expression + manifestation + holdings (%d)',
              $link, $state_name, $state->entities, $n),
              'error');
          }
        }
        else {
          $n = ($state->work + $state->expression + $state->manifestation + $state->holdings);
          if ($state->entities != $n) {
            $style = 'color: red';
            drupal_set_message(sprintf('%s - %s: entities (%d) != work + expression + manifestation + holdings (%d)',
              $link, $state_name, $state->entities, $n),
              'error');
          }
        }
      }
      elseif ($property == 'metadata' && $state_name != 'harvested' && isset($state->metadata)) {
        $style = 'color: green';
        if ($state->metadata != $state->entities) {
          drupal_set_message(sprintf('%s - %s: %s metadata (%d) != entities (%d)', $link, $property, $state_name, $state->metadata, $state->entities), 'error');
          $style = 'color: red';
        }
      }
      elseif ($property == 'nodes' && $state_name != 'harvested' && isset($state->manifestation)) {
        $style = 'color: green';
        if ($state->nodes != $state->manifestation) {
          $style = 'color: red';
          drupal_set_message(sprintf('%s - %s: %s nodes (%d) != manifestation (%d)', $link, $property, $state_name, $state->nodes, $state->manifestation), 'error');
        }
      }
      elseif ($property == 'relations' && $state_name != 'harvested') {
        $wem = ($state->work + $state->expression + $state->manifestation);
        $emh = ($state->expression + $state->manifestation + $state->holdings);
        $style = 'color: green';
        if ($state->relations < $wem || $state->relations < $emh) {
          $style = 'color: red';
          drupal_set_message(sprintf('%s - %s: %s relations (%d) < w+e+m (%d) || relations (%d) < e+m+h (%d)', $link, $property, $state_name, $state->relations, $wem, $state->relations, $emh), 'warning');
        }
      }
      elseif (isset($metadata_types[$property])) {
        if ($state_name == 'harvested') {
          $sum = 0;
          foreach ($record_types as $record_type) {
            if ($record_type == 'deleted') {
              continue;
            }
            if (isset($state->{$record_type}[$property])) {
              $sum += $state->{$record_type}[$property];
            }
          }
          $style = 'color: green';
          if ($sum != $state->metadata[$property]) {
            $style = 'color: red';
            drupal_set_message(sprintf('%s - %s sum (%d) != %s (%d)', $link, $state_name, $sum, $property, $state->metadata[$property]), 'error');
          }
        }
        elseif ($state_name == 'current') {
          $sum = 0;
          if (isset($states['harvested']->{'new'}[$property])) {
            $sum += $states['harvested']->{'new'}[$property];
          }
          if (isset($states['harvested']->{'deleted'}[$property])) {
            $sum -= $states['harvested']->{'deleted'}[$property];
          }

          if (($states['previous']->{$property} + $sum) == $state->{$property}) {
            $style = 'color: green';
          }
          else {
            $style = 'color: red';
            drupal_set_message(sprintf('%s - %d + %d != %d', $link, $states['previous']->{$property}, $sum, $state->{$property}), 'error');
          }
        }
      }

      if ($style) {
        $row[] = array(
          'data' => $cell,
          'style' => $style
        );
      }
      else {
        $row[] = $cell;
      }
    }

    $rows[] = $row;
    if (isset($metadata_types[$property])) {
      foreach ($record_types as $record_type) {
        $line++;
        $number = 0;
        if (isset($states['harvested']->{$record_type}[$property])) {
          $number = $states['harvested']->{$record_type}[$property];
        }

        $row = array($line);
        $row[] = ' &nbsp; &nbsp; ' . xc_metadata_statistics_get_label($record_type);
        $row[] = t('n/a');
        $row[] = number_format($number, 0, ',', ' ');
        $row[] = t('n/a');
        $rows[] = array(
          'data' => $row,
          'style' => 'font-size: 80%;',
        );
      }
    }
  }

  $headers = array('#', t('Properties'), t('Before harvest'), t('Harvested'), t('Current'));
  $output = theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('class' => array('xc-statistics'))));

  $output .= xc_metadata_statistics_harvest_list();

  $output .= '<p>' . l('You might want to see statistics about Apache Solr index', 'admin/xc/solr/info') . '</p>';

  return $output;
}

/**
 * Collects and returns statistical information about metadata
 *
 * @param string $type
 *   The type of statistical information. Possible values are
 *   - previous: the pre-harvest state's statistics
 *   - harvested: the statistics about harvested records
 *   - current: the current state's statistics
 *
 * @return Object
 *   A stdClass object contains pairs of properties and counts belonging to properties.
 *   The keys are:
 *   - entities: the number of entities
 *   - oai_dc: the number of Dublin Core records
 *   - work: the number of FRBR work records
 *   - expression: the number of FRBR expression records
 *   - manifestation: the number of FRBR manifestation records
 *   - holdings: the number of FRBR holdings records
 *   - metadata: the number of actual metadata part of any type of entities
 *   - nodes: the number of nodes created
 *   - relations: the number of relations between entities
 *   - new: the number of new records
 *   - updated: the number of updated records
 *   - deleted: the number of deleted records
 *
 * @see xc_metadata_statistics_empty
 */
function xc_metadata_statistics($type = 'current') {
  switch ($type) {
    case 'previous':
      $statistics = variable_get('xc_metadata_stat_previous', xc_metadata_statistics_empty());
      break;

    case 'harvested':
      $statistics = variable_get('xc_metadata_stat_harvested', xc_metadata_statistics_empty());
      $entity_types = array('work', 'expression', 'manifestation', 'holdings', 'oai_dc');
      $data_types = array('metadata', 'new', 'deleted', 'updated', 'types');
      foreach ($data_types as $data_type) {
        foreach ($entity_types as $entity_type) {
          if (!isset($statistics->{$data_type}[$entity_type])) {
            $statistics->{$data_type}[$entity_type] = 0;
          }
        }
      }
      break;

    case 'current':
      $sql = 'SELECT
            (SELECT count(*) FROM {xc_entity_properties}) AS entities,
            (SELECT count(*) FROM {xc_sql_metadata}) AS metadata,
            (SELECT count(*) FROM {node}) AS nodes,
            (SELECT count(*) FROM {xc_entity_relationships}) AS relations';
      $statistics = db_query($sql)->fetchObject();
      $result = db_query('SELECT metadata_type, COUNT(metadata_type) AS count
                          FROM {xc_entity_properties} GROUP BY metadata_type');
      $statistics->oai_dc = 0;
      $statistics->work = 0;
      $statistics->expression = 0;
      $statistics->manifestation = 0;
      $statistics->holdings = 0;
      foreach ($result as $data) {
        $statistics->{$data->metadata_type} = $data->count;
      }

      $statistics->solr = xc_metadata_statistics_get_solr_document_number();
      break;

    default:
      $statistics = variable_get('xc_metadata_stat_previous', xc_metadata_statistics_empty());
      break;
  }
  return $statistics;
}

/**
 * Wrapper function for xc_solr_info_get_solr_document_number() to get the number of document
 * inside Solr index.
 *
 * @return int
 *   The number of Solr document
 */
function xc_metadata_statistics_get_solr_document_number() {
  include_once drupal_get_path('module', 'xc_solr') . '/xc_solr.info.inc';
  return xc_solr_info_get_solr_document_number();
}

/**
 * Create an empty statistics object
 *
 * @return Object
 *   A stdClass object contains pairs of properties and counts belonging to properties.
 *   Each count is 0 in this initial state of the object
 *   The keys are:
 *   - entities: the number of entities
 *   - oai_dc: the number of Dublin Core records
 *   - work: the number of FRBR work records
 *   - expression: the number of FRBR expression records
 *   - manifestation: the number of FRBR manifestation records
 *   - holdings: the number of FRBR holdings records
 *   - metadata: the number of actual metadata part of any type of entities
 *   - nodes: the number of nodes created
 *   - relations: the number of relations between entities
 *   - new: the number of new records
 *   - updated: the number of updated records
 *   - deleted: the number of deleted records
 */
function xc_metadata_statistics_empty() {
  $statistics = (Object) array(
    'entities' => 0,
    'oai_dc' => 0,
    'work' => 0,
    'expression' => 0,
    'manifestation' => 0,
    'holdings' => 0,
    'metadata' => array(),
    'nodes' => array(),
    'relations' => array(),
    'new' => array(),
    'updated' => array(),
    'deleted' => array(),
    'types' => array(),
    'solr' => 0,
  );
  return $statistics;
}

/**
 * Returns the properties used by the statistics table
 */
function xc_metadata_statistics_get_properties() {
  return array(
    'entities', 'work', 'expression', 'manifestation', 'holdings', 'oai_dc',
    'metadata', 'nodes', 'relations',
    'new', 'updated', 'deleted', 'types',
    'solr',
  );
}

/**
 * Gets the label belonging to a key in the metadata statistics.
 *
 * @param $key (string)
 *   A key in the metadata statistics like 'work' or 'nodes'.
 *
 * @return (string)
 *   If the key is registed, it returns a human readable label, otherwise FALSE value.
 */
function xc_metadata_statistics_get_label($property) {
  switch ($property) {
    case 'entities':      return t('All entities'); break;
    case 'work':          return t('Work'); break;
    case 'expression':    return t('Expression'); break;
    case 'manifestation': return t('Manifestation'); break;
    case 'holdings':      return t('Holdings'); break;
    case 'oai_dc':        return t('Dublin Core'); break;
    case 'metadata':      return t('Metadata'); break;
    case 'nodes':         return t('Nodes'); break;
    case 'relations':     return t('Uplinks from'); break;
    case 'new':           return t('New'); break;
    case 'updated':       return t('Updated'); break;
    case 'deleted':       return t('Deleted'); break;
    case 'unknown':       return t('Unknown'); break;
    case 'solr':          return t('Solr documents'); break;
    case 'types':         return t('Types'); break;
    default:              return FALSE; break;
  }
}

/**
 * Get abbreviations of properties
 */
function xc_metadata_statistics_get_abbreviation($property) {
  switch ($property) {
    case 'entities':      return t('All'); break;
    case 'work':          return t('W'); break;
    case 'expression':    return t('E'); break;
    case 'manifestation': return t('M'); break;
    case 'holdings':      return t('H'); break;
    case 'oai_dc':        return t('DC'); break;
    case 'metadata':      return t('Meta'); break;
    case 'nodes':         return t('Nodes'); break;
    case 'relations':     return t('Rel'); break;
    case 'new':           return t('New'); break;
    case 'updated':       return t('Upd'); break;
    case 'deleted':       return t('Del'); break;
    default:              return FALSE; break;
  }
}

/**
 * Get the list of harvest statistics
 */
function xc_metadata_statistics_harvest_list() {
  $harvests = oaiharvester_saved_batch_get_all();
  $properties = xc_metadata_statistics_get_properties();
  $metadata_types = array_flip(array('work', 'expression', 'manifestation', 'holdings', 'oai_dc'));
  $record_types = array('new', 'updated', 'deleted');
  $array_property_in_harvested = array_flip(array('metadata', 'nodes', 'relations', 'new', 'updated', 'deleted', 'types'));

  $total = array(
    'TOTAL',
    ''
  );
  $rows = array();
  foreach ($harvests as $harvest) {
    $reports = unserialize($harvest->reports);
    if (!isset($reports['xc_oaiharvester_bridge']['metadata_statistics'])) {
      continue;
    }
    $state = $reports['xc_oaiharvester_bridge']['metadata_statistics'];
    $schedule = oaiharvester_schedule_load($harvest->schedule_id, FALSE);

    $cells = array($schedule->schedule_name);
    $cells[] = l(
      format_date($harvest->timestamp, 'custom', 'Y-m-d H:i'),
      'admin/xc/harvester/schedule/' . $harvest->schedule_id . '/batch/' . $harvest->id
    );
    $tt = 2;
    foreach ($properties as $property) {
      if ($property == 'types') {
        continue;
      }

      $number = 0;
      if (isset($array_property_in_harvested[$property])) {
        $number = isset($state->{$property}['total']) ? $state->{$property}['total'] : 0;
      }
      elseif (isset($metadata_types[$property])) {
        $number = (isset($state->new[$property]) ? $state->new[$property] : 0)
                + (isset($state->updated[$property]) ? $state->updated[$property] : 0);
      }
      else {
        $number = isset($state->$property) ? $state->$property : 0;
      }
      $tt++;
      if (!isset($total[$tt])) {
        $total[$tt] = 0;
      }
      $total[$tt] += $number;
      $cells[] = str_replace(' ', '&nbsp;', number_format($number, 0, ',', ' '));
      // $cells[] = $reports['xc_oaiharvester_bridge']['metadata_statistics']->$property;
    }
    $number = isset($state->deleted['unknown']) ? $state->deleted['unknown'] : 0;
    $cells[] = str_replace(' ', '&nbsp;', number_format($number, 0, ',', ' '));
    $total[$tt] += $number;
    $rows[] = $cells;
  }
  foreach ($total as $tt => $val) {
    if (is_numeric($val)) {
      $total[$tt] = str_replace(' ', '&nbsp;', number_format($val, 0, ',', ' '));
    }
  }
  $rows[] = $total;

  if (empty($rows)) {
    return FALSE;
  }

  $headers = array(t('Schedule'), t('Date'));
  foreach ($properties as $property) {
    if ($property == 'types') {
      continue;
    }
    $headers[] = xc_metadata_statistics_get_label($property);
    // $headers[] = sprintf('<span title="%s">%s</span>', xc_metadata_statistics_get_label($property), xc_metadata_statistics_get_abbreviation($property));
  }
  $headers[] = t('Unknown');
  $output = '<h3>' . t('Result of individual harvests') . '</h3>';
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  return $output;
}
