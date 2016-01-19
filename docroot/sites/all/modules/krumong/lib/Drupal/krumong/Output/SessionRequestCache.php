<?php

namespace Drupal\krumong\Output;
use Drupal\krumong\Util;


/**
 * Can delay stuff from one request to the next.
 */
class SessionRequestCache {

  protected $storage;
  protected $cssJs;

  /**
   * @param &array $storage
   *   Reference to a nested array within $_SESSION
   */
  function __construct($session_key, $cssJs) {

    if (!isset($_SESSION[$session_key]) || !is_array($_SESSION[$session_key])) {
      $_SESSION[$session_key] = array();
    }

    $this->storage = &$_SESSION[$session_key];

    if (!isset($this->storage['has_messages'])) {
      $this->storage['has_messages'] = FALSE;
    }

    if (!isset($this->storage['js'])) {
      $this->storage['js'] = array();
    }

    $this->cssJs = $cssJs;
  }

  /**
   * Post a HTML message, and remember in session that we need css/js.
   */
  function message($html, $type = 'status') {
    drupal_set_message($html, $type);
    $this->storage['has_messages'] = TRUE;
  }

  /**
   * Add some inline js, e.g. calling console.log().
   */
  function jsInline($js) {
    $this->storage['js'][] = $js;
  }

  /**
   * Called from hook_process_status_messages().
   * Any messages or inline js sent after this event will be postponed to a
   * future request.
   */
  function hook_process_status_messages() {

    if ($this->storage['has_messages']) {
      $this->cssJs->addCssJsOnce();
      $this->storage['has_messages'] = FALSE;
    }

    if (!empty($this->storage['js'])) {
      $this->cssJs->addCssJsOnce();
      foreach ($this->storage['js'] as $js) {
        drupal_add_js($js, 'inline');
      }
      $this->storage['js'] = array();
    }
  }
}
