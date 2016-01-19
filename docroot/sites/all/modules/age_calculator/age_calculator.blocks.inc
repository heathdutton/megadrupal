<?php

/**
 * @file
 * Defines Age calculator block.
 */

/**
 * Implements hook_block_info().
 */
function age_calculator_block_info() {
  $blocks = array();
  $blocks['age_calculator'] = array(
    'info' => t('Age Calculator'),
  );
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function age_calculator_block_view($delta = '') {
  $block = array();
  switch ($delta) {
    case 'age_calculator':
      $block['subject'] = '';
      $block_age_calculator_form = drupal_get_form("age_calculator_form");
      $block['content'] = $block_age_calculator_form;
      break;
  }
  return $block;
}
