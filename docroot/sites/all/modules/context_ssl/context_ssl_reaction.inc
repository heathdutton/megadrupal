<?php

/**
 * @file
 *
 * Context reaction implementation.
 */

/**
 * Context options form and reaction.
 */
class context_ssl_reaction extends context_reaction {

  function options_form($context) {
    $values = $this->fetch_from_context($context);
    $form['#tree'] = TRUE;
    $form['ssl_enabled'] = array(
      '#title' => t('Force pages to be'),
      '#type' => 'radios',
      '#options' => array(
        TRUE => t('Secure (with SSL)'),
        FALSE => t('Insecure (no SSL)'),
      ),
      '#default_value' => !empty($values['ssl_enabled']) ? $values['ssl_enabled'] : FALSE,
    );
    return $form;
  }

  /**
   * See if context contains redirect reaction.
   */
  function execute() {
    $contexts = $this->get_contexts();

    foreach ($contexts as $context) {
      if (isset($context->reactions[$this->plugin]['ssl_enabled'])) {
        $this->ssl_redirect($context->reactions[$this->plugin]['ssl_enabled']);
      }
    }
  }

  /**
   * Check current URL and redirect user to appropriate secure/non secure page.
   *
   * @param $ssl_enabled
   *   Whether the intent is to redirect to a secure page or not.
   */
  function ssl_redirect($ssl_enabled = TRUE) {
    if ($ssl_enabled && !$this->is_secure()) {
      $this->ssl_goto(TRUE);
    }
    elseif (!$ssl_enabled && $this->is_secure()) {
      $this->ssl_goto(FALSE);
    }
  }

  /**
   * Do the redirect.
   *
   * @param $ssl
   *   Whether to redirect to a secure page or not.
   */
  function ssl_goto($ssl = TRUE) {
    $url = ($ssl ? 'https://' : 'http://') . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']; 
    header("Location: $url"); 
    exit;
  }

  /**
   * Check if current URL is secure or not.
   */
  function is_secure() {
    // Sometimes IIS can set HTTPS to 'off' when on a non secure page.
    return ((!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off') || $_SERVER['SERVER_PORT'] == 443);
  }

}
