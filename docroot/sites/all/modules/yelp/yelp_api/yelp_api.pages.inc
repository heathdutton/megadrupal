<?php
  /**
   * @file yelp_api.pages.inc
   * General functions for the yelp api module
   */

  /**
   * Format the parameters used for a search query. Invoked by yelp_api_get_results()
   * 
   * @param array $unformatted
   *  an unformatted array of search parameters
   *
   * @return array
   *  a correctly formatted search parameters array
   *
   * @see yelp_api_get_results()
   */
  function _yelp_api_format_search_parameters($unformatted) {
    module_load_include('inc' , 'yelp_api' , 'yelp_api.tokens');
    $search_parameters = array();
    foreach ($unformatted as $key => $value) {
      //does value have tokens in it?
      if (module_exists('token') && is_string($value) && preg_match('/\[[^\]]+\]/', $value)) {
        $value = preg_replace_callback('/\[([^\]]+)\]/', '_yelp_api_callback_replace_tokens', $value);
      }
      
      if ($value!==FALSE) {
        //clean values based on key
        switch ($key) {
          case 'term':
            $search_parameters['term'] = trim(strip_tags($value));
            break;
          case 'max_results':
            if ((intval($value)!=$value)||($value<1)||($value>20)) {
              $value = 20;
            }
            $search_parameters['limit'] = $value;
            break;
          case 'offset':
            if (intval($value)!=$value) {
              $value = 0;
            }
            $search_parameters['offset'] = $value;
            break;
          case 'sort':
            if (!in_array($value , array(0 , 1 , 2))) {
              $value = 1;
            }
            $search_parameters['sort'] = $value;
            break;
          case 'categories':
            $categories = yelp_api_get_categories(TRUE);
            if (!is_array($value)) {
              $value = explode(',' , $value); 
            }
            //only use valid categories
            $cvals = array();
            foreach ($value as $cat) {
              if (isset($categories[$cat])) {
                $cvals[] = $cat;
              }
            }
            $value = implode(',' , $cvals); 
            $search_parameters['category_filter'] = $value;
            break;
          case 'radius':
            if ((intval($value)!=$value)||$value>25) {
              $value = 25;
            }
            //convert miles to meters (1 mile = 1609.344 meters)
            $search_parameters['radius_filter'] = ($value * 1609.344);
            break;
          case 'deals':
            if (($value!=0) && ($value != 1)) {
              $value = 0;
            }
            $search_parameters['deals_filter'] = $value;
            break;
          case 'location':
            $search_parameters['location'] = preg_replace('/\s+/' , '+' , trim(str_replace('|' , ' ' , strip_tags($value))));
            break;
          case 'cc':
            if ($value == 'UK') {
              $value = 'GB';
            }
            elseif (!in_array($value , array_keys(_yelp_api_country_codes()))) {
              $value = variable_get('yelp_api_default_cc' , 'US');
            }
            $search_parameters['cc'] = $value;
            break;
          case 'lang':
            if (!in_array($value , array_keys(_yelp_api_lang_codes()))) {
              $value = variable_get('yelp_api_default_lang' , 'en');
            }
            $search_parameters['lang'] = $value;
            break;
          case 'lang_filter':
            if (($value!=0)&&($value!=1)) {
              $value = variable_get('yelp_api_defualt_lang_filter' , 0);
            }
            $search_parameters['lang_filter'] = $value;
            break;
        }
      }
    }
    
    if (!isset($search_parameters['cc'])) {
      $search_parameters['cc'] = variable_get('yelp_api_default_cc' , 'US');
    }
    if (!isset($search_parameters['lang'])) {
      $search_parameters['lang'] = variable_get('yelp_api_default_lang' , 'en');
    }
    if (!isset($search_parameters['lang_filter'])) {
      $search_parameters['lang_filter'] = variable_get('yelp_api_defualt_lang_filter' , 0);
    }
    if (!isset($search_parameters['sort'])) {
      $search_parameters['sort'] = variable_get('yelp_api_defualt_sort' , 1);
    }
    //allow other modules to alter the query if need be
    module_load_include('inc' , 'yelp_api' , 'yelp_api.hooks');
    $search_parameters = yelp_api_hook_alter_search_parameters($search_parameters);
    
    return $search_parameters;
  }

  /**
   * Callback function to call yelp_api_replace_token. Invoked by _yelp_api_format_search_parameters
   * 
   * @param array $maches
   *  array consisting of two elements, the first is the matched string (token) and the second is the token name
   *
   * @return string
   *  a string with the token replaced
   */
  function _yelp_api_callback_replace_tokens($matches) {
      return yelp_api_replace_token($matches[0]);
  }
  
  /**
   * Uses formatted search parameters to build a search query string.
   * Invoked by yelp_api_perform_search()
   *
   * @param array $search_parameters
   *  a correctly fommated array of search parameters
   *
   * @return string
   *  A string value of urlencoded search parameters
   *
   * @see yelp_api_perform_search()
   */
  function _yelp_api_format_search_query($search_parameters) {
    $query = array();
    if (is_array($search_parameters)){
      foreach ($search_parameters as $key  => $value) {
        $value = urlencode(trim(preg_replace('/\s+/' , ' ' , $value)));
        $query[] = $key . '=' . $value;
      }
    }
    return implode('&' , $query);
  }
  
  /**
   * Retrieves search results from Yelp. Invokes a Yelp API search call.
   *
   * @param array $search_parameters
   *  an array of unformatted search parameters
   *
   * @return array
   *  a json_decoded array of search results
   */
  function yelp_api_get_results($search_parameters) {
    if (!is_array($search_parameters)) {
      if (is_numeric($search_parameters)) {
        $search_parameters = (array)yelp_api_get_data($search_parameters);
      }
      else{
        return FALSE;
      }
    }
    //format the search parameters
    $search_parameters = _yelp_api_format_search_parameters($search_parameters);
    $results = yelp_api_perform_search($search_parameters);
    return $results;
  }
  
  /**
   * Performs a search of the Yelp API to retrieve an array of businesses.
   * Function will first to see if a listing of businesses for the passed parameters is cached.
   * Invoked by yelp_api_get_results()
   *
   * @param array $search_parameters
   *  an array of Yelp search parameters
   *
   * @return array
   *  Returns an array of Yelp search result objects, or a boolean FALSe if failed.
   *
   * @see yelp_api_get_results()
   */
  function yelp_api_perform_search($search_parameters) {
    //build search query
    $search_query       = _yelp_api_format_search_query($search_parameters);
    //see if results are cached
    if ($results = cache_get('yelp-api-search-' . $search_query , 'cache')) {
      $results = $results->data;
      //do nothing
    //else perform the search  
    }
    elseif ($results = _yelp_api_apiCall('http://api.yelp.com/v2/search?' . $search_query)) {
      //save results to cache for x hour(s) ,  default is 1
      $results_cache_hours  = variable_get('yelp_api_cache_results' , 1);
      cache_set('yelp-api-search-' . $search_query , $results , 'cache' , mktime(date('H')+$results_cache_hours));
    }

    if ($results) {
      $results = json_decode($results);
      module_load_include('inc' , 'yelp_api' , 'yelp_api.hooks');
      //allow other modules to alter the returned results
      $results = yelp_api_hook_alter_search_results($results , $search_parameters);
    }
    else{
      $results = FALSE;
    }
    return $results;
  }
  
  /**
   * Returns an arrray of stored Yelp API key values
   *
   * @return array
   *  an associative array of Yelp API keys
   *    - consumer key
   *    - consumer secret
   *    - token
   *    - token secret
   */
   function _yelp_api_get_keys() {
    return array(
      'consumer_key'      => variable_get('yelp_api_consumer_key' , FALSE) , 
      'consumer_secret'   => variable_get('yelp_api_consumer_secret' , FALSE) , 
      'token'             => variable_get('yelp_api_token' , FALSE) , 
      'token_secret'     => variable_get('yelp_api_token_secret' , FALSE) , 
    );
   }
  
  /**
   * Takes an unsigned url call to yelp and signs it using OAuth. Invoked by _yelp_api_apiCall()
   *
   * @param string $unsigned_url
   *  the unsigned yelp URL
   *
   * @return string
   *  the signed url ,  or FALSE (if missing oauth credentials)
   *
   * @see _yelp_api_apiCall()
   */
  function _yelp_api_generate_signed_url($unsigned_url) {
    module_load_include('php' , 'yelp_api' , 'OAuth');
    $keys = _yelp_api_get_keys();
    list($consumer_key , $consumer_secret , $token , $token_secret) = array_values($keys); 
    if ($consumer_key && $consumer_secret && $token && $token_secret) {
      //token object built using OAuth
      $token            = new OAuthToken($token , $token_secret);
      //consumer object build using OAuth
      $consumer          = new OAuthconsumer($consumer_key , $consumer_secret);
      //encode signiture
      $signature_method  = new OAuthSignatureMethod_HMAC_SHA1();
      //build oAuth request
      $oauth_request    = OAuthRequest::from_consumer_and_token($consumer , $token , 'GET' , $unsigned_url);
      //sign the request
      $oauth_request->sign_request($signature_method , $consumer , $token);
      //return the signed url
      return $oauth_request->to_url();
    }
    else{
      $missing = array();
      foreach ($keys as $key => $value) {
        if (!$value) {
          $missing[] = $key;
        }
      }
      watchdog('yelp_api' , 'Cannot generate signed url missing keys: %keys' , array('%keys' => implode(', ' , $missing)) , WATCHDOG_ERROR);
    }
    //missing a nessisary value ,  return FALSE . 
    return FALSE;
  }
  
  /**
   * Makes a call to the YELP API with a signed url.
   *
   * @param string $unsigned_url
   *   the unsigned Yelp url to call
   *
   * @return json
   *  Return a json string of data or a boolean FALSE if failed.
   */
  function _yelp_api_apiCall($unsigned_url) {
    //need to generate oauth url
    if ($signed_url = _yelp_api_generate_signed_url($unsigned_url)) {
      if ($data = _yelp_api_doCall($signed_url)) {
        $test = json_decode($data);
        // check returned data for errors (and log)
        if (isset($test->error)) {
          $error = 'TEXT:' . $test->error->text . ' , ID:' . $test->error->id . (isset($test->error->field) ? ' , FIELD:' . $test->error->field:'');
          drupal_set_message(check_plain('YELP API ERROR: ' . $test->error->text) , 'error');
          watchdog('yelp_api' , 'Query[%url] returned error[%error]' , array('%url' => $unsigned_url , '%error' => $error) , WATCHDOG_ERROR);
          $data = FALSE;
        }
      }
      else{
        $data = FALSE;
      }
    }
    else{
      $data = FALSE;
    }
    return $data;
  }
  
  
  /**
   * Retrieve data from an external source by making a cURL call or file_get_contents
   *
   * @param string $url
   *  the url to call
   *
   * @return string
   *  The data reterieved from the call, or boolean FALSE if failed.
   */
  function _yelp_api_doCall($url) {
    //able to use cURL to retrieve data?
    if (function_exists('curl_init')) {
      $ch = curl_init($url);
      curl_setopt($ch , CURLOPT_RETURNTRANSFER , 1);
      curl_setopt($ch ,  CURLOPT_HEADER ,  0);
      $data  = curl_exec($ch);
      curl_close($ch);
    //no curl ,  then try to get the contents
    }
    elseif ($data = @file_get_contents($url)) {
      //yay ,  we have the data
    }
    else{
      $data = FALSE;
    }
		
		if ($data){
	    return $data;
		}else{
			watchdog('yelp_api','Unable to make a successful call to \'%url\'',array('%url'=>$url),WATCHDOG_WARNING);
		}
		return false;
  }
  
  /**
   * Retrieves a yelp_api object based on the yelp_id stored in the yelp_data database table.
   * Yelp API objects contain search parameters for the yelp api.
   *
   * @param int $yelp_id
   *   The id of the yelp object
   *
   * @return object
   *  Returns a yelp_api object or boolean FALSe if failed.
   */
  function yelp_api_get_data($yelp_id) {
    $data = db_select('yelp_data' , 'yd')->fields('yd')->condition('yelp_id' , $yelp_id)->execute()->fetchObject();
    //unserialize the catagories
    $data->categories = unserialize($data->categories);
    //allow other modules to alter the yelp object
    module_load_include('inc' , 'yelp_api' , 'yelp_api.hooks');
    $data = yelp_api_hook_load_yelp_data($data , $yelp_id);
    return $data;
  }
  
  /**
   * Saves Yelp API search parameters to the yelp_data database table
   *
   * @param array $data
   *  an associative array of yelp_data fields and values
   *
   * @return int
   *  Returns the yelp_id for the newly saved data, or boolean FALSe if failed.
   */
  function yelp_api_save_data($data) {
    module_load_include('inc' , 'yelp_api' , 'yelp_api.hooks');
    if (isset($data['yelp-id'])) {
      $yelp_id = $data['yelp-id'];
      unset($data['yelp-id']);
      //allow other modules to alter the yelp_data before updating
      $data = yelp_api_hook_pre_update_yelp_data($data , $yelp_id);
      //filter for only fields for the yelp_data table
      $save_data = yelp_api_filter_data($data);
      db_update('yelp_data')->fields($save_data)->condition('yelp_id' , $yelp_id)->execute();
    }
    else{
      //allow other modules to alter the yelp_data before updating
      $data = yelp_api_hook_pre_save_yelp_data($data);
      //filter for only fields for the yelp_data table
      $save_data = yelp_api_filter_data($data);
      $yelp_id = db_insert('yelp_data')->fields($save_data)->execute();
    }
    
    if ($yelp_id) {
      //allow other modules to use data after update
      yelp_api_hook_post_save_yelp_data($data , $yelp_id);
    }
    return $yelp_id;
  }
  
  /**
   * Deletes stored yelp api search parameters from the yelp_data table based on yelp_id
   *
   * @param int $yelp_id
   *  The yelp_id of the data to delete
   *
   * @return nothing
   *  this function returns nothnig
   */
  function yelp_api_delete_data($yelp_id) {
    //allow other modules to execute code before deleting yelp data
    module_load_include('inc' , 'yelp_api' , 'yelp_api.hooks');
    yelp_api_hook_pre_delete_yelp_data($yelp_id);
    db_delete('yelp_data')->condition('yelp_id' , $yelp_id)->execute();
    yelp_api_hook_post_delete_yelp_data($yelp_id);
  }
  
  /**
   * Filters data about to be saved to the ylep_data database table so taht it only contains valid field values.
   * Invoked from yelp_api_save_data()
   *
   * @param array $data
   *  an associative array of database fields and values
   *
   * @return array
   *  A filtered array containing only the fields from the yelp_data table.
   *
   * @see yelp_api_save_data()
   */
  function yelp_api_filter_data($data) {
    //get columns in our table
    $filtered_data = array();
    if ($schema = drupal_get_schema('yelp_data')){
      foreach ($schema['fields'] as $field=>$info) {
        if (isset($data[$field])) {
          $filtered_data[$field] = $data[$field];
        }
      }
      $data = $filtered_data;
    }
    
    return $data;
  }
  
  /**
   * Returns a list of counrty codes available to Yelp
   *
   * @return array
   *  an associative array of country codes
   */
  function _yelp_api_country_codes() {
    return array(
      'US'  => 'United States' , 
      'CA'  => 'Canada' , 
      'GB'  => 'United Kingdom' , 
    );
  }
  
  /**
   * Returns a list of language codes available to Yelp
   *
   * @return array
   *  an accosicative array of language codes
   */
  function _yelp_api_lang_codes() {
    return array(
      'aa'  => 'Afar' , 
      'ab'  => 'Abkhaz' , 
      'ae'  => 'Avestan' , 
      'af'  => 'Afrikaans' , 
      'ak'  => 'Akan' , 
      'am'  => 'Amharic' , 
      'an'  => 'Aragonese' , 
      'ar'  => 'Arabic' , 
      'as'  => 'Assamese' , 
      'av'  => 'Avaric' , 
      'ay'  => 'Aymara' , 
      'az'  => 'Azerbaijani' , 
      'ba'  => 'Bashkir' , 
      'be'  => 'Belarusian' , 
      'bg'  => 'Bulgarian' , 
      'bh'  => 'Bihari' , 
      'bi'  => 'Bislama' , 
      'bm'  => 'Bambara' , 
      'bn'  => 'Bengali' , 
      'bo'  => 'Tibetan Standard ,  Tibetan ,  Central' , 
      'br'  => 'Breton' , 
      'bs'  => 'Bosnian' , 
      'ca'  => 'Catalan; Valencian' , 
      'ce'  => 'Chechen' , 
      'ch'  => 'Chamorro' , 
      'co'  => 'Corsican' , 
      'cr'  => 'Cree' , 
      'cs'  => 'Czech' , 
      'cu'  => 'Old Church Slavonic ,  Church Slavic ,  Church Slavonic ,  Old Bulgarian ,  Old Slavonic' , 
      'cv'  => 'Chuvash' , 
      'cy'  => 'Welsh' , 
      'da'  => 'Danish' , 
      'de'  => 'German' , 
      'dv'  => 'Divehi; Dhivehi; Maldivian;' , 
      'dz'  => 'Dzongkha' , 
      'ee'  => 'Ewe' , 
      'el'  => 'Greek ,  Modern' , 
      'en'  => 'English' , 
      'eo'  => 'Esperanto' , 
      'es'  => 'Spanish; Castilian' , 
      'et'  => 'Estonian' , 
      'eu'  => 'Basque' , 
      'fa'  => 'Persian' , 
      'ff'  => 'Fula; Fulah; Pulaar; Pular' , 
      'fi'  => 'Finnish' , 
      'fj'  => 'Fijian' , 
      'fo'  => 'Faroese' , 
      'fr'  => 'French' , 
      'fy'  => 'Western Frisian' , 
      'ga'  => 'Irish' , 
      'gd'  => 'Scottish Gaelic; Gaelic' , 
      'gl'  => 'Galician' , 
      'gn'  => 'Guaraní' , 
      'gu'  => 'Gujarati' , 
      'gv'  => 'Manx' , 
      'ha'  => 'Hausa' , 
      'he'  => 'Hebrew (modern)' , 
      'hi'  => 'Hindi' , 
      'ho'  => 'Hiri Motu' , 
      'hr'  => 'Croatian' , 
      'ht'  => 'Haitian; Haitian Creole' , 
      'hu'  => 'Hungarian' , 
      'hy'  => 'Armenian' , 
      'hz'  => 'Herero' , 
      'ia'  => 'Interlingua' , 
      'id'  => 'Indonesian' , 
      'ie'  => 'Interlingue' , 
      'ig'  => 'Igbo' , 
      'ii'  => 'Nuosu' , 
      'ik'  => 'Inupiaq' , 
      'io'  => 'Ido' , 
      'is'  => 'Icelandic' , 
      'it'  => 'Italian' , 
      'iu'  => 'Inuktitut' , 
      'ja'  => 'Japanese (ja)' , 
      'jv'  => 'Javanese (jv)' , 
      'ka'  => 'Georgian' , 
      'kg'  => 'Kongo' , 
      'ki'  => 'Kikuyu ,  Gikuyu' , 
      'kj'  => 'Kwanyama ,  Kuanyama' , 
      'kk'  => 'Kazakh' , 
      'kl'  => 'Kalaallisut ,  Greenlandic' , 
      'km'  => 'Khmer' , 
      'kn'  => 'Kannada' , 
      'ko'  => 'Korean' , 
      'kr'  => 'Kanuri' , 
      'ks'  => 'Kashmiri' , 
      'ku'  => 'Kurdish' , 
      'kv'  => 'Komi' , 
      'kw'  => 'Cornish' , 
      'ky'  => 'Kirghiz ,  Kyrgyz' , 
      'la'  => 'Latin' , 
      'lb'  => 'Luxembourgish ,  Letzeburgesch' , 
      'lg'  => 'Luganda' , 
      'li'  => 'Limburgish ,  Limburgan ,  Limburger' , 
      'ln'  => 'Lingala' , 
      'lo'  => 'Lao' , 
      'lt'  => 'Lithuanian' , 
      'lu'  => 'Luba-Katanga' , 
      'lv'  => 'Latvian' , 
      'mg'  => 'Malagasy' , 
      'mh'  => 'Marshallese' , 
      'mi'  => 'Maori' , 
      'mk'  => 'Macedonian' , 
      'ml'  => 'Malayalam' , 
      'mn'  => 'Mongolian' , 
      'mr'  => 'Marathi (Mara?hi)' , 
      'ms'  => 'Malay' , 
      'mt'  => 'Maltese' , 
      'my'  => 'Burmese' , 
      'na'  => 'Nauru' , 
      'nb'  => 'Norwegian Bokmål' , 
      'nd'  => 'North Ndebele' , 
      'ne'  => 'Nepali' , 
      'ng'  => 'Ndonga' , 
      'nl'  => 'Dutch' , 
      'nn'  => 'Norwegian Nynorsk' , 
      'no'  => 'Norwegian' , 
      'nr'  => 'South Ndebele' , 
      'nv'  => 'Navajo ,  Navaho' , 
      'ny'  => 'Chichewa; Chewa; Nyanja' , 
      'oc'  => 'Occitan' , 
      'oj'  => 'Ojibwe ,  Ojibwa' , 
      'om'  => 'Oromo' , 
      'or'  => 'Oriya' , 
      'os'  => 'Ossetian ,  Ossetic' , 
      'pa'  => 'Panjabi ,  Punjabi' , 
      'pi'  => 'Pali' , 
      'pl'  => 'Polish' , 
      'ps'  => 'Pashto ,  Pushto' , 
      'pt'  => 'Portuguese' , 
      'qu'  => 'Quechua' , 
      'rm'  => 'Romansh' , 
      'rn'  => 'Kirundi' , 
      'ro'  => 'Romanian ,  Moldavian ,  Moldovan' , 
      'ru'  => 'Russian' , 
      'rw'  => 'Kinyarwanda' , 
      'sa'  => 'Sanskrit (Sa?sk?ta)' , 
      'sc'  => 'Sardinian' , 
      'sd'  => 'Sindhi' , 
      'se'  => 'Northern Sami' , 
      'sg'  => 'Sango' , 
      'si'  => 'Sinhala ,  Sinhalese' , 
      'sk'  => 'Slovak' , 
      'sl'  => 'Slovene' , 
      'sm'  => 'Samoan' , 
      'sn'  => 'Shona' , 
      'so'  => 'Somali' , 
      'sq'  => 'Albanian' , 
      'sr'  => 'Serbian' , 
      'ss'  => 'Swati' , 
      'st'  => 'Southern Sotho' , 
      'su'  => 'Sundanese' , 
      'sv'  => 'Swedish' , 
      'sw'  => 'Swahili' , 
      'ta'  => 'Tamil' , 
      'te'  => 'Telugu' , 
      'tg'  => 'Tajik' , 
      'th'  => 'Thai' , 
      'ti'  => 'Tigrinya' , 
      'tk'  => 'Turkmen' , 
      'tl'  => 'Tagalog' , 
      'tn'  => 'Tswana' , 
      'to'  => 'Tonga (Tonga Islands)' , 
      'tr'  => 'Turkish' , 
      'ts'  => 'Tsonga' , 
      'tt'  => 'Tatar' , 
      'tw'  => 'Twi' , 
      'ty'  => 'Tahitian' , 
      'ug'  => 'Uighur ,  Uyghur' , 
      'uk'  => 'Ukrainian' , 
      'ur'  => 'Urdu' , 
      'uz'  => 'Uzbek' , 
      've'  => 'Venda' , 
      'vi'  => 'Vietnamese' , 
      'vo'  => 'Volapük' , 
      'wa'  => 'Walloon' , 
      'wo'  => 'Wolof' , 
      'xh'  => 'Xhosa' , 
      'yi'  => 'Yiddish' , 
      'yo'  => 'Yoruba' , 
      'za'  => 'Zhuang ,  Chuang' , 
      'zh'  => 'Chinese' , 
      'zu'  => 'Zulu' , 
    );
  }
  
  /**
   * Check to see if the user wants to use the addressfield module with the yelp_api
   *
   * @return boolean
   *  returns TRUE or FALSE
   */
  function yelp_api_use_addressfield() {
    return (variable_get('yelp_api_use_addressfield' , 0) && yelp_api_patched_addressfield());
  }
  
  /**
   * Checks to see if a patched version of the addressfield module is installed
   *  looking to see that addressfield module has been patched with the following patch
   *  @link http://drupal.org/node/970048#comment-5712492
   *
   * @return boolean
   *  returns boolean TRUE or FALSE
   */
  function yelp_api_patched_addressfield() {
    //first ,  is the addressfield module installed?
    if (module_exists('addressfield')) {
      //second is the  . module file patched?
      //will have the function 'addressfield_element_process'
      module_load_include('module' , 'addressfield');
      if (function_exists('addressfield_element_process')) {
        return TRUE;
      }
    }
    return FALSE;
  }
  
  
