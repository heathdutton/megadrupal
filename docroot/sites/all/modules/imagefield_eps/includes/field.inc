<?php
/**
 * @file
 * Mostly a clone of the image module's field image.
 * Implement an image field, based on the image module's image field.
 */

// Define number of colors for image conversion.
define('IMAGEFIELD_EPS_NUM_COLORS', 255);

/**
 * Implements hook_field_info().
 */
function imagefield_eps_field_info() {
  return array(
    'eps' => array(
      'label' => t('EPS To Image'),
      'description' => t('This field stores the ID of an image file as an integer value.'),
      'settings' => array(
        'uri_scheme' => variable_get('file_default_scheme', 'public'),
      ),
      'instance_settings' => array(
        'file_extensions' => 'eps png jpg gif',
        'file_directory' => '',
        'max_filesize' => '',
        'alt_field' => 0,
        'title_field' => 0,
        'max_resolution' => '',
        'min_resolution' => '',
        'default_image' => 0,
      ),
      'default_widget' => 'eps',
      'default_formatter' => 'eps',
      'property_type' => 'field_item_eps',
      'property_callbacks' => array(
        'entity_metadata_field_file_callback',
        'field_item_eps_property_info_callback',
      ),
    ),
  );
}

/**
 * Callback to define metadata about entity properties.
 */
function field_item_eps_property_info_callback(&$info, $entity_type, $field, $instance, $field_type) {
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$field['field_name']];

  $property['property info']['file'] = array(
    'type' => 'file',
    'label' => t('The eps file.'),
    'getter callback' => 'entity_metadata_field_file_get',
    'setter callback' => 'entity_metadata_field_file_set',
    'required' => TRUE,
  );
  $property['property info']['alt'] = array(
    'type' => 'text',
    'label' => t('The "Alt" attribute text'),
    'setter callback' => 'entity_property_verbatim_set',
  );
  $property['property info']['title'] = array(
    'type' => 'text',
    'label' => t('The "Title" attribute text'),
    'setter callback' => 'entity_property_verbatim_set',
  );
  
  if (empty($instance['settings']['alt_field'])) {
    unset($property['property info']['alt']);
  }
  if (empty($instance['settings']['title_field'])) {
    unset($property['property info']['title']);
  }
}

/**
 * Implements hook_field_widget_info().
 */
function imagefield_eps_field_widget_info() {
  return array(
    'eps' => array(
      'label' => t('EPS'),
      'field types' => array('eps'),
      'settings' => array(
        'progress_indicator' => 'throbber',
        'preview_image_style' => 'thumbnail',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_instance_settings_form().
 */
function imagefield_eps_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  // Use the file field instance settings form as a basis.
  $form = file_field_instance_settings_form($field, $instance);

  // Remove the description option.
  unset($form['description_field']);

  // Add title and alt configuration options.
  $form['alt_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable <em>Alt</em> field'),
    '#default_value' => $settings['alt_field'],
    '#description' => t('The alt attribute may be used by search engines, screen readers, and when the image cannot be loaded.'),
    '#weight' => 10,
  );
  $form['title_field'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable <em>Title</em> field'),
    '#default_value' => $settings['title_field'],
    '#description' => t('The title attribute is used as a tooltip when the mouse hovers over the image.'),
    '#weight' => 11,
  );

  return $form;
}

/**
 * Implements hook_field_widget_settings_form().
 */
function imagefield_eps_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  // Use the file widget settings form.
  $form = file_field_widget_settings_form($field, $instance);

  $form['preview_image_style'] = array(
    '#title' => t('Preview image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#empty_option' => '<' . t('no preview') . '>',
    '#default_value' => $settings['preview_image_style'],
    '#description' => t('The preview image will be shown while editing the content.'),
    '#weight' => 15,
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function imagefield_eps_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // Add display_field setting to field because file_field_widget_form()
  // assumes it is set.
  $field['settings']['display_field'] = 0;

  $elements = file_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);

  foreach (element_children($elements) as $delta) {

    // If not using custom extension validation, ensure this is an image.
    $supported_extensions = array('eps', 'png', 'jpg', 'gif');
    $extensions = isset($elements[$delta]['#upload_validators']['file_validate_extensions'][0]) ? $elements[$delta]['#upload_validators']['file_validate_extensions'][0] : implode(' ', $supported_extensions);
    $extensions = array_intersect(explode(' ', $extensions), $supported_extensions);
    $elements[$delta]['#upload_validators']['file_validate_extensions'][0] = implode(' ', $extensions);

    // Add all extra functionality provided by the image widget.
    $elements[$delta]['#process'][] = 'imagefield_eps_field_widget_process';
  }

  if ($field['cardinality'] == 1) {
    // If there's only one field, return it as delta 0.
    if (empty($elements[0]['#default_value']['fid'])) {
      $elements[0]['#description'] = theme('file_upload_help', array('description' => $instance['description'], 'upload_validators' => $elements[0]['#upload_validators']));
    }
  }
  else {
    $elements['#file_upload_description'] = theme('file_upload_help', array('upload_validators' => $elements[0]['#upload_validators']));
  }
  return $elements;
}


/**
 * Perform image conversion.
 */
function imagefield_eps_image_conversion($uri, &$file_uri, &$filename = NULL) {
  $ext = drupal_substr($uri, -3);
  if (preg_match('/EPS/i', $ext)) {
    $source = drupal_realpath($uri);
    $target = file_uri_target($uri);
    $dir = explode('/', $target);
    unset($dir[count($dir) - 1]);
    $dir = file_build_uri('imagefield_eps_images/' . implode('/', $dir));
    if (!is_dir(drupal_realpath($dir))) {
      drupal_mkdir($dir, 0777, TRUE);
    }

    $filename = str_replace(array('.eps', '.EPS'), '.png', $target);
    $file_uri = file_build_uri('imagefield_eps_images') . '/' . $filename;
    $destination = drupal_realpath($file_uri);

    if (!file_exists($destination)) {
      $profiles = libraries_get_path('colorprofiles') . '/';
      $cmyk = '-profile "' . drupal_realpath($profiles . 'CMYK/USWebUncoated.icc') . '"';
      $rgb = '-profile "' . drupal_realpath($profiles . 'RGB/AdobeRGB1998.icc') . '"';
      $script = "convert " . escapeshellarg($source) . " {$cmyk} {$rgb} -quality 100% -colors ";
      $script .= IMAGEFIELD_EPS_NUM_COLORS;
      $script .= " -colorspace RGB PNG32:" . escapeshellarg($destination);
      exec($script);
    }
  }
}

/**
 * An element #process callback for the eps field type.
 *
 * Expands the eps type to include the alt and title fields.
 */
function imagefield_eps_field_widget_process($element, &$form_state, $form) {
  $item = $element['#value'];
  $item['fid'] = $element['fid']['#value'];

  $instance = field_widget_instance($element, $form_state);

  $settings = $instance['settings'];
  $widget_settings = $instance['widget']['settings'];

  $element['#theme'] = 'image_widget';
  $element['#attached']['css'][] = drupal_get_path('module', 'image') . '/image.css';

  // Add the image preview.
  if ($element['#file'] && $widget_settings['preview_image_style']) {
    $file_uri = NULL;
    // Configure redirect from eps to image if necessary.
    imagefield_eps_image_conversion($element['#file']->uri, $file_uri);

    if ($file_uri) {
      $variables = array(
        'style_name' => $widget_settings['preview_image_style'],
        'path' => $file_uri,
      );
    }
    else {
      $variables = array(
        'style_name' => $widget_settings['preview_image_style'],
        'path' => $element['#file']->uri,
      );
    }

    // Determine image dimensions.
    if (isset($element['#value']['width']) && isset($element['#value']['height'])) {
      $variables['width'] = $element['#value']['width'];
      $variables['height'] = $element['#value']['height'];
    }
    else {
      $info = image_get_info($element['#file']->uri);

      if (is_array($info)) {
        $variables['width'] = $info['width'];
        $variables['height'] = $info['height'];
      }
      else {
        $variables['width'] = $variables['height'] = NULL;
      }
    }

    $element['preview'] = array(
      '#type' => 'markup',
      '#markup' => theme('image_style', $variables),
    );

    // Store the dimensions in the form so the file doesn't have to be accessed
    // again. This is important for remote files.
    $element['width'] = array(
      '#type' => 'hidden',
      '#value' => $variables['width'],
    );
    $element['height'] = array(
      '#type' => 'hidden',
      '#value' => $variables['height'],
    );
  }

  // Add the additional alt and title fields.
  $element['alt'] = array(
    '#title' => t('Alternate text'),
    '#type' => 'textfield',
    '#default_value' => isset($item['alt']) ? $item['alt'] : '',
    '#description' => t('This text will be used by screen readers, search engines, or when the image cannot be loaded.'),
    // @see http://www.gawds.org/show.php?contentid=28
    '#maxlength' => 512,
    '#weight' => -4,
    '#access' => (bool) $item['fid'] && $settings['alt_field'],
  );
  $element['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($item['title']) ? $item['title'] : '',
    '#description' => t('The title is used as a tool tip when the user hovers the mouse over the image.'),
    '#maxlength' => 1024,
    '#weight' => -3,
    '#access' => (bool) $item['fid'] && $settings['title_field'],
  );

  return $element;
}


/**
 * Implements hook_field_settings_form().
 */
function imagefield_eps_field_settings_form($field, $instance) {
  $defaults = field_info_field_settings($field['type']);
  $settings = array_merge($defaults, $field['settings']);

  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form['uri_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $scheme_options,
    '#default_value' => $settings['uri_scheme'],
    '#description' => t('Select where the final files should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
  );

  return $form;
}

/**
 * Implements hook_field_load().
 */
function imagefield_eps_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  file_field_load($entity_type, $entities, $field, $instances, $langcode, $items, $age);
}

/**
 * Implements hook_field_prepare_view().
 */
function imagefield_eps_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  // If there are no files specified at all, use the default.
  foreach ($entities as $id => $entity) {
    if (empty($items[$id])) {
      $fid = 0;
      // Use the default for the instance if one is available.
      if (!empty($instances[$id]['settings']['default_image'])) {
        $fid = $instances[$id]['settings']['default_image'];
      }
      // Otherwise, use the default for the field.
      elseif (!empty($field['settings']['default_image'])) {
        $fid = $field['settings']['default_image'];
      }

      // Add the default image if one is found.
      if ($fid && ($file = file_load($fid))) {
        $items[$id][0] = (array) $file + array(
          'is_default' => TRUE,
          'alt' => '',
          'title' => '',
        );
      }
    }
  }

  foreach ($items as $key => $value) {
    foreach ($value as $index => $file) {
      $file_uri = NULL;
      $filename = NULL;
      imagefield_eps_image_conversion($file['uri'], $file_uri, $filename);
      if ($file_uri && $filename) {
        $items[$key][$index]['filename'] = $filename;
        $items[$key][$index]['uri'] = $file_uri;
      }
    }
  }
}

/**
 * Implements hook_field_presave().
 */
function imagefield_eps_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_presave($entity_type, $entity, $field, $instance, $langcode, $items);

  // Determine the dimensions if necessary.
  foreach ($items as &$item) {
    $loadfile = file_load($item['fid']);
    if (strlen($item['width']) == 0 || strlen($item['height']) == 0) {
      $file_uri = NULL;
      // Configure redirect from eps to image if necessary.
      imagefield_eps_image_conversion($loadfile->uri, $file_uri);

      if ($file_uri) {
        $info = image_get_info($file_uri);
      }
      else {
        $info = image_get_info(file_load($item['fid'])->uri);
      }

      if (is_array($info)) {
        $item['width'] = $info['width'];
        $item['height'] = $info['height'];
      }
    }
  }
}

/**
 * Implements hook_field_insert().
 */
function imagefield_eps_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_insert($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_update().
 */
function imagefield_eps_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_update($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_delete().
 */
function imagefield_eps_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_delete($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_delete_revision().
 */
function imagefield_eps_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, &$items) {
  file_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_is_empty().
 */
function imagefield_eps_field_is_empty($item, $field) {
  return file_field_is_empty($item, $field);
}

/**
 * Implements hook_field_formatter_info().
 */
function imagefield_eps_field_formatter_info() {
  $formatters = array(
    'eps' => array(
      'label' => t('EPS Image'),
      'field types' => array('eps'),
      'settings' => array('image_style' => '', 'image_link' => ''),
    ),
  );

  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function imagefield_eps_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $image_styles = image_style_options(FALSE);
  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
    '#options' => $image_styles,
  );

  $link_types = array(
    'content' => t('Content'),
    'file' => t('File'),
  );
  $element['image_link'] = array(
    '#title' => t('Link image to'),
    '#type' => 'select',
    '#default_value' => $settings['image_link'],
    '#empty_option' => t('Nothing'),
    '#options' => $link_types,
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function imagefield_eps_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  $image_styles = image_style_options(FALSE);
  // Unset possible 'No defined styles' option.
  unset($image_styles['']);
  // Styles could be lost because of enabled/disabled modules that defines
  // their styles in code.
  if (isset($image_styles[$settings['image_style']])) {
    $summary[] = t('Image style: @style', array('@style' => $image_styles[$settings['image_style']]));
  }
  else {
    $summary[] = t('Original image');
  }

  $link_types = array(
    'content' => t('Linked to content'),
    'file' => t('Linked to file'),
  );
  // Display this setting only if image is linked.
  if (isset($link_types[$settings['image_link']])) {
    $summary[] = $link_types[$settings['image_link']];
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function imagefield_eps_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  // Check if the formatter involves a link.
  if ($display['settings']['image_link'] == 'content') {
    $uri = entity_uri($entity_type, $entity);
  }
  elseif ($display['settings']['image_link'] == 'file') {
    $link_file = TRUE;
  }

  foreach ($items as $delta => $item) {
    if (isset($link_file)) {
      $uri = array(
        'path' => file_create_url($item['uri']),
        'options' => array(),
      );
    }
    $element[$delta] = array(
      '#theme' => 'image_formatter',
      '#item' => $item,
      '#image_style' => $display['settings']['image_style'],
      '#path' => isset($uri) ? $uri : '',
    );
  }

  return $element;
}
