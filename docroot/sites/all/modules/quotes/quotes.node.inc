<?php

/**
 * @file
 * The quotes module allows users to maintain a list of quotes that
 * can be displayed in any number of administrator-defined quote
 * blocks.
 *
 * @copyright Copyright (c) 2003-2007 Jim Riggs.  All rights reserved.
 * @author Jim Riggs <drupal at jim and lissa dot com>
 */

/**
 * Implements hook_form().
 */
function quotes_form($form, $form_state) {
  $form = array('quotes_data' => array());

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => FALSE,
    '#default_value' => $node->title,
    '#description' => t("Enter the title for the quote(s). If you include the variable %id, it will be replaced by the new quote's ID."),
    '#weight' => -10,
  );

  // Set default value, php 5.3 does not like undefined properties, .
  $quotes_import_format = 'single';
  if (user_access('import quotes')) {
    $form['quotes_data']['quotes_import_format'] = array(
      '#type' => 'radios',
      '#title' => t('Format'),
      '#required' => TRUE,
      '#default_value' => isset($node->quotes_format) ? $node->quotes_format : 'single',
      '#options' => array(
        'single' => t('Single quote.'),
        'text' => t('Import tab-separated text.'),
        'fortune' => t('Import Fortune file.'),
      ),
    );
  }
  else {
    $form['quotes_data']['quotes_import_format'] = array(
      '#type' => 'value',
      '#value' => 'single',
    );
  }

  $form['quotes_data']['quotes_author'] = array(
    '#type' => 'textarea',
    '#title' => t('Author'),
    '#rows' => 2,
    '#maxlength' => 1023,
    '#default_value' => $node->quotes_author,
  );

  $form['quotes_data']['quotes_citation'] = array(
    '#type' => 'textarea',
    '#title' => t('Citation'),
    '#rows' => 2,
    '#maxlength' => 1023,
    '#default_value' => $node->quotes_citation,
  );

  if (user_access('promote quotes to block')) {
    $form['quotes_data']['quotes_promote'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display in quote blocks'),
      '#default_value' => (isset($node->quotes_promote) ? $node->quotes_promote : 1),
    );
  }

  $form['quotes_data'] = array(
    '#type' => 'text_format',
    '#format' => isset($edit['format']) ? $edit['format'] : NULL,
  );
  $form_state['#redirect'] = 'quotes';

  return $form;
}

/**
 * Implements hook_validate().
 */
function quotes_validate($node, &$form) {
  // Bail if we are doing a single quote.
  if ($node->quotes_import_format == 'single') {
    return;
  }

  _quotes_parse_import($node, TRUE);
}
