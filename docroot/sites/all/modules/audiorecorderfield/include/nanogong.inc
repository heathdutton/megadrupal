<?php 

/*
 * NanoGong recorder applet
 */
function audiorecorderfield_recorder_nanogong($element, $widget=NULL) {
    
    // The widget is being presented, so apply the JavaScript.
    drupal_add_js(drupal_get_path('module', 'audiorecorderfield') . '/js/deployJava.js');
    drupal_add_js(drupal_get_path('module', 'audiorecorderfield') . '/js/audiorecorder_nanogong.js');

    $field_id = audiorecorderfield_str_replace_once('field_', '', $element['#field_name']);
    //Change "_" into "-"
    $field_id=str_replace('_', '-', $field_id);
    $langcode = $element['#language'];
    $delta = $element['#parents'][2];
    $field_name = $field_id."-". $langcode."-".$delta;
    //$file_id = $element['#field_name'].'['.$langcode.']['.$delta.'][fid]';

    
    //TODO: Load options here (width, height,MaxDuration...)
    /*$width = isset($widget['player_width'])?$widget['recorder_width']:300;
    $height = isset($widget['player_height'])? $widget['recorder_height']:100;
    */
    //Nanogong configuration http://gong.ust.hk/nanogong/info_config.html
    $config = '<param name="ShowSaveButton" value="false" />'; //Disable Save button
    $config .= '<param name="ShowTime" value="true" /> '; //Show Time
    $config .= '<param name="ShowSpeedButton" value="false" /> '; //Disable speed button
    $config .= '<param name="MaxDuration" value="1200" /> '; //Set MaxDuration to maximum (20 minutes) 
    $config .= '<param name="AudioFormat" value="ImaADPCM" />'; //Set AudioFormat to Wav
    
    if (!empty($element['#value']['uri'])) {
      $audiofile = file_create_url($element['#value']['uri']);
        $info = pathinfo($audiofile);
        $button_op='Remove';
        $class ='audiorecorderfield-remove';
        //$class_suffix='player';
        $op = $info['extension'];
        $output = '<div id="nanogong-' . $field_name . '-wrapper">' . audiofield_get_player($audiofile, $op) . '</div>';
    }
    else {
        $button_op='Upload';
        $class ='audiorecorderfield-upload';
        $class_suffix='recorder';
        $width='113px';
         //Add css
        drupal_add_css(drupal_get_path('module', 'audiorecorderfield') . '/css/audiorecorderfield.css');
        //Nanogong Applet
        $output = '<div class="nanogong-' . $class_suffix . '" id="nanogong-' . $field_name . '-wrapper">
        <applet id="nanogong-' .$field_name . '" archive="' . base_path() . drupal_get_path('module', 'audiorecorderfield') . '/recorders/nanogong.jar" 
            code="gong.NanoGong" width="' . $width . '" height="40">
            ' . $config . '
            </applet></div>';
            
        //Java fallback
        $id = 'nanogong-' . $field_name;
        $fallback = audiorecorderfield_nanogong_java_fallback($id);    
        $output.='<div class="nanogong-fallback" style="display:none;">'.$fallback.'</div>';    
    }

    //Upload/Remove button
    $output .= '<div><input id="nanogong-button-' . $field_name . '" class="form-submit '.$class.'" 
    type="button" value="' . $button_op . '" onclick="submitVoice(\'' . $field_id . '\',\'' . $langcode . '\',\'' . $delta . '\',\'' .$element['#field_name'] . '\')" /></div>';
    //$output.='<input type="hidden" value="0" name="'.$file_id.'">';
    return $output;
}

/*
 * Nanogong recorder applet callback. This is what happens when 
 * 'upload' is pressed in the applet
 */
function audiorecorderfield_nanogong_file_receive() {
  header("Cache-control: private");
  header("Content-Type: text/plain");
 
  //TODO save duration in field
  //$duration = strip_tags($_POST['duration']);
  $upload_dir_stream = file_default_scheme() . '://' ;
    
  if ($_FILES['voicefile']['error']>0) {
    watchdog('audiorecorder_field', "ERROR - error code: " . $_FILES['voicefile']['error']);
    $fid = "NULL";
  }
  else {
    $_FILES['files'] = $_FILES['voicefile'];
    $_FILES['files']['name'] = str_replace('temp', time(), $_FILES['files']['name']);
    foreach ($_FILES['files'] as $key => $value) {
      $_FILES['files'][$key] = array(0 => $value);
    }
    $validators['file_validate_extensions']=array('wav');
    if (!$file = file_save_upload(0, $validators, $upload_dir_stream)) {
      watchdog('audiorecorder_field', "ERROR - file_save_upload failed", array(), WATCHDOG_ERROR);
      $fid = "NULL";
    } 
    else {
      $fid = $file->fid;
    }
  }
  //Return fid to javascript
  print("$fid"); 
}

function audiorecorderfield_nanogong_player($player_path, $audio_file, $id="nanogong") {
    //Add css
    drupal_add_css(drupal_get_path('module', 'audiorecorderfield') . '/css/audiorecorderfield.css');
    $config = '<param name="ShowSaveButton" value="false" />'; //Disable Save button
    $config .= '<param name="ShowTime" value="true" /> '; //Show Time
    $config .= '<param name="ShowRecordButton" value="false" /> '; //Disable Record button
    $config .= '<param name="ShowSpeedButton" value="false" /> '; //Disable speed button
    $output='<div class="nanogong-player" id="' . $id . '-wrapper">
    <applet id="' . $id . '" archive="' . $player_path . '" code="gong.NanoGong" width="82" height="40">  
            ' . $config . '<param name="SoundFileURL" value="' . $audio_file . '"/>
    </applet></div>';
    
    //Java fallback
    $fallback = audiorecorderfield_nanogong_java_fallback($id);    
    $output.='<div class="nanogong-fallback" style="display:none;">'.$fallback.'</div>';    

    return $output;
}

function audiorecorderfield_nanogong_java_fallback($id) {
  $op = variable_get('audiorecordefield_java_fallback', 0);
  if ($op == 'upload') {
    $fallback = '
    <label for="edit-upload-'.$id.'">Click to Upload a recording: </label>
    <input type="file" size="15" id="edit-upload-'.$id.'" class="form-file" name="files[upload-'.$id.']">

    ';
  }
  else {
    $fallback = t('In order to use Nanogong Recorder you must download and install !java', 
                array('!java' => '<a href="http://java.com/en/download/installed.jsp" target="_blank">Java Plugin</a>'));
  }
  
  return $fallback;
}