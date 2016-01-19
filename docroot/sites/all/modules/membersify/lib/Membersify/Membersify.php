<?php

/**
 * Class Membersify
 */
abstract class Membersify {
  public static $public_key;
  public static $secret_key;
  public static $api_url = "https://www.membersify.com/api1/";
  public static $api_version = "1.1";

  /**
   * Sets the Membersify API keys.
   *
   * @param string $public_key
   *   The public key from Membersify.com.
   * @param string $secret_key
   *   The secret key from Membersify.com.
   */
  public static function setKeys($public_key, $secret_key)
  {
    self::$public_key = $public_key;
    self::$secret_key = $secret_key;
  }

  /**
   * Gets the settings array for the site.
   *
   * @return mixed
   */
  public static function getSettings() {
    return Membersify_ApiResource::request("site/get_settings");
  }

  /**
   * Records the member levels with Membersify.
   *
   * @param array $levels
   *   An array of levels, keyed by id.
   *
   * @return mixed
   */
  public static function sendLevels($levels) {
    return Membersify_ApiResource::request("site/save_levels", array('levels' => $levels));
  }

  /**
   * Registers the webhook URL and site url with Membersify.
   *
   * @param string $url
   *   The site url.
   * @param string $webhook_url
   *   The webhook url.
   *
   * @return mixed
   */
  public static function saveWebhook($url, $webhook_url) {
    return Membersify_ApiResource::request("site/save_webhook", array('webhook_url' => $webhook_url, 'url' => $url));
  }
}
