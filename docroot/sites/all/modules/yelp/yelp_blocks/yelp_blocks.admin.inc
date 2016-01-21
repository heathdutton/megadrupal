<?php
  /**
   * @file yelp_blocks.admin.inc
   * Sdmin functions for the yelp_blocks module
   */
  
  /**
   * Provides a list of the available yelp blocks.
   * 
   * @return string
   *  Returns a themed table or yelp blocks.
   *
   */
   function yelp_blocks_admin_blocks() {
    $rows  = array();
    module_load_include('inc', 'yelp_blocks', 'yelp_blocks.pages');
    if ($yelp_blocks = yelp_blocks_get_blocks()) {
      // list blocks in a table form
      foreach ($yelp_blocks as $ybid  => $yelp_block) {
        $row  = array(
        // Title | 
          $yelp_block->admin_title, 
        // machine_name | 
          $yelp_block->machine_name, 
        // operations (edit(|delete))
          l(t('configure'), 'admin/structure/block/manage/yelp_blocks/' . $ybid . '/configure') . 
        // disable delete for 'default' block
          '|' . l(t('delete'), 'admin/structure/yelp-blocks/' . $ybid . '/delete'), 
        );
        $rows[] = $row;
      }
    }
    else{
      $rows[] = array(array('colspan' => 3, 'data' => 'No Yelp Blocks Found . ' . l(t('Create One'), 'admin/structure/yelp-blocks/add')));
    }
    $headers = array(t('Yelp Block'), t('machine name'), t('operations'));
    $output = theme('table', array('header' => $headers, 'rows' => $rows));
    return $output;
   }
   
  function yelp_blocks_admin_add_block_redirect() {
    drupal_goto('admin/structure/yelp-blocks/add');
  }
   
   /** 
    * Form constructor for creating a new Yelp block
    *
    * @see yelp_blocks_admin_add_block_form_validate()
    * @see yelp_blocks_admin_add_block_form_submit()
    * @ingroup forms
    */
  function yelp_blocks_admin_add_block_form($form, &$form_state) {
    module_load_include('inc', 'yelp_blocks', 'yelp_blocks.pages');
    $form = _yelp_blocks_form_fields($form, $form_state);
    //add custom validation function
    $form['#validate'][]  = 'yelp_blocks_admin_add_block_form_validate';
    $form['submit']  = array(
      '#type'     => 'submit', 
      '#value'   => 'Save', 
    );
    return $form;
  }
  
  /**
   * Form validation handler for yelp_blocks_admin_add_block_form()
   *
   * @see yelp_blocks_admin_add_block_form_submit()
   */
  function yelp_blocks_admin_add_block_form_validate($form, &$form_state) {
    // validate machine name field
    $machine_name = $form_state['values']['machine_name'];
    if (_yelp_blocks_check_machine_name_exists($machine_name)) {
      form_set_error('machine_name', t('Machine name must be unique.'));
    }
  }
  
  /**
   * form submission handler for yelp_blocks_admin_add_block_form()
   *
   * @see yelp_blocks_admin_add_block_form_validate()
   */
  function yelp_blocks_admin_add_block_form_submit($form, &$form_state) {
    module_load_include('inc', 'yelp_blocks', 'yelp_blocks.pages');
    module_load_include('inc', 'yelp_api', 'yelp_api.form');
    $yelp_block = array(
      'admin_title'   => $form_state['values']['admin_title'], 
      'machine_name' => $form_state['values']['machine_name'], 
      'title'         => $form_state['values']['title'], 
      'cache'         => $form_state['values']['cache_settings'], 
      'yelp_id'       => yelp_api_form_fields_submit($form_state), 
    );
    
    //attempt to save the block
    if (yelp_blocks_save_yelp_block($yelp_block)) {
      drupal_set_message(t('Yelp block created.'));
      //cache_clear_all(NULL, 'cache_block');
    }
    else{
      yelp_api_delete_data($yelp_block['yelp_id']);
      drupal_set_message(t('Error saving yelp block.'), 'error');
    }
  }
  
  /**
   * Confirms user wants to Delete a yelp block and all of its associated data.
   *
   * @param string $machine_name
   *  the machine name of the yelp block to delete.
   *
   * @return form
   *  Returns a confimration form about deleteing this data.
   */
  function yelp_blocks_admin_delete_block($machine_name) {
    module_load_include('inc', 'yelp_blocks', 'yelp_blocks.pages');
    if ($yelp_block = yelp_blocks_get_blocks($machine_name)) {
      return drupal_get_form('yelp_blocks_admin_confirm_delete_form', $yelp_block);
    }
    else{
      drupal_set_message(t('Invalid block machine name provided'), 'error');
      drupal_goto('admin/structure/yelp-blocks');
    }
  }
  
  /**
   * Form constructor for deleting a yelp block
   * 
   * @see yelp_blocks_admin_confirm_delete_form_submit()
   * @ingroup forms
   */
  function yelp_blocks_admin_confirm_delete_form($form, &$form_state, $yelp_block) {
    $form['#yelp_block'] = $yelp_block;
    
    return confirm_form($form, 
      t('Are you sure you want to delete the yelp block "' . $yelp_block->admin_title . '"?'), 
      'admin/structure/yelp-blocks', 
      t('This action cannot be undone')
    );
  }
  
  /**
   * Form submission handler for yelp_blocks_admin_confirm_delete_form()
   */
  function yelp_blocks_admin_confirm_delete_form_submit($form, &$form_state) {
    module_load_include('inc', 'yelp_blocks', 'yelp_blocks.pages');
    $yelp_block = $form['#yelp_block'];
    if (yelp_blocks_delete_yelp_block($yelp_block)) {
      drupal_set_message(t('Yelp block deleted.'));
    }
    else{
      drupal_set_message(t('Error deleting yelp block'));
    }
    drupal_goto('admin/structure/yelp-blocks');
  }