<?php

/**
 * @file
 * Builds placeholder replacement tokens for AdInsight Clarity.
 */

/**
 * Implements hook_token_info().
 */
function adinsight_clarity_token_info() {

  $type = array(
    'name' => t('AdInsight Clarity'),
    'description' => t('Tokens related to the AdInsight Clarity tracking library.'),
  );

  $adminsight = array();

  $adinsight['account'] = array(
    'name' => t('Account ID'),
    'description' => t('The current AdInsight Clarity account ID.'),
  );
  $adinsight['pool'] = array(
    'name' => t('Number Pool'),
    'description' => t('The ID of the number pool from which to draw dynamic numbers.'),
  );
  $adinsight['base-number'] = array(
    'name' => t('Base Number'),
    'description' => t('The base phone number for non-JavaScript users, replaced by a number from the number pool.'),
  );
  $adinsight['tag'] = array(
    'name' => t('HTML span tag'),
    'description' => t('A completely formatted telephone number span, allowing a token to be used in the theme and replaced with the correct phone number'),
  );

  return array(
    'types' => array('adinsight_clarity' => $type),
    'tokens' => array('adinsight_clarity' => $adinsight),
  );
}

/**
 * Implements hook_tokens().
 */
function adinsight_clarity_tokens($type, $tokens, array $data = array(), array $options = array()) {

  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'adinsight_clarity') {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'account':
          $value = variable_get('adinsight_clarity_account', '');
          $replacements[$original] = $sanitize ? check_plain($value) : $value;
          break;

        case 'pool':
          $value = variable_get('adinsight_clarity_pool', '');
          $replacements[$original] = $sanitize ? check_plain($value) : $value;
          break;

        case 'base-number':
          $value = variable_get('adinsight_clarity_base_phone', '');
          $replacements[$original] = $sanitize ? check_plain($value) : $value;
          break;

        case 'tag':
          $replacements[$original] = _adinsight_clarity_build_tag();
          break;

      }
    }
  }

  return $replacements;
}
