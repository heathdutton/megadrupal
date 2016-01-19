<?php
/**
 * @file
 * Code for the cm_agenda_live ajax functions.
 */

/**
 * Function for returning live updates to viewers.
 */
function cm_agenda_live_presenter_ajax($agenda = NULL) {
  $return = array(
    'html' => '',
    'speaker' => '',
    'chapter' => '',
    'delay' => 0,
    'background_text' => '',
    'background_title' => '',
  );
  if (empty($agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'])) {
    drupal_json_output($return);
    return;
  }
  $fid = $agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'];

  $table = _cm_agenda_live_generate_table($fid);
  $speaker_event = cm_agenda_live_get_active_event($agenda);
  $speaker_name = !empty($speaker_event) ? $speaker_event->field_media_event_title[LANGUAGE_NONE][0]['safe_value'] : '';
  $class = !empty($speaker_event) ? "active" : '';
  $chapters = cm_agenda_media_chapter_events($fid);

  $background = '';
  $background_title = '';
  $items = array();
  foreach ($chapters as $chapter) {
    $item = array(
      'data' => '',
      'class' => array(),
    );
    $classes = array();
    $item['class'][] = 'cm-agenda-chapter';
    if (!empty($chapter->field_media_event_start[LANGUAGE_NONE][0]['value'])) {
      if (!empty($chapter->field_media_event_end[LANGUAGE_NONE][0]['value'])) {
        $item['class'][] = 'finished';
      }
      else {
        $background_title = $chapter->field_media_event_title[LANGUAGE_NONE][0]['safe_value'];
        if (!empty($chapter->field_media_event_longtext[LANGUAGE_NONE][0]['safe_value'])) {
          $background = $chapter->field_media_event_longtext[LANGUAGE_NONE][0]['safe_value'];
        }
        $item['class'][] = 'started';
      }
    }
    else {
      $item['class'][] = 'not-started';
    }
    $item['data'] = '<span class="cm-agenda-chapter-spanlink">' . $chapter->field_media_event_title[LANGUAGE_NONE][0]['safe_value'] . '</span>';
    $items[] = $item;
  }
  $variables = array(
    'items' => $items,
    'attributes' => array('class' => array(
        'cm-agenda-chapter-list',
        'scrollbox',
      )),
    'type' => 'ul',
  );
  $chapters_table = theme('item_list', $variables);
  $state = cm_agenda_get_state($agenda);

  drupal_json_output(array(
    'html' => render($table),
    'speaker' => $speaker_name,
    'class' => $class_name,
    'chapter' => $chapters_table,
    'delay' => !empty($agenda->field_cm_agenda_delay_time[LANGUAGE_NONE][0]['value']) ? $agenda->field_cm_agenda_delay_time[LANGUAGE_NONE][0]['value'] : 0,
    'background_text' => $background,
    'background_title' => $background_title,
    'state' => $state,
    'admin' => user_access('administer agendas'),
  ));
}

/**
 * Count down data.
 */
function cm_agenda_live_countdown_ajax($agenda = NULL) {
  $start = new DateObject($agenda->field_cm_agenda_start_time[LANGUAGE_NONE][0]['value'], $agenda->field_cm_agenda_start_time[LANGUAGE_NONE][0]['timezone_db']);
  $start->setTimezone(new DateTimeZone('UTC'));
  drupal_json_output(array(
    'start' => $start->format('D M d Y H:i:s O'),
    'now' => date('D M d Y H:i:s O'),
    'viewer_offset' => !empty($agenda->field_cm_agenda_start_viewer[LANGUAGE_NONE][0]['value']) ? $agenda->field_cm_agenda_start_viewer[LANGUAGE_NONE][0]['value'] : 0,
  ));
}

/**
 * Builds the chapter table.
 */
function _cm_agenda_live_generate_table($nid) {
  $query = db_select('field_data_field_media_event_media_ref', 'f');
  $query->condition('f.field_media_event_media_ref_target_id', $nid);
  $query->fields('f', array('entity_id'));
  $result = $query->execute();
  $eids = $result->fetchCol();
  $events = entity_load('media_event', $eids);
  $rows = array();
  foreach ($events as $event) {
    $row = array(
      'start' => isset($event->field_media_event_start[LANGUAGE_NONE][0]['value']) ? $event->field_media_event_start[LANGUAGE_NONE][0]['value'] : '-',
      'stop' => isset($event->field_media_event_end[LANGUAGE_NONE][0]['value']) ? $event->field_media_event_end[LANGUAGE_NONE][0]['value'] : '-',
      'type' => $event->field_media_event_type[LANGUAGE_NONE][0]['value'],
      'text' => $event->field_media_event_title[LANGUAGE_NONE][0]['value'],
    );
    $rows[] = $row;

  }
  $variables = array(
    'header' => array(
      'Start',
      'Stop',
      'Type',
      'Text',
    ),
    'rows' => $rows,
    'attributes' => array('id' => 'cm-agenda-events-table'),
  );
  return theme('table', $variables);
}

/**
 * Page callback: Outputs JSON for taxonomy autocomplete suggestions and limit by agendas speeker node.
 *
 * @param int $agenda_nid
 *   The nid of the agenda.
 *
 * @param array $tags_typed
 *   (optional) A comma-separated list of term names entered in the
 *   autocomplete form element. Only the last term is used for autocompletion.
 *   Defaults to '' (an empty string).
 */
function cm_agenda_live_taxonomy_autocomplete($agenda_nid = '', $tags_typed = '') {
  $agenda = node_load($agenda_nid);
  if (empty($agenda)) {
    print t('Agenda @agenda_nid not found.', array('@agenda_nid' => $agenda_nid));
    return;
  }

  if (empty($agenda->field_cm_agenda_speaker_root[LANGUAGE_NONE][0]['tid'])) {
    module_load_include('inc', 'taxonomy', 'taxonomy.pages');
    return taxonomy_autocomplete('field_cm_agenda_speaker', $tags_typed);
  }
  $root_tid = $agenda->field_cm_agenda_speaker_root[LANGUAGE_NONE][0]['tid'];

  // If the request has a '/' in the search text, then the menu system will have
  // split it into multiple arguments, recover the intended $tags_typed.
  $args = func_get_args();
  // Shift off the $field_name argument.
  array_shift($args);
  $tags_typed = implode('/', $args);

  // The user enters a comma-separated list of tags. We only autocomplete the
  // last tag.
  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));

  $term_matches = array();
  if ($tag_last != '') {
    $voc = taxonomy_vocabulary_machine_name_load('cm_agenda_speaker');
    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_term_hierarchy', 'h', 'h.tid = t.tid');
    $query->fields('t', array(
      'tid',
      'name',
    ));
    $query->condition('h.parent', $root_tid);
    $query->condition('t.vid', $voc->vid);
    $query->addTag('term_access');
    $query->addTag('translatable');
    // Do not select already entered terms.
    if (!empty($tags_typed)) {
      $query->condition('t.name', $tags_typed, 'NOT IN');
    }
    $query->condition('t.name', '%' . db_like($tag_last) . '%', 'LIKE');
    $query->range(0, 10);
    $tags_return = $query->execute()->fetchAllKeyed();
    $prefix = count($tags_typed) ? drupal_implode_tags($tags_typed) . ', ' : '';
    foreach ($tags_return as $tid => $name) {
      $n = $name;
      // Term names containing commas or quotes must be wrapped in quotes.
      if (strpos($name, ',') !== FALSE || strpos($name, '"') !== FALSE) {
        $n = '"' . str_replace('"', '""', $name) . '"';
      }
      $term_matches[$prefix . $n] = check_plain($name);
    }
  }
  drupal_json_output($term_matches);
}
