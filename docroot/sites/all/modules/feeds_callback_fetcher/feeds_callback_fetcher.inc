<?php

/**
 * @file
 *
 */


class feeds_callback_fetcher extends FeedsFileFetcher {

  /**
   * Fetch content from a source and return it.
   *
   * Every class that extends FeedsFetcher must implement this method.
   *
   * @param $source
   *   Source value as entered by user through sourceForm().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);

    if (!function_exists($source_config['callback'])) {
      throw new Exception(t('Callback @callback doesn\'t exists.', array('@callback' => $source_config['callback'])));
    }

    $file = drupal_tempnam(file_directory_temp(), 'feeds-');
    file_put_contents($file, call_user_func($source_config['callback']));

    if (filesize($file) == 0) {
      throw new Exception(t('Callback returned nothing, temporary filesize is null.'));
    }

    return new FeedsFileFetcherResult($file);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'callback' => '',
    );
  }

  /**
   * Override parent::sourceForm().
   */
  public function sourceForm($form_state) {
    $form = array();
    $form['callback'] = array(
      '#type' => 'textfield',
      '#title' => t('Callback'),
      '#description' => t('Specify a callback'),
      '#default_value' => $this->config['callback'],
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function sourceFormValidate(&$values) {
    $values['callback'] = trim($values['callback']);
    if (!function_exists($values['callback'])) {
      form_set_error('feeds][feeds_callback_fetcher][callback', t('Callback must exists.'));
    }
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['callback'] = array(
      '#type' => 'textfield',
      '#title' => t('Callback'),
      '#description' => t('Specify a callback'),
      '#default_value' => $this->config['callback'],
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    $values['callback'] = trim($values['callback']);
    if (!function_exists($values['callback'])) {
      form_set_error('feeds][feeds_callback_fetcher][callback', t('Callback must exists.'));
    }
  }

}
