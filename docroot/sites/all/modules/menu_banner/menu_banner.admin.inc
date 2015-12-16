<?php

/**
 * @file
 * Module settings and configuration administration UI.
 */

/**
 * Menu handler for admin/config/user-interface/menu-banner.
 */
function menu_banner_settings() {
  $form = array();
  $form['menu_banner_default_image'] = array(
    '#title' => t('Default banner image'),
    '#type' => 'managed_file',
    '#upload_location' => 'public://menu_banner_images',
    '#default_value' => isset($form['menu_banner_default_image'])
      ? $form['menu_banner_default_image']
      : variable_get('menu_banner_default_image', ''),
    '#upload_validators' => array(
      'file_validate_extensions' => array('png gif jpg'),
      // 'file_validate_size' => array(MAX_FILE_SIZE * 1024 * 1024),
    ),
  );
  $menu_banner_default_text = variable_get('menu_banner_default_text', array(
    'value' => '',
    'format' => filter_default_format(),
  ));
  $form['menu_banner_default_text'] = array(
    '#title' => t('Default banner text'),
    '#type' => 'text_format',
    '#default_value' => isset($form['menu_banner_text']['value']) ? $form['menu_banner_text']['value']
       : $menu_banner_default_text['value'],
    '#rows' => 3,
    '#format' => isset($form['menu_banner_text']['format']) ? $form['menu_banner_text']['format']
       : $menu_banner_default_text['format'],
  );
  $form['#submit'][] = 'menu_banner_settings_submit';

  return system_settings_form($form);
}

/**
 * Settings page submit callback.
 */
function menu_banner_settings_submit($form, &$form_state) {
  $banner_image_fid = $form_state['values']['menu_banner_default_image'];

  // Add image.
  if ($banner_image_fid != 0) {
    $file = file_load($banner_image_fid);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'menu_banner', 'banner_default_image', 1);
  }
  // Delete image.
  elseif ($banner_image_fid == 0) {
    $fid = variable_get('menu_banner_default_image', array());
    $file = $fid ? file_load($fid) : FALSE;
    if ($file) {
      file_usage_delete($file, 'menu_banner', 'banner_default_image', 1);
      file_delete($file);
    }
  }
}
