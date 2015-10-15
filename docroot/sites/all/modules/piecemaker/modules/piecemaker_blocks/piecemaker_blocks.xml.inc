<?php
/**
 * @file
 *   Piecemaker XML related Functions
 */

/**
 * Callback for the building of an XML array
 */
function _piecemaker_blocks_xml($delta) {
  $block = piecemaker_blocks_block_get($delta);
  
  $profile = (array) piecemaker_profile_load($block['pid']);
  
  $xml['Settings'] = $profile['settings'];
  $xml['Transitions'] = $profile['transitions'];
  
  foreach ($block['files'] as $file) {
    $type = $file['type'];
    $func = "_build_{$type}_array";
    if (function_exists($func)) {
      $items[] = call_user_func($func, $file);
    }
  }
  $xml['Contents'] = $items;
  
  return $xml;
}

/**
 * Builds an Image file type array
 */
function _build_Image_array($file) {
  $image = file_load($file['fid']);
  $opts = $file['options']['Image'];
  $source = image_style_url($opts['style'], $image->uri);
  $item['#type'] = $file['type'];
  $item['#attributes']['Source'] = $source;
  if(!empty($file['options']['Title'])) {
    $item['#attributes']['Title'] = $file['options']['Title'];
  }
  if(!empty($opts['Text']['value'])) {
    $item['Text'] = check_markup($opts['Text']['value'], $opts['Text']['format']);
  }
  if(!empty($opts['Hyperlink'])) {
    $url = url($opts['Hyperlink'], array(
      'absolute' => TRUE,
      'external' => url_is_external($opts['Hyperlink']),
    )); 
    $item['Hyperlink'] = array(
      '#attributes' => array(
        'URL' => $url,
        'Target' => empty($opts['Target']) ? '_self' : $opts['Target'],
      ),  
    );
  }
  return $item;
}

/**
 * Builds a Video file type array
 */
function _build_Video_array($file) {
  $video = file_load($file['fid']);
  $opts = $file['options']['Video'];
  $item['#type'] = $file['type'];
  $item['#attributes']['Source'] = $video->uri;
  if(!empty($file['options']['Title'])) {
    $item['#attributes']['Title'] = $file['options']['Title'];
  }
  $item['#attributes']['Width'] = $opts['Width'];
  $item['#attributes']['Height'] = $opts['Height'];
  $item['#attributes']['Autoplay'] = $opts['Autoplay'];
  if(!empty($file['options']['fvimage']['fid'])) {
    $image = file_load($file['options']['fvimage']['fid']);
    $item['Image'] = array(
      '#attributes' => array(
        'Source' => $image->uri,
      ),
    );
  }
  return $item;
}

/**
 * Builds a Flash file type array
 */
function _build_Flash_array($file) {
  $flash = file_load($file['fid']);
  $item['#type'] = $file['type'];
  $item['#attributes']['Source'] = $flash->uri;
  if(!empty($file['options']['Title'])) {
    $item['#attributes']['Title'] = $file['options']['Title'];
  }
  if(!empty($file['options']['fvimage']['fid'])) {
    $image = file_load($file['options']['fvimage']['fid']);
    $item['Image'] = array(
      '#attributes' => array(
        'Source' => $image->uri,
      ),
    );
  }
  return $item;
}