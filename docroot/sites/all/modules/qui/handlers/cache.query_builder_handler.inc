<?php
/**
 * @file
 * Class file for cache functionality.
 */

/**
 * Class Qui_Cache_Handler
 */
class Qui_Cache_Handler extends Qui_Query_Builder_Handler {

  public $name = 'cache';
  public $enabled = TRUE;
  private $cookie_name = 'qui_form_cache';
  private $form_build_id = FALSE;

  /**
   * Sets the cookie name and sets the cache.
   */
  public function __construct($controller) {
    parent::__construct($controller);
    $this->cache_message = t('* All changes are stored temporarily. Click Save to make your changes permanent. Click Cancel to discard your changes.');
    $this->form_build_id = $this->formValue('form_build_id');
    if ($this->reportValue('id')) {
      $this->cookie_name .= '_' . $this->reportValue('id');
    }
    $this->setCache();
  }

  /**
   * Set the cache cookie and message.
   */
  public function setCache() {
    if ($this->form_build_id) {
      $this->setCacheMessage();
      $this->saveCacheCookie();
    }
    return $this;
  }

  /**
   * Save the cache cookie with the form_build_id.
   */
  private function saveCacheCookie() {
    user_cookie_save(array($this->cookie_name => $this->form_build_id));
  }

  /**
   * Sets the cache message if it should be shown.
   */
  private function setCacheMessage() {
    if ($this->buttonAction() == 'generatePreview' || $this->buttonAction() == 'cancelSubmit') {
      return FALSE;
    }
    drupal_set_message($this->cache_message, 'warning');
  }

  /**
   * Get the cache cookie value.
   */
  private function getCacheCookie() {
    $name = 'Drupal_visitor_' . $this->cookie_name;
    if (isset($_COOKIE[$name])) {
      return $_COOKIE[$name];
    }
    return FALSE;
  }

  /**
   * Delete the cache cookie.
   */
  public function deleteCache() {
    user_cookie_delete($this->cookie_name);
  }

  /**
   * Gets the cached form state and sets it.
   */
  public function getCache() {
    if (!$this->form_build_id && $this->getCacheCookie()) {
      if ($cached = form_get_cache($this->getCacheCookie(), $this->controller->form_state)) {
        $this->controller->form = $cached;
        $this->setCacheMessage();
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * On submit, delete the cache cookie.
   */
  public function submit() {
    $this->deleteCache();
  }

}
