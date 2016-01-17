<?php
/**
 * @file
 * Theme functions for footer sitemap
 */

/**
 * Outputs links as a tree.
 *
 * @param array $variables
 *   Associative array of variables:
 *   array('links' => array(), 'attributes' => array(), 'exclude_mlids' => array()).
 * @return string
 *   Formatted HTML tree with links.
 */
function theme_tree_links($variables) {
  global $language;

  $links = $variables['links'];
  $attributes = $variables['attributes'] + array('class' => 'links');
  $exclude_mlids = $variables['exclude_mlids'];
  $depth = isset($variables['depth']) ? $variables['depth'] : 1;

  $output = '';

  foreach (array_keys($links) as $key) {
    if (in_array(str_replace('menu-', '', $key), $exclude_mlids)) {
      unset($links[$key]);
    }
  }

  $num_links = count($links);

  if ($num_links > 0) {
    $leaf_count = 0;
    $branch_count = 0;

    $i = 1;

    foreach ($links as $key => $link) {

      $class = array($key);

      // Add superfish-style classes
      $class[] = 'depth-' . $depth;
      if (isset($link['children']) && is_array($link['children'])) {
        $class[] = 'total-children-' . count($link['children']);

        // The following is a bit of a hack, but the only way to make it non-
        // hacky would seem to involve removing all recursion in this function
        $child_leaf_count = 0;
        $child_branch_count = 0;
        foreach ($link['children'] as $child) {
          if (isset($child['children']) && is_array($child['children'])) {
            $child_branch_count++;
          }
          else {
            $child_leaf_count++;
          }
        }
        $class[] = 'parent-children-' . $child_branch_count;
        $class[] = 'single-children-' . $child_leaf_count;
      }
      else {
        $class[] = 'fs-no-children';
      }

      // Add first, last and active classes to the list of
      // links to help out themers.
      if ($i == 1) {
        $class[] = ' first';
      }
      if ($i == $num_links) {
        $class[] = ' last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
          && (empty($link['language']) || $link['language']->language == $language->language)) {
        $class[] = ' active';
      }
      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $output .= l($link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap
        // these in <span> for adding title and class attributes
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      if (isset($link['children']) && is_array($link['children'])) {
        $output .= theme('tree_links', array(
          'links' => $link['children'],
          'attributes' => $attributes,
          'exclude_mlids' => $exclude_mlids,
        ));
      }

      $i++;
      $output .= "</li>\n";
    }

    // add superfish-style classes
    $attributes['class'] .= ' total-items-' . $num_links;
    $attributes['class'] .= ' parent-items-' . $branch_count;
    $attributes['class'] .= ' single-items-' . $leaf_count;

    // wrap the list items in a ul
    $output = '<ul' . drupal_attributes($attributes) . '>' . $output . '</ul>';
  }

  return $output;
}
