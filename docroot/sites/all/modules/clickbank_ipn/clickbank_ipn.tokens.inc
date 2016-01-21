<?php

/**
 * @file
 * Builds placeholder replacement tokens for order-related data.
 * NEEDS WORK
 */


/**
 * Implements hook_token_info().
 */
function clickbank_ipn_token_info() {
  $type = array(
    'name' => t('Clickbank Orders', array(), array('context' => 'a drupal clickbank order')),
    'description' => t('Tokens related to individual orders.'),
    'needs-data' => 'clickbank-order',
  );

  // Tokens for orders.
  $clickbankorder = array();

  $clickbankorder['receipt'] = array(
    'name' => t('Receipt ID', array(), array('context' => 'a clickbank order')),
    'description' => t('The internal numeric ID of the order.'),
  );
  $clickbankorder['version'] = array(
    'name' => t('Version', array(), array('context' => 'version of instant notification')),
    'description' => t('The version of the clickbank instant notification. See https://support.clickbank.com/entries/22803622-instant-notification-service'),
  );
  $clickbankorder['type'] = array(
    'name' => t('Order type'),
    'description' => t('The type of the order.'),
  );
  $clickbankorder['type-name'] = array(
    'name' => t('Order type name'),
    'description' => t('The human-readable name of the order type.'),
  );
  $clickbankorder['currency'] = array(
    'name' => t('Currency'),
    'description' => t('Currency of the Purchase [only populated for vendor]'),
  );
  $clickbankorder['affiliate'] = array(
    'name' => t('Affiliate'),
    'description' => t('Affiliate nickname'),
  );
  $clickbankorder['transactionType'] = array(
    'name' => t('transactionType'),
    'description' => t('Type of Transaction (SALE, JV_SALE, INSF, RFND, CGBK, BILL, JV_BILL, TEST_SALE, TEST_JV_SALE, TEST_BILL, TEST_JV_BILL, TEST_RFND)'),
  );
  $clickbankorder['orderLanguage'] = array(
    'name' => t('orderLanguage'),
    'description' => t('Language of the Order'),
  );
  $clickbankorder['totalProductAmount'] = array(
    'name' => t('totalProductAmount'),
    'description' => t('Total Amount of the Sale'),
  );

  // Chained tokens for orders.
  $clickbankorder['created'] = array(
    'name' => t('Date created'),
    'description' => t('The date the order was created.'),
    'type' => 'date',
  );
  $clickbankorder['changed'] = array(
    'name' => t('Date changed'),
    'description' => t('The date the order was last updated.'),
    'type' => 'date',
  );

  return array(
    'types' => array('clickbank-order' => $type),
    'tokens' => array('clickbank-order' => $clickbankorder),
  );
}

/**
 * Implements hook_tokens().
 */
function clickbank_ipn_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'clickbank-order' && !empty($data['clickbank-order'])) {
    $clickbankorder = $data['clickbank-order'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the order.
        case 'receipt':
          $replacements[$original] = $clickbankorder->receipt;
          break;

        case 'version':
          $replacements[$original] = $clickbankorder->version;
          break;

        case 'type':
          $replacements[$original] = $sanitize ? check_plain($clickbankorder->type) : $clickbankorder->type;
          break;

        case 'type-name':
          $type_name = clickbank_order_type_get_name($clickbankorder->type);
          $replacements[$original] = $sanitize ? check_plain($type_name) : $type_name;
          break;

        case 'currency':
          $replacements[$original] = $clickbankorder->currency;
          break;

        case 'affiliate':
          $replacements[$original] = $clickbankorder->affiliate;
          break;

        case 'transactionType':
          $replacements[$original] = $clickbankorder->transactionType;
          break;

        case 'orderLanguage':
          $replacements[$original] = $clickbankorder->orderLanguage;
          break;

        case 'totalProductAmount':
          $replacements[$original] = $clickbankorder->totalProductAmount;
          break;


        // Default values for the chained tokens handled below.
        case 'created':
          $replacements[$original] = format_date($clickbankorder->created, 'medium', '', NULL, $language_code);
          break;

        case 'changed':
          $replacements[$original] = format_date($clickbankorder->changed, 'medium', '', NULL, $language_code);
          break;
      }
    }

    foreach (array('created', 'changed') as $date) {
      if ($created_tokens = token_find_with_prefix($tokens, $date)) {
        $replacements += token_generate('date', $created_tokens, array('date' => $clickbankorder->{$date}), $options);
      }
    }
  }

  return $replacements;
}