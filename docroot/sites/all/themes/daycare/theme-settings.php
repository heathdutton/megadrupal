<?php
/**
 * @file
 * Form override fo theme settings
 */
/**
 * implements daycare_form_system_theme_settings_alter()
 */
function daycare_form_system_theme_settings_alter(&$form, $form_state) {

  $form['options_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme Specific Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['options_settings']['daycare_tabs'] = array(
    '#type' => 'checkbox',
    '#title' =>  t('Use the ZEN tabs'),
    '#description'   => t('Check this if you wish to replace the default tabs by the ZEN tabs'),
    '#default_value' => theme_get_setting('daycare_tabs'),
  );
  $form['options_settings']['daycare_breadcrumb'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Breadcrumb settings'),
    '#attributes'    => array('id' => 'daycare-breadcrumb'),
  );
  $form['options_settings']['daycare_breadcrumb']['daycare_breadcrumb'] = array(
    '#type'          => 'select',
    '#title'         => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('daycare_breadcrumb'),
    '#options'       => array(
                          'yes'   => t('Yes'),
                          'admin' => t('Only in admin section'),
                          'no'    => t('No'),
                        ),
  );
  $form['options_settings']['daycare_breadcrumb']['daycare_breadcrumb_separator'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Breadcrumb separator'),
    '#description'   => t('Text only. Don’t forget to include spaces.'),
    '#default_value' => theme_get_setting('daycare_breadcrumb_separator'),
    '#size'          => 5,
    '#maxlength'     => 10,
    '#prefix'        => '<div id="div-daycare-breadcrumb-collapse">', // jquery hook to show/hide optional widgets
  );
  $form['options_settings']['daycare_breadcrumb']['daycare_breadcrumb_home'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show home page link in breadcrumb'),
    '#default_value' => theme_get_setting('daycare_breadcrumb_home'),
  );
  $form['options_settings']['daycare_breadcrumb']['daycare_breadcrumb_trailing'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Append a separator to the end of the breadcrumb'),
    '#default_value' => theme_get_setting('daycare_breadcrumb_trailing'),
    '#description'   => t('Useful when the breadcrumb is placed just before the title.'),
  );
  $form['options_settings']['daycare_breadcrumb']['daycare_breadcrumb_title'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Append the content title to the end of the breadcrumb'),
    '#default_value' => theme_get_setting('daycare_breadcrumb_title'),
    '#description'   => t('Useful when the breadcrumb is not placed just before the title.'),
    '#suffix'        => '</div>', // #div-daycare-breadcrumb-collapse"
  );

  //IE specific settings.
  $form['options_settings']['daycare_ie'] = array(
    '#type' => 'fieldset',
    '#title' => t('Internet Explorer Stylesheets'),
    '#attributes' => array('id' => 'daycare-ie'),
  );
  $form['options_settings']['daycare_ie']['daycare_ie_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Internet Explorer stylesheets in theme'),
    '#default_value' => theme_get_setting('daycare_ie_enabled'),
    '#description' => t('If you check this box you can choose which IE stylesheets in theme get rendered on display.'),
  );
  $form['options_settings']['daycare_ie']['daycare_ie_enabled_css'] = array(
    '#type' => 'fieldset',
    '#title' => t('Check which IE versions you want to enable additional .css stylesheets for.'),
    '#states' => array(
      'visible' => array(
        ':input[name="daycare_ie_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['options_settings']['daycare_ie']['daycare_ie_enabled_css']['daycare_ie_enabled_versions'] = array(
    '#type' => 'checkboxes',
    '#options' => array(
      'ie8' => t('Internet Explorer 8'),
      'ie9' => t('Internet Explorer 9'),
      'ie10' => t('Internet Explorer 10'),
    ),
    '#default_value' => theme_get_setting('daycare_ie_enabled_versions'),
  );
  $form['options_settings']['clear_registry'] = array(
    '#type' => 'checkbox',
    '#title' =>  t('Rebuild theme registry on every page.'),
    '#description'   => t('During theme development, it can be very useful to continuously <a href="!link">rebuild the theme registry</a>. WARNING: this is a huge performance penalty and must be turned off on production websites.', array('!link' => 'http://drupal.org/node/173880#theme-registry')),
    '#default_value' => theme_get_setting('clear_registry'),
  );

}

/**
 * Custom theme settings
 * Advanced settings
 */
$form['adv_header'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE, );
$form['adv_header']['header_bg_file'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the header image'),
    '#default_value' => theme_get_setting('header_bg_file'),
    '#description' => t('If the background image is bigger than the header area, it is clipped. If it\'s smaller than the header area, it is tiled to fill the header area. To remove the background image, blank this field and save the settings.'),
    '#size' => 40,
    '#maxlength' => 120,
  );
  $form['adv_header']['header_bg'] = array(
    '#type' => 'file',
    '#title' => t('Upload header image'),
    '#size' => 40,
    '#attributes' => array('enctype' => 'multipart/form-data'),
    '#description' => t('If you don\'t jave direct access to the server, use this field to upload your header background image'),
    '#element_validate' => array('daycare_header_bg_validate'),
  );
  $form['adv_header']['bg_file'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the Body background image'),
    '#default_value' => theme_get_setting('bg_file'),
    '#description' => t('If the background image is bigger than the body area, it is clipped. If it\'s smaller than the body area, it is tiled to fill the header area. To remove the background image, blank this field and save the settings.'),
    '#size' => 40,
    '#maxlength' => 120,
  );
  $form['adv_header']['background'] = array(
    '#type' => 'file',
    '#title' => t('Upload Body background image'),
    '#size' => 40,
    '#attributes' => array('enctype' => 'multipart/form-data'),
    '#description' => t('If you don\'t jave direct access to the server, use this field to upload your background image'),
    '#element_validate' => array('daycare_background_validate'),
  );
  /* Add Social Links */
  $form['social_settings']['socialicon'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Icons'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['social_settings']['socialicon']['socialicon_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Social Icons'),
    '#default_value' => theme_get_setting('socialicon_display' , 'daycare'),
    '#description'   => t("Check this option to show Social Icon. Uncheck to hide."),
  );
  $form['social_settings']['socialicon']['twitter_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Profile URL'),
    '#default_value' => theme_get_setting('twitter_url', 'daycare'),
    '#description'   => t("Enter your Twitter Profile URL. Leave blank to hide."),
  );
  $form['social_settings']['socialicon']['facebook_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Profile URL'),
    '#default_value' => theme_get_setting('facebook_url', 'daycare'),
    '#description'   => t("Enter your Facebook Profile URL. Leave blank to hide."),
  );
  $form['social_settings']['socialicon']['linkedin_url'] = array(
    '#type' => 'textfield',
    '#title' => t('LinkedIn Profile URL'),
    '#default_value' => theme_get_setting('linkedin_url', 'daycare'),
    '#description'   => t("Enter your LinkedIn Profile URL. Leave blank to hide."),
  );
/**
 * Check and save the uploaded header background image
 */
/**
 * implements daycare_header_bg_validate
 */
function daycare_header_bg_validate($element, &$form_state) {
  global $base_url;

  $validators = array('file_validate_is_image' => array());
  $file = file_save_upload('header_bg', $validators, "public://", FILE_EXISTS_REPLACE);

  if ($file) {
    // change file's status from temporary to permanent and update file database
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    $file_url = file_create_url($file->uri);
    $file_url = str_ireplace($base_url . '/' , '' , $file_url);
   // set to form
    $form_state['values']['header_bg_file'] = $file_url;
  }
}

/**
 * Check and save the uploaded background image
 */
/**
 * implements ddaycare_background_validate
 */
function daycare_background_validate($element, &$form_state) {
  global $base_url;

  $validators = array('file_validate_is_image' => array());
  $file = file_save_upload('background', $validators, "public://", FILE_EXISTS_REPLACE);

  if ($file) {
    // change file's status from temporary to permanent and update file database
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    $file_url = file_create_url($file->uri);
    $file_url = str_ireplace($base_url . '/', '', $file_url);

    // set to form
    $form_state['values']['bg_file'] = $file_url;
  }
}