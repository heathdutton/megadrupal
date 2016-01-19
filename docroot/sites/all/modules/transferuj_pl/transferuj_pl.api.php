<?php

/**
 * @file
 * Hook documentation.
 */

/**
 * Alters the data that is sent to transferuj.pl when redirecting payers.
 *
 * @param array $data
 *   An array of variables that will be sent to gateway.
 * @param Payment $payment
 *   Pending payment that we are sending data about.
 */
function hook_transferuj_pl_redirect_data_alter(array &$data, Payment $payment) {
}

/**
 * Responds to transferuj.pl payment feedback.
 *
 * @param array $data
 *   An array of variables that came from gateway.
 * @param Payment $payment
 *   Payment the feedback is related to.
 */
function hook_transferuj_pl_feedback(array $data, Payment $payment) {
}
