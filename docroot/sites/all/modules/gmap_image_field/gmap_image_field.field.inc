<?php

/**
 * @file
 * Implement an image field, based on the file module's file field.
 */

/**
 * Implements hook_field_info().
 */
function gmap_image_field_field_info() {
  return array(
    'gmap_image' => array(
      'label' => t('GMap Image'),
      'description' => t('Create a custom image map and use it on node.'),
      'settings' => array(
        'tile_size' => 256,
        'tile_format' => 'jpg',
        'custom_backgrounds' => TRUE,
        'custom_centers' => TRUE,
        'enable_descriptions' => TRUE,
        'bundles' => array(),
      ),
      'instance_settings' => array(),
      'default_widget' => 'gmap_image_widget_default',
      'default_formatter' => 'gmap_image_formatter_default',
      'cardinality' => 1,
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function gmap_image_field_field_widget_info() {
  return array(
    'gmap_image_widget_default' => array(
      'label' => t('Default'),
      'field types' => array('gmap_image'),
      'weight' => 100,
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_info_alter().
 *
 * Allow builtin image module render gmap_images_field as simple images.
 */
function gmap_image_field_field_formatter_info_alter(&$info) {
  if (array_key_exists('image', $info)) {
    // Seems that image module is enabled, and we have image formatter.
    $info['image']['field types'][] = 'gmap_image';
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function gmap_image_field_field_formatter_info() {
  return array(
    'gmap_image_formatter_default' => array(
      'label' => t('GMap engine'),
      'field types' => array('gmap_image'),
      'settings'  => array(
        'repeat_map' => FALSE,
        'bounds_lock' => TRUE,
        'default_background' => '#ffffff',
        'custom_controls' => FALSE,
        'comlete_disable_controls' => FALSE,
        'map_width' => '100%',
        'map_height' => '400px',
        'map_overview' => FALSE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function gmap_image_field_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {

  $display = $instance['display'][$view_mode];

  $settings = $display['settings'];

  $element = array();

  $element['map_width'] = array(
    '#title' => t('Width of map placeholder (%, px, em)'),
    '#type' => 'textfield',
    '#size' => 6,
    '#maxlength' => 12,
    '#default_value' => $settings['map_width'],
  );

  $element['map_height'] = array(
    '#title' => t('Height of map placeholder (%, px, em)'),
    '#type' => 'textfield',
    '#size' => 6,
    '#maxlength' => 12,
    '#default_value' => $settings['map_height'],
  );

  $element['custom_controls'] = array(
    '#title' => t('Use custom controls'),
    '#type' => 'checkbox',
    '#default_value' => $settings['custom_controls'],
  );

  $element['map_overview'] = array(
    '#title' => t('Show overview of image (thumbnail or bottom right)'),
    '#type' => 'checkbox',
    '#default_value' => $settings['map_overview'],
  );

  $element['comlete_disable_controls'] = array(
    '#title' => t('Complete disable controls'),
    '#type' => 'checkbox',
    '#default_value' => $settings['comlete_disable_controls'],
  );

  $element['default_background'] = array(
    '#title' => t('Default image background (html colorname or #RRGGBB)'),
    '#type' => 'textfield',
    '#size' => 7,
    '#maxlength' => 12,
    '#default_value' => $settings['default_background'],
  );

  $element['repeat_map'] = array(
    '#title' => t('Repeat map horizontaly'),
    '#type' => 'checkbox',
    '#default_value' => $settings['repeat_map'],
  );

  $element['bounds_lock'] = array(
    '#title' => t('Lock map bounds'),
    '#type' => 'checkbox',
    '#default_value' => $settings['bounds_lock'],
  );

  return $element;
}


/**
 * Implements hook_field_formatter_settings_summary().
 */
function gmap_image_field_field_formatter_settings_summary($field, $instance, $view_mode) {

  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $set_settings_list = array();

  if (!empty($settings['custom_controls'])) {
    $set_settings_list[] = t('custom controls');
  }

  if (!empty($settings['default_background'])) {
    $set_settings_list[] = t('backround');
  }

  if (!empty($settings['map_repeat'])) {
    $set_settings_list[] = t('map repeat');
  }

  if (!empty($settings['bounds_lock'])) {
    $set_settings_list[] = t('bounds lock');
  }

  if (!empty($settings['map_width'])) {
    $set_settings_list[] = t('fixed width');
  }

  if (!empty($settings['map_height'])) {
    $set_settings_list[] = t('fixed height');
  }

  if (!empty($settings['map_overview'])) {
    $set_settings_list[] = t('map overview');
  }

  if (!empty($settings['comlete_disable_controls'])) {
    $set_settings_list[] = t('disable all controls');
  }

  return t('Map with settings (:settings)', array(':settings' => implode(', ', $set_settings_list)));
}

/**
 * Implements hook_field_settings_form().
 */
function gmap_image_field_field_settings_form($field, $instance, $has_data) {

  if ($field['type'] == 'gmap_image') {
    $form = array();

    $form['tile_size'] = array(
      '#title' => t('Tile size'),
      '#type' => 'textfield',
      '#default_value' => $field['settings']['tile_size'],
      '#description' => t('Tile size in pixels (defautl used by google maps is 256)'),
    );

    $form['tile_format'] = array(
      '#title' => t('Tiles format'),
      '#type' => 'select',
      '#options' => drupal_map_assoc(array('jpg', 'png')),
      '#default_value' => $field['settings']['tile_format'],
      '#description' => t('PNG has better quality than JPG, but may increase the processing time on slow servers.'),
    );

    if ($has_data) {
      $form['tile_size']['#disabled'] = TRUE;
      $form['tile_size']['#value'] = $form['tile_size']['#default_value'];
      $form['tile_format']['#disabled'] = TRUE;
      $form['tile_format']['#value'] = $form['tile_format']['#default_value'];
    }

    $form['custom_backgrounds'] = array(
      '#title' => t('Use per image background color'),
      '#type' => 'checkbox',
      '#default_value' => $field['settings']['custom_backgrounds'],
    );

    $form['custom_centers'] = array(
      '#title' => t('Use per image centers'),
      '#type' => 'checkbox',
      '#default_value' => $field['settings']['custom_centers'],
    );

    $form['enable_descriptions'] = array(
      '#title' => t('Allow image descriptions'),
      '#type' => 'checkbox',
      '#default_value' => $field['settings']['enable_descriptions'],
      '#description' => t('Allow user input description about images'),
    );

    return $form;
  }
}

/**
 * Implements hook_form_alter().
 *
 * Force cardinality for the field to 1.
 */
function gmap_image_field_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'field_ui_field_edit_form' && $form['#field']['type'] == 'gmap_image') {
    $form['field']['cardinality'] = array(
      '#type' => 'value',
      '#value' => 1,
    );
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function gmap_image_field_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  switch ($display['type']) {
    case 'gmap_image_formatter_default':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'gmap_image_field_show_map',
          '#map' => _gmap_image_field_get_map_settings($entity, $field, $item, $display['settings']),
        );
      }
      break;
  }
  return $element;
}

/**
 * Implements hook_field_widget_form().
 */
function gmap_image_field_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $element['#type'] = 'fieldset';
  $element['#title'] = t('Map Image');

  $element_fid_id = 'gmap-image-field-input-' . $field['id'];

  $element['fid'] = array(
    '#title' => t('Image'),
    '#type' => 'managed_file',
    '#description' => t('Image with multiple dimension (weight%2 = 0 and height%2 = 0). For better results try using image with resolution bigger than 4096x.'),
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('png gif jpg jpeg'),
      '_gmap_image_field_check_is_image' => array(),
    ),
    '#id' => $element_fid_id,
  );

  $fid = NULL;
  if (!empty($form_state['values'])) {
    if (!empty($form_state['values'][$element['#field_name']][$langcode][$delta]['fid'])) {
      $fid = $form_state['values'][$element['#field_name']][$langcode][$delta]['fid'];
    }
  }
  else {
    if (!empty($items[$delta]['fid'])) {
      $element['fid']['#default_value'] = $items[$delta]['fid'];
      $fid = $items[$delta]['fid'];
    }
  }

  if ($fid && $file = file_load($fid)) {

    $image_preview_markup = theme('image', array(
      // @TODO, native preview url,
      // ATM is not possible because bug in image_style module.
      'path' => file_create_url(preg_replace('#^temporary://#', 'public://', image_style_path('medium', $file->uri))),
      'attributes' => array(
        'id' => 'gmap-image-field-image-' . $delta,
      ),
    ));

    $element['fid']['preview'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="image-widget form-managed-file clearfix">',
      '#suffix' => '</div>',
      '#weight' => 0,
      '#markup' => $image_preview_markup,
    );

  }

  // Extra field options.
  $element['minzoomlevel'] = array(
    '#type' => 'value',
    '#value' => empty($items[$delta]['minzoomlevel']) ? 0 : $items[$delta]['minzoomlevel'],
  );
  $element['maxzoomlevel'] = array(
    '#type' => 'value',
    '#value' => empty($items[$delta]['maxzoomlevel']) ? 5 : $items[$delta]['maxzoomlevel'],
  );
  $element['zoom'] = array(
    '#type' => 'value',
    '#value' => empty($items[$delta]['zoom']) ? 0 : $items[$delta]['zoom'],
  );
  $element['center'] = array(
    '#type' => 'value',
    '#value' => empty($items[$delta]['center']) ? '' : $items[$delta]['center'],
  );
  $element['background'] = array(
    '#type' => 'value',
    '#value' => empty($items[$delta]['background']) ? '' : $items[$delta]['background'],
  );
  $element['description'] = array(
    '#type' => 'value',
    '#value' => empty($items[$delta]['description']) ? '' : $items[$delta]['description'],
  );
  $element['custom_centers'] = array(
    '#type' => 'value',
    '#value' => empty($items[$delta]['custom_centers']) ? '' : $items[$delta]['custom_centers'],
  );

  if ($fid) {

    // Because there is a strange Drupal behavior,
    // that prevent storing field data, when using,
    // $element['fid']['minzoomlevel,zoom,...'] we have to simulate field,
    // hidding, in this way :(
    $visibility_state = array(
      'invisible' => array(
        '#' . $element_fid_id . ' input[type="hidden"]' => array('value' => 0),
      ),
    );

    $minzl = $element['minzoomlevel']['#value'];

    $element['minzoomlevel'] = array(
      '#type' => 'select',
      '#title' => t('Minimum zoomlevel'),
      '#options' => drupal_map_assoc(range(0, $element['maxzoomlevel']['#value'])),
      '#default_value' => $minzl,
      '#states' => $visibility_state,
    );

    $element['zoom'] = array(
      '#type' => 'select',
      '#title' => t('Initial zoomlevel'),
      '#options' => drupal_map_assoc(range($minzl, $element['maxzoomlevel']['#value'])),
      '#default_value' => $element['zoom']['#value'],
      '#states' => $visibility_state,
    );

    if ($field['settings']['custom_centers']) {
      $element['center'] = array(
        '#type' => 'textfield',
        '#title' => t('Initial center coordinates (X,Y)'),
        '#default_value' => $element['center']['#value'],
        '#id' => 'gmap-image-field-image-center-' . $delta,
        '#maxlength' => 128,
        '#states' => $visibility_state,
      );

      // Attach next code here, because if the center field is not input field,
      // there is no point of the js.
      $click_js = '
      jQuery("#gmap-image-field-image-' . $delta . '")
        .click(function(e) {
          var x = e.pageX - jQuery(this).offset().left;
          var y = e.pageY - jQuery(this).offset().top;
          px = x / jQuery(this).width();
          py = y / jQuery(this).height();
          zx = (180*2 * px - 180); // long
          zy = (85*2 * py - 85) * -1; // lat
          // Convert to latlng.
          jQuery("#gmap-image-field-image-center-' . $delta . '")
            .val(zy.toFixed(5)*1 + "," + zx.toFixed(5)*1);
        })
        .css({ cursor: \'crosshair\', margin: \'1em 0\' });';
      $element['fid']['preview']['#attached']['js'][$click_js] = array(
        'type' => 'inline',
        'group' => JS_THEME,
        'scope' => 'footer',
      );
    }

    if ($field['settings']['custom_backgrounds']) {
      $element['background'] = array(
        '#type' => 'textfield',
        '#title' => t('HTML background color (name or #RRGGBB)'),
        '#default_value' => $element['background']['#value'],
        '#size' => 7,
        '#maxlength' => 12,
        '#states' => $visibility_state,
      );
    }

    if ($field['settings']['enable_descriptions']) {
      $element['description'] = array(
        '#type' => 'textarea',
        '#title' => t('Description'),
        '#default_value' => $element['description']['#value'],
        '#states' => $visibility_state,
      );
    }
  }

  return $element;
}

/**
 * Implements hook_field_is_empty().
 */
function gmap_image_field_field_is_empty($item, $field) {
  if ($field['type'] == 'gmap_image') {
    if (empty($item['fid'])) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
}

/**
 * Implements hook_field_load().
 */
function gmap_image_field_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {

  $fids = array();
  foreach ($entities as $id => $entity) {
    // Load the files from the files table.
    foreach ($items[$id] as $delta => $item) {
      if (!empty($item['fid'])) {
        $fids[] = $item['fid'];
      }
    }
  }
  $files = file_load_multiple($fids);
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      // If the file does not exist, mark the entire item as empty.
      if (empty($item['fid']) || !isset($files[$item['fid']])) {
        $items[$id][$delta] = NULL;
      }
      else {
        $items[$id][$delta] = array_merge($item, (array) $files[$item['fid']]);
      }
    }
  }
}

/**
 * Implements hook_field_prepare_view().
 */
function gmap_image_field_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  // Remove files specified to not be displayed.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      if (!file_field_displayed($item, $field)) {
        unset($items[$id][$delta]);
      }
    }
    // Ensure consecutive deltas.
    $items[$id] = array_values($items[$id]);
  }
}

/**
 * Implements hook_field_insert().
 */
function gmap_image_field_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($field['type'] == 'gmap_image') {
    gmap_image_field_field_update($entity_type, $entity, $field, $instance, $langcode, $items);
  }
}

/**
 * Implements hook_field_update().
 */
function gmap_image_field_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  if ($field['type'] == 'gmap_image') {

    // Build a display of the current FIDs.
    $current_fids = array();
    foreach ($items as $item) {
      $current_fids[] = $item['fid'];
    }

    // Create a bare-bones entity so that we can load its previous values.
    $original = entity_create_stub_entity($entity_type, array(
      $id,
      $vid,
      $bundle));
    field_attach_load($entity_type,
      array($id => $original),
      FIELD_LOAD_CURRENT,
      array('field_id' => $field['id']));

    // First of all, remove old unused fields.
    $original_fids = array();
    if (!empty($original->{$field['field_name']}[$langcode])) {
      foreach ($original->{$field['field_name']}[$langcode] as $original_item) {
        $original_fids[] = $original_item['fid'];
        if (isset($original_item['fid']) && !in_array($original_item['fid'], $current_fids)) {
          // Dec the file usage count by 1 and delete the file if possible.
          _gmap_image_field_field_delete($entity_type, $entity, $field, $instance, $langcode, $original_item);
        }
      }
    }

    // And second, proceed newest.
    foreach ($items as &$item) {

      // Sanitize the center value.
      $center = explode(',', $item['center'], 2);
      $item['center'] = round((float) $center[0], 10) . ',' . round((float) (isset($center[1]) ? $center[1] : 0), 10);

      if (!in_array($item['fid'], $original_fids)) {

        $tiles_dir = variable_get('gmap_image_field_store', 'public://gmap-image-field');

        if (file_prepare_directory($tiles_dir, FILE_CREATE_DIRECTORY)) {
          $file = file_load($item['fid']);
          $file->status = 1;
          file_save($file);
          file_move($file, $tiles_dir . '/' . $file->filename);
          file_usage_add($file, 'gmap_image_field', $entity_type, $id);
          $tiler_info = _gmap_image_field_image_tiler($item['fid'], $field['settings']);
          if ($tiler_info) {
            $item['minzoomlevel'] = $tiler_info['minzoomlevel'];
            $item['maxzoomlevel'] = $tiler_info['maxzoomlevel'];
          }
          else {
            $item['fid'] = NULL;
          }
        }
        else {
          watchdog('GMAP_IMAGE_FIELD', 'Tiles directory (:directory) not available.', array(':directory' => $tiles_dir));
          $item['fid'] = NULL;
        }
      }
    }

  }

}

/**
 * Implements hook_field_delete().
 */
function gmap_image_field_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($field['type'] == 'gmap_image') {
    foreach ($items as $item) {
      _gmap_image_field_field_delete($entity_type, $entity, $field, $instance, $langcode, $item);
    }
  }
}

/**
 * Remove file from usage, file and tiles.
 *
 * @param string $entity_type
 *   entity type
 * @param entity $entity
 *   entity object
 * @param array $field
 *   field object
 * @param array $instance
 *   field instance
 * @param string $langcode
 *   field language
 * @param array $item
 *   current item
 *
 * @return bool
 *   success
 */
function _gmap_image_field_field_delete($entity_type, $entity, $field, $instance, $langcode, $item) {

  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  $file = file_load($item['fid']);
  if ($file) {
    if (!empty($file->status)) {
      $tilesdir = drupal_dirname($file->uri) . '/' . drupal_basename($file->uri, '.' . pathinfo($file->filename, PATHINFO_EXTENSION));
      if (is_dir($tilesdir)) {
        file_unmanaged_delete_recursive($tilesdir);
      }
    }
    file_usage_delete($file, 'gmap_image_field', $entity_type, $id);
    file_delete($file);
  }

  return TRUE;
}

/**
 * Callback that validate content type.
 *
 * @param object $file
 *   file object
 *
 * @return array
 *   messages if error
 */
function _gmap_image_field_check_is_image($file) {
  if (!empty($file->filemime) && strpos($file->filemime, 'image/') === 0) {
    return array();
  }
  else {
    return array(
      t('Only image files are accepted.'),
    );
  }
}

/**
 * Image tiler.
 *
 * @param int $drupal_file_source
 *   fid
 * @param array $settings
 *   settings array
 *
 * @return bool
 *   success
 */
function _gmap_image_field_image_tiler($drupal_file_source, $settings = array()) {

  if (_gmap_image_field_check_image_magick() && $drupal_file_source && ($file = file_load($drupal_file_source))) {

    $convert_executable = variable_get('gmap_image_field_convert_executable', GMAP_IMAGE_FIELD_IMAGICK_DEFAULT_EXEC);

    if (!$convert_executable) {
      watchdog('GMAP_IMAGE_FIELD', 'ImageMagic not found on the system (:PHP_OS)', array(':PHP_OS' => PHP_OS));
      return FALSE;
    }

    $imagepath = drupal_realpath($file->uri);
    $image_info = getimagesize($imagepath);
    $max_res = round($image_info[0] / $settings['tile_size']) * $settings['tile_size'];
    $tile_size_h = round($image_info[1] / $image_info[0] * $settings['tile_size']);

    $iwd = drupal_dirname($file->uri);

    $convert_executable = escapeshellcmd($convert_executable);

    $minzoomlevel = 0;
    for ($zoomlevel = $minzoomlevel, $cur_w = $settings['tile_size']; $cur_w <= $max_res; $cur_w *= 2, $zoomlevel++) {
      $zl_directory = $iwd . '/' . drupal_basename($file->uri, '.' . pathinfo($file->filename, PATHINFO_EXTENSION)) . '/' . $zoomlevel;
      if (file_prepare_directory($zl_directory, FILE_CREATE_DIRECTORY)) {

        // Prepare command line for creating zoomlevel source image file.
        $zl_image_source = drupal_realpath($zl_directory) . '/zoomlevel_' . $zoomlevel . '_src.' . $settings['tile_format'];
        $cmd_prepare_image_zl = $convert_executable .
          ' -filter box -resize ' . escapeshellarg($cur_w . 'x') . ' ' .
          escapeshellarg($imagepath) . ' ' .
          escapeshellarg($zl_image_source);

        // Prepare tiles for current zoomlevel.
        $zl_tilename = escapeshellarg(drupal_realpath($zl_directory) . '/{filename}.' . $settings['tile_format']);
        $zl_tilename = str_replace('{filename}', '%[filename:tile]', $zl_tilename);
        $cmd_prepare_tiles_zl = $convert_executable . ' ' . escapeshellarg($zl_image_source) . ' -strip -interlace Plane -crop ' . $settings['tile_size'] . 'x' . $tile_size_h . ' -set filename:tile %[fx:page.x/' . $settings['tile_size'] . ']_%[fx:page.y/' . $tile_size_h . '] +repage +adjoin ' . $zl_tilename;

        $zl_prepare_image_error = system($cmd_prepare_image_zl);

        if (!$zl_prepare_image_error) {

          // If we are not on Windows server,
          // and we have option to run tiler in background,... do it :)
          // This will decrease the user waiting on node saving,
          // while execute the command in background.
          if (strtoupper(substr(PHP_OS, 0, 3)) !== 'WIN' && variable_get('gmap_image_field_run_tiler_in_background', FALSE)) {
            $zl_prepare_tiles_error .= ' > /dev/null 2>&1 &';
          }

          $zl_prepare_tiles_error = system($cmd_prepare_tiles_zl);
          watchdog('GMAP_IMAGE_FIELD', 'Problem in creating tiles for zoomlevel - :zoomlevel', array(
            ':zoomlevel' => $zoomlevel,
          ));
        }
        else {
          watchdog('GMAP_IMAGE_FIELD', 'Problem in creating zoomlevel (:zoomlevel) file - (:file)', array(
            ':zoomlevel' => $zoomlevel,
            ':file' => $zl_image_source,
          ));
        }

        if (!$zl_prepare_image_error && !$zl_prepare_tiles_error) {
          drupal_set_message(t('Successful created tiles for zoomlevel - :zoomlevel', array(':zoomlevel' => $zoomlevel)));
        }

      }
      else {
        drupal_set_message(t('Problem while creating tiles for zoomlevel :zoomlevel.', array(':zoomlevel' => $zoomlevel)), 'error');
      }
    }

    return array(
      'minzoomlevel' => $minzoomlevel,
      'maxzoomlevel' => ($zoomlevel - 1),
    );

  }
  return FALSE;
}

/**
 * Test for ImageMagick.
 *
 * @return bool
 *   available or not.
 */
function _gmap_image_field_imagick_check() {
  if (!(function_exists('system') && function_exists('shell_exec'))) {
    return FALSE;
  }
  $convert_executable = variable_get('gmap_image_field_convert_executable', GMAP_IMAGE_FIELD_IMAGICK_DEFAULT_EXEC);
  $o = shell_exec(escapeshellcmd($convert_executable) . ' ' . escapeshellcmd('-version'));
  return ($o && strpos($o, 'Version:') !== FALSE);
}
