<?php

/**
* Plugin definition
*/
$plugin = array(
  'title' => t('Opentable reservation form'),
  'single' => TRUE,
  'category' => array(t('Opentable')),
  'edit form' => 'restaurant_opentable_opentable_reservation_form_content_type_form',
  'defaults' => array(
    'mode' => 'standard',
  ),
);


/**
 * Settings Form
 */
function restaurant_opentable_opentable_reservation_form_content_type_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['mode'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#options' => array(
      'standard' => t('Standard'),
      'tall' => t('Tall'),
      'wide' => t('Wide'),
    ),
    '#required' => TRUE,
    '#default_value' => !empty($conf['mode']) ? $conf['mode'] : 'standard',
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );

  return $form;
}

function restaurant_opentable_opentable_reservation_form_content_type_form_submit($form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (!empty($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Render Callback
 */
function restaurant_opentable_opentable_reservation_form_content_type_render($subtype, $conf, $args) {
  $block = new stdClass();
  $url = _restaurant_opentable_get_url();
  $info = _restaurant_opentable_get_info($url);
  $mode = $conf['mode'];

  $block->title = isset($conf['title']) ? $conf['title'] : t('Reserve on Opentable');
  $block->content = theme('restaurant_opentable_reservation_form', array(
    'restaurant_id' => $info['restaurant_id'],
    'mode' => $mode,
  ));

  return $block;
}
