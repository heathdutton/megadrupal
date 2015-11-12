<?php

/**
 * @file
 * Contains custom theme functions.
 */

/**
 * Implements hook_theme().
 */
function circle_theme() {
  return array(
    'nomarkup' => array(
      'render element' => 'element',
      'function' => 'circle_nomarkup',
    ),
    'clean' => array(
      'function' => 'circle_clean',
      'variables' => array('variables' => NULL, 'key' => NULL),
    ),
  );
}

/**
 * Alternatvie to field when you need clean content.
 */
function circle_nomarkup(&$variables) {
  // Use it like this:
  // $content['field_NAME']['#theme'] = "nomarkup";
  $output = '';
  if (!empty($variables['items'])) {
    foreach ($variables['items'] as $item) {
      $output .= drupal_render($item);
    }
  }
  return $output;
}

/**
 * Implements hook_submittedby().
 */
function circle_clean(&$variables, $key) {
  // Render all the variables we get from node preprocess.
  return drupal_render($variables);
}

/**
 * Returns rendarable array from provided elements.
 */
function _circle_get_element($name, $variables) {
  $elements = array(
    'submitted' => array(
      'label' => array(
        '#prefix' => '<span class="submitted-by-name">',
        '#markup' => t('Submitted by') . ' ',
        '#suffix' => '</span>',
        '#weight' => -20,
      ),
      'username' => array(
        '#markup' => isset($variables['name']) ? $variables['name'] : '',
        '#weight' => -15,
      ),
      'on_text' => array(
        '#prefix' => '<span class="submitted-by-on">',
        '#markup' => ' ' . t('on') . ' ',
        '#suffix' => '</span>',
        '#weight' => -10,
      ),
      'datetime' => array(
        '#prefix' => '<span class="submitted-by-date">',
        '#markup' => isset($variables['date']) ? $variables['date'] : '',
        '#suffix' => '</span>',
        '#weight' => -5,
      ),
    ),
  );
  return isset($elements[$name]) ? $elements[$name] : array();
}
