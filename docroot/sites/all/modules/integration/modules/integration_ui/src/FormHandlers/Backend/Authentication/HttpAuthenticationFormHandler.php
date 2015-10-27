<?php

/**
 * @file
 * Contains HttpAuthenticationFormHandler.
 */

namespace Drupal\integration_ui\FormHandlers\Backend\Authentication;

use Drupal\integration_ui\AbstractForm;

/**
 * Class HttpAuthenticationFormHandler.
 *
 * @package Drupal\integration_ui\Backend\Authentication
 */
class HttpAuthenticationFormHandler extends AbstractForm {

  /**
   * {@inheritdoc}
   */
  public function form(array &$form, array &$form_state, $op) {
    $configuration = $this->getConfiguration($form_state);
    $form['username'] = array(
      '#title' => t('Username'),
      '#type' => 'textfield',
      '#default_value' => $configuration->getComponentSetting('authentication_handler', 'username'),
      '#required' => FALSE,
    );
    $form['password'] = array(
      '#title' => t('Password'),
      '#type' => 'textfield',
      '#default_value' => $configuration->getComponentSetting('authentication_handler', 'password'),
      '#required' => FALSE,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function formSubmit(array $form, array &$form_state) {

  }

  /**
   * {@inheritdoc}
   */
  public function formValidate(array $form, array &$form_state) {

  }

}
