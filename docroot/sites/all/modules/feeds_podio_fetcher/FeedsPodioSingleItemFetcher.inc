<?php

class FeedsPodioSingleItemFetcher extends FeedsFetcher {

  public function fetch(FeedsSource $source){
    if (($library = libraries_load('podio-php')) && !empty($library['loaded'])) {
      $source_config = $source->getConfigFor($this);

      Podio::setup($source_config['client_id'], $source_config['client_secret']);

      if(!Podio::is_authenticated()){
        // Obtain access token using authorization code from the first step of the authentication flow
        try {
          Podio::authenticate('app', array('app_id' => $source_config['app_id'], 'app_token' => $source_config['app_token']));
          // Authentication was a success, now you can start making API calls.
        }
        catch (PodioError $e) {
          watchdog('FeedsPodioFetcher', $e->body['error_description'], array(), WATCHDOG_CRITICAL, 'link');
        }
      }
      $response = PodioItem::get( $source_config['item_id'] );

      return new FeedsFetcherResult($response->as_json(true));
    }else{
      watchdog('FeedsPodioFetcher', 'Failed to load Podio PHP Client', array(), WATCHDOG_CRITICAL, 'link');
    }
  }

  /**
   * Expose source form.
   */
  public function sourceForm($source_config) {
    $form = array();
    $form['client_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Client ID'),
      '#description' => t('Enter Client ID.'),
      '#default_value' => isset($source_config['client_id']) ? $source_config['client_id'] : '',
      '#maxlength' => NULL,
      '#required' => TRUE,
    );
    $form['client_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Client secret'),
      '#description' => t('Enter Client secret.'),
      '#default_value' => isset($source_config['client_secret']) ? $source_config['client_secret'] : '',
      '#maxlength' => NULL,
      '#required' => TRUE,
    );
    $form['app_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Podio APP ID'),
      '#description' => t('Enter Podio App ID.'),
      '#default_value' => isset($source_config['app_id']) ? $source_config['app_id'] : '',
      '#maxlength' => NULL,
      '#required' => TRUE,
    );
    $form['app_token'] = array(
      '#type' => 'textfield',
      '#title' => t('Podio App token'),
      '#description' => t('Enter Podio App token.'),
      '#default_value' => isset($source_config['app_token']) ? $source_config['app_token'] : '',
      '#maxlength' => NULL,
      '#required' => TRUE,
    );
    $form['item_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Podio item ID'),
      '#description' => t('Enter Podio Item ID.'),
      '#default_value' => isset($source_config['item_id']) ? $source_config['item_id'] : '',
      '#maxlength' => NULL,
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values) {
    // podio is going to do this for us
  }
}
