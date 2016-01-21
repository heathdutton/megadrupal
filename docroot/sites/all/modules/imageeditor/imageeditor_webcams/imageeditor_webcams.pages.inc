<?php
/**
 * @file
 * Different pages for Image Editor Webcams module.
 */

/**
 * Page callback for Mail.ru Camera webcam.
 */
function imageeditor_webcams_mailru_camera() {
  if (module_exists('libraries')) {
    $fileapi_path = libraries_get_path('FileAPI');
  }
  else {
    $fileapi_path = 'sites/all/libraries/FileAPI';
  }
  drupal_add_js('window.FileAPI = {media: true, staticPath: "/' . $fileapi_path . '/dist/"}', 'inline');
  drupal_add_js($fileapi_path . '/dist/FileAPI.min.js', array('scope' => 'footer', 'weight' => -10));
  //drupal_add_js($fileapi_path . '/dist/jquery.fileapi.min.js', array('scope' => 'footer'));
  $path = drupal_get_path('module', 'imageeditor_webcams');
  drupal_add_js($path . '/js/imageeditor.mailru_camera.js', array('scope' => 'footer'));
  drupal_add_css($path . '/css/imageeditor_webcams.css');

  return theme('imageeditor_webcams');
}

/**
 * Page callback for Photobooth webcam.
 */
function imageeditor_webcams_photobooth() {
  if (module_exists('libraries')) {
    drupal_add_js(libraries_get_path('photobooth') . '/photobooth_min.js', array('scope' => 'footer'));
  }
  else {
    drupal_add_js('sites/all/libraries/photobooth' . '/photobooth_min.js', array('scope' => 'footer'));
  }
  $path = drupal_get_path('module', 'imageeditor_webcams');
  drupal_add_js($path . '/js/imageeditor.photobooth.js', array('scope' => 'footer'));
  drupal_add_css($path . '/css/imageeditor_webcams.css');

  return theme('imageeditor_webcams');
}

/**
 * Page callback for jQuery webcam.
 */
function imageeditor_webcams_webcam() {
  drupal_add_js('sites/all/libraries/jquery-webcam/jquery.webcam.js');
  drupal_add_js(drupal_get_path('module', 'imageeditor_webcams') . '/js/imageeditor.webcam.js');
  drupal_add_js(array('imageeditor' => array('webcam' => array('swffile' => '/sites/all/libraries/jquery-webcam/jscam.swf'))), 'setting');

  $output = '<div id="imageeditor_webcam_status"></div>';
  $output .= '<div id="imageeditor_webcam"></div>';
  $output .= '<div id="imageeditor_webcam_cams"></div>';
  $output .= theme('item_list', array(
    'title' => 'Available options',
    'items' => array(
      array('data' => t('Take a picture instantly'), 'id' => 'imageeditor_webcam_capture'),
      array('data' => t('Take a picture after 5 seconds'), 'id' => 'imageeditor_webcam_capture5'),
    ),
  ));

  return $output;
}

/**
 * Page callback for ScriptCam webcam.
 */
function imageeditor_webcams_scriptcam() {
  drupal_add_js('sites/all/libraries/swfobject/src/swfobject.js');
  drupal_add_js('sites/all/libraries/jquery-scriptcam/scriptcam.js');
  drupal_add_js(drupal_get_path('module', 'imageeditor_webcams') . '/js/imageeditor.scriptcam.js');
  drupal_add_js(array('imageeditor' => array('scriptcam' => array('path' => '/sites/all/libraries/jquery-scriptcam/'))), 'setting');

  $output = '<div id="imageeditor_scriptcam_status"></div>';
  $output .= '<div id="imageeditor_scriptcam"></div>';
  $output .= '<select id="imageeditor_scriptcam_cams"></select>';
  $output .= '<select id="imageeditor_scriptcam_mics"></select>';

  return $output;
}
