<?php
/**
 * @file
 * This CTools plugin grabs a cookie value
 * and lets you use it as a context in panels.
 */

$plugin = array(
  'title' => t("Cookie context"),
  'description' => t('Grabs a cookie value and adds it as a ctools context'),
  'context' => 'ctools_cookie_context_context_create_cookiecontext',
  'context name' => 'cookiecontext',
  'keyword' => 'cookiecontext',
  'edit form' => 'ctools_cookie_context_settings_form',
  'convert list' => 'ctools_cookie_context_convert_list',
  'convert' => 'ctools_cookie_context_convert',
);

/**
 * Create a context, either from manual configuration (form).
 *
 * @param boolean $empty
 *   If true, just return an empty context.
 * @param array $data
 *   If from settings form, an array as from a form. If from argument, a string.
 * @param boolean $conf
 *   TRUE if the $data is coming from admin configuration.
 *   FALSE if it's from a URL arg.
 *
 * @return array
 *   a Context object.
 */
function ctools_cookie_context_context_create_cookiecontext($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('cookiecontext');
  $context->plugin = 'cookiecontext';
  if ($empty) {
    var_dump('empty');
    return $context;
  }

  // Grab the cookie value if the cookie exist.
  if (isset($_COOKIE[$data['cookie_name']])) {
    $cookie_context = $_COOKIE[$data['cookie_name']];

    if (!empty($data) && $cookie_context) {
      $context->data = $cookie_context;
      $context->title = t("Cookie context");
      return $context;
    }
  }

  // If cookie don't exist in browser, send back fallback value.
  $context->data = $data['fallback_value'];
  $context->title = t("Cookie context");
  return $context;
}

/**
 * Form builder; settings for the context.
 */
function ctools_cookie_context_settings_form($form, &$form_state) {
  $form['cookie_name'] = array(
    '#title' => t('Cookie name'),
    '#description' => t('Enter the name of the cookie'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  if (isset($form_state['conf']['cookie_name'])) {
    $form['cookie_name']['#default_value'] = $form_state['conf']['cookie_name'];
  }

  $form['fallback_value'] = array(
    '#title' => t('Fallback value'),
    '#description' => t("Enter a value that must be returned if the above specified cookie don't exist ."),
    '#type' => 'textfield',
  );
  if (!empty($form_state['conf']['fallback_value'])) {
    $form['fallback_value']['#default_value'] = $form_state['conf']['fallback_value'];
  }
  return $form;
}

/**
 * Submit handler; settings form for the context.
 */
function ctools_cookie_context_settings_form_submit($form, &$form_state) {
  $form_state['conf']['cookie_name'] = $form_state['values']['cookie_name'];
  $form_state['conf']['fallback_value'] = $form_state['values']['fallback_value'];
}

/**
 * Provide a list of sub-keywords.
 *
 * This is used to provide keywords from the context for use in a content type,
 * pane, etc.
 */
function ctools_cookie_context_convert_list() {
  return array(
    'cookie_value' => t('Cookie value'),
  );
}

/**
 * Convert a context into a string to be used as a keyword by content types.
 */
function ctools_cookie_context_convert($context, $type) {
  switch ($type) {
    case 'cookie_value':
      return $context->data;
  }
}
