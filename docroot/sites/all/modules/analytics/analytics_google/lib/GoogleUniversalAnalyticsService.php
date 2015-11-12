<?php

class GoogleUniversalAnalyticsService extends AnalyticsService {

  public function getDefaultOptions() {
    return array(
      'id' => NULL,
    );
  }

  public function getFormOptions(&$form, &$form_state) {
    $options['id'] = array(
      '#title' => t('Tracking ID'),
      '#type' => 'textfield',
      '#default_value' => $this->service->options['id'],
      '#element_validate' => array('_analytics_google_validate_id'),
      '#required' => TRUE,
      '#size' => 15,
    );

    return $options;
  }

  public function getOutput(array $context) {
    $actions = array();
    $actions[] = array('create', $this->service->options['id'], 'auto', array('name' => $this->service->machine_name));
    $actions[] = array($this->service->machine_name . '.send', 'pageview');
    if (variable_get('analytics_privacy_anonymize_ip', FALSE)) {
      $actions[] = array($this->service->machine_name . '.set', 'anonymizeIp', TRUE);
    }

    $actions = array_merge($actions, module_invoke_all('analytics_google_universal_actions', $this, $context));
    drupal_alter('analytics_google_universal_actions', $actions, $this, $context);

    $code = theme('analytics_google_universal_js', array(
      'actions' => $actions,
      'service' => $this,
    ));
    $output = array();
    $output['#attached']['js'][] = array(
      'data' => $code,
      'type' => 'inline',
      //'weight' => 100000,
      'preprocess' => FALSE,
      'scope' => 'footer',
    );

    return $output;
  }

  public function getCacheableUrls() {
    $urls = array();
    // if ($this->service->options['cache']) {
      $urls[] = 'https://www.google-analytics.com/analytics.js';
    // }
    return $urls;
  }
}
