<?php

/**
 * @file
 * Hooks provided by the Unlink default configurations.
 */

/**
 * Provides list of custom exportable configurations.
 *
 * You can also provide your own list of the exportable configuration which you
 * want to unlink from default hooks to database.
 * 
 *
 * @return array
 *  An array of custom components.
 *
 * @see unlink_defaults_unlink_defaults_components();
 */
function hook_unlink_defaults_components() {

  return array(
    'your_component' => array(

      // List of configurations of component.
      'list' => array(
        'configuration1' => array(
          'title' => 'Configuration1',
          'module' => 'some_module',

          // You may use for this constants from ctools, like EXPORT_IN_CODE,
          // EXPORT_IN_DATABASE, or you may use constants from entity module,
          // like ENTITY_IN_CODE, ENTITY_IN_DB.
          'storage' => $status == ENTITY_IN_CODE ? t('In code') : t('In database'),
        ),
        'configurations2' => array(
          // See first example "configuration1"....
        ),
      ),

      // Module of component.
      'module' => 'my_module',
    ), 
  );
}
