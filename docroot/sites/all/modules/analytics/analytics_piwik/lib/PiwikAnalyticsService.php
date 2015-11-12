<?php

class PiwikAnalyticsService extends AnalyticsService {

  public function getDefaultOptions() {
    return array(
      'url' => NULL,
      'id' => NULL,
    );
  }

  public function getFormOptions(&$form, &$form_state) {
    $options['url'] = array(
      '#type' => 'textfield',
      '#title' => t('URL'),
      '#description' => t('The URL to your Piwik base directory.'),
      '#default_value' => $this->service->options['url'],
      '#element_validate' => array('_analytics_piwik_validate_url'),
      '#required' => TRUE,
    );
    $options['id'] = array(
      '#type' => 'textfield',
      '#title' => t('Site ID'),
      '#default_value' => $this->service->options['id'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#required' => TRUE,
      '#size' => 10,
    );

    return $options;
  }

  public function getOutput(array $context) {
    $actions = array();
    $actions[] = array('setSiteId', (int) $this->service->options['id']);
    $actions[] = "_paq.push(['setTrackerUrl', u+'piwik.php']);";
    $actions[] = array('trackPageView');
    $actions[] = array('enableLinkTracking');

    $actions = array_merge($actions, module_invoke_all('analytics_piwik_actions', $this, $context));
    drupal_alter('analytics_piwik_actions', $actions, $this, $context);

    $code = theme('analytics_piwik_js', array(
      'actions' => $actions,
      'service' => $this,
      'url' => $this->service->options['url'],
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

  /*public function getCacheableUrls() {
    $urls = array();
    // Only allow this URl to be cached if it is served from a different domain.
    if (parse_url($this->service->options['url'], PHP_URL_HOST) != parse_url($GLOBALS['base_url'], PHP_URL_HOST)) {
      $urls[] = $this->service->options['url'] . 'piwik.js';
    }
    return $urls;
  }*/
}
