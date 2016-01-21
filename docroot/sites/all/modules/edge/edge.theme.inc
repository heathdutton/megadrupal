<?php

/**
 * @file
 * Cutting Edge theme functions.
 */

/**
 * Returns HTML for a list or nested list of items.
 *
 * @param $variables
 *   An associative array containing:
 *   - items: A list of items to render. String values are rendered as is. Each
 *     item can also be an associative array containing:
 *     - data: The string content of the list item.
 *     - children: A list of nested child items to render that behave
 *       identically to 'items', but any non-numeric string keys are treated as
 *       HTML attributes for the child list that wraps 'children'.
 *     - ...: All other key/value pairs are used as HTML attributes for the list
 *       item in 'data'.
 *   - title: The title of the list.
 *   - type: The type of list to return (e.g. "ul", "ol").
 *   - attributes: The attributes applied to the list element.
 */
function theme_edge_item_list($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $list_attributes = $variables['attributes'];

  $output = '';
  if ($items) {
    $output .= '<' . $type . drupal_attributes($list_attributes) . '>';

    $num_items = count($items);
    $i = 0;
    foreach ($items as $key => $item) {
      $i++;
      $attributes = array();

      if (is_array($item)) {
        $value = '';
        if (isset($item['data'])) {
          $value .= $item['data'];
        }
        $attributes = array_diff_key($item, array('data' => 0, 'children' => 0));

        // Append nested child list, if any.
        if (isset($item['children'])) {
          // List attributes are normally defined in the 'attributes' variable,
          // but for nested child lists, all non-numeric keys in 'children' must
          // be treated as child list attributes.
          $child_list_attributes = array();
          foreach ($item['children'] as $child_key => $child_item) {
            if (is_string($child_key)) {
              $child_list_attributes[$child_key] = $child_item;
              unset($item['children'][$child_key]);
            }
          }
          $value .= theme('item_list', array(
            'items' => $item['children'],
            'type' => $type,
            'attributes' => $child_list_attributes,
          ));
        }
      }
      else {
        $value = $item;
      }

      $attributes['class'][] = ($i % 2 ? 'odd' : 'even');
      if ($i == 1) {
        $attributes['class'][] = 'first';
      }
      if ($i == $num_items) {
        $attributes['class'][] = 'last';
      }

      $output .= '<li' . drupal_attributes($attributes) . '>' . $value . '</li>';
    }
    $output .= "</$type>";
  }

  // Only output the list container and title, if there are any list items.
  if ($output !== '') {
    if ($title !== '') {
      $title = '<h3>' . $title . '</h3>';
    }
    $output = '<div class="item-list">' . $title . $output . '</div>';
  }

  return $output;
}

/**
 * Returns HTML for a set of links.
 *
 * @param $variables
 *   An associative array containing:
 *   - links: A keyed array of links to be themed. The key for each link is used
 *     as its css class. Each link should be itself an array, with the following
 *     keys:
 *     - title: the link text
 *     - href: the link URL. If omitted, the 'title' is shown as a plain text
 *       item in the links list.
 *     - html: (optional) set this to TRUE if 'title' is HTML so it will be
 *       escaped.
 *     Array items are passed on to the l() function's $options parameter when
 *     creating the link.
 *   - attributes: A keyed array of attributes.
 *   - heading: An optional keyed array or a string for a heading to precede the
 *     links. When using an array the following keys can be used:
 *     - text: the heading text
 *     - level: the heading level (e.g. 'h2', 'h3')
 *     - class: (optional) an array of the CSS classes for the heading
 *     When using a string it will be used as the text of the heading and the
 *     level will default to 'h2'.
 *
 *     Headings should be used on navigation menus and any list of links that
 *     consistently appears on multiple pages. To make the heading invisible
 *     use the 'element-invisible' CSS class. Do not use 'display:none', which
 *     removes it from screen-readers and assistive technology. Headings allow
 *     screen-reader and keyboard only users to navigate to or skip the links.
 *     See http://juicystudio.com/article/screen-readers-display-none.php
 *     and http://www.w3.org/TR/WCAG-TECHS/H42.html for more information.
 */
function theme_edge_links($variables) {
  global $language_url;

  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  $output = '';

  if ($links) {
    // Prepend the heading to the list, if any.
    if ($heading) {
      // Convert a string heading into an array, using a H2 tag by default.
      if (is_string($heading)) {
        $heading = array('text' => $heading);
      }
      // Merge in default array properties into $heading.
      $heading += array(
        'level' => 'h2',
        'attributes' => array(),
      );
      // @todo D8: Remove backwards compatibility for $heading['class'].
      if (isset($heading['class'])) {
        $heading['attributes']['class'] = $heading['class'];
      }

      $output .= '<' . $heading['level'] . drupal_attributes($heading['attributes']) . '>';
      $output .= check_plain($heading['text']);
      $output .= '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 0;
    foreach ($links as $key => $link) {
      $i++;

      $class = array();
      // Use the array key as class name.
      $class[] = drupal_html_class($key);
      // Add odd/even, first, and last classes.
      $class[] = ($i % 2 ? 'odd' : 'even');
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }
      $item = '';

      // Handle links.
      if (isset($link['href'])) {
        $is_current_path = ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()));
        $is_current_language = (empty($link['language']) || $link['language']->language == $language_url->language);
        if ($is_current_path && $is_current_language) {
          $class[] = 'active';
        }
        // Pass in $link as $options, they share the same keys.
        $item = l($link['title'], $link['href'], $link);
      }
      // Handle title-only text items.
      else {
        // Merge in default array properties into $link.
        $link += array(
          'html' => FALSE,
          'attributes' => array(),
        );
        $item .= '<span' . drupal_attributes($link['attributes']) . '>';
        $item .= ($link['html'] ? $link['title'] : check_plain($link['title']));
        $item .= '</span>';
      }

      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';
      $output .= $item;
      $output .= '</li>';
    }

    $output .= '</ul>';
  }

  return $output;
}

