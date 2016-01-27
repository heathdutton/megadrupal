<?php
/**
 * @file
 * TMGMT server page callbacks.
 */

function tmgmt_server_ds_status_overview() {

  try {
    $connector = new TMGMTServerDSConnector(variable_get('tmgmt_ds_url'));
    $response = $connector->testConnection();

    $status = $response['status'];
    $identifier = $response['identifier'];
  }
  catch (TMGMTRemoteException $e) {
    return array(
      '#type' => 'markup',
      '#markup' => t('Unable to connect to the TMGMT Directory, please configure the <a href="@url">API connection</a>.',
        array('@url' => url('admin/directory-server/settings'))),
      '#prefix' => '<div class="messages warning">',
      '#suffix' => '</div>',
    );
  }

  return array(
    '#theme' => 'tmgmt_server_ds_todo',
    '#attached' => array('css' => array(drupal_get_path('module', 'tmgmt_server') . '/css/tmgmt_server_ds.css')),
    '#status' => $status,
    '#identifier' => check_plain($identifier),
  );
}

/**
 * Page callback for the client entity listing.
 */
function tmgmt_server_client_profile_page($user) {
  $output = array();
  $original = drupal_get_title();

  // Override the title that was set by the Entity API module.
  drupal_set_title($original, PASS_THROUGH);

  // Try to prepend the client view to the output.
  $output['clients']['#markup'] = views_embed_view('tmgmt_server_clients');

  return $output;
}

function tmgmt_server_client_details_profile_page($account, $client) {
  $form = entity_form('tmgmt_server_client', $client);
  return drupal_render($form);
}

function tmgmt_server_client_add_profile_page($account) {
  $client = entity_get_controller('tmgmt_server_client')->create(array('uid' => $account->uid));
  $form = entity_form('tmgmt_server_client', $client);
  drupal_set_title(t('Add new client'));
  return drupal_render($form);
}
