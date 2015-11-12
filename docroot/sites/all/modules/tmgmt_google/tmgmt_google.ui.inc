<?php

/**
 * @file
 * Provides Google Translator ui controller.
 */

/**
 * Google translator ui controller.
 */
class TMGMTGoogleTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * Overrides TMGMTDefaultTranslatorUIController::pluginSettingsForm().
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {

    $form['api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Google API key'),
      '#default_value' => $translator->getSetting('api_key'),
      '#description' => t('Please enter your Google API key or visit <a href="@url">Google APIs console</a> to create new one.',
        array('@url' => 'https://code.google.com/apis/console')),
    );

    return parent::pluginSettingsForm($form, $form_state, $translator);
  }

}
