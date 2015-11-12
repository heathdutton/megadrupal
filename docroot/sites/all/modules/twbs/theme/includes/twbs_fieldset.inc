<?php

/**
 * Returns HTML for a fieldset form element and its children.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #attributes, #children, #collapsed, #collapsible,
 *     #description, #id, #title, #value.
 *
 * @ingroup themeable
 */
function twbs_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));

  $output = '';

  $output .= '<div class="panel panel-default">';

  if (!empty($element['#title'])) {
    $options = array(
      'html' => TRUE,
      'fragment' => $element['#id'],
      'attributes' => array(
        'data-toggle' => 'collapse',
      ),
    );
    if (isset($element['#group'])) {
      $options['attributes']['data-parent'] = '#' . $element['#group'];
    }
    $output .= '<div class="panel-heading"><h4 class="panel-title">';
    $output .= l($element['#title'], NULL, $options);
    $output .= '</h4></div>';
  }

  $attributes['id'] = $element['#id'];
  $attributes['class'] = array('panel-collapse', 'collapse');
  if (isset($element['#collapsed']) && !$element['#collapsed']) {
    $attributes['class'][] = 'in';
  }
  $output .= '<div' . drupal_attributes($attributes) . '><div class="panel-body">';
  if (!empty($element['#description'])) {
    $output .= '<p>' . $element['#description'] . '</p>';
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</div></div>';

  $output .= '</div>';

  return $output;
}
