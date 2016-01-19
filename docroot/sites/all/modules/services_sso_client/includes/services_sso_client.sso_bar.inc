<?php

/**
 * Callback to generate the content portion of the sso bar block.
 */
function services_sso_client_sso_bar() {
  if (user_access('administer site')) {
    drupal_add_css(drupal_get_path('module', 'services_sso_client') . '/css/sso_bar.css');
    $sso_server = variable_get('services_sso_client_server_address', '');
    $info = '<li class="info-block sso-server clearfix"><span class="label label-info">Server:</span> ' . $sso_server . '</li>';

    $endpoint_url = $sso_server . '/sso/server-workflow.json';
    $response = drupal_http_request($endpoint_url);
    if ($response->code == 200) {
      $data = json_decode($response->data);
      $server_workflow = $data->workflow;

      if (!empty($server_workflow)) {
        $server_workflow_class = drupal_html_class('sso-workflow-' . $server_workflow);
        $info .= '<li class="info-block sso-workflow clearfix"><span class="label label-important">Workflow:</span> ' . $server_workflow . '</li>';

        if ($server_workflow == 'prod') {
          global $base_url;
          $path = drupal_get_path('module', 'services_sso_client');
          drupal_add_css("#page {background-image: url({$base_url}/{$path}/img/live.png);}", array('type' => 'inline'));
        }
      }
    }
    if (!empty($info)) {
      $output = '<ul class="info clearfix">' . $info . '</ul>';
    }

    return '<div class="sso-bar ' . $server_workflow_class . '">' . $output . '</div>';
  }
}
