<?php

/*
 Drupal release
 James Michael-Hill
 www.netforumondrupal.com

 xWebSecureOD Class - an extension of the php SoapClient class to provide for
 seamless sliding token based authentication with Avectra's netFORUM On Demand / Team.

 Developed under php 5.1.6/apache 2.  PHP 5 is REQUIRED.

 Where possible functionality is parallel to the regular SoapClient with the exception of the
 constructor and the __soapCall() method - which will not take any soap headers.

 The constructor requires xwebUserName and xwebUserPass to be set.
 These can be your regular user name and password, but that is NOT RECOMMENDED.  See the
 Avectra documentation for best practices, but in a nutshell there should be a dedicated user, and the
 password is set in the usr_pwd field of the fw_user table.  For more examples see the included
 xwebExample.php file.

 Drupal Version:
 This release is intended for use solely with the drupal CMS system.
 Changes have been made to the database caching routines to work with the drupal framework.  If you're
 interested in using this class for other purposes please contact me or look for the official
 xWebSecureOD class.

 */
class xwebSecureODClient extends SoapClient {
  private $userName;
  private $userPass;
  private $authToken;
  private $xwebNamespace;
  private $overloadedWsdlFunctions = array();
  private $wsdlNonOverloadFunctions = array('Authenticate');

  //new for response caching
  private $cacheExpireTime = ""; //the default is 4 hours ago, and it is set in the constructor
  private $cachedFunctions = array(
    'GetFacadeObject',
    'GetQuery',
    'GetCustomerBalance',
    'GetCustomerMembership',
    'GetCustomerByZip',
    'GetCustomerByCityState',
    'GetCustomerByRecordDate',
    'GetCustomerByName',
    'GetCustomerById',
    'GetCustomerEvent',
    'GetCustomerSession',
    'GetEventListByName',
    'GetEventByKey',
    'GetSessionListByName',
    'GetSessionByKey',
    'GetSessionListByEvent',
    'GetEventCustomerList',
    'GetSessionCustomerList',
    'GetProductBalances',
    'GetCommitteeByKey',
    'GetCommitteeByCode',
    'GetCommitteeListByCode',
    'GetCommitteeListByCstId',
  );
  private $cacheWsdlLoc = "";
  private $cachingOn = false;
  private $cachedResponse = false;
  private $responseFromCache = false;

  private $offlineMode = false;

  //for debugging purposes.  Not perfect, but if something is going awry it gives some insight
  public $log;

  function __construct($wsdl, $options = null) {
    if (isset($options['xwebUserName'], $options['xwebUserPass'])) {
      $this->userName = $options['xwebUserName'];
      $this->userPass = $options['xwebUserPass'];
      parent::__construct($wsdl, $options);
      $this->setOverloadedWsdlFunctions();
      $this->cacheWsdlLoc = $wsdl;
      //fetch the stored auth token from drupal
      //$this->authToken = variable_get('netforum_auth_token','');
    }
    else {
      //throw constructor error if we don't have the needed parameters
      throw new Exception("Invalid parameters in xwebSecureClient constructor:  xwebUserName and xwebUserPass are required");
    }
    $this->cacheExpireTime = strftime("%Y-%m-%d %H:%M", strtotime("-4 hours"));
    $this->log .= "Finished constructor\n";
  }

  function __call($method, $arguments) {
    //the _call method is executed for every method call in this class.  We're using it to wrap every wsdl function call in the xweb authentication scheme

    //we're only overloading the functions that the wsdl defines here, so check to see if it is in our list
    if (in_array($method, $this->overloadedWsdlFunctions) && ! in_array($method, $this->wsdlNonOverloadFunctions)) {
      $this->log .= "Overloading the call to $method method\n";
      //note that this is the overloaded soap call method that adds the auth tokens
      return $this->__soapCall($method, $arguments);
    }
  }

  function __soapCall($fname, $arguments, $options = null, $inputHeaders = null, &$responseHeaders = '') {
    //overload the soap call function to only take a wsdl function name and an array of arguments, inject our auth token, and save the response auth token
    $this->log .= "Beginning __SoapCall\n";
    $this->cachedResponse = false;

    if ($this->cachingOn === true ) {
      $response = $this->cacheRetreive($fname, $arguments);
      if ($response) {
        $this->log .= "Returning cached response to call\n";
        $this->cachedResponse = true;
        return $response;
      }
      else if ($this->offlineMode === true) { // if we're in offline mode don't continue the request to the live server
        return;
      }
    }
    try {
      $response = parent::__soapCall($fname, $arguments, $options, $this->getAuthHeaders(), $responseHeaders);
      $this->authToken = $responseHeaders['AuthorizationToken']->Token;
      if ($this->cachingOn === true && in_array($fname, $this->cachedFunctions)) {
        $this->log .= "Caching response to soap call for future use\n";
        $this->cacheStore($fname, $arguments, $response);
      }
    }
    catch (SoapFault $exception) {
      // if it is a bad token try re-authenticating - but only once
      if (stristr($exception->faultstring, "Invalid Token Value")) {
        $this->log .= "Caught exception with invalid token value, re-authenticating and trying one more time\n";
        $this->authToken = '';
        try {
          $response = parent::__soapCall($fname, $arguments, null, $this->getAuthHeaders(), $responseHeaders);
          $this->authToken = $responseHeaders['AuthorizationToken']->Token;
          //store the auth token with drupal for later use
          //variable_set('netforum_auth_token',$this->authToken);
          if ($this->cachingOn === true && in_array($fname, $this->cachedFunctions)) {
            $this->log .= "Caching response to soap call for future use\n";
            $this->cacheStore($fname, $arguments, $response);
          }
        }
        catch (SoapFault $exception) {
          $this->log .= "Caught exception in soap call to $fname again - bad authentication token\n";
          throw $exception;
        }
      }
      else {
        $this->log .= "Caught exception in soap call to $fname \n";
        //reset the auth token since a bad request invalidates any previous auth token.  This will save us a step if we try again.
        $this->authToken = '';
        throw $exception;
      }
    }

    return $response;
  }

  function __doRequest($request, $location, $action, $version, $oneway = 0) {
    //really, this is only overloaded for debugging purposes - I want to be able to see what the final soap call is for each step,
    //that way we can track the auth tokens.  Feel free to remove this as needed
    $this->log .= "Beginning __doRequest\n";
    $this->log .= "Params for __doRequest: \nRequest: $request\nLocation: $location\nAction: $action\nVersion: $version\n\n";
    return parent::__doRequest($request, $location, $action, $version, $oneway);
  }

  public function clearLog() {
    $this->log = '';
  }

  //Turn on caching for requests.  It takes the db handle, the db type, and the expire time.  By default this is four hours ago, but can be overridden.
  //it should come as a regular unixtime seconds from 1970, since we'll later call stftime("format",$expire_time) on it.
  public function setCaching($expire_time = '') {
    if ($expire_time != '') {
      if (is_string($expire_time)) {
        $expire_time = strtotime($expire_time);
      }
      if ($expire_time > REQUEST_TIME) { //this is a boo boo, we want negatives here, so set it to the difference
        $expire_time = REQUEST_TIME - ($expire_time - REQUEST_TIME);
      }
      $this->cacheExpireTime = strftime("%Y-%m-%d %H:%M", $expire_time);
    }
    $this->cachingOn = true;
    return true;
  }

  //turns the caching off
  public function disableCaching() {
    if ($this->cachingOn) {
      $this->log .= "Disabling caching\n";
      $this->cachingOn = false;
    }
  }

  public function cachingOn() {
    return $this->cachingOn;
  }

  //If the last response returned was cached, this will return true.
  public function cachedLastResponse() {
    return $this->cachedResponse;
  }

  //If the last response came from the cache this will return true;
  public function lastResponseFromCache() {
    return $this->responseFromCache;
  }

  public function enableOfflineMode() {
    if ($this->cachingOn) {
      $this->log .= "Enabling offline mode\n";
      $this->offlineMode = true;
    }
  }

  public function disableOfflineMode() {
    if ($this->offlineMode === true) {
      $this->log .= "Disabling offline mode\n";
      $this->offlineMode = false;
    }
  }

  private function getAuthHeaders() {
    //this function is used to get the proper headers for inclusion in our own __soapCall method

    // if we don't have a saved auth token get one
    if ( (! isset($this->authToken)) || trim($this->authToken == '')) {
      $this->log .= "Fetching new authToken\n";
      //these are the params set in the constructor
      $authReqParams = array(
        'userName' => $this->userName,
        'password' => $this->userPass,
      );
      $responseHeaders = '';
      try {
        //run the soap call to get it - with the headers.  Use the parent soap call in case we overload our soap method
        $response = parent::__SoapCall("Authenticate", array('parameters' => $authReqParams), null, null, $responseHeaders);
        $this->authToken = $responseHeaders['AuthorizationToken']->Token;
        //store the auth token with drupal for later use
        //variable_set('netforum_auth_token',$this->authToken);
        $this->xwebNamespace = $response->AuthenticateResult;
      }
      catch (SoapFault $exception) {
        throw $exception;
      }
    }

    //return the header we oh so want.
    return new SoapHeader($this->xwebNamespace, 'AuthorizationToken', array('Token' => $this->authToken), true);
  }

  private function setOverloadedWsdlFunctions() {
    //this method will grab a list of wsdl defined functions that we will be overloading using the magic __call() method
    $functions = parent::__getFunctions();
    foreach ($functions as $fname) {
      //strip the actual function name out for our uses
      $start = strpos($fname, ' ');
      $end = strpos($fname, '(');
      //append the name of the function to our internal list, which we check in every __call()
      $this->overloadedWsdlFunctions[] = trim(substr($fname, $start, ($end - $start)));
    }
  }


  //this will cache the request and for a given function call and arguments.
  private function cacheStore($fname = '', $arguments = '', $response = '') {
    if ( $fname == '' || $arguments == '' || (! is_object($response) && $response == '')) {
      $this->log .= "Could not store response, invalid parameters passed\n";
      return false;
    }
    if ( ! in_array($fname, $this->cachedFunctions)) {
      $this->log .= "Could not store response, $fnanme is not on the list of cacheable functions\n";
      return false;
    }
        
    try {
      $res = db_insert('netforum_request_cache')->fields(
        array(
          'user_name' => $this->userName, 
          'wsdl_loc' => $this->cacheWsdlLoc, 
          'request' => $fname, 
          'arguments_sha1_hash'  => sha1(serialize($arguments)), 
          'response' => serialize($response), 
          'add_date' => strftime("%Y-%m-%d %H:%M"),
        )
      )->execute();  
    }
    catch (Exception $exception) {
      $this->log .= "Error on insert - " . print_r($exception->errorInfo, true). "\n";
      return false;
    }
    
    $this->log .= "Cached call to $fname\n";
    return true;
    
  }

  private function cacheRetreive($fname = '', $arguments = '') {
    if ($fname == '' || $arguments == '' ) {
      $this->log .= "Could not fetch response from cache, invalid parameters passed\n";
      return false;
    }
    if ( ! in_array($fname, $this->cachedFunctions)) {
      $this->log .= "Could not fetch response from cache, $fname is not on the list of cacheable functions\n";
      return false;
    }

    $res = db_query("SELECT response FROM {netforum_request_cache}
				    WHERE user_name = :user_name AND
				    wsdl_loc = :wsdl_loc AND
				    request = :request AND
				    arguments_sha1_hash = :hash AND
				    add_date >= :add_date
				    ORDER BY add_date DESC LIMIT 1", 
				    array(
				      ':user_name' => $this->userName,
				      ':wsdl_loc' => $this->cacheWsdlLoc,
				      ':request' => $fname,
				      ':hash' => sha1(serialize($arguments)),
				      ':add_date' => $this->cacheExpireTime,
				    ))->fetchField();
    
    if ($res !== false) {
      $this->log .= "Found cached response to $fname, returning from database\n";
      $this->responseFromCache = true;
      return unserialize($res); //return the plain ol' response
    }
    else {
      $this->log .= "No cached response found for $fname in database\n";
      return false;
    }
  }

}

?>
