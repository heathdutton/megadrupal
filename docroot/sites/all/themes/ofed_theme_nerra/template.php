<?php

/**
 * @file
 * Needs to be documented.
 */

/**
 * Register a theme implementations.
 * @return array
 *   Return empty array.
 */
function nerra_theme() {
  return array();
}

/**
 * Common Implementation.
 */
require_once 'template_generic.php';
require_once 'template_menu.php';
require_once 'template_view.php';
require_once 'template_module.php';

/**
 * Needs to be documented.
 */
function nerra_links__locale_block(&$variables) {
  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  global $language_url;
  $output = '';

  if (count($links) > 0) {
    $output = '';

    // Treat the heading first if it is present to prepend it to the
    // list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading
        // is a string.
        $heading = array(
          'text' => $heading,
          // Set the default level of the heading.
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

    $output .= '<ul' . drupal_attributes($attributes) . '>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = array($key);

      // Add first, last and active classes to the list of links
      // to help out themers.
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page())) && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'active';
      }
      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        // REPLACE THE DEFAULT BEHAVIOR.
        // CHANGE THE VALUE TO USE THE SHORT TAG AND NOT THE FULL NAME.
        $class_string = implode('|', $attributes['class']);
        if (preg_match('#(language-switcher)#', $class_string)) {
          $output .= l($key, $link['href'], $link);
        }
        else {
          $output .= l($link['title'], $link['href'], $link);
        }
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for
        // adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }

        // REPLACE THE DEFAULT BEHAVIOR
        // IF CONTENT IS NOT TRANSLATED, THE LANGUAGE IN LANGUAGE SWITCHER CAN
        // NOT LINK TO FRONTPAGE.
        $class_string = implode('|', $attributes['class']);
        if (preg_match('#(language-switcher)#', $class_string)) {
          $output .= l($key, '', $link);
        }
        else {
          $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
        }
      }

      $i++;
      $output .= "</li>\n";
    }
    $output .= '</ul>';
  }

  return $output;
}
