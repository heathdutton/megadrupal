<?php

/**
 * @file
 * Hooks provided by the Userbar module.
 */

/**
 * @defgroup userbar_hooks
 * @{
 * The Userbar API allows modules to define content for which different the content
 * status can be shown to the user and provide the changed content to the user.
 * @}
 */
/**
 * @addtogroup hooks
 * @{
 */

/**
 * Display the status provided by this module for its content
 * 
 *  @return
 *   array indexed on module name which is indexed on content identifiers. Allowed values
 *   description - Description of the content identifier
 *   display - Sample information shown to user in admininstration or user personalization form
 *   type - image or html. Modules can provide html that has the show the status of the content
 *   
 *   @ingroup userbar_hooks
 *   @see hook_userbar_view()
 */
function hook_userbar_info() {
  return array('mymodule' => array(
    'mymodule_new_nodes' => array('description' => t('Shows if there are new nodes since last visit'), 
      'display' => drupal_get_path('module', 'mymodule') . '/images/new_nodes_icon.png', 'type' => 'image'),
    'mymodule_updated_nodes' => array('description' => t('Shows number of updated nodes since last visit'),
      'display' => "<some html text>", 'type' => 'html')));
}

/**
 * Display the status of the content selected 
 * @param $indexes
 *   An assocative array of content id as specified in hook_userbar_info chosen by the current logged on user
 *   
 * @return
 *   An associate array of results of content status indexed on module name
 *   	display - Icon or HTML content to show
 *   	type - image or html
 *   	tip - Tool tip to show
 *   	callback - Path to navigate on user click
 *     
 * @ingroup userbar_hooks
 * @see hook_userbar_info()
 */
function hook_userbar_view($indexes) {
  $output = array();
  foreach ($indexes as $content_id) {
    switch ($content_id) {
      case 'mymodule_new_nodes':
        // perform necessary check
        if ($has_new_node) {
          $output[$content_id] = array('display' => drupal_get_path('module', 'mymodule') . '/images/new_nodes_icon.png', 'type' => 'image', 'tip' => t('There are %num new nodes', array('%num' => $num)), 'callback' => 'mymodule/show');
        }
        else {
          $output[$content_id] = array('display' => drupal_get_path('module', 'mymodule') . '/images/no_new_nodes_icon.png', 'type' => 'image', 'tip' => t('There are no new nodes since last visit.'));
        }
        break;

      case 'mymodule_updated_nodes':
        //check if there are updated nodes
        $output[$content_id] = array('display' => '<some html text>', 'type' => 'html');
        break;

    }
  }
  return array('mymodule' => $output);
}


/**
 * @} End of "addtogroup hooks".
 */
