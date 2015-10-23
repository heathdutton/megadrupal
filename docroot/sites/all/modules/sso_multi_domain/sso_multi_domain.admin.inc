<?php

/**
 * @file
 * Provides page callbacks for configuration page.
 */

/**
 * Callback function for Single Sign On setting page.
 */
function sso_multi_domain_configure($form, &$form_state) {

  $form['sso_settings'] = array(
    '#title' => t("Configure Settings for Single Sign On"),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
  );

  $form['sso_settings']['sso_multi_domain_domain_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter Domain Names per line (Without http)'),
    '#rows' => 5,
    '#default_value' => variable_get('sso_multi_domain_domain_list', ''),
    '#description' => t('Please enter the domain names per line in which you 
      want single sign on to be enabled. For example, if you enter domain1.com, 
      domain2.com, then when users login to this current domain then they will 
      be able to login to these two domains as well. e.g, domain1.com or 
      www.domain.com. Don\'t use http.'),
    '#required' => TRUE,
  );
  $form['sso_settings']['sso_multi_domain_allow_registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Account Creation'),
    '#default_value' => variable_get('sso_multi_domain_allow_registration', FALSE),
    '#description' => t('Check this option if you want to allow account creation 
       if user does not exists on your website when any requesting websites tries to 
       login via this module.'),
  );

  $form['sso_settings']['sso_multi_domain_allow_roles_sysc'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Roles Synchronization During Account Creation'),
    '#default_value' => variable_get('sso_multi_domain_allow_roles_sysc', FALSE),
    '#description' => t('Check this option if you want to allow synchronization  
       of roles from requesting website when role does not exists. For example, 
       if a user have role editor and he tries to login to a sister website. 
       When the request send to login to this website by this module, the 
       system will create "editor" role in this website and assign it to the 
       user.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="sso_multi_domain_allow_registration"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $message = variable_get('sso_multi_domain_message', '');
  $form['sso_settings']['sso_multi_domain_message'] = array(
    '#type' => 'text_format',
    '#title' => t('Text for single sign on processing page'),
    '#default_value' => isset($message['value']) ? $message['value'] : t('Please wait while we are processsing'),
    '#format' => isset($message['format']) ? $message['format'] : 'plain_text',
    '#description' => t('This text will be visible on sso processing page where 
        loader image appears.'),
  );

  $form['sso_settings']['sso_multi_domain_loader_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text Below Loader Image'),
    '#description' => t('This text will be visible just below the loader image.'),
    '#default_value' => variable_get('sso_multi_domain_loader_text', t('Please wait...')),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $loader_image_fid = variable_get('sso_multi_domain_loader_image_fid', '');

  // The below mentioned code is use to display uploaded image and provide
  // delete the existing image as well.
  if ($loader_image_fid) {
    $loader_image = file_load($loader_image_fid);
    $form['sso_settings']['loader_image_picture'] = array(
      '#type' => 'value',
      '#value' => $loader_image,
    );
    $form['sso_settings']['loader_image_current'] = array(
      '#markup' => theme('image_style', array('style_name' => 'thumbnail', 'path' => $loader_image->uri)),
    );
    $form['sso_settings']['loader_image_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete image'),
      '#access' => !empty($loader_image_fid),
      '#description' => t('Check this box to delete current loader image.'),
    );
    $form['sso_settings']['loader_image_fid'] = array(
      '#type' => 'hidden',
      '#value' => $loader_image_fid,
    );
  }
  $form['sso_settings']['loader_image'] = array(
    '#title' => t('Loader Image'),
    '#type' => 'file',
    '#description' => t("Upload an image for the loader. If not uploaded then 
        default image will be used."),
  );

  $form['sso_settings']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Implements hook_validate().
 */
function sso_multi_domain_configure_validate($form, &$form_state) {
  if (!empty($_FILES['files']['name']['loader_image'])) {
    $file = file_save_upload('loader_image', array(
      'file_validate_is_image' => array(),
      'file_validate_extensions' => array('png gif jpg jpeg'),
    ));
    // If the file passed validation:
    if ($file) {
      // Move the file, into the Drupal file system.
      if ($file = file_move($file, 'public://')) {
        // Save the file for use in the submit handler.
        $form_state['storage']['loader_image'] = $file;
      }
      else {
        form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
      }
    }
    else {
      form_set_error('file', t('No file was uploaded.'));
    }
  }
}

/**
 * Implements hook_submit().
 */
function sso_multi_domain_configure_submit($form, &$form_state) {

  if (isset($form_state['values']['loader_image_picture']) && is_object($form_state['values']['loader_image_picture'])) {
    $loader_image_fid = $form_state['values']['loader_image_picture']->fid;
  }
  else {
    $loader_image_fid = 0;
  }

  if (isset($form_state['values']['loader_image_delete']) &&
      $form_state['values']['loader_image_delete']) {
    file_delete($form_state['values']['loader_image_picture']);
    $loader_image_fid = 0;
  }

  if (!empty($_FILES['files']['name']['loader_image'])) {
    $file = $form_state['storage']['loader_image'];
    // We are done with the file, remove it from storage.
    unset($form_state['storage']['loader_image']);
    // Make the storage of the file permanent.
    $file->status = FILE_STATUS_PERMANENT;
    // Save file status.
    file_save($file);
    $loader_image_fid = $file->fid;
  }
  variable_set('sso_multi_domain_domain_list', $form_state['values']['sso_multi_domain_domain_list']);
  variable_set('sso_multi_domain_allow_registration', $form_state['values']['sso_multi_domain_allow_registration']);
  variable_set('sso_multi_domain_allow_roles_sysc', $form_state['values']['sso_multi_domain_allow_roles_sysc']);
  variable_set('sso_multi_domain_message', $form_state['values']['sso_multi_domain_message']);
  variable_set('sso_multi_domain_loader_text', $form_state['values']['sso_multi_domain_loader_text']);
  variable_set('sso_multi_domain_loader_image_fid', $loader_image_fid);
}
