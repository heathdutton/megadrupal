<?php

/**
 * @file
 * Administrative settings forms for the GitHub WebHook module.
 */

/**
 * Administrative settings for the GitHub WebHook module.
 *
 * @see github_webhook_settings_validate()
 * @see github_webhook_settings_submit()
 *
 * @ingroup forms
 */
function github_webhook_settings($form, &$form_state) {

  $form['github_webhook_url'] = array(
    '#title' => t('WebHook URL'),
    '#type' => 'textfield',
    '#default_value' => variable_get('github_webhook_url', 'github/webhook/%github_webhook_token'),
    '#description' => t('The relative URL of the GitHub webhook endpoint exposed by this application.'),
  );

  $form['github_webhook_require_token'] = array(
    '#title' => t('Require a token'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('github_webhook_require_token', 1),
    '#description' => t('Require that a valid security token is passed through the <code>%github_webhook_token</code> wildcard loader in the endpoint path.'),
  );

  $form['github_webhook_token'] = array(
    '#title' => t('Security token'),
    '#type' => 'textfield',
    '#default_value' => github_webhook_token(),
    '#states' => array(
      'visible' => array(
        ':input[name="github_webhook_require_token"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['github_webhook_token_position'] = array(
    '#type' => 'value',
    '#value' => variable_get('github_webhook_token_position', 0),
  );

  $form['#validate'][] = 'github_webhook_settings_validate';
  $form['#submit'][] = 'github_webhook_settings_submit';

  return system_settings_form($form);
}

/**
 * Form validation handler for github_webhook_settings().
 *
 * Forces a menu rebuild if the webhook URL changes, ensures that the
 * %github_webhook_token wildcard is present in the path if a security token is
 * required.
 *
 * @see github_webhook_settings_submit()
 */
function github_webhook_settings_validate($form, &$form_state) {
  $position = github_webhook_argument_position($form_state['values']['github_webhook_url']);
  form_set_value($form['github_webhook_token_position'], $position, $form_state);
  if ($form_state['values']['github_webhook_require_token'] && $position === FALSE) {
    form_set_error('github_webhook_url', t('Path must contain the %github_webhook_token wildcard loader'));
  }
}

/**
 * Form submission handler for github_webhook_settings().
 *
 * Forces a menu rebuild if the webhook URL changes.
 *
 * @see github_webhook_settings_validate()
 */
function github_webhook_settings_submit($form, &$form_state) {
  $original_url = $form_state['complete form']['github_webhook_url']['#default_value'];
  if ($original_url != $form_state['values']['github_webhook_url']) {
    variable_set('menu_rebuild_needed', TRUE);
  }
}

/**
 * Returns the argument position of the %github_webhook_token argument.
 *
 * @param string $path
 *   The path to the GitHub webhook endpoint.
 *
 * @retunr int|FALSE
 */
function github_webhook_argument_position($path) {
  $position = 0;
  $parts = explode('/', $path);
  foreach ($parts as $part) {
    if ('%github_webhook_token' == $part) {
      return $position;
    }
    elseif (0 === strpos($part, '%')) {
      $position++;
    }
  }
  return FALSE;
}
