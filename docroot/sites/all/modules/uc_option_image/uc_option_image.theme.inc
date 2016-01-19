<?php
/**
 * @file
 * Theme functions for displaying option images
 */
function theme_uc_option_image_selected(&$variables) {
  $values = $variables['values'];
  $aid    = $variables['aid'];
  $pid    = $variables['pid'];

  $uc_attribute = uc_attribute_load($aid);
  //dpm($uc_attribute, 'attribute');
  if (!$uc_attribute) {
    return "";
  }

  if (empty($values['fid']) || empty($values['fid']['value'])) {
    return '';
  }

  if (isset($values['selected_style']) && isset($values['selected_style']['value']) 
      && (!empty($values['selected_style']['value']))) {
    $stylename = $values['selected_style']['value'];
  } else {
    return '';
  }

  drupal_add_js(drupal_get_path('module', 'uc_option_image') . '/uc_option_image.js');

  $options = array();
  foreach ($uc_attribute->options as $oid => $option) {
    $values = uc_option_image_values('uc_product', array('aid' => $aid, 'pid' => $pid, 'oid' => $oid));
    if (isset($values['fid']) && !empty($values['fid']['value'])) {
      $file = file_load($values['fid']['value']);
      $img  = theme('image_style', array('style_name' => $stylename, 'path' => $file->uri));
      $options[$oid] = $img;
    }

  }
  drupal_add_js(array('uc_option_image-' . $aid => $options), 'setting');

  return "
    <div id='uc-option-image-selected-$aid' class='uc-option-image uc-option-image-selected'></div>
  ";
}

function theme_uc_option_image_inline(&$variables) {
  $values = $variables['values'];

  if (empty($values['fid']) || empty($values['fid']['value'])) {
    return '';
  }

  if (isset($values['inline_style']) && isset($values['inline_style']['value']) 
      && (!empty($values['inline_style']['value']))) {
    $stylename = $values['inline_style']['value'];
  } else {
    $stylename = 'medium';
  }

  $file = file_load($values['fid']['value']);
  if (!$file) {
    return '';
  }

  return "
    <span class='uc-option-image uc-option-image-inline'>
      " . theme('image_style', array('style_name' => $stylename, 'path' => $file->uri)) . "
    </span>
  ";
}
