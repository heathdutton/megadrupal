<?php

/**
 * @file
 * Needs to be documented.
 */

/**
 * Theming of the module text_resize.
 * @return string
 *   Needs to be documented.
 */
function nerra_text_resize_block() {
  $reset_button = variable_get('text_resize_reset_button', FALSE);
  if ($reset_button) {
    $output = '<ul class="text-resize">';
    $output .= '<li><a href="javascript:;" class="changer" id="text_resize_decrease" title="' . t('Decrease text size') . '">A -</a></li>';
    $output .= '<li><a href="javascript:;" class="changer" id="text_resize_reset" title="' . t('Reset text size') . '">A</a></li>';
    $output .= '<li><a href="javascript:;" class="changer" id="text_resize_increase" title="' . t('Increase text size') . '">A +</a></li>';
    $output .= '</ul>';
  }
  else {
    $output = '<ul class="text-resize">';
    $output .= '<li><a href="javascript:;" class="changer" id="text_resize_decrease" title="' . t('Decrease text size') . '">A -</a></li>';
    $output .= '<li><a href="javascript:;" class="changer" id="text_resize_increase" title="' . t('Increase text size') . '">A +</a></li>';
    $output .= '</ul>';
  }
  return $output;
}

/**
 * Process variables for search-block-form.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see search-block-form.tpl.php
 */
function nerra_preprocess_search_block_form(&$variables) {
  $variables['search'] = array();
  $hidden = array();
  // Provide variables named after form keys so themers can print each element
  // independently.
  foreach (element_children($variables['form']) as $key) {
    $type = $variables['form'][$key]['#type'];
    if ($type == 'hidden' || $type == 'token') {
      $hidden[] = render($variables['form'][$key]);
    }
    else {
      if ($key == 'actions') {
        $variables['search'][$key] = render($variables['form'][$key]['submit']);
      }
      else {
        $variables['search'][$key] = render($variables['form'][$key]);
      }
    }
  }
  // Hidden form elements have no value to themers. No need for separation.
  $variables['search']['hidden'] = implode($hidden);
  // Collect all form elements to make it easier to print the whole form.
  $variables['search_form'] = implode($variables['search']);
}
