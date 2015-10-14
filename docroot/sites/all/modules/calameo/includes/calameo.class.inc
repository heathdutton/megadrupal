<?php
/**
 * @file
 *
 * Defines the Calameo API request class to encapsulate the public methods.
 * http://help.calameo.com/index.php?title=API:Introduction
 */

class Calameo {
  var $config = NULL;

  public function __construct($config = NULL) {
    $this->config = $config;
  }

  /**
   * Main request method, returns PHP array from Calameo
   */
  protected function _request($args = array()) {
    // Make sure we have the required data, error out with something resembling the API error response if not
    if (empty($this->config) || empty($this->config->endpoint) || empty($this->config->api_key) || empty($this->config->api_secret)) {
      return $this->_make_error_response(0, 'Missing account API request configuration');
    }

    // Fill in defaults if not already provided
    $def_args = array(
    );
    $args += $def_args;

    // Force specific arguments
    $args['apikey'] = $this->config->api_key;
    $args['output'] = 'PHP';
    $args['expires'] = time() + 3600; // Set expiration to 1 hour from now

    // Create a new cURL resource
    $retval = $this->_make_error_response(0, 'Unidentified error occurred');
    $ch = curl_init();
    if ($ch) {
      // Pull and set custom curl options if available
      $curlopts = variable_get('calameo_curl_options', array());
      drupal_alter('calameo_curl_options', $curlopts);
      curl_setopt_array($ch, $curlopts);

      // Set required curl options and execute
      $req_curlopts = array(
        CURLOPT_HEADER => TRUE,
        CURLOPT_RETURNTRANSFER => TRUE,
      );
      curl_setopt_array($ch, $req_curlopts);
      $endpoint = $this->config->endpoint;

      // Loop over redirects
      do {
        // Construct URL
        $req_url = $this->_build_url($endpoint, $args);
        _calameo_dbg($req_url);
        curl_setopt($ch, CURLOPT_URL, $req_url);
        $resp = curl_exec($ch);
        if (FALSE !== $resp) {
          // Pull info from response and see if we had an error
          $info = curl_getinfo($ch);
          if ($info['http_code'] >= 400) {
            _calameo_log('Server returned error code @errno', array('@errno' => $info['http_code']), WATCHDOG_ERROR, l('Calameo Request', $req_url, array('absolute' => TRUE)));
            $msg = t('Server returned error code @errno', array('@errno' => $info['http_code']));
            $retval = $this->_make_error_response(0, $msg);
            break;
          }
          else {
            // Split out response headers into name => value array
            list($headers, $data) = explode("\r\n\r\n", $resp, 2);
            $headers = $this->_parse_headers($headers);
            _calameo_dbg($headers);
            _calameo_dbg($data);

            // Test for redirect in the event curl wasn't able to automagically follow due to server config
            if (!empty($headers['location'])) {
              $endpoint = $headers['location'];
            }
            else {
              $retval = unserialize($data);
              if (!$retval) {
                $retval = $this->_make_error_response(0, 'Unable to unserialize API response');
              }
              else {
                $retval = $retval['response'];
              }
            }
          }
        }
        else {
          _calameo_log('curl_exec failed: @error [@errno]', array('@error' => curl_error($ch), '@errno' => curl_errno($ch)), WATCHDOG_ERROR, l('Calameo Request', $req_url, array('absolute' => TRUE)));
        }
      } while (FALSE !== $resp && !empty($headers['location']));

      // Close and cleanup
      curl_close($ch);
    }
    else {
      $retval = $this->_make_error_response(0, 'Unable to create curl resource');
    }

    // Return PHP array
    return $retval;
  }

  /**
   * Util method to generate API request signature per:
   * http://help.calameo.com/index.php?title=API:How_To_Sign_Your_Requests
   */
  protected function _build_url($endpoint, $args) {
    $sig = $this->config->api_secret;
    ksort($args);
    foreach ($args as $name => $val) {
      $sig .= $name . $val;
    }

    $args['signature'] = md5($sig);
    return url($endpoint, array('absolute' => TRUE, 'query' => $args));
  }

  /**
   * Util function to parse out HTTP response headers
   */
  function _parse_headers($headers) {
    $headers_arr = array();

    $headers = explode("\r\n", $headers);
    foreach ($headers as $header) {
      if ('HTTP' == substr($header, 0, 4)) {
        continue;
      }
      list($name, $value) = explode(':', $header, 2);
      $name = strtolower(trim($name));
      if (in_array($name, array('x-soda2-fields', 'x-soda2-types'))) {
        $headers_arr[$name] = json_decode($value);
      }
      else {
        $headers_arr[$name] = trim($value);
      }
    }

    return $headers_arr;
  }

  /**
   * Util method to generate an error response mockup
   */
  protected function _make_error_response($code, $msg) {
    return array(
      'requestid' => '',
      'requests' => 0,
      'status' => 'error',
      'error' => array(
        'code' => $code,
        'message' => $msg,
      ),
    );
  }

  /**
   * Util method to loop until all items have been retrieved
   */
  protected function _fetch_items($args, $order = NULL, $way = NULL, $start = NULL, $step = NULL) {
    // Suss out optional args
    $args['way'] = !empty($way) ? $way : 'UP';
    $args['start'] = !empty($start) ? $start : 0;
    if (!empty($order)) {
      $args['order'] = $order;
    }
    if (!empty($step)) {
      $args['step'] = $step;
    }

    // If step is defined, then we execute a normal call
    if (isset($args['step'])) {
      $retval = $this->_request($args);
    }
    // Otherwise, pull everything from start to end
    else {
      $retval = NULL;
      $start = 0;
      $step = 50;
      do {
        $args['start'] = $start;
        $args['step'] = $step;
        $resp = $this->_request($args);
        if (!empty($resp['content']['items'])) {
          if (!$retval) {
            $retval = $resp;
          }
          else {
            $retval['content']['items'] = array_merge($retval['content']['items'], $resp['content']['items']);
          }
        }
        $start += $step;
      } while (!empty($resp['content']['items']));

      if (!empty($retval['content']['items'])) {
        $retval['content']['start'] = 0;
        $retval['content']['step'] = $retval['content']['total'] = count($retval['content']['items']);
      }
    }

    return $retval;
  }

  /*///////////////////////////////////////////////////////////////////
  // ACCOUNTS
  ///////////////////////////////////////////////////////////////////*/

  /**
   * API.getAccountInfos
   * http://help.calameo.com/index.php?title=API:API.getAccountInfos
   */
  public function getAccountInfos() {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
    );
    return $this->_request($args);
  }

  /**
   * API.fetchAccountSubscriptions
   * http://help.calameo.com/index.php?title=API:API.fetchAccountSubscriptions
   */
  public function fetchAccountSubscriptions($order = NULL, $way = NULL, $start = NULL, $step = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
    );
    return $this->_fetch_items($args, $order, $way, $start, $step);
  }

  /**
   * API.fetchAccountBooks
   * http://help.calameo.com/index.php?title=API:API.fetchAccountBooks
   */
  public function fetchAccountBooks($order = NULL, $way = NULL, $start = NULL, $step = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
    );
    return $this->_fetch_items($args, $order, $way, $start, $step);
  }

  /*///////////////////////////////////////////////////////////////////
  // SUBSCRIPTIONS
  ///////////////////////////////////////////////////////////////////*/

  /**
   * API.fetchAccountSubscribers
   * http://help.calameo.com/index.php?title=API:API.fetchAccountSubscribers
   */
  public function fetchAccountSubscribers($order = NULL, $way = NULL, $start = NULL, $step = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
    );
    return $this->_fetch_items($args, $order, $way, $start, $step);
  }

  /**
   * API.getSubscriptionInfos
   * http://help.calameo.com/index.php?title=API:API.getSubscriptionInfos
   */
  public function getSubscriptionInfos($subscription_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
    );
    return $this->_request($args);
  }

  /**
   * API.fetchSubscriptionBooks
   * http://help.calameo.com/index.php?title=API:API.fetchSubscriptionBooks
   */
  public function fetchSubscriptionBooks($subscription_id, $order = NULL, $way = NULL, $start = NULL, $step = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
    );
    return $this->_fetch_items($args, $order, $way, $start, $step);
  }

  /**
   * API.fetchSubscriptionSubscribers
   * http://help.calameo.com/index.php?title=API:API.fetchSubscriptionSubscribers
   */
  public function fetchSubscriptionSubscribers($subscription_id, $order = NULL, $way = NULL, $start = NULL, $step = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
    );
    return $this->_fetch_items($args, $order, $way, $start, $step);
  }

  /*///////////////////////////////////////////////////////////////////
  // PUBLICATIONS
  ///////////////////////////////////////////////////////////////////*/

  /**
   * API.getBookInfos
   * http://help.calameo.com/index.php?title=API:API.getBookInfos
   */
  public function getBookInfos($book_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'book_id' => $book_id,
    );
    return $this->_request($args);
  }

  /**
   * API.activateBook
   * http://help.calameo.com/index.php?title=API:API.activateBook
   */
  public function activateBook($book_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'book_id' => $book_id,
    );
    return $this->_request($args);
  }

  /**
   * API.deactivateBook
   * http://help.calameo.com/index.php?title=API:API.deactivateBook
   */
  public function deactivateBook($book_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'book_id' => $book_id,
    );
    return $this->_request($args);
  }

  /**
   * API.updateBook
   * http://help.calameo.com/index.php?title=API:API.updateBook
   */
  public function updateBook($book_id, $properties = array()) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'book_id' => $book_id,
    );
    $args += $properties;
    return $this->_request($args);
  }

  /**
   * API.deleteBook
   * http://help.calameo.com/index.php?title=API:API.deleteBook
   */
  public function deleteBook($book_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'book_id' => $book_id,
    );
    return $this->_request($args);
  }

  /**
   * API.fetchBookTocs
   * http://help.calameo.com/index.php?title=API:API.fetchBookTocs
   */
  public function fetchBookTocs($book_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'book_id' => $book_id,
    );
    return $this->_request($args);
  }

  /**
   * API.fetchBookComments
   * http://help.calameo.com/index.php?title=API:API.fetchBookComments
   */
  public function fetchBookComments($book_id, $order = NULL, $way = NULL, $start = NULL, $step = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'book_id' => $book_id,
    );
    return $this->_fetch_items($args, $order, $way, $start, $step);
  }

  /**
   * API.renewBookPrivateUrl
   * http://help.calameo.com/index.php?title=API:API.renewBookPrivateUrl
   */
  public function renewBookPrivateUrl($book_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'book_id' => $book_id,
    );
    return $this->_request($args);
  }

  /*///////////////////////////////////////////////////////////////////
  // PUBLISHING
  ///////////////////////////////////////////////////////////////////*/

  // TODO - Most of these requests must be multipart/form-data encoded and the endpoint must be http://upload.calameo.com/1.0

  /*///////////////////////////////////////////////////////////////////
  // SUBSCRIBERS
  ///////////////////////////////////////////////////////////////////*/

  /**
   * API.getSubscriberInfos
   * http://help.calameo.com/index.php?title=API:API.getSubscriberInfos
   */
  public function getSubscriberInfos($subscription_id, $login) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_request($args);
  }

  /**
   * API.activateSubscriber
   * http://help.calameo.com/index.php?title=API:API.activateSubscriber
   */
  public function activateSubscriber($subscription_id, $login) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_request($args);
  }

  /**
   * API.deactivateSubscriber
   * http://help.calameo.com/index.php?title=API:API.deactivateSubscriber
   */
  public function deactivateSubscriber($subscription_id, $login) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_request($args);
  }

  /**
   * API.addSubscriber
   * http://help.calameo.com/index.php?title=API:API.addSubscriber
   */
  public function addSubscriber($subscription_id, $login, $properties = array()) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    $args += $properties;
    return $this->_request($args);
  }

  /**
   * API.updateSubscriber
   * http://help.calameo.com/index.php?title=API:API.updateSubscriber
   */
  public function updateSubscriber($subscription_id, $login, $properties = array()) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    $args += $properties;
    return $this->_request($args);
  }

  /**
   * API.deleteSubscriber
   * http://help.calameo.com/index.php?title=API:API.deleteSubscriber
   */
  public function deleteSubscriber($subscription_id, $login) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_request($args);
  }

  /**
   * API.fetchSubscriberBooks
   * http://help.calameo.com/index.php?title=API:API.fetchSubscriberBooks
   */
  public function fetchSubscriberBooks($subscription_id, $login, $order = NULL, $way = NULL, $start = NULL, $step = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_fetch_items($args, $order, $way, $start, $step);
  }

  /**
   * API.authSubscriberSession
   * http://help.calameo.com/index.php?title=API:API.authSubscriberSession
   */
  public function authSubscriberSession($subscription_id, $login) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_request($args);
  }

  /**
   * API.checkSubscriberSession
   * http://help.calameo.com/index.php?title=API:API.checkSubscriberSession
   */
  public function checkSubscriberSession($session_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'session_id' => $session_id,
    );
    return $this->_request($args);
  }

  /**
   * API.deleteSubscriberSession
   * http://help.calameo.com/index.php?title=API:API.deleteSubscriberSession
   */
  public function deleteSubscriberSession($session_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'session_id' => $session_id,
    );
    return $this->_request($args);
  }

  /*///////////////////////////////////////////////////////////////////
  // SUBSCRIBER DRMS
  ///////////////////////////////////////////////////////////////////*/

  /**
   * API.fetchSubscriberDRMSingles
   * http://help.calameo.com/index.php?title=API:API.fetchSubscriberDRMSingles
   */
  public function fetchSubscriberDRMSingles($subscription_id, $login) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_request($args);
  }

  /**
   * API.fetchSubscriberDRMPeriods
   * http://help.calameo.com/index.php?title=API:API.fetchSubscriberDRMPeriods
   */
  public function fetchSubscriberDRMPeriods($subscription_id, $login) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_request($args);
  }

  /**
   * API.fetchSubscriberDRMSeries
   * http://help.calameo.com/index.php?title=API:API.fetchSubscriberDRMSeries
   */
  public function fetchSubscriberDRMSeries($subscription_id, $login) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_request($args);
  }

  /**
   * API.addSubscriberDRMSingle
   * http://help.calameo.com/index.php?title=API:API.addSubscriberDRMSingle
   */
  public function addSubscriberDRMSingle($subscription_id, $login, $book_id, $extras = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
      'book_id' => $book_id,
    );
    if (!empty($extras)) {
      $args['extras'] = $extras;
    }
    return $this->_request($args);
  }

  /**
   * API.addSubscriberDRMPeriod
   * http://help.calameo.com/index.php?title=API:API.addSubscriberDRMPeriod
   */
  public function addSubscriberDRMPeriod($subscription_id, $login, $from, $to, $extras = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
      'from' => $from,
      'to' => $to,
    );
    if (!empty($extras)) {
      $args['extras'] = $extras;
    }
    return $this->_request($args);
  }

  /**
   * API.addSubscriberDRMSerie
   * http://help.calameo.com/index.php?title=API:API.addSubscriberDRMSerie
   */
  public function addSubscriberDRMSerie($subscription_id, $login, $from, $books, $extras = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
      'from' => $from,
      'books' => $books,
    );
    if (!empty($extras)) {
      $args['extras'] = $extras;
    }
    return $this->_request($args);
  }

  /**
   * API.updateSubscriberDRMPeriod
   * http://help.calameo.com/index.php?title=API:API.updateSubscriberDRMPeriod
   */
  public function updateSubscriberDRMPeriod($subscription_id, $login, $period_id, $from, $to, $extras = NULL) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
      'period_id' => $period_id,
      'from' => $from,
      'to' => $to,
    );
    if (!empty($extras)) {
      $args['extras'] = $extras;
    }
    return $this->_request($args);
  }

  /**
   * API.updateSubscriberDRMSerie
   * http://help.calameo.com/index.php?title=API:API.updateSubscriberDRMSerie
   *
   * TODO - the documentation is identical to API.deleteSubscriberDRMSerie, might need to confirm method
   */
  public function updateSubscriberDRMSerie($subscription_id, $login, $serie_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
      'serie_id' => $serie_id,
    );
    return $this->_request($args);
  }

  /**
   * API.deleteSubscriberDRMSingle
   * http://help.calameo.com/index.php?title=API:API.deleteSubscriberDRMSingle
   */
  public function deleteSubscriberDRMSingle($subscription_id, $login, $book_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
      'book_id' => $book_id,
    );
    return $this->_request($args);
  }

  /**
   * API.deleteSubscriberDRMPeriod
   * http://help.calameo.com/index.php?title=API:API.deleteSubscriberDRMPeriod
   */
  public function deleteSubscriberDRMPeriod($subscription_id, $login, $period_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
      'period_id' => $period_id,
    );
    return $this->_request($args);
  }

  /**
   * API.deleteSubscriberDRMSerie
   * http://help.calameo.com/index.php?title=API:API.deleteSubscriberDRMSerie
   */
  public function deleteSubscriberDRMSerie($subscription_id, $login, $serie_id) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
      'serie_id' => $serie_id,
    );
    return $this->_request($args);
  }

  /**
   * API.clearSubscriberDRMs
   * http://help.calameo.com/index.php?title=API:API.clearSubscriberDRMs
   */
  public function clearSubscriberDRMs($subscription_id, $login) {
    $args = array(
      'action' => 'API.' . __FUNCTION__,
      'subscription_id' => $subscription_id,
      'login' => $login,
    );
    return $this->_request($args);
  }
};
