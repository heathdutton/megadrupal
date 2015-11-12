<?php
/**
 * @file
 * particle_emitter.config_images.inc.
 */

/**
 * Implemets hook_form().
 */
function particle_emitter_images_form($form, &$form_state) {

  $form['#attached']['css'][drupal_get_path('module', 'particle_emitter') . '/css/particle_emitter.css'] = array();

  $form['#tree'] = TRUE;
  $form['particle_emitter_images_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image Management'),
    '#prefix' => '<div id="images-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  $kh_data_array = variable_get('particle_emitter_images', '');

  $data = array();
  if (!empty($kh_data_array)) {
    $data = $kh_data_array;
  }
  if (!isset($form_state['image_count']) && !empty($kh_data_array)) {
    $form_state['image_count'] = count($data);
  }

  if (empty($form_state['image_count'])) {
    $form_state['image_count'] = 1;
  }
  $_data = array_values($data);
  $data = $_data;

  $form['particle_emitter_images_fieldset']['description1'] = array(
    '#markup' => '<div>' . t('In this section you can manage your custom images. To add a new custom image click the "Add new image" button. To activate the images go to image configuration') . '</div>',
  );

  $form['particle_emitter_images_fieldset']['particle_emitter_description_br'] = array(
    '#markup' => '<br clear="all" />',
  );

  $form['particle_emitter_images_fieldset']['particle_emitter_add_image'] = array(
    '#type' => 'submit',
    '#value' => 'Add new image',
    '#title' => t('Add new image'),
    '#submit' => array('particle_emitter_images_form_add_image'),
    '#ajax' => array(
      'callback' => 'particle_emitter_images_form_callback',
      'wrapper' => 'images-fieldset-wrapper',
    ),
  );

  $form['particle_emitter_images_fieldset']['description2'] = array(
    '#markup' => '<div>' . t('Overview of custom images avaiable:') . '</div>',
  );

  for ($i = 0; $i < $form_state['image_count']; $i++) {

    $particle_emitter_item_link = (isset($data[$i]['link']))?$data[$i]['link'] : '';
    
    if ((isset($data[$i]['file']['file_managed_container']['file_managed_element'])) AND
       ($data[$i]['file']['file_managed_container']['file_managed_element'] != 0)) {
       
      $particle_emitter_item_file = $data[$i]['file']['file_managed_container']['file_managed_element'];
    }
    else {
      $particle_emitter_item_file = 0;
    }

    $form['particle_emitter_images_fieldset'][$i] = array(
      '#prefix' => /*($i == 0) ? $header : */'<div class="particle-emitter-images-image particle-emitter-images-image-' . $i . '">',
      '#suffix' => '</div>',
    );

    $form['particle_emitter_images_fieldset'][$i]['file'] = array(
      'file_managed_container' => array(
        'file_managed_element' => array(
          '#title' => t('Image'),
          '#title_display' => 'invisible',
          '#type' => 'managed_file',
          '#upload_location' => 'public://particle_emitter/',
          '#default_value' => $particle_emitter_item_file,
          '#upload_validators' => array(
            'file_validate_extensions' => array('gif png jpg jpeg'),
            'file_validate_size' => array(1000000),
          ),
          '#description' => t('Image'),
        ),
      ),
    );

    $form['particle_emitter_images_fieldset'][$i]['link'] = array(
      '#type' => 'textfield',
      '#title' => t('Link'),
      '#title_display' => 'invisible',
      '#description' => t('Image link'),
      '#default_value' => $particle_emitter_item_link,
      '#size' => 40,
      '#maxlength' => 120,
      '#required' => FALSE,
    );

    $form['particle_emitter_images_fieldset'][$i]['particle_emitter_image_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove this custom image'),
    );

    $form['particle_emitter_images_fieldset'][$i]['particle_emitter_br'] = array(
      '#markup' => '<br clear="all" />',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function particle_emitter_images_form_add_image($form, &$form_state) {
  $form_state['image_count']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset including all items.
 */
function particle_emitter_images_form_callback($form, $form_state) {
  return $form['particle_emitter_images_fieldset'];
}

/**
 * Delete managed file.
 */
function particle_emitter_delete_managed_file($fileid) {
  $file =  file_load($fileid) ;
  if ($file) {
    file_usage_delete($file, 'particle_emitter', 'particle_emitter', 1);
    file_delete($file);
    return $fileid;
  }
  return false;
}

/**
 * Save managed file.
 */
function particle_emitter_save_managed_file($fileid) {
  $file = file_load($fileid);
  if ($file->status==0) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'particle_emitter', 'particle_emitter', 1);
    return $fileid;
  }
  return false;
}

/**
 * Final submit handler.
 *
 * Reports what values were finally set.
 */
function particle_emitter_images_form_submit($form, &$form_state) {

  $submittedvars  = array();
  $savedvariables = array();
  $savedvariablestmp = variable_get('particle_emitter_images');

  if (isset($form_state['values']['particle_emitter_images_fieldset']['particle_emitter_add_image'])) {
      unset($form_state['values']['particle_emitter_images_fieldset']['particle_emitter_add_image']);
  }

  if (isset($form_state['values']['particle_emitter_images_fieldset']['particle_emitter_image_delete'])) {
      unset($form_state['values']['particle_emitter_images_fieldset']['particle_emitter_image_delete']);
  }

  for ($i = count($form_state['values']['particle_emitter_images_fieldset'])-1; 0 <= $i; $i--) {

    $submittedvars['particle_emitter_image_delete']    = $form_state['values']['particle_emitter_images_fieldset'][$i]['particle_emitter_image_delete'];
    $submittedvars['link'] = $form_state['values']['particle_emitter_images_fieldset'][$i]['link'];
    $submittedvars['file']      = $form_state['values']['particle_emitter_images_fieldset'][$i]['file']['file_managed_container']['file_managed_element'];

    $savedvariables['particle_emitter_image_delete']    = (isset($savedvariablestmp[$i]['particle_emitter_image_delete'])) ? $savedvariablestmp[$i]['particle_emitter_image_delete'] : '';
    $savedvariables['link'] = (isset($savedvariablestmp[$i]['link'])) ? $savedvariablestmp[$i]['link'] : '';
    $savedvariables['file']      = (isset($savedvariablestmp[$i]['file']['file_managed_container']['file_managed_element'])) ? $savedvariablestmp[$i]['file']['file_managed_container']['file_managed_element'] : '';

    if($submittedvars['particle_emitter_image_delete'] == 1) {
      if($submittedvars['file'] != 0) {
        if($returnvalue = particle_emitter_delete_managed_file($submittedvars['file'])) {
          drupal_set_message("Particle Emitter managed file ".$returnvalue[0]." successfully deleted. (320)");
          $form_state['values']['particle_emitter_images_fieldset'][$i]['file']['file_managed_container']['file_managed_element'] = 0 ;
        }
        else
          drupal_set_message("Particle Emitter managed file ".$submittedvars['file']." could not be deleted.(326)", $type = 'error');
      }
      unset($form_state['values']['particle_emitter_images_fieldset'][$i]);
      drupal_set_message("Particle Emitter item ".$i." successfully deleted. (330)");
    }
    elseif($submittedvars['particle_emitter_image_delete'] == 0) {
      if($submittedvars['file'] != 0){
        if($returnvalue = particle_emitter_save_managed_file($submittedvars['file'])) {
          drupal_set_message("Particle Emitter managed file ".$returnvalue." successfully saved.");
          $form_state['values']['particle_emitter_images_fieldset'][$i]['file']['file_managed_container']['file_managed_element'] = $returnvalue;
        }
      }
      else if($savedvariables['file'] != 0 && $submittedvars['file'] == 0) {
        if($returnvalue = particle_emitter_delete_managed_file($savedvariables['file'])) {
          drupal_set_message("Particle Emitter managed file ".$savedvariables['file']." successfully deleted. (350)");
          $form_state['values']['particle_emitter_images_fieldset'][$i]['file']['file_managed_container']['file_managed_element'] = 0;
        }
      }
    }
  }
  variable_set("particle_emitter_images", array_values($form_state['values']['particle_emitter_images_fieldset']));
  drupal_set_message("All image sets successfully saved.");
  cache_clear_all();
}

/**
 * Implements hook_form_validate().
 */
function particle_emitter_images_form_validate($form, &$form_state) {
  $_data = $form_state['values']['particle_emitter_images_fieldset'];
  unset($_data['particle_emitter_add_image']);
  $data = array_values($_data);

  foreach ($data as $k => $v) {
    if (!empty($v['link'])) {
      if (!preg_match(
        '/^(http|https)\:\/\/[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(:[a-zA-Z0-9]*)?\/?([a-zA-Z0-9\-\._\?\,\'\/\\\+&amp;%\$#\=~])*$/', $v['link'])) {
        form_set_error('url', t('Please provide a valid URL. Example: http://www.example.org'));
      }
    }
  }
}
