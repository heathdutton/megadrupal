<?php

/**
 * @file
 * Theme and template preprocess functions.
 */

/**
 * Preprocess affiliate store product page.
 *
 * @see affiliate-store.tpl.php
 */
function template_preprocess_affiliate_store(&$variables) {
  $build = $variables['elements'];
  $message = &$variables['message'];
  $browse = &$variables['browse'];
  $browse_title = &$variables['browse_title'];
  $browse_items = &$variables['browse_items'];
  $products = &$variables['products'];
  $term = &$variables['term'];
  $term_name = &$variables['term_name'];
  $tid = &$variables['tid'];
  $vid = &$variables['vid'];

  $browse_title = $build['#browse_title'];
  $browse_items = $build['#browse_items'];
  $term = $build['#term'];

  $message = drupal_render($build['message']);
  // Only display browse fieldset if there is browse item.
  $browse = $browse_items ? drupal_render($build['browse']) : '';
  $products = drupal_render($build['nodes']);
  $products .= drupal_render($build['pager']);

  // Term information of currently displayed product category.
  if ($term) {
    $term_name = affiliate_store_get_term_synonym($term, $term->name);
    $term_name = check_plain($term_name);
    $tid = $term->tid;
    $vid = $term->vid;
    $vocab = affiliate_store_get_vocabulary_name($vid);

    // Allow template override by term and vocabulary.
    $variables['theme_hook_suggestions'][] = "affiliate_store__$vocab";
    $variables['theme_hook_suggestions'][] = (
      "affiliate_store__{$vocab}__" . drupal_strtolower($term_name)
    );
    $variables['theme_hook_suggestions'][] = "affiliate_store__$tid";
  }
  else {
    $term_name = '';
    $tid = 0;
    $vid = 0;
  }
}

/**
 * Workaround of a core bug that does not call specific preprocess functions.
 * See issue http://drupal.org/node/939462
 */
function affiliate_store_preprocess_views_view(&$variables) {
  if ($variables['view']->name === 'affiliate_store') {
    template_preprocess_views_view__affiliate_store($variables);
  }
}

/**
 * Preprocess affiliate store views.
 *
 * @see views-view--affiliate-store.tpl.php
 */
function template_preprocess_views_view__affiliate_store(&$variables) {
  affiliate_store_add_css('views_affiliate_store');
}

/**
 * Output currency symbol from currency code.
 *
 * @param array $variables
 *   An associative array with the following keys:
 *   - code: string Standard 3-letter currency code.
 *
 * @return string
 *   Currency symbol, or uppercased currency code if no symbol found.
 *
 * @ingroup themable
 */
function theme_affiliate_store_currency($variables) {
  $code = $variables['code'];
  $currency_symbols = array(
    'CHF' => 'Fr',
    'CNY' => '元',
    'EUR' => '€',
    'GBP' => '£',
    'JPY' => '¥',
    'NOK' => 'Kr',
    'USD' => '$',
  );
  $code = drupal_strtoupper(check_plain($code));
  return !empty($currency_symbols[$code]) ? $currency_symbols[$code] : $code;
}

/**
 * Output product link.
 *
 * @param array $variables
 *   An associative array with the following keys:
 *   - text: string Link anchor text.
 *   - url: string Product URL.
 *
 * @return string
 *   Product link HTML.
 */
function theme_affiliate_store_go($variables) {
  $text = $variables['text'];
  $url = $variables['url'];
  $attributes = array(
    'class' => array('affiliate-store-go'),
    'rel' => 'nofollow',
    'target' => '_blank',
  );
  return l($text, $url, array('attributes' => $attributes));
}

/**
 * Wrap an image element as a link.
 *
 * @param array $variables
 *   An associative array with the following keys:
 *   - element: string An HTML element to wrap in a link.
 *   - url: string Destination URL when the element is clicked.
 *   - title: string (optional) Title attribute of the generated link. Default
 *     to empty.
 *
 * @return string
 *   Link HTML.
 */
function theme_affiliate_store_image($variables) {
  $element = $variables['element'];
  $url = $variables['url'];
  $title = $variables['title'];
  $options = array(
    'html' => TRUE,
    'attributes' => array('title' => check_plain($title)),
  );
  return l($element, $url, $options);
}

/**
 * Output menu tree.
 *
 * @param array $variables
 *   An associative array with the following keys:
 *   - tree: array Menu items, each menu item is an associative array with the
 *     following keys:
 *     - name: string Name of menu item.
 *     - description: string (optional) Menu item description, will be used as
 *       link title attribute if path is specified.
 *     - path: string (optional) Menu item internal path or external URL, will
 *       be output as link.
 *     - active: boolean (optional) TRUE to signify the menu item is currently
 *       active or selected.
 *     - children: array (optional) Nested menu items if any.
 *
 * @return string
 *   Menu tree HTML.
 */
function theme_affiliate_store_menu($variables) {
  $tree = $variables['tree'];
  $output = '<ul class="menu">';
  $keys = array_keys($tree);
  for ($i = 0, $count = count($keys); $i < $count; ++$i) {
    $value = $tree[$keys[$i]];
    $classes = array();

    if (!empty($value['children'])) {
      $classes[] = !empty($value['active']) ? 'expanded' : 'collapsed';
    }
    else {
      $classes[] = 'leaf';
    }

    if ($i == 0) {
      $classes[] = 'first';
    }
    elseif ($i == ($count - 1)) {
      $classes[] = 'last';
    }

    if (!empty($value['active'])) {
      $classes[] = 'active-trail';
    }

    $output .= '<li class="' . implode(' ', $classes) . '">';
    if (!empty($value['path'])) {
      $options = array();
      if (!empty($value['description'])) {
        $options['attributes']['title'] = check_plain($value['description']);
      }
      $output .= l($value['name'], $value['path'], $options);
    }
    else {
      $output .= check_plain($value['name']);
    }

    if (!empty($value['children']) && !empty($value['active'])) {
      $func = __FUNCTION__;
      $output .= $func(array('tree' => $value['children']));
    }
    $output .= '</li>';
  }
  $output .= '</ul>';
  return $output;
}
