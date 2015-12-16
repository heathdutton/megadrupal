<?php

// $Id: openerp.install,v 1.3 2011/04/19 12:05:48 ZestyBeanz -LIPIN  Exp $

/**
 * @file
 * OpenERP Proxy API
 * 
 * Copyright(c) 2010, Pinacono Co., Ltd.
 */

/**
 * @todo implement hooks?
 */

// ----------------------------------------------------------------------------
class OpenERPProxy {
  protected $messages = array(); /** message storage */
  protected $user     = NULL;    /** user information */

  // --------------------------------------------------------------------------
  // static API

  /**
  * Singleton to initialize a OpenERP api object
  */
  public static function getProxy() {
    static $obj = NULL;

    if ( ! isset($obj) ) {
      $obj = new OpenERPProxy();
    }

    return $obj;
  }

  /**
  * Volatile caching facilities for OpenERP API. This caching is per http request.
  * It will be re-initialized on every http request.
  *
  * @param array $hash_params - array of parameters to be used as the cache key
  * @param mixed $data - data to put to cache or NULL to get data from cache
  * @return mixed - data as set or available in cache
  */
  public static function cache($hash_params, $data = NULL) {
    static $cache = array();

    $key = md5(serialize($hash_params));

    if ( isset($data) ) {
      $cache[$key] = $data;
    }
    elseif ( ! isset($cache[$key]) ) {
      $cache[$key] = NULL;
    }

    return $cache[$key];
  }

  /**
  * System and error message handler
  *
  * @param string $msg - message to be set or NULL to get all available messages
  * @return mixed - available message(s) in an array when $msg is NULL or last set message
  */
  function message($msg = NULL) {
    static $msgs = array();

    if ( isset($msg) ) {
      $msgs[] = $msg;
    }
    else {
      $msg  = $msgs;
      $msgs = array();
    }

    return $msg;
  }

  /**
  * Protected method to get the available mcrypt context
  *
  * @return array - mcrypt context information
  */
  protected static function getmCryptContext() {
    static $ctx;

    if ( ! isset($ctx) ) {
      $algorithm = variable_get('openerp.algorithm', MCRYPT_DES);
      $mode      = MCRYPT_MODE_ECB;
      $iv_size   = mcrypt_get_iv_size($algorithm, $mode);

      $ctx = array(
        'algorithm' => $algorithm,
        'mode'      => $mode,
        'iv_size'   => $iv_size,
        'iv'        => mcrypt_create_iv($iv_size),
        'key'       => variable_get('openerp.key', FALSE),
      );
    }

    return $ctx;
  }

  /**
  * encrypt text using mcrypt
  *
  * @param string $plain
  * @return string encrypted cipher text
  */
  public static function encrypt($plain) {
    if ( extension_loaded('mcrypt') ) {
      $ctx = OpenERPProxy::getmCryptContext();
      return base64_encode(mcrypt_encrypt($ctx['algorithm'], $ctx['key'], $plain, $ctx['mode'], $ctx['iv']));
    }

    return $plain;
  }

  /**
  * decrypt text using mcrypt
  *
  * @param string $cipher
  * @return string decrypted plain text
  */
  public static function decrypt($cipher) {
    if ( extension_loaded('mcrypt') ) {
      $ctx = OpenERPProxy::getmCryptContext();
      return trim(mcrypt_decrypt($ctx['algorithm'], $ctx['key'], base64_decode($cipher), $ctx['mode'], $ctx['iv']));
    }

    return $cipher;
  }


  // --------------------------------------------------------------------------
  // AUthentication and Preferences API

  /**
  * send plain request to OpenERP
  *
  * @param string $module - OpenERP module name
  * @param string $action - action to perform
  * @param array $args - arguments for the action
  * @return mixed - result returned from OpenERP, according to the request
  */
  public function request($module, $action, $args = array()) {
  	$ultimate_callback_args = array();
    $url  = variable_get('openerp.url', '');
    if ( empty($url) ) {
      OpenERPProxy::message('Invalid OpenERP URL, please re-configure OpenERP');
      return NULL;
    }
  	$path = $url."/".$module;
  	$ultimate_callback_args[0]=$path;
  	$ultimate_callback_args[1][$action]=$args;
        // real request
        $status = call_user_func_array('xmlrpc',$ultimate_callback_args );

    // looking for errors;
    $errno = xmlrpc_errno();
    $msg   = xmlrpc_error_msg();
    if ( $errno | $msg) {
      OpenERPProxy::message("OpenERP error - {$errno}<br/><pre>$msg</pre>");
    }
    
    return $status;
  }

  /**
  * send request to OpenERP server with user priviledges, auto authenticate the user if required.
  *
  * @param string $module - OpenERP module name
  * @param string $action - action to perform
  * @param array $args - arguments for the action
  * @return mixed - result returned from OpenERP, according to the request
  */
  public function userRequest($module, $action, $args = array()) {
    if ( $this->login() === FALSE ) {
      return FALSE;
    }
    return $this->request($module, $action, array_merge(array($this->user->db, $this->user->uid, $this->user->passwd), $args));
  }

  /**
  * Login to OpenERP and get the operation context
  *
  * @return mixed - FALSE on error, user id as an integer on success
  */
  public function login() {
    global $user;
   

    if ( isset($this->user->uid) ) {
    
    	    
      return $this->user->uid;
      
    }

    // try loading account info corresponding to the current user
    if ( (! isset($this->user) || empty($this->user) ) && ($this->user = openerp_loaduser($user->uid)) === FALSE ) {
      $this->user = new stdClass();
     
   
    }

    // load defaults, if required
    $url = isset($this->user->url)    ? $this->user->url    : variable_get('openerp.url', NULL);
    $db  = isset($this->user->db)     ? $this->user->db     : variable_get('openerp.database', NULL);
    $usr = isset($this->user->user)   ? $this->user->user   : variable_get('openerp.user', NULL);
    $pwd = isset($this->user->passwd) ? $this->user->passwd : OpenERPProxy::decrypt(variable_get('openerp.password', NULL));

    // check qualification
    if ( ! isset($url) || ! isset($usr) || ! isset($pwd) || ! isset($db) ) {
      OpenERPProxy::message('Incomplete user account information');
     return FALSE;
      
    }

    if (($uid = $this->request('common', 'login',array($db, $usr, $pwd))) === FALSE ) {
    	
      OpenERPProxy::message('OpenERP cannot authenticate user');
     
      return FALSE;
    }

    $this->user->uid    = $uid;
    $this->user->url    = $url;
    $this->user->db     = $db;
   // $this->user->user   = $login;
    $this->user->passwd = $pwd;

    
    // get user context
    $context = new stdClass();

    if ( ($r = $this->execute('res.users', 'context_get')) === FALSE ) {
    	
      return FALSE;
    }

    $context->context = (object) $r;

    if ( ($r = $this->read('res.users', array($uid))) === FALSE ) {
    	
      return FALSE;
    }

    $context->user = (object) $r[0];
    $context->lang = (object) $r[0];
    $context->timezone = $r;

    // populate openerp context
    $this->user->context = $context;
    return $uid;
    
   
  }

  /**
  * Perform the OpenERP's 'execute' action on an 'object'
  *
  * @param string $object - OpenERP object name
  * @param string $action - action to perform using the object
  * @param array $args - arguments
  * @param bool $context - also append the user's context to the end of argument list
  *
  * @return mixed - results as return by OpenERP.
  */
  public function execute($object, $action, $args = array(), $context = FALSE) {
      $merge=array_merge(array($object,$action), $args);
      $c = array();
      if ( $context ) {
      	if ( isset($this->user->context->context) ) {
      		foreach ( $this->user->context->context as $name => $val ) {
      			$c[$name] = $this->user->context->context->{$name};
        }
      }
      $args[] = $c;
    }
    return $this->userRequest('object', 'execute', array_merge(array($object, $action), $args));
  }

  // --------------------------------------------------------------------------
  // OpenERP Model API

  /**
  * Perform 'create' operation on OpenERP model
  *
  * @param string $model - OpenERP model name
  * @param mixed $data - object or array of data to be create in an object
  */
  public function create($model, $data) {
    return $this->execute($model, 'create', array((object)$data));
  }

  /**
  * Count number of rows corresponding to the search filter
  *
  * @param string $model - OpenERP model
  * @param array $search - array of key list, e.g., array(array('key', '=', 'value'))
  * @return mixed - FALSE on error, number of corresponding rows on success
  */
  public function count($model, $search = array()) {
    if ( ($count = OpenERPProxy::cache(array($model, $search))) === NULL ) {
      $count = OpenERPProxy::cache(array($model, $search), $this->execute($model, 'search_count', array($search), TRUE));
    }

    return $count;
  }

  /**
  * Peroform 'search' operation on OpenERP model name
  *
  * @param string $model - OpenERP model
  * @param array $search - array of key list, e.g., array(array('key', '=', 'value'))
  * @param int $offset - offset
  * @param int $limit - maximum number of rows of result (0 = nolimit)
  * @return mixed - FALSE on error, array of results on success
  */
  public function search($model, $search = array(), $offset = 0, $limit = -1) {
    if ( $limit < 0 ) {
      $limit = (int) variable_get('openerp.default_page_size', 40);
    }
    if ( ($data = OpenERPProxy::cache(array($model, $search, (int) $offset, (int) $limit))) === NULL ) {
      if ( ($data = $this->execute($model, 'search', array($search, (int) $offset, (int) $limit)))=== FALSE ) {
      
      	return FALSE;
      }
     OpenERPProxy::cache(array($model, $search, $offset, $limit), $data);
   }
    return $data;
  }

  /**
  * Perform 'read' operation on OpenERP model
  *
  * @param string $model - OpenERP model name
  * @param mixed $ids - id or list of ids in an array
  * @param mixed $fields - field name or list of fields name in an array
  * @return mixed - FALSE on error, array of results on success
  */
  public function read($model, $ids = array(), $fields = array()) {
    if ( ($data = OpenERPProxy::cache(array($model, $ids, $fields))) === NULL ) {
      $data = OpenERPProxy::cache(array($model, $ids, $fields), $this->execute($model, 'read', array((array) $ids, (array) $fields)));
    }
    return $data;
  }

  /**
  * retrieve the result of the specific page of data
  *
  * @param string $model - OpenERP model
  * @param array $search - array of key list, e.g., array(array('key', '=', 'value'))
  * @param int $offset - row to start
  * @param int $limit - maximum number of rows of result ( <= 0 = use default)
  * @return mixed - FALSE on error, array of results on success
  */
  public function page($model, $search = array(), $offset, $limit = 0) {
    if ( $limit <= 0 ) {
      $limit = (int) variable_get('openerp.default_page_size', 40);
    }

    if ( ($ids = $this->search($model, $search, $offset, $limit)) === FALSE ) {
      return FALSE;
    }

    if ( ($data = $this->read($model, $ids)) === FALSE ) {
      return FALSE;
    }

    return $data;
  }

  /**
  * Perform 'write' operation on OpenERP model
  *
  * @param string $model - OpenERP model name
  * @param mixed $ids - id or list of ids in an array
  * @param mixed $data - object or array of data to be written
  * @return bool - FALSE on error, TRUE on success
  */
  public function write($model, $ids, $data) {
    return $this->execute($model, 'write', array((array)$ids, (object) $data));
  }

  /**
  * Perform 'unlink' operation on OpenERP model
  *
  * @param string $model - OpenERP model name
  * @param mixed $ids - id or list of ids in an array
  * @return bool - FALSE on error, TRUE on success
  */
  public function unlink($model, $ids = array()) {
    return $this->execute($model, 'unlink', array((array) $ids));
  }
}
