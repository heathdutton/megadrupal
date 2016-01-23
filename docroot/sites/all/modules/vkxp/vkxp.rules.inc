<?php

/**
 * @file
 * Rules integration for the VK CrossPoster module.
 */

/**
 * Implements hook_rules_action_info().
 */
function vkxp_rules_action_info() {
  return array(
    'vkxp_post_to_wall_action' => array(
      'label' => t('Post node to VK wall'),
      'group' => t('System'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Content'),
        ),
      ),
    ),
  );
}

/**
 * Action: Post node to VK wall.
 */
function vkxp_post_to_wall_action($node) {
  // Get all flags to see if node could be crossposted.
  $vkxp_enabled = variable_get('vkxp_node_enabled_' . $node->type);
  $access_token = variable_get('vkxp_access_token');

  // Check requiered data to post.
  if ($vkxp_enabled && $access_token) {

    // Get node data that will be crossposted.
    $message = _vkxp_get_node_message($node);
    $images  = _vkxp_get_node_images($node);
    $url     = url('node/' . $node->nid, array('absolute' => TRUE));

    // Post node to VK.
    _vkxp_post_to_wall($message, $images, $url);
  }
}
