<?php
/**
 * @file
 * Textimage admin page callback
 */

/**
 * Todo.
 */
function textimage_settings_form() {
  $form = array();

  $fonts_path = variable_get('textimage_fonts_path', drupal_get_path('module', 'textimage') . '/fonts');
  $form['fonts'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Fonts'),
    '#description' => t('Number of fonts found: @fonts', array('@fonts' => count(_textimage_font_list($fonts_path)))),
  );
  $form['fonts']['textimage_fonts_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $fonts_path,
    '#maxlength' => 255,
    '#description' => t('Location of the directory where the TrueType (.ttf) or OpenType (.otf) fonts are stored.') . '<br />' . t('Relative paths will be resolved relative to the Drupal installation directory.'),
  );

  $images_path = variable_get('textimage_images_path', drupal_get_path('module', 'textimage') . '/backgrounds');
  $form['images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Background images'),
    '#description' => t('Number of background images found: @images', array('@images' => count(_textimage_image_list($images_path)))),
  );
  $form['images']['textimage_images_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $images_path,
    '#maxlength' => 255,
    '#description' => t('Location of the directory where the background images are stored. Images in this directory can be overlaid with dynamic text in a preset.') . '<br />' . t('Relative paths will be resolved relative to the Drupal installation directory.'),
  );

  return system_settings_form($form);
}

/**
 * Todo.
 */
function textimage_settings_form_validate($form, &$form_state) {
  // Check for valid fonts path.
  if ($form_state['values']['textimage_fonts_path'] != "" && !is_dir(rtrim($form_state['values']['textimage_fonts_path'], '\\/'))) {
    form_set_error('textimage_fonts_path', t('The fonts path "%path" is invalid.', array('%path' => $form_state['values']['textimage_fonts_path'])));
  }

  // Check for valid backgorund images path.
  if ($form_state['values']['textimage_images_path'] != "" && !is_dir(rtrim($form_state['values']['textimage_images_path'], '\\/'))) {
    form_set_error('textimage_images_path', t('The background images path "%path" is invalid.', array('%path' => $form_state['values']['textimage_fonts_path'])));
  }
}

/**
 * Todo.
 */
function textimage_preset_edit($form, &$form_state, $op, $pid = NULL) {
  $preset = array();
  $image_presets = array();

  if (isset($form_state['values'])) {
    $preset = $form_state['values'];
  }
  elseif ($op == 'edit' && is_numeric($pid)) {
    $preset = _textimage_preset_load($pid);
  }

  $fonts_path   = variable_get('textimage_fonts_path', drupal_get_path('module', 'textimage') . '/fonts');
  $font_options = _textimage_font_list($fonts_path);

  $images_path = variable_get('textimage_images_path', drupal_get_path('module', 'textimage') . '/backgrounds');
  $image_files = drupal_map_assoc(_textimage_image_list($images_path));

  $presets = textimage_get_presets();
  $presets_list = array();
  foreach ($presets as $p) {
    $presets_list[$p->pid] = $p;
  }
  foreach ($presets as $p) {
    if ($p->pid != $pid) {
      $preset_test = $p;
      $preset_err = FALSE;
      while (is_numeric($preset_test->settings['background']['image'])) {
        if ($preset_test->settings['background']['image'] == $pid) {
          $preset_err = TRUE;
          break;
        }

        $preset_test = $presets_list[$preset_test->settings['background']['image']];
      }

      if (!$preset_err) {
        $image_presets[$p->pid] = $p->name;
      }
    }
  }

  $image_options = array(
    t('Default:') => array('' => t('Default (use background color)')),
    t('Use the result of a Textimage Preset:') => $image_presets,
    t('Use a Background Image:') => $image_files,
  );

  $form = array();
  $form['#tree'] = TRUE;

  if ($op == 'edit') {
    $form['pid'] = array(
      '#type' => 'hidden',
      '#value' => $pid,
    );
  }
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Preset Name'),
    '#description' => t('Preset names should be short and only use alphanumeric characters.'),
    '#default_value' => isset($preset['name']) ? $preset['name'] : '',
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('A short description displayed in the list of presets.'),
    '#default_value' => isset($preset['description']) ? $preset['description'] : '',
    '#rows' => 1,
  );
  // Preview textimage.
  $preview = '';

  if (count($preset) > 0) {
    $preview = $preset;
    $preview['pid'] = 0;
    $preview_text = isset($preview['settings']['preview']['text']['default']) ? $preview['settings']['preview']['text']['default'] : t('Preview');

    $additional_text = array();
    if (isset($preset['settings']['preview']['text']['additional'])) {
      $additional_text = $preset['settings']['preview']['text']['additional'];
      rsort($additional_text);
    }

    $preview = textimage_build_preview(array(
      'preset' => $preview,
      'text'   => $preview_text,
      'additional_text' => $additional_text,
    ));
  }

  $form['preview'] = array(
    '#type'   => 'item',
    '#markup' => '<strong>' . t('Preview') . ":</strong>\n" . '<div id="textimage-preview"><img src="' . file_create_url($preview) . '"></div>',
  );

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // Text settings.
  $form['settings']['font'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Font settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['settings']['font']['file'] = array(
    '#type'    => 'select',
    '#title'   => t('Font'),
    '#options' => $font_options,
    '#default_value' => isset($preset['settings']['font']['file']) ? $preset['settings']['font']['file'] : '',
    '#description' => t('Select the font to be used in this image. If no fonts are listed, check the <a href="!path">settings for Fonts Path</a>.', array('!path' => url('admin/config/media/textimage/settings'))),
    '#required'    => TRUE,
  );
  $form['settings']['font']['size'] = array(
    '#type'  => 'textfield',
    '#title' => t('Size'),
    '#field_suffix'  => t('px'),
    '#description'   => t('Enter the size in pixels of the text to be generated.'),
    '#default_value' => isset($preset['settings']['font']['size']) ? $preset['settings']['font']['size'] : 16,
    '#maxlength' => 5,
    '#size' => 3,
    '#required' => TRUE,
    '#element_validate' => array('_textimage_number_validate'),
  );
  $form['settings']['font']['color']['hex'] = array(
    '#type'  => 'textfield',
    '#title' => t('Color'),
    '#description'   => t('Enter the hex color code you wish to use for the generated text (i.e. #000000).'),
    '#default_value' => isset($preset['settings']['font']['color']['hex']) ? $preset['settings']['font']['color']['hex'] : '#000000',
    '#maxlength' => 7,
    '#size' => 8,
    '#element_validate' => array('_textimage_hex_validate'),
    '#required' => TRUE,
    '#attributes' => array(
      'class' => array('color'),
    ),
  );
  $form['settings']['font']['color']['opacity'] = array(
    '#type'  => 'textfield',
    '#title' => t('Opacity'),
    '#default_value' => isset($preset['settings']['font']['color']['opacity']) ? $preset['settings']['font']['color']['opacity'] : '100',
    '#maxlength' => 3,
    '#size' => 2,
    '#field_suffix' => '%',
    '#element_validate' => array('_textimage_number_validate'),
  );

  $form['settings']['text'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Text settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['settings']['text']['maximum_width'] = array(
    '#type'  => 'textfield',
    '#title' => t('Maximum width'),
    '#field_suffix' => t('px'),
    '#description' => t('Text lines wider than this will be wrapped. Leave blank to disable wrapping.'),
    '#default_value' => isset($preset['settings']['text']['maximum_width']) ? $preset['settings']['text']['maximum_width'] : '',
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('_textimage_number_validate'),
  );
  $form['settings']['text']['fixed_width'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Fixed width?'),
    '#description' => t('If checked the size of generated image will always be equal to the max width') . '.',
    '#default_value' => isset($preset['settings']['text']['fixed_width']) ? $preset['settings']['text']['fixed_width'] : '',
  );
  $form['settings']['text']['align'] = array(
    '#type'  => 'select',
    '#title' => t('Text Align'),
    '#description' => t('Only works when fixed width is enabled') . '.',
    '#options' => array(
      ALIGN_LEFT => t('Left'),
      ALIGN_CENTER => t('Center'),
      ALIGN_RIGHT => t('Right'),
    ),
    '#default_value' => isset($preset['settings']['text']['align']) ? $preset['settings']['text']['align'] : '',
  );
  $form['settings']['text']['angle'] = array(
    '#type'  => 'textfield',
    '#title' => t('Text Rotation'),
    '#field_suffix' => t('&deg;'),
    '#description' => t('Enter the angle in degrees at which the text will be displayed. Positive numbers rotate the text clockwise, negative numbers counter-clockwise.'),
    '#default_value' => isset($preset['settings']['text']['angle']) ? $preset['settings']['text']['angle'] : 0,
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('_textimage_number_validate'),
  );
  $form['settings']['text']['case'] = array(
    '#type'  => 'select',
    '#title' => t('Convert to case'),
    '#options' => array(
      '' => t('Default'),
      'upper' => t('UPPERCASE'),
      'lower' => t('lowercase'),
      'ucwords' => t('Uppercase Words'),
      'ucfirst' => t('Uppercase first'),
    ),
    '#description' => t('Covert the input text to a consistent format. The default makes no changes to input text.'),
    '#default_value' => isset($preset['settings']['text']['case']) ? $preset['settings']['text']['case'] : '',
  );

  // Outline.
  $form['settings']['text']['stroke'] = array(
    '#type'  => 'item',
    '#title' => t('Outline'),
  );
  $form['settings']['text']['stroke']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description'   => t('Optionally add a stroke outline around the text. Enter the stroke width in pixels.'),
    '#default_value' => isset($preset['settings']['text']['stroke']['width']) ? $preset['settings']['text']['stroke']['width'] : 0,
    '#maxlength' => 2,
    '#size' => 3,
    '#field_suffix' => 'px',
    '#element_validate' => array('_textimage_number_validate'),
  );
  $form['settings']['text']['stroke']['color'] = array(
    '#type'  => 'textfield',
    '#title' => t('Color'),
    '#default_value' => isset($preset['settings']['text']['stroke']['color']) ? $preset['settings']['text']['stroke']['color'] : '#000000',
    '#maxlength' => 7,
    '#size' => 8,
    '#attributes' => array(
      'class' => array('color'),
    ),
    '#description' => t('Enter the hex color code you wish to use for the stroke outline (i.e. #000000)'),
    '#element_validate' => array('_textimage_hex_validate'),
  );

  // Margin.
  $form['settings']['text']['margin'] = array(
    '#type'  => 'item',
    '#title' => t('Margin'),
    '#description' => t('Specify the margin in pixels to be added around the generated text.'),
  );
  $form['settings']['text']['margin']['top'] = array(
    '#type'  => 'textfield',
    '#title' => t('Top'),
    '#field_suffix'  => t('px'),
    '#default_value' => isset($preset['settings']['text']['margin']['top']) ? $preset['settings']['text']['margin']['top'] : 0,
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('_textimage_number_validate'),
  );
  $form['settings']['text']['margin']['right'] = array(
    '#type'  => 'textfield',
    '#title' => t('Right'),
    '#field_suffix'  => t('px'),
    '#default_value' => isset($preset['settings']['text']['margin']['right']) ? $preset['settings']['text']['margin']['right'] : 0,
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('_textimage_number_validate'),
  );
  $form['settings']['text']['margin']['bottom'] = array(
    '#type'  => 'textfield',
    '#title' => t('Bottom'),
    '#field_suffix'  => t('px'),
    '#default_value' => isset($preset['settings']['text']['margin']['bottom']) ? $preset['settings']['text']['margin']['bottom'] : 0,
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('_textimage_number_validate'),
  );
  $form['settings']['text']['margin']['left'] = array(
    '#type'  => 'textfield',
    '#title' => t('Left'),
    '#field_suffix'  => t('px'),
    '#default_value' => isset($preset['settings']['text']['margin']['left']) ? $preset['settings']['text']['margin']['left'] : 0,
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('_textimage_number_validate'),
  );

  // Background Settings.
  $form['settings']['background'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Background settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['settings']['background']['color'] = array(
    '#type'  => 'textfield',
    '#title' => t('Color'),
    '#description'  => t('Enter the hex color code you wish to use for the background of the generated image (i.e. #FFFFFF). Leave blank for transparent.'),
    '#default_value' => isset($preset['settings']['background']['color']) ? $preset['settings']['background']['color'] : '#FFFFFF',
    '#maxlength' => 7,
    '#size' => 8,
    '#element_validate' => array('_textimage_hex_validate'),
    '#attributes' => array(
      'class' => array('color'),
    ),
  );
  $form['settings']['background']['image'] = array(
    '#type'  => 'select',
    '#title' => t('Background Type'),
    '#options' => $image_options,
    '#default_value' => isset($preset['settings']['background']['image']) ? $preset['settings']['background']['image'] : '',
    '#ajax' => array(
      'callback' => 'textimage_js_preview',
    ),
    '#description' => t('Select the background type to be used in this image.'),
  );
  $form['settings']['background']['xoffset'] = array(
    '#type'  => 'textfield',
    '#title' => t('Text X-Offset'),
    '#field_suffix'  => 'px',
    '#default_value' => isset($preset['settings']['background']['xoffset']) ? $preset['settings']['background']['xoffset'] : 0,
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('_textimage_number_validate'),
  );
  $form['settings']['background']['yoffset'] = array(
    '#type'  => 'textfield',
    '#title' => t('Text Y-Offset'),
    '#field_suffix'  => 'px',
    '#description'   => t('Specify the x and y coordinates on the image you where the top-left corner of the dynamic text should be positioned.'),
    '#default_value' => isset($preset['settings']['background']['yoffset']) ? $preset['settings']['background']['yoffset'] : 0,
    '#maxlength' => 4,
    '#size' => 4,
    '#element_validate' => array('_textimage_number_validate'),
  );

  // Preview Textimage.
  $form['settings']['preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['preview']['text'] = array(
    '#type'   => 'item',
    '#prefix' => '<div id="textimage-preview-text">',
    '#suffix' => '</div>',
  );
  $form['settings']['preview']['text']['default'] = array(
    '#type'  => 'textarea',
    '#title' => t('Preview text'),
    '#default_value' => isset($preset['settings']['preview']['text']['default']) ? $preset['settings']['preview']['text']['default'] : t('Preview'),
    '#rows' => 2,
  );

  // Add additional text fields.
  if (isset($preset['settings']['preview']['text']['additional'])) {
    foreach ($preset['settings']['preview']['text']['additional'] as $id => $value) {
      $form['settings']['preview']['text']['additional'][$id] = array(
        '#type'  => 'textarea',
        '#title' => t('Additional text'),
        '#default_value' => isset($preset['settings']['preview']['text']['additional'][$id]) ? $preset['settings']['preview']['text']['additional'][$id] : t('Preview'),
        '#rows' => 2,
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save preset'),
  );

  $form['actions']['preview'] = array(
    '#type'  => 'button',
    '#value' => t('Preview'),
    '#ajax'  => array(
      'callback' => 'textimage_js_preview',
    ),
  );

  return $form;
}

/**
 * Todo.
 */
function theme_textimage_preset_edit(&$form) {
  drupal_add_css(drupal_get_path('module', 'textimage') . '/misc/css/textimage_admin.css');
  drupal_render_children($form);
}

/**
 * Todo.
 */
function textimage_preset_edit_validate($form, &$form_state) {
  // Check for illegal characters in preset names.
  if (preg_match('/[^0-9a-zA-Z_]/', $form_state['values']['name'])) {
    form_set_error('name', t('Please only use alphanumic characters and underscores (_) for preset names.'));
  }

  // Check for numbers at beginning of preset names.
  if (preg_match('/^[0-9]/', $form_state['values']['name'])) {
    form_set_error('name', t("Preset names can't begin with a number."));
  }

  // Check for duplicate preset names.
  $preset = _textimage_preset_load($form_state['values']['name']);
  if ($preset['name'] && (!isset($form_state['values']['pid']) || $form_state['values']['pid'] != $preset['pid'])) {
    form_set_error('name', t('The name %name is already in use by another preset.', array('%name' => $form_state['values']['name'])));
  }
}

/**
 * Todo.
 */
function textimage_preset_edit_submit($form, &$form_state) {
  if (isset($form_state['storage']['validate_error'])) {
    return;
  }
  if (!isset($form_state['values']['pid'])) {
    $return = _textimage_preset_create($form_state['values']['name'], $form_state['values']['description'], $form_state['values']['settings']);
  }
  elseif (is_numeric($form_state['values']['pid'])) {
    $return = _textimage_preset_update($form_state['values']['pid'], $form_state['values']['name'], $form_state['values']['description'], $form_state['values']['settings']);
    if (!$return) {
      drupal_set_message(t('No change in preset values.'), 'warning');
      drupal_goto('admin/config/media/textimage/preset/list');
    }
  }

  if ($return) {
    _textimage_flush_cache();
    drupal_theme_rebuild();
    drupal_set_message(t('Updated preset %name', array('%name' => $form_state['values']['name'])));
    drupal_goto('admin/config/media/textimage/preset/list');
  }
  else {
    drupal_set_message(t('The preset was unable to be updated.'), 'error');
  }
}

/**
 * Todo.
 */
function textimage_preset_list() {
  $fonts_path = variable_get('textimage_fonts_path', drupal_get_path('module', 'textimage') . '/fonts');
  $presets = textimage_get_presets();

  $header = array(
    t('Name'),
    t('Summary'),
    t('Description'),
    array(
      'data' => t('Operations'),
      'colspan' => '3',
    ),
  );
  $rows = array();

  foreach ($presets as $preset) {
    $font = _textimage_font_name($fonts_path . '/' . $preset->settings['font']['file']);

    $rows[] = array(
      $preset->name,
      $font['name'] . ' ' . $preset->settings['font']['size'] . t('px'),
      $preset->description,
      l(t('edit'), 'admin/config/media/textimage/preset/' . $preset->pid . '/edit'),
      l(t('delete'), 'admin/config/media/textimage/preset/' . $preset->pid . '/delete'),
      l(t('flush cache'), 'admin/config/media/textimage/preset/' . $preset->pid . '/flush'),
    );
  }
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('No presets defined. <a href="!url">Create a new preset</a>.', array('!url' => url('admin/config/media/textimage/preset/new'))),
        'colspan' => '6',
        'class' => 'message'),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Todo.
 */
function textimage_preset_delete_confirm($form, &$form_state, $pid) {
  $preset = _textimage_preset_load($pid);

  $form['pid'] = array('#type' => 'value', '#value' => $preset['pid']);
  $form['name'] = array('#type' => 'value', '#value' => $preset['name']);

  return confirm_form($form,
    t('Are you sure you want to delete the preset %name?', array('%name' => $preset['name'])),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/media/textimage/preset/list',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Todo.
 */
function textimage_preset_delete_confirm_submit($form, &$form_state) {
  _textimage_preset_delete($form_state['values']['pid']);
  _textimage_flush_cache();
  drupal_set_message(t('Deleted preset %name', array('%name' => $form_state['values']['name'])));
  drupal_goto('admin/config/media/textimage/preset/list');
}

/**
 * Todo.
 */
function textimage_preset_flush_confirm($form, &$form_state, $pid) {
  $preset = _textimage_preset_load($pid);

  $form['pid'] = array('#type' => 'value', '#value' => $preset['pid']);
  $form['name'] = array('#type' => 'value', '#value' => $preset['name']);

  return confirm_form($form,
    t('Are you sure you want to flush the image cache for preset %name?', array('%name' => $preset['name'])),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/media/textimage/preset/list',
    t('This action cannot be undone.'),
    t('Flush Cache'), t('Cancel')
  );
}

/**
 * Todo.
 */
function textimage_preset_flush_confirm_submit($form, &$form_state) {
  _textimage_preset_flush($form_state['values']['pid']);
  drupal_goto('admin/config/media/textimage/preset/list');
}

/**
 * Create a preset.
 *
 * @param string $name
 *   name of the preset to be created
 * @param string $description
 *   description
 * @param string|array $settings
 *   settings
 */
function _textimage_preset_create($name, $description, $settings) {
  // Serialize preset settings if not yet done.
  if (!is_string($settings)) {
    $settings = serialize($settings);
  }

  return db_insert('textimage_preset')
    ->fields(array(
      'name' => $name,
      'description' => $description,
      'settings' => $settings,
    ))
    ->execute();
}

/**
 * Update a preset.
 *
 * @param int $id
 *   preset id
 * @param string $name
 *   new name for the preset
 * @param string $description
 *   description
 * @param string $settings
 *   settings
 */
function _textimage_preset_update($id, $name, $description, $settings) {
  $name = check_plain($name);
  $result = db_update('textimage_preset')
    ->fields(array(
      'name' => $name,
      'description' => $description,
      'settings' => serialize($settings),
    ))
    ->condition('pid', $id)
    ->execute();
  _textimage_preset_flush($id);
  return $result;
}

/**
 * Todo.
 */
function _textimage_preset_delete($id) {
  $result = db_delete('textimage_preset')
    ->condition('pid', $id)
    ->execute();
  _textimage_preset_flush($id, TRUE);
  return $result;
}

/**
 * Flush cached media for a preset.
 *
 * @param string $pid
 *   A preset id.
 * @param bool $delete_data
 *   Boolean whether to delete textimage data or not
 */
function _textimage_preset_flush($pid, $delete_data = FALSE) {
  if ($pid !== 0) {
    drupal_set_message(t('Flushed Preset Images (ID: @pid)', array('@pid' => $pid)));
  }
  file_unmanaged_delete_recursive(variable_get('file_default_scheme', 'public') . '://textimage/' . $pid);
  if ($delete_data) {
    db_delete('textimage_image')
      ->condition('pid', $pid)
      ->execute();
  }
  else {
    $query = db_query('SELECT file FROM {textimage_image} WHERE pid LIKE :pid', array(':pid' => $pid))->fetchAll();
    foreach ($query as $file_obj) {
      // Clear cached versions of this image file in all styles.
      image_path_flush($file_obj->file);
      $filename = explode('/', $file_obj->file);
      $filename = $filename[count($filename) - 1];
      // Restore images in reference with images stored at {textimage_image}
      textimage_deliver($pid, $filename, FALSE);
    }
  }
}

/**
 * Clear a cached image.
 *
 * @param mixed $preset
 *   A preset name or ID.
 * @param string $file
 *   Image file.
 */
function _textimage_clear_cache_image($preset, $file) {
  if (is_numeric($preset)) {
    $pid = $preset;
  }
  else {
    $textimage_presets = textimage_get_presets();
    $pid = $textimage_presets[$preset]->pid;
  }
  $textimage_cache_dir = variable_get('file_default_scheme', 'public') . '://textimage/' . $pid;
  $uri = "$textimage_cache_dir/$file";
  // Clear textimage cache for this image.
  file_unmanaged_delete($uri);
  db_delete('textimage_image')
    ->condition('pid', $pid)
    ->condition('file', $uri)
    ->execute();
}

/**
 * Todo.
 */
function _textimage_flush_cache() {
  if (module_exists('content')) {
    cache_clear_all('*', 'cache_content', TRUE);
  }
}

/**
 * Returns an array of files with .otf/.ttf extensions in a directory.
 *
 * @param string $fontdir
 *   Full path of the font directory.
 *
 * @return array
 *   Array of font files.
 */
function _textimage_font_list($fontdir) {
  $filelist = array();
  if (is_dir($fontdir) && $handle = opendir($fontdir)) {
    while ($file = readdir($handle)) {
      if (preg_match("/\.[ot]tf$/i", $file) == 1) {
        $font = _textimage_font_name($fontdir . '/' . $file);
        $filelist[$file] = $font['name'];
      }
    }
    closedir($handle);
  }

  asort($filelist);
  return $filelist;
}

/**
 * Todo.
 */
function _textimage_font_name($filename) {
  $fd = fopen($filename, "r");
  $text = fread($fd, filesize($filename));
  fclose($fd);

  $number_of_tabs = _textimage_dec2hex(ord($text[4])) . _textimage_dec2hex(ord($text[5]));
  for ($i = 0; $i < hexdec($number_of_tabs); $i++) {
    $tag = $text[12 + $i * 16] . $text[12 + $i * 16 + 1] . $text[12 + $i * 16 + 2] . $text[12 + $i * 16 + 3];
    if ($tag == "name") {
      $offset_name_table_hex = _textimage_dec2hex(ord($text[12 + $i * 16 + 8])) . _textimage_dec2hex(ord($text[12 + $i * 16 + 8 + 1])) . _textimage_dec2hex(ord($text[12 + $i * 16 + 8 + 2])) . _textimage_dec2hex(ord($text[12 + $i * 16 + 8 + 3]));
      $offset_name_table_dec = hexdec($offset_name_table_hex);
      $offset_storage_hex = _textimage_dec2hex(ord($text[$offset_name_table_dec + 4])) . _textimage_dec2hex(ord($text[$offset_name_table_dec + 5]));
      $offset_storage_dec = hexdec($offset_storage_hex);
      $number_name_records_hex = _textimage_dec2hex(ord($text[$offset_name_table_dec + 2])) . _textimage_dec2hex(ord($text[$offset_name_table_dec + 3]));
      $number_name_records_dec = hexdec($number_name_records_hex);
      break;
    }
  }

  $storage_dec = $offset_storage_dec + $offset_name_table_dec;
  $storage_hex = drupal_strtoupper(dechex($storage_dec));
  $font = array(
    'copyright' => '',
    'family' => '',
    'subfamily' => '',
    'name' => '',
  );

  for ($j = 0; $j < $number_name_records_dec; $j++) {
    $platform_id_hex = _textimage_dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 0])) . _textimage_dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 1]));
    $platform_id_dec = hexdec($platform_id_hex);
    $name_id_hex = _textimage_dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 6])) . _textimage_dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 7]));
    $name_id_dec = hexdec($name_id_hex);
    $string_length_hex = _textimage_dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 8])) . _textimage_dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 9]));
    $string_length_dec = hexdec($string_length_hex);
    $string_offset_hex = _textimage_dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 10])) . _textimage_dec2hex(ord($text[$offset_name_table_dec + 6 + $j * 12 + 11]));
    $string_offset_dec = hexdec($string_offset_hex);

    if ($name_id_dec == 0 && empty($font['copyright'])) {
      for ($l = 0; $l < $string_length_dec; $l++) {
        if (ord($text[$storage_dec + $string_offset_dec + $l]) >= 32) {
          $font['copyright'] .= $text[$storage_dec + $string_offset_dec + $l];
        }
      }
    }

    if ($name_id_dec == 1 && empty($font['family'])) {
      for ($l = 0; $l < $string_length_dec; $l++) {
        if (ord($text[$storage_dec + $string_offset_dec + $l]) >= 32) {
          $font['family'] .= $text[$storage_dec + $string_offset_dec + $l];
        }
      }
    }

    if ($name_id_dec == 2 && empty($font['subfamily'])) {
      for ($l = 0; $l < $string_length_dec; $l++) {
        if (ord($text[$storage_dec + $string_offset_dec + $l]) >= 32) {
          $font['subfamily'] .= $text[$storage_dec + $string_offset_dec + $l];
        }
      }
    }

    if ($name_id_dec == 4 && empty($font['name'])) {
      for ($l = 0; $l < $string_length_dec; $l++) {
        if (ord($text[$storage_dec + $string_offset_dec + $l]) >= 32) {
          $font['name'] .= $text[$storage_dec + $string_offset_dec + $l];
        }
      }
    }

    if ($font['copyright'] != "" && $font['family'] != "" && $font['subfamily'] != "" && $font['name'] != "") {
      break;
    }
  }

  return $font;
}

/**
 * Auxiliar function to convert from decimal to hex.
 */
function _textimage_dec2hex($dec) {
  $hex = dechex($dec);
  return str_repeat("0", 2 - drupal_strlen($hex)) . drupal_strtoupper($hex);
}

/**
 * Auxiliar function to validate either the entered value is a valid hex color code or not.
 */
function _textimage_number_validate($field, $field_name) {
  if (!empty($field['#value']) && !is_numeric($field['#value'])) {
    form_set_error($field_name, t('The value for %field must be a number.', array('%field' => $field['#title'])));
  }
}

/**
 * Auxiliar function to validate either the entered value is a valid hex color code or not.
 */
function _textimage_hex_validate($field, $field_name) {
  if (!empty($field['#value']) && !preg_match('/^#[0123456789ABCDEF]{1,6}$/i', $field['#value'])) {
    form_set_error($field_name, t('The value for %field must be in a hexidecimal format (i.e. #FFFFFF is white).', array('%field' => $field['#title'])));
  }
}

/**
 * Returns an array of files with jpg, png, and gif extensions.
 *
 * From a specified directory.
 *
 * @param string $imagesdir
 *   Full path of the images directory.
 *
 * @return array
 *   Array of image files.
 */
function _textimage_image_list($imagesdir) {
  $filelist = array();
  if (is_dir($imagesdir) && $handle = opendir($imagesdir)) {
    while ($file = readdir($handle)) {
      if (preg_match("/\.gif|\.png|\.jpg$/i", $file) == 1) {
        $filelist[] = $imagesdir . '/' . $file;
      }
    }
    closedir($handle);
  }

  return $filelist;
}
