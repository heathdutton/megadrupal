<?php
/**
 * @file
 * Hooks provided by the Party Hats module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Act on a Party when some Hats are assigned.
 *
 * @param $party
 *   The party the hats have been assigned to
 * @param $hats
 *   The hats that have been assigned to the party. An array of hat objects
 *   keyed by hat name
 *
 * @see party_hat_hats_assign()
 */
function hook_party_hat_assign_hats($party, $hats) {
}

/**
 * Act on a Party when some Hats are unassigned.
 *
 * @param $party
 *   The party the hats have been unassigned from
 * @param $hats
 *   The hats that have been unassigned from the party. An array of hat objects
 *   keyed by hat name
 *
 * @see party_hat_hats_unassign()
 */
function hook_party_hat_unassign_hats($party, $hats) {
}

/**
 * @}
 */
