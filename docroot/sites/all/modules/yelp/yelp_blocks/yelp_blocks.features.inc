<?php
  /**
   * @file yelp_blocks.features.inc
   *  function relating to making yelp blocks exportable by features
   */
   
  /**
   * Returns a associative array of blocks that can be exported
   *
   * @return array
   *  an associative array of yelp blocks for export
   */
  function yelp_blocks_features_export_options() {
    $exportables = array();
    module_load_include('inc', 'yelp_blocks', 'yelp_blocks.pages');
    if ($yelp_blocks = yelp_blocks_get_blocks()) {
      foreach ($yelp_blocks as $machine_name  => $block) {
        $exportables[$machine_name] = $block->admin_title;
      }
    }
    return $exportables;
  }
  
  /**
  * Provides export data for selected yelp blocks for features
  *
  * @param array $data
  *   this is the machine name for the component in question
  * @param array &$export
  *   array of all components to be exported
  * @param string $module_name
  *   The name of the feature module to be generated . 
  * @return array
  *   The pipe array of further processors that should be called
  */
  function yelp_blocks_features_export($data, &$export, $module_name = '') { 
    // yelp_blocks_default_block integration is provided by Features . 
    $export['dependencies']['features'] = 'features';
    $export['dependencies']['yelp_blocks'] = 'yelp_blocks';
    $export['dependencies']['yelp_api'] = 'yelp_api';
    module_load_include('inc', 'yelp_blocks', 'yelp_blocks.pages');
    foreach ($data as $machine_name) {
      if ($yelp_block = yelp_blocks_get_blocks($machine_name)) {
        $export['features']['yelp_blocks'][$machine_name] = $machine_name;
      }
    }
    
    return $export;
  }
  
  /**
   * Render one or more component objects to code . 
   *
   * @param string $module_name
   *   The name of the feature module to be exported . 
   * @param array $data
   *   An array of machine name identifiers for the objects to be rendered . 
   * @param array $export
   *   The full export array of the current feature being exported .  This is only
   *   passed when hook_features_export_render() is invoked for an actual feature
   *   update or recreate, not during state checks or other operations . 
   * @return array
   *   An associative array of rendered PHP code where the key is the name of the
   *   hook that should wrap the PHP code .  The hook should not include the name
   *   of the module, e . g .  the key for `hook_example` should simply be `example` . 
   */
  function yelp_blocks_features_export_render($module, $data) {
    $code = array();
    module_load_include('inc', 'yelp_blocks', 'yelp_blocks.pages');
    foreach ($data as $machine_name) {
      if ($yelp_block = yelp_blocks_get_blocks($machine_name)) {
        unset($yelp_block->yelp_block_id);
        unset($yelp_block->yelp_id);
        unset($yelp_block->yelp_settings->yelp_id);
        $code[$machine_name] = $yelp_block;
      }
    }
    $code = "  return " . features_var_export($code, '  ') . ";";
    return array('yelp_blocks_features_default_block'  => $code);
  }
  
  /**
   * Implements hook_features_revert(). 
   */
  function yelp_blocks_features_revert($module) {
    yelp_blocks_features_rebuild($module);
  }
   
  /**
   * Implements hook_features_rebuild() . 
   *
   * Rebuilds @yelp_block from code defaults . 
   */
  function yelp_blocks_features_rebuild($module) {
    $yelp_blocks = module_invoke($module, 'yelp_blocks_features_default_block');
    module_load_include('inc', 'yelp_blocks', 'yelp_blocks.pages');
    module_load_include('inc', 'yelp_api', 'yelp_api.pages');

    foreach ($yelp_blocks as $machine_name  => $yelp_block) {
      
      //convert yelp_block into a keyed array for updating
      $data = array(
        'admin_title'       => $yelp_block['admin_title'], 
        'title'             => $yelp_block['title'], 
        'cache_settings'   => $yelp_block['cache'], 
      );
      //yelp_api data
      $api_data = array(
        //add yelp_api data
        'radius'           => $yelp_block['yelp_settings']['radius'], 
        'max_results'       => $yelp_block['yelp_settings']['max_results'], 
        'lang'             => $yelp_block['yelp_settings']['lang'], 
        'categories'       => serialize($yelp_block['yelp_settings']['categories']), 
        'cc'               => $yelp_block['yelp_settings']['cc'], 
        'address'           => $yelp_block['yelp_settings']['location'], 
      );
      //updating a block?
      if ($old_block = yelp_blocks_get_blocks($machine_name)) {
        $api_data['yelp-id']  = $old_block->yelp_id;
        $data = array_merge($data, $api_data);
        unset($data['categories']);
        foreach ($yelp_block['yelp_settings']['categories'] as $category) {
          $data['yelp-category-' . $category] = $category;
        }
        yelp_blocks_update_block($old_block, $data);
      }
      else{
        //save yelp api data
        $data['yelp_id'] = yelp_api_save_data($api_data);
        //save new block to system
        yelp_blocks_save_yelp_block($data);
      }
    }
  }