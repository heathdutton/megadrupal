<?php

/**
 * @file
 * Provides the Nativy Translator ui controller.
 */

/**
 * Nativity translator ui controller.
 */
class TMGMTNativyTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * Overrides TMGMTDefaultTranslatorUIController::pluginSettingsForm().
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {
    $form['api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('API key'),
      '#default_value' => $translator->getSetting('api_key'),
      '#description' => t('Please enter your Nativy connect API key.'),
    );
    $form['private_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Private key'),
      '#default_value' => $translator->getSetting('private_key'),
      '#description' => t('Please enter your Nativy Private key.'),
    );
    $form['use_test'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the test environment'),
      '#default_value' => $translator->getSetting('use_test'),
      '#description' => t('Check to use the testing environment.'),
    );
    return parent::pluginSettingsForm($form, $form_state, $translator, $busy);
  }

  /**
   * Get a setting from a job.
   *
   * @param $job The job the setting is retreived from.
   * @param $setting The setting to be retreived.
   * @param $default A default value to be return if the setting is not present.
   * @return  The setting value in the job or the default value.
   */
  protected function getJobSetting(TMGMTJob $job, $setting, $default = '') {
    return isset($job->settings[$setting]) ? $job->settings[$setting] : $default;
  }

  /**
   * Defines plugin job settings form.
   *
   * Default values are taken from https://www.nativy.com/connectdoc/apimethods/postorder.aspx
   *
   * @param $form array
   * @param $form_state array
   * @return Settings form.
   */
  public function checkoutSettingsForm($form, &$form_state, TMGMTJob $job) {

    $settings['requested'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );

    $settings['short_description'] = array(
      '#type' => 'textfield',
      '#title' => t('Short description'),
      '#default_value' => $this->getJobSetting($job, 'short_description'),
    );

    $settings['long_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Long description'),
      '#default_value' => $this->getJobSetting($job, 'long_description'),
    );

    $settings['correction_required'] = array(
      '#type' => 'checkbox',
      '#title' => t('Require a corrector'),
      '#description' => t('If checked, a corrector will be assigned to the order. A corrector proofreads the translation of the translator and makes suggestions to him if there is something incorrect with the translation.'),
      '#default_value' => $this->getJobSetting($job, 'correction_required', FALSE),
    );

    $settings['weighting'] = array(
      '#type' => 'fieldset',
      '#title' => t('Priorities for order ranking'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $default_weightings = $this->getJobSetting($job, 'weighting', array());

    $settings['weighting']['weighting_price'] = array(
      '#type' => 'select',
      '#title' => t('Low price'),
      '#options' => drupal_map_assoc(array(20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80)),
      '#default_value' => isset($default_weightings['weighting_price']) ? $default_weightings['weighting_price'] : 40,
      '#description' => t('Indicates importance of the price.'),
    );

    $settings['weighting']['weighting_duration'] = array(
      '#type' => 'select',
      '#title' => t('Quick delivery'),
      '#options' => drupal_map_assoc(array(20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80)),
      '#default_value' => isset($default_weightings['weighting_duration']) ? $default_weightings['weighting_duration'] : 60,
      '#description' => t('Indicates importance of the delivery time.'),
    );

    $settings['weighting']['weighting_textmatching'] = array(
      '#type' => 'select',
      '#title' => t('Strong text matching'),
      '#options' => drupal_map_assoc(array(20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80)),
      '#default_value' => isset($default_weightings['weighting_textmatching']) ? $default_weightings['weighting_textmatching'] : 75,
      '#description' => t('Indicates importance of the text matching to previous translations.'),
    );

    $settings['weighting']['weighting_cooperation'] = array(
      '#type' => 'select',
      '#title' => t('Frequent cooperation'),
      '#options' => drupal_map_assoc(array(20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80)),
      '#default_value' => isset($default_weightings['weighting_cooperation']) ? $default_weightings['weighting_cooperation'] : 25,
      '#description' => t('Indicates importance of the usage of a previous translation cooperation.'),
    );

    $settings['request_order'] = array(
      '#type' => 'submit',
      '#value' => t('Request offer'),
      '#validate' => array('tmgmt_job_form_validate'),
      '#submit' => array('tmgmt_nativy_job_settings_submit'),
    );

    if ($this->getJobSetting($job, 'requested', FALSE)) {
      $response = $job->getTranslatorController()->getOrder($job);

      if (empty($response)) {
        drupal_set_message(t('Failed to retrieve an offer from Nativy.'), 'error');
        return $settings;
      }

      $options = array();
      foreach ($response as $offer) {
        $options[$offer->rank] = t('@name (@price EUR)', array('@name' =>  $offer->translator_firstname, '@price' => $offer->price_sum_gross));
      }

      $settings['rank'] = array(
        '#type' => 'radios',
        '#title' => t('Rank'),
        '#options' => $options,
        '#required' => TRUE,
      );

      foreach ($response as $offer) {
        $details = array(
          '#type' => 'fieldset',
          '#title' => $offer->translator_firstname,
          '#states' => array(
            'visible' => array(
              'input[name="settings[rank]"]' => array('value' => $offer->rank),
            ),
          ),
        );

        $details['orderId'] = array(
          '#type' => 'value',
          '#value' => $offer->orderid,
        );

        $details['image'] = array(
          '#theme' => 'image',
          '#path' => $offer->translator_picture_link,
        );

        if (!empty($offer->corrector_firstname)) {
          $details['translator_price'] = array(
            '#type' => 'item',
            '#title' => t('Translator Price'),
            '#markup' => t('@price EUR', array('@price' => $offer->price_translator_net)),
          );

          $details['corrector_name'] = array(
            '#type' => 'item',
            '#title' => t('Corrector name'),
            '#markup' => check_plain($offer->corrector_firstname),
          );
          $details['corrector_image'] = array(
            '#theme' => 'image',
            '#path' => $offer->corrector_picture_link,
          );

          $details['corrector_price'] = array(
            '#type' => 'item',
            '#title' => t('Corrector price'),
            '#markup' => t('@price EUR', array('@price' => $offer->price_corrector_net)),
          );
        }

        $details['price'] = array(
          '#type' => 'item',
          '#title' => t('Price'),
          '#markup' => t('@price EUR (including @tax EUR taxes)', array('@price' => $offer->price_sum_gross, '@tax' => $offer->price_sum_tax)),
        );

        $details['payment_link'] = array(
          '#type' => 'value',
          '#value' => $offer->payment_link,
        );

        $settings['rank_details'][$offer->rank] = $details;
      }
    }
    return $settings;
  }

  /**
   * Overrides TMGMTDefaultTranslatorUIController::pluginSettingsForm().
   */
  public function checkoutInfo(TMGMTJob $job) {
    $label = $job->getTranslator()->label();
    $output['#title'] = t('@translator translation job information', array('@translator' => $label));
    $output['#type'] = 'fieldset';

    if ($order = $job->getTranslatorController()->nativyRetrieveOrder($job)) {
      $status = t('Unknown status');
      switch ($order->status) {
        case 'orders_status_open':
          $status = t('Open');
          break;
        case 'orders_status_confirmationpending':
          $status = t('Confirmation pending');
          break;
        case 'orders_status_inprogress':
          $status = t('In progress');
          break;
        case 'orders_status_translationcomplete':
          $status = t('Complete');
          break;
        case 'orders_status_canceled':
          $status = t('Canceled');
          break;
      }

      $output['status'] = array(
        '#type' => 'item',
        '#title' => t('Status'),
        '#markup' => $status,
      );

      if ($job->isActive() && $order->status == 'orders_status_translationcomplete') {
        $output['fetch_translation'] = array(
          '#type' => 'submit',
          '#value' => t('Fetch translation'),
          '#submit' => array('tmgmt_nativy_job_fetch_translation_submit'),
        );
      }
    }
    else {
      $output['status'] = array(
        '#type' => 'item',
        '#title' => t('Status'),
        '#markup' => t('Unable to retrieve order from Nativy.'),
      );
    }

    return $output;
  }

}

