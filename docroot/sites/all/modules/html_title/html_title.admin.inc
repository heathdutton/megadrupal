<?php

/**
 * @file
 * Module settings and configuration administration UI.
 *
 * @ingroup html_title
 */

/**
 * Form definition; admin settings.
 */
function html_title_admin_settings() {
  $form['description'] = array(
    '#value' => t('Only the HTML tags below may be allowed in node titles. Any tags not enabled here will be removed. Note that all HTML will be removed in feed output and JSON output in views.'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  $form['html_title_allowed_elements'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available tags'),
    '#default_value' => variable_get('html_title_allowed_elements', array('em', 'sub', 'sup')),
    '#options' => array(
      'abbr' => check_plain('<abbr>'),
      'b' => check_plain('<b>'),
      'bdi' => check_plain('<bdi>'),
      'cite' => check_plain('<cite>'),
      'code' => check_plain('<code>'),
      'em' => check_plain('<em>'),
      'i' => check_plain('<i>'),
      'strong' => check_plain('<strong>'),
      'sub' => check_plain('<sub>'),
      'sup' => check_plain('<sup>'),
      'wbr' => check_plain('<wbr>'),
    ),
  );

  return system_settings_form($form);
}
