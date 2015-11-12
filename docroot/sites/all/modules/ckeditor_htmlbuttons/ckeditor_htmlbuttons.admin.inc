<?php

/**
 * @file
 * Administrative page callbacks for the CKEditor HTML Buttons module.
 */

/**
 * CKEditor HTML Buttons overview page - view buttons and edit them.
 */
function ckeditor_htmlbuttons_overview() {
  $rows = array();
  // Load the buttons and display them on the admin form.
  $buttons = ckeditor_htmlbutton_load_all();
  foreach ($buttons as $button) {
    $row = array();
    $row[] = !empty($button['icon']) ? theme('image', array('path' => $button['icon'])) : '';
    $row[] = $button['title'];
    $row[] = l(t('edit'), 'admin/config/content/ckeditor-htmlbuttons/edit/' . $button['name']);
    $row[] = l(t('delete'), "admin/config/content/ckeditor-htmlbuttons/delete/" . $button['name']);

    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No buttons available.'),
        'colspan' => 4,
      ),
    );
  }
  $rows[] = array(
    array(
      'data' => l(t('Create a new button'), 'admin/config/content/ckeditor-htmlbuttons/add'),
      'colspan' => 4,
    ),
  );

  $header = array(
    array(
      'data' => '',
      'width' => '1',
    ),
    array(
      'data' => t('Name'),
      'width' => '70%',
    ),
    array(
      'data' => t('Operations'),
      'colspan' => 2,
    ),
  );

  if (module_exists('wysiwyg') || module_exists('ckeditor')) {
    if (module_exists('wysiwyg')) {
      $help_variables = array('!profiles' => l(t('WYSIWYG Profile'), 'admin/config/content/wysiwyg'));
    }
    else {
      $help_variables = array('!profiles' => l(t('CKEditor'), 'admin/config/content/ckeditor'));
    }
    $help = t('Here you can create custom html toolbar buttons. The buttons will need to be enabled from the !profiles settings.', $help_variables) . '</p>';
  }
  else {
    $help = t('You need to install either !ckeditor or !wysiwyg module before the buttons you define here are possible to use.', array(
      '!ckeditor' => l(t('CKEditor'), 'https://www.drupal.org/project/ckeditor'),
      '!wysiwyg' => l(t('Wysiwyg'), 'https://www.drupal.org/project/wysiwyg'),
    ));
  }

  return '<p>' . $help . '</p>' . theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'id' => 'ckeditor_htmlbuttons',
  ));
}


/**
 * CKEditor HTML Buttons create/modify form.
 */
function ckeditor_htmlbuttons_button_form($something, $form_state, $button = NULL) {
  if (isset($form_state['confirm_delete'])) {
    // Rebuild the form to confirm term deletion.
    $form['name'] = array('#type' => 'value', '#value' => $form_state['values']['name']);
    $form['delete'] = array('#type' => 'value', '#value' => TRUE);
    return _ckeditor_htmlbuttons_delete_confirm_form($form, $form_state['values']['title']);
  }

  if (!empty($button)) {
    // Add the current values as defaults to the form, if editing an existing
    // item.
    $form_state['values'] = $button;
  }

  $form = array();
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Button Name'),
    '#default_value' => isset($form_state['values']['title']) ? $form_state['values']['title'] : '',
    '#description' => t('Select a name for this button.'),
    '#maxlength' => 80,
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'ckeditor_htmlbuttons_name_exists',
      'source' => array('title'),
    ),
    '#description' => t('A unique machine-readable name for this button. It must only contain lowercase letters, numbers, and underscores.'),
  );
  // Load icon if it has one.
  $image = '';
  if (isset($form_state['values']['fid']) && $form_state['values']['fid']) {
    $image_uri = file_load($form_state['values']['fid'])->uri;
    if ($image_uri) {
      $image = theme('image', array('path' => $image_uri));
    }
  }
  $form['button_icon'] = array(
    '#type' => 'file',
    '#title' => t('Choose a file'),
    '#size' => 22,
    '#description' => t('Allowed file types: <strong>png gif</strong>.<br>Images must be exactly <strong>16x16</strong> pixels.'),
    '#prefix' => $image,
  );
  $form['button_icon_fid'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($form_state['values']['fid']) ? $form_state['values']['fid'] : 0,
  );
  // Add delete button if it has an icon.
  if (isset($form_state['values']['fid']) && $form_state['values']['fid']) {
    $form['button_icon_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete the icon.'),
    );
  }
  $form['html'] = array(
    '#type' => 'textarea',
    '#title' => t('Button Template'),
    '#rows' => 10,
    '#default_value' => isset($form_state['values']['html']) ? $form_state['values']['html'] : '',
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  if (!empty($button)) {
    if (!empty($form_state['values']['name'])) {
      $form['name']['#default_value'] = $form_state['values']['name'];
      $form['name']['#disabled'] = TRUE;
      $form['name']['#value'] = $form_state['values']['name'];
    }

    // If it's an existing button, offer a delete button.
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }
  return $form;
}

/**
 * CKEditor HTML Buttons form submit - delete and save handlers.
 */
function ckeditor_htmlbuttons_button_form_submit($form, &$form_state) {
  // If we're deleting the button.
  if ($form_state['clicked_button']['#id'] == 'edit-delete') {
    // Show the confirmation.
    $form_state['rebuild'] = TRUE;
    $form_state['confirm_delete'] = TRUE;
    return;
  }
  // Delete confirmation provided.
  if (isset($form_state['values']['delete'])) {
    if ($form_state['values']['delete'] === TRUE) {
      // Delete image if one was uploaded.
      if (!empty($form_state['values']['button_icon_fid'])) {
        file_delete($form_state['values']['button_icon_fid']);
      }
      ckeditor_htmlbuttons_delete_button($form_state['values']['name']);
      drupal_set_message(t('The button has been deleted.'));
      $form_state['redirect'] = 'admin/config/content/ckeditor-htmlbuttons';
      return;
    }
  }
  // Drop image if selected and checked.
  if (isset($form_state['values']['button_icon_delete']) && $form_state['values']['button_icon_delete']) {
    file_delete(file_load($form_state['values']['button_icon_fid']));
    // Unset the fid previously used.
    $form_state['values']['button_icon_fid'] = 0;
  }
  // Prepare file if needed.
  $filepath = 'public://ckeditor_htmlbuttons/';
  file_prepare_directory($filepath, FILE_CREATE_DIRECTORY);
  // Save the image, validate it against file_validate_extensions.
  $file = file_save_upload('button_icon', array(
    'file_validate_extensions' => array('png gif'),
    'file_validate_image_resolution' => array('16x16', '16x16'),
  ), $filepath);
  if ($file) {
    // Set status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    $file = file_save($file);
    if ($file) {
      $form_state['values']['fid'] = $file->fid;
      // Delete previous file if it had one.
      if ($form_state['values']['button_icon_fid']) {
        file_delete(file_load($form_state['values']['button_icon_fid']));
      }
    }
  }
  else {
    $form_state['values']['fid'] = $form_state['values']['button_icon_fid'];
  }

  // Save the button.
  if (ckeditor_htmlbutton_save_button($form_state['values'])) {
    drupal_set_message(t('The button has been saved.'));
  }
  else {
    drupal_set_message(t('There was an error saving the button to the database.'));
  }

  // Redirect back to the overview page.
  $form_state['redirect'] = 'admin/config/content/ckeditor-htmlbuttons';
}


/**
 * Menu callback -- ask for confirmation of rule deletion.
 */
function ckeditor_htmlbuttons_delete_confirm($something, &$form_state) {
  $form['name'] = array(
    '#type' => 'value',
    '#value' => isset($form_state['build_info']['args'][0]) ? $form_state['build_info']['args'][0] : 0,
  );

  $button = ckeditor_htmlbutton_load_button(isset($form_state['build_info']['args'][0]) ? $form_state['build_info']['args'][0] : 0);
  return _ckeditor_htmlbuttons_delete_confirm_form($form, $button['title']);
}

/**
 * Execute node deletion.
 */
function ckeditor_htmlbuttons_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    ckeditor_htmlbuttons_delete_button($form_state['values']['name']);
  }

  $form_state['redirect'] = 'admin/config/content/ckeditor-htmlbuttons';
}

/**
 * Helper function to generate the delete confirm form.
 */
function _ckeditor_htmlbuttons_delete_confirm_form($form, $button_title) {
  return confirm_form($form,
    t("Are you sure you want to delete the button '%title'?", array('%title' => $button_title)),
    'admin/config/content/ckeditor-htmlbuttons',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}
