<?php
/**
 * @file
 * Process functions.
 */

/**
 * Element state: 'default'. Add upload fields.
 */
function _jcrop_fapi_element_state_default_process(&$element, $form_state, $form) {
  $fid = isset($element['#value']['fid']) ? $element['#value']['fid'] : 0;

  $ajax_settings = array(
    'path' => 'file/ajax/' . implode('/', $element['#array_parents']) . '/' . $form['form_build_id']['#value'],
    'wrapper' => $element['#id'] . '-ajax-wrapper',
    'effect' => 'fade',
    'progress' => array(
      'type' => $element['#progress_indicator'],
      'message' => $element['#progress_message'],
    ),
  );

  // Set up the buttons first since we need to check if they were clicked.
  $element['upload_button'] = array(
    '#name' => implode('_', $element['#parents']) . '_upload_button',
    '#type' => 'submit',
    '#value' => t('Upload'),
    '#validate' => array(),
    '#submit' => array('jcrop_fapi_jcrop_image_file_submit'),
    '#limit_validation_errors' => array($element['#parents']),
    '#ajax' => $ajax_settings,
    '#weight' => -5,
    '#attributes' => array(
      'class' => array(
        implode('_', $element['#parents']) . '_upload_button',
      ),
    ),
  );

  $element['fid'] = array(
    '#type' => 'hidden',
    '#value' => $fid,
  );

  $ajax_settings = array(
    'path' => 'file/ajax/' . implode('/', $element['#array_parents']) . '/' . $form['form_build_id']['#value'],
    'wrapper' => $element['#id'] . '-ajax-wrapper',
    'effect' => 'fade',
    'progress' => array(
      'type' => $element['#progress_indicator'],
      'message' => $element['#progress_message'],
    ),
  );

  // The file upload field itself.
  $element['upload'] = array(
    '#name' => 'files[' . implode('_', $element['#parents']) . ']',
    '#type' => 'file',
    '#title' => t('Choose a file'),
    '#title_display' => 'invisible',
    '#size' => $element['#size'],
    '#theme_wrappers' => array(),
    '#weight' => -10,
    '#ajax' => $ajax_settings,
  );

  // Add cropper form fields.
  $crop_fields = _jcrop_fapi_add_crop_fields($element, FALSE);
  $element = array_merge($element, $crop_fields);

  // Add the extension list to the page as JavaScript settings.
  if (isset($element['#upload_validators']['file_validate_extensions'][0])) {
    $extension_list = implode(',', array_filter(explode(' ', $element['#upload_validators']['file_validate_extensions'][0])));
    $element['upload']['#attached']['js'] = array(
      array(
        'type' => 'setting',
        'data' => array(
          'file' => array(
            'elements' => array(
              '#' . $element['#id'] . '-upload' => $extension_list,
            ),
          ),
        ),
      ),
    );
  }
}

/**
 * Element state: 'edit'. Add cropping fields.
 */
function _jcrop_fapi_element_state_edit_process(&$element, $form_state, $form) {
  $element['#theme'] = 'jcrop_element';

  // Attach js.
  $path = drupal_get_path('module', 'jcrop_fapi');
  $element['#attached']['js'][] = "$path/js/jcrop_fapi.js";

  // Add Jcrop library.
  libraries_load('jcrop', 'source');
  $file_to_crop = $element['#file'];
  $element['cropbox'] = array(
    '#markup' => theme('image', array(
      'path' => $file_to_crop->uri,
      'attributes' => array(
        'class' => 'cropbox',
        'id' => $element['#id'] . '-cropbox',
        'style' => 'display: none;',
      ),
      )
    ),
    '#weight' => -20,
  );

  // Title'n description of the edit image step.
  if (isset($element['#jcrop_settings']['cropbox_title'])) {
    $element['cropbox']['#prefix'] = '<div class="title">' . $element['#jcrop_settings']['cropbox_title'] . '</div>';
  }
  if (isset($element['#jcrop_settings']['cropbox_description'])) {
    $element['cropbox']['#suffix'] = '<div class="description">' . $element['#jcrop_settings']['cropbox_description'] . '</div>';
  }

  // Crop actions.
  $ajax_settings = array(
    'path' => 'file/ajax/' . implode('/', $element['#array_parents']) . '/' . $form['form_build_id']['#value'],
    'wrapper' => $element['#id'] . '-ajax-wrapper',
    'effect' => 'fade',
    'progress' => array(
      'type' => $element['#progress_indicator'],
      'message' => $element['#progress_message'],
    ),
  );

  $element['save'] = array(
    '#name' => implode('_', $element['#parents']) . '_save_crop',
    '#type' => 'submit',
    '#value' => t('Crop'),
    '#validate' => array(),
    '#submit' => array('jcrop_fapi_jcrop_image_save_submit'),
    '#limit_validation_errors' => array($element['#parents']),
    '#ajax' => $ajax_settings,
    '#weight' => -15,
    '#attributes' => array(
      'class' => array('save_crop'),
    ),
  );
  $element['remove_button'] = array(
    '#name' => implode('_', $element['#parents']) . '_remove_button',
    '#type' => 'submit',
    '#value' => t('Remove'),
    '#validate' => array(),
    '#submit' => array('jcrop_fapi_jcrop_image_file_submit'),
    '#limit_validation_errors' => array($element['#parents']),
    '#ajax' => $ajax_settings,
    '#weight' => -10,
    '#attributes' => array(
      'class' => array('remove_button'),
    ),
  );
  $element['cancel'] = array(
    '#name' => implode('_', $element['#parents']) . '_save_cancel',
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#validate' => array(),
    '#submit' => array('jcrop_fapi_jcrop_image_cancel_submit'),
    '#limit_validation_errors' => array($element['#parents']),
    '#ajax' => $ajax_settings,
    '#weight' => -5,
    '#attributes' => array(
      'class' => array('save_cancel'),
    ),
  );

  // Jcrop settings.
  list($res_w, $res_h) = explode('x', $element['#jcrop_settings']['resolution']);
  list($crop_w, $crop_h) = explode('x', $element['#jcrop_settings']['croparea']);

  $image = image_load(drupal_realpath($file_to_crop->uri));

  // Add cropping fields.
  $crop_fields = _jcrop_fapi_add_crop_fields($element, $image);
  $element = array_merge($element, $crop_fields);

  $settings = array(
    $element['#id'] => array(
      'box' => array(
        'ratio' => $res_h ? $element['#jcrop_settings']['enforce_ratio'] * $res_w / $res_h : 0,
        'box_width' => $crop_w,
        'box_height' => $crop_h,
        'enforce_ratio' => $element['#jcrop_settings']['enforce_ratio'],
      ),
      'minimum' => array(
        'width'   => $element['#jcrop_settings']['enforce_minimum'] ? $res_w : NULL,
        'height'  => $element['#jcrop_settings']['enforce_minimum'] ? $res_h : NULL,
      ),
      // True size properties.
      'width' => $image->info['width'],
      'height' => $image->info['height'],
    ),
  );

  // Attach settings js.
  $element['#attached']['js'][] = array(
    'data' => array('jcrop_fapi_crop' => $settings),
    'type' => 'setting',
    'scope' => 'header',
  );
}

/**
 * Element state: 'saved'.
 */
function _jcrop_fapi_element_state_saved_process(&$element, $form_state, $form) {
  $element['#theme'] = 'jcrop_element';
  $cropped_file = file_load($element['fid']['#value']);

  // Add cropped image.
  $element['cropbox'] = array(
    '#markup' => theme('image', array(
        'path' => $cropped_file->uri,
        'width' => $element['#jcrop_settings']['cropped_img_width'],
        )),
    '#weight' => -20,
  );

  $ajax_settings = array(
    'path' => 'file/ajax/' . implode('/', $element['#array_parents']) . '/' . $form['form_build_id']['#value'],
    'wrapper' => $element['#id'] . '-ajax-wrapper',
    'effect' => 'fade',
    'progress' => array(
      'type' => $element['#progress_indicator'],
      'message' => $element['#progress_message'],
    ),
  );

  $element['edit'] = array(
    '#name' => implode('_', $element['#parents']) . '_edit_crop',
    '#type' => 'submit',
    '#value' => t('Edit Image'),
    '#validate' => array(),
    '#submit' => array('jcrop_fapi_jcrop_image_edit_submit'),
    '#limit_validation_errors' => array($element['#parents']),
    '#ajax' => $ajax_settings,
    '#weight' => -15,
  );

  $element['remove_button'] = array(
    '#name' => implode('_', $element['#parents']) . '_remove_button',
    '#type' => 'submit',
    '#value' => t('Remove'),
    '#validate' => array(),
    '#submit' => array('jcrop_fapi_jcrop_image_file_submit'),
    '#limit_validation_errors' => array($element['#parents']),
    '#ajax' => $ajax_settings,
    '#weight' => -10,
  );

  // Add cropping fields.
  $crop_fields = _jcrop_fapi_add_crop_fields($element);
  $element = array_merge($element, $crop_fields);
}

/**
 * Crop 'n resize the image.
 */
function _jcrop_fapi_image_crop_resize($src, $crop = NULL, $scale = NULL, $dst = NULL) {
  $image = image_load($src);
  if ($image) {
    $result = TRUE;
    if ($crop) {
      $result = $result && image_crop($image, $crop['x'], $crop['y'], $crop['width'], $crop['height']);
    }
    if ($scale) {
      $result = $result && image_scale_and_crop($image, $scale['width'], $scale['height']);
    }
    $result = $result && image_save($image, $dst ? $dst : $src);
    return $result;
  }

  return FALSE;
}

/**
 * Builds cropping form fileds with values.
 */
function _jcrop_fapi_add_crop_fields($element, $image = FALSE) {
  $defaults = $element['#value'];

  if (!$element['#value']['changed'] && $image) {
    // Set default selection.
    $defaults['x'] = 0;
    $defaults['y'] = 0;
    $defaults['height'] = $image->info['height'];
    $defaults['width'] = $image->info['width'];
  }

  foreach ($defaults as $name => $default) {
    $fields[$name] = array(
      '#type' => 'hidden',
      '#title' => $name,
      '#attributes' => array('class' => array('edit-image-crop-' . $name)),
      '#default_value' => $default,
      '#value' => $default,
    );
  }

  return $fields;
}
