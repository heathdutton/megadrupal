<?php

/**
 * This hook is invoked after an remote user from the SSO server has
 * been locally copied.
 *
 * @param $user object
 *  Remote user object.
 * @param $account object
 *  Successfully saved and returned local user object.
 */
function hook_services_sso_client_update_remote_process($user, $account) {

}

/**
 * Draft version of a hook to run on a successfully checked
 * and propagated account from the SSO.
 */
function hook_services_sso_client_update_remote_update($uid) {

}
