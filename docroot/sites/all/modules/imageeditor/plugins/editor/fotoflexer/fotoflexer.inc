<?php
/**
 * @file
 * FotoFlexer editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('FotoFlexer'),
  'description' => t('Simple photo editor'),
  'class' => 'fotoflexer',
  'site' => 'http://fotoflexer.com/',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'popup',
  'image_url_param' => 'ff_image_url',
  'options' => array(
    'launch_url' => 'http://fotoflexer.com/API/API_Loader_v1_01.php',
    'ff_cancel_url' => url(imageeditor_ajax_close_path() . '/fotoflexer', array('absolute' => TRUE)),
    'ff_callback_url' => url(imageeditor_ajax_save_path() . '/fotoflexer', array('absolute' => TRUE)),
  ),
  'lang_option' => 'ff_lang',
  'lang_default' => 'en-US',
  'lang_map' => array(
    'en'=> 'en-US', //English
    'de'=> 'de-DE', //German
    'es'=> 'es-ES', //Spanish
    'fr'=> 'fr-FR', //French
    'it'=> 'it-IT', //Italian
    'ja'=> 'ja-JP', //Japanese
    'ru'=> 'ru-RU', //Russian
    'zh-hans'=> 'zh-CN', //Chinese simplified
    'zh-hant'=> 'zh-TW', //Chinese traditional
    'ko' => 'ko-KR', //Korean
    'nb' => 'nb-NO', //Norwegian Bokmal
    'pl' => 'pl-PL', //Polish
    'pt-br' => 'pt-BR', //Portuguese Brazil
    'pt-pt' => 'pt-PT', //Portuguese Portugal
    'sv' => 'sv_SE', //Swedish
    'vi' => 'vi-VN', //Vietnamese
    'da' => 'da-DK', //Danish
    'nl' => 'nl-NL', //Dutch
    'fi' => 'fi-FI', //Finnish
    'tr' => 'tu-TR', //Turkish
  ),
  // 'initialize_callback' => 'imageeditor_fotoflexer_initialize_callback',
  'save_callback' => 'imageeditor_fotoflexer_save_callback',
  'js' => 'fotoflexer.js',
  'css' => 'fotoflexer.css',
);

function imageeditor_fotoflexer_initialize_callback(&$editor) {
}

function imageeditor_fotoflexer_save_callback() {
  $image = '';
  if (isset($_GET['image'])) {
    $image = rawurldecode($_GET['image']);
  }
  $js_code = 'if (opener) {';
  $js_code .= 'opener.Drupal.settings.imageeditor.save.image = "' . $image . '";';
  $js_code .= 'opener.Drupal.imageeditor.save();';
  $js_code .= 'self.close();';
  $js_code .= '}';
  drupal_add_js($js_code, 'inline');
}
