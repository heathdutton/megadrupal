<?php

/**
 * @file
 * Multilingual variable definitions
 *
 * @see http://drupal.org/project/variable
 */


/**
 * Implements hook_variable_info().
 */
function uc_out_of_stock_variable_info($options) {

  // This is the very minimum we need, some descriptive name.

  $variable['uc_out_of_stock_text'] = array(
    'title' => t('Out of stock replacement HTML'),
    'type' => 'text_format',
    'description' => t('The HTML that will replace the Add To Cart button if no stock is available.'),
    'group' => 'uc_out_of_stock',
  );

  return $variable;
}

/**
 * Implements hook_variable_group_info().
 */
function uc_out_of_stock_variable_group_info() {
  $groups['uc_out_of_stock'] = array(
    'title' => t('Out of stock notification settings'),
    'description' => t('Site information and maintenance mode'),
    //'access' => 'administer store',
    'path' => array('admin/store/settings/uc_out_of_stock'),
  );
  return $groups;
}

