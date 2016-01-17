<?php

/**
 * @file
 * Hooks provided by the Party User module.
 */

/**
 * Act when a user acquires a party.
 *
 * @param Party $party
 *  The Party that has been acquired by the user on registration.
 * @param $user
 *  The user entity of the account that has acquired the party during
 *  registration.
 * @param $method
 *  The acquisition method that was used, can be either 'create' or 'acquire':
 *  - 'create' means a new party has been created for this user.
 *  - 'acquire' means an existing party has been found.
 *
 * @see party_user_user_insert()
 */
function hook_party_user_acquisition($party, $user, $method) {

}
