<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */


/**
 * @defgroup skaflash SkaFlash module integrations.
 *
 * Module integrations with the SkaFlash module.
 */

/**
 * @defgroup better_statistics_hooks SkaFlash's hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend the
 * SkaFlash module.
 */

/**
 * React when a SkaFlash push notification is received.
 *
 * @param $data
 *   An associative array of data sent to the specified endpoint by the SkaFlash
 *   API. Array keys passed include:
 *   - email: The fan's e-mail address.
 *   - phone: The fan's telephone number.
 *   - timestamp: The date/time at which SkaFlash received the address.
 */
function hook_skaflash_pushed($data) {
  // Send an e-mail to the e-mail address provided.
  mail($data['email'], 'Thanks!', 'Thanks for becoming a fan!');
}

/**
 * @}
 */
