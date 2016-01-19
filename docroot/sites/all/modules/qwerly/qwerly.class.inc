<?php
/**
 * @file
 * Qwerly class
 * Handles requests to the Qwerly API service, including errors.
 */
class Qwerly {

  var $url = 'http://api.qwerly.com/v1';

  var $api_key = FALSE;
  var $request = FALSE;

  /**
   * Qwerly([$key]) - Create a new instance with optional set key
   */
  function Qwerly($key = FALSE) {
    if ($key) {
      $this->set_key($key);
    }

    $this->base_request = $this->new_request();
    $this->request = $this->new_request();
  }

  /****
   * *
   * *  Helper profile/service-level functions
   * *
   * */
  function twitter($username) {
    $this->set_action('twitter');
    $this->set_var($username);

    return $this->request();
  }

  function facebook($id) {
    $this->set_action(is_numeric($id) ? 'facebook' : 'facebook/username');
    $this->set_var($id);

    return $this->request();
  }

  function email($email) {
    $this->set_action('email');
    $this->set_var($email);

    return $this->request();
  }

  /**
   * Returns a default request object
   */
  function new_request() {
    $o = new stdClass;
    $o->api_key = $this->api_key;
    $o->services = FALSE;
    $o->error = FALSE;

    return $o;
  }

  /**
   * set_var($var) - set the search variable
   */
  function set_var($var) {
    $this->request->var = $var;
    return TRUE;
  }

  /**
   * set_key($key) - set the API Key
   */
  function set_key($key) {
    $this->api_key = $key;
    return TRUE;
  }

  /**
   * set_callback($callback) - set the JSON callback to return to
   *
   * This will not work if you are parsing with PHP, and will turn off
   * parsing for this request. Use for a Javascript pass-through.
   */
  function set_callback($callback) {
    $this->request->callback = $callback;
    return TRUE;
  }

  /**
   * set_request_id($rid) - set the request ID variable
   */
  function set_request_id($rid) {
    $this->request->request_id = $rid;
    return TRUE;
  }

  /**
   * set_user_id($uid) - set the user ID
   * set_account_id($aid) - set the account ID
   *
   * 'If you have several users who have connected their Facebook or
   *  other third-party accounts to allow lookups via Qwerly, you
   *  should set the user_id and or account_id parameter that specifies
   *  on behalf of which user you are making the request. This ensures
   *  that privacy settings are preserved.'
   */
  function set_user_id($uid) {
    $this->request->user_id = $uid;
    return TRUE;
  }
  function set_account_id($aid) {
    $this->request->account_id = $aid;
    return TRUE;
  }

  /**
   * set_action($action) - set the action of the request
   *
   * In (twitter, facebook, facebook/username, email)
   */
  function set_action($action) {
    $action = explode('/', $action);

    if (in_array($action[0], array('twitter', 'facebook', 'email',))) {
      if ($action[0] == 'facebook' && $action[1] == 'username') {
        $action[0] = 'facebook/username';
      }
      $this->request->action = $action[0];

      if (array_pop($action) == 'services') {
        $this->set_services(TRUE);
      }
      return TRUE;
    }
    else {
      $this->request->error = 'Invalid action ' . $action[0] . ' set.';
      return FALSE;
    }
  }

  /**
   * set_services($bool) - define next request as a 'services' request.
   */
  function set_services($bool) {
    $this->request->services = (bool) $bool;
  }

  /**
   * request([$object]) - perform the request, merging an optional $object with
   *                      the @request object.
   *
   * Returns: a stdClass (JSON) object on success, or FALSE on error.
   */
  function request($object = FALSE) {
    if (!is_object($object)) {
      $object = @(object) $object;
    }
    // merge object into request, trying to use defined functions first.
    foreach ($object as $key => $value) {
      if (method_exists($this, 'set_' . $key)) {
        call_user_method('set_' . $key, $this, $value);
      }
      else {
        // if it's here then it's probably not used in the actual request.
        $request->$key = $value;
      }
    }

    // return if there are errors at this stage.
    if ($this->request->error) {
      return FALSE;
    }
    // merging done, form request URL.
    $url = array($this->url, $this->request->action, $this->request->var,);

    if ($this->request->services) {
      $url[] = 'services';
    }
    // copy request to last_request var before clearing stuff
    $this->last_request = $this->request;

    // clear out unwanted vars.
    unset($this->request->action);
    unset($this->request->var);
    unset($this->request->services);

    // build ?foo=bar&red=blue portion of query
    $params = http_build_query((array) $this->request);

    // put the request object back to empty for next request.
    $this->request = $this->new_request();

    // put it all together
    $url = implode('/', $url) . '?' . $params; //implode('&', $vars);
    $this->last_request->url = $url;


    // eggs-are-cute using best function
    if (function_exists('curl_init')) {
      $result = $this->_do_curl($url);
    }
    else {
      $result = $this->_do_base($url);
    }

    if (!$result) {
      return FALSE;
    }
    // try handle the return data as JSON
    if ($json = @json_decode($result)) {

      $this->last_request->result = $json;
      $this->last_request->status = $json->status;

      return $this->last_request;

    }
    else {
      // couldn't decode as JSON
      $this->last_request->error = 'Qwerly: ' . strip_tags($result);
      return FALSE;
    }
  }

  /**
   * Return from a URL using CURL (requires CURL, duh)
   */
  private function _do_curl($url) {
    $ch = curl_init();

    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    $res = curl_exec($ch);
    curl_close($ch);

    if ($res == FALSE) {
      $this->last_request->error = curl_error($ch);
    }
    return $res;
  }

  /**
   * Return from a URL using file_get_contents (may fail depending on php.ini)
   */
  private function _do_base($url) {
    $data = @file_get_contents(urlencode($url));

    if ($data == FALSE) {
      $this->last_request->error = "Unable to read from $url with file_get_contents";
    }
    return $data;
  }
}
