<?php

/**
 * @file
 * commerce_reverse_auction.rules.inc
 * Contains Rules conditions triggered by commerce_reverse_auction module.
 */

/**
 * Implements hook_rules_event_info(). 
 */
function commerce_reverse_auction_rules_event_info() {
  $events = array(
    'commerce_reverse_auction_bid_selected' => array(
      'label' => t('A Bid on a commerce reverse auction got selected'),
      'group' => t('Commerce Reveese Auction'),
      'variables' => array(
        'bid' => array(
          'type' => 'commerce_auction_bid',
          'label' => t('Reverse Auction Bid'),
        ),
        'reverse_auction' => array(
          'type' => 'node',
          'label' => t('Reverse auction display node')
        ),
      ),
    ),
  );
  return $events;
}

