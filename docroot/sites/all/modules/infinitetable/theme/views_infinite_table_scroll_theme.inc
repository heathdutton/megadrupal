<?php
/**
 * @file
 * Theme infinite scroll page
 */

/**
 * Pager theming level.
 */
function theme_views_infinite_table_scroll_pager($variables) {
  $tags = $variables['tags'];
  $limit = isset($variables['limit']) ? $variables['limit'] : 10;
  $view_name = $variables['view_name'];
  $element = isset($variables['element']) ? $variables['element'] : 0;
  $current_display = $variables['current_display'];
  $content_selector = isset($variables['content_selector']) ? $variables['content_selector'] : 'div.view-content';
  $items_selector = isset($variables['items_selector']) ? $variables['items_selector'] : 'div.view-content .views-row';
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];
  $tbody_height = $variables['tbody_height'];
  $thead_height = $variables['thead_height'];
  $tfoot_height = $variables['tfoot_height'];
  $view_args = $variables['view_args'];
  $pagerclass = 'pager';
  global $pager_page_array, $pager_total;
  $pager_middle = ceil($quantity / 2);
  $pager_current = $pager_page_array[$element] + 1;
  $pager_max = $pager_total[$element];
  $li_previous = theme('pager_previous', array(
    'text' => (isset($tags[1]) ? $tags[1] : t('‹‹')),
    'limit' => $limit, 'element' => $element,
    'interval' => 1,
    'parameters' => $parameters)
  );
  if (empty($li_previous)) {
    $li_previous = "&nbsp;";
  }
  $li_next = theme('pager_next',
    array(
      'text' => (isset($tags[3]) ? $tags[3] : t('››')),
      'limit' => $limit,
      'element' => $element,
      'interval' => 1,
      'parameters' => $parameters,
    )
  );
  if (empty($li_next)) {
    $li_next = "&nbsp;";
  }
  if ($pager_total[$element] > 1) {
    $items[] = array(
      'class' => array('pager-previous'),
      'data' => $li_previous,
    );
    $items[] = array(
      'class' => array('pager-current'),
      'data' => t('@current of @max', array('@current' => $pager_current, '@max' => $pager_max)),
    );
    $items[] = array(
      'class' => array('pager-next'),
      'data' => $li_next,
    );
    $settings = array(
      'views_infinite_table_scroll' => array(
        array(
          'view_name' => $view_name,
          'display' => $current_display,
          'pager_selector' => 'ul.' . $pagerclass,
          'next_selector' => 'li.pager-next a:first',
          'content_selector' => $content_selector,
          'items_selector' => $items_selector,
          'tbody_height' => $tbody_height,
          'thead_height' => $thead_height,
          'tfoot_height' => $tfoot_height,
          'page_total' => current($pager_total),
          'view_args' => $view_args,
        ),
      ),
    );
    drupal_add_js($settings, array('type' => 'setting', 'scope' => JS_DEFAULT));
    if (module_exists('libraries')) {
      drupal_add_js(libraries_get_path('tbodyscroll') . '/tbodyscroll.js');
    }
    else {
      drupal_add_js(drupal_get_path('module', 'views_infinite_table_scroll') . '/tbodyscroll.js');
    }
    drupal_add_js(drupal_get_path('module', 'views_infinite_table_scroll') . '/js/views_infinite_table_scroll.js');
    return theme('item_list',
      array(
        'items' => $items,
        'title' => NULL,
        'type' => 'ul',
        'attributes' => array('class' => array($pagerclass)),
      )
    );
  }
}
