<?php

/**
 * @file
 * Theme functions for the Rotoslider.
 */

/**
 * Style plugin to render each item in a Rotoslider.
 */
function template_preprocess_rotoslider_view_slider(&$variables) {
  // Prepare settings to be passed to JS.
  $settings = array();
  foreach (array_filter($variables['options']['settings']) as $name => $setting) {
    if ($name == 'percentage') {
      $settings[$name] = $setting / 100;
    }
    elseif ($name == 'speed') {
      $settings[$name] = (int) $setting;
    }
    elseif ($name == 'cycle') {
      $settings[$name] = (int) $setting;
    }
  }

  if (!empty($settings)) {
    drupal_add_js(array('rotoslider' => $settings), 'setting');
  }

  if (!empty($variables['options']['settings']['link'])) {
    $options['base_field'] = $variables['view']->base_field;
  }
  $options['image'] = !empty($variables['options']['settings']['image']);
  if (!empty($variables['options']['settings']['image_style'])) {
    $options['nav_style'] = $variables['options']['settings']['image_style'];
  }

  // Add a slide for each row.
  $items = _rotoslider_process($variables['options']['fields'], $variables['rows'], $variables['view']->field, $options);

  // Either create an initial slide from the settings, or use the first slide.
  if (!empty($variables['options']['initial']['default'])) {
    unset($variables['options']['initial']['default']);
    $heading = _rotoslider_process($variables['options']['initial'], array($variables['rows'][0]), $variables['view']->field);
    $heading = reset($heading);
    array_unshift($items, $heading);
  }

  // Build the element for theme_rotoslider_slider().
  $variables['element'] = array(
    '#theme' => 'rotoslider_slider',
    '#items' => $items,
    '#image' => $options['image'],
  );
}

/**
 * Skip the usage of a tpl.php file.
 */
function theme_rotoslider_view_slider($variables) {
  return drupal_render($variables['element']);
}

/**
 * Builds a renderable array of a Rotoslider.
 *
 * @param $variables
 *   An associative array containing:
 *   - items: An associative array of items to be displayed in the slider. The
 *     first element will be used as the initial state of the slider. Each item
 *     should be itself an array, with the following elements:
 *     - title: The slider item text.
 *     - image: The image URL.
 *     - text: The description associated with this item.
 *   - image: A boolean which determines if the slider navigation should be
 *     text-based or image-based.
 */
function template_preprocess_rotoslider_slider(&$variables) {
  $items = $variables['items'];
  $image = $variables['image'];
  $path = drupal_get_path('module', 'rotoslider');

  static $count;
  $count++;

  // Build three arrays, each with its fields.
  $links = array();
  $images = array();
  $text = array();
  foreach ($items as $item) {
    // Append an integer to each set of data to keep them unique.
    $key = drupal_html_class('a' . $item['title']['#markup'] . '-' . $count);
    // An individual item to be passed to theme_links().
    $link_text = $item['title']['#markup'];
    if (!empty($item['title']['#path'])) {
      $link_text = $item['title'];
      unset($link_text['#markup']);
      if (!empty($link_text['#nav_style'])) {
        $link_text['#style_name'] = $link_text['#nav_style'];
      }
      $link_text = drupal_render($link_text);
    }
    $links[] = array(
      'title' => $link_text,
      'href' => "#",
      'external' => TRUE,
      'html' => TRUE,
      'attributes' => array(
        'data-key' => $key,
      ),
    );
    $images[] = array(
      '#alt' => $item['title']['#markup'],
      '#attributes' => array(
        'data-key' => $key,
      ),
    ) + $item['image'];

    // A container with heading (possibly a link) and text as children.
    $heading_text = $item['title'];
    if (!empty($item['uri'])) {
      $heading_text = array(
        '#theme' => 'link',
        '#text' => $item['title']['#markup'],
        '#path' => $item['uri']['path'],
        '#options' => $item['uri']['options'] + array(
          'attributes' => array(),
          'html' => FALSE,
        ),
      );
    }
    $text[] = array(
      '#type' => 'container',
      '#attributes' => array(
        'data-key' => $key,
        'class' => array(
          'rotoslider-text',
        ),
      ),
      'heading' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array(
            'heading',
          ),
        ),
        'heading' => $heading_text,
      ),
      'text' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array(
            'text',
          ),
        ),
        'text' => $item['text'],
      ),
    );
  }

  $heading = array_shift($links);

  $element = array(
    '#attached' => array(
      'js' => array(
        $path . '/js/rotoslider.js',
      ),
    ),
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'rotoslider',
      ),
    ),
    'image' => array(
      '#attributes' => array(
        'class' => array(
          'rotoslider-image',
        ),
      ),
    ) + array_shift($images),
    'images' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'rotoslider-images',
        ),
      ),
      'images' => $images,
    ),
    'descriptions' => array(
      '#pre_render' => array('rotoslider_container_pre_render'),
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'rotoslider-descriptions',
        ),
      ),
      'text' => $text,
    ),
    'menu' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'rotoslider-menu',
        ),
      ),
      'links' => array(
        '#theme' => 'links',
        '#attributes' => array(
          'class' => array(
            'rotoslider-nav',
          ),
        ),
        '#heading' => array(
          'text' => $heading['title'],
          'level' => 'div',
          'class' => array(
            'heading',
          ),
        ),
        '#links' => $links,
      ),
    ),
  );

  $element['descriptions']['text'][0]['#attributes']['class'] = array('rotoslider-description');
  if (!empty($image)) {
    $element['#attributes']['class'][] = 'rotoslider-image-nav';
    $element['#attached']['css'][] = $path . '/css/rotoslider.image.css';
    unset($element['menu']['links']['#heading']);
  }
  else {
    $element['#attributes']['class'][] = 'rotoslider-text-nav';
    $element['#attached']['css'][] = $path . '/css/rotoslider.text.css';
    $element['descriptions']['text'][0]['heading']['#access'] = FALSE;
  }

  $variables['element'] = $element;
}

/**
 * #pre_render callback: Prevents empty containers from printing.
 */
function rotoslider_container_pre_render($element) {
  $children_keys = element_children($element);
  $children = drupal_render_children($element, $children_keys);
  if (empty($children)) {
    $element = array('#markup');
  }
  else {
    foreach ($children_keys as $key) {
      show($element[$key]);
    }
  }
  return $element;
}

/**
 * Returns HTML for a Rotoslider.
 */
function theme_rotoslider_slider($variables) {
  return drupal_render($variables['element']);
}

/**
 * Helper function: Turns raw Views data into santized values.
 *
 * @param $options
 *   Array of either Views field names or hardcoded values, keyed by field type.
 * @param $rows
 *   Array of objects containing raw Views data, keyed by Views field aliases.
 * @param $fields
 *   Array of Views field handler objects, keyed by Views field names.
 *
 * @return
 *   Array of numerically indexed items. Each item should itself be an array,
 *   with the following elements:
 *   - title: The item's plain text title.
 *   - image: The item's image absolute URL.
 *   - text: The item's decription, which may contain markup.
 */
function _rotoslider_process($options, $rows, $fields, $settings = array()) {
  $items = array();

  $info = array();
  foreach (array_filter($options) as $type => $field_name) {
    if (empty($fields[$field_name])) {
      $info[$type] = $field_name;
    }
    else {
      $info[$type] = isset($fields[$field_name]->field_info) ? "field_$field_name" : $fields[$field_name]->field_alias;
    }
  }

  // Get the placeholder image.
  $placeholder = variable_get('rotoslider_placeholder');
  foreach ($rows as $row) {
    $values = array();

    // Find the URI of the original content.
    // @todo Make this a proper field.
    if (!empty($settings['base_field']) && !empty($row->_field_data[$settings['base_field']])) {
      $values['uri'] = entity_uri($row->_field_data[$settings['base_field']]['entity_type'], $row->_field_data[$settings['base_field']]['entity']);
    }

    foreach (array('image', 'text', 'title') as $type) {
      $value = '';
      // The field contains a Views field alias.
      if (isset($row->$info[$type])) {
        $value = $row->$info[$type];
      }
      // The field contains the actual value.
      elseif (isset($info[$type])) {
        $value = $info[$type];
      }
      // Do not add items without a title.
      elseif ($type == 'title') {
        continue;
      }

      // Use the rendered value of a field.
      if (is_array($value) && isset($value[0]['rendered'])) {
        $value = $value[0]['rendered'];
      }

      // Rewrite the image to use either theme_image() or theme_image_style().
      if ($type == 'image') {
        // Views gives us an array for theme_image_formatter(), rewrite it.
        if (is_array($value) && isset($value['#theme']) && $value['#theme'] == 'image_formatter') {
          $value['#style_name'] = $value['#image_style'];
          $value['#theme'] = 'image_style';
          $value['#path'] = $value['#item']['uri'];
          if (!empty($settings['nav_style'])) {
            $value['#nav_style'] = $settings['nav_style'];
          }
        }
        else {
          // If a placeholder is set and the image is missing, use it.
          if (empty($value) && !empty($placeholder)) {
            $path = $placeholder;
          }
          // If the image is present, use it.
          elseif (!empty($value) && !is_array($value)) {
            $path = $value;
          }
          if (!empty($path)) {
            $value = array(
              '#theme' => 'image',
              '#path' => $path,
              '#attributes' => array(),
            );
          }
        }
      }

      // Wrap plain text as a render array.
      if (!is_array($value)) {
        $value = array('#markup' => $value);
      }

      if ($type == 'title' && !empty($settings['image']) && !empty($values['image'])) {
        $value += $values['image'];
      }

      $values[$type] = $value;
    }

    $items[] = $values;
  }

  return $items;
}
