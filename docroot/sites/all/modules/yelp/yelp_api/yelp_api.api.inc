<?php
  /**
   * @file yelp_api.api.inc
   *  defines yelp_api hooks that can be used by other modules to perform yelp searches
   */
  
  /**
   * Alters the search parameters used in a Yelp API search query.
   *
   * This hook is invoked from _yelp_api_format_search_parameters() when formating a Yelp API search query,
   * but before being formatted by the function _yelp_api_format_search_query().
   * 
   * @param array $search_parameters
   *    An associative array of search parameters used to make a yelp search API call
   *    By default the $search_parameters array contains the following:
   *      - location: string containing location to use as center for search.
   *      - cc: the country code to search results in (valid values are US, CA, GB). default to 'US'.
   *      - lang: The language code to search results in. default to 'en'.
   *      - lang_filter: boolean value to limit search results to only the specified lang. default to 0.
   *      - sort: numeric value of sort method. default to 1.
   *      - max_results: int value of how many results to return. max is 20, default is 4.
   *      - radius: numeric value of size of radius to search (in km). max is 40233.6 (about 25miles)
   *      - offset: int value of how many results to offset by. default is 0.
   *      - categories: a comma seperated list of Yelp categories to filter by.
   *      - deals: (optional) boolean value to include deals information with results. default is 0.
   *      - term: (optional) string to filter results by.
   *
   * @return array
   *    the modified array of parameters. Should still be an associative array
   *
   * @ingroup yelp_api_hooks
   * @ingroup hooks
   */ 
  function hook_yelp_api_alter_search_parameters($search_parameters) {
    // filter for only results that contain the term 'tacos'.
    $search_parameters['term']   = 'tacos';
    // sort by most popular
    $search_parameters['sort']  = 2;
    // return 15 results
    $search_parameters['max_results'] = 15;
    //return modified search parameters
    return $search_parameters;
  }
  
  /**
   * Alters the results returned by a Yelp API call.
   *
   * This hook is invoked from yelp_api_perform_search() after results have been retrieved from a search.
   * 
   * @param array $search_results
   *  A keyed array of objects as returned by Yelp.
   *  Each object contains at least the following attributes:
   *    - id:(string) result Yelp ID
   *    - name: (string) Name of result
   *    - url: (string) Url to Yelp result
   *    - phone: (string) phone number of result
   *    - display_phone: (string) display version of the result phone number
   *    - image_url: (string) url to image for result
   *    - location: (object) location information about this result
   *      - cross_streets (string)
   *      - city (string)
   *      - address (string)
   *      - display_address (array)
   *      - geo_accuracy(int)
   *      - neighborhoods (array)
   *      - postal_code (string)
   *      - country_code (string)
   *      - coordinate (object)
   *        - latitude (float)
   *        - longitude (float)
   *      - state_code (string)
   *    - mobile_url: (string) mobile url to Yelp result
   *    - rating: (int) Yelp rating of the result
   *    - rating_img_url: (string) url of rating (stars) image
   *    - rating_img_url_small: (string) url to small version of rating (stars) image
   *    - rating_image_url_large: (string) url to large version of rating image
   *    - review_count: (int) number of reviews result has on Yelp
   *    - snippet_text: (string) snippet of text from latest review
   *    - snippet_image_url: (string) url to user photo for snippet
   *    - categories: (array) associative array of yelp categories this result is in (display name => category_id)
   *
   * @param array $search_parameters
   *  The array of search parameters that were used to retrieve the search results.
   *
   * @return array
   *  Returns a modified array of search reult objects.
   *
   * @see http://www.yelp.com/developers/documentation/v2/search_api
   * @see hook_yelp_api_alter_search_parameters()
   *
   * @ingroup yelp_api_hooks
   * @ingroup hooks
   *
   */
  function hook_yelp_api_alter_search_results($search_results, $search_parameters) {
    // if filtering for only tacos results
    if ( isset($search_parameters['term']) && ($search_parameters['term'] == 'tacos') ) {
      //filter only results that have at least 4 stars
      $filtered_results = array();
      foreach ($search_results as $result) {
        if ($result->rating >= 4) {
          $filtered_results[] = $result;
        }
      }
      $search_results = $filtered_results;
      unset($filtered_results);
    }
    return $search_results;
  }
  
  /** 
   * Alter Yelp search data after being loaded from the database
   *
   * This hook is invoked from yelp_api_get_data() when search data (not results) are being loaded from the database.
   * Modules may add additional attributes to the object, or modify the existing attributes.
   * 
   * @param object $yelp_data
   *  a yelp_api object which contains search parameters
   *  The object has the following attributes:
   *  - yelp_id: int id of the yelp_data
   *  - location: string containing the unformatted location data (or a token value)
   *  - radius: numeric value of radius in miles
   *  - max_results: int value of number of search results to return
   *  - cc: two character country code (or a token value)
   *  - lang: two character language code (or a token value)
   *  - categories: array of Yelp categories
   *
   * @param int $yelp_id
   *  the yelp_api id of the object
   *
   * @return object
   *    return the modified yelp_data object
   *
   * @ingroup yelp_api_hooks
   * @ingroup hooks
   */
  function hook_yelp_api_load_yelp_data($yelp_data, $yelp_id) {
    //if language is set to 'en', set country to US
    if ($yelp_data->lang == 'en') {
      $yelp_data->cc = 'US';
    }
    //if object #1, set radius to 15miles
    if ($yelp_id == 1) {
      $yelp_data->radius = 15;
    }
    return $yelp_data;
  }
  
  
  /**
   * Alters yelp search data before being updated in the Database.
   *
   * This hook is invoked by the yelp_api_save_data() function right before updating an existing yelp_data object
   * Modules may alter the data before it is saved to the database. Modules may add aditional values to the array, 
   * which will be ignored by the update.
   *
   * @param array $yelp_data
   *  An associative array of data ready to be updated in the database.
   *  the array keys are the fields in the yelp_data database table:
   *  - location: string containing the unformatted location data (or a token value)
   *  - radius: numeric value of radius in miles
   *  - max_results: int value of number of search results to return
   *  - cc: two character country code (or a token value)
   *  - lang: two character language code (or a token value)
   *  - categories: serialized array of Yelp categories
   *
   * @param int $yelp_id
   *  The numeric Id of the yelp_data object about to be updated
   *
   * @return array
   *  a modified associative array of yelp_data fields
   *
   * @ingroup yelp_api_hooks
   * @ingroup hooks
   */
  function hook_yelp_api_pre_update_yelp_data($yelp_data, $yelp_id) {
    //if lang is 'en' set country to US
    if ($yelp_data['lang'] == 'en') {
      $yelp_data['cc'] = 'US';
    }
    
    //if yelp_id == 1, set radius to 15
    if ($yelp_id == 1) {
      $yelp_data['radius'] = 15;
    }
    
    $yelp_data['myfield'] = 'X';
  
    return $yelp_data;
  }
  
  
  /**
   * Alters new yelp search data right before being saved to the database
   *
   * This hook is invoked by the yelp_api_save_data() function right before saving new search data tot the yelp_data database table.
   * Modules may alter the data before it is saved to the database. Modules may add aditional values to the array, 
   * which will be ignored by the save.
   *
   * @param array $yelp_data
   *  An associative array of data ready to be updated in the database.
   *  the array keys are the fields in the yelp_data database table:
   *  - location: string containing the unformatted location data (or a token value)
   *  - radius: numeric value of radius in miles
   *  - max_results: int value of number of search results to return
   *  - cc: two character country code (or a token value)
   *  - lang: two character language code (or a token value)
   *  - categories: serialized array of Yelp categories
   *
   * @return array
   *  a modified associative array of yelp_data fields
   *
   * @ingroup yelp_api_hooks
   * @ingroup hooks
   */
  function hook_yelp_api_pre_save_yelp_data($yelp_data) {
    // make sure categories has the category 'bars'
    $categories = unserialize($yelp_data['categories']);
    if (!in_array('bars', $categories)) {
      $categories[] = 'bars';
    }
    $yelp_data['categories'] = serialize($categories);
    
    //set mat rexusts to the max value (20)
    $yelp_data['max_results'] = 20;
    
    $yelp_data['myfield'] = 'Y';
    
    return $yelp_data;
  }
  
  /**
   * Perform action right after yelp search data has been saved to the database
   *
   * This hook is invoked by the yelp_api_save_data() function right after yelp search data has been either saved or updated in the database.
   * Allows modules to perform actions right after data is saved to the database.
   *
   * @param array $yelp_data
   *  An associative array of data that was just saved/updated in the database.
   *  the array keys are the fields in the yelp_data database table:
   *  - location: string containing the unformatted location data (or a token value)
   *  - radius: numeric value of radius in miles
   *  - max_results: int value of number of search results to return
   *  - cc: two character country code (or a token value)
   *  - lang: two character language code (or a token value)
   *  - categories: serialized array of Yelp categories
   *
   * @param int $yelp_id
   *  The numeric Id of the yelp_data object that was just saved/updated.
   *
   * @return nothing
   *  this hook does not return anything
   *
   * @ingroup yelp_api_hooks
   * @ingroup hooks
   */
  function hook_yelp_api_post_save_yelp_data($yelp_data, $yelp_id) {
    // yelp data was just saved to the database,
    // save data to our custom table
    if ($yelp_data['myfield'] == 'X') {
      db_update('mytable')
        ->fields(array('radius' => $yelp_data['radius'], 'myfield' => 'X', 'max_results' => $yelp_data['max_results']))
        ->condition('yelp_id', $yelp_id)
        ->execute();
    }
    else{
      db_insert('mytable')
        ->fields(array('radius' => $yelp_data['radius'], 'yelp_id' => $yelp_id, 'max_results' => $yelp_data['max_results']))
        ->execute();
    }
  }
  
  /**
   * Perform an action right before a yelp search object is deleted from the database
   *
   * This hook is invoked from the yelp_api_delete_data() function reight before deleting a yelp search object from the yelp_data database table.
   *
   * @param int $yelp_id
   *  the yelp_api id of the data about to be deleted
   *
   * @return nothing
   *  this function returns nothing
   *
   * @ingroup yelp_api_hooks
   * @ingroup hooks
   *
   */
  function hook_yelp_api_pre_delete_yelp_data($yelp_id) {
    //remove data from my table for this object.
    db_delete('mytable')->condition('yelp_id' => $yelp_id)->execute();
  }
  
  /**
   * Perform an action right after a yelp search object is deleted from the database
   *
   * This hook is invoked from the yelp_api_delete_data() function right after deleting a yelp search object from the yelp_data database table.
   *
   * @param int $yelp_id
   *  the yelp_api id of the data about to be deleted
   *
   * @return nothing
   *  this function returns nothing
     *
   * @ingroup yelp_api_hooks
   * @ingroup hooks
   *
   */
  function hook_yelp_api_pre_delete_yelp_data($yelp_id) {
    //remove data from my table for this object.
    mymodule_reidex_mydata();
  }