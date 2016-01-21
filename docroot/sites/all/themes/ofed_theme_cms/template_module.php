<?php
/**
 * @file
 * CMS theme template file
 */

/**
 * Theming of the module text_resize.
 */
function cms_theme_text_resize_block() {
  if (_get_text_resize_reset_button() == TRUE) {
    $output = t('<a href="javascript:;" class="changer" id="text_resize_decrease"><sup>-</sup>A</a> <a href="javascript:;" class="changer" id="text_resize_reset">A</a> <a href="javascript:;" class="changer" id="text_resize_increase"><sup>+</sup>A</a><div id="text_resize_clear"></div>');
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
function cms_theme_preprocess_search_block_form(&$variables) {
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
