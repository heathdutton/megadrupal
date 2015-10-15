<?php

/**
 * @file
 * The theme system, which controls the output of the Umage field.
 */

/**
 * Returns HTML for a group of image upload widgets.
 *
 * @param $variables
 * @return string
 */
function theme_nice_imagefield_widget_multiple($variables) {
  $element = $variables['element'];

  // Get our list of widgets in order (needed when the form comes back after
  // preview or failed validation).
  $widgets = array();
  foreach (element_children($element) as $key) {
    if (isset($element[$key]['#default_value']['_weight'])) {
      $element[$key]['_weight']['#value'] = $element[$key]['#default_value']['_weight'];
    }
    $widgets[] = &$element[$key];
  }
  usort($widgets, '_field_sort_items_value_helper');

  $rows = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'nice-imagefield-sortable',
        'clearfix',
      ),
    ),
  );

  $attributes_style = NULL;
  if (!empty($element['#nice_imagefield_size'])) {
    foreach ($element['#nice_imagefield_size'] as $attribute => $value) {
      $attributes_style .=  $attribute . ':' . $value . 'px;';
    }

    $rows['#attached']['css'] = array(
      'div.ui-sortable-placeholder { ' . $attributes_style . ' }' => array(
        'type' => 'inline',
      ),
    );
  }

  foreach ($widgets as $key => &$widget) {
    // Save the uploading row for last.
    if ($widget['#file'] == FALSE) {
      $widget['#title'] = $element['#file_upload_title'];
      $widget['#description'] = $element['#file_upload_description'];
      continue;
    }

    // Delay rendering of the "Display" option and the weight selector, so that
    // each can be rendered later in its own column.
    if (isset($element['#display_field']) && $element['#display_field']) {
      hide($widget['display']);
    }
    hide($widget['_weight']);

    // Render everything else together in a column, without the normal wrappers.
    $widget['#theme_wrappers'] = array();
    $information = drupal_render($widget);

    $display = '';
    if (isset($element['#display_field']) && $element['#display_field']) {
      unset($widget['display']['#title']);
      $display = array(
        'data' => render($widget['display']),
        'class' => array('checkbox'),
      );
    }

    $weight = render($widget['_weight']);

    // Arrange the item with all of the rendered elements.
    $row = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('nice-imagefield-element'),
        'style' => $attributes_style,
      ),
    );

    $row['info'] = array(
      '#markup' => $information,
    );

    if (isset($element['#display_field']) && $element['#display_field']) {
      $row['display'] = array(
        '#markup' => $display,
        '#prefix' => '<div class="hidden">',
        '#suffix' => '</div>',
      );
    }

    $row['weight'] = array(
      '#markup' => $weight,
      '#prefix' => '<div class="nice-imagefield-weight">',
      '#suffix' => '</div>',
    );

    $rows[] = $row;
  }

  $output = drupal_render($rows);
  $output .= drupal_render_children($element);
  return $output;
}

/**
 * Returns HTML for an image field widget.
 *
 * @param $variables
 * @return string
 */
function theme_nice_imagefield_widget_image($variables) {
  $element = $variables['element'];
  $output = '';

  if (isset($element['preview']) && isset($element['remove_button'])) {
    $attributes = array();
    if (isset($element['#nice_imagefield_size']['height'])) {
      $attributes['style'] = 'height: ' . $element['#nice_imagefield_size']['height'] . 'px;';
    }
    $output .= '<div class="nice-imagefield-card"' . drupal_attributes($attributes) . '>';
  }

  if (isset($element['preview'])) {
    $output .= '<div class="nice-imagefield-image front">';
    $output .= '<div class="loading"></div>';
    $output .= drupal_render($element['preview']);
    $output .= '<div class="buttons">';
    if (!empty($element['alt']['#access']) || !empty($element['title']['#access'])) {
      $output .= '<input type="button" class="form-submit flip-back" value="' . t('Edit') . '" />';
    }
    if (isset($element['remove_button']) && ((isset($element['remove_button']['#access']) && $element['remove_button']['#access']) || !isset($element['remove_button']['#access']))) {
      $output .= drupal_render($element['remove_button']);
    }
    $output .= '</div>';
    $output .= '</div>';
  }

  if ($element['fid']['#value'] != 0) {
    $element['filename']['#markup'] = NULL;
  }

  if (!empty($element['alt']['#access']) || !empty($element['title']['#access'])) {
    $output .= '<div class="nice-imagefield-form back">';
    $output .= '<div class="inner">';
    if (isset($element['alt'])) {
      unset($element['alt']['#description']);
      $output .= drupal_render($element['alt']);
    }
    if (isset($element['title'])) {
      unset($element['title']['#description']);
      $output .= drupal_render($element['title']);
    }

    $output .= '<input type="button" class="form-submit flip-front" value="' . t('Ok') . '" />';
    $output .= '</div>';
    $output .= '</div>';
  }

  if (isset($element['preview']) && isset($element['remove_button'])) {
    $output .= '</div>';
  }

  $output .= drupal_render_children($element);
  return $output;
}
