<?php

/**
 * @file
 * Provides REST service translation UI controller.
 */

/**
 * File translator plugin controller.
 */
class TMGMTRestTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * {@inheritdoc}
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {
    $form['rest_url'] = array(
      '#type' => 'textfield',
      '#required' => TRUE,
      '#title' => t('REST URL'),
      '#default_value' => $translator->getSetting('rest_url'),
      '#description' => t('Please enter the URL of the REST endpoint to post the translations.'),
    );
    $form['app_secret'] = array(
      '#type' => 'textfield',
      '#required' => TRUE,
      '#title' => t('Secret key'),
      '#default_value' => $translator->getSetting('app_secret'),
      '#description' => t('Please enter the secret key for validation communication.'),
    );
    return parent::pluginSettingsForm($form, $form_state, $translator);
  }

  /**
   * {@inheritdoc}
   */
  public function checkoutSettingsForm($form, &$form_state, TMGMTJob $job) {
    return parent::checkoutSettingsForm($form, $form_state, $job);
  }

  /**
   * {@inheritdoc}
   */
  public function checkoutInfo(TMGMTJob $job) {
    // If the job is finished, it's not possible to import translations anymore.
    if ($job->isFinished()) {
      return parent::checkoutInfo($job);
    }
    $form = array();
    return $this->checkoutInfoWrapper($job, $form);
  }

}
