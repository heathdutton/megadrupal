<?php

/**
 * Create a dropbutton menu.
 *
 * @param $title
 *   The text to place in the clickable area to activate the dropbutton. This
 *   text is indented to -9999px by default.
 * @param $links
 *   A list of links to provide within the dropbutton, suitable for use
 *   in via Drupal's theme('links').
 * @param $image
 *   If true, the dropbutton link is an image and will not get extra decorations
 *   that a text dropbutton link will.
 * @param $class
 *   An optional class to add to the dropbutton's container div to allow you
 *   to style a single dropbutton however you like without interfering with
 *   other dropbuttons.
 */
function twbs_links__ctools_dropbutton($vars) {
  $links = $vars['links'];

  $output = '';

  if (count($links)) {
    $output .= '<div class="btn-group">';

    $link = array_shift($links);
    $link['attributes']['role'] = 'button';
    $link['attributes']['class'][] = 'btn';
    if (!preg_grep('/^btn-.*$/', $link['attributes']['class'])) {
      $link['attributes']['class'][] = 'btn-default';
    }
    $output .= l($link['title'], $link['href'], $link);

    if (count($links)) {
      $attributes = array(
        'type' => 'button',
        'data-toggle' => 'dropdown',
        'class' => array(
          'btn',
          'btn-default',
          'dropdown-toggle',
        ),
      );
      $output .= '<button ' . drupal_attributes($attributes) . '>';
      $output .= '<span class="caret"></span>';
      $output .= '<span class="sr-only">Toggle Dropdown</span>';
      $output .= '</button>';

      $output .= theme('links', array(
        'links' => $links,
        'heading' => '',
        'attributes' => array(
          'class' => array(
            'dropdown-menu',
          ),
          'role' => 'menu',
        ),
      ));
    }

    $output .= '</div>';
  }

  return $output;
}
