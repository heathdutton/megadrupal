<?php

/*
 * Soundcloud flash recorder
 */
function audiorecorderfield_recorder_soundcloud($element, $widget = NULL) {
  return theme('audiorecorderfield_soundcloud', array('element' => $element)); 
}

function theme_audiorecorderfield_soundcloud($variables) {
  $element = $variables['element'];
    $field_id = audiorecorderfield_str_replace_once('field_', '', $element['#field_name']);
    //Change "_" into "-"
    $field_id=str_replace('_', '-', $field_id);
    $langcode = $element['#language'];
    $delta = $element['#parents'][2];
    $field_name = $field_id."-". $langcode."-".$delta;
    //$file_id = $element['#field_name'].'['.$langcode.']['.$delta.'][fid]';
    
    $op = NULL;
    drupal_add_css(drupal_get_path('module', 'audiorecorderfield') . '/css/audiorecorderfield.css');
    drupal_add_js(array('soundcloud_recorder' => array(base_path().drupal_get_path('module', 'audiorecorderfield').'/recorders/recorder.swf')), 'setting');
    drupal_add_js(drupal_get_path('module', 'audiorecorderfield') . '/js/recorder.js');   
    drupal_add_js(drupal_get_path('module', 'audiorecorderfield') . '/js/audiorecorder_soundcloud.js');   
    
    if (!empty($element['#value']['uri'])) {
      $audiofile = file_create_url($element['#value']['uri']);
      $info = pathinfo($audiofile);
      $button_op='Remove';
      $class ='audiorecorderfield-remove';
       
      $op = $info['extension'];
      $output = '<div id="soundcloud-wrapper">' . audiofield_get_player($audiofile, $op) . '</div>';
    }
    else {
        $button_op='Upload';
        $class ='audiorecorderfield-upload';
        
        $output = theme('audiorecorderfield_soundcloud_skin', array('field_name' => $field_name));
    }
    $output.='<div><input type="button" onclick="javascript:upload(\'' . $field_id . '\',\'' . $langcode . '\',\'' . $delta . '\',\'' .$element['#field_name'] . '\')"  
        id="soundcloud-button-' . $field_name  . '" class="form-submit '.$class.'" value="'.$button_op.'"/></div>';
    //$output.='<input type="hidden" value="0" name="'.$file_id.'">';    
    return $output;
}

/*Skin for Soundcloud Flash recorder. Override to implement different skin*/
function theme_audiorecorderfield_soundcloud_skin($variables) {
  $field_name = $variables['field_name'];

  //Add css
  drupal_add_css(drupal_get_path('module', 'audiorecorderfield') . '/css/audiorecorderfield.css');
  $output = '<div id="soundcloud-' . $field_name . '-wrapper">
        <div>
          <span onclick="record(\'' . $field_name .'\')" class="soundcloud-record">Record</span>
          <span onclick="stop(\'' . $field_name . '\')" class="soundcloud-stop" style="display:none;">Stop</span>
          <span onclick="play(\'' . $field_name . '\')" style="display:none;" class="soundcloud-play">Play</span>
          <span onclick="pause(\'' . $field_name . '\')" style="display:none;" class="soundcloud-pause">Pause</span>
        </div>
        <span id="time-' . $field_name  . '">0:00</span>
        <a class="record-again-btn" style="display:none;" href="javascript:record_again(\'' . $field_name . '\')">Start over</a>
        </div>
        ';
  return $output;      
}

/*
 * Soundcloud recorder callback. This is what happens when 
 * 'upload' is pressed in the flash.
 */
function audiorecorderfield_soundcloud_file_receive() {
  $upload_dir_stream = file_default_scheme() . '://' ;
  // TODO: get file extension from real file type
  $new_name = time() . '.wav';
  $_FILES['files'] = $_FILES['track'];
  $_FILES['files']['name'] = $new_name;

  foreach ($_FILES['files'] as $key => $value) {
    if ($key == 'tmp_name') {
      $_FILES['files'][$key] = array(0 => $value['asset_data']);
    }
    else if($key !='error') {
      $_FILES['files'][$key] = array(0 => $value);
    }
  }
  $_FILES['files']['error'][0] = UPLOAD_ERR_OK;
  $validators['file_validate_extensions'] = array('wav');
  if (!$file = file_save_upload(0, $validators,  $upload_dir_stream)) {
    watchdog('audiofield_soundcloud', "ERROR - file_save_upload failed");
    $fid = "NULL";
  } 
  else {
    $fid = $file->fid;
  }
  
  //Return fid to javascript
  print("$fid"); 
}