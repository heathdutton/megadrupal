<?php

/**
 * @file
 * Action callbacks for the media_youtube_upload module.
 */

/**
 * Implements hook_action_info().
 */
function media_youtube_upload_action_info() {
  return array(
    'media_youtube_upload_update_privacy_action' => array(
      'type' => 'file',
      'label' => t('Change the privacy of a YouTube video.'),
      'behavior' => array('changes_property'),
      'configurable' => TRUE,
      'triggers' => array('any'),
    ),
    'media_youtube_upload_update_category_action' => array(
      'type' => 'file',
      'label' => t('Change the category of a YouTube video.'),
      'behavior' => array('changes_property'),
      'configurable' => TRUE,
      'triggers' => array('any'),
    ),
  );
}

/**
 * Update privacy action.
 *
 * @param object $file
 *   The file the action is being executed on.
 * @param array $context
 *   The context of the action.
 *
 * @global object $user
 *   The user for which we do an access control check.
 *
 * @see media_youtube_upload_action_info()
 */
function media_youtube_upload_update_privacy_action($file, $context) {
  global $user;
  if (file_entity_access('edit', $file, $user)) {
    if ($file->type === 'youtube') {
      $file->field_file_youtube_privacy[LANGUAGE_NONE][0]['value'] = $context['privacy'];
      file_entity_file_update($file);
    }
  }
}

/**
 * Configuration form for privacy update action.
 *
 * @param array $context
 *   The context of the action being executed.
 *
 * @return array $form
 *   The configuration form for the action being executed.
 *
 * @see media_youtube_upload_update_privacy_action_validate()
 * @see media_youtube_upload_update_privacy_action_submit()
 */
function media_youtube_upload_update_privacy_action_form($context) {
  $form = array();

  $privacy_options = array(
    'private' => t('Private'),
    'public' => t('Public'),
    'unlisted' => t('Unlisted'),
  );
  $form['privacy'] = array(
    '#type' => 'select',
    '#title' => t('Privacy'),
    '#options' => $privacy_options,
    '#default_value' => $privacy_options['private'],
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Validation function for the privacy update action form.
 *
 * @see media_youtube_upload_update_privacy_action_form()
 */
function media_youtube_upload_update_privacy_action_validate($form, $form_state) {
  // TODO: Validate the selected value.
}

/**
 * Submit function for the privacy update action form.
 *
 * @see media_youtube_upload_update_privacy_action_form()
 */
function media_youtube_upload_update_privacy_action_submit($form, $form_state) {
  return array('privacy' => $form_state['values']['privacy']);
}

/**
 * Update category action.
 *
 * @param object $file
 *   The file the action is being executed on.
 * @param array $context
 *   The context of the action.
 *
 * @global object $user
 *   The user for which we do an access control check.
 *
 * @see media_youtube_upload_action_info()
 */
function media_youtube_upload_update_category_action($file, $context) {
  global $user;
  if (file_entity_access('edit', $file, $user)) {
    if ($file->type === 'youtube') {
      $file->field_file_youtube_category[LANGUAGE_NONE][0]['value'] = $context['category'];
      file_entity_file_update($file);
    }
  }
}
/**
 * Configuration form for category update action.
 *
 * @param array $context
 *   The context of the action being executed.
 *
 * @return array $form
 *   The configuration form for the action being executed.
 *
 * @see media_youtube_upload_update_category_action_validate()
 * @see media_youtube_upload_update_category_action_submit()
 */
function media_youtube_upload_update_category_action_form($context) {
  $form = array();

  // Get the categories.
  $ytapi = new MediaYoutubeUploadYtapi();
  $categories = $ytapi->setCategories();
  if (empty($categories['error'])) {
    return $categories;
  }

  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#options' => $categories,
    '#default_value' => '_none',
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Validation function for the category update action form.
 *
 * @see media_youtube_upload_update_category_action_form()
 */
function media_youtube_upload_update_category_action_validate($form, $form_state) {
  // TODO: Validate the selected value.
}

/**
 * Submit function for the category update action form.
 *
 * @see media_youtube_upload_update_category_action_form()
 */
function media_youtube_upload_update_category_action_submit($form, $form_state) {
  return array('category' => $form_state['values']['category']);
}
