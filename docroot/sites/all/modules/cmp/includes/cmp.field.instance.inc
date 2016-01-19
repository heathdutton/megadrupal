<?php
/**
 * @file
 * Hooks and callbacks for field instance.
 */

/**
 * Implements hook_field_instance_settings_form().
 */
function cmp_field_instance_settings_form($field, $instance) {
  $settings_defaults = $instance['settings'];
  $settings_saved = isset($instance['settings']['wrapper']) ? $instance['settings']['wrapper'] : array();
  $settings = array_merge($settings_defaults, $settings_saved);

  $form['wrapper'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('CaptureMyPage field instance settings'),
    '#weight' => 10,
  );

  $form['wrapper']['renderDelay'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Render Delay'),
    '#description'      => t('Number of milliseconds to wait after a page loads before taking the screenshot.'),
    '#default_value'    => (int) $settings['renderDelay'],
    '#size'             => 5,
    '#maxlength'        => 5,
    '#field_suffix'     => t('ms'),
    '#element_validate' => array('element_validate_integer'),
  );

  $form['wrapper']['userAgent'] = array(
    '#type'          => 'textfield',
    '#title'         => t('User-agent'),
    '#description'   => t('Custom user-agent.'),
    '#default_value' => $settings['userAgent'],
  );

  $form['wrapper']['streamType'] = array(
    '#type'          => 'select',
    '#title'         => t('Stream Type'),
    '#description'   => t('If streaming is used, this designates the file format of the streamed rendering. Possible values are "png", "jpg", and "jpeg".'),
    '#options'       => array(
      'png'  => 'png',
      'jpg'  => 'jpg',
      'jpeg' => 'jpeg',
    ),
    '#default_value' => $settings['streamType'],
  );

  $form['wrapper']['quality'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Quality'),
    '#description'      => t('JPEG compression quality. A higher number will look better, but creates a larger file. Quality setting has no effect when streaming.'),
    '#default_value'    => (int) $settings['quality'],
    '#size'             => 5,
    '#maxlength'        => 3,
    '#field_suffix'     => t('%'),
    '#element_validate' => array('_cmp_percent_validate'),
  );

  $form['wrapper']['timeout'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Timeout'),
    '#description'      => t('Number of milliseconds to wait before killing the process and assuming webshotting has failed. (0 is no timeout.)'),
    '#default_value'    => (int) $settings['timeout'],
    '#size'             => 5,
    '#maxlength'        => 5,
    '#field_suffix'     => t('ms'),
    '#element_validate' => array('element_validate_integer'),
  );

  $form['wrapper']['windowSize'] = array(
    '#type'         => 'item',
    '#title'        => t('Window Size'),
    '#description'  => t('The dimensions of the browser window. screenSize is an alias for this property.'),
    '#field_prefix' => '<div class="container-inline">',
    '#field_suffix' => '</div>',
  );

  $form['wrapper']['windowSize']['windowSize_width'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Width'),
    '#title_display'    => 'invisible',
    '#default_value'    => (int) $settings['windowSize_width'],
    '#size'             => 5,
    '#maxlength'        => 4,
    '#field_suffix'     => ' x ',
    '#element_validate' => array('element_validate_integer'),
  );

  $form['wrapper']['windowSize']['windowSize_height'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Height'),
    '#title_display'    => 'invisible',
    '#default_value'    => (int) $settings['windowSize_height'],
    '#size'             => 5,
    '#maxlength'        => 4,
    '#field_suffix'     => ' ' . t('pixels'),
    '#element_validate' => array('element_validate_integer'),
  );

  $shotSize_desc = t('The area of the page document, starting at the upper left corner, to render. Possible values are "window", "all", and a number defining a pixel length.');
  $shotSize_desc .= '<br />';
  $shotSize_desc .= t('"window" causes the length to be set to the length of the window (i.e. the shot displays what is initially visible within the browser window). ');
  $shotSize_desc .= '<br />';
  $shotSize_desc .= t('"all" causes the length to be set to the length of the document along the given dimension.');

  $form['wrapper']['shotSize'] = array(
    '#type'        => 'item',
    '#title'       => t('Window Size'),
    '#description' => $shotSize_desc,
  );

  $form['wrapper']['shotSize']['shotSize_width'] = array(
    '#type'          => 'select',
    '#title'         => t('Width'),
    '#options'       => array(
      'window' => 'window',
      'all'    => 'all',
    ),
    '#default_value' => $settings['shotSize_width'],
  );

  $form['wrapper']['shotSize']['shotSize_height'] = array(
    '#type'          => 'select',
    '#title'         => t('Height'),
    '#options'       => array(
      'window' => 'window',
      'all'    => 'all',
    ),
    '#default_value' => $settings['shotSize_height'],
  );

  $form['wrapper']['shotOffset'] = array(
    '#type'        => 'item',
    '#title'       => t('Shot Offset'),
    '#description' => t('The left and top offsets define the upper left corner of the screenshot rectangle. The right and bottom offsets allow pixels to be removed from the shotSize dimensions (e.g. a shotSize height of "all" with a bottom offset of 30 would cause all but the last 30 rows of pixels on the site to be rendered).'),
  );

  $form['wrapper']['shotOffset']['shotOffset_left'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Left'),
    '#default_value'    => (int) $settings['shotOffset_left'],
    '#size'             => 5,
    '#maxlength'        => 5,
    '#element_validate' => array('element_validate_integer'),
  );

  $form['wrapper']['shotOffset']['shotOffset_right'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Right'),
    '#default_value'    => (int) $settings['shotOffset_right'],
    '#size'             => 5,
    '#maxlength'        => 5,
    '#element_validate' => array('element_validate_integer'),
  );

  $form['wrapper']['shotOffset']['shotOffset_top'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Top'),
    '#default_value'    => (int) $settings['shotOffset_top'],
    '#size'             => 5,
    '#maxlength'        => 5,
    '#element_validate' => array('element_validate_integer'),
  );

  $form['wrapper']['shotOffset']['shotOffset_bottom'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Bottom'),
    '#default_value'    => (int) $settings['shotOffset_bottom'],
    '#size'             => 5,
    '#maxlength'        => 5,
    '#element_validate' => array('element_validate_integer'),
  );

  $form['wrapper']['useCache'] = array(
    '#type'          => 'select',
    '#title'         => t('Use Cache'),
    '#description'   => t('Load snapshot from cache or not.'),
    '#options'       => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#default_value' => (int) $settings['useCache'],
  );

  $form['wrapper']['customCSS'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Custom CSS'),
    '#description'   => t('When taking the screenshot, adds custom CSS rules if defined.'),
    '#default_value' => $settings['customCSS'],
  );

  // Add title and alt configuration options.
  $form['wrapper']['alt_field'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable <em>Alt</em> field'),
    '#default_value' => $settings['alt_field'],
    '#description'   => t('The alt attribute may be used by search engines, screen readers, and when the image cannot be loaded.'),
    '#weight'        => 10,
  );

  $form['wrapper']['title_field'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable <em>Title</em> field'),
    '#default_value' => $settings['title_field'],
    '#description'   => t('The title attribute is used as a tooltip when the mouse hovers over the image.'),
    '#weight'        => 11,
  );

  $form['wrapper']['file_directory'] = array(
    '#type'             => 'textfield',
    '#title'            => t('File directory'),
    '#description'      => t('Optional subdirectory within the upload destination where files will be stored. Do not include preceding or trailing slashes.'),
    '#default_value'    => $settings['file_directory'],
    '#element_validate' => array('_page2images_file_directory_validate'),
    '#weight'           => 12,
  );

  $form['wrapper']['default_image'] = array(
    '#type'            => 'managed_file',
    '#title'           => t('Default image'),
    '#description'     => t('If no image, this image will be shown on display.'),
    '#default_value'   => $settings['default_image'],
    '#upload_location' => $settings['uri_scheme'] . '://default_images/',
    '#weight'          => 13,
  );

  return $form;
}

/**
 * Implements hook_field_delete_instance().
 */
function cmp_field_delete_instance($instance) {
  // Only act on image fields.
  $field = field_read_field($instance['field_name']);
  if ($field['type'] != 'cmp') {
    return;
  }

  // The value of a managed_file element can be an array if the #extended
  // property is set to TRUE.
  $fid = $instance['settings']['wrapper']['default_image'];
  if (is_array($fid)) {
    $fid = $fid['fid'];
  }

  // Remove the default image when the instance is deleted.
  if ($fid && ($file = file_load($fid))) {
    file_usage_delete($file, 'cmp', 'default_image', $instance['id']);
  }
}

/**
 * Implements hook_field_update_instance().
 */
function cmp_field_update_instance($instance, $prior_instance) {
  // Only act on image fields.
  $field = field_read_field($instance['field_name']);
  if ($field['type'] != 'cmp') {
    return;
  }

  // The value of a managed_file element can be an array if the #extended
  // property is set to TRUE.
  $fid_new = isset($instance['settings']['wrapper']['default_image']) ? $instance['settings']['wrapper']['default_image'] : $instance['settings']['default_image'];
  if (is_array($fid_new)) {
    $fid_new = $fid_new['fid'];
  }
  $fid_old = isset($prior_instance['settings']['wrapper']['default_image']) ? $prior_instance['settings']['wrapper']['default_image'] : $prior_instance['settings']['default_image'];
  if (is_array($fid_old)) {
    $fid_old = $fid_old['fid'];
  }

  // If the old and new files do not match, update the default accordingly.
  $file_new = $fid_new ? file_load($fid_new) : FALSE;
  if ($fid_new != $fid_old) {
    // Save the new file, if present.
    if ($file_new) {
      $file_new->status = FILE_STATUS_PERMANENT;
      file_save($file_new);
      file_usage_add($file_new, 'cmp', 'default_image', $instance['id']);
    }
    // Delete the old file, if present.
    if ($fid_old && ($file_old = file_load($fid_old))) {
      file_usage_delete($file_old, 'cmp', 'default_image', $instance['id']);
    }
  }

  // If the upload destination changed, then move the file.
  if ($file_new && (file_uri_scheme($file_new->uri) != $field['settings']['uri_scheme'])) {
    $directory = $field['settings']['uri_scheme'] . '://default_images/';
    file_prepare_directory($directory, FILE_CREATE_DIRECTORY);
    file_move($file_new, $directory . $file_new->filename);
  }
}
