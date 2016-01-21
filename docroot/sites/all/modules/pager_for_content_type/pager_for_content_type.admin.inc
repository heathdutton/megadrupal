<?php

/**
 * @file
 * Describe configuration section for module
 */

/**
 * General configuration form for controlling "pager for content type" module.
 * @return Array
 *   Form data
 */
function pager_for_content_type_admin_settings() {
  $node_types = node_type_get_types();

  $more_links_options = array(
    '0' => 'Off',
    '4' => 4,
    '5' => 6,
    '10' => 10,
  );

  $form['pager_for_content_type_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['pager_for_content_type_general']['pager_for_content_type_previous_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Previous" text'),
    '#default_value' => variable_get('pager_for_content_type_previous_text', t('NEWER POSTS')),
    '#size' => 30,
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $form['pager_for_content_type_general']['pager_for_content_type_next_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Next" text'),
    '#default_value' => variable_get('pager_for_content_type_next_text', t('OLDER POSTS')),
    '#size' => 30,
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $form['pager_for_content_type_content_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content type options'),
    '#description' => t('Pager will available on checked content types (only in full view mode)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  foreach ($node_types as $node_type) {

    $form['pager_for_content_type_content_type'][$node_type->type] = array(
      '#type' => 'fieldset',
      '#title' => t($node_type->name),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $key = $node_type->type . '_pager_for_content_type_on';
    $form['pager_for_content_type_content_type'][$node_type->type][$key] = array(
      '#type' => 'checkbox',
      '#title' => t('On'),
      '#default_value' => variable_get($key, 0),
    );

    $key = $node_type->type . '_pager_for_content_type_author';
    $form['pager_for_content_type_content_type'][$node_type->type][$key] = array(
      '#type' => 'checkbox',
      '#title' => t('Pager by node author'),
      '#default_value' => variable_get($key, 0),
    );

    $key = $node_type->type . '_pager_for_content_type_more_links';
    $form['pager_for_content_type_content_type'][$node_type->type][$key] = array(
      '#type' => 'select',
      '#title' => t('Show more nodes titles after the pager'),
      '#description' => t('First half before pager, second half after pager'),
      '#default_value' => variable_get($key, 0),
      '#options' => $more_links_options,
    );
  }
  return system_settings_form($form);
}
