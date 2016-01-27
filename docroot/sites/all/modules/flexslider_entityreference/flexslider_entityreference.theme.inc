<?php
/**
 * @file
 * Theming functions for the flexslider_entityreference module.
 * Mostly overrides of exisitng theme hooks by flexslider.
 */

/**
 * Template process handler for 'flexslider_entityreference'.
 */
function template_process_flexslider_entityreference(&$vars) {
  $settings =& $vars['settings'];
  $options =& $settings['optionset'];

  $vars['flexslider'] += array(
    '#theme' => 'flexslider',
    '#items' => array(),
    '#settings' => $settings,
  );
  foreach (element_children($vars['entities']) as $delta) {
    $slide =& $vars['entities'][$delta]['slide'];
    $slide['entity'] = drupal_render($slide['entity']);
    $slide['#settings'] = $vars['settings'];

    $vars['flexslider']['#items'][$delta] = array(
      'slide' => $slide,
      'thumb' => drupal_render($vars['entities'][$delta]['thumb']),
    );
  }
}

/**
 * Default theme implementation for 'flexslider_entityreference'.
 */
function theme_flexslider_entityreference($vars) {
  $output = '';
  $output .= drupal_render($vars['flexslider']);
  return $output;
}

/**
 * Default theme implementation for 'flexslider_entityreference_list'.
 *
 * @see theme_flexslider_list().
 */
function theme_flexslider_entityreference_list($vars) {
  // Reference configuration variables
  $optionset = &$vars['settings']['optionset'];
  $items = &$vars['items'];
  $attributes = &$vars['settings']['attributes'];
  $type = &$vars['settings']['type'];

  $output = array();

  // Build the list
  if (!empty($items)) {
    $output['items'] = array(
      '#prefix' => '<' . $type . drupal_attributes($attributes) . '>',
      '#suffix' => '</' . $type . '>',
    );
    foreach ($items as $delta => $item) {
      $output['items'][$delta] = array(
        '#theme' => 'flexslider_list_item',
        '#item' => $item['slide'],
      );
      $output['items'][$delta]['#settings']['optionset'] = $optionset;
      if (!empty($item['thumb'])) {
        $output['thumbs'][$delta] = array(
          '#prefix' => '<li>',
          '#suffix' => '</li>',
          '#markup' => $item['thumb'],
        );
      }
    }
  }
  if (!empty($output['thumbs'])) {
    $output['thumbs'] += array(
      '#prefix' => '<ul' . drupal_attributes($vars['settings']['thumbs_attributes_array']) . '>',
      '#suffix' => '</ul>',
    );
  }

  return drupal_render($output);
}

/**
 * Default theme implementation for 'flexslider_entityreference_list_item'.
 *
 * @see theme_flexslider_list_item().
 */
function theme_flexslider_entityreference_list_item($vars) {
  return '<li' . drupal_attributes($vars['settings']['attributes']) . '>' . $vars['item']['entity'] . $vars['caption'] . "</li>\n";
}
