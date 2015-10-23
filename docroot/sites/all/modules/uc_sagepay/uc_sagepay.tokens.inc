<?php

/**
 * @file
 * Token hooks for the uc_sagepay module.
 */

/**
 * Implements hook_token_info().
 */
function uc_sagepay_token_info() {
  $tokens['uc_order']['sagepay-vendortxcode'] = array(
    'name' => t('Sage Pay transaction code'),
    'description' => t('The Sage Pay VendorTXCode associated with the order.'),
  );

  return array('tokens' => $tokens);
}

/**
 * Implements hook_tokens().
 */
function uc_sagepay_tokens($type, $tokens, $data = array(), $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();

  if ($type == 'uc_order' && !empty($data['uc_order'])) {
    $order = $data['uc_order'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'sagepay-vendortxcode':
          $comments = uc_order_comments_load($order->order_id, TRUE);
          if (is_array($comments)) {
            foreach ($comments as $comment) {
              $subpatterns = array();
              preg_match('/VendorTxCode: (.*)/', $comment->message, $subpatterns);
              if (!empty($subpatterns[1])) {
                $replacements[$original] = $sanitize ? check_plain($subpatterns[1]) : $subpatterns[1];
              }
            }
          }
          break;
      }
    }
  }

  return $replacements;
}
