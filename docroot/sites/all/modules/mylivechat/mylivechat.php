<?php

/**
 * @file
 * MyLiveChat module for Drupal
 */
class MyLiveChat {

  /**
   * Singleton pattern
   */
  protected static $instance = NULL;

  /**
   * Module directory
   */
  protected $module_dir = NULL;

  /**
   * Singleton pattern
   */
  public static function get_instance() {
    if (is_null(self::$instance)) {
      self::$instance = new MyLiveChat();
    }

    return self::$instance;
  }

  /**
   * Constructor
   */
  protected function __construct() {
    $this->module_dir = drupal_get_path('module', 'mylivechat');
  }

  /**
   * Resets module settings
   */
  public function reset_settings() {
    variable_del('mylivechat_id');
    variable_del('mylivechat_displaytype');
    variable_del('mylivechat_membership');
    variable_del('mylivechat_encrymode');
    variable_del('mylivechat_encrykey');
  }

  /**
   * License number validation
   */
  public function validate_id($mylivechatid) {
    $license = (int) $mylivechatid;
    if ($license === 0)
      return FALSE;

    return preg_match('/^[0-9]{8}$/', $license);
  }

  /**
   * Checks if MyLiveChat settings are properly set up
   */
  public function is_installed() {
    $mylivechat_id = variable_get('mylivechat_id');
    $mylivechat_displaytype = variable_get('mylivechat_displaytype');
    $mylivechat_membership = variable_get('mylivechat_membership');
    $mylivechat_encrymode = variable_get('mylivechat_encrymode');
    $mylivechat_encrykey = variable_get('mylivechat_encrykey');

    $isIntegrateUser = FALSE;
    if ($mylivechat_membership == "yes") {
      $isIntegrateUser = TRUE;
    }

    if (is_null($mylivechat_id))
      return FALSE;

    if ($this->validate_id($mylivechat_id) == FALSE)
      return FALSE;

    return TRUE;
  }

  /**
   * Checks if MyLiveChat tracking code is installed properly
   */
  public function tracking_code_installed() {
    if ($this->is_installed() == FALSE)
      return FALSE;

    return TRUE;
  }

  /**
   * Checks if LiveChat button code is installed properly
   */
  public function chat_button_installed() {
    if ($this->is_installed() == FALSE) {
      return FALSE;
    }

    $result = db_query('SELECT status FROM {block} WHERE module=:module', array(':module' => 'mylivechat'));
    $row = $result->fetchObject();

    if (!$row || $row->status != '1')
      return FALSE;

    return TRUE;
  }

  public function install_codes() {
    $this->add_tracking_code();
  }

  /**
   * Install tracking code
   */
  public function add_tracking_code() {
    if ($this->is_installed() == FALSE) {
      return FALSE;
    }
  }

  /**
   * Returns MyLiveChat button HTML code
   */
  public function getChatCode() {
    if ($this->is_installed() == FALSE) {
      return FALSE;
    }

    $mylivechat_id = variable_get('mylivechat_id');
    $mylivechat_displaytype = variable_get('mylivechat_displaytype');
    $mylivechat_membership = variable_get('mylivechat_membership');
    $mylivechat_encrymode = variable_get('mylivechat_encrymode');
    $mylivechat_encrykey = variable_get('mylivechat_encrykey');

    $isIntegrateUser = FALSE;
    if ($mylivechat_membership == "yes") {
      $isIntegrateUser = TRUE;
    }

    $chat_button = "<div class=\"mod_mylivechat\">";

    if ($mylivechat_displaytype == "inline") {
      $chat_button .= "<script type=\"text/javascript\" src=\"https://www.mylivechat.com/chatinline.aspx?hccid=" . $mylivechat_id . "\"></script>";
    }
    elseif ($mylivechat_displaytype == "button") {
      $chat_button .= "<script type=\"text/javascript\" src=\"https://www.mylivechat.com/chatbutton.aspx?hccid=" . $mylivechat_id . "\"></script>";
    }
    elseif ($mylivechat_displaytype == "box") {
      $chat_button .= "<script type=\"text/javascript\" src=\"https://www.mylivechat.com/chatbox.aspx?hccid=" . $mylivechat_id . "\"></script>";
    }
    elseif ($mylivechat_displaytype == "widget") {
      $chat_button .= "<script type=\"text/javascript\" src=\"https://www.mylivechat.com/chatwidget.aspx?hccid=" . $mylivechat_id . "\"></script>";
    }
    else {
      $chat_button .= "<script type=\"text/javascript\" src=\"https://www.mylivechat.com/chatlink.aspx?hccid=" . $mylivechat_id . "\"></script>";
    }

    global $user;
    if (in_array('authenticated user', $user->roles) && $isIntegrateUser == TRUE) {
      if ($mylivechat_encrykey == NULL || strlen($mylivechat_encrykey) == 0) {
        $chat_button .= "<script type=\"text/javascript\">MyLiveChat_SetUserName('" . $this->EncodeJScript($user->name) . "');</script>";
      }
      else {
        $chat_button .= "<script type=\"text/javascript\">MyLiveChat_SetUserName('" . $this->EncodeJScript($user->name) . "','" . $this->GetEncrypt($user->uid . "", $mylivechat_encrymode, $mylivechat_encrykey) . "');</script>";
      }
    }

    $chat_button .= "</div>";
    // Return chat button code
    return $chat_button;
  }

  public function include_admin_css() {
    
  }

  public function include_admin_js() {
    drupal_add_js($this->module_dir . '/js/jquery-1.6.min.js');
    drupal_add_js($this->module_dir . '/js/mylivechat.js');
  }

  public function GetEncrypt($data, $encrymode, $encrykey) {
    if ($encrymode == "basic") {
      return $this->BasicEncrypt($data, $encrykey);
    }
    return $data;
  }

  public function BasicEncrypt($data, $encryptkey) {
    $EncryptLoopCount = 4;

    $vals = $this->MakeArray($data, TRUE);
    $keys = $this->MakeArray($encryptkey, FALSE);

    $len = sizeof($vals);
    $len2 = sizeof($keys);

    for ($t = 0; $t < $EncryptLoopCount; $t++) {
      for ($i = 0; $i < $len; $i++) {
        $v = $vals[$i];
        $im = ($v + $i) % 5;

        for ($x = 0; $x < $len; $x++) {
          if ($x == $i)
            continue;
          if ($x % 5 != $im)
            continue;

          for ($y = 0; $y < $len2; $y++) {
            $k = $keys[$y];
            if ($k == 0)
              continue;

            $vals[$x] += $v % $k;
          }
        }
      }
    }
    return implode('-', $vals);
  }

  public function MakeArray($str, $random) {
    $len = pow(2, floor(log(strlen($str), 2)) + 1) + 8;
    if ($len < 32)
      $len = 32;

    $arr = Array();
    $strarr = str_split($str);
    if ($random == TRUE) {
      for ($i = 0; $i < $len; $i++)
        $arr[] = ord($strarr[rand() % strlen($str)]);

      $start = 1 + rand() % ($len - strlen($str) - 2);

      for ($i = 0; $i < strlen($str); $i++)
        $arr[$start + $i] = ord($strarr[$i]);

      $arr[$start - 1] = 0;
      $arr[$start + strlen($str)] = 0;
    }
    else {
      for ($i = 0; $i < $len; $i++)
        $arr[] = ord($strarr[$i % strlen($str)]);
    }

    return $arr;
  }

  public function EncodeJScript($str) {
    $chars = "0123456789ABCDEF";
    $chars = str_split($chars);

    $sb = "";
    $l = strlen($str);
    $strarr = str_split($str);
    for ($i = 0; $i < $l; $i++) {
      $c = $strarr[$i];
      if ($c == '\\' || $c == '"' || $c == '\'' || $c == '>' || $c == '<' || $c == '&' || $c == '\r' || $c == '\n') {
        if ($sb == "") {
          if ($i > 0) {
            $sb .= substr($str, 0, $i);
          }
        }
        if ($c == '\\') {
          $sb .="\\x5C";
        }
        elseif ($c == '"') {
          $sb .="\\x22";
        }
        elseif ($c == '\'') {
          $sb .="\\x27";
        }
        elseif ($c == '\r') {
          $sb .="\\x0D";
        }
        elseif ($c == '\n') {
          $sb .="\\x0A";
        }
        elseif ($c == '<') {
          $sb .="\\x3C";
        }
        elseif ($c == '>') {
          $sb .="\\x3E";
        }
        elseif ($c == '&') {
          $sb .="\\x26";
        }
        else {
          $code = $c;
          $a1 = $code & 0xF;
          $a2 = ($code & 0xF0) / 0x10;
          $sb .="\\x";
          $sb .=$chars[$a2];
          $sb .=$chars[$a1];
        }
      }
      elseif ($sb != "") {
        $sb .= $c;
      }
    }
    if ($sb != "")
      return $sb;
    return $str;
  }
}
