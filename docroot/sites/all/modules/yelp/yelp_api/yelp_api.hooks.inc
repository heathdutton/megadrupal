<?php
  /**
   * @file yelp_api.hooks.inc
   *
   *  Invokes the use of custom hooks by other modules.
   */
   
  /**
   * Calls the hook: HOOK_yelp_api_alter_search_parameters.
   * 
   *  Allows other modules to alter the search query parameters before a query is made.
   *  Implements the HOOK_yelp_api_alter_search_parameters() as invoked by other modules.
   * 
   *  @see hook_yelp_api_alter_search_parameters()
   *
   */
  function yelp_api_hook_alter_search_parameters($search_parameters) {
    foreach (module_implements('yelp_api_alter_search_parameters') as $module) {
      $search_parameters = module_invoke($module , 'yelp_api_alter_search_parameters' , $search_parameters);
    }
    return $search_parameters;
  }
  
  /**
   * Calls the hook: HOOK_yelp_api_alter_search_results().
   *
   * Allows other modules to alter the search results afer being feteched from Yelp API (or cahced values)
   * Implements the HOOK_yelp_api_alter_search_results() as invoked by other modules.
   *  Hoook allows modules to alter the results returned from a yelp search
   *
   * @see hook_yelp_api_alter_search_results()
   *
   */
  function yelp_api_hook_alter_search_results($search_results , $search_parameters) {
    foreach (module_implements('yelp_api_alter_search_results') as $module) {
      $search_results = module_invoke($module , 'yelp_api_alter_search_results' , $search_results , $search_parameters);
    }
    return $search_results;
  }
  
  /**
   * Calls the hook: HOOK_yelp_api_load_yelp_data().
   *
   * Hook allows other modules to alter the yelp_data object after being fetched from the database but before being returned
   * to the user.
   *
   * @see hook_yelp_api_load_yelp_data()
   *
   */
  function yelp_api_hook_load_yelp_data($yelp_data , $yelp_id) {
    foreach (module_implements('yelp_api_load_yelp_data') as $module) {
      $yelp_data = module_invoke($module , 'yelp_api_load_yelp_data' , $yelp_data , $yelp_id);
    }
    return $yelp_data;
  }
  
  /**
   * Calls the HOOK_yelp_api_pre_update_yelp_data()
   * 
   * Hook allows other modules to alter the yelp search data right before being updated in the database
   *
   * @see hook_yelp_api_pre_update_yelp_data()
   *
   */
  function yelp_api_hook_pre_update_yelp_data($yelp_data , $yelp_id) {
    foreach (module_implements('yelp_api_update_yelp_data') as $module) {
      $yelp_data = module_invoke($module , 'yelp_api_update_yelp_data' , $yelp_data , $yelp_id);
    }
    return $yelp_data;
  }
  
  /**
   * Calls the HOOK_yelp_api_pre_save_yelp_data()
   *
   * Hook allows other modules to alter new yelp search data right before being saved in the database.
   *
   * @see hook_yelp_api_pre_save_yelp_data()
   *
   */
  function yelp_api_hook_pre_save_yelp_data($yelp_data) {
    foreach (module_implements('yelp_api_pre_save_yelp_data') as $module) {
      $yelp_data = module_invoke($module , 'yelp_api_pre_save_yelp_data' , $yelp_data);
    }
    return $yelp_data;
  }
  
  /**
   * Calls the HOOK_yelp_api_post_save_yelp_data()
   *
   * Hook allows other modules to perform an action after yelp data has been saved/updated to the database.
   *
   * @see hook_yelp_api_post_save_yelp_data()
   *
   */
  function yelp_api_hook_post_save_yelp_data($yelp_data , $yelp_id) {
    module_invoke_all('yelp_api_post_save_yelp_data' , $yelp_data , $yelp_id);
  }
  
  /**
   * Calls the Hook_yelp_api_pre_delete_yelp_data()
   *
   * Hook allows other modules to perform actions right before yelp data is deleted from the database.
   *
   * @see hook_yelp_api_pre_dete_yelp_data()
   *
   */
  function yelp_api_hook_pre_delete_yelp_data($yelp_id) {
    module_invoke_all('yelp_api_pre_delete_yelp_data' , $yelp_id);
  }
    
  /**
   * Calls the Hook_yelp_api_post_delete_yelp_data()
   *
   * Hook allows other modules to perform actions right after yelp data is deleted from the database.
   *
   * @see hook_yelp_api_post_dete_yelp_data()
   *
   */
  function yelp_api_hook_post_delete_yelp_data($yelp_id) {
    module_invoke_all('yelp_api_post_delete_yelp_data' , $yelp_id);
  }