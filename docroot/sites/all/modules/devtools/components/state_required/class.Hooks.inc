<?php
namespace devtools\components\state_required;

/**
 * Temporary workaround until https://drupal.org/node/1592688 is fixed in core.
 */
class Hooks extends \devtools\Hooks {

  /*----------------------------------------------------------------------------
      DRUPAL HOOKS
  ----------------------------------------------------------------------------*/

  /**
   * Extends hook_js_alter().
   */
  function hook_js_alter(&$javascript) {
    $file_path = $this->_components_path . '/state_required/js/state_required.js';

    if (!isset($javascript[$file_path])) {
      $add_js = FALSE;

      foreach ($javascript as $key => $data) {
        if (preg_match('/states\.js$/', $key)) {
          $add_js = TRUE;
          break;
        }
      }

      if ($add_js) {
        $javascript[$file_path] = array(
          'group' => '',
          'weight' => 10,
          'data' => $file_path,
          'type' => 'file',
          'every_page' => FALSE,
          'scope' => 'footer',
          'cache' => TRUE,
          'defer' => FALSE,
          'preprocess' => TRUE,
        );
      }
    }
  }

  /*----------------------------------------------------------------------------
      MODULE HOOKS
  ----------------------------------------------------------------------------*/

}



