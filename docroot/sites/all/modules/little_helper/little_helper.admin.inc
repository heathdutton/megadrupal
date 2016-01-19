<?php
/**
 * @file
 * Includes any administrator functionality
 */

/**
 * Helper Function.
 * @Return: A form array defining the backend form where
 * the admin will configure this module.
 * (array)
 */
function little_helper_generate_settings_form() {

  $arr_form = array();

  // Image upload field.
  $arr_form['little_helper_config_img'] = array(
    '#type' => 'managed_file',
    '#name' => t('Chatter Image'),
    '#title' => t('Image of your Helper'),
    '#description' =>
    t('An image of a helper chatting.') . '<br/>' .
    t(
      'The dimensions of the image should be between @max_width - @max_height and @min_width - @min_height',
      array(
        '@max_width' => variable_get('little_helper_chatter_maxwidth') . 'px',
        '@min_width' => variable_get('little_helper_chatter_minwidth') . 'px',
        '@max_height' => variable_get('little_helper_chatter_maxheight') . 'px',
        '@min_height' => variable_get('little_helper_chatter_minheight') . 'px',
      )
    ),
    '#default_value' => variable_get('little_helper_config_img', ''),
    '#upload_location' => 'public://little_helper_chatter_images',
    '#upload_validators' => array(
      'file_validate_extensions' => array('png gif jpg jpeg'),
      'file_validate_image_resolution' => array(
        variable_get('little_helper_chatter_maxwidth') . 'x' . variable_get('little_helper_chatter_maxheight'),
        variable_get('little_helper_chatter_minwidth') . 'x' . variable_get('little_helper_chatter_minheight'),
      ),
    ),
  );

  // Text area for tips.
  $arr_form['little_helper_config_tips'] = array(
    '#type' => 'textarea',
    '#title' => t('Tips'),
    '#description' =>
    t('The tips you want your helper to say.') . '<br/>' .
    t('One tip per line.') . '<br/><strong>' .
    t('HTML markup is allowed for links and bold words.') . '</strong>',
    '#default_value' => variable_get('little_helper_config_tips'),
    '#required' => TRUE,
    '#resizable' => FALSE,
  );

  $arr_form = system_settings_form($arr_form);

  // Attach additional handlers defined in this script.
  array_push($arr_form['#submit'], 'little_helper_chatter_settings_submit');

  return $arr_form;
}

/**
 * Additional functionality after the settings form is submitted.
 */
function little_helper_chatter_settings_submit($form, &$form_state) {

  // Filter saved tips properly.
  variable_set(
    'little_helper_config_tips',
    filter_xss(
      $form_state['values']['little_helper_config_tips'],
      array(
        'a',
        'em',
        'i',
        'strong',
      )
    )
  );

  // Save the helper image as a permanent file.
  if ($form_state['values']['little_helper_config_img'] != 0) {
    $o_helperImage = file_load($form_state['values']['little_helper_config_img']);
    $o_helperImage->status = FILE_STATUS_PERMANENT;
    file_usage_add($o_helperImage, 'little_helper', 'user', 1);
    file_save($o_helperImage);
  }

}
