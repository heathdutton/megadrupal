<?php

/**
* Post form submission data to Act-On and convert visitors to known via cURL, allowing no direct touch
* between Act-On and your website vistitor's browser to be necessary
*/
class ActonConnection
{
  protected $_postItems = array();

  public function getPostItems()
  {
    return $this->_postItems;
  }

  /**
   * for setting your form's POST items (key is your form input's name, value is the input value)
   * @param string $key first part of key=value for form field submission (name in name=John)
   * @param string $value latter part of key=value for form field submission (John in name=John)
   */
  public function setPostItems($key, $value)
  {
    $this->_postItems[$key] = (string)$value;
  }

  protected function getDomain($address)
  {
    $pieces = parse_url($address);
    $domain = isset($pieces['host']) ? $pieces['host'] : '';
    if (preg_match('/(?P<domain>[a-z0-9][a-z0-9\-]{1,63}\.[a-z\.]{2,6})$/i', $domain, $regs)) {
      return $regs['domain'];
    }
    return false;
  }

  protected function getUserIP()
  {
    //check proxy
    if (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
      $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    } else {
      $ip = $_SERVER['REMOTE_ADDR'];
    }
    return $ip;
  }

  /**
   * process form data for submission to your Act-On external form URL
   * @param string $extPostUrl your external post (Proxy URL) for your Act-On "proxy" form
   */
  public function processConnection($extPostUrl)
  {
    $this->setPostItems('_ipaddr', $this->getUserIP()); // Act-On accepts manually defined IPs if using field name '_ipaddr'
    $fields = http_build_query($this->getPostItems()); // encode post items into query-string
    $handle = curl_init();
    curl_setopt($handle, CURLOPT_POST, 1);
    curl_setopt($handle, CURLOPT_URL, "$extPostUrl");
    curl_setopt($handle, CURLOPT_USERAGENT, $_SERVER['HTTP_USER_AGENT']);
    curl_setopt($handle, CURLOPT_HEADER, 1);
    curl_setopt($handle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_setopt($handle, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($handle, CURLOPT_POSTFIELDS, $fields);
    $response = curl_exec($handle);

    if ($response === FALSE) {
      $response = "cURL Error: " . curl_error($handle);
      if (user_access('administer acton')) {
        drupal_set_message($response, "error");
      }
    } else {
      preg_match('/^Set-Cookie:\s*([^;]*)/mi', $response, $ra); // pull response "set-cookie" values from cURL response header
      parse_str($ra[1], $cookie); // select the "set-cookie" for visitor conversion and store to array $cookie
      // set updated website visitor tracking cookie with processed "set- cookie" content from curl
      setrawcookie(key($cookie), implode(",", $cookie), time() + 86400 * 365, "/", $this->getDomain($extPostUrl)); // set cookie expiry date to 1 year
    }
    curl_close($handle);
  }

}