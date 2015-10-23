<?php

/**
 * @file
 * Mediabox display pages class.
 */

/**
 * Upload form.
 *
 * @param $form
 * @param $form_state
 * @param $field_data array
 *   Mediabox field data.
 * @return array
 *   Form array.
 */
function mediabox_ui_upload_form($form, &$form_state, $field_data) {
  module_load_include('inc', 'mediabox_library', 'includes/mediabox_library.forms');

  $field_info = field_info_field($field_data['field_name']);
  $instance = field_info_instance($field_data['entity_type'], $field_data['field_name'], $field_data['bundle_name']);

  $form_state['mediabox_field_data'] = $field_data;

  // Get plupload form.
  $upload_options = array(
    'upload_validators' => array('file_validate_extensions' => array($instance['settings']['file_extensions'])),
    'event_callbacks' => array('UploadComplete' => 'Drupal.mediabox.uploadCompleteCallback'),
    'plupload_settings' => array(),
  );
  if (!empty($instance['settings']['max_filesize'])) {
    $upload_options['plupload_settings']['max_file_size'] = $instance['settings']['max_filesize'];
  }
  $form = mediabox_library_form_upload($form, $form_state, $upload_options);

  unset($form['bulk_update']);

  // For this upload we can not change mediabox bundles, so lets set field
  // mediabox bundle.
  $form['type'] = array('#type' => 'value', '#value' => $field_info['settings']['mediabox_bundle']);

  // Overwrite submit button - hide submit button because submitting of this
  // form will happen automatically when plupload finish uploading of files.
  $form['submit'] = array(
    '#type' => 'submit',
    '#attributes' => array('class' => array('element-invisible')),
    '#value' => t('Upload Selected Files'),
    '#ajax' => array(
      'callback' => 'mediabox_ui_upload_form_submit_ajax',
    ),
    // nojs support, not implemented for now basically.
    //'#submit' => array('mediabox_ui_upload_form_submit'),
  );

  return $form;
}

/**
 * Ajax submit handler.
 */
function mediabox_ui_upload_form_submit_ajax($form, &$form_state) {
  $field_info = field_info_field($form_state['mediabox_field_data']['field_name']);
  $field_instance = field_info_instance($form_state['mediabox_field_data']['entity_type'], $form_state['mediabox_field_data']['field_name'], $form_state['mediabox_field_data']['bundle_name']);

  // Build upload destination uri.
  $destination = $field_info['settings']['uri_scheme'] . '://';
  if (!empty($field_instance['settings']['file_directory'])) {
    $destination .= $field_instance['settings']['file_directory'] . '/';
  }

  module_load_include('inc', 'mediabox_library', 'includes/mediabox_library.forms');
  $ids = mediabox_library_plupload_save($form_state['values']['files'], $form_state['values']['type'], $destination);
  if (empty($ids)) {
    $commands[] = mediabox_ui_command_close_dialog();
    $commands[] = mediabox_ui_command_info_dialog(t('Error'), t('Error while uploading files.'));
  }
  else {
    $commands[] = mediabox_ui_command_add_files($ids);
  }
  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Upload form callback.
 *
 * @param $js boolean
 * @param $entity_type
 * @param $bundle_name
 * @param string $field_name
 *   Mediabox field name for which we need upload form.
 * @return string
 *   Rendered upload form.
 */
function mediabox_ui_upload_page($js, $entity_type, $bundle_name, $field_name) {
  drupal_add_css(drupal_get_path('module', 'mediabox_ui') . '/css/mediabox_ui.css');

  $output = drupal_get_form('mediabox_ui_upload_form', array(
    'entity_type' => $entity_type,
    'bundle_name' => $bundle_name,
    'field_name' => $field_name,
  ));

  $instance = field_info_instance($entity_type, $field_name, $bundle_name);

  $commands[] = mediabox_ui_command_dialog(
    drupal_render($output),
    array(
      'title' => check_plain($instance['settings']['mediabox_texts']['upload_button']),
      'buttons' => array(
        'mediabox-browser-close' => array(
          'text' => t('Cancel'),
          'class' => 'button form-submit btn btn-default',
        ),
      ),
    ),
    array('field' => $field_name)
  );
  print ajax_render($commands);
  drupal_exit();
}

/**
 * Ajax callback for browser dialog.
 *
 * @param $js
 * @param $entity_type
 * @param $bundle
 * @param string $field_name
 *   Name of the mediabox field.
 * @return string
 *   Rendered Mediabox UI browser.
 */
function mediabox_ui_browser_page($js, $entity_type, $bundle, $field_name) {
  // If there is no mediabox entitites then lets show upload dialog.
  $view = views_get_view('mediabox_browser', 'user_browser');
  $view->execute();
  if (count($view->result) == 0) {
    mediabox_ui_upload_page($js, $entity_type, $bundle, $field_name);
  }

  // We have entities in view so lets continue.

  drupal_add_css(drupal_get_path('module', 'mediabox_ui') . '/css/mediabox_ui.css');

  $field_info = field_info_field($field_name);
  $instance = field_info_instance($entity_type, $field_name, $bundle);

  if (empty($instance)) {
    $commands[] = mediabox_ui_command_info_dialog(t('Error'), t('Can not load mediabox field instance.'));
  }
  else {
    $commands[] = mediabox_ui_command_dialog(
      views_embed_view('mediabox_browser', 'user_browser'),
      array(
        'title' => check_plain($instance['settings']['mediabox_texts']['add_button']),
        'buttons' => array(
          'mediabox-browser-add' => array(
            'text' => check_plain($instance['settings']['mediabox_texts'][$field_info['cardinality'] == 1 ? 'add_image_text' : 'add_images_text']),
            'class' => 'button form-submit btn btn-default btn-primary',
            'callback' => 'mediaboxBrowserDialogAdd',
          ),
          'mediabox-browser-upload' => array(
            'text' => check_plain($instance['settings']['mediabox_texts']['upload_button']),
            'class' => 'button form-submit btn btn-default use-ajax',
            'href' => url('mediabox/upload/nojs/' . $instance['entity_type'] . '/' . $instance['bundle'] . '/' . $field_name),
          ),
          'mediabox-browser-close' => array(
            'text' => t('Close browser'),
            'class' => 'button form-submit btn btn-default',
          ),
        ),
      ),
      array(
        'field' => $field_name,
        'field_cardinality' => $field_info['cardinality'],
        'ids' => isset($_GET['ids']) ? $_GET['ids'] : ''
      )
    );
  }
  print ajax_render($commands);
  drupal_exit();
}

/**
 * Get table of mediabox_display bundles
 * Implementation of page callback admin/structure/mediabox-ui
 * @return string
 */
function mediabox_ui_types() {
  $bundles = mediabox_ui_db_get_bundles();

  if (is_array($bundles)) {
    $header = array(
      array('data' => t('Name')),
      array('data' => t('Operations'), 'colspan' => 4),
    );

    foreach ($bundles as $key => $bundle) {
      $detail = theme('mediabox_type_detail', array(
        'title' => $bundle->title,
        'type' => $bundle->type,
        'description' => isset($bundle->description) ? $bundle->description : ''
      ));

      $items[] = array(
        'data' => array(
          $detail,
          l(t('edit'), 'admin/structure/mediabox-ui/manage/' . $bundle->type),
          l(t('manage fields'), 'admin/structure/mediabox-ui/manage/' . $bundle->type . '/fields'),
          l(t('manage display'), 'admin/structure/mediabox-ui/manage/' . $bundle->type . '/display'),
          l(t('delete'), 'admin/structure/mediabox-ui/' . $bundle->type . '/delete'),
        )
      );
    }

    $variables = array(
      'header' => $header,
      'rows' => isset($items) ? $items : array(),
      'empty' => '',
    );

    return theme('table', $variables);
  }

  drupal_set_message(t('There are no media display types yet added'), 'warning');
  return '';
}
