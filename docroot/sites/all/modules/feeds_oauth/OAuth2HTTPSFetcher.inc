<?php

/**
 * @file
 * Definition of the import batch object created on the fetching stage by
 * OAuth2HTTPSFetcher.
 */

/**
 * Support OAuth 2.0 authentication.
 */
class OAuth2HTTPSFetcher extends OAuthHTTPFetcher {
  /**
   * Declare defaults.
   */
  public function configDefaults() {
    return array(
      'scope' => '',
    ) + parent::configDefaults();
  }

  /**
   * Add form options.
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['scope'] = array(
      '#type' => 'textarea',
      '#title' => t('Scope'),
      '#description' => t('Scope of the authorization request, one per line or comma-separated.'),
      '#default_value' => $this->config['scope'],
    );
    return $form;
  }
}
