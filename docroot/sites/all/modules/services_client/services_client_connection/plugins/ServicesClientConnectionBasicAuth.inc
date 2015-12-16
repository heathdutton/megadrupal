<?php

/**
 * @file
 * Session authentication for 3.x version
 */

class ServicesClientConnectionBasicAuth extends ServicesClientConnectionAuth {

  /**
   * Implements configForm().
   */
  public function configForm(&$form, &$form_state) {
    $form['username'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#default_value' => isset($this->config['username']) ? $this->config['username'] : '',
    );

    $form['password'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#default_value' => isset($this->config['password']) ? $this->config['password'] : '',
    );
  }

  /**
   * Implements configFormSubmit().
   */
  public function configFormSubmit(&$form, &$form_state) {
    parent::configFormSubmit($form, $form_state);

    $form_state['config']['username'] = $form_state['values']['username'];
    $form_state['config']['password'] = $form_state['values']['password'];
  }

  /**
   * Implements prepareRequest().
   *
   * @param ServicesClientConnectionHttpRequest $request
   */
  public function prepareRequest(ServicesClientConnectionHttpRequest &$request) {
    parent::prepareRequest($request);

    $credentials = $this->config['username'] . ':' . $this->config['password'];
    $credentials = base64_encode($credentials);

    $request->http_headers['Authorization'] = 'Basic ' . $credentials;
  }
}
