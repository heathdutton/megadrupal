<?php
/**
 * @file
 * Admin pages functionality module commit_author.
 */

/**
 * Form commit_author module settings.
 */
function commit_author_admin_settings_form() {
  $form = array();

  $form['commit_author_show_core'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show author if notice is from drupal core.'),
    '#default_value' => variable_get('commit_author_show_core', TRUE),
  );

  $form['commit_author_show_contrib'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show author if notice is from contrib modules.'),
    '#default_value' => variable_get('commit_author_show_contrib', TRUE),
  );

  $form['commit_author_show_custom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show author if notice is from custom modules.'),
    '#default_value' => variable_get('commit_author_show_custom', TRUE),
  );

  $form['commit_author_show_theme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show author if notice is from themes.'),
    '#default_value' => variable_get('commit_author_show_theme', TRUE),
  );

  $form['commit_author_not_author'] = array(
    '#type' => 'checkbox',
    '#title' => t("Do not show author if message is 'Not Committed Yet'"),
    '#default_value' => variable_get('commit_author_not_author', FALSE),
  );

  return system_settings_form($form);
}
