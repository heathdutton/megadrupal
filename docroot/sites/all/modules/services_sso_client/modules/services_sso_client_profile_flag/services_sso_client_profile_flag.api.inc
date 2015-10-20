<?php

/**
 * hook_services_sso_client_profile_flag_action_pre_apply().
 *
 * This hook is invoked just **BEFORE** the ->apply() method is called
 * on a particular profileFlag's particular profileFlagAction instance.
 *
 * @param profileFlag $profile_flag
 *  The profileFlag object that contains multiple profileFlagAction instances.
 * @param profileFlagAction $action
 *  The profileFlagAction object that actually calls the ->apply() method.
 * @param array $uids
 *  A list of valid user uids that have that particular flag+flag action combination
 *  associated with them.
 */
function hook_services_sso_client_profile_flag_action_pre_apply(&$profile_flag, &$action, $uids) {

}

/**
 * hook_services_sso_client_profile_flag_action_post_apply().
 *
 * This hook is invoked just **AFTER** the ->apply() method is called
 * on a particular profileFlag's particular profileFlagAction instance.
 *
 * @param profileFlag $profile_flag
 *  The profileFlag object that contains multiple profileFlagAction instances.
 * @param profileFlagAction $action
 *  The profileFlagAction object that actually calls the ->apply() method.
 * @param array $uids
 *  A list of valid user uids that have that particular flag+flag action combination
 *  associated with them.
 */
function hook_services_sso_client_profile_flag_action_post_apply(&$profile_flag, &$action, $uids) {

}
