<?php
/**
 * @file
 * Link related theme functions.
 */

/**
 * Formats a link field widget.
 */
function strapped_link_field($vars) {
  drupal_add_css(drupal_get_path('module', 'link') . '/link.css');
  $element = $vars['element'];
  // Prefix single value link fields with the name of the field.
  if (empty($element['#field']['multiple'])) {
    if (isset($element['url']) && !isset($element['title'])) {
      $element['url']['#title_display'] = 'invisible';
    }
  }

  // If there is only 1 field then hide the fieldset wrapper as it look silly.
  $show_wrapper = (!empty($element['attributes']['target']) || !empty($element['attributes']['title']));


  $output = $show_wrapper ? '<fieldset class="panel panel-default form-wrapper"><div class="panel-body">' : "";
  $output .= '<div class="link-field-subrow clearfix">';
  if (isset($element['title'])) {
    $output .= '<div class="link-field-title link-field-column">' . drupal_render($element['title']) . '</div>';
  }
  $output .= '<div class="link-field-url' . (isset($element['title']) ? ' link-field-column' : '') . '">' . drupal_render($element['url']) . '</div>';
  $output .= '</div>';
  if (!empty($element['attributes']['target'])) {
    $output .= '<div class="link-attributes">' . drupal_render($element['attributes']['target']) . '</div>';
  }
  if (!empty($element['attributes']['title'])) {
    $output .= '<div class="link-attributes">' . drupal_render($element['attributes']['title']) . '</div>';
  }
  $output .= $show_wrapper ? '</div></fieldset>' : '';
  return $output;
}

/**
 * @param $element
 * @return mixed
 */
function strapped_process_link_field_element($element) {
    // Remove the form group wrapper around the textfield as the parent has one already.
    $element['url']['#form_group_wrapper'] = FALSE;

    return $element;
}