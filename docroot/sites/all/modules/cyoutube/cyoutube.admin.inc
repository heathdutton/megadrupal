<?php

/**
 * @file
 * Youtube Sync Admin UI
 *
 * contains all forms and saving function for the YouTube Configuration UI
 */

/**
 * Defines a settings form.
 */
function cyoutube_settings($form) {
  $form['cyoutube_m1'] = array(
    '#type' => 'markup',
    '#markup' => t('It is the place to set up the YouTube sync. This module will provide some flexibility to developers to sync youTube videos with local video file entity and its fields. It provides the interface to set values of file entity custom fields by implementing a hook. For more detail, refer to API document.'),
  );
  $form['cyoutube_fieldset1'] = array(
    '#type' => 'fieldset',
    '#title' => t('YouTube Developer Credential'),
  );
  $form['cyoutube_fieldset2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Source of Channels'),
  );
  $form['cyoutube_fieldset3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entity field mapping'),
  );
  $form['cyoutube_fieldset2']['cyoutube_channel_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Field used to store the YouTube channel name.'),
    '#default_value' => variable_get('cyoutube_channel_name', ''),
    '#description' => t("If there are more fields? use ',' to seperate between different field name. It will apply same field accoss different entities and bundles."),
  );
  $form['cyoutube_fieldset1']['cyoutube_developer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Developer Key'),
    '#default_value' => variable_get('cyoutube_developer_key', ''),
  );
  $form['cyoutube_fieldset3']['cyoutube_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Field machine name used to store the YouTube video description.'),
    '#default_value' => variable_get('cyoutube_description', ''),
    '#description' => t("Leave it empty if you do not want to import this field from youtube."),
    '#element_validate' => array('cyoutube_video_file_field_textarea_validate'),
  );
  $form['cyoutube_fieldset3']['cyoutube_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Field machine name used to store the YouTube id.'),
    '#default_value' => variable_get('cyoutube_id', ''),
    '#description' => t("Leave it empty if you do not want to import this field from youtube."),
    '#element_validate' => array('cyoutube_video_file_field_text_validate'),
  );
  $form['cyoutube_fieldset3']['cyoutube_video_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Field machine name used to store the YouTube video id.'),
    '#default_value' => variable_get('cyoutube_video_id', ''),
    '#description' => t("Setup database index for the value column of this field will enhance the performance on import videos, Leave it empty if you do not want to import this field from youtube."),
    '#required' => TRUE,
    '#element_validate' => array('cyoutube_video_file_field_text_validate'),
  );
  $form['cyoutube_fieldset3']['cyoutube_thumbnail_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Field machine name used to store the YouTube default video thumbnail.'),
    '#default_value' => variable_get('cyoutube_thumbnail_default', ''),
    '#description' => t("Leave it empty if you do not want to import this field from youtube."),
    '#element_validate' => array('cyoutube_video_file_field_validate'),
  );
  $form['cyoutube_fieldset3']['cyoutube_thumbnail_medium'] = array(
    '#type' => 'textfield',
    '#title' => t('Field machine name used to store the YouTube medium video thumbnail.'),
    '#default_value' => variable_get('cyoutube_thumbnail_medium', ''),
    '#description' => t("Leave it empty if you do not want to import this field from youtube."),
    '#element_validate' => array('cyoutube_video_file_field_validate'),
  );
  $form['cyoutube_fieldset3']['cyoutube_thumbnail_high'] = array(
    '#type' => 'textfield',
    '#title' => t('Field machine name used to store the YouTube high video thumbnail.'),
    '#default_value' => variable_get('cyoutube_thumbnail_high', ''),
    '#description' => t("Leave it empty if you do not want to import this field from youtube."),
    '#element_validate' => array('cyoutube_video_file_field_validate'),
  );
  $form['cyoutube_fieldset3']['cyoutube_published_timestamp'] = array(
    '#type' => 'textfield',
    '#title' => t('Field machine name used to store the YouTube published timestamp.'),
    '#default_value' => variable_get('cyoutube_published_timestamp', ''),
    '#description' => t("Leave it empty if you do not want to import this field from youtube."),
    '#element_validate' => array('cyoutube_video_file_field_date_validate'),
  );
  return system_settings_form($form);
}

/**
 * Validates the fields from form.
 */
function cyoutube_video_file_field_validate($element, $form_state) {
  $field = $element['#value'];
  if (empty($field)) {
    return;
  }
  $a = field_info_instance('file', $field, 'video');
  if (empty($a)) {
    form_error($element, t('It is not a field from video bundle of file entity.'));
  }
}

/**
 * Validates the long text fields from form.
 */
function cyoutube_video_file_field_textarea_validate($element, $form_state) {
  $field = $element['#value'];
  if (empty($field)) {
    return;
  }
  $a = field_info_instance('file', $field, 'video');
  if (empty($a)) {
    form_error($element, t('It is not a field from video bundle of file entity.'));
  }
  else {
    if (isset($a['widget']['type']) && isset($a['widget']['module'])) {
      if ($a['widget']['type'] == 'text_textarea' && $a['widget']['module'] == 'text') {
        return;
      }
    }
  }
  form_error($element, t('Need to be a long text field'));
}

/**
 * Validates the text fields from form.
 */
function cyoutube_video_file_field_text_validate($element, $form_state) {
  $field = $element['#value'];
  if (empty($field)) {
    return;
  }
  $a = field_info_instance('file', $field, 'video');
  if (empty($a)) {
    form_error($element, t('It is not a field from video bundle of file entity.'));
  }
  else {
    if (isset($a['widget']['type']) && isset($a['widget']['module'])) {
      if ($a['widget']['type'] == 'text_textfield' && $a['widget']['module'] == 'text') {
        return;
      }
    }
  }
  form_error($element, t('Need to be a text field'));
}

/**
 * Validates the date fields from form.
 */
function cyoutube_video_file_field_date_validate($element, $form_state) {
  $field = $element['#value'];
  if (empty($field)) {
    return;
  }
  $a = field_info_instance('file', $field, 'video');
  if (empty($a)) {
    form_error($element, t('It is not a field from video bundle of file entity.'));
  }
  else {
    if (isset($a['widget']['type']) && isset($a['widget']['module'])) {
      if ($a['widget']['module'] == 'date') {
        return;
      }
    }
  }
  form_error($element, t('Need to be a date field'));
}
