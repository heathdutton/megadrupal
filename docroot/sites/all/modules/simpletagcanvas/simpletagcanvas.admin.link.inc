<?php

/**
 * @file
 * SimpleTagCanvas backend link konfiguration and file upload
 */

define('SIMPLETAGCANVAS_DEFAULT_LINKTEXT', 'LINK');
define('SIMPLETAGCANVAS_DEFAULT_LINKTARGET', 'http://drupal.org');
define('SIMPLETAGCANVAS_DEFAULT_LINKCOLOR', 'blue');
define('SIMPLETAGCANVAS_DEFAULT_IMAGE', 'false');
define('SIMPLETAGCANVAS_DEFAULT_IMAGESTYLE', '40x40');

/**
 * Implements hook_form().
 */
function simpletagcanvas_link_form($form, &$form_state) {

  $form['#attached']['css'][drupal_get_path('module', 'simpletagcanvas') . '/css/simpletagcanvas.admin.css'] = array();

  $form['description'] = array(
    '#markup' => '<div>' . t('Add the linktext and there relevant url here.') . '</div>',
  );

  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  $form['#tree'] = TRUE;
  $form['names_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('link management'),
    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="names-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  // Get data from data base and build the number of fields.
  $kh_data_array = variable_get('simpletagcanvas_storagearray', '');

  if (!empty($kh_data_array)) {
    $data = $kh_data_array;
  }
  if (!isset($form_state['num_names']) && !empty($kh_data_array)) {
    $form_state['num_names'] = count($data);
  }

  // Build the fieldset with the proper number of names.
  // $form_state['num_names'] to determine the number of textfields to build.
  if (empty($form_state['num_names'])) {
    $form_state['num_names'] = 1;
  }

  for ($i = 0; $i < $form_state['num_names']; $i++) {

    // If data is there - get the data.
    $simpletagcanvas_item_linktext   = (isset($data[$i]['linktext'])) ? $data[$i]['linktext'] : SIMPLETAGCANVAS_DEFAULT_LINKTEXT . $i;
    $simpletagcanvas_item_url        = (isset($data[$i]['url'])) ? $data[$i]['url'] : SIMPLETAGCANVAS_DEFAULT_LINKTARGET;
    $simpletagcanvas_item_linkcolor  = (isset($data[$i]['linkcolor'])) ? $data[$i]['linkcolor'] : SIMPLETAGCANVAS_DEFAULT_LINKCOLOR;
    $simpletagcanvas_item_imagestyle = (isset($data[$i]['imagestyle'])) ? $data[$i]['imagestyle'] : SIMPLETAGCANVAS_DEFAULT_IMAGESTYLE;

    if ((isset($data[$i]['file']['file_managed_container']['file_managed_element'])) AND
       ($data[$i]['file']['file_managed_container']['file_managed_element'] != 0)) {

      $simpletagcanvas_item_file = $data[$i]['file']['file_managed_container']['file_managed_element'];
    }
    else {
      $simpletagcanvas_item_file = 0;
    }

    $header = '<div class="two-col tch">
               <div class="colhead1 ch"></div>
               <div class="colhead2 ch"></div>
               <div class="colhead3 ch"></div>
               <div class="colhead4 ch"></div>
               <div class="colhead5 ch"></div>
               <div class="colhead6 ch"></div>
               <div class="clear"></div>
               </div>
               <div class="two-col">';

    $form['names_fieldset'][$i] = array(
      '#prefix' => ($i == 0) ? $header : '<div class="two-col">',
      '#suffix' => '</div>',
    );

    $form['names_fieldset'][$i]['select'] = array(
      '#type' => 'checkbox',
      '#title' => t('del'),
      '#prefix' => '<div class="col1">',
      '#suffix' => '</div>',
    );

    $form['names_fieldset'][$i]['linktext'] = array(
      '#type' => 'textfield',
      '#required' => FALSE,
      '#size' => 30,
      '#maxlength' => 50,
      '#title' => t('Linktext'),
      '#title_display' => 'invisible',
      '#default_value' => $simpletagcanvas_item_linktext,
      '#description' => t('linktext'),
      '#prefix' => '<div class="col2">',
      '#suffix' => '</div>',
    );

    $form['names_fieldset'][$i]['url'] = array(
      '#type' => 'textfield',
      '#required' => FALSE,
      '#size' => 30,
      '#maxlength' => 150,
      '#title' => t('target url'),
      '#title_display' => 'invisible',
      '#default_value' => $simpletagcanvas_item_url,
      '#description' => t('target url'),
      '#prefix' => '<div class="col3">',
      '#suffix' => '</div>',
    );

    $form['names_fieldset'][$i]['linkcolor'] = array(
      '#type' => 'select',
      '#options' => array(
        'red'    => t('red'),
        'green'  => t('green'),
        'purple' => t('purple'),
        'blue'   => t('blue'),
        'black'  => t('black'),
        'white'  => t('white')),
      '#default_value' => $simpletagcanvas_item_linkcolor,
      '#description' => t('text color'),
      '#prefix' => '<div class="col4">',
      '#suffix' => '</div>',
    );

    $form['names_fieldset'][$i]['imagestyle'] = array(
      '#type' => 'select',
      '#options' => array(
        '10' => t('10x10'),
        '20' => t('20x20'),
        '30' => t('30x30'),
        '40' => t('40x40'),
        '50' => t('50x50'),
        '60' => t('60x60'),
        '70' => t('70x70'),
        '80' => t('80x80')),
      '#default_value' => $simpletagcanvas_item_imagestyle,
      '#description' => t('image preset'),
      '#prefix' => '<div class="col5">',
      '#suffix' => '</div>',
    );

    $form['names_fieldset'][$i]['file'] = array(
      'file_managed_container' => array(
        '#prefix' => '<div class="col6">',
        '#suffix' => '</div><div class="clear"></div>',
        'file_managed_element' => array(
          '#title_display' => 'invisible',
          '#type' => 'managed_file',
          '#upload_location' => 'public://simpletagcanvas/',
          '#default_value' => $simpletagcanvas_item_file,
          '#upload_validators' => array(
            'file_validate_extensions' => array('gif png jpg jpeg'),
             // Pass the maximum file size in bytes.
            'file_validate_size' => array(1000000),
          ),
          '#description' => t('Image ') . $i,
        ),
      ),
    );
  }

  $form['names_fieldset']['add_link'] = array(
    '#type' => 'submit',
    '#value' => t('Add one more'),
    '#title' => t('url'),
    '#submit' => array('simpletagcanvas_form_add_one'),
    '#ajax' => array(
      'callback' => 'simpletagcanvas_callback',
      'wrapper' => 'names-fieldset-wrapper',
    ),
  );

  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and delete selected ones'),
  );

  return $form;
}


/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function simpletagcanvas_form_add_one($form, &$form_state) {
  $form_state['num_names']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function simpletagcanvas_callback($form, $form_state) {
  return $form['names_fieldset'];
}

/**
 * Delete managed file.
 */
function simpletagcanvas_delete_managed_file($fileid) {
  $file = file_load($fileid);
  if ($file) {
    // When a module is managing a file, it must manage the usage count.
    // Here we decrement the usage count with file_usage_delete().
    file_usage_delete($file, 'simpletagcanvas', 'simpletagcanvas', 1);
    // The file_delete() function takes a file object and checks to see if
    // the file is being used by any other modules. If it is the delete
    // operation is cancelled, otherwise the file is deleted.
    file_delete($file);
    return $fileid;
  }
  return FALSE;
}

/**
 * Save managed file.
 */
function simpletagcanvas_save_managed_file($fileid) {
  $file = file_load($fileid);
  if ($file->status == 0) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);

    // When a module is managing a file, it must manage the usage count.
    // Here we increment the usage count with file_usage_add().
    file_usage_add($file, 'simpletagcanvas', 'simpletagcanvas', 1);
    return $fileid;
  }
  return FALSE;
}

/**
 * Implements hook_form_submit().
 */
function simpletagcanvas_link_form_submit($form, &$form_state) {
  $submittedvars  = array();
  $savedvariables = array();
  $savedvariablestmp = variable_get('simpletagcanvas_storagearray');

  if (isset($form_state['values']['names_fieldset']['add_link'])) {
    unset($form_state['values']['names_fieldset']['add_link']);
  }

  if (isset($form_state['values']['names_fieldset']['remove_link'])) {
    unset($form_state['values']['names_fieldset']['remove_link']);
  }

  for ($i = count($form_state['values']['names_fieldset']) - 1; 0 <= $i; $i--) {
    $submittedvars['select'] = $form_state['values']['names_fieldset'][$i]['select'];
    $submittedvars['linktext'] = $form_state['values']['names_fieldset'][$i]['linktext'];
    $submittedvars['url'] = $form_state['values']['names_fieldset'][$i]['url'];
    $submittedvars['linkcolor'] = $form_state['values']['names_fieldset'][$i]['linkcolor'];
    $submittedvars['imagestyle'] = $form_state['values']['names_fieldset'][$i]['imagestyle'];
    $submittedvars['file'] = $form_state['values']['names_fieldset'][$i]['file']['file_managed_container']['file_managed_element'];

    $savedvariables['select'] = (isset($savedvariablestmp[$i]['select'])) ? $savedvariablestmp[$i]['select'] : '';
    $savedvariables['linktext'] = (isset($savedvariablestmp[$i]['linktext'])) ? $savedvariablestmp[$i]['linktext'] : '';
    $savedvariables['url'] = (isset($savedvariablestmp[$i]['url'])) ? $savedvariablestmp[$i]['url'] : '';
    $savedvariables['linkcolor'] = (isset($savedvariablestmp[$i]['linkcolor'])) ? $savedvariablestmp[$i]['linkcolor'] : '';
    $savedvariables['imagestyle'] = (isset($savedvariablestmp[$i]['imagestyle'])) ? $savedvariablestmp[$i]['imagestyle'] : '';
    $savedvariables['file'] = (isset($savedvariablestmp[$i]['file']['file_managed_container']['file_managed_element'])) ? $savedvariablestmp[$i]['file']['file_managed_container']['file_managed_element'] : '';
    // Delete SimpleTagCanvas Item.
    if ($submittedvars['select'] == 1) {
      // Delete managed file if available.
      if ($submittedvars['file'] != 0) {
        if ($returnvalue = simpletagcanvas_delete_managed_file($submittedvars['file'])) {
          drupal_set_message(t('SimpleTagCanvas managed file @stm_managedfile was deleted.', array('@stm_managedfile' => $returnvalue[0])));
          $form_state['values']['names_fieldset'][$i]['file']['file_managed_container']['file_managed_element'] = 0;
        }
        else {
          drupal_set_message(t('SimpleTagCanvas managed file @stm_managedfile could not be deleted.', array('@stm_managedfile' => $submittedvars['file'])), 'error');
        }
      }

      unset($form_state['values']['names_fieldset'][$i]);
      drupal_set_message(t('SimpleTagCanvas managed file @stm_managedfile was deleted.', array('@stm_managedfile' => $i)));
    }
    elseif ($submittedvars['select'] == 0) {
      if ($submittedvars['file'] != 0) {
        if ($returnvalue = simpletagcanvas_save_managed_file($submittedvars['file'])) {
          drupal_set_message(t('SimpleTagCanvas managed file @stm_managedfile was saved.', array('@stm_managedfile' => $returnvalue)));
          $form_state['values']['names_fieldset'][$i]['file']['file_managed_container']['file_managed_element'] = $returnvalue;
        }
      }
      // If there is a old managed file which is not longer needed,
      // delete it and remove it from $form_state
      elseif ($savedvariables['file'] != 0 && $submittedvars['file'] == 0) {
        if ($returnvalue = simpletagcanvas_delete_managed_file($savedvariables['file'])) {
          drupal_set_message(t('SimpleTagCanvas managed file @stm_managedfile was deleted.', array('@stm_managedfile' => $savedvariables['file'])));
          $form_state['values']['names_fieldset'][$i]['file']['file_managed_container']['file_managed_element'] = 0;
        }
      }
    }
  }

  variable_set("simpletagcanvas_storagearray", array_values($form_state['values']['names_fieldset']));
  cache_clear_all();
}

/**
 * Implements hook_form_validate().
 */
function simpletagcanvas_link_form_validate($form, &$form_state) {
  if (isset($form_state['values']['names_fieldset']['add_link'])) {
    unset($form_state['values']['names_fieldset']['add_link']);
  }

  if (isset($form_state['values']['names_fieldset']['remove_link'])) {
    unset($form_state['values']['names_fieldset']['remove_link']);
  }

  for ($i = count($form_state['values']['names_fieldset']) - 1; 0 <= $i; $i--) {
    if (($form_state['values']['names_fieldset'][$i]['select'] != 1)) {
      if (simpletagcanvas_validate_url($form_state['values']['names_fieldset'][$i]['url'])) {
        form_set_error('names_fieldset][' . $i . '][url', t('target url: Please provide a valid url.'));
      }
    }
  }
}
