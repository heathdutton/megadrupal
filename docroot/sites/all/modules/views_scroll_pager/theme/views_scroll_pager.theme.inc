<?php

/**
 * @file
 * Theme functions Views scroll pager module.
 */

/**
 * Returns layout for Views horizontal scroll pager.
 *
 * @param array $variables
 *   Theme variables.
 *
 * @return null|string
 *   HTML layout.
 */
function theme_views_scroll_pager_horizontal_scroll($variables) {
  global $language, $pager_page_array, $pager_total;

  // Main variables.
  $element = $variables['element'];
  $parameters = $variables['parameters'];

  // Markers calculation.
  $pager_current = $pager_page_array[$element] + 1;
  $pager_first = 1;
  $pager_max = $pager_total[$element];

  if ($pager_max > 1) {
    $items = array();

    // Pager build.
    for ($i = $pager_first; $i <= $pager_max; $i++) {
      if ($i < $pager_current) {
        $items[] = array(
          'class' => array('pager-item'),
          'data' => theme('pager_previous',
            array(
              'text' => $i,
              'element' => $element,
              'interval' => ($pager_current - $i),
              'parameters' => $parameters,
            )
          ),
        );
      }
      if ($i == $pager_current) {
        $items[] = array(
          'class' => array('pager-current'),
          'data' => $i,
        );
      }
      if ($i > $pager_current) {
        $items[] = array(
          'class' => array('pager-item'),
          'data' => theme('pager_next',
            array(
              'text' => $i,
              'element' => $element,
              'interval' => ($i - $pager_current),
              'parameters' => $parameters,
            )
          ),
        );
      }
    }

    // Pager classes.
    $classes = array();
    $classes[] = $variables['views_class'];

    $output = theme('views_scroll_pager_layout',
      array(
        'pager_class' => ' ' . implode(',', $classes),
        'pager_items' => theme('item_list', array('items' => $items, 'attributes' => array('class' => array('pager')))),
        'total_pages' => format_plural(
          $pager_max,
          '@count page',
          '@count pages',
          array(),
          array('langcode' => $language->language)
        ),
      )
    );

    // Adds jScroll library.
    libraries_load('jscrollpane');
    ctools_add_js('views_scroll_pager', 'views_scroll_pager');
    ctools_add_css('views_scroll_pager', 'views_scroll_pager');

    // Adds info about current and max pages.
    $settings = array(
      $variables['views_class'] => array(
        'currentPage' => $pager_current,
        'totalPages' => $pager_max,
      ),
    );

    drupal_add_js(array('views_scroll_pager' => $settings), 'setting');
  }

  return isset($output) ? $output : NULL;
}
