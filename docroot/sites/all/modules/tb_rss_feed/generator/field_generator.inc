<?php
function tb_rss_feed_text_generate($field_value, $object, $field, $instance, $bundle) {
  $object_field = array();
  $summary = $field_value = strip_tags($field_value);  
  $summary = strip_tags($field_value);
  if(strlen($summary) > 200){
    $summary = substr($summary, 0, 200);
    $summary = substr($summary, 0, strrpos($summary,' '));    
  }    
  $object_field['summary'] = $summary.'...';
  $object_field['value'] = $field_value;
  $object_field['format'] = 'full_html';
  return $object_field;
}

function tb_rss_feed_image_generate($url, $object, $field, $instance, $bundle) {
  $info = pathinfo($url);
  $filename = 'tb_rss_feed_img_'.time();
  $extension = isset($info['extension']) ? strtolower($info['extension']) : 'png';
  if(!in_array($extension, array('jpg, jpeg, png, gif'))) $extension = 'png';

  $object_field = array();
  $path = tb_rss_feed_download_image($url);
  if ($path) {
    $source = new stdClass();
    $source->uri = $path;
    $source->uid = 1;
    $source->filemime = 'image/' . pathinfo($path, PATHINFO_EXTENSION);
    $destination_dir = $field['settings']['uri_scheme'] . '://' . $instance['settings']['file_directory'];
    file_prepare_directory($destination_dir, FILE_CREATE_DIRECTORY);
    $counter = 0;
    $destination = $destination_dir . '/' . $filename . "." . $extension;
    $realpath = drupal_realpath($destination); 
    while (file_exists($realpath)) {
      $counter ++;
      $destination = $destination_dir . '/' . $filename . ($counter ? "_" . $counter : "") . "." . $extension;
      $realpath = drupal_realpath($destination); 
    }
    $source->filename = $filename . ($counter ? "_" . $counter : "") . "." . $extension;
    $file = file_move($source, $destination, FILE_CREATE_DIRECTORY);
  }
  else {
    return NULL;
  }

  $object_field['fid'] = $file->fid;
  $object_field['alt'] = '';
  $object_field['title'] = '';
  return $object_field;  
}

function tb_rss_feed_download_image($url) {
  $info = pathinfo($url);
  $extension = isset($info['extension']) ? $info['extension'] : 'png';
  $tmp_file = drupal_tempnam('temporary://', 'imagefield_');
  if ($tmp_file) {
    $content = crawlPage($url);
    if ($content) {
        $destination = $tmp_file . '.' . $extension;
        $path = file_unmanaged_move($tmp_file, $destination, FILE_CREATE_DIRECTORY);
        if ($path) {
            file_put_contents(drupal_realpath($path), $content);
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
    
  }
  return $destination;
}

function tb_rss_feed_taxonomy_generate($field_value, $object, $field, $instance, $bundle) {
  $field_value = trim($field_value);
  $tid = db_query('SELECT tid FROM {taxonomy_term_data} WHERE name = :name', array(':name' => $field_value))->fetchField();
  if ($tid === FALSE) {
    $vocabulary = taxonomy_vocabulary_load(variable_get('rss_feed_types', 0));
    $term_info = array(
      'name' => $field_value,
      'description' => $field_value,
      'vid' => $vocabulary->vid,
    );
    $term_object = (object) $term_info;
    taxonomy_term_save($term_object);
    $tid = $term_object->tid;
  }
  $object_field = array();
  $object_field['tid'] = (int) $tid;
  return $object_field;
}

function tb_rss_feed_file_generate($field_value, $object, $field, $instance, $bundle) {
  return array("");
}

function tb_rss_feed_number_generate($field_value, $object, $field, $instance, $bundle) {
  return array(is_numeric($field_value) ? $field_value : 0);
}

function tb_rss_feed_date_generate($field_value, $object, $field, $instance, $bundle) {
  $time = strtotime($field_value);
  $str = date('Y-m-d H:i:s', $time); 
  return array('value' => $str);
}

function tb_rss_feed_link_generate($field_value, $object, $field, $instance, $bundle){
  $object_field = array();
  $object_field['url'] = $field_value;
  $object_field['title'] = '';
  $object_field['attributes'] = array();
  return $object_field;
}


function tb_rss_feed_youtube_generate($field_value, $object, $field, $instance, $bundle){
  $object_field = array();
  $object_field['input'] = $field_value;
  $data = explode('=', $field_value);
  $object_field['video_id'] = $data[count($data)-1];
  return $object_field;
}

function crawlPage($url) {
    if (function_exists('curl_init')) {
        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, $url);
        curl_setopt($curl, CURLOPT_HEADER, 0);
        curl_setopt($curl, CURLOPT_TIMEOUT, 600);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($curl, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.5) Gecko/20041107 Firefox/1.0');
        $content = curl_exec($curl);
        curl_close($curl);
    } else {
        $content = @file_get_contents($url);
    }
    return $content;
}