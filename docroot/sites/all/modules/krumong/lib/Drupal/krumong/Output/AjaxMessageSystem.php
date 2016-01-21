<?php

namespace Drupal\krumong\Output;
use Drupal\krumong\Util;


/**
 * Under construction.
 */
class AjaxMessageSystem {

  protected $storage;
  protected $requestId;
  protected $requestStorage;
  protected $requestMessages;

  /**
   * @param &array $storage
   *   Reference to a nested array within $_SESSION
   */
  function __construct(&$storage) {

    // Weed out old empty ones.
    foreach ($storage as $id => $info) {
      if (empty($info['messages'])) {
        unset($storage[$id]);
      }
    }

    $this->storage =& $storage;

    // Create a new slot for the current request.
    $this->requestId = $id = Util::randomstring(30);
    $storage[$id] = array(
      'requestId' => $id,
      'q' => $_GET['q'],
      'microtime' => microtime(TRUE),
      'messages' => array(),
    );
    // $this->activeStorage =& $storage[$id];
    // $this->messages =& $storage[$id]['messages'];
  }

  function jsMessage() {
  }

  /**
   * Dump a variable and make it show up in Drupal's message area.
   *
   * @param mixed $data
   *   any variable to dump.
   */
  function dpm($data) {
    $this->dpmByRef($data);
  }

  function dpmByRef(&$data) {
    if (!user_access('access devel information')) {
      return;
    }
    $theme = new TreeTheme_JsDataPrefixed();
    $renderer = new TreeRenderer_BreadthFirst($theme);
    $js = $renderer->render($data, $this->calledFrom());

    $this->storage[$this->requestId]['messages'][] = $js;
  }

  /**
   * Gets triggered on drupal_process_status_messages(), short before they are
   * printed.
   */
  function setMessages() {
    if (!user_access('access devel information')) {
      return;
    }
    $html = '<div class="krumong-monitor" style="display:none;"></div>';
    drupal_set_message($html);
    drupal_add_css(drupal_get_path('module', 'krumong') . '/css/krumong.css');
    drupal_add_js(drupal_get_path('module', 'krumong') . '/js/krumong.js');
  }

  /**
   * Menu callback for krumong/ajax/%krumong_request
   */
  function ajax($id) {
    $data = @$this->storage[$id];
    if (empty($data)) {
      $js = '{}';
    }
    else {
      if (empty($data['messages'])) {
        $messages_js = '[]';
      }
      else {
        $messages_js = '[' . implode(',', $data['messages']) . ']';
      }
      unset($data['messages']);
      $data['messages'] = 999;
      $js = json_encode($data);
      if (substr($js, -4) !== '999}') {
        throw new Exception("json_encode() result should end with '999}'.");
      }
      $js = substr($js, 0, -4) . $messages_js . '}';
    }
    return $js;
  }
}
