<?php

/**
 * Implementation of hook_configure().
 *
 */

function booktree_configure() {
  $form = array();
  $form['booktree_start'] = array(
    '#type' => 'textfield',
    '#title' => t('Root Node ID'),
    '#required' => TRUE,
    '#default_value' => variable_get('booktree_start', 5),
    '#description' => t('Start point of the tree (the root).')

  );

  $form['booktree_deep'] = array(
    '#type' => 'textfield',
    '#title' => t('Deep Max'),
    '#required' => TRUE,
    '#default_value' => variable_get('booktree_deep', 5),
    '#description' => t('Max deep of the tree with the root.')

  );
  $form['booktree_trim'] = array(
    '#type' => 'textfield',
    '#title' => t('Trimmer'),
    '#required' => TRUE,
    '#default_value' => variable_get('booktree_trim', 45),
    '#description' => t('Max lenght of title.')

  );
  return system_settings_form($form);

}