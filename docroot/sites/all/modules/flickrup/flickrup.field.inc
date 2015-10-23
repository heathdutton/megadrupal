<?php
/**
 * @file
 * Defines a FlickrUp field type. Code to using Flickr photos as a field.
 */

/**
 * Implements hook_field_info().
 */
function flickrup_field_info() {
  $sizes = array_keys(flickrup_photo_sizes());

  return array(
    'flickrupfield' => array(
      'label' => t('Flickr photo (FlickrUp)'),
      'description' => t('Uploads photos on the Flickr service and uses Flickr photos as a field.'),
      'instance_settings' => array(
        'photo_info_enabled' => 1,
      ),
      'default_widget' => 'flickrupfield',
      'default_formatter' => $sizes[0],
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function flickrup_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  $form['photo_info_enabled'] = array(
    '#type' => 'select',
    '#title' => t('Title and description of the image:'),
    '#default_value' => $settings['photo_info_enabled'],
    '#options' => array(
      1 => t('enabled'),
      0 => t('disabled'),
    ),
  );

  return $form;
}

/**
 * Implements hook_field_widget_info().
 */
function flickrup_field_widget_info() {
  return array(
    'flickrupfield' => array(
      'label' => t('Flickr photo (FlickrUp)'),
      'field types' => array('flickrupfield'),
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_element_info().
 */
function flickrup_element_info() {
  return array(
    'flickrupfield' => array(
      '#input' => TRUE,
      '#process' => array('flickrup_element_process'),
      '#element_validate' => array('flickrup_element_validate'),
      '#theme_wrappers' => array('form_element'),
      '#theme' => array('flickrup_field'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function flickrup_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // Check authorization status.
  if (!_flickrup_is_authorized()) {
    flickrup_set_error(t('Please <a href="@configure-link">configure the FlickrUp module settings</a>', array(
      '@configure-link' => url('admin/config/media/flickrup'),
    )));

    return NULL;
  }

  $default_value = isset($items[$delta]) ? $items[$delta] : '';

  if (isset($form_state['triggering_element']['#submit'][0]) ? $form_state['triggering_element']['#submit'][0] == 'field_add_more_submit' : FALSE) {
    $default_value = '';
  }

  if ((isset($form_state['triggering_element']['#submit'][0]) ? $form_state['triggering_element']['#submit'][0] == 'flickrup_submit_handler' : FALSE) && $field['cardinality'] == 1) {
    $default_value = '';
  }

  $element += array(
    '#type' => $instance['widget']['type'],
    '#default_value' => $default_value,
  );

  $element['#attached']['css'] = array(
    drupal_get_path('module', 'flickrup') . '/css/flickrupfield.css',
  );

  return $element;
}

/**
 * Element process callback for flickrupfield.
 *
 * Clear "Notice: Undefined index: file in file_ajax_upload()".
 * See more: https://www.drupal.org/node/1067470.
 */
function flickrup_element_process($element, &$form_state, $form = array()) {
  $errors = drupal_get_messages('error');
  if (isset($errors['error'])) {
    foreach ($errors['error'] as $error) {
      if (!strpos($error, 'Undefined index: file')) {
        flickrup_set_error($error);
      }
    }
  }

  $instance = field_widget_instance($element, $form_state);
  $settings = $instance['settings'];

  $element['id'] = array(
    '#type' => 'hidden',
    '#default_value' => !empty($element['#value']['id']) ? $element['#value']['id'] : NULL,
  );
  $element['metadata'] = array(
    '#type' => 'hidden',
    '#default_value' => !empty($element['#value']['metadata']) ? $element['#value']['metadata'] : NULL,
  );

  if (empty($element['#value']['id'])) {
    $element['file'] = array(
      '#type' => 'managed_file',
      '#title' => t('Image file'),
      '#default_value' => !empty($element['#value']['file']) ? $element['#value']['file'] : '',
      '#upload_location' => 'public://',
      '#upload_validators' => array(
        'file_validate_is_image' => array(),
        'file_validate_extensions' => array('png gif jpg jpeg'),
      ),
      '#description' => t('Allowed file types: .jpg, .jpeg, .png or .gif.') . '<br />' . t('Files must be less than !size.',
          array(
            '!size' => format_size(file_upload_max_size()),
          )),
      '#required' => $element['#required'],
      '#theme' => 'flickrup_field_photo_file_preview',
    );

    if ($settings['photo_info_enabled']) {
      $element['photo_title'] = array(
        '#type' => 'textfield',
        '#title' => t('Image title'),
        '#default_value' => !empty($element['#value']['photo_title']) ? $element['#value']['photo_title'] : '',
        '#required' => $element['#required'],
      );
      $element['photo_description'] = array(
        '#type' => 'textarea',
        '#title' => t('Image description'),
        '#default_value' => !empty($element['#value']['photo_description']) ? $element['#value']['photo_description'] : '',
      );
    }
  }
  else {
    // Preview.
    $photo_metadata = unserialize($element['#value']['metadata']);
    $element['preview'] = array(
      '#markup' => theme('flickrup_field_photo_preview', array(
        'photo_metadata' => $photo_metadata,
        'photo_info_enabled' => $settings['photo_info_enabled'],
      )),
    );

    // Remove button.
    $field_parents = isset($element['#field_parents']) ? $element['#field_parents'] : array();
    $field_name = isset($element['#field_name']) ? $element['#field_name'] : NULL;
    $language = isset($element['#language']) ? $element['#language'] : LANGUAGE_NONE;
    $delta = isset($element['#delta']) ? $element['#delta'] : 0;
    // Get parent which we will use into Remove Button Element.
    $parents = array_merge(
      $field_parents,
      array($field_name, $language, $delta));
    $element['remove_button'] = array(
      '#delta' => $delta,
      '#name' => implode('_',
          $parents) . '_remove_button',
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#validate' => array(),
      '#attributes' => array(),
      '#submit' => array('flickrup_submit_handler'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'path' => 'flickrup/ajax',
        'effect' => 'fade',
      ),
      '#weight' => 1000,
    );
  }

  return $element;
}

/**
 * Implements hook_field_is_empty().
 */
function flickrup_field_is_empty($item, $field) {
  if (isset($item['photo_title'])) {
    return !(!empty($item['id']) || (!empty($item['file']) && !empty($item['photo_title'])));
  }
  else {
    return !(!empty($item['id']) || !empty($item['file']));
  }
}

/**
 * Element validate callback for flickrupfield.
 */
function flickrup_element_validate(&$element, &$form_state) {
  $instance = field_widget_instance($element, $form_state);
  $settings = $instance['settings'];

  if (isset($form_state['triggering_element']['#submit'][0]) ? $form_state['triggering_element']['#submit'][0] == 'field_add_more_submit' : FALSE) {
    if (empty($element['#value']['id'])) {
      if ($element['#value']['file']['fid'] == '0') {
        form_error($element['file'], t('Please upload an image file!'));
      }
      if ($settings['photo_info_enabled'] && empty($element['#value']['photo_title'])) {
        form_error($element['photo_title'], t('Please enter an image title!'));
      }
    }
  }
  elseif (isset($element['#value']['file']['fid']) ? $element['#value']['file']['fid'] != '0' : FALSE || !empty($element['#value']['photo_title']) || !empty($element['#value']['photo_description'])) {
    if (empty($element['#value']['id'])) {
      if ($element['#value']['file']['fid'] == '0') {
        form_error($element['file'], t('Please upload an image file!'));
      }
      if ($settings['photo_info_enabled'] && empty($element['#value']['photo_title'])) {
        form_error($element['photo_title'], t('Please enter an image title!'));
      }
    }
  }
}

/**
 * Implements hook_field_presave().
 */
function flickrup_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as $delta => $item) {
    if (!empty($items[$delta]['file'])) {
      $file = file_load($items[$delta]['file']);
      $path = drupal_realpath($file->uri);
      $is_public = variable_get('flickrup_is_public', 0);

      $title = (isset($items[$delta]['photo_title'])) ? $items[$delta]['photo_title'] : '';
      $description = (isset($items[$delta]['photo_description'])) ? $items[$delta]['photo_description'] : '';
      $flickr = flickrup_phpFlickr();

      if ($flickr) {
        $upload_photo_id = $flickr->sync_upload($path, $title, $description, '', $is_public);
        if ($upload_photo_id) {
          $items[$delta]['id'] = $upload_photo_id;
          $sizes = flickrup_photo_get_sizes($upload_photo_id);
          $metadata = array(
            'title' => $title,
            'description' => $description,
            'sizes' => $sizes,
          );
          $items[$delta]['metadata'] = serialize($metadata);
        }
        else {
          flickrup_set_error($flickr->getErrorMsg(), $flickr->getErrorCode());
        }
      }
    }
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function flickrup_field_formatter_info() {
  $sizes = flickrup_photo_sizes();
  $formatters = array();
  foreach ($sizes as $size => $info) {
    $formatters[$size] = array(
      'label' => $info['label'],
      'field types' => array('flickrupfield'),
      'settings' => array(
        'galleria_width' => '600',
        'galleria_height' => '450',
        'galleria_photo_size' => '-',
        'lightbox_class' => '',
        'lightbox_rel' => '',
        'lightbox_opening_size' => '-',
      ),
    );
  }

  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function flickrup_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  // Add settings to formatter of Galleria if this one is active.
  if ($display['type'] == 'g') {
    $element['galleria_width'] = array(
      '#title' => t('width'),
      '#type' => 'textfield',
      '#size' => 4,
      '#default_value' => $settings['galleria_width'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#required' => TRUE,
    );
    $element['galleria_height'] = array(
      '#title' => t('height'),
      '#type' => 'textfield',
      '#size' => 4,
      '#default_value' => $settings['galleria_height'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#required' => TRUE,
    );
    $element['galleria_photo_size'] = array(
      '#title' => t('photo size'),
      '#type' => 'select',
      '#options' => flickrup_galleria_photo_sizes(),
      '#default_value' => $settings['galleria_photo_size'],
      '#required' => TRUE,
    );
  }
  // If the module Colorbox is enabled will add overlay settings.
  elseif (module_exists('colorbox')) {
    $element['lightbox_class'] = array(
      '#title' => t('class'),
      '#type' => 'textfield',
      '#default_value' => $settings['lightbox_class'],
      '#description' => t('e.g.: colorbox'),
    );
    $element['lightbox_rel'] = array(
      '#title' => t('rel'),
      '#type' => 'textfield',
      '#default_value' => $settings['lightbox_rel'],
      '#description' => t('e.g.: gallery-all'),
    );
    $element['lightbox_opening_size'] = array(
      '#title' => t('opening size'),
      '#type' => 'select',
      '#options' => flickrup_lightbox_photo_sizes(),
      '#default_value' => $settings['lightbox_opening_size'],
      '#description' => t('the image size to open in the overlay browser when we click on the image.'),
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function flickrup_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = '';

  if ($display['type'] == 'g') {
    $sizes = flickrup_galleria_photo_sizes();
    $summary = t('Galleria plugin size: (@galleria_width x @galleria_height) Photo size: @galleria_photo_size', array(
      '@galleria_width' => $settings['galleria_width'],
      '@galleria_height' => $settings['galleria_height'],
      '@galleria_photo_size' => $sizes[$settings['galleria_photo_size']],
    ));
  }
  elseif (module_exists('colorbox')) {
    $summary = t('Overlay settings');
  }

  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function flickrup_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  if ($display['type'] === 'g' && $items) {
    global $base_url;
    libraries_load('galleria');
    $path_to_plugin_theme = $base_url . '/' . libraries_get_path('galleria') . '/themes/classic/galleria.classic.js';
    $field_name[] = str_replace('_', '-', $field['field_name']);
    $galleria_width[] = $display['settings']['galleria_width'];
    $galleria_height[] = $display['settings']['galleria_height'];

    $element['#attached']['js'][] = array(
      'data' => array(
        'flickrup_field_name' => $field_name,
        'flickrup_path_to_plugin_theme' => $path_to_plugin_theme,
        'flickrup_galleria_width' => $galleria_width,
        'flickrup_galleria_height' => $galleria_height,
      ),
      'type' => 'setting',
    );

    $element['#attached']['js'][] = array(
      'data' => drupal_get_path('module', 'flickrup') . '/js/galleria.config.js',
      'type' => 'file',
    );

    if (variable_get('flickrup_css', 1)) {
      $element['#attached']['css'] = array(
        'data' => drupal_get_path('module', 'flickrup') . '/css/flickrup.css',
        'type' => 'file',
      );
    }

  }

  foreach ($items as $delta => $item) {
    $variables = array(
      '#node' => $entity,
      '#item' => $item,
      '#formatter' => $display['type'],
      '#field_name' => $field['field_name'],
      '#type' => 'flickrupfield',
      '#settings' => $display['settings'],
    );
    $element[$delta]['#markup'] = theme('flickrup_field_formatter', $variables);
  }

  return $element;
}

/**
 * Implements hook_theme().
 */
function flickrup_theme() {
  return array(
    'flickrup_field' => array(
      'render element' => 'element',
    ),
    'flickrup_field_formatter' => array(
      'variables' => array('element'),
    ),
    'flickrup_field_photo_preview' => array(
      'variables' => array(
        'photo_metadata',
        'photo_info_enabled',
      ),
    ),
    'flickrup_field_photo_file_preview' => array(
      'render element' => 'element',
    ),
    'flickrup_photo' => array(
      'variables' => array(
        'img',
        'formatter',
        'photo_metadata',
        'node',
        'field_name',
        'settings',
        'teaser',
      ),
    ),
    'flickrup_auth_token' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Theme for the flickrupfield element.
 */
function theme_flickrup_field($variables) {
  $element = $variables['element'];
  $output = '';
  if (empty($element['preview'])) {
    if (isset($element['photo_title']) && $element['photo_description']) {
      $header = array(
        t('Image'),
        t('Info'),
      );
      $rows = array(
        array(
          array(
            'data' => drupal_render($element['file']),
            'class' => array('flickrup-field-widget-photo'),
          ),
          array(
            'data' => drupal_render($element['photo_title']) . drupal_render($element['photo_description']),
            'class' => array('flickrup-field-widget-info'),
          ),
        ),
      );
      $output .= theme('table', array(
        'header' => $header,
        'rows' => $rows,
      ));
    }
    else {
      $header = array(
        t('Image'),
      );
      $rows = array(
        array(
          array(
            'data' => drupal_render($element['file']),
            'class' => array('flickrup-field-widget-photo'),
          ),
        ),
      );
      $output .= theme('table', array(
        'header' => $header,
        'rows' => $rows,
      ));
    }
  }
  else {
    $output .= drupal_render($element['preview']);
    $output .= drupal_render($element['remove_button']);
  }

  $output .= drupal_render_children($element);

  return $output;
}

/**
 * Theme for the flickrupfield formatter.
 */
function theme_flickrup_field_formatter($element) {
  $item = $element['#item'];
  if (empty($item['id'])) {
    return NULL;
  }
  $photo_metadata = unserialize($item['metadata']);
  $node = $element['#node'];
  $formatter = $element['#formatter'];
  $field_name = $element['#field_name'];
  $settings = $element['#settings'];
  $teaser = (arg(0) == 'node' && arg(1) == $node->nid) ? FALSE : TRUE;

  return theme('flickrup_photo', array(
    'formatter' => $formatter,
    'photo_metadata' => $photo_metadata,
    'node' => $node,
    'field_name' => $field_name,
    'settings' => $settings,
    'teaser' => $teaser,
  ));

}

/**
 * Theme for the Flick photo.
 */
function theme_flickrup_photo($variables) {
  $formatter = $variables['formatter'];
  $photo_metadata = $variables['photo_metadata'];
  $node = $variables['node'];
  $settings = $variables['settings'];
  $teaser = $variables['teaser'];

  $image_url = flickrup_get_img_url($photo_metadata, $formatter);
  $image_attributes = flickrup_get_img_attributes($photo_metadata, $formatter);

  $image = theme('image', array(
    'path' => $image_url,
    'alt' => $image_attributes['alt'],
    'title' => $image_attributes['title'],
    'attributes' => array(
      'height' => $image_attributes['height'],
      'width' => $image_attributes['width'],
    ),
    'getsize' => FALSE,
  ));

  // If Galleria formatter is selected.
  $library = libraries_detect('galleria');
  if ($formatter === 'g' && $library['installed']) {
    $thumbnail_image_url = flickrup_get_img_url($photo_metadata, 't');
    $general_image_url = flickrup_get_img_url($photo_metadata, $settings['galleria_photo_size']);
    $big_image_url = flickrup_get_img_url($photo_metadata, 'o');

    $thumbnail_image = theme('image', array(
      'path' => $thumbnail_image_url,
      'attributes' => array(
        'data-big' => $big_image_url,
        'data-title' => $image_attributes['title'],
        'data-description' => $image_attributes['description'],
      ),
    ));
    $output = l($thumbnail_image, $general_image_url, array(
      'absolute' => TRUE,
      'html' => TRUE,
    ));

  }
  elseif (module_exists('colorbox') && $settings['lightbox_class'] && $settings['lightbox_rel'] && $settings['lightbox_opening_size']) {
    $big_image_url = flickrup_get_img_url($photo_metadata, $settings['lightbox_opening_size']);
    $output = l($image, $big_image_url, array(
      'attributes' => array(
        'title' => $image_attributes['alt'],
        'class' => $settings['lightbox_class'],
        'rel' => $settings['lightbox_rel'],
      ),
      'absolute' => TRUE,
      'html' => TRUE,
    ));
  }
  else {
    $output = $image;
  }

  // Checking on if it's a teaser, will add node link to a photo.
  if (!$teaser) {
    return $output;
  }
  elseif ($formatter !== 'g' && isset($node->nid)) {
    $output = l($image, 'node/' . $node->nid, array(
      'attributes' => array('title' => $image_attributes['title']),
      'absolute' => TRUE,
      'html' => TRUE,
    ));

    return $output;
  }

  return $output;
}

/**
 * Theme for the flickrupfield saved photo.
 */
function theme_flickrup_field_photo_preview($variables) {
  $photo_metadata = $variables['photo_metadata'];
  $photo_info_enabled = $variables['photo_info_enabled'];

  $image_url = flickrup_get_img_url($photo_metadata, 'm');
  $image_attributes = flickrup_get_img_attributes($photo_metadata, 'm');

  $image = theme('image', array(
    'path' => $image_url,
    'alt' => $image_attributes['alt'],
    'title' => check_plain($photo_metadata['title']),
    'attributes' => array(
      'height' => $image_attributes['height'],
      'width' => $image_attributes['width'],
    ),
    'getsize' => FALSE,
  ));

  if ($photo_info_enabled) {
    $header = array(
      t('Image'),
      t('Info'),
    );

    $rows = array(
      array(
        'data' => array(
          array(
            'data' => $image,
            'rowspan' => 2,
            'class' => array('flickrup-field-widget-saved-photo'),
          ),
          check_plain($photo_metadata['title']),
        ),
      ),
      array(
        'data' => array(check_plain($photo_metadata['description'])),
      ),
    );
    return theme('table', array(
      'header' => $header,
      'rows' => $rows,
    ));
  }
  else {
    $header = array(
      t('Image'),
    );

    $rows = array(
      array(
        $image,
      ),
    );
    return theme('table', array(
      'header' => $header,
      'rows' => $rows,
    ));
  }
}

/**
 * Theme for the flickrupfield photo file.
 */
function theme_flickrup_field_photo_file_preview($variables) {
  $element = $variables['element'];

  // Renders element as usual.
  $base = drupal_render_children($element);

  $image_styles = image_styles();
  if ($element['fid']['#value'] != 0 && in_array('flickrup_preview', array_keys($image_styles))) {
    $output = theme('image_style', array(
      'style_name' => 'flickrup_preview',
      'path' => file_load($element['fid']['#value'])->uri,
      'getsize' => FALSE,
    ));

    $output = theme('html_tag', array(
      'element' => array(
        '#tag' => 'div',
        '#attributes' => array(
          'class' => 'flickrup-photo-file-preview',
        ),
        '#value' => $output,
      ),
    ));

    $output .= $base;

    $output = theme('html_tag', array(
      'element' => array(
        '#tag' => 'div',
        '#attributes' => array(
          'class' => 'flickrup-photo-file-wrapper',
        ),
        '#value' => $output,
      ),
    ));

    return $output;
  }

  return $base;
}

/**
 * Theme for the flickrup auth token form element.
 */
function theme_flickrup_auth_token($variables) {
  $element = $variables['element'];
  $output = drupal_render_children($element);

  $output .= theme('html_tag', array(
    'element' => array(
      '#tag' => 'label',
      '#value' => t('Auth Token'),
    ),
  ));

  if (!_flickrup_is_authorized()) {
    $output .= theme('image', array(
      'path' => drupal_get_path('module', 'flickrup') . '/images/false.png',
    ));
    $output .= theme('html_tag', array(
      'element' => array(
        '#tag' => 'p',
        '#value' => t('No token. Please see <a href="@url">help</a> for additional instructions.', array('@url' => url('admin/help/flickrup', array('fragment' => 'auth')))),
      ),
    ));
  }
  else {
    $output .= theme('image', array(
      'path' => drupal_get_path('module', 'flickrup') . '/images/true.png',
    ));

    $output .= theme('html_tag', array(
      'element' => array(
        '#tag' => 'p',
        '#value' => t('Have a token'),
      ),
    ));

  }

  $output = theme('html_tag', array(
    'element' => array(
      '#tag' => 'div',
      '#attributes' => array(
        'class' => 'form-item form-type-textfield form-item-flickrup-auth-token',
      ),
      '#value' => $output,
    ),
  ));

  return $output;
}

/**
 * Submit callback to remove an item from the field UI multiple wrapper.
 *
 * When a remove button is submitted, we need to find the item that it
 * referenced and delete it. Since field UI has the deltas as a straight
 * unbroken array key, we have to renumber everything down. Since we do this
 * we *also* need to move all the deltas around in the $form_state['values']
 * and $form_state['input'] so that user changed values follow. This is a bit
 * of a complicated process.
 */
function flickrup_submit_handler($form, &$form_state) {
  $button = $form_state['triggering_element'];
  $delta = $button['#delta'];
  $inpt = 'input';

  // Where in the form we'll to find the parent element.
  $address = array_slice($button['#array_parents'], 0, -2);

  // Go one level up in the form, to the widgets container.
  $parent_element = drupal_array_get_nested_value($form, $address);
  $field_name = $parent_element['#field_name'];
  $langcode = $parent_element['#language'];
  $parents = $parent_element['#field_parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);

  // Go ahead and renumber everything from our delta to the last
  // item down one. This will overwrite the item being removed.
  for ($i = $delta; $i <= $field_state['items_count']; $i++) {
    $old_element_address = array_merge($address, array($i + 1));
    $new_element_address = array_merge($address, array($i));

    $moving_element = drupal_array_get_nested_value($form, $old_element_address);
    $moving_element_value = drupal_array_get_nested_value($form_state['values'], $old_element_address);
    $moving_element_input = drupal_array_get_nested_value($form_state[$inpt], $old_element_address);

    // Tell the element where it's being moved to.
    $moving_element['#parents'] = $new_element_address;

    // Move the element around.
    form_set_value($moving_element, $moving_element_value, $form_state);
    drupal_array_set_nested_value($form_state[$inpt], $moving_element['#parents'], $moving_element_input);

    // Move the entity in our saved state.
    if (isset($field_state['entity'][$i + 1])) {
      $field_state['entity'][$i] = $field_state['entity'][$i + 1];
    }
    else {
      unset($field_state['entity'][$i]);
    }
  }

  // Replace the deleted entity with an empty one. This helps to ensure that
  // trying to add a new entity won't resurrect a deleted entity
  // from the trash bin.
  // $count = count($field_state['entity']);
  // Then remove the last item. But we must not go negative.
  if ($field_state['items_count'] > 0) {
    $field_state['items_count']--;
  }

  // Fix the weights. Field UI lets the weights be in a range of
  // (-1 * item_count) to (item_count). This means that when we remove one,
  // the range shrinks; weights outside of that range then get set to
  // the first item in the select by the browser, floating them to the top.
  // We use a brute force method because we lost weights on both ends
  // and if the user has moved things around, we have to cascade because
  // if I have items weight weights 3 and 4, and I change 4 to 3 but leave
  // the 3, the order of the two 3s now is undefined and may not match what
  // the user had selected.
  $input = drupal_array_get_nested_value($form_state[$inpt], $address);
  // Sort by weight.
  uasort($input, '_field_sort_items_helper');

  // Change weight everything in the correct order.
  $weight = -1 * $field_state['items_count'];
  foreach ($input as $key => $item) {
    if ($item) {
      $input[$key]['_weight'] = $weight++;
    }
  }
  drupal_array_set_nested_value($form_state[$inpt], $address, $input);
  field_form_set_state($parents, $field_name, $langcode, $form_state, $field_state);

  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback remove field when remove click is trigger.
 *
 * In this callback we will replace field items. Main job
 * to delete field item we will done into submit handler.
 */
function flickrup_ajax() {
  if (isset($_POST['ajax_html_ids'])) {
    unset($_POST['ajax_html_ids']);
  }

  list($form, $form_state) = ajax_get_form();
  drupal_process_form($form['#form_id'], $form, $form_state);
  // Get the information on what we're removing.
  $button = $form_state['triggering_element'];
  // Go two levels up in the form, to the whole widget.
  $element = drupal_array_get_nested_value($form, array_slice($button['#array_parents'], 0, -3));
  // Now send back the proper AJAX command to replace it.
  $return = array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace('#' . $element['#id'], drupal_render($element)),
    ),
  );

  // Because we're doing this ourselves, messages aren't automatic. We have
  // to add them.
  $messages = theme('status_messages');
  if ($messages) {
    $return['#commands'][] = ajax_command_prepend('#' . $element['#id'],
      $messages);
  }

  return $return;
}
