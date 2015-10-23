<?php
namespace devtools\components\ajax;

class Hooks extends \devtools\Hooks {
  /*----------------------------------------------------------------------------
      DRUPAL HOOKS
  ----------------------------------------------------------------------------*/

  /**
   * Extends hook_library_alter().
   */
  function hook_library_alter(&$libraries, $module) {
    // add our JS file after core's  ajax.js
    if ($module == 'system' && isset($libraries['drupal.ajax'])) {
      $libraries['drupal.ajax']['js'][$this->_path . '/js/ajax.js'] = array(
        'weight' => $libraries['drupal.ajax']['js']['misc/ajax.js']['weight'] + 1,
      );
    }
  }

  /*----------------------------------------------------------------------------
      MODULE HOOKS
  ----------------------------------------------------------------------------*/

}