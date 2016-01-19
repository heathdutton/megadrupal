<?php


/**
 * Preprocess variables for theming the webform components form.
 */
function template_preprocess_webform_captureplus_form(&$variables) {
  $form = $variables['form'];
  $node = $form['#node'];

  $header = array(t('Field'), t('Capture+ group'), t('Capture+ field'), t('Mode'));
  $rows = array();
  if (!empty($node->webform['components'])) {
    $component_tree = array();
    $page_count = 1;
    _webform_components_tree_build($node->webform['components'], $component_tree, 0, $page_count);
    $component_tree = _webform_components_tree_sort($component_tree);
    // Build the table rows recursively.
    foreach ($component_tree['children'] as $cid => $component) {
      _webform_captureplus_form_rows($node, $cid, $component, 0, $form, $rows);
    }
  }
  else {
    $rows[] = array(array('data' => t('No Components.'), 'colspan' => 9));
  }
  $variables['rows'] = $rows;
  $variables['header'] = $header;
  $variables['form'] = $form;
}

/**
 * Theme the node components form. Use a table to organize the components.
 *
 * @return
 *   Formatted HTML form, ready for display.
 */
function theme_webform_captureplus_form($variables) {
  $output = '';
  $output .= drupal_render_children($variables['form']['warning']);
  $output .= drupal_render($variables['form']['settings']);
  $output .= theme('table', array('header' => $variables['header'], 'rows' => $variables['rows'], 'attributes' => array('id' => 'webform-components')));
  $output .= drupal_render_children($variables['form']);
  return $output;
}
