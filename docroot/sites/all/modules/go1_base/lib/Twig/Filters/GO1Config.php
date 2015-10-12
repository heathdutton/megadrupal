<?php
namespace Drupal\go1_base\Twig\Filters;

/**
 * Callback for go1_config filter.
 */
class GO1Config {
  private $module;
  private $id;
  private $key;

  public function __construct($string) {
    list($this->module, $this->id, $this->key) = explode(':', $string, 3);
  }

  public function render() {
    return go1_config($this->module, $this->id)->get($this->key);
  }
}
