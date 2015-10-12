<?php
namespace Drupal\go1_base\Helper;

/**
 * Usage:
 * @code
 *   go1_id(new Drupal\go1_base\Helper\SubRequest('go1test_theming/users'))->request();
 * @code
 */
class SubRequest {
  private $path;
  private $original_path;

  /**
   * @param string $path
   */
  public function __construct($path = '') {
    $this->original_path = $_GET['q'];

    if (!empty($path)) {
      $this->setPath($path);
    }
  }

  public function __destruct() {
    $_GET['q'] = $this->original_path;
  }

  /**
   * @param string $path
   */
  public function setPath($path) {
    $this->path = $path;
    $_GET['q'] = $path;
  }

  /**
   * @param string $path
   *
   * @return mixed Return string or render array.
   */
  public function request($path = NULL) {
    if (!empty($path)) {
      $this->setPath($path);
    }

    return menu_execute_active_handler($this->path, $deliver = FALSE);
  }
}
