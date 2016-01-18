<?php

/**
 * @file
 * Hooks provided by the WoW API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide a data service via the WoW Data Service API.
 *
 * @return
 *  An array whose keys are module name and data resource key and whose values
 *  identify properties of those data resources that the system needs to know
 *  about:
 * - base table: (optional) The name of the base table. If not provided default
 * to {module}_{data key}.
 * - remote path: The remote path of the data resource.
 * - fetch class: The class to fetch data in.
 * - refresh threshold: The default refresh threshold in seconds.
 * - import callback: (optional) The name of the import callback to call. This
 * callback is called for each items returned by the service. If not provided
 * default to {module}_{data key}_import().
 */
function hook_wow_data_resources() {
  return array(
    // wow_character is the 'base module'.
    'wow_character' => array(
      // races is the 'data key' to look for.
      'races' => array(
        'remote path' => 'character/races',
        'fetch class' => 'CharacterRace',
        'refresh threshold' => 2592000, // 30 days.
      ),
    ),
  );
}

/**
 * @} End of "addtogroup hooks".
 */
