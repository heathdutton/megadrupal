<?php
/**
 * @file
 * Media watermark module integration.
 *
 * Main interface for insert and update actions.
 */

/**
 * Form constructor for the media watermark form.
 *
 * @see media_watermark_form_validate()
 * @see media_watermark_form_submit()
 *
 * @ingroup forms
 */
function media_watermark_form($form, &$form_state) {
  if (arg(5)) {
    drupal_set_title(t('Edit watermark'));
    $values = media_watermark_get_watermark(arg(5));
  }
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['watermark_image'] = array(
    '#type' => 'managed_file',
    '#title' => t('Watermark image'),
    '#upload_location' => 'public://watermark',
    '#upload_validators' => array('file_validate_extensions' => array('png gif')),
    '#description' => t('allowed files extensions are .png and .gif'),
    '#required' => TRUE,
    '#default_value' => isset($values->fid) ? $values->fid : '',
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($values->name) ? $values->name : '',
    '#description' => t('Set watermark name.'),
    '#required' => TRUE,
  );
  $form['hor_position'] = array(
    '#type' => 'select',
    '#title' => t('Horizontal position of watermark'),
    '#options' => array(
      'left' => t('left'),
      'middle' => t('middle'),
      'right' => t('right'),
    ),
    '#default_value' => isset($values->hor_position) ? $values->hor_position : 'left',
    '#required' => TRUE,
  );
  $form['ver_position'] = array(
    '#type' => 'select',
    '#title' => t('Vertical position of watermark'),
    '#options' => array(
      'top' => t('top'),
      'center' => t('center'),
      'bottom' => t('bottom'),
    ),
    '#default_value' => isset($values->ver_position) ? $values->ver_position : 'bottom',
    '#required' => TRUE,
  );
  $form['hor_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontal margin, px'),
    '#default_value' => isset($values->hor_margin) ? $values->hor_margin : '0',
    '#description' => t('Set minus or plus signed value for moving watermark to left or right from defined position.'),
    '#required' => TRUE,
  );
  $form['ver_margin'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical margin, px'),
    '#default_value' => isset($values->ver_margin) ? $values->ver_margin : '0',
    '#description' => t('Set minus or plus signed value for moving watermark to higher or lower from defined position.'),
    '#required' => TRUE,
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Submit'));

  return $form;
}

/**
 * Form validation handler for media_watermark_form().
 *
 * @see media_watermark_form_submit()
 */
function media_watermark_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['watermark_image'])) {
    // Name validation.
    if (is_string($form_state['values']['name']) && strlen($form_state['values']['name']) > 255) {
      form_set_error('name', t('Too long name, it should be less then or equal to 255 character.'));
    };
    // Hor margin validation.
    if (!is_numeric($form_state['values']['hor_margin'])) {
      form_set_error('hor_margin', t('Horizontal margin, should be numeric.'));
    }
    elseif (strlen($form_state['values']['hor_margin']) > 20) {
      form_set_error('hor_margin', t('Too long horizontal margin, it should be less then or equal to 20 digits.'));
    }
    // Ver margin validation.
    if (!is_numeric($form_state['values']['ver_margin'])) {
      form_set_error('ver_margin', t('Vertical margin, should be numeric.'));
    }
    elseif (strlen($form_state['values']['ver_margin']) > 20) {
      form_set_error('ver_margin', t('Too long vertical margin, it should be less then or equal to 20 digits.'));
    }
  }
}

/**
 * Form submission handler for media_watermark_form().
 *
 * @see media_watermark_form_validate()
 */
function media_watermark_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['watermark_image'])) {
    $data = array(
      'fid' => $form_state['values']['watermark_image'],
      'name' => $form_state['values']['name'],
      'hor_position' => $form_state['values']['hor_position'],
      'ver_position' => $form_state['values']['ver_position'],
      'hor_margin' => $form_state['values']['hor_margin'],
      'ver_margin' => $form_state['values']['ver_margin'],
    );

    $file = file_load($form_state['values']['watermark_image']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    global $user;
    file_usage_add($file, 'media_watermark', 'image', $user->uid);

    if (arg(5)) {
      db_update('media_watermark')
        ->fields($data)
        ->condition('wid', arg(5))
        ->execute();
      drupal_set_message(t("Watermark has been updated."));

    }
    else {
      db_insert('media_watermark')
        ->fields($data)
        ->execute();
      drupal_set_message(t("Watermark has been added."));
    }
  }
  $form_state['redirect'] = 'admin/config/media/watermark';
}
