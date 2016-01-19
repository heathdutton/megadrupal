<?php

/**
 * @file
 * Theme functions for the photo gallery pager.
 */

/**
 * Returns HTML for a link to a specific query result page.
 *
 * Tweak the pager links for the photo gallery item view.
 * @see theme_pager_link()
 *
 * @ingroup themeable
 */
function theme_g2migrate_pager_link($variables) {
  $text = $variables['text'];
  $page_new = $variables['page_new'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $attributes = $variables['attributes'];
  $fragment = $variables['fragment'];
  $href = isset($variables['href']) ? $variables['href'] : $_GET['q'];

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = drupal_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;
    if (!isset($titles)) {
      $titles = array(
        t('« first') => t('Go to first page'),
        t('‹ previous') => t('Go to previous page'),
        t('next ›') => t('Go to next page'),
        t('last »') => t('Go to last page'),
      );
    }
    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    elseif (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  // @todo l() cannot be used here, since it adds an 'active' class based on the
  //   path only (which is always the current path for pager links). Apparently,
  //   none of the pager links is active at any time - but it should still be
  //   possible to use l() here.
  // @see http://drupal.org/node/1410574
  $options = array('query' => $query);
  if ($fragment) {
    $options['fragment'] = $fragment;
  }
  $attributes['href'] = url($href, $options);
  return '<a' . drupal_attributes($attributes) . '>' . check_plain($text) . '</a>';
}

/**
 * Custom pager.
 *
 * Add fragments to the links. Based on theme_views_mini_pager().
 */
function theme_g2migrate_pager($vars) {
  global $pager_page_array, $pager_total;

  $tags = $vars['tags'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];
  $fragments = $vars['fragments'];

  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Define common options for theme('g2migrate_pager_link').
  $base = array(
    'element' => $element,
    'parameters' => $parameters,
  );

  if ($pager_max > 1) {

    if ($pager_current == 1) {
      $li_previous = "&nbsp;";
    }
    else {
      $page_new = pager_load_array($pager_page_array[$element] - 1, $element, $pager_page_array);
      $li_previous = theme('g2migrate_pager_link',
        $base + array(
          'text' => (isset($tags[1]) ? $tags[1] : t('‹‹')),
          'page_new' => $page_new,
          'fragment' => $fragments['previous'],
        )
      );
    }

    if ($pager_current == $pager_max) {
      $li_next = "&nbsp;";
    }
    else {
      $page_new = pager_load_array($pager_page_array[$element] + 1, $element, $pager_page_array);
      $li_next = theme('g2migrate_pager_link',
        $base + array(
          'text' => (isset($tags[3]) ? $tags[3] : t('››')),
          'page_new' => $page_new,
          'fragment' => $fragments['next'],
        )
      );
    }

    $items_per_page = g2migrate_settings('items_per_page');
    $page_new = floor(($pager_current - 1) / $items_per_page);
    $li_current = theme('g2migrate_pager_link',
      $base + array(
        'text' => t('Back to list view'),
        'page_new' => array($page_new),
        'fragment' => $fragments['current'],
        'href' => preg_replace('/item$/', 'list', $_GET['q']),
      )
    );

    $items[] = array(
      'class' => array('pager-previous'),
      'data' => $li_previous,
    );

    $items[] = array(
      'class' => array('pager-current'),
      'data' => $li_current,
    );

    $items[] = array(
      'class' => array('pager-next'),
      'data' => $li_next,
    );
    return theme('item_list',
      array(
        'items' => $items,
        'title' => NULL,
        'type' => 'ul',
        'attributes' => array('class' => array('pager')),
      )
    );
  }
}
