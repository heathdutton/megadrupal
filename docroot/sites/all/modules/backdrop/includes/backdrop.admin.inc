<?php
/**
 * Created by JetBrains PhpStorm.
 * User: Nico Heulsen
 * Email: info@artx.be
 * Date: 25/11/11
 * Time: 11:50
 */


/**
 * Returns a admin settings form
 *
 * @param $form
 * @param $form_state
 * @return array
 */
function backdrop_admin_settings_form($form, &$form_state) {
  $php_max = ini_get('upload_max_filesize');


  $form['backdrop_settings_file_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Filepath'),
    '#description' => t('Enter the filepath relative to the default files folder. No trailing slash!'),
    '#default_value' => variable_get('backdrop_settings_file_path', 'backdrops'),
  );

  $form['backdrop_settings_max_upload_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Max uploadsize'),
    '#field_suffix' => 'Kb',
    '#size' => 5,
    '#description' => t('Enter the max file size a user may upload (in KB). Current upload is limited in php configuration by <strong>!limit</strong>', array('!limit' => $php_max)),
    '#default_value' => variable_get('backdrop_settings_max_upload_size', '1024'),
  );

  $form['backdrop_settings_allowed_file_types'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed Filetypes'),
    '#description' => t('Enter a list of file extensions for all backdrop files, seperated by a space.'),
    '#default_value' => variable_get('backdrop_settings_allowed_file_types', 'jpg png gif'),
  );

  $form['backdrop_settings_max_resolution'] = array(
    '#type' => 'textfield',
    '#title' => t('Resolution'),
    '#size' => 10,
    '#description' => t('The maximum resolution (widthxheight)'),
    '#default_value' => variable_get('backdrop_settings_max_resolution'),
  );

  $form['backdrop_settings_exact_resolution'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exact resolution'),
    '#description' => t('When checked, the resolution of the image uploaded should match exactly the with and height given above.'),
    '#default_value' => variable_get('backdrop_settings_exact_resolution'),
  );

  return system_settings_form($form);
}