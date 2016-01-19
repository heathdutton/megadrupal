<?php

/**
 * Implements hook_ilink_info().
 */
function ilink_test_fire_ilink_info() {
  return array(
    'title' => t('Fire'),
    'description' => t('Fire description placeholder'),
    'label' => t('Fire!!'),
    'href' => 'ilink_test/fire/%color',
    'parameters' => array(),
    'js events' => array('mouseenter'),
    'attached' => array(
      'js' => array(ctools_attach_js('fire', 'ilink_test')),
    ),
    'hook menu' => array(
      'ilink_test/fire/%' => array(
        'title' => 'Fire',
        'page callback' => 'drupal_get_form',
        'page arguments' => array('ilink_test_fire', 2),
        'access callback' => TRUE,
      ),
    ),
  );
}

function ilink_test_fire($form, &$form_state, $color) {
  $form['color'] = array(
    '#type' => 'textfield',
    '#title' => t('Color'),
    '#default_value' => $color,
  );

  return $form;
}
