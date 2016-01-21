<?php

/**
 * @file
 * Contains the administration pages for HTML HEAD Comment.
 *
 */

function html_head_comment_settings($form_state) {
  
drupal_add_css("#edit-html-head-comment-message {font-family: monospace; height: 250px;}", 'inline');

$form['html_head_comment_active'] = array(
  '#type' => 'checkbox',
  '#title' => t('Activate HTML HEAD Comment'),
  '#description' => t('Do you want the html comment to be visible in the html source?'),
  '#default_value' => variable_get('html_head_comment_active', TRUE),
);

  $form['html_head_comment_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#description' => t('This is the message which is placed inside a html comment: &lt;!-- YOUR MESSAGE HERE --&gt; '),
    '#default_value' => filter_xss(variable_get('html_head_comment_message', 'Site Design and Development by Drupal - http://drupal.org')),
    '#cols' => 80,
    '#rows' => 30,
  );
  return system_settings_form($form);
}
