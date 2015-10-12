<?php
namespace devtools\components\jquery;

class Hooks extends \devtools\Hooks {
  /**
   * Extends hook_init().
   */
  public function hook_init() {
    devtools_add_js($this->_path . '/js/jquery.js');
  }
}