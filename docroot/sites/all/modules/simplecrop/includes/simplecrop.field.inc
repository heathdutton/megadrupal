<?php

/**
 * @file
 * Contains information about field widget and related functions.
 */

/**
 * Implements hook_field_widget_info().
 */
function simplecrop_field_widget_info() {
  return array(
    'simplecrop_widget' => array(
      'label' => t('SimpleCrop'),
      'field types' => array('image'),
      'settings' => array(

        // Keep the default image settings.
        'progress_indicator' => 'throbber',
        'preview_image_style' => FALSE,

        // Simplecrop settings.
        'simplecrop' => array(
          'upload_display' => SIMPLECROP_DISPLAY_ORIGINAL_IMAGE,
          'initial_display' => SIMPLECROP_DISPLAY_CROPPED_IMAGE,
          'hide_filename' => TRUE,
          'source' => array(
            'scale' => array('width' => 350, 'height' => 350),
          ),
          'cropped' => array(
            'scale' => array('width' => 100, 'height' => 100),
          ),
          'crop' => array(
            'ratio' => array('width' => '', 'height' => ''),
            'min_area' => array('width' => '', 'height' => ''),
            'max_area' => array('width' => '', 'height' => ''),
            'initial_area' => SIMPLECROP_CROP_AREA_MAXIMIZE,
          ),
        ),
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    )
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function simplecrop_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  // Use the image widget settings form.
  $form = image_field_widget_settings_form($field, $instance);

  // Disable preview functionality that comes from image widget,
  // but keep it in form to avoid php notices that element is missing.
  $form['preview_image_style']['#default_value'] = FALSE;
  $form['preview_image_style']['#access'] = FALSE;

  $form['simplecrop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Simplecrop settings'),
    '#weight' => 15,
  );

  $form['simplecrop']['upload_display'] = array(
    '#type' => 'select',
    '#title' => t('What user should see after he uploaded new image?'),
    '#options' => array(
      SIMPLECROP_DISPLAY_ORIGINAL_IMAGE => t('Original image with crop selection'),
      SIMPLECROP_DISPLAY_CROPPED_IMAGE => t('Cropped image')
    ),
    '#default_value' => $settings['simplecrop']['upload_display'],
    '#description' => t('Choose what should be displayed first right after new image was uploaded.'),
  );

  $form['simplecrop']['initial_display'] = array(
    '#type' => 'select',
    '#title' => t('What user should see initially when opens edit image form?'),
    '#options' => array(
      SIMPLECROP_DISPLAY_ORIGINAL_IMAGE => t('Original image with crop selection'),
      SIMPLECROP_DISPLAY_CROPPED_IMAGE => t('Cropped image')
    ),
    '#default_value' => $settings['simplecrop']['initial_display'],
    '#description' => t('Choose what should be displayed when user opens content edit form with simplecrop image.'),
  );

  $form['simplecrop']['hide_filename'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide filename'),
    '#description' => t('Remove info about file name with its size from widget display.'),
    '#default_value' => $settings['simplecrop']['hide_filename'],
  );

  // Settings of source image and cropping area.
  $source_settings = $settings['simplecrop']['source'];
  $form['simplecrop']['source'] = array(
    '#type' => 'fieldset',
    '#title' => t('Original image settings'),
    '#description' => t('Settings of uploaded image where user selects cropping area.'),
  );

  $form['simplecrop']['source']['scale'] = array(
    '#type' => 'item',
    '#title' => t('Maximum resolution'),
    '#field_prefix' => '<div class="container-inline">',
    '#field_suffix' => '</div>',
    '#description' => t('Display of image resolution will be downscaled to the following values if uploaded image height or width is bigger that this value.'),
  );

  $form['simplecrop']['source']['scale']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Source image scale width'),
    '#title_display' => 'invisible',
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $source_settings['scale']['width'],
    '#required' => TRUE,
    '#size' => 5,
    '#maxlength' => 5,
    '#field_suffix' => ' x ',
  );

  $form['simplecrop']['source']['scale']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Source image scale height'),
    '#title_display' => 'invisible',
    '#field_suffix' => t('pixels'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $source_settings['scale']['height'],
    '#required' => TRUE,
    '#size' => 5,
    '#maxlength' => 5,
  );

  // Cropped image settings.
  $cropped_settings = !empty($settings['simplecrop']['cropped']) ? $settings['simplecrop']['cropped'] : array();
  $form['simplecrop']['cropped'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cropped image settings'),
    '#description' => t('Settings of cropped image.'),
  );

  $form['simplecrop']['cropped']['scale'] = array(
    '#type' => 'item',
    '#title' => t('Maximum resolution'),
    '#field_prefix' => '<div class="container-inline">',
    '#field_suffix' => '</div>',
    '#description' => t('Maximum resolution of cropped image display. Resolution will be downscaled to the following values if crop area height or width is bigger that this value.'),
  );

  $form['simplecrop']['cropped']['scale']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Source image scale width'),
    '#title_display' => 'invisible',
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $cropped_settings['scale']['width'],
    '#required' => TRUE,
    '#size' => 5,
    '#maxlength' => 5,
    '#field_suffix' => ' x ',
  );

  $form['simplecrop']['cropped']['scale']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Source image scale height'),
    '#title_display' => 'invisible',
    '#field_suffix' => t('pixels'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $cropped_settings['scale']['height'],
    '#required' => TRUE,
    '#size' => 5,
    '#maxlength' => 5,
  );

  // Settings of crop area.
  $crop_settings = $settings['simplecrop']['crop'];
  $form['simplecrop']['crop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Crop area settings'),
  );

  $form['simplecrop']['crop']['ratio'] = array(
    '#type' => 'item',
    '#title' => t('Aspect ratio'),
    '#field_prefix' => '<div class="container-inline">',
    '#field_suffix' => '</div>',
    '#description' => t('Aspect ratio of cropping area. Leave blank to omit fixed ratio. <br/> Example: "3 : 1". That means that width of cropping area must be 3 times bigger than height.'),
  );

  $form['simplecrop']['crop']['ratio']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Aspect ratio width'),
    '#title_display' => 'invisible',
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $crop_settings['ratio']['width'],
    '#size' => 2,
    '#maxlength' => 2,
    '#field_suffix' => ':',
  );

  $form['simplecrop']['crop']['ratio']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Aspect ratio height'),
    '#title_display' => 'invisible',
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => $crop_settings['ratio']['height'],
    '#size' => 2,
    '#maxlength' => 2,
  );

  $areas = array('min_area' => 'Minimum', 'max_area' => 'Maximum');
  foreach ($areas as $area => $name) {
    $form['simplecrop']['crop'][$area] = array(
      '#type' => 'item',
      '#title' => t('@area resolution of crop area', array('@area' => $name)),
      '#field_prefix' => '<div class="container-inline">',
      '#field_suffix' => '</div>',
      '#description' => t('@area width and height of crop area that user may select on image. Leave blank to skip this setting.', array('@area' => $name)) . '<br/>' .
        t('Important note: this limits apply to original image resolution. That means that if you original image will be scaled - these limits also will be scaled.'),
    );

    $form['simplecrop']['crop'][$area]['width'] = array(
      '#type' => 'textfield',
      '#title' => t('@area crop area width', array('@area' => $name)),
      '#title_display' => 'invisible',
      '#element_validate' => array('element_validate_integer_positive'),
      '#default_value' => $crop_settings[$area]['width'],
      '#size' => 5,
      '#maxlength' => 5,
      '#field_suffix' => ' x ',
    );

    $form['simplecrop']['crop'][$area]['height'] = array(
      '#type' => 'textfield',
      '#title' => t('@area crop area height', array('@area' => $name)),
      '#title_display' => 'invisible',
      '#element_validate' => array('element_validate_integer_positive'),
      '#default_value' => $crop_settings[$area]['height'],
      '#size' => 5,
      '#maxlength' => 5,
      '#field_suffix' => t('pixels')
    );
  }

  $form['simplecrop']['crop']['initial_area'] = array(
    '#type' => 'select',
    '#title' => t('Initial crop area'),
    '#options' => array(
      SIMPLECROP_CROP_AREA_MINIMIZE => t('Minimize'),
      SIMPLECROP_CROP_AREA_MAXIMIZE => t('Maximize'),
      SIMPLECROP_CROP_AREA_NONE => t('Not selected'),
    ),
    '#default_value' => $crop_settings['initial_area'],
    '#description' => theme('item_list', array('items' => array(
      t('Minimize - if minimum resolution of crop area is defined - then after upload image will have this area selected. Otherwise no area selection.'),
      t('Maximize - if maximum resolution of crop area is defined - then after upload image will have this area selected. Othewise complete image will be selected.'),
      t('Not selected - no default selection of uploaded image.'),
    ))),
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function simplecrop_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  // First keep default image widget form.
  $elements = image_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);

  // Then add new process function to the image field
  // to support our custom crop behavior.
  foreach (element_children($elements) as $delta) {
    $elements[$delta]['#process'][] = 'simplecrop_field_widget_process';
  }

  return $elements;
}

/**
 * Element #process callback for the image_image field type.
 *
 * Expands the image_image type to include the crop behavior.
 */
function simplecrop_field_widget_process($element, &$form_state, $form) {

  // Get field instance settings.
  $instance = field_widget_instance($element, $form_state);
  $settings = $instance['widget']['settings']['simplecrop'];

  if (!empty($element['#file'])) {

    // Get file properties for better usability.
    $uri = $element['#file']->uri;
    $fid = $element['#file']->fid;

    // Ensure that image exists. Otherwise doesn't make sense to continue.
    if (!file_exists($uri)) {
      watchdog('simplecrop', 'Image path %uri does not exists. Simplecrop widget process has been aborted.', array('%uri' => $uri), WATCHDOG_WARNING);
      return $element;
    }

    // Check if current image was initially loaded in the edit form.
    // If so - display widget according to configuration of the initial display setting.
    if (empty($form_state['simplecrop'][$fid]['display'])) {
      $form_state['simplecrop'][$fid]['display'] = $settings['initial_display'];
    }

    // Get current display of an image.
    $display = $form_state['simplecrop'][$fid]['display'];

    // Store parents of image to be able to fetch them later by parents array.
    $form_state['simplecrop'][$fid]['parents'] = $element['#parents'];

    // Get a current image info and keep its original resolution.
    $image_info = image_get_info($uri);

    // Make sure that image toolkit is installed on a server and available.
    // Otherwise no possibility to continue widget process.
    if (empty($image_info)) {
      watchdog('simplecrop', 'No image toolkit installed. Simplecrop widget process has been aborted.', array(), WATCHDOG_ERROR);
      return $element;
    }

    $original_width = $image_info['width'];
    $original_height = $image_info['height'];

    // If current form state already has info about this image crop,
    // then use that data.
    if (!empty($form_state['simplecrop'][$fid]['crop'])) {
      $crop_data = $form_state['simplecrop'][$fid]['crop'];
    }
    // Otherwise try to retrive crop info from database.
    elseif ($crop = simplecrop_crop_load($uri)) {
      $form_state['simplecrop'][$fid]['crop'] = $crop_data = $crop->data;
    }

    $cropped_area = array();
    if (!empty($crop_data)) {

      // Define width and height of cropped area.
      $cropped_area['width']  = abs($crop_data['x'] - $crop_data['x2']);
      $cropped_area['height'] = abs($crop_data['y'] - $crop_data['y2']);
    }
    else {

      // No crop area, means that we keep original image resolution.
      $cropped_area['width'] = $original_width;
      $cropped_area['height'] = $original_height;
    }

    $cropped_area_original_width = $cropped_area['width'];

    // Downscale resolution of cropped area to width and height from settings.
    image_dimensions_scale($cropped_area, $settings['cropped']['scale']['width'], $settings['cropped']['scale']['height']);

    // Calculate difference between downscaled cropped area resolution and its original resolution.
    $scale_ratio = $cropped_area['width'] / $cropped_area_original_width;

    // Calculate resolution of cropped image that we need to display.
    // Scale ratio shows how much we need to downscale original resolution.
    $cropped_image_width = round($original_width * $scale_ratio);
    $cropped_image_height = round($original_height * $scale_ratio);

    $container_style = array(
      'width:' . $cropped_area['width'] . 'px;',
      'height:' . $cropped_area['height'] . 'px;',
    );

    // Calculate offset of crop area.
    $position_left = !empty($crop_data['x']) ? round($crop_data['x'] * $scale_ratio) : 0;
    $position_top = !empty($crop_data['y']) ? round($crop_data['y'] * $scale_ratio) : 0;

    // Set css offsets for cropped image, so for the user will be visible
    // only cropped area.
    $cropped_image_style = array(
      'left:' . -$position_left . 'px;',
      'top:' . -$position_top . 'px;',
    );

    // Container for cropped image.
    // Needed here to display only cropped area of image.
    $element['cropped_image'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'style' => !empty($container_style) ? $container_style : array(),
      ),
    );

    // Show cropped image.
    $element['cropped_image']['image'] = array(
      '#theme' => 'image',
      '#width' => $cropped_image_width,
      '#height' => $cropped_image_height,
      '#attributes' => array(
        'style' => !empty($cropped_image_style) ? $cropped_image_style : array(),
      ),
      '#path' => $uri,
    );

    // If settings has scale behavior, then we need to downscale current image.
    if (!empty($settings['source']['scale']['width']) || !empty($settings['source']['scale']['height'])) {
      image_dimensions_scale($image_info, $settings['source']['scale']['width'], $settings['source']['scale']['height']);
    }

    // Display downscaled original image.
    // To this images we apply jCrop library.
    $element['source_image'] = array(
      '#theme' => 'image',
      '#width' => $image_info['width'],
      '#height' => $image_info['height'],
      '#path' => $uri,
    );

    // Adjust the Ajax settings so that on upload and remove of any individual
    // file, the entire group of file fields is updated together.
    $field = field_widget_field($element, $form_state);
    if ($field['cardinality'] != 1) {
      $parents = array_slice($element['#array_parents'], 0, -1);
      $path = 'simplecrop/ajax/' . implode('/', $parents) . '/' . $form['form_build_id']['#value'];
      $field_element = drupal_array_get_nested_value($form, $parents);
      $wrapper = $field_element['#id'] . '-ajax-wrapper';
    }
    else {
      $path = 'simplecrop/ajax/' . implode('/', $element['#parents']) . '/' . $form['form_build_id']['#value'];
      $wrapper = $element['upload_button']['#ajax']['wrapper'];
    }

    // Button that applies crop area and show cropped image.
    $element['display_reload'] = array(
      '#name' => implode('_', $element['#parents']) . '_display_reload',
      '#type' => 'submit',
      '#validate' => array(),
      '#submit' => array('simplecrop_field_widget_change_display_submit'),
      '#limit_validation_errors' => array($element['#parents']),
      '#ajax' => array(
        'path' => $path,
        'wrapper' => $wrapper,
      ),
    );

    // Keep original size for later coordinates calculations.
    $element['original_size'] = array(
      '#type' => 'value',
      '#value' => array('width' => $original_width, 'height' => $original_height),
    );

    // Keep original size for later coordinates calculations.
    $element['scaled_size'] = array(
      '#type' => 'value',
      '#value' => array('width' => $image_info['width'], 'height' => $image_info['height']),
    );

    // We need to keep ratio between original image and scaled image.
    // We can calculate it using height or width values.
    $scale_ratio = (float) number_format($original_width / $image_info['width'], 3, '.', '');

    // We need to rescale min and max resolutions according to a new
    // scale ratio
    foreach (array('min_area', 'max_area') as $area) {
      foreach (array('width', 'height') as $side) {
        if (!empty($settings['crop'][$area][$side])) {
          $settings['crop'][$area][$side] = round($settings['crop'][$area][$side] / $scale_ratio);
        }
      }
    }

    // Collect js settings for image. They'll be used for jCrop processment.
    $js_settings = array();

    // Pick cropped area as a default selection area if user already defined his crop.
    if (!empty($crop_data)) {

      // We need to rescale coordinates according to a new scale ratio.
      foreach (array('x', 'y', 'x2', 'y2') as $name) {
        $crop_data[$name] = round($crop_data[$name] / $scale_ratio);
      }
      $js_settings['initial_area'] = $crop_data;
    }
    // Calculate initial crop area if user didn't select his crop yet.
    else {
      $js_settings['initial_area'] = _simplecrop_define_initial_crop_area($image_info['width'], $image_info['height'], $settings['crop']);
    }

    // Add resolution of scaled image to js settings.
    $js_settings['resolution'] = array('width' => $image_info['width'], 'height' => $image_info['height']);

    // Add aspect ratio to a settings (if exists).
    if (!empty($settings['crop']['ratio']['width']) && !empty($settings['crop']['ratio']['height'])) {
      $js_settings['ratio'] = $settings['crop']['ratio']['width'] / $settings['crop']['ratio']['height'];
    }

    // Add settings for min and max crop area resolutions.
    foreach (array('min_area', 'max_area') as $area) {
      if (!empty($settings['crop'][$area]['width']) && !empty($settings['crop'][$area]['height'])) {
        $js_settings[$area] = array($settings['crop'][$area]['width'], $settings['crop'][$area]['height']);
      }
    }

    // Add js with image settings for jCrop plugin.
    // Avoid duplicated additions to js scope.
    $added_js = &drupal_static('simplecrop_added_js_settings');
    if (empty($added_js[$fid])) {
      drupal_add_js(array('simpleCrop' => array('images' => array(array('fid' => $fid, 'settings' => $js_settings)))), 'setting');
      $added_js[$fid] = TRUE;
    }

    // Prepopulate default values for coordinate fields.
    foreach (array('x', 'y', 'x2', 'y2') as $name) {
      $element['data'][$name] = array(
        '#type' => 'hidden',
        '#default_value' => !empty($js_settings['initial_area'][$name]) ? $js_settings['initial_area'][$name] : 0,
        '#attributes' => array('class' => array($name)),
      );
    }

    // Hide filename if admin wants so.
    if (!empty($settings['hide_filename']) && !empty($element['filename'])) {
      $element['filename']['#access'] = FALSE;
    }

    // Show and hide different widget fields depends on current display.
    _simplecrop_field_widget_switch_display($element, $display);

    // Theme widget using our internal theme function.
    $element['#theme'] = 'simplecrop_widget';

    // Add js and css for cropping.
    $element['#attached']['js'][]  = drupal_get_path('module', 'simplecrop') . '/jquery.jcrop/js/jquery.Jcrop.min.js';
    $element['#attached']['css'][] = drupal_get_path('module', 'simplecrop') . '/jquery.jcrop/css/jquery.Jcrop.min.css';
    $element['#attached']['js'][]  = drupal_get_path('module', 'simplecrop') . '/js/simplecrop.js';
    $element['#attached']['css'][] = drupal_get_path('module', 'simplecrop') . '/css/simplecrop.css';
  }

  // Add another submit handler to upload button.
  // We need this to store initial widget display after new image upload
  // in forms cache storage.
  if (!empty($element['upload_button'])) {
    $element['upload_button']['#submit'][] = 'simplecrop_file_field_submit';
  }

  return $element;
}

/**
 * Crop submit callback.
 * Switches between displays and saves crop data.
 */
function simplecrop_field_widget_change_display_submit(&$form, &$form_state) {

  // Get parents array of triggered button
  $image_parents = $form_state['triggering_element']['#parents'];

  // Remove name of triggered button.
  array_pop($image_parents);

  // Get image field value.
  $value = drupal_array_get_nested_value($form_state['values'], $image_parents);

  // Switch current display.
  $display = &$form_state['simplecrop'][$value['fid']]['display'];
  $display = $display == SIMPLECROP_DISPLAY_ORIGINAL_IMAGE ? SIMPLECROP_DISPLAY_CROPPED_IMAGE : SIMPLECROP_DISPLAY_ORIGINAL_IMAGE;

  // Do some actions after switching from image with crop selection
  // to cropped image display.
  if ($display == SIMPLECROP_DISPLAY_CROPPED_IMAGE) {

    // It makes sense to rebuild crop area coordinates only when
    // we switched from original display, because other display
    // doesn't contain crop area selection.

    // Ensure that we have all required data to save the crop.
    if (!empty($value['data']) && !empty($value['scaled_size']) && !empty($value['original_size']) && !empty($value['fid'])) {

      $crop = &$form_state['simplecrop'][$value['fid']]['crop'];

      // Unscale coordinates from scaled image to original.
      $crop['x']  = round($value['original_size']['width']  / $value['scaled_size']['width']  * $value['data']['x']);
      $crop['y']  = round($value['original_size']['height'] / $value['scaled_size']['height'] * $value['data']['y']);
      $crop['x2'] = round($value['original_size']['width']  / $value['scaled_size']['width']  * $value['data']['x2']);
      $crop['y2'] = round($value['original_size']['height'] / $value['scaled_size']['height'] * $value['data']['y2']);
    }
  }

  // Show and hide different widget fields depends on current display.
  // We need this to store actual data about displays in forms cache.
  $field = &drupal_array_get_nested_value($form, $image_parents);
  _simplecrop_field_widget_switch_display($field, $display);

  // Rebuild image widget with a new display.
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit callback for image upload button.
 * We need this to save current widget display in forms cache.
 */
function simplecrop_file_field_submit($form, &$form_state) {

  $image_parents = $form_state['triggering_element']['#parents'];

  // Remove name of triggered button.
  array_pop($image_parents);

  // Get image field value.
  $value = drupal_array_get_nested_value($form_state['values'], $image_parents);

  // If image was not uploaded, then we shouldn't switch the display.
  if (empty($value)) {
    return;
  }

  $field = &drupal_array_get_nested_value($form, $image_parents);

  // Get settings for this field instance.
  $instance = field_widget_instance($field, $form_state);
  $settings = $instance['widget']['settings']['simplecrop'];

  // Save value of current widget display.
  $form_state['simplecrop'][$value['fid']]['display'] = $settings['upload_display'];

  // Show and hide different widget fields depends on current display.
  // We need this to store actual data about displays in forms cache.
  _simplecrop_field_widget_switch_display($field, $settings['upload_display']);
}

/**
 * Implements hook_field_attached_submit().
 */
function simplecrop_field_attach_submit($entity_type, $entity, $form, &$form_state) {

  // If form doesn't have simplecrop widgets - nothing to process.
  if (empty($form_state['simplecrop'])) {
    return;
  }

  // Save image crops to the database.
  foreach ($form_state['simplecrop'] as $image) {

    // Ensure that submitted form contains image field processed
    // by simplecrop widget.
    if (drupal_array_get_nested_value($form, $image['parents'])) {

      // Load submitted image values.
      $value = drupal_array_get_nested_value($form_state['values'], $image['parents']);
      _simplecrop_save_crop_from_values($value);
    }
  }
}

/**
 * Saves submitted image crop value to the database.
 */
function _simplecrop_save_crop_from_values($value) {

  // If image for some unknown reasons doesn't have file ID,
  // then we can't do nothing here to process a submit request.
  if (empty($value['fid'])) {
    return;
  }

  // Ensure that we have all required data to save the crop.
  if (!empty($value['data']) && !empty($value['scaled_size']) && !empty($value['original_size'])) {

    // Unscale coordinates from scaled image to original.
    $x  = round($value['original_size']['width']  / $value['scaled_size']['width']  * $value['data']['x']);
    $y  = round($value['original_size']['height'] / $value['scaled_size']['height'] * $value['data']['y']);
    $x2 = round($value['original_size']['width']  / $value['scaled_size']['width']  * $value['data']['x2']);
    $y2 = round($value['original_size']['height'] / $value['scaled_size']['height'] * $value['data']['y2']);

    // Save crop data for each image.
    $file = file_load($value['fid']);
    simplecrop_crop_save($file->uri, array('x' => $x, 'y' => $y, 'x2' => $x2, 'y2' => $y2));

    // Flush image styles for this path to regenerate image.
    image_path_flush($file->uri);
  }
}

/**
 * Calculates initial area of crop selection.
 *
 * @param $width
 *   Width of displayed image.
 *
 * @param $height
 *   Height of displayed image.
 *
 * @param $settings
 *   Field instance settings for initial crop area.
 *
 * @return array|bool
 *   Initial area or FALSE if no initial area.
 */
function _simplecrop_define_initial_crop_area($width, $height, $settings) {

  // If crop area should be as big as possible.
  if ($settings['initial_area'] == SIMPLECROP_CROP_AREA_MAXIMIZE) {

    // If we don't have maximum area defined in settings - then use image
    // resolution as initial area.
    if (empty($settings['max_area']['width']) || empty($settings['max_area']['height'])) {
      $area_width = $width;
      $area_height = $height;
    }
    // If maximum area is defined - we need to take the leaser values from it to
    // keep the crop area inside of an image resolution.
    else {
      $area_width = $settings['max_area']['width'] < $width ? $settings['max_area']['width'] : $width;
      $area_height = $settings['max_area']['height'] < $height ? $settings['max_area']['height'] : $height;
    }
  }
  // If crop area should be as small as possible.
  elseif ($settings['initial_area'] == SIMPLECROP_CROP_AREA_MINIMIZE) {

    // If we don't have minimal area defined in settings - then no initial area.
    if (empty($settings['min_area']['width']) || empty($settings['min_area']['height'])) {
      return FALSE;
    }
    // If minimal area is defined - we need to take the leaser values from it to
    // keep the crop area as minimal as possible.
    else {
      $area_width = $settings['min_area']['width'] < $width ? $settings['min_area']['width'] : $width;
      $area_height = $settings['min_area']['height'] < $height ? $settings['min_area']['height'] : $height;
    }
  }
  // If no default crop area needed - then do not calculate this area.
  else {
    return FALSE;
  }

  // Now we have initial area width and height, but we need also to
  // match it to selected aspect ratio.
  // @TODO: possible issue when new height or width is less/more than required.
  if (!empty($settings['ratio']['width']) && !empty($settings['ratio']['height'])) {

    $aspect_ratio = $settings['ratio']['width'] / $settings['ratio']['height'];
    $image_ratio = $area_width / $area_height;

    // Width is too big.
    if ($aspect_ratio < $image_ratio) {
      $area_width = round($area_width * $aspect_ratio / $image_ratio);
    }
    // Height is too big.
    elseif ($aspect_ratio > $image_ratio) {
      $area_height = round($area_height * $image_ratio / $aspect_ratio);
    }
  }

  // Now we need to calculate an initial position of initial area.
  $x = ($width - $area_width) / 2;
  $x2 = $x + $area_width;
  $y = ($height - $area_height) / 2;
  $y2 = $y + $area_height;

  return array('x' => $x, 'y' => $y, 'x2' => $x2, 'y2' => $y2);
}

/**
 * Shows and hides several image field widgets depends on current display.
 */
function _simplecrop_field_widget_switch_display(&$element, $display) {
  $crop_display = $display == SIMPLECROP_DISPLAY_CROPPED_IMAGE;
  $element['cropped_image']['#access'] = $crop_display ? TRUE : FALSE;
  $element['source_image']['#access'] = $crop_display ? FALSE : TRUE;
  $element['data']['#access'] = $crop_display ? FALSE : TRUE;
  $element['display_reload']['#value'] = $crop_display ? t('Edit crop') : t('Apply crop');
}

/**
 * Preprocess function for simplecrop widget template.
 */
function template_preprocess_simplecrop_widget(&$vars) {
  $element = &$vars['element'];

  // Add default image widget classes.
  $vars['classes_array'][] = 'form-managed-file';
  $vars['classes_array'][] = 'clearfix';

  // Keep default image display of file size.
  if ($element['fid']['#value'] != 0) {
    $element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
  }

  // Added unique class to each image widget.
  if (!empty($element['#file']->fid)) {
    $vars['classes_array'][] = 'image-' . $element['#file']->fid;
  }
}
