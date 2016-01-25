<?php

/**
 * @file
 * Please supply a file description.
 */

/**
 * @file
 * Provides myGengo translation plugin controller.
 */
class TMGMTOhtTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * Overrides TMGMTDefaultTranslatorUIController::pluginSettingsForm().
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {
    $form['api_account_id'] = array(
      '#type' => 'textfield',
      '#title' => t('OHT API Account ID'),
      '#default_value' => $translator->getSetting('api_account_id'),
      '#description' => t('Please enter yout Account ID for OneHourTranslation service.'),
    );
    $form['api_secret_key'] = array(
      '#type' => 'textfield',
      '#title' => t('OHT API Secret key'),
      '#default_value' => $translator->getSetting('api_secret_key'),
      '#description' => t('Please enter your secret API key for OneHourTranslation service.'),
    );
    $form['use_sandbox'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the sandbox'),
      '#default_value' => $translator->getSetting('use_sandbox'),
      '#description' => t('Check to use the testing environment.'),
    );
    return parent::pluginSettingsForm($form, $form_state, $translator, $busy);
  }
}
