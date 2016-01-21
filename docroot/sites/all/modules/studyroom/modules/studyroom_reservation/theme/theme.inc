<?php
/**
 * @file
 * Preprocessors and helper functions to make theming easier.
 */

/**
 * Displays the list of available reservation types for reservation creation.
 *
 * @ingroup themeable
 */
function theme_studyroom_reservation_choose_type($vars) {
  $types = $vars['types'];
  $output = '<h3>Choose type of reservation to create:</h3>';
  $output .= '<ul class="studyroom-reservation-type-list">';
  foreach ($types as $type) {
    if ($type->ui) {
      $link = array('add', $type->type, $vars['space'], $vars['datetime']);
      $output .= '<li class="clearfix">';
      $output .= '<span class="label">' . l($type->label, STUDYROOM_RESERVATION_PATH . implode('/', array_filter($link))) . '</span>';
      $output .= '<div class="description">' . filter_xss_admin($type->description) . '</div>';
      $output .= '</li>';
    }
  }
  $output .= '</ul>';

  return $output;
}

/**
 * Returns the rendered tabledrag form for the entity overview listing.
 *
 * @ingroup themeable
 */
function theme_studyroom_reservation_type_overview_tabledrag_form($variables) {
  $form = $variables['form'];
  $rows = array();
  foreach (element_children($form) as $key) {
    $form[$key]['weight']['#attributes']['class'] = array('entity-ui-weight');
    $row = $form[$key]['#row'];
    // Replace the weight column with the dropdown.
    $row['weight'] = array('data' => $form[$key]['weight']);
    $rows[$key] = array('data' => $row, 'class' => array('draggable'));
  }

  $render = array(
    '#theme' => 'table',
    '#header' => $form['#header'],
    '#rows' => $rows,
    '#empty' => t('None.'),
    '#attributes' => array('id' => 'entity-ui-overview-form'),
  );

  drupal_add_tabledrag('entity-ui-overview-form', 'order', 'sibling', 'entity-ui-weight');
  return drupal_render($render);
}
