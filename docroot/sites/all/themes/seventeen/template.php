<?php
/**
 * @file
 * Overriding default theme functions.
 */

/**
 * Implements hook_preprocess_page().
 */
function seventeen_preprocess_page(&$vars) {
  $vars['max_width'] = theme_get_setting('max_width') . 'px';
}

/**
 * Implements hook_css_alter().
 *
 * The path_to_theme() for subtheme return base theme path so
 * we cannot use hook_preprocess_html().
 *
 * @see path_to_theme()
 * @see seven_preprocess_html()
 * @see http://drupal.org/node/1026156
 */
function seventeen_css_alter(&$css) {
  $css['themes/seven/ie.css']['data'] = drupal_get_path('theme', 'seventeen') . '/css/ie.css';
}

/**
 * Implements theme_menu_local_action().
 *
 * @see http://drupal.org/node/1167350
 */
function seventeen_menu_local_action($vars) {
  $link = $vars['element']['#link'];

  $link += array(
    'href' => '',
    'localized_options' => array(),
  );
  $link['localized_options']['attributes']['class'][] = 'button';
  $link['localized_options']['attributes']['class'][] = 'add';

  return '<li>' . l($link['title'], $link['href'], $link['localized_options']) . '</li>';
}

/**
 * Override of theme_pager().
 *
 * Implement "Showing 1-50 of 2345  Next 50 >" type of output.
 */
function seventeen_pager($vars) {
  $tags = $vars['tags'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];
  $quantity = $vars['quantity'];
  global $pager_page_array, $pager_total, $pager_total_items, $pager_limits;

  $total_items = $pager_total_items[$element];

  if ($total_items == 0) {
    // No items to display.
    return;
  }

  $total_pages = $pager_total[$element];
  $limit = $pager_limits[$element];
  $showing_min = $pager_page_array[$element] * $limit + 1;
  $showing_max = min(($pager_page_array[$element] + 1) * $limit, $total_items);
  $pager_current = $pager_page_array[$element];

  $output = '<div class="short-pager">';
  if ($pager_current > 0) {
    // Add link to the first page.
    $vars = array(
      'text' => t('First'),
      'attributes' => array('title' => t('Go to the first page')),
      'element' => $element,
    );
    $output .= '<div class="short-pager-first">' . theme('pager_link', $vars) . '</div>';

    // Add link to prev page.
    $vars = array(
      'text' => t('Prev @limit', array('@limit' => $limit)),
      'page_new' => pager_load_array($pager_current - 1, $element, $pager_page_array),
      'element' => $element,
      'parameters' => $parameters,
      'attributes' => array('title' => t('Go to the previous page')),
    );
    $output .= '<div class="short-pager-prev">' . theme('pager_link', $vars) . '</div>';
  }

  $output .= '<div class="short-pager-main">' . t('Showing @range <span class="short-pager-total">of @total</span>', array('@range' => $showing_min . ' - ' . $showing_max, '@total' => $total_items)) . '</div>';

  if (($pager_current < ($total_pages - 1)) && ($total_pages > 1)) {
    // Add link to next page.
    $vars = array(
      'text' => t('Next @limit', array('@limit' => $limit)),
      'page_new' => pager_load_array($pager_current + 1, $element, $pager_page_array),
      'element' => $element,
      'parameters' => $parameters,
      'attributes' => array('title' => t('Go to the previous page')),
    );
    $output .= '<div class="short-pager-next">' . theme('pager_link', $vars) . '</div>';

    // Add link to last page.
    $vars = array(
      'text' => t('Last'),
      'attributes' => array('title' => t('Go to the last page')),
      'element' => $element,
      'page_new' => pager_load_array($total_pages - 1, $element, $pager_page_array),
    );
    $output .= '<div class="short-pager-last">' . theme('pager_link', $vars) . '</div>';
  }
  // Close tag for short-pager.
  $output .= '</div>';

  return $output;
}

/**
 * Implements theme_checkbox().
 */
function seventeen_checkbox($variables) {
  if (!theme_get_setting('style_checkboxes')) {
    return theme_checkbox($variables);
  }
  drupal_add_css(drupal_get_path('theme', 'seventeen') . '/css/checkboxes.css');
  $element = $variables['element'];
  $element['#attributes']['type'] = 'checkbox';
  $attributes = array('id', 'name', '#return_value' => 'value');
  element_set_attributes($element, $attributes);

  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-checkbox'));
  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';
  $output .= '<label for="' . $element['#attributes']['id'] . '"><span></span></label>';
  return $output;
}
