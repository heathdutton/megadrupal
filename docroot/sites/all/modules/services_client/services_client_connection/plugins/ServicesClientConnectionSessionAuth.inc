<?php

/**
 * @file
 * Session authentication for 3.x version
 */

class ServicesClientConnectionSessionAuth extends ServicesClientConnectionAuth {

  /**
   * Session ID
   */
  protected $sessid = NULL;

  /**
   * Session name
   */
  protected $session_name = NULL;

  /**
   * Logged in remote user
   */
  protected $user = NULL;

  /**
   * CSRF token.
   */
  protected $csrf_token = NULL;

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

    $form['token'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use tokens'),
      '#description' => t("Services 3.5+ requires CSRF tokens in each request."),
      '#default_value' => isset($this->config['token']) ? $this->config['token'] : '',
    );
  }

  /**
   * Implements configFormSubmit().
   */
  public function configFormSubmit(&$form, &$form_state) {
    parent::configFormSubmit($form, $form_state);

    $form_state['config']['username'] = $form_state['values']['username'];
    $form_state['config']['password'] = $form_state['values']['password'];
    $form_state['config']['token'] = $form_state['values']['token'];
  }

  /**
   * Implements login().
   */
  public function login() {
    $response = $this->client->action('user', 'login', array(
      'username' => $this->config['username'],
      'password' => $this->config['password'],
    ));

    $this->sessid = $response['sessid'];
    $this->session_name = $response['session_name'];
    $this->user = $response['user'];

    // If configured to use CSRF token retrieve token first.
    if (!empty($this->config['token'])) {
      $response = $this->client->action('user', 'token');
      if (!empty($response['token'])) {
        $this->csrf_token = $response['token'];
      }
    }
  }

  /**
   * Implements logout().
   */
  public function logout() {
    if ($this->sessid) {
      $response = $this->client->action('user', 'logout');

      $this->sessid = NULL;
      $this->session_name = NULL;
      $this->user = NULL;
      $this->csrf_token = NULL;
    }
  }

  /**
   * Implements prepareRequest().
   *
   * @param ServicesClientConnectionHttpRequest $request
   */
  public function prepareRequest(ServicesClientConnectionHttpRequest &$request) {
    parent::prepareRequest($request);

    if ($this->sessid) {
      $request->cookie[] = $this->session_name . '=' . $this->sessid;
    }

    if ($this->csrf_token) {
      $request->http_headers['X-CSRF-Token'] = $this->csrf_token;
    }
  }
}
