<?php

/**
 * @file
 * Builds placeholder replacement tokens for node_expire_extras.
 */

/**
 * Implements hook_token_info().
 */
function node_expire_extras_token_info() {
  $info['tokens']['node']['expire'] = array(
    'name' => t('Expiry date'),
    'description' => t("The date when the node expires."),
  );
  $info['tokens']['node']['expired'] = array(
    'name' => t('Expired flag'),
    'description' => t("The flag indicates, that node is expired."),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function node_expire_extras_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'expire':
          if (isset($node->expire)) {
            $replacements[$original] = $node->expire;
          }
          break;

        case 'expired':
          if (isset($node->expired)) {
            $replacements[$original] = $node->expired;
          }
          break;
      }
    }
    /*
    // Chained token replacement.
    if (isset($node->publish_on) && $publish_tokens
      = token_find_with_prefix($tokens, 'scheduler-publish')) {
        $replacements += token_generate('date', $publish_tokens,
          array('date' => $node->publish_on), $options);
    }
    if (isset($node->unpublish_on) && $unpublish_tokens
      = token_find_with_prefix($tokens, 'scheduler-unpublish')) {
      $replacements += token_generate('date', $unpublish_tokens,
        array('date' => $node->unpublish_on), $options);
    }
    */
  }

  return $replacements;
}
