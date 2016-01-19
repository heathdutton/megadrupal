<?php
/**
 * @file
 * Code for the media_event pages.
 */

/**
 * Media Event view callback.
 */
function media_event_view($media_event) {
  drupal_set_title(entity_label('media_event', $media_event));
  return entity_view('media_event', array(entity_id('media_event', $media_event) => $media_event), 'full');
}

/*
 * Media Event copy form
 */
function media_event_copy_form($form, &$form_state) {
  $form['from'] = array(
    '#type' => 'textfield',
    '#title' => 'From Media id',
    '#required' => TRUE,
  );
  $form['to'] = array(
    '#type' => 'textfield',
    '#title' => 'To Media id',
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Copy'),
  );
  return $form;
}

function media_event_copy_form_validate($form, &$form_state) {
  $frommedia = entity_load('file', array($form_state['values']['from']));
  if (empty($frommedia)) {
    form_set_error('from', t('Media file could not be loaded'));
  }
  $tomedia = entity_load('file', array($form_state['values']['to']));
  if (empty($tomedia)) {
    form_set_error('to', t('Media file could not be loaded'));
  }
}

function media_event_copy_form_submit($form, &$form_state) {
  $count = media_events_copy($form_state['values']['from'], $form_state['values']['to']);
  drupal_set_message(t('@count media events copied', array('@count' => $count)));
}

/*
 * Media Event copy form
 */
function media_event_offset_form($form, &$form_state) {
  $form['media'] = array(
    '#type' => 'textfield',
    '#title' => 'Media id',
    '#required' => TRUE,
  );

  $form['offset'] = array(
    '#type' => 'textfield',
    '#title' => 'Offset',
    '#default' => 0,
    '#element_validate' => array('element_validate_integer'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Offset'),
  );
  return $form;
}

function media_event_offset_form_validate($form, &$form_state) {
  $media = entity_load('file', array($form_state['values']['media']));
  if (empty($media)) {
    form_set_error('media', t('Media file could not be loaded'));
  }
}

function media_event_offset_form_submit($form, &$form_state) {
  $query = db_select('field_data_field_media_event_media_ref', 'f');
  $query->condition('f.field_media_event_media_ref_target_id', $form_state['values']['media']);
  $query->fields('f', array('entity_id'));

  $result = $query->execute();
  $eids = $result->fetchCol();
  $count = 0;
  if (count($eids) > 0) {
    $events = entity_load('media_event', $eids);
    foreach ($events as $event) {
      if (isset($event->field_media_event_start[LANGUAGE_NONE][0]['value'])) {
        $event->field_media_event_start[LANGUAGE_NONE][0]['value'] += $form_state['values']['offset'];
      }
      if (isset($event->field_media_event_end[LANGUAGE_NONE][0]['value'])) {
        $event->field_media_event_end[LANGUAGE_NONE][0]['value'] += $form_state['values']['offset'];
      }
      $event->save();
      $count++;
    }
  }
  drupal_set_message(t('@count media events offseted', array('@count' => $count)));
}
