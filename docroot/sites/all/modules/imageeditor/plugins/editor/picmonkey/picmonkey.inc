<?php
/**
 * @file
 * Picmonkey editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('PicMonkey'),
  'description' => t('Photo editor'),
  'class' => 'picmonkey',
  'site' => 'http://www.picmonkey.com/',
  'api_key' => FALSE,
  'api_key_codename' => 'imageeditor_picmonkey_api_key',
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'image_url_param' => '_import',
  'options' => array(
    '_apikey' => variable_get('imageeditor_picmonkey_api_key', ''),
    '_close_target' => url(imageeditor_ajax_close_path() . '/picmonkey', array('absolute' => TRUE)),
    '_export_agent' => 'browser',
    '_export_method' => 'GET',
    '_export_title' => 'Save to ' . variable_get('site_name', ''),
    '_host_name' => variable_get('site_name', ''),
    '_replace' => 'ask', // Picmonkey will ask: replace an image or create a new one.
    //'_expand_button' => 'true', //adds Expand button and calls onPicknikExpand function
    //'_original_thumb' => '', //url to thumbnail
    '_export' => url(imageeditor_ajax_save_path() . '/picmonkey', array('absolute' => TRUE)),
  ),
  'lang_option' => '_locale',
  'lang_default' => 'en_US',
  'lang_map' => array(
    'en'=> 'en_US', //English
    'de'=> 'de_DE', //German
    'es'=> 'es_ES', //Spanish
    'fr'=> 'fr_FR', //French
    'it'=> 'it_IT', //Italian
    'ja'=> 'jp_JP', //Japanese
    'ru'=> 'ru_RU', //Russian
    'zh-hans'=> 'zh_CN', //Chinese simplified
    'zh-hant'=> 'zh_HK', //Chinese traditional
    'id' => 'id_ID', //Indonesian
    'ko' => 'ko_KR', //Korean
    'nn' => 'no_NO', //Norwegian Nynorsk
    'nb' => 'no_NO', //Norwegian Bokmal
    'pt-br' => 'pt_BR', //Portuguese Brazil
    'pt-pt' => 'pt_BR', //Portuguese Portugal
    'sv' => 'sv_SV', //Swedish
    'vi' => 'vi_VN', //Vietnamese
  ),
  'initialize_callback' => 'imageeditor_picmonkey_initialize_callback',
  'save_callback' => 'imageeditor_picmonkey_save_callback',
  'settings_form_callback' => 'imageeditor_picmonkey_settings_form_callback',
  'js' => 'picmonkey.js',
  'css' => 'picmonkey.css',
);

function imageeditor_picmonkey_initialize_callback(&$editor) {
  global $is_https;
  $editor['options']['launch_url'] = ($is_https ? 'https' : 'http') . '://www.picmonkey.com/service/';
}

function imageeditor_picmonkey_save_callback() {
  $image = '';
  if (isset($_GET['file'])) {
    $directory = imageeditor_temporary_directory();
    $data = drupal_http_request(rawurldecode($_GET['file']));
    $destination = $directory . '/' . $_COOKIE['imageeditor_filename'];
    $file = file_unmanaged_save_data($data->data, $destination, FILE_EXISTS_REPLACE);
    $image = file_create_url($file);
  }
  $js_code = 'if (parent) {';
  if (isset($_GET['_imageid'])) {
    $js_code .= 'parent.Drupal.settings.imageeditor.save.replace = ' . 1 . ';';
  }
  else {
    $js_code .= 'parent.Drupal.settings.imageeditor.save.replace = ' . 0 . ';';
  }
  $js_code .= 'parent.Drupal.settings.imageeditor.save.image = "' . $image . '";';
  $js_code .= 'parent.Drupal.imageeditor.save();';
  $js_code .= 'parent.Drupal.imageeditor.overlay.hide();';
  $js_code .= '}';
  drupal_add_js($js_code, 'inline');
}

function imageeditor_picmonkey_settings_form_callback() {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('PicMonkey'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $link = 'https://docs.google.com/document/pub?id=1OhlOYcCvJiV44c13XpDx0HJV3UArKgRya3lq_Ahr5T0';
  $form['imageeditor_picmonkey_api_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('API key'),
    '#description'   => l($link, $link, array('attributes' => array('target' => '_blank'))),
    '#default_value' => variable_get('imageeditor_picmonkey_api_key', ''),
    '#size'          => 50,
    '#maxlength'     => 100,
    '#required'      => FALSE,
    '#weight'        => 25,
  );

  return $form;
}
