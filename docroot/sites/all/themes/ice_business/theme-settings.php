<?php

/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */

  // We are editing the $form in place, so we don't need to return anything.
  function icebusiness_form_system_theme_settings_alter(&$form, $form_state, $form_id = NULL) {
 if (isset($form_id)) {
    return;
  }

  $form['support']['ice_html5_respond_meta'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Add HTML5 and responsive scripts and meta tags to every page.'),
    '#default_value' => theme_get_setting('ice_html5_respond_meta'),
    '#options' => array(
    'meta' => t('Add meta tags to support responsive design on mobile devices.'),
    ),
    '#description' => t('IE 6-8 require a JavaScript polyfill solution to add basic support of HTML5 and CSS3 media queries. If you prefer to use another polyfill solution, such as <a href="!link">Modernizr</a>, you can disable these options. Mobile devices require a few meta tags for responsive designs.', array('!link' => 'http://www.modernizr.com/')),
    );
  $form['icebusiness_settings']['twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Username'),
    '#default_value' => theme_get_setting('twitter_username'),
    '#description'   => t("Enter your Twitter Full Url (http://www.twitter.com/abc)."),
  );
  $form['icebusiness_settings']['facebook_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Username'),
    '#default_value' => theme_get_setting('facebook_username'),
    '#description'   => t("Enter your Facebook Full Url (http://www.facebook.com/abc)."),
  );
    $form['icebusiness_settings']['linkedin_username'] = array(
    '#type' => 'textfield',
    '#title' => t('linkedin Username'),
    '#default_value' => theme_get_setting('linkedin_username'),
    '#description'   => t("Enter your LinkedIn Full Url (http://www.linkedin.com/abc)."),
  );
     $form['icebusiness_settings']['blog_username'] = array(
    '#type' => 'textfield',
    '#title' => t('blog Username'),
    '#default_value' => theme_get_setting('blog_username'),
    '#description'   => t("Enter your blog Full Url."),
  );

/************* slider function ***************************/

  $form['slider'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slider Images'),
    '#description' => t("Please Upload your slider image.")
  );
  $form['slider']['use_slider'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable default slider.'),
    '#default_value' =>  theme_get_setting('use_slider'),
   '#description'=> 'Image Diemension : 914x290',
  );  

   $form['slider']['image1_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to slider image'), 
   '#default_value' =>  base_path().file_stream_wrapper_get_instance_by_uri('public://')->getDirectoryPath().'/'.file_uri_target(theme_get_setting('image1_path')),
  );

    $form['slider']['image_1_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload your slider image'),
   '#description'=> 'Image Diemension : 914x290',
  );
   $form['slider']['image2_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to slider image'), 
   '#default_value' =>  base_path().file_stream_wrapper_get_instance_by_uri('public://')->getDirectoryPath().'/'.file_uri_target(theme_get_setting('image2_path')),
  );

    $form['slider']['image_2_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload your slider image'),
   '#description'=> 'Image Diemension : 914x290',
  );

$form['slider']['image3_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to slider image'), 
   '#default_value' =>  base_path().file_stream_wrapper_get_instance_by_uri('public://')->getDirectoryPath().'/'.file_uri_target(theme_get_setting('image3_path')),
  );

    $form['slider']['image_3_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload your slider image'),
   '#description'=> 'Image Diemension : 914x290',
  );

  $form['#submit'][] = 'icebusiness_custom_settings_submit';
 
  return $form;
}


function icebusiness_custom_settings_submit($form, &$form_state) {

  // Check for a new uploaded file, and use that if available.
  if ($file = file_save_upload('image_1_upload')) {
    $parts = pathinfo($file->filename);
    $destination = 'public://' . $parts['basename'];
    $file->status = FILE_STATUS_PERMANENT;
    if (file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
        $_POST['image1_path'] = $form_state['values']['image1_path'] = $destination;
     }
  }
  if ($file = file_save_upload('image_2_upload')) {

    $parts = pathinfo($file->filename);
       $destination = 'public://' . $parts['basename'];
    $file->status = FILE_STATUS_PERMANENT;
    if (file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
        $_POST['image2_path'] = $form_state['values']['image2_path'] = $destination;
     }
  }
  if ($file = file_save_upload('image_3_upload')) {

    $parts = pathinfo($file->filename);
        $destination = 'public://' . $parts['basename'];
    $file->status = FILE_STATUS_PERMANENT;
    if (file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
        $_POST['image3_path'] = $form_state['values']['image3_path'] = $destination;
     }
  }

}
