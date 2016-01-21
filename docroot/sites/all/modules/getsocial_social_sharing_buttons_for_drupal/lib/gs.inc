<?php

/**
 * @file
 * GeSocial Functions.
 *
 * Extra funtions to make API calls and hadle requests.
 */

/**
 * Class gs (GetSocial).
 */
class GetsocialShareButtonsGs {
  private $gsUrl = "https://api.at.getsocial.io";
  public $gsAccount = "https://getsocial.io/";
  private $gsUrlApi = "//api.at.getsocial.io";
  public $apiUrl = "https://getsocial.io/api/v1/";

  /**
   * Class Constructor.
   */
  public function __construct($api_key, $identifier, $lang) {
    $this->api_key = $api_key;
    $this->identifier = $identifier;
    $this->lang = $lang == 'not_set' ? 'en' : $lang;
  }

  /**
   * Returns the complete API URL.
   */
  public function getsocialShareButtonsApiUrl($path) {
    return $this->apiUrl . $path;
  }

  /**
   * Returns de complete Site URL.
   */
  public function getsocialShareButtonsGetSite() {
    if ($this->api_key != '') :
      return $this->api('sites/' . $this->api_key);
    else :
      return NULL;
    endif;
  }

  /**
   * Return gsAccount.
   */
  public function getsocialShareButtonsGsAccount() {
    return $this->gsAccount;
  }

  /**
   * Add global variables to the Drupal JS.
   */
  public function getsocialShareButtonsSetJsBackendVariables() {
    drupal_add_js(array('getsocial_share_buttons' => array('getSocialApiUrl' => $this->apiUrl)), array('type' => 'setting'));
    drupal_add_js(array('getsocial_share_buttons' => array('getSocialApiKey' => $this->api_key)), array('type' => 'setting'));
  }
  /**
   * Inject the GetSocial library JS.
   */
  public function getsocialShareButtonsSetJsFrontendVariables() {
    drupal_add_js(array('getsocial_share_buttons' => array('getSocialUrlApi' => $this->gsUrlApi)), array('type' => 'setting'));
    drupal_add_js(array('getsocial_share_buttons' => array('getSocialIndentifier' => $this->identifier)), array('type' => 'setting'));
  }

}
