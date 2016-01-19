<?php

/**
 * @file
 * Theme callbacks for Image Matrix module.
 */

/**
 * Display a view using Image Matrix style.
 */
function template_preprocess_views_view_imagematrix(&$variables) {
  $view     = $variables['view'];
  $options  = $view->style_plugin->options;

  // Split rows into blocks based on plugin settings.
  $variables['blocks'] = array_chunk($variables['rows'], $options['block_images_count']);

  // Prepare classes for blocks and field groups.
  $variables['block_classes'] = $variables['block_styles'] = array();
  $variables['field_group_classes'] = $variables['field_group_styles'] = array();

  // Force required size of each block with inline style.
  $block_styles = array();
  $block_styles[] = 'width: ' . $options['block_width'] . 'px';
  if (!empty($options['block_height'])) {
    $block_styles[] = 'height: ' . $options['block_height'] . 'px';
  }
  $variables['block_styles'] = implode('; ', $block_styles);

  // Prepare class names for blocks.
  $count = 0;
  $max = count($variables['blocks']);
  foreach ($variables['blocks'] as $block_id => $block) {
    $count++;
    $classes = array();
    $classes[] = 'views-imagematrix-block';
    $classes[] = 'views-imagematrix-block-' . $count;
    $classes[] = 'views-imagematrix-block-' . ($count % 2 ? 'odd' : 'even');
    if ($count == 1) {
      $classes[] = 'views-imagematrix-block-first';
    }
    if ($count == $max) {
      $classes[] = 'views-imagematrix-block-last';
    }
    // Flatten the classes to a string for each block for the template file.
    $variables['block_classes'][$block_id] = implode(' ', $classes);

    // Prepare class names for field groups inside each block.
    $field_group_count = 0;
    $field_group_max = count($block);
    foreach ($block as $field_group_id => $field_group) {
      $field_group_count++;
      $classes = array();
      $classes[] = 'views-imagematrix-fields';
      $classes[] = 'views-imagematrix-fields-float-' . $view->imagematrix['blocks'][$block_id][$field_group_id]['float'];
      $classes[] = 'views-imagematrix-fields-' . $field_group_count;
      $classes[] = 'views-imagematrix-fields-' . ($field_group_count % 2 ? 'odd' : 'even');
      if ($field_group_count == 1) {
        $classes[] = 'views-imagematrix-fields-first';
      }
      if ($field_group_count == $field_group_max) {
        $classes[] = 'views-imagematrix-fields-last';
      }
      // Flatten the classes to a string for each field group for the template file.
      $variables['field_group_classes'][$block_id][$field_group_id] = implode(' ', $classes);

      // Force required size of each block with inline style.
      $field_group_styles = array();
      $field_group_styles[] = 'width: ' . $view->imagematrix['blocks'][$block_id][$field_group_id]['width'] . 'px';
      $field_group_styles[] = 'height: ' . $view->imagematrix['blocks'][$block_id][$field_group_id]['height'] . 'px';
      $field_group_styles[] = 'padding: ' . $options['image_padding'] . 'px';
      $variables['field_group_styles'][$block_id][$field_group_id] = implode('; ', $field_group_styles);
    }
  }

  // Add some basic CSS so that hopefully image matrix
  // is generated properly out of the box.
  drupal_add_css(IMAGEMATRIX_PATH . '/theme/imagematrix.css');
}

/**
 * Process a single Image Matrix field within a view.
 */
function imagematrix_preprocess_views_view_field(&$variables) {

  if (!empty($variables['field']->options['type']) && $variables['field']->options['type'] == 'imagematrix') {
    $view     = $variables['view'];
    $options  = $view->style_plugin->options;

    $field_name = 'field_' . $variables['field']->field_info['field_name'];

    if (!empty($variables['row']->{$field_name})) {
      foreach ($variables['row']->{$field_name} as $delta => $item) {

        // We will use "rendered" array here. It is not really rendered yet,
        // but has few more values added to the array by field formatter
        // compared to the "raw" version.
        $item = $item['rendered']['#item'];

        // Prepare image HTML.
        $path_elements = array('imagematrix_image', $item['fid'], $item['width'], $item['height']);
        $image_variables = array(
          'path' => url(implode('/', $path_elements)),
          'width' => $item['width'],
          'height' => $item['height'],
          'alt' => $item['alt'],
          'title' => $item['title'],
          'attributes' => array(
            'class' => 'imagematrix-image imagematrix-image-' . $item['fid'],
          ),
        );
        $output = theme('image', $image_variables);

        // Add link HTML around image if required.
        $settings = $variables['field']->options['settings'];
        if (!empty($settings['image_link'])) {
          $target_path = $item['links'][$settings['image_link']];
          if ($settings['image_link'] == 'file') {
            // $item['field_type'] will be set only for "media" and "video"
            // fields, it is not set for "image" fields.
            if (isset($item['field_type']) && $item['field_type'] == 'video') {
              $target_path = file_create_url($item['video']['uri']);
            }
            else if (!empty($settings['target_image_style']) && $image_style = image_style_load($settings['target_image_style'])) {
              $target_path = image_style_url($settings['target_image_style'], $item['uri']);
            }
          }
          $attributes = array();
          if (!empty($item['title'])) {
            $attributes['title'] = $item['title'];
          }
          if (!empty($item['custom_class'])) {
            $attributes['class'] = $item['custom_class'];
          }
          if (!empty($item['custom_rel'])) {
            $attributes['rel'] = $item['custom_rel'];
            if ($item['custom_rel_id']) {
              $attributes['rel'] .= $item['entity_id'];
            }
          }
          $output = l($output, $target_path, array('html' => TRUE, 'attributes' => $attributes));
        }

        // Pass image "alt" value if "Add alt element" setting is enabled.
        // This settings could be available only for "image" fields,
        // and only if "alt" field has been enabled in field settings.
        if (isset($settings['add_alt_element']) && $settings['add_alt_element']) {
          $element = array(
            '#tag' => 'div',
            '#attributes' => array('class' => 'image-alt'),
            '#value' => $item['alt'],
          );
          $output .= theme_html_tag(array('element' => $element));
        }
        // Pass image "title" value if "Add title element" setting is enabled.
        // This settings could be available only for "image" fields,
        // and only if "title" field has been enabled in field settings.
        if (isset($settings['add_title_element']) && $settings['add_title_element']) {
          $element = array(
            '#tag' => 'div',
            '#attributes' => array('class' => 'image-title'),
            '#value' => $item['title'],
           );
          $output .= theme_html_tag(array('element' => $element));
        }

        $variables['output'] = $output;

        // Add Matrix classes to wrapper div.
        $variables['field']->options['element_wrapper_class'] = 'views-field-imagematrix';

      }
    }
  }
}

function imagematrix_preprocess_field(&$variables) {
  if (
    isset($variables['element'])
    && is_array($variables['element'])
    && isset($variables['element']['#formatter'])
    && $variables['element']['#formatter'] == 'imagematrix'
  ) {
    $settings = $variables['element']['#settings'];

    $variables['classes_array'][] = 'field-imagematrix clearfix';

    $styles = array();
    $styles[] = 'width: ' . $settings['block_width'] . 'px';
    if (!empty($settings['block_height'])) {
      $styles[] = 'height: ' . $settings['block_height'] . 'px';
    }
    foreach ($variables['items'] as $block_id => $block) {
      $variables['item_attributes_array'][$block_id]['style'] = implode('; ', $styles);
    }
  }
}

function template_preprocess_field_imagematrix_block(&$variables) {
  $images = $variables['images'];

  $max = count($images);
  foreach ($images as $item_id => $item) {
    $count = $item_id + 1;

    if (!empty($item['uri'])) {
      $link = $item['uri'];
    }

    // Theme single image.
    $path_elements = array('imagematrix_image', $item['fid'], $item['width'], $item['height']);
    $item['uri'] = url(implode('/', $path_elements));
    $image_variables = array('item' => $item);
    if (!empty($link)) {
      $image_variables['path'] = array(
        'path' => $link,
        'options' => array(),
      );
      if (!empty($item['title'])) {
        $image_variables['path']['options']['attributes']['title'] = $item['title'];
      }
      else if (!empty($item['alt'])) {
        $image_variables['path']['options']['attributes']['title'] = $item['alt'];
      }
      if (!empty($variables['settings']['custom_class'])) {
        $image_variables['path']['options']['attributes']['class'] = array($variables['settings']['custom_class']);
      }
      if (!empty($variables['settings']['custom_rel'])) {
        $image_variables['path']['options']['attributes']['rel'] = $variables['settings']['custom_rel'];
        if ($variables['settings']['custom_rel_id']) {
          $image_variables['path']['options']['attributes']['rel'] .= $item['entity_id'];
        }
      }
    }
    $variables['images'][$item_id] = theme('image_formatter', $image_variables);
    // Pass image "alt" value if "Add alt element" setting is enabled.
    if ($variables['settings']['add_alt_element']) {
      $variables['image_alts'][$item_id] = $item['alt'];
    }
    // Pass image "title" value if "Add title element" setting is enabled.
    if ($variables['settings']['add_title_element']) {
      $variables['image_titles'][$item_id] = $item['title'];
    }

    // Image div classes.
    $classes = array();
    $classes[] = 'field-imagematrix-image';
    $classes[] = 'field-imagematrix-image-' . $item['fid'];
    $classes[] = 'field-imagematrix-image-' . ($count % 2 ? 'odd' : 'even');
    if ($count == 1) {
      $classes[] = 'field-imagematrix-image-first';
    }
    if ($count == $max) {
      $classes[] = 'field-imagematrix-image-last';
    }
    $variables['image_classes'][$item_id] = implode(' ', $classes);

    // Image div styles.
    $styles = array();
    $styles[] = 'width: ' . $item['width'] . 'px';
    $styles[] = 'height: ' . $item['height'] . 'px';
    $styles[] = 'padding: ' . $variables['settings']['image_padding'] . 'px';
    $styles[] = 'float: ' . $item['float'];
    $variables['image_styles'][$item_id] = implode('; ', $styles);

  }

}
