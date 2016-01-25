<?php

/**
 * @file
 * Contains ContextioAccount.
 */

namespace Drupal\fluxcontextio\Plugin\Service;

use Drupal\fluxcontextio\ContextioClient;
use Drupal\fluxservice\Plugin\Entity\Account;
use ContextIO;

/**
 * Account plugin implementation for Contextio.
 */
class ContextioAccount extends Account implements ContextioAccountInterface {

  /**
   * Defines the plugin.
   */
  public static function getInfo() {
    return array(
      'name' => 'fluxcontextio',
      'label' => t('Contextio account'),
      'description' => t('Provides Contextio integration for fluxkraft.'),
      'service' => 'fluxcontextio',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(array &$form_state) {
    $settings = $this->getDefaultSettings();
    $form = parent::settingsForm($form_state);

    if (!empty($_REQUEST['contextio_token'])) {
      $result = $this->client()->getConnectToken(null,array(
        'token' => $_REQUEST['contextio_token']
      ));
      if ($result === false) {
        throw new Exception("Unable to find account.");
      }   
      $data = $result->getData();
      $accountid = $data['account']['id'];
      $form['account_id'] = array(
        '#type' => 'hidden',
        '#value' => $accountid,
      );

      $r = $this->client()->getAccount($accountid);
      $data = $r->getData();
      $this->data['email'] = $data['email_addresses'][0];
    }

    $form['email'] = array(
      '#type' => 'hidden',
      '#value' => $this->data['email'],
    );

/*
    // this form would allow everything to happen on site

    $form['id'] = array(
      '#type' => 'textfield',
      '#title' => 'Remote Id',
      '#default_value' => $this->remote_id,
      '#disabled' => TRUE,
    );
    $form['account'] = array(
      '#type' => 'fieldset',
      '#title' => 'Email Account',
    );
    $form['account']['email'] = array(
      '#type' => 'textfield',
      '#title' => 'Email',
      '#default_value' => $this->data['email'],
    );
    $form['account']['username'] = array(
      '#type' => 'textfield',
      '#title' => 'Username',
      '#default_value' => $this->data['username'],
    );
    $form['account']['password'] = array(
      '#type' => 'password',
      '#title' => 'Password',
    );
    // IMAP details
    $form['account']['imap'] = array(
      '#type' => 'fieldset',
      '#title' => 'Settings',
    );
    $form['account']['imap']['server'] = array(
      '#type' => 'textfield',
      '#title' => 'Server',
      '#default_value' => $this->data['server'],
    );
    $form['account']['imap']['use_ssl'] = array(
      '#type' => 'checkbox',
      '#title' => 'Use SSL',
      '#default_value' => $settings['use_ssl'],
      '#attributes' => array('checked' => 'checked'),
    );
    $form['account']['imap']['port'] = array(
      '#type' => 'textfield',
      '#title' => 'Port',
      '#default_value' => isset($this->data['port']) ? $this->data['port'] : $settings['port'],
    );
*/
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsFormValidate(array $form, array &$form_state) {
    $parents = isset($form['#parents']) ? $form['#parents'] : array();
    $values = (array) drupal_array_get_nested_value($form_state['values'], $parents);
    $contextIO = $this->client();
    $email = $form_state['values']['label'];
    $result = $contextIO->listAccounts(array(
      'email' => $email
    ));
    $data = $result->getData();
    if ($result === $false || empty($data)) {
      // Get a new connect token from Context.IO. For complete documentation,
      // see http://context.io/docs/2.0/connect_tokens#post
      $result = $contextIO->addConnectToken(null, array(
        "callback_url" => "http://".$_SERVER['HTTP_HOST'] .'/?q=' . $_GET['q'],
        "email" => strval($email),
        "service_level" => "BASIC"
      ));
      if ($result === false) {
        throw new Exception("Unable to get a connect token.");
      }
      else {
        $state = drupal_get_token(serialize($this));
        $redirect = $this->getRedirectUrl($this);

        $settings = $this->data;
        $settings->set('state', $state);
        $settings->set('redirect', $redirect);

        // Temporarily save the account entity so we can refer to it later.
        $store = fluxservice_tempstore("fluxservice.account.{$this->bundle()}");
        $store->setIfNotExists($this->identifier(), $this);

        // redirect user to the connect token UI
        $token = $result->getData();
        $_SESSION['ContextIO-connectToken'] = $token['token'];
        header("Location: ". $token['browser_redirect_url']);
        exit();
      }
    }
    else {
      $form_state['storage']['account'] = reset($result->getData());
    }
  } 
  
  /**
   * {@inheritdoc}
   */
  public function settingsFormSubmit(array $form, array &$form_state) {
    $parents = isset($form['#parents']) ? $form['#parents'] : array();
    $values = (array) drupal_array_get_nested_value($form_state['values'], $parents);

    $params = array(
      'email' => $values['email'],
      'username' => $values['account']['username'],
      'password' => $values['account']['password'],
      'server' => $values['account']['imap']['server'],
      'use_ssl' => $values['account']['imap']['use_ssl'],
      'port' => $values['account']['imap']['port'],
      'type' => 'IMAP',
    );
    $settings = array_intersect_key($params, $this->getDefaultSettings());

    // Write the submitted settings into the collection.
    $this->data->mergeArray($settings);

    $this->data->mergeArray($form_state['storage']['account']);
    $this->remote_id = $form_state['storage']['account']['id'];
  }

  /**
   * {@inheritdoc}
   */
/*
  public function prepareAccount() {
    parent::prepareAccount();
    $key = $this->getService()->getConsumerKey();
    $secret = $this->getService()->getConsumerSecret();

    unset($_SESSION['contextio_api']);


    // Temporarily save the account entity so we can refer to it later.
    $store = fluxservice_tempstore("fluxservice.account.{$this->bundle()}");
    $store->setIfNotExists($this->identifier(), $this);


    $redirect = $this->getRedirectUrl();

    // Redirects the user to the authentication site of contextio,
    // and returns the user upon success to the redirect url.
    new Curl($key, $secret, $storage, $redirect);
  }
*/
  /**
   * {@inheritdoc}
   */
  public static function getAccountForOAuthCallback($key, $plugin) {
    $store = fluxservice_tempstore("fluxservice.account.{$plugin}");
    return $store->getIfOwner($key);
  }

  /**
   * Builds the URL to redirect to after visiting contextio for authentication.
   *
   *
   * @return string
   *   The URL to redirect to after visiting the Contextio OAauth endpoint for
   *   requesting access privileges from a user.
   */
  protected function getRedirectUrl() {
    return url("fluxservice/oauth/{$this->bundle()}/{$this->identifier()}", array('absolute' => TRUE));
  }

  /**
   * {@inheritdoc}
   */
  public function client() {
    $key = $this->getService()->getConsumerKey();
    $secret = $this->getService()->getConsumerSecret();

    $contextIO = new ContextIO($key, $secret);
    return $contextIO;
  }

  /**
   * {@inheritdoc}
   */
  public function getDefaultSettings() {
    return array(
      'username' => '',
      'email' => '',
      'server' => '',
      'use_ssl' => 1,
      'port' => 993
    );
  }

  public function getContextIO() {
  }

}
