<?php
// ======================================
// TRIGGERS
// ======================================


/*
 * Triggers:
 * + Gift card is activated
 * + Gift card is redeemed
 */

/**
 * Implements hook_rules_event_info()
 */
function ms_gift_cards_rules_event_info() {
  $items = array(
    'ms_gift_cards_event_activation' => array(
      'label' => t('Gift Card Activated'),
      'group' => 'MS Gift Cards',
      'help' => t("Triggers when a user activated a gift card."),
      'variables' => array(
        'account' => array('type' => 'user', 'label' => t('Account')),
        'gift_card' => array('type' => 'ms_gift_card', 'label' => t('Membership')),
        'order' => array('type' => 'ms_order', 'label' => t('Order', array(), array('context' => 'ms_core'))),
        'product' => array( 'type' => 'ms_product', 'label' => t('Product')),
        'payment' => array('type' => 'ms_payment', 'label' => t('Payment')),
      ),
    ),
    'ms_gift_cards_event_redemption' => array(
      'label' => t('Gift Card Redeemed'),
      'group' => 'MS Gift Cards',
      'help' => t("Triggers when a user redeems their gift card."),
      'variables' => array(
        'account' => array('type' => 'user', 'label' => t('Account'),),
        'gift_card' => array('type' => 'ms_gift_card', 'label' => t('Membership'),),
        'order' => array('type' => 'ms_order', 'label' => t('Order', array(), array('context' => 'ms_core')),),
        'product' => array('type' => 'ms_product', 'label' => t('Product'),),
      ),
    ),
  );

  return $items;
}


// ======================================
// DATA TYPES
// ======================================

/**
 * Implements hook_rules_data_info()
 */
function ms_gift_cards_rules_data_info() {
  return array(
    'ms_gift_card' => array(
      'label' => t('Gift Card'),
      'wrap' => TRUE,
      'group' => t('MS Gift Cards'),
    ),
  );
}
