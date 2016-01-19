<?php
/**
 * @file
 * Administrative page callbacks for the imageflow module.
 */


/**
 * Menu callback; Listing of all current presets.
 */
function imageflow_page_preset_list() {
  $presets = imageflow_presets();

  $header = array(t('preset Name'), array(
      'data' => t('Operations'),
      'colspan' => 2,
    ));
  $rows = array();
  foreach ($presets as $name => $preset) {
    $rows[] = array(
      l($preset['title'], 'admin/config/media/imageflow/edit/' . $name),
      l(t('edit'), 'admin/config/media/imageflow/edit/' . $name),
      // Hide the delete link for the 'default' set.
      ($name == 'default') ? '' : l(t('delete'), 'admin/config/media/imageflow/delete/' . $name),
    );
  }

  return theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'empty' => t('There are currently no presets. <a href="!url">Add a new one</a>.', array('!url' => url('admin/config/media/imageflow/add'))),
  ));
}

/**
 * Form builder; Form for adding a new preset.
 */
function imageflow_form_preset_add($form, &$form_state) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#maxlength' => '255',
    '#title' => t('Title'),
    '#description' => t('A human-readable title for this preset.'),
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => '255',
    '#machine_name' => array(
      'source' => array('title'),
      'exists' => 'imageflow_preset_exists',
    ),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Create new preset'),
    ),
    'cancel' => array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'admin/config/media/imageflow',
    ),
  );

  return $form;
}

/**
 * Submit handler for adding a new preset.
 */
function imageflow_form_preset_add_submit($form, &$form_state) {
  $preset = array(
    'name' => $form_state['values']['name'],
    'title' => $form_state['values']['title'],
    'options' => array(
      'buttons' => TRUE,
    ),
  );
  if (module_exists('imagecache_reflect')) {
    $preset['imagestyle'] = 'imageflow_reflect';
  }
  $preset = imageflow_preset_save($preset, TRUE);
  drupal_set_message(t('preset %name was created.', array('%name' => $preset['name'])));
  $form_state['redirect'] = 'admin/config/media/imageflow/edit/' . $preset['name'];
}


/**
 * Theme to embed tables into forms.
 */
function theme_imageflow_form_table($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form) as $row_key) {
    $row = array();
    foreach (element_get_visible_children($form[$row_key]) as $cell_key) {
      $cell = array('data' => drupal_render($form[$row_key][$cell_key]));
      if (!empty($form[$row_key][$cell_key]['#table_attributes'])) {
        $cell += $form[$row_key][$cell_key]['#table_attributes'];
      }
      $row[] = $cell;
    }
    $rows[] = $row;
  }

  $variables = array();
  foreach ($form as $key => $value) {
    if (element_property($key)) {
      $variables[substr($key, 1)] = $value;
    }
  }
  $variables['rows'] = $rows;

  return theme('table', $variables);
}

/**
 * Option elements.
 * 
 * This function returns an array defining the form elements used to
 * edit the different options.
 */
function imageflow_option_elements() {
  return array(
    'animationSpeed' => array(
      '#type' => 'textfield',
      '#title' => t('Animation speed'),
      '#description' => t('Animation speed in ms.'),
      '#element_validate' => array('_imageflow_validate_integer'),
      '#default_value' => 50,
    ),
    'aspectRatio' => array(
      '#type' => 'textfield',
      '#title' => t('Aspect ratio'),
      '#description' => t('Aspect ratio of the ImageFlow container (width divided by height).'),
      '#element_validate' => array('_imageflow_validate_number'),
      '#default_value' => 1.964,
    ),
    'buttons' => array(
      '#type' => 'checkbox',
      '#title' => t('Buttons'),
      '#description' => t('Toggle navigation buttons.'),
      '#default_value' => TRUE,
    ),
    'captions' => array(
      '#type' => 'checkbox',
      '#title' => t('Captions'),
      '#description' => t('Disables / enables the captions.'),
      '#default_value' => TRUE,
    ),
    'circular' => array(
      '#type' => 'checkbox',
      '#title' => t('Circular'),
      '#description' => t('Toggle circular rotation.'),
      '#default_value' => TRUE,
    ),
    'glideToStartID' => array(
      '#type' => 'checkbox',
      '#title' => t('Glide to start ID'),
      '#description' => t('Toggle glide animation to start ID.'),
      '#default_value' => TRUE,
    ),
    'imageCursor' => array(
      '#type' => 'textfield',
      '#title' => t('Image cursor'),
      '#description' => t('Cursor type for the images (try "pointer").'),
      '#default_value' => 'default',
    ),
    'imageFocusM' => array(
      '#type' => 'textfield',
      '#title' => t('imageFocusM'),
      '#description' => t('Multiplicator for the focussed image size in percent.'),
      '#element_validate' => array('_imageflow_validate_integer'),
      '#default_value' => 1.0,
    ),
    'imageFocusMax' => array(
      '#type' => 'textfield',
      '#title' => t('imageFocusMax'),
      '#description' => t('Maximum number of images on each side of the focussed one.'),
      '#element_validate' => array('_imageflow_validate_integer'),
      '#default_value' => 4,
    ),
    'imageScaling' => array(
      '#type' => 'checkbox',
      '#title' => t('imageScaling'),
      '#description' => t('Toggle image scaling.'),
      '#default_value' => TRUE,
    ),
    'imagesHeight' => array(
      '#type' => 'textfield',
      '#title' => t('imagesHeight'),
      '#description' => t('Height of the images div container in percent.'),
      '#element_validate' => array('_imageflow_validate_opacity'),
      '#default_value' => 0.67,
    ),
    'imagesM' => array(
      '#type' => 'textfield',
      '#title' => t('imagesM'),
      '#description' => t('Multiplicator for all images in percent.'),
      '#element_validate' => array('_imageflow_validate_opacity'),
      '#default_value' => 1.0,
    ),
    'opacity' => array(
      '#type' => 'checkbox',
      '#title' => t('Opacity'),
      '#description' => t('Disables / enables image opacity.'),
      '#default_value' => TRUE,
    ),
    'percentLandscape' => array(
      '#type' => 'textfield',
      '#title' => t('percentLandscape'),
      '#description' => t('Scale landscape format.'),
      '#element_validate' => array('_imageflow_validate_integer'),
      '#default_value' => 118,
    ),
    'percentOther' => array(
      '#type' => 'textfield',
      '#title' => t('percentOther'),
      '#description' => t('Scale portrait and square format.'),
      '#element_validate' => array('_imageflow_validate_integer'),
      '#default_value' => 100,
    ),
    'preloadImages' => array(
      '#type' => 'checkbox',
      '#title' => t('preloadImages'),
      '#description' => t('Disables / enables the loading bar and image preloading.'),
      '#default_value' => TRUE,
    ),
    'scrollbarP' => array(
      '#type' => 'textfield',
      '#title' => t('scrollbarP'),
      '#description' => t('Width of the scrollbar in percent.'),
      '#element_validate' => array('_imageflow_validate_opacity'),
      '#default_value' => 0.6,
    ),
    'slider' => array(
      '#type' => 'checkbox',
      '#title' => t('slider'),
      '#description' => t('Disables / enables the scrollbar.'),
      '#default_value' => TRUE,
    ),
    'sliderCursor' => array(
      '#type' => 'textfield',
      '#title' => t('sliderCursor'),
      '#description' => t('Cursor type for the slider (try "default").'),
      '#default_value' => 'e-resize',
    ),
    'sliderWidth' => array(
      '#type' => 'textfield',
      '#title' => t('sliderWidth'),
      '#description' => t('Width of the slider in px.'),
      '#element_validate' => array('_imageflow_validate_integer'),
      '#default_value' => 14,
    ),
    'slideshow' => array(
      '#type' => 'checkbox',
      '#title' => t('slideshow'),
      '#description' => t('Toggle slideshow.'),
      '#default_value' => TRUE,
    ),
    'slideshowSpeed' => array(
      '#type' => 'textfield',
      '#title' => t('slideshowSpeed'),
      '#description' => t('Time between slides in ms.'),
      '#element_validate' => array('_imageflow_validate_integer'),
      '#default_value' => 1500,
    ),
    'slideshowAutoplay' => array(
      '#type' => 'checkbox',
      '#title' => t('slideshowAutoplay'),
      '#description' => t('Toggle automatic slideshow play on startup.'),
      '#default_value' => TRUE,
    ),
    'startID' => array(
      '#type' => 'textfield',
      '#title' => t('startID'),
      '#description' => t('Glide to this image number on startup.'),
      '#element_validate' => array('_imageflow_validate_integer'),
      '#default_value' => 1,
    ),
    'startAnimation' => array(
      '#type' => 'checkbox',
      '#title' => t('startAnimation'),
      '#description' => t('Animate images moving in from the right on startup.'),
      '#default_value' => TRUE,
    ),
    'xStep' => array(
      '#type' => 'textfield',
      '#title' => t('xStep'),
      '#description' => t('Step width on the x-axis in px.'),
      '#element_validate' => array('_imageflow_validate_integer'),
      '#default_value' => 150,
    ),
  );
}

/**
 * Returns the form element to use to edit the given option.
 */
function imageflow_option_element($option, $value) {
  $elements = imageflow_option_elements();
  $element = isset($elements[$option]) ? $elements[$option] : array('#type' => 'textfield');

  if ($value !== NULL) {
    if ($element['#type'] == 'select') {
      if ($value === TRUE) {
        $value = 'true';
      }
      elseif ($value === FALSE) {
        $value = 'false';
      }
    }
    $element['#default_value'] = $value;
  }

  return $element;
}

/**
 * Form  builder; Form to edit a given preset.
 */
function imageflow_form_preset_edit($form, &$form_state, $preset) {
  if (empty($form_state['preset'])) {
    $form_state['preset'] = $preset;
  }
  else {
    $preset = $form_state['preset'];
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#maxlength' => '255',
    '#title' => t('Title'),
    '#default_value' => $preset['title'],
    '#description' => t('A human-readable title for this preset.'),
    '#required' => TRUE,
  );

  $form['imagestyle'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#default_value' => $preset['imagestyle'],
    '#empty_option' => t('None (original image)'),
    '#options' => image_style_options(FALSE),
  );
  if (!module_exists('imagecache_reflect')) {
    $form['imagestyle'] += array(
      '#description' => t("Install the image reflect module to add a reflection preset."),
    );
  }

  if (module_exists('colorbox')) {
    $form['lightbox'] = array(
      '#title' => t('OnClick behavior'),
      '#type' => 'select',
      '#default_value' => $preset['lightbox'],
      '#options' => array(
        'normal' => t('Normal url'),
        'colorbox' => t('Colorbox'),
        'colorboxiframe' => t('Colorbox iframe'),
      ),
    );
  }

  // Option table.
  $form['options'] = array(
    '#theme' => 'imageflow_form_table',
    '#tree' => TRUE,
    '#header' => array(t('Name'), t('Value'), t('Operations')),
  );

  $i = 0;
  foreach ($preset['options'] as $key => $value) {
    $option_element = imageflow_option_element($key, $value);

    $form['options'][] = array(
      'name' => array(
        '#type' => 'item',
        '#title' => check_plain($key),
        '#description' => isset($option_element['#title']) ? $option_element['#title'] : '',
      ),
      'value' => $option_element + array(
        '#option_name' => $key,
        '#title_display' => 'none',
      ),
      'delete' => array(
        '#type' => 'submit',
        '#name' => 'button_del_' . $i++,
        '#value' => t('Delete'),
        '#submit' => array('imageflow_form_preset_edit_submit_delete'),
        '#limit_validation_errors' => array(),
      ),
    );
  }

  // 'Add option' row at the end of the table.
  $options = array_diff(array_keys(imageflow_option_elements()), array_keys($preset['options']));
  $options = empty($options) ? array() : array_combine($options, $options);
  $form['options'][] = array(
    'add_option_row' => array(
      '#table_attributes' => array('colspan' => '3', 'class' => array('container-inline')),
      '#tree' => FALSE,
      'new_option' => array(
        '#type' => 'select',
        '#options' => $options,
        '#empty_option' => t('Select or enter:'),
      ),
      'new_option_custom' => array(
        '#type' => 'textfield',
        '#states' => array(
          'visible' => array(
            ':input[name="new_option"]' => array('value' => ''),
          ),
        ),
      ),
      'button_add' => array(
        '#type' => 'submit',
        '#name' => 'add_option',
        '#value' => t('Add option'),
        '#submit' => array('imageflow_form_preset_edit_submit_add'),
        '#limit_validation_errors' => array(
          array('new_option'),
          array('new_option_custom'),
        ),
      ),
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#name' => 'submit',
      '#value' => t('Save preset'),
    ),
    'cancel' => array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'admin/config/media/imageflow',
    ),
  );

  return $form;
}

/**
 * Validate a form element that should have an integer value.
 */
function _imageflow_validate_integer($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || intval($value) != $value || $value < 0)) {
    form_error($element, t('%name must be a positive integer.', array('%name' => $element['#title'])));
  }
}

/**
 * Validate a form element that should have a number as value.
 */
function _imageflow_validate_number($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && !is_numeric($value)) {
    form_error($element, t('%name must be a number.', array('%name' => $element['#option_name'])));
  }
}

/**
 * Validate a form element that should have a value between 0 and 1.
 */
function _imageflow_validate_opacity($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || $value < 0 || $value > 1)) {
    form_error($element, t('%name must be a value between 0 and 1.', array('%name' => $element['#option_name'])));
  }
}

/**
 * Submit handler for 'Add option' button; Add a new option to the set.
 */
function imageflow_form_preset_edit_submit_add($form, &$form_state) {
  $preset = &$form_state['preset'];

  if (!empty($form_state['values']['new_option'])) {
    $new_option_element = 'new_option';
  }
  elseif (!empty($form_state['values']['new_option_custom'])) {
    $new_option_element = 'new_option_custom';
  }

  if (isset($new_option_element)) {
    $new_option = $form_state['values'][$new_option_element];
    if (!array_key_exists($new_option, $preset['options'])) {
      // Add the new option with a NULL value.
      // The input element cares for a default value.
      $preset['options'][$new_option] = NULL;
      // Reset the input field.
      $form_state['input'][$new_option_element] = '';
      drupal_set_message(t('Option %name added.', array('%name' => $new_option)));
    }
    else {
      form_set_error($new_option_element, t('This set already includes the %name option.', array('%name' => $new_option)));
    }
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for 'Delete' buttons; Delete an option from the set.
 */
function imageflow_form_preset_edit_submit_delete($form, &$form_state) {
  $preset = &$form_state['preset'];

  $rowindex = $form_state['triggering_element']['#parents'][1];
  $option = $form['options'][$rowindex]['value']['#option_name'];

  unset($preset['options'][$option]);
  drupal_set_message(t('Option %name removed.', array('%name' => $option)));

  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for 'Save preset' button; Save the changed preset.
 */
function imageflow_form_preset_edit_submit($form, &$form_state) {
  $preset = &$form_state['preset'];

  $preset['title'] = $form_state['values']['title'];
  $preset['imagestyle'] = $form_state['values']['imagestyle'];
  if (isset($form_state['values']['lightbox'])) {
    $preset['lightbox'] = $form_state['values']['lightbox'];
  }

  if (isset($form_state['values']['options'])) {
    foreach ($form_state['values']['options'] as $index => $values) {
      $element = $form['options'][$index]['value'];
      $value  = $values['value'];

      if ($value !== '') {
        if ($element['#type'] == 'checkbox') {
          $value = (bool) $value;
        }
        elseif (is_numeric($value)) {
          $value = (float) $value;
        }
        elseif (strcasecmp($value, 'true') == 0) {
          $value = TRUE;
        }
        elseif (strcasecmp($value, 'false') == 0) {
          $value = FALSE;
        }
      }

      $option = $element['#option_name'];
      $preset['options'][$option] = $value;
    }
  }

  imageflow_preset_save($preset);
  drupal_set_message(t('preset %name changed.', array('%name' => $preset['name'])));
  $form_state['redirect'] = 'admin/config/media/imageflow';
}


/**
 * Form builder; Form to delete a given preset.
 */
function imageflow_preset_form_delete($form, &$form_state, $preset) {
  $form_state['preset'] = &$preset;

  return confirm_form($form,
    t('Are you sure you want to delete the preset %name?', array('%name' => $preset['name'])),
    'admin/config/media/imageflow',
    NULL,
    t('Delete'),  t('Cancel')
  );
}

/**
 * Submit handler for deleting an preset.
 */
function imageflow_preset_form_delete_submit($form, &$form_state) {
  $preset = &$form_state['preset'];

  if ($preset['name'] == 'default') {
    // Prevent deletion of the default set so we can fall back to it.
    drupal_set_message(t('The default preset may not be deleted!', 'error'));
  }
  else {
    imageflow_preset_delete($preset);
    drupal_set_message(t('preset %name was deleted.', array('%name' => $preset['name'])));
  }

  $form_state['redirect'] = 'admin/config/media/imageflow';
}

/**
 * Form builder; Form for advanced module settings.
 */
function imageflow_form_settings() {
  $form = array();

  $form['library'] = array(
    '#type' => 'fieldset',
    '#title' => 'Imageflow Library',
  );

  $variant = variable_get('imageflow_library_variant', 'source');

  $library = libraries_detect('imageflow');
  $path = $library['library path'];

  if (!$path) {
    drupal_set_message(t('Imageflow library not found.'), 'error');
    $form['library']['missing'] = array(
      '#type' => 'item',
      '#title' => t('Library missing'),
      '#description' => t('The Imageflow library cannot be found.'),
    );
  }
  else {
    $form['library']['variant'] = array(
      '#type' => 'radios',
      '#title' => t('Library variant'),
      '#default_value' => $variant,
      '#options' => array(
        'source' => t('Source'),
        'minified' => t('Minified'),
      ),
      '#description' => t('Choose whether to use the source or minified version of the imageflow library.'),
      '#required' => TRUE,
    );

    $file = $path . '/' . key($library['variants'][$variant]['files']['js']);
    $form['library']['lib_js_file'] = array(
      '#type' => 'item',
      '#title' => t('Current JS file'),
      '#markup' => $file ? t('<code>@file</code>', array('@file' => $file)) : t('Unknown'),
    );

    $file = $path . '/' . key($library['variants'][$variant]['files']['css']);
    $form['library']['lib_css_file'] = array(
      '#type' => 'item',
      '#title' => t('Current CSS file'),
      '#markup' => $file ? t('<code>@file</code>', array('@file' => $file)) : t('Unknown'),
    );

    $form['library']['info'] = array(
      '#theme' => 'imageflow_form_table',
      '#header' => array(t('Name'), t('Value')),
      '#empty' => t('Library not found!'),
    );
    $info = array('name', 'vendor url', 'download url', 'library path');
    foreach ($library as $key => $value) {
      if (in_array($key, $info)) {
        $form['library']['info'][] = array(
          array(
            '#markup' => check_plain(ucfirst($key)),
          ),
          array(
            '#markup' => t('<code>@file</code>', array('@file' => $value)),
          ),
        );
      }
    }

    $form['library']['button_clearcache'] = array(
      '#type' => 'submit',
      '#name' => 'button_clearcache',
      '#value' => t('Clear cache'),
      '#submit' => array('imageflow_form_settings_submit_clearcache'),
    );

    $form['library']['button_submit'] = array(
      '#type' => 'submit',
      '#name' => 'button_submit',
      '#value' => t('Submit'),
      '#submit' => array('imageflow_form_settings_submit'),
    );
  }

  return $form;
}

/**
 * Submit handler for the advanced module settings form button 'Clear cache'.
 */
function imageflow_form_settings_submit_clearcache($form, &$form_state) {
  libraries_flush_caches();
  drupal_set_message(t('Cache cleared.'));
}

/**
 * Submit handler for the advanced module settings.
 */
function imageflow_form_settings_submit($form, &$form_state) {
  $value = $form_state['values']['variant'];
  variable_set('imageflow_library_variant', $value);
  drupal_set_message(t('Library variant %value saved.', array('%value' => $value)));
}
