<?php

/**
 * @file
 * Mediabox entity forms.
 */

/**
 * Mediabox core settings form.
 * @param $form
 * @param $form_state
 * @return array
 */
function mediabox_form_settings($form , &$form_state) {
  $form['mediabox_library_path'] = array(
    '#type' => 'textfield',
    '#description' => t('Path to mediabox library view.')
      // @todo - we should fully test/check this feature.
      . '<br/><strong>' . t('NOTE: This feature is not fully implemented/tested. Use carefully.') . '</strong>',
    '#default_value' => variable_get('mediabox_library_path', 'admin/structure/mediabox-library'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * mediabox form - create/edit
 * @param $form
 * @param $form_state
 * @param $mediabox
 * @return array
 */
function mediabox_form_media($form, &$form_state, $mediabox) {
  $form = array();

  $form['mediabox'] = array(
    '#type' => 'value',
    '#value' => $mediabox,
  );

//  $form['image_info']['image_url'] = array(
//    '#title' => t('Url of file'),
//    '#type' => 'textfield',
//    '#value' => file_create_url($file->uri),
//    '#disabled' => TRUE,
//  );

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => isset($mediabox->title) ? $mediabox->title : '',
  );

  $form['alternate_text'] = array(
    '#title' => t('Alternate text'),
    '#type' => 'textfield',
    '#default_value' => isset($mediabox->alternate_text) ? $mediabox->alternate_text : '',
  );

  $form['caption'] = array(
    '#title' => t('Caption'),
    '#type' => 'textfield',
    '#default_value' => isset($mediabox->caption) ? $mediabox->caption : '',
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => isset($mediabox->description) ? $mediabox->description : '',
  );

  $form['actions'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('mediabox_form_media_submit'),
    '#weight' => 0,
  );

  $form['actions']['cancel'] = array(
    '#type' => 'markup',
    '#attributes' => array(
      'class' => array('button'),
    ),
    '#markup' => l(t('Cancel'), variable_get('mediabox_library_path', 'admin/structure/mediabox-library'),
      array(
        'attributes' => array('class' => array('button')),
    )),
    '#weight' => 2,
  );

  if (!empty($mediabox->id)) {
    $form['actions']['remove'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Remove'), 'mediabox/' . $mediabox->id . '/delete',
        array(
          'destination' => variable_get('mediabox_library_path', 'admin/structure/mediabox-library'),
          'attributes' => array('class' => array('button')),
      )),
      '#weight' => 1,
    );
  }

  field_attach_form('mediabox', $mediabox, $form, $form_state);
  return $form;
}

function mediabox_form_media_validate($form, &$form_state) {

  $mediabox = $form_state['values']['mediabox'];
  field_attach_form_validate('mediabox', $mediabox, $form, $form_state);
}

/**
 * Confirm delete - form callback
 *
 * @param $form
 * @param $form_state
 * @param $mediabox
 * @return array
 */
function mediabox_form_delete_confirm($form, &$form_state, $mediabox) {

  $form['mediabox'] = array(
    '#type' => 'value',
    '#value' => $mediabox,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete mediabox %title?', array('%title' => $mediabox->title)),
    '',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Confirm delete - submit callback
 *
 * @param $form
 * @param $form_state
 */
function mediabox_form_delete_confirm_submit($form, &$form_state) {
  $mediabox = $form_state['values']['mediabox'];

  if (mediabox_delete($mediabox)) {
    drupal_set_message(t('mediabox file %title was deleted', array('%title' => $mediabox->title)));
  }

  $form_state['redirect'] = variable_get('mediabox_library_path', 'admin/structure/mediabox-library');
}

/**
 * @param $form
 * @param $form_state
 */
function mediabox_form_media_submit($form, &$form_state) {
  $data = $form_state['values'];
  $mediabox = $data['mediabox'];

  // attach field submit
  field_attach_submit('mediabox', $mediabox, $form, $form_state);

  // assign values to entity properties
  $mediabox->title = $data['title'];
  $items = field_get_items('mediabox', $mediabox, 'mediabox_image');
  $mediabox->fid = $items[0]['fid'];
  $mediabox->alternate_text = $data['alternate_text'];
  $mediabox->caption = $data['caption'];
  $mediabox->description = $data['description'];

  if (mediabox_save($mediabox)) {
    drupal_set_message(t('Media file successfully saved.'), 'status');
    $form_state['redirect'] = variable_get('mediabox_library_path', 'admin/structure/mediabox-library');
  }
  else {
    drupal_set_message(t('Can not save media file!'), 'error');
  }
}
