<?php

/**
 * @file
 * hook_menu callback file for pm_user_image module.
 */

/**
 * Form constructor for the PM User Image Settings form.
 */
function pm_user_image_settings() {
  $form = array();
  $form['#prefix'] = "<div class='pm-user-image-desc'><strong>" . t('Image settings for the image to be shown in private message user auto complete') . '</strong></br></br>';
  $form['#suffix'] = "</div>";
  
  $form['pm_user_image_height'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_number'),
    '#description' => t('Specified the height of image to be shown on autocomplete Exampel :- 50'),
    '#title' => t('Image Height'),
    '#default_value' => variable_get('pm_user_image_height', 50),
  );
  
  $form['pm_user_image_width'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_number'),
    '#title' => t('Image Width'),
    '#description' => t('Specified the weight of image to be shown on autocomplete Exampel :- 50'),
    '#default_value' => variable_get('pm_user_image_width', 50),
  );
  
  $form['pm_user_image_path_type'] = array(
    '#type' => 'radios',
    '#title' => t('Image Path Type'),
    '#description' => t('User image from external source or internal image path'),
    '#options' => array(
      1 => t('External'),
      2 => t('Internal'),
    ),
    '#default_value' => variable_get('pm_user_image_path_type', 2),
  );
  
  $form['pm_user_image_default_external'] = array(
    '#type' => 'textfield',
    '#description' => t('Provide the full image path like @external_path', array('@external_path' => 'http://www.xyz.com/image')),
    '#title' => t('Default Image'),
    '#default_value' => variable_get('pm_user_image_default_external', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="pm_user_image_path_type"]' => array('value' => 1),      
      ),
    ),
  );
  
  $form['pm_user_image_default_internal'] = array(
    '#type' => 'managed_file',
    '#title' => t('Default Image'),
    '#description' => t('Allowed Extensions: jpg, jpeg, png, gif'),
    '#states' => array(
      'visible' => array(
        ':input[name="pm_user_image_path_type"]' => array('value' => 2),      
      ),
    ),
    '#default_value' => variable_get('pm_user_image_default_internal', ''),
    '#upload_location' => 'public://pm_user_image/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('png jpg jpeg'),
      'file_validate_size' => array(1 * 1024 * 1024),  
    ),
  );
  
  $style_list = array_keys(image_styles());
  $image_styles = array();
  foreach ($style_list as $key => $val) {
    $image_styles[$val] = ucwords($val);
  }
  $form['pm_user_image_image_style'] = array(
    '#type' => 'select',
    '#title' => t('Image Style'),
    '#description' => t('Select the image style you want to apply. Default style :- thumbnail'),
    '#options' => $image_styles,
    '#default_value' => variable_get('pm_user_image_image_style', 'thumbnail'),
  );
  
  $form['#submit'][] = 'pm_user_image_settings_submit';
  
  return system_settings_form($form);
}

/**
 * Form validation handler for pm_user_image_settings().
 */
function pm_user_image_settings_validate(&$form, &$form_state) {
  $image_height = $form_state['values']['pm_user_image_height'];
  $image_width = $form_state['values']['pm_user_image_width'];
  $path_type = $form_state['values']['pm_user_image_path_type'];
  $external_path = $form_state['values']['pm_user_image_default_external'];
  if ($image_height < 1) {
    form_set_error('pm_user_image_height', t('Image height can not be less than 1'));
  }
  if ($image_width < 1) {
    form_set_error('pm_user_image_width', t('Image width can not be less than 1'));
  }
  if ($path_type == 1) {
    if (!valid_url($external_path, TRUE)) {
      form_set_error('pm_user_image_default_external', t('Invalid default image url'));
    }
  }
}

/**
 * Submission handler for pm_user_image_settings form.
 * @param type $form
 * @param type $form_state 
 */
function pm_user_image_settings_submit($form, &$form_state) {
  $file = file_load($form_state['values']['pm_user_image_default_internal']);
  $file->status = FILE_STATUS_PERMANENT;  //Make the status of the file permananet.
  file_save($file);
}
