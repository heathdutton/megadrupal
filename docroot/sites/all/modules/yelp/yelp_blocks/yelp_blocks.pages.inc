<?php
  /**
   * @file yelp_blocks.pages.inc
   * Additional functions used by the yelp_blocks module
   */
   
  /**
   * Boolean to check if machine_name of yelp block already exists
   *
   * @param string $machine_name
   *   the name to check
   *
   * @return boolean
   *  returns a boolean TRUE or FALSE
   */
  function _yelp_blocks_check_machine_name_exists($machine_name) {
    $test = 'SELECT machine_name FROM {yelp_blocks} WHERE machine_name = :machine_name';
    $test  = db_query_range($test, 0, 1, array(':machine_name' => $machine_name))->fetchField();
    return ($test == $machine_name);
  }  
   
  /**
   * Retrieves single or multiple yelp blocks
   *
   * @param string $machine_name
   *   a machine name of a specific yelp block. Optional
   *
   * @return object
   *  returns either a single yelp block object, or an array of multiple yelp block objects
   */
  function yelp_blocks_get_blocks($machine_name=FALSE) {
    module_load_include('inc', 'yelp_api', 'yelp_api.pages');
    $yelp_blocks = array();
    $yelp_blocks_query = db_select('yelp_blocks', 'yb')->fields('yb');
    if ($machine_name) {
      $yelp_blocks_query->condition('yb . machine_name', $machine_name);
    }
    $results = $yelp_blocks_query->execute();
    foreach ($results as $result) {
      $result->yelp_settings = yelp_api_get_data($result->yelp_id);
      $yelp_blocks[$result->machine_name] = $result;
    }
    
    if ($machine_name) {
      return isset($yelp_blocks[$machine_name]) ? $yelp_blocks[$machine_name]:FALSE;
    }
    return $yelp_blocks;
  }
  
  /**
   * Renders the HTML content of a block
   *
   * @param object $yelp_block (object) 
   *  a yelp block object
   *
   * @return string
   *  Returns themed HTML output for a yelp block.
   */
  function yelp_blocks_render_block($yelp_block) {
    if ($results = yelp_blocks_get_results($yelp_block)) {
      $businesses  = array();
    
      foreach ($results->businesses as $business) {
        $businesses[] = theme('yelp_block_business__' . $yelp_block->machine_name, array('business' => $business, 'yelp_block' => $yelp_block));
      }
    }
    else{
      $businesses = FALSE;
      $results    = (object)array('total' => 0);
    }
    return theme('yelp_block_search_results__' . $yelp_block->machine_name, array('yelp_block' => $yelp_block, 'businesses' => $businesses, 'total' => $results->total, 'results' => $results));
  }
  
  /** 
   * Form constructor for the yelp block configure form.
   *
   * @params object $yelp_block
   *   the yelp block object
   *
   * @see yelp_blocks_configure_form_validate()
   * @ingroup forms
   */
  function yelp_blocks_block_configure_form($form, $yelp_block) {
    $form_state = array();
    // build the form fields for the yelp block
    $form = _yelp_blocks_form_fields($form, $form_state, $yelp_block);
    //disable the machine name field
    $form['machine_name']['#disabled']        = TRUE;
    unset($form['machine_name']['#machine_name']);
    $form['machine_name']['#type'] = 'textfield';
    //add default values to fields
    $form['yelp-blocks']['cache_settings']['#default_value']  = $yelp_block->cache;
    $form['settings']['yelp-blocks'] = $form['yelp-blocks'];
    unset($form['yelp-blocks']);
    $form['settings']['yelp-settings'] = $form['yelp-settings'];
    unset($form['yelp-settings']);
    return $form;
  }
  
  /**
   * Form validation handler for yelp_blocks_block_configure_form()
   */
  function yelp_blocks_configure_form_validate($form, &$form_state) {
    //_yelp_blocks_form_fields_validate($form, $form_state);
  }
  
  /**
   * Updates the yelp block settings
   *
   * @param object $yelp_block
   *  The yelp block object
   * @params array $edit 
   *  The updated values
   */
  function yelp_blocks_update_block($yelp_block, $edit) {
    module_load_include('inc', 'yelp_api', 'yelp_api.form');
    $block_update = array(
      'admin_title'   => $edit['admin_title'], 
      'title'         => $edit['title'], 
      'cache'         => $edit['cache_settings'], 
      'yelp_id'       => yelp_api_form_fields_submit(array('values' => $edit)), 
    );
    
    db_update('yelp_blocks')->fields($block_update)->condition('machine_name', $yelp_block->machine_name)->execute();
  }
  
  /**
   * Retrieves search results for a yelp_block
   *
   * @param object $yelp_block
   *  a yelp block object
   *
   * @return array
   *  an array of retrieved search results or boolean FALSE
   *
   */
  function yelp_blocks_get_results($yelp_block) {
    module_load_include('inc', 'yelp_api', 'yelp_api.pages');
    if ($results = yelp_api_get_results($yelp_block->yelp_id)) {
      return $results;
    }
    return FALSE;
  }
  
  /**  
   * Saves a new yelp block
   *
   * @param array $yelp_block
   *  A yelp block array
   *
   * @return boolean
   *  returns a boolean TRUE or FALSE
   */
  function yelp_blocks_save_yelp_block($yelp_block) {
    // make sure machine_name does not exist
    if (_yelp_blocks_check_machine_name_exists($yelp_block['machine_name'])) {
      //block already exists? return FALSE
      return FALSE;
    }
    else{
      // save block to db with insert
      $ybid = db_insert('yelp_blocks')->fields($yelp_block)->execute();
      if ($ybid>0) {
        //cache_clear_all(NULL, 'cache_block');
        return TRUE;
      }
    }
    return FALSE;
  }
  
  /**
   * Deletes a yelp block and all data from the database
   *
   * @param object $yelp_block
   *  the yelp_block to delete
   *
   * @return boolean
   *  returns a boolean TRUE or FALSE
   */
  function yelp_blocks_delete_yelp_block($yelp_block) {
    //delete yelp API entry
    if ($yelp_block->yelp_id) {
      module_load_include('inc', 'yelp_api', 'yelp_api.pages');
      yelp_api_delete_data($yelp_block->yelp_id);
    }
    db_delete('yelp_blocks')->condition('yelp_block_id', $yelp_block->yelp_block_id)->execute();
    if (_yelp_blocks_check_machine_name_exists($yelp_block->machine_name)) {
      return FALSE;
    }
    else{
      cache_clear_all(NULL, 'cache_block');
    }
    return TRUE;
  }
  
  /**
   * Form constructor for Yelp Blocks fields
   *
   * @param object $yelp_block
   *  an optional yelp lock to modify
   *
   * @see yelp_blocks_form_fields_validate()
   * @ingroup forms
   */
  function _yelp_blocks_form_fields($form, &$form_state, $yelp_block=FALSE) {
    // block admin title field
    $form['admin_title'] = array(
      '#type'           => 'textfield', 
      '#title'         => t('Admin Title'), 
      '#description'   => t('Title for this block for the Administration section'), 
      '#required'       => TRUE, 
      '#default_value' => '', 
      '#weight'         => -2, 
    );
    
    // machine name field (generate)?
    $form['machine_name'] = array(
      '#type'           => 'machine_name', 
      '#title'         => t('Machine Name'), 
      '#description'   => t('machine-friendly name'), 
      '#size'           => 25, 
      '#maxlength'     => 25, 
      '#default_value' => '', 
      '#machine_name'   => array(
        'exists'   => '_yelp_blocks_check_machine_name_exists', 
        'source'   => array('admin_title'), 
        'error'     => t('field contains invalid value'), 
      ), 
      '#required'       => TRUE, 
      '#weight'         => -1, 
    );
    
    $form['yelp-blocks'] = array(
      '#type'    => 'fieldset', 
      '#title'   => 'Yelp Block Settings', 
      '#weight'   => 0, 
    );
    // block title field
    $form['yelp-blocks']['title']    = array(
      '#type'           => 'textfield', 
      '#title'         => t('Block title'), 
      '#description'   => t('The default title for the block. Use \'&#60;none&#62;\' to display no title, or leave blank to use the default block title.'), 
      '#default_value' => '', 
    );
    if (module_exists('token')) {
      $form['yelp-blocks']['title']['#description'] .=t('This field supports tokens . ');
    }
    
    // cache settings field
    $form['yelp-blocks']['cache_settings'] = array(
      '#type'     => 'select', 
      '#title'   => t('Block cache setting'), 
      '#options' => array(
        'DRUPAL_CACHE_PER_ROLE'  => t('Cache per user role'), 
        'DRUPAL_CACHE_PER_USER'   => t('Cache per user'), 
        'DRUPAL_CACHE_PER_PAGE'   => t('Cache per page'), 
        'DRUPAL_CACHE_GLOBAL'     => t('Cache globally'), 
        'DRUAPL_NO_CACHE'         => t('Do not cache'), 
      ), 
      '#default_value'           => 'DRUPAL_CACHE_PER_ROLE', 
      '#required'                 => TRUE, 
    );
              
    if (is_object($yelp_block)) {
      $form['admin_title']['#value']                     = $yelp_block->admin_title;
      $form['machine_name']['#value']                   = $yelp_block->machine_name;
      $form['yelp-blocks']['title']['#value']            = strlen(trim($yelp_block->title))>0 ? $yelp_block->title:'<none>';
      $form['yelp-blocks']['cache_settings']['#value']  = $yelp_block->cache;
    }
    
    $form['#validate'][] = 'yelp_blocks_form_fields_validate';
    module_load_include('inc', 'yelp_api', 'yelp_api.form');
    $form = yelp_api_form_fields($form, $form_state, $yelp_block);
    return $form;
  }
  
  /**
   * Form validation handler for _yelp_blocks_form_fields()
   */
  function yelp_blocks_form_fields_validate($form, &$form_state) {
  
  }
   