<?php

/**
 * Hijack theme_fieldset().
 * Without this function Drupal cannot integrate jQueryUI properly
 * The most important factor is without this it cannot degrade
 * gracefuly when used without js enabled, and even with js enabled
 * somehow the class collapsed and not collapsed is not parseable
 * from jQuery plugin.
 */
function jqueryui_vtcore_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));

  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';

  $fieldset_wrapper_attributes = array(
    'class' => array(
      'ui-widget-content',
      'fieldset-wrapper',
      'clearfix',
    ),
  );

  // Entering mode where fieldset has title
  if (!empty($element['#title'])) {
    $open = array(
      'class' => array(
        'ui-corner-top',
        'ui-state-active',
    		'ui-no-border-bottom',
      ),
    );
    $collapsed = array(
      'class' => array(
        'ui-corner-all',
        'ui-state-default',
      ),
    );

    $fieldset_wrapper_attributes['class'][] = 'ui-no-border-top';
    $fieldset_wrapper_attributes['class'][] = 'ui-corner-bottom';

    if (isset($element['#collapsed']) && $element['#collapsed'] == true) {
      $legend_attributes = $collapsed;
      $icon = 'ui-icon-triangle-1-e';
    }
    else {
      $legend_attributes = $open;
      $icon = 'ui-icon-triangle-1-s';
    }

    if (isset($element['#collapsible']) && $element['#collapsible'] == false) {
      $icon = 'ui-icon-bullet';
    }

    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend' . drupal_attributes($legend_attributes) . '><span class="fieldset-legend"><span class="ui-icon ' . $icon . '"></span>' . $element['#title'] . '</span></legend>';
  }

  if (empty($element['title'])) {
    $fieldset_wrapper['class'][] = 'ui-corner-all';
  }

  $output .= '<div' . drupal_attributes($fieldset_wrapper_attributes) . '>';

  if (!empty($element['#description'])) {
    $output .= '<div class="fieldset-description">' . $element['#description'] . '</div>';
  }

  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</div>';
  $output .= "</fieldset>\n";
  return $output;
}
