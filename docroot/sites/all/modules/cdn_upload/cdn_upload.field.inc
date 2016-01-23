<?php

/**
 * Implements hook_field_widget_info().
 *
 * @return type
 */
function cdn_upload_field_widget_info() {
  return array(
    's3_direct_upload' => array(
      'label' => t('S3 Direct Upload'),
      'field types' => array('text'),
      'settings' => array(
        's3_upload_bucket' => '',
        's3_upload_max_size' => 15000000,
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    )
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function cdn_upload_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $form = array();
  if ($widget['type'] == 's3_direct_upload') {
    $defaults = field_info_widget_settings($widget['type']);
    $settings = array_merge($defaults, $widget['settings']);
    $form['s3_upload_bucket'] = array(
      '#type' => 'textfield',
      '#title' => t('S3 Bucket'),
      '#required' => TRUE,
      '#description' => t('The bucket name you wish to direct upload to.'),
      '#default_value' => $settings['s3_upload_bucket'],
      '#weight' => 200,
    );
    $form['s3_upload_max_size'] = array(
      '#type' => 'textfield',
      '#title' => t('Upload Max Size'),
      '#required' => TRUE,
      '#description' => t('The maximum size for the uploads.'),
      '#default_value' => $settings['s3_upload_max_size'],
      '#weight' => 201,
    );
  }
  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function cdn_upload_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  // Add the upload widget.
  $element['upload'] = $element + array(
    '#theme' => 'cdn_upload',
    '#upload_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
    '#cdn_upload_settings' => array(
      'bucket' => $instance['widget']['settings']['s3_upload_bucket'],
      'maxSize' => $instance['widget']['settings']['s3_upload_max_size'],
    )
  );

  // Set the value of the widget.
  $element['value'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
    '#prefix' => '<div style="display:none;">',
    '#suffix' => '</div>',
  );

  return $element;
}
