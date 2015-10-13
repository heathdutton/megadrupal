<?php

/**
 * @file
 * Provides token support for Giftcards products
 */

/*
 * Implements hook_token_info().
 */
function commerce_gc_product_token_info_alter(&$info) {
  $info['tokens']['commerce-line-item']['giftcard-message-text'] = array(
    'name' => t('Giftcard message text'),
    'description' => t('Formatted presentation of message field'),
    'type' => 'commerce-order',
  );
}

/*
 * Implements hook_tokens().
 */
function commerce_gc_product_tokens($type, $tokens, array $data = array(), array $options = array()) {
  if ($type == 'commerce-line-item') {
    $replacements = array();    
    
    foreach ($tokens as $name => $original) {
      // Add a token for giftcard balance.
      if ($name == 'giftcard-message-text' && !empty($data['commerce-line-item'])) {
        
        // Determine if this line item bundle has a message field and whether it
        // is empty.
        $message = '';
        if (!empty($data['commerce-line-item'])) {
          $line_item = $data['commerce-line-item'];
          
          if (field_info_instance('commerce_line_item', 'commerce_gc_message', $line_item->type)) {
            $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
            if ($line_item_wrapper->commerce_gc_message->value()) {
              // TODO "conceal sender" setting for giftcard product bundles.
              if ($line_item_wrapper->order->value() && $line_item_wrapper->order->mail->value()) {
                $message = t('Message from @sender: 

"@message"', array(
                  '@sender' => $line_item_wrapper->order->mail->value(),
                  '@message' => $line_item_wrapper->commerce_gc_message->value()
                ));
              }
              else {
                $message = t('Message: 

"@message"', array(
                  '@message' => $line_item_wrapper->commerce_gc_message->value()
                ));
              }
              $message = $message . '

';
            }
          }
        }
        
        $replacements[$original] = $message;
      }
    }
    return $replacements;
  }
}
