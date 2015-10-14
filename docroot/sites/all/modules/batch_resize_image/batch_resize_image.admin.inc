<?php

function batch_resize_image_confirm_form() {
  $form['threshold_size'] = array(
      '#type' => 'textfield', 
      '#title' => t('Threshold size'), 
      '#description' => t('The file which size is over this bytes will check to resize.'),
      '#default_value' => 2000000, 
      '#size' => 20, 
      '#maxlength' => 15, 
      '#required' => TRUE, 
    );
  $form['size_limit'] = array(
      '#type' => 'textfield', 
      '#title' => t('Maximum width or height'), 
      '#description' => t('The file which is check to resize will fit the width or height to this number.'),
      '#default_value' => 2048, 
      '#size' => 20, 
      '#maxlength' => 6, 
      '#required' => TRUE, 
    );
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Transform'),
    '#submit' => array('batch_resize_image_confirm_form_submit'),
  );

  return $form;
}

function batch_resize_image_confirm_form_submit($form, &$form_state) {
  $batch = array(
    'title' => t('Processing resize image Batch'),
    'operations' => array(
       array('batch_resize_image_process_batch', array($form['size_limit']['#value'], $form['threshold_size']['#value'])),
    ),
    'finished' => 'batch_resize_image_process_batch_finished',
    'init_message' => t('Resize image Batch is starting.'),
    'file' => drupal_get_path('module', 'batch_resize_image') . '/batch_resize_image.admin.inc',
  );
  batch_set($batch);

}



function batch_resize_image_process_batch($size_limit, $threshold_size, &$context) {



  $db_query = db_query("SELECT * FROM {file_managed} ");
  $files = $db_query->fetchAll();
  if (empty($context['sandbox'])) {
    // setup progress
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($files);
  }

  $limit = 5;
  $db_query_step = db_query_range("SELECT * FROM {file_managed}", $context['sandbox']['progress'], 5);
  $files = $db_query_step->fetchAll();
  // $size_limit = 2048;
  // $i = 0;
  foreach($files as $key => $file){
    $result = array();
    $result['file'] = $file;
    $context['message'] = t('Now processing the files %progress of %max', array(
      '%progress' => $context['sandbox']['progress'], 
      '%max' => $context['sandbox']['max']+1,
      )
    );
    $context['sandbox']['progress']++;


    if(substr($file->filemime,0,5) == 'image'){
      $image = image_load($file->uri);
      $result['image'] = $image;
      if($image->info['file_size'] > $threshold_size && ($image->info['width'] > $size_limit || $image->info['height'] > $size_limit)){
          if($image->info['width'] > $image->info['height']){
            $width = $size_limit;
            $height = $image->info['height'] * $width / $image->info['width'];
          }else{
            $height = $size_limit;
            $width = $image->info['width'] * $height / $image->info['height'];
          }
        image_resize($image,$width,$height);
        image_save($image);
        // $i ++ ;
        $result['message'] = '"' . $file->filename . "\" has been transformed.";
        $context['results']['resized'][] = $result;
      }else{
        $result['message'] = '"' . $file->source . "\" doesn't need to be transformed.";
        $context['results']['not_resized'][] = $result;
      }
    }else{
      $result['message'] = $file->source . " isn't an image file.";
      $context['results']['not_resized'][] = $result;
    }
    $context['results']['all'][] = $result;
  }

  $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  
}

function batch_resize_image_process_batch_finished($success, $results, $operations) {
  if($success){
    watchdog('batch_resize_image', 'Batch success');
    drupal_set_message(t('Success'));
    // dpm($results);
  }
  else{
    watchdog('debug', 'Batch error');
  }
}
