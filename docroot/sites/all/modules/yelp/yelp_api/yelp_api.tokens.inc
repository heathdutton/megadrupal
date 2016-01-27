<?php
  /**
   * @file yelp_api.tokens.inc
   *  contains functions related to using tokens
   */
  
  /**
   * Attempts to figure out what data the given token needs and performs a token_replace()
   *
   * @param string $token
   *    The token to be replaced .
   *   
   * @return string
   *    The returned value from the token, or the token itself if not replaced . 
   */
  function yelp_api_replace_token($token) {
    $value = FALSE;
    // get info on all tokens
    $tokens  = token_info();
    // assume token is structered as [type:pointer:field]
    // only care about the type
    $type = trim(str_replace('[', '', array_shift(explode(':', $token))));
    //see if type is listed
    if (isset($tokens['types'][$type])) {
      //does this token require data?
      if (isset($tokens['types'][$type]['needs-data'])) {
        //attempt to find data object
        if ($data = yelp_api_find_token_data($tokens['types'][$type]['needs-data'])) {
          $value = token_replace($token, $data);
        }
      }
      else{
        //does not need data so just replace . 
        $value = token_replace($token);
      }
    }
    //return the value if the token was replaced . 
    return ($value && ($value!=$token)) ? $value:FALSE;
  }
  
  /**
   * Attempts to find the data object needed to replace a token. Invoked by yelp_api_replace_token()
   *
   * @param string $data_type
   *  the name of the data type
   *
   * @return array
   *  either boolean FALSE if data is not found, or an associative array structed as array('$data_type' => $data);
   *
   * @see yelp_api_replace_token()
   */
  function yelp_api_find_token_data($data_type) {
    $data = FALSE;
    switch ($data_type) {
      case 'node':
        //get the current node
        if (arg(0) == 'node' && is_numeric(arg(1))) {
          //load the current node
          $data  = array('node' => node_load(arg(1)));
        }
        break;
      case 'node_type':
        break;
      case 'menu-link':
        break;
      case 'path':
        break;
      case 'array':
        break;
      case 'comment':
        break;
      case 'date':
        break;
      case 'file':
        break;
      case 'term':
        break;
      case 'vocabulary':
        break;
      case 'user':
        global $user;
        if ($user->uid > 0) {
          $data = array('user' => $user);
        }
        break;
      case 'menu':
        break;
      default:
        //nothing
        break;
    }
    return $data;
  }