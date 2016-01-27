<?php
/**
 * @file
 * @todo-3.1 Rewrite this to be an adaptor implementation maybe move adaptors
 * into sdk so we can avoid this horrible code convention?
 */

/**
 * Docblock
 */
class DrupalAdapter implements \janrain\Adapter {

  const SKU_SOCIAL_LOGIN = 0;
  const SKU_STANDARD = 1;

  protected $settings;

  /**
   * Drupal "Function" comment.
   */
  public function __construct($data = array()) {
    $this->settings = new \ArrayObject($data, \ArrayObject::ARRAY_AS_PROPS);
    if (!isset($_SESSION['janrain'])) {
      $_SESSION['janrain'] = array();
    }
  }

  /**
   * Drupal "Function" comment.
   */
  public function getItem($key) {
    return isset($this->settings->$key) ? $this->settings->$key : NULL;
  }
  /**
   * Drupal "Function" comment.
   */
  public function setItem($key, $value) {
    $this->settings->$key = $value;
  }
  /**
   * Drupal "Function" comment.
   */
  public function toJson() {
    return json_encode($this->settings->getArrayCopy());
  }
  /**
   * Drupal "Function" comment.
   */
  public function getIterator() {
    return $this->settings->getIterator();
  }

  /**
   * Drupal "Function" comment.
   */
  public function save() {
    variable_set('janrain_settings', $this->toJson());
  }

  /**
   * Drupal "Function" comment.
   */
  public static function fromDrupal() {
    // Load config from drupal if it exists.
    $out = self::fromJson(variable_get('janrain_settings', '{}'));
    return $out;
  }

  /**
   * Drupal "Function" comment.
   */
  public static function fromJson($json) {
    $data = json_decode($json);
    return new self($data);
  }

  /**
   * Drupal "Function" comment.
   */
  public static function getSessionItem($key) {
    return isset($_SESSION['janrain'][$key]) ? $_SESSION['janrain'][$key] : NULL;
  }
  /**
   * Drupal "Function" comment.
   */
  public static function setSessionItem($key, $value) {
    $_SESSION['janrain'][$key] = $value;
  }
  /**
   * Drupal "Function" comment.
   */
  public static function dropSessionItem($key = NULL) {
    if ($key) {
      unset($_SESSION['janrain'][$key]);
    }
    else {
      $_SESSION['janrain'] = array();
    }
  }

  /**
   * Drupal "Function" comment.
   */
  public function getLocale() {
    global $language;
    if (isset($language->language)) {
      return str_replace('_', '-', $language->language);
    }
    // Return sensible default if language->language missing.
    return 'en-US';
  }

  /**
   * Drupal "Function" comment.
   */
  public function setLoginPage() {
    global $base_root;
    $ruri = request_uri();
    // Don't set for requests against service endpoints.
    if (0 === stripos($ruri, '/services/') || 0 === stripos($ruri, '/janrain/registration/')) {
      // Widgets wont be rendered on service endpoints.
      return;
    }
    // Not a service endpoint, set the current page.
    self::setSessionItem('capture.currentUri', $base_root . $ruri);
  }
}
