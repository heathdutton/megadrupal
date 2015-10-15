<?php
/**
 * @file
 * Code for the cm_agenda_editor pages.
 */

/**
 * Internal function for converings seconds to H:i:s format.
 */
function _cm_agenda_format_seconds($seconds) {
  if ($seconds < 0) {
    $seconds = 0;
  }
  if ($seconds >= 60 * 60 * 24) {
    $seconds = 86399;
  }
  $hours = floor($seconds / 3600);
  $mins = floor(($seconds - ($hours * 3600)) / 60);
  $secs = floor($seconds % 60);
  return sprintf("%02d:%02d:%02d", $hours, $mins, $secs);
}

/**
 * Page for showung the ediro.
 */
function cm_agenda_editor_page($agenda) {
  if (empty($agenda->field_cm_agenda_video[LANGUAGE_NONE][0]['fid'])) {
    drupal_set_message(t('Media file could not be found'));
    return '';
  }
  $file = entity_load_single('file', $agenda->field_cm_agenda_video[LANGUAGE_NONE][0]['fid']);

  $events = cm_agenda_media_events($file->fid);
  $count = 0;
  $items = '';
  $jsvalues = array(
    'agendanid' => $agenda->nid,
    'mediaid' => $file->fid,
    'groups' => array(
      array(
        'id' => 'cm_agenda_speaker',
        'content' => t('Speaker'),
      ),
      array(
        'id' => 'cm_agenda_chapter',
        'content' => t('Chapter'),
      ),
    ),
  );
  $headers = array(
    t('Title'),
    t('Start'),
    t('End'),
    t('New start'),
    t('New end'),
    '',
  );
  $rows = array();
  foreach ($events as $event) {
    $count++;
    $start = '00:00:00';
    $startsecond = 0;
    if (!empty($event->field_media_event_start[LANGUAGE_NONE][0]['value'])) {
      $start = _cm_agenda_format_seconds($event->field_media_event_start[LANGUAGE_NONE][0]['value']);
      $startsecond = $event->field_media_event_start[LANGUAGE_NONE][0]['value'];
    };
    $end = '23:59:59';
    if (!empty($event->field_media_event_end[LANGUAGE_NONE][0]['value'])) {
      $end = _cm_agenda_format_seconds($event->field_media_event_end[LANGUAGE_NONE][0]['value']);
    };

    $type = $event->field_media_event_type[LANGUAGE_NONE][0]['value'];
    $group = 1;
    if ($type == 'cm_agenda_chapter') {
      $group = 2;
    }
    $jsvalues['items'][] = array(
      'type' => 'range',
      'group' => $type,
      'id' => $event->media_event_id,
      'content' => $event->field_media_event_title[LANGUAGE_NONE][0]['safe_value'],
      'start' => '1978-11-19 ' . $start,
      'end' => '1978-11-19 ' . $end,
      'className' => $type,
    );
    $rows[] = array(
      'data' => array(
        l($event->field_media_event_title[LANGUAGE_NONE][0]['safe_value'], '', array(
          'fragment' => '',
          'external' => TRUE,
          'attributes' => array(
            'data-popcorn-chapter-source' => $file->fid,
            'data-popcorn-start' => $startsecond,
            'class' => $type . '_name',
          ),
        )),
        $start,
        $end,
        array(
          'data' => '',
          'class' => 'newstart',
        ),
        array(
          'data' => '',
          'class' => 'newend',
        ),
        l(t("edit"), 'media-event/' . $event->media_event_id . '/edit', array('attributes' => array('target' => '_blank'))),
      ),
      'id' => 'event_' . $event->media_event_id,
      'class' => array($type),
    );
  }

  libraries_load('visjs');
  drupal_add_js(drupal_get_path('module', 'cm_agenda_editor') . '/cm_agenda_editor.js');
  drupal_add_css(drupal_get_path('module', 'cm_agenda_editor') . '/css/cm_agenda_editor.css');
  drupal_add_js(array('cm_agenda_editor' => $jsvalues), 'setting');
  $params = array();
  $params['node']['mediafiles']['media']['media'][0] = new MediaFile($file);
  $player = mediafront_get_player('popcorn', $params);

  $content = '<div class="cm-agenda-editor"><div class="cm-agenda-editor-player-events"><div class="cm-agenda-left"><div class="cm-agenda-video">' . $player . '</div><div id="cm-agenda-editor-save" class="cm-agenda-editor-save"><button type="button">' . t('Save') . '</button></div><div id="cm-agenda-editor-message"></div></div>';

  $table = theme('table', array(
    'header' => $headers,
    'rows' => $rows,
  ));
  $content .= '<div class="cm-agenda-right"><div class="cm-agenda-chapter-container">' . render($table) . '</div></div></div>';
  $content .= '<div id="timeline" class="cm-agenda-editor-timeline"></div>';
  $content .= '<div id="log"></div></div>';

  return $content;

}
