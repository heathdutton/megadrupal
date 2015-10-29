<?php

/**
 * @file
 * Views functionality for webcams module.
 */

/**
 * Implementation of hook_views_handlers().
 */
function webcams_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'webcams') . '/handlers',
    ),
    'handlers' => array(
      'webcams_handler_field' => array(
        'parent' => 'views_handler_field_node',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function webcams_views_data() {
  $data = array();
  
  $data['node']['webcams_nid'] = array(
    'real field' => 'nid',
    'group' => t('Webcams'),
    'title' => t('Display Webcams'),
    'help' => t('Display webcam images. Use size to override the default webcam sizes. <em>i.e. 320x240</em>.'),
    'field' => array(
      'handler' => 'webcams_handler_field',
    ),
  );
  
  return $data;
}

/**
 * Views handler for displaying the image.
 */
function webcams_views_handler_single($fieldinfo, $fielddata, $value, $data) {
  $node = node_load($data->nid);

  $webcam = $node->webcams[0];
  if (empty($webcam)) {
    return '';
  }
  
  if (!empty($fielddata['options'])) {
    $options = explode('x', $fielddata['options'], 2);
    if (!is_numeric($options[0]) || !is_numeric($options[1])) {
      unset($options);
    }
    else {
      $webcam->width = $options[0];
      $webcam->height = $options[1];
    }
  }

  $delay  = $webcam->delay * 1000;
  $errmsg = t('Webcam <em>@name</em> has timed out...', array('@name' => $webcam->name));

  $js = "\n$.webcam.init('webcams{$webcam->nid}-webcam{$webcam->wid}', '{$webcam->name}', '{$webcam->url}', '{$webcam->default_url}', {$webcam->width}, {$webcam->height}, {$delay}, {$webcam->timeout}, 'webcams{$webcam->nid}-content', '{$errmsg}');";
  
  $output  = theme('webcams_webcam', $webcam, $fielddata['options']);
  $output .= '<div id="webcams'. $data->nid .'-content" class="webcams-content"></div>'."\n";

  drupal_add_css(drupal_get_path('module', 'webcams') .'/webcams.css');
  drupal_add_js(drupal_get_path('module', 'webcams') .'/webcams.js');
  drupal_add_js($js, 'inline', 'footer', FALSE);
  
  return $output;
}

/**
 * Views handler for displaying the image in a link to the the image node
 */
function webcams_views_handler_all($fieldinfo, $fielddata, $value, $data) {
  $js = '';
  $output = '';
  $node = node_load($data->nid);
  
  if (!empty($fielddata['options'])) {
    $options = explode('x', $fielddata['options'], 2);
    if (!is_numeric($options[0]) || !is_numeric($options[1])) {
      unset($options);
    }
  }

  for ($i = 0; $i < count($node->webcams); $i++) {
    $webcam = $node->webcams[$i];
    if (empty($webcam)) {
      continue;
    }
    
    if (isset($options[0]) && isset($options[1])) {
      $webcam->width = $options[0];
      $webcam->height = $options[1];
    }

    $delay  = $webcam->delay * 1000;
    $errmsg = t('Webcam <em>@name</em> has timed out...', array('@name' => $webcam->name));

    $js .= "\n$.webcam.init('webcams{$webcam->nid}-webcam{$webcam->wid}', '{$webcam->name}', '{$webcam->url}', '{$webcam->default_url}', {$webcam->width}, {$webcam->height}, {$delay}, {$webcam->timeout}, 'webcams{$webcam->nid}-content', '{$errmsg}');";
    
    $output .= theme('webcams_webcam', $webcam, $fielddata['options']);
  }
  $output .= '<div id="webcams'. $data->nid .'-content" class="webcams-content"></div>'."\n";

  drupal_add_css(drupal_get_path('module', 'webcams') .'/webcams.css');
  drupal_add_js(drupal_get_path('module', 'webcams') .'/webcams.js');
  drupal_add_js($js, 'inline', 'footer', FALSE);
  
  return $output;
}

/**
 * Views - Generate a list of all the valid sizes that are available
 */
function webcams_views_handler_filter_image_size($op) {
  foreach (_image_get_sizes() as $key => $size) {
    $a[$key] = $size['label'];
  }
  return $a;
}
