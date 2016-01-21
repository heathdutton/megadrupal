<?php
namespace devtools\components;

class Debug extends \devtools\patterns\Singleton {
  protected static $_instance;

  protected $_allowed_ips = array(
    '127.0.0.1',
    '0.0.0.0',
    '::1',
  );

  /**
   * Prepares and outputs the styled data.
   * @param  [type]  $data        [description]
   * @param  [type]  $return      [description]
   * @return [type]               [description]
   */
  public function output($data, $return) {
    // user must be allowed to see debug output
    if (!self::is_debug()) {
      return FALSE;
    }

    $output = self::style($data);

    if ($return) {
      return $output;
    }
    else {
      echo $output;
    }
  }

  /**
   * Prepares and outputs "permanent" debug output (non-user/ip 
   * restricted, but rather dependent on a GET key).
   *
   * Don't use this for any sensitive data!
   * 
   * @param  [type]  $data        [description]
   * @return [type]               [description]
   */
  public function permOutput($data) {
    $key = 'devtools_debug';

    // key may be changed in settings.php to make it site-specific
    global $conf;
    if (isset($conf['devtools_perm_debug_key']) && !empty($conf['devtools_perm_debug_key'])) {
      $key = $conf['devtools_perm_debug_key'];
    }

    // user must be allowed to see debug output
    if (!isset($_GET[$key])) {
      return FALSE;
    }

    $output = self::style($data);

    drupal_set_message(print_r($output, 1));
  }

  /**
   * Checks to see whether the current user is allowed to see debugging information;
   * @return - boolean
   */
  protected function is_debug() {
    global $user;
    
    return (in_array($_SERVER['REMOTE_ADDR'], $this->getIps()) || $user->uid == 1);
  }

  /**
   * Wraps the print_r output in styled <pre> tags.
   * @param  [type] $data [description]
   * @return [type]       [description]
   */
  protected function style($data) {
    $output = '<pre style="
      background: #eee;
      border: 1px #ccc solid;
      color: #111;
      font-size: 1.1em;
      margin: 10px;
      min-width: 1000px;
      padding: 10px;
      position: relative;
      text-align: left;
      text-transform: none;
      width: 100%;
      z-index: 999999;
    ">';
    $output .= print_r($data, 1);
    $output .= '</pre>';

    return $output;
  }

  /**
   * Returns list of IP addresses which are allowed to see debugging info.
   * @return [type] [description]
   */
  public function getIps() {
    return $this->_allowed_ips;
  }

  /**
   * Adds an IP address to the allowed IPs list.
   * @param [type] $ip [description]
   */
  public function addIp($ip) {
    $this->_allowed_ips[] = $ip;
    $this->_allowed_ips = array_unique($this->_allowed_ips);
  }

  /**
   * Removes IP address from allowed IPs list.
   * @param  [type] $ip [description]
   */
  public function removeIp($ip) {
    if ($index = array_search($ip, $this->_allowed_ips)) {
      unset($this->_allowed_ips[$index]);
    }
  }
}