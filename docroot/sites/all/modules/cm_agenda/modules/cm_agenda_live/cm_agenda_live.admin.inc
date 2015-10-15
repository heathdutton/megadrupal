<?php
/**
 * @file
 * Code for the cm_agenda_live administrator functions.
 */

/**
 * Page for presenting the administation page.
 */
function cm_agenda_live_admin_form($form, &$form_state, $agenda) {
  // When rebuilding the form we need to reload the node. In normal case this
  // is cached.
  $agenda = node_load($agenda->nid);
  if (module_exists('context')) {
    if ($plugin = context_get_plugin('condition', 'node')) {
      $plugin->execute($agenda, 'form');
    }
  }
  if (empty($agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'])) {
    return $form;
  }

  drupal_add_js(drupal_get_path('module', 'cm_agenda_live') . '/cm_agenda_live.admin.js');
  $fid = $agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'];
  $form['video'] = field_view_field('node', $agenda, 'field_cm_agenda_live', 'default');
  $form['agenda'] = array(
    '#type' => 'value',
    '#value' => $agenda->nid,
  );
  $form['agenda_node'] = array('#node' => $agenda);

  /*
   * Speaker fieldset
   */
  $speaker_event = cm_agenda_live_get_active_event($agenda);
  $active_class = !empty($speaker_event) ? ' class="active"' : '';

  $form['speaker'] = array(
    '#type' => 'fieldset',
    '#title' => t('Speaker'),
    '#attributes' => array('class' => array(
        'cm-agenda-speaker',
        'cm-agenda-live-admin-speaker',
      )),
  );
  $form['speaker']['speakerbox'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="speakerbox"' . $active_class . '>',
    '#suffix' => '</div>',
    '#markup' => !empty($speaker_event) ? $speaker_event->field_media_event_title[LANGUAGE_NONE][0]['safe_value'] : '',
  );

  $form['speaker']['clear'] = array(
    '#type' => 'submit',
    '#ajax' => array(
      'callback' => 'cm_agenda_live_admin_speaker_callback',
      'wrapper' => 'speakerbox',
    ),
    '#value' => t('Clear'),
    '#submit' => array('cm_agenda_live_admin_speaker_clear_submit'),
  );

  $form['speaker']['speaker'] = array(
    '#type' => 'textfield',
    '#title' => t('Speaker'),
    '#autocomplete_path' => 'ajax/agenda_speaker_autocomplete/' . $agenda->nid,
    '#attributes' => array('accesskey' => 's'),
    '#maxlength' => 1024,
  );

  $form['speaker']['submit'] = array(
    '#type' => 'submit',
    '#ajax' => array(
      'callback' => 'cm_agenda_live_admin_speaker_callback',
      'wrapper' => 'speakerbox',
    ),
    '#attributes' => array('accesskey' => 'a'),
    '#name' => 'submit-speaker',
    '#value' => t('Publish speaker'),
    '#submit' => array('cm_agenda_live_admin_speaker_submit'),
  );

  /*
   * Metadata fieldset
   */
  $form['metadata'] = array(
    '#type' => 'fieldset',
    '#title' => t('Metadata'),
    '#attributes' => array('class' => array(
        'cm-agenda-metadata',
        'cm-agenda-live-admin-metadata',
      )),
  );

  $form['metadata']['delay'] = array(
    '#title' => t('Delay (in seconds)'),
    '#type' => 'textfield',
    '#default_value' => !empty($agenda->field_cm_agenda_delay_time[LANGUAGE_NONE][0]['value']) ? $agenda->field_cm_agenda_delay_time[LANGUAGE_NONE][0]['value'] : 0,
    '#prefix' => '<div id="delaybox">',
    '#suffix' => '</div>',
  );
  $form['metadata']['delaysubmit'] = array(
    '#type' => 'submit',
    '#ajax' => array(
      'callback' => 'cm_agenda_live_admin_delay_callback',
      'wrapper' => 'delaybox',
    ),
    '#value' => t('Update delay'),
    '#submit' => array('cm_agenda_live_admin_delay_submit'),
  );

  $form['workflow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Workflow'),
    '#attributes' => array(
      'id' => 'cm-agenda-workflow',
      'class' => array(
        'cm-agenda-workflow',
        'cm-agenda-live-admin-workflow',
      ),
    ),
  );

  $display = array(
    'label' => 'hidden',
    'settings' => array('format_type' => 'medium'),
  );

  $state = cm_agenda_get_state($agenda);
  $info = field_info_field('field_cm_agenda_workflow_state');
  $form['workflow']['state'] = array(
    '#markup' => '<div class="cm-agenda-workflow-state-container"><div class="cm-agenda-workflow-state-label">' . t('Current state') . '</div><div class="cm-agenda-workflow-state cm-agenda-state-' . $state . '"> ' . $info['settings']['allowed_values'][$state] . '</div></div>',
    '#weight' => 10,
  );

  if ($state == 'restrict' || $state == 'live') {
    $form['workflow']['start'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array(
          'cm-agenda-time',
          'cm-agenda-starttime',
          'clearfix',
        )),
      '#weight' => 20,
    );
  }

  if ($state == 'restrict') {
    $form['workflow']['golivesubmit'] = array(
      '#type' => 'submit',
      '#ajax' => array(
        'callback' => 'cm_agenda_live_admin_workflow_callback',
        'wrapper' => 'cm-agenda-workflow',
      ),
      '#value' => t('Publish agenda with countdown'),
      '#submit' => array('cm_agenda_live_admin_golive_submit'),
      '#weight' => 50,
    );
  }
  if ($state == 'restrict' || $state == 'live') {
    $node_wrapper = entity_metadata_wrapper('node', $agenda);
    $field = format_date($node_wrapper->field_cm_agenda_start_time->value(), 'medium');

    $startoutput = t('Start time') . ': ' . render($field);
    $form['workflow']['start']['starttimebox'] = array(
      '#type' => 'markup',
      '#markup' => !empty($startoutput) ? $startoutput : ' - ',
      '#prefix' => '<div id="starttimebox" class="cm-agenda-timebox">',
      '#suffix' => '</div>',
      '#weight' => 20,
    );
  }
  if ($state == 'live') {
    $form['workflow']['start']['startsubmit'] = array(
      '#type' => 'submit',
      '#ajax' => array(
        'callback' => 'cm_agenda_live_admin_workflow_callback',
        'wrapper' => 'cm-agenda-workflow',
      ),
      '#value' => t('Set start time to now'),
      '#submit' => array('cm_agenda_live_admin_starttime_submit'),
      '#weight' => 21,
    );
    $form['workflow']['gopostlivesubmit'] = array(
      '#type' => 'submit',
      '#ajax' => array(
        'callback' => 'cm_agenda_live_admin_workflow_callback',
        'wrapper' => 'cm-agenda-workflow',
      ),
      '#value' => t('End live stream and go to postproduction'),
      '#submit' => array('cm_agenda_live_admin_gopostlive_submit'),
      '#weight' => 50,
    );
  }
  if ($state == 'post_live') {
    if (empty($agenda->field_cm_agenda_video[LANGUAGE_NONE][0]['fid'])) {
      $form['workflow']['novod'] = array(
        '#markup' => '<div>No VOD file for agenda</div>',
        '#weight' => 30,
      );
    }
    else {
      $events = cm_agenda_media_events($agenda->field_cm_agenda_video[LANGUAGE_NONE][0]['fid']);
      if (empty($events)) {
        $form['workflow']['copyevent'] = array(
          '#markup' => '<div>No events for VOD</div>',
          '#weight' => 30,
        );
        $form['workflow']['copyeventssubmit'] = array(
          '#type' => 'submit',
          '#ajax' => array(
            'callback' => 'cm_agenda_live_admin_workflow_callback',
            'wrapper' => 'cm-agenda-workflow',
          ),
          '#value' => t('Copy events from Live to VOD'),
          '#submit' => array('cm_agenda_live_admin_copyevents_submit'),
          '#weight' => 40,
        );
      }
      $form['workflow']['govodsubmit'] = array(
        '#type' => 'submit',
        '#ajax' => array(
          'callback' => 'cm_agenda_live_admin_workflow_callback',
          'wrapper' => 'cm-agenda-workflow',
        ),
        '#value' => t('Publish VOD'),
        '#submit' => array('cm_agenda_live_admin_govod_submit'),
        '#weight' => 50,
      );
    }
  }
  /*
   * Chapters
   */
  $form['chapter-list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Chapters'),
    '#attributes' => array('class' => array('cm-agenda-live-admin-chapter')),
  );

  $chapter_options = array(0 => '<span>' . t('Not started') . '</span>');
  $chapters = array();
  if (!empty($agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'])) {
    $chapters = cm_agenda_media_chapter_events($agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid']);
  }
  $default = 0;

  $form['chapter-list']['header'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('cm-agenda-admin-chapter-header')),
  );
  $form['chapter-list']['header']['submit-top'] = array(
    '#type' => 'submit',
    '#ajax' => array(
      'callback' => 'cm_agenda_live_admin_chapter_callback',
      'wrapper' => 'chapterbox',
    ),
    '#attributes' => array('accesskey' => 'u'),
    '#value' => t('Publish chapter'),
    '#name' => 'submit-chapter',
    '#submit' => array('cm_agenda_live_admin_chapter_form_submit'),
  );
  $form['chapter-list']['header']['add-top'] = array('#markup' => l(t('Add chapter'), 'media-event/add', array(
      'query' => array('field_media_event_media_ref' => $fid),
      'attributes' => array(
        'target' => '_blank',
        'class' => 'cm-add-chapter',
      ),
    )));

  $form['chapter-list']['container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'chapterbox',
      'class' => array(
        'cm-agenda-live-admin-chapter-list',
        'cm-agenda-chapter-container',
      ),
    ),
  );

  $form['chapter-list']['container']['chapters'] = array(
    '#type' => 'radios',
    '#prefix' => '<div class="cm-agenda-chapter-list scrollbox">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('cm-agenda-live-admin-chapter-list')),
  );

  foreach ($chapters as $chapter) {
    $label_classes = array();
    $started = FALSE;
    if (!empty($chapter->field_media_event_start[LANGUAGE_NONE][0]['value'])) {
      $form['chapter-list']['container']['chapters'][0]['#disabled'] = TRUE;
      $started = TRUE;
      if ($default == 0) {
        $default = -1;
      }
    }
    if (!empty($chapter->field_media_event_end[LANGUAGE_NONE][0]['value'])) {
      $label_classes[] = 'finished';
    }
    else {
      if ($started) {
        $label_classes[] = 'started';
        $default = $chapter->media_event_id;
      }
      else {
        $label_classes[] = 'not-started';
      }
    }
    $chapter_options[$chapter->media_event_id] = '<span class="' . implode(' ', $label_classes) . '">' . $chapter->field_media_event_title[LANGUAGE_NONE][0]['safe_value'] . '</span> ' . l(t('Edit'), 'media-event/' . $chapter->media_event_id . '/edit', array('attributes' => array('target' => '_blank')));
  }
  $chapter_options[-2] = '<span>' . t('Break') . '</span>';
  $chapter_options[-1] = '<span>' . t('Finished') . '</span>';
  $form['chapter-list']['container']['chapters']['#default_value'] = $default;
  $form['chapter-list']['container']['chapters']['#options'] = $chapter_options;
  return $form;
}

/**
 * Ajax callback for clearing speaker.
 */
function cm_agenda_live_admin_speaker_clear_submit($form, &$form_state) {
  $agenda = node_load($form_state['values']['agenda']);
  $speaker_event = cm_agenda_live_get_active_event($agenda);
  if (!empty($speaker_event)) {
    $speaker_event->field_media_event_end[LANGUAGE_NONE][0]['value'] = cm_agenda_get_timeoffset($agenda);
    $speaker_event->save();
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for saving speaker data.
 */
function cm_agenda_live_admin_speaker_submit($form, &$form_state) {
  $agenda = node_load($form_state['values']['agenda']);
  $speaker_event = cm_agenda_live_get_active_event($agenda);
  if (!empty($speaker_event)) {
    $speaker_event->field_media_event_end[LANGUAGE_NONE][0]['value'] = cm_agenda_get_timeoffset($agenda);
    $speaker_event->save();
  }
  $terms = taxonomy_get_term_by_name($form_state['values']['speaker'], 'cm_agenda_speaker');
  $term = NULL;
  if (count($terms) >= 1) {
    $term = array_shift(array_values($terms));
  }
  else {
    $voc = taxonomy_vocabulary_machine_name_load('cm_agenda_speaker');
    $term = new stdClass();
    $term->name = check_plain($form_state['values']['speaker']);
    $term->vid = $voc->vid;
    if (!empty($agenda->field_cm_agenda_speaker_root[LANGUAGE_NONE][0]['tid'])) {
      $term->parent = $agenda->field_cm_agenda_speaker_root[LANGUAGE_NONE][0]['tid'];
    }
    taxonomy_term_save($term);
  }
  $speaker = $term->name;
  $entity = entity_create('media_event', array());

  $entity->field_media_event_type[LANGUAGE_NONE][0]['value'] = 'cm_agenda_speaker';
  $entity->field_cm_agenda_speaker[LANGUAGE_NONE][0] = (array)$term;
  $entity->field_media_event_title[LANGUAGE_NONE][0]['value'] = $speaker;
  $entity->field_media_event_media_ref[LANGUAGE_NONE][0]['target_id'] = $agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'];
  $entity->field_media_event_start[LANGUAGE_NONE][0]['value'] = cm_agenda_get_timeoffset($agenda);
  $entity->field_media_event_end[LANGUAGE_NONE] = array();
  $entity->save();
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for updating state to live.
 */
function cm_agenda_live_admin_golive_submit($form, &$form_state) {
  $agenda = node_load($form_state['values']['agenda']);
  $agenda->field_cm_agenda_workflow_state[LANGUAGE_NONE][0]['value'] = 'live';
  $agenda->path['pathauto'] = 0;
  node_save($agenda);
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for updating starttime data.
 */
function cm_agenda_live_admin_starttime_submit($form, &$form_state) {
  $agenda = node_load($form_state['values']['agenda']);
  $agenda->field_cm_agenda_start_time[LANGUAGE_NONE][0]['value'] = format_date(strtotime('now'), 'custom', 'Y-m-d H:i:s', 'UTC');
  $agenda->path['pathauto'] = 0;
  node_save($agenda);
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for updating state to post_live.
 */
function cm_agenda_live_admin_gopostlive_submit($form, &$form_state) {
  $agenda = node_load($form_state['values']['agenda']);
  $agenda->field_cm_agenda_workflow_state[LANGUAGE_NONE][0]['value'] = 'post_live';
  $agenda->path['pathauto'] = 0;
  node_save($agenda);
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for updating state to vod.
 */
function cm_agenda_live_admin_govod_submit($form, &$form_state) {
  $agenda = node_load($form_state['values']['agenda']);
  $agenda->field_cm_agenda_workflow_state[LANGUAGE_NONE][0]['value'] = 'vod';
  $agenda->path['pathauto'] = 0;
  node_save($agenda);
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for copy events from live to vod.
 */
function cm_agenda_live_admin_copyevents_submit($form, &$form_state) {
  $agenda = node_load($form_state['values']['agenda']);
  $count = media_events_copy($agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'], $agenda->field_cm_agenda_video[LANGUAGE_NONE][0]['fid']);
  drupal_set_message(t('@count media events copied', array('@count' => $count)));
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for saving chapter data.
 */
function cm_agenda_live_admin_chapter_form_submit($form, &$form_state) {
  // Find active chapter and end it.
  $agenda = node_load($form_state['values']['agenda']);
  $event = cm_agenda_live_get_active_event($agenda, 'cm_agenda_chapter');
  if ($event->media_event_id != $form_state['values']['chapters']) {
    if (!empty($event)) {
      $event->field_media_event_end[LANGUAGE_NONE][0]['value'] = cm_agenda_get_timeoffset($agenda);
      $event->save();
    }
  }

  // Break event.
  if ($form_state['values']['chapters'] == -2) {
    $entity = entity_create('media_event', array());
    $entity->field_media_event_type[LANGUAGE_NONE][0]['value'] = 'cm_agenda_chapter';
    $entity->field_media_event_title[LANGUAGE_NONE][0]['value'] = t('Break');
    $entity->field_media_event_media_ref[LANGUAGE_NONE][0]['target_id'] = $agenda->field_cm_agenda_live[LANGUAGE_NONE][0]['fid'];
    $entity->field_media_event_start[LANGUAGE_NONE][0]['value'] = cm_agenda_get_timeoffset($agenda);
    $entity->field_media_event_end[LANGUAGE_NONE] = array();
    $entity->save();
    $form_state['input']['chapters'] = $entity->media_event_id;
  }
  if ($form_state['values']['chapters'] > 0 && $event->media_event_id != $form_state['values']['chapters']) {
    // Start chapter.
    $events = entity_load('media_event', array($form_state['values']['chapters']));
    $event = $events[$form_state['values']['chapters']];
    if (!empty($event)) {
      if (empty($event->field_media_event_start[LANGUAGE_NONE][0]['value'])) {
        $event->field_media_event_start[LANGUAGE_NONE][0]['value'] = cm_agenda_get_timeoffset($agenda);
        $event->save();
      }
      else {
        $newentity = entity_create('media_event', array());
        $newentity->field_media_event_type[LANGUAGE_NONE][0]['value'] = 'cm_agenda_chapter';
        $newentity->field_media_event_title[LANGUAGE_NONE][0]['value'] = $event->field_media_event_title[LANGUAGE_NONE][0]['value'];
        $newentity->field_media_event_media_ref[LANGUAGE_NONE][0]['target_id'] = $event->field_media_event_media_ref[LANGUAGE_NONE][0]['target_id'];
        if (!empty($event->field_media_event_longtext[LANGUAGE_NONE][0]['value'])) {
          $newentity->field_media_event_longtext[LANGUAGE_NONE][0]['value'] = $event->field_media_event_longtext[LANGUAGE_NONE][0]['value'];
          $newentity->field_media_event_longtext[LANGUAGE_NONE][0]['format'] = $event->field_media_event_longtext[LANGUAGE_NONE][0]['format'];
        }
        $newentity->field_media_event_start[LANGUAGE_NONE][0]['value'] = cm_agenda_get_timeoffset($agenda);
        $newentity->field_media_event_end[LANGUAGE_NONE] = array();
        $newentity->save();
        $form_state['input']['chapters'] = $newentity->media_event_id;
      }
    }
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for saving delay data.
 */
function cm_agenda_live_admin_delay_submit($form, &$form_state) {
  $agenda = node_load($form_state['values']['agenda']);
  $agenda->field_cm_agenda_delay_time[LANGUAGE_NONE][0]['value'] = $form_state['values']['delay'];
  $agenda->path['pathauto'] = 0;
  node_save($agenda);
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback for updating chapter element.
 */
function cm_agenda_live_admin_chapter_callback($form, &$form_state) {
  $element = $form['chapter-list']['container'];
  return $element;
}

/**
 * Ajax callback for updating speaker element.
 */
function cm_agenda_live_admin_speaker_callback($form, &$form_state) {
  $element = $form['speaker']['speakerbox'];
  return $element;
}

/**
 * Ajax callback for updating workflow element.
 */
function cm_agenda_live_admin_workflow_callback($form, &$form_state) {
  $element = $form['workflow'];
  return $element;
}

/**
 * Ajax callback for updating delay element.
 */
function cm_agenda_live_admin_delay_callback($form, &$form_state) {
  $element = $form['metadata']['delay'];
  return $element;
}
