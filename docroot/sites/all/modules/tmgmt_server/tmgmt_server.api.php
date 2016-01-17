<?php

/**
 * Allows to alter the response sent to client upon translation request.
 *
 * @param array $processed_request
 *   Data that will be sent to the client as a response.
 * @param TMGMTJob $job
 *   Translation job that just has been created.
 * @param array $remote_sources
 *   Array of remote source entities.
 */
function hook_tmgmt_server_processed_request_alter(&$processed_request, TMGMTJob $job, $remote_sources) {

}

/**
 * Allows to alter the remote source entity just after it has been created.
 *
 * @param TMGMTRemoteSource $remote_source
 *   Remote source entity.
 * @param $translation_item
 *   Translation item sent by client.
 */
function hook_tmgmt_server_remote_source_alter(TMGMTRemoteSource $remote_source, $translation_item) {

}

/**
 * Provides the server language pairs of which it is capable to translate.
 */
function hook_tmgmt_server_language_pairs() {

}

/**
 * Called when new remote client is created via service.
 *
 * @param object $account
 *   Drupal user object.
 * @param TMGMTClient $remote_client
 *   Remote client entity.
 * @param array $user_info
 *   Additional user info specified by
 */
function hook_tmgmt_server_client_created_via_service($account, TMGMTClient $remote_client, array $user_info) {

}

