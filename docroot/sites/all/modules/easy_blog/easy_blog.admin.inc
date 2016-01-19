<?php

/**
 * @file
 * Settings forms
 */

/**
 * Create settings form in drupal configuration setings section for module.
 */
function easy_blog_admin_settings() {
  $form['easy_blog_settings_archive'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blog Archive'),
    '#collapsible' => TRUE,
  );

  $form['easy_blog_settings_archive']['easy_blog_settings_nodes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show post titles in Archive?'),
    '#default_value' => variable_get('easy_blog_settings_nodes', 0),
    '#description' => t('Select "Yes" for show node titles in archive tree.'),
  );

  $form['easy_blog_settings_archive']['easy_blog_settings_count_nodes'] = array(
    '#type' => 'textfield',
    '#size' => 3,
    '#title' => t('Number of post titles in Archive'),
    '#description' => t('0 - show all titles.'),
    '#default_value' => variable_get('easy_blog_settings_count_nodes', 10),
  );

  return system_settings_form($form, TRUE);
}
