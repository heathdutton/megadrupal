<?php
/**
 * @file
 * Code for the cm_agenda_live viewer functions.
 */

/**
 * Displays the live page for anonymous visitors.
 */
function cm_agenda_live($agenda = NULL) {
  $var = array('node' => $agenda);
  $state = cm_agenda_get_state($agenda);
  $admin_message = FALSE;
  if ($state == 'restrict') {
    if (!user_access('administer agendas')) {
      drupal_access_denied();
      return;
    }
    drupal_set_message(t('Agenda is not published. Anonymous users will get access denied'), 'warning');
  }
  if ($state == 'post_live' || $state == 'vod') {
    if (!user_access('administer agendas')) {
      drupal_goto('node/' . $agenda->nid);
      return;
    }
    drupal_set_message(t('Normal users will be redirected to') . ' ' . l(t('VOD'), 'node/' . $agenda->nid), 'warning');
    $admin_message = TRUE;
  }

  if (!empty($agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'])) {
    drupal_add_js(drupal_get_path('module', 'cm_agenda') . '/enscroll.js');
    $settings = array('cm_agenda_live' => array(
        'agenda_nid' => $agenda->nid,
        'agenda_title' => $agenda->title,
      ));
    if ($admin_message) {
      $settings['cm_agenda_live']['admin_message'] = TRUE;
    }
    drupal_add_js($settings, 'setting');
    drupal_add_js(drupal_get_path('module', 'cm_agenda_live') . '/cm_agenda_live.js');

    $fid = $agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'];

    $var['video'] = field_view_field('node', $agenda, 'field_cm_agenda_live', 'default');
    $var['description'] = field_view_field('node', $agenda, 'field_cm_agenda_description', 'default');
    $var['chapters'] = cm_agenda_media_chapter_events($fid);
    $speaker = new stdClass();
    $speaker->media_event_id = 0;
    $speaker->title = '';
    $var['speakers'] = array($speaker);
    $chapter_info = new stdClass();
    $chapter_info->media_event_id = 0;
    $chapter_info->title = '';
    $chapter_info->text = '';
    $var['chapter_info_list'] = array($chapter_info);
    $var['more_classes'][] = 'cm-agenda-live';
  }
  if (module_exists('context')) {
    if ($plugin = context_get_plugin('condition', 'node')) {
      $plugin->execute($agenda, 'view');
    }
  }
  $content = theme('cm_agenda', $var);
  return $content;
}
