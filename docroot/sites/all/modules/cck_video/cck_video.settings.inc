<?php
/**
 * @file
 * Settings page callback file for the cck_video module.
 */

/**
 * Menu callback;
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cck_video_admin_settings($form, &$form_state) {
  $form['cck_video_ext'] = array(
    '#type' => 'fieldset',
    '#weight' => -20,
    '#title' => t('File extensions'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['cck_video_ext']['cck_video_video_ext'] = array(
    '#type' => 'textfield',
    '#title' => t('Video Extension'),
    '#default_value' => variable_get('cck_video_video_ext', 'flv,swf,mp4,wmv'),
    '#description' => t('These extensions will use as video.'),
  );

  $form['cck_video_ext']['cck_video_image_ext'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Extension'),
    '#default_value' => variable_get('cck_video_image_ext', 'jpg,bmp,png'),
    '#description' => t('These extensions will use html IMG tag instead. If file extension does not meet Video and Image extention then will be dispalayed as file link.'),
  );

  $form['cck_video_dimension'] = array(
    '#type' => 'fieldset',
    '#weight' => -10,
    '#title' => t('Video File Dimension'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['cck_video_dimension']['cck_video_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Video Width'),
    '#default_value' => variable_get('cck_video_width', '570'),
    '#description' => t('Only affect to Video size. Image size still autosize,'),
  );

  $form['cck_video_dimension']['cck_video_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Video Height'),
    '#default_value' => variable_get('cck_video_height', '334'),
    '#description' => t('Only affect to Video size. Image size still autosize,'),
  );

  return system_settings_form($form);
}

