<?php

/**
 * @file
 * This file contains functions to retrieve Fortissimmo data
 */

/**
 * Get all Fortissimmo active accounts
 */
function fortissimmo_get_accounts($reset = FALSE) {
  $accounts = &drupal_static(__FUNCTION__);

  if (!isset($accounts) || $reset) {
    $accounts = db_select('tblbedrijf_office', 'office')
      ->fields('office', array('fortissimmo_bedrijfid', 'Account_name'))
      ->execute()
      ->fetchAllKeyed();
  }

  return $accounts;
}

/**
 * Get the available transaction types
 */
function fortissimmo_get_transactions($accountid = NULL, $lang = NULL, $reset = FALSE) {
  $transactions = &drupal_static(__FUNCTION__);
  global $language;

  if (!$lang) {
    $lang = $language->language;
  }
  if (!$accountid) {
    $accountid = variable_get('fortissimmo_default_account', 0);
  }

  if (!isset($transactions[$accountid][$lang]) || $reset) {
    $transactions[$accountid][$lang] = db_select('tbl_uzoekt_transaction', 'tx')
      ->condition('tx.bedrijfid', $accountid, '=')
      ->condition('tx.lang', drupal_strtoupper($lang), '=')
      ->fields('tx', array('value', 'name'))
      ->execute()
      ->fetchAllKeyed();
  }

  return $transactions[$accountid][$lang];
}

/**
 * Get the available city values
 */
function fortissimmo_get_property_cities($accountid = NULL, $reset = FALSE) {
  $cities = &drupal_static(__FUNCTION__);

  if (!$accountid) {
    $accountid = variable_get('fortissimmo_default_account', 0);
  }

  if (!isset($cities[$accountid]) || $reset) {
    $cities[$accountid] = db_select('tbl' . $accountid . '_pand', 'pand')
      ->fields('pand', array('city', 'cityValue'))
      ->groupBy('city')
      ->orderBy('cityValue')
      ->execute()
      ->fetchAllKeyed();
  }

  return $cities[$accountid];
}

/**
 * Get the property title for a given Fortissimmo property ID
 */
function fortissimmo_property_get_title($fortissimmo_id, $reset = FALSE) {
  $titles = &drupal_static(__FUNCTION__);

  if (!isset($titles[$fortissimmo_id]) || $reset) {
    $accountid = fortissimmo_property_get_accountid($fortissimmo_id);
    if (is_numeric($accountid) && $accountid > 0) {
      $result = db_select('tbl' . $accountid . '_pand', 'pand')
        ->condition('pand.fortissimmo', $fortissimmo_id, '=')
        ->fields('pand', array('title', 'reference'))
        ->execute()
        ->fetchObject();

      if ($result) {
        if (!empty($result->title)) {
          $titles[$fortissimmo_id] = $result->title;
        }
        else {
          $titles[$fortissimmo_id] = $result->reference;
        }
        drupal_alter('fortissimmo_title', $titles[$fortissimmo_id]);
      }
      else {
        $titles[$fortissimmo_id] = '';
      }
    }
  }

  return $titles[$fortissimmo_id];
}

/**
 * Get the transaction type for a given Fortissimmo property ID
 */
function fortissimmo_property_get_transaction_type($fortissimmo_id, $reset = FALSE) {
  $transaction_types = &drupal_static(__FUNCTION__);

  if (!isset($transaction_types[$fortissimmo_id]) || $reset) {
    $accountid = fortissimmo_property_get_accountid($fortissimmo_id);
    if (is_numeric($accountid) && $accountid > 0) {
      $result = db_select('tbl' . $accountid . '_pand', 'pand')
        ->condition('pand.fortissimmo', $fortissimmo_id, '=')
        ->fields('pand', array('transaction', 'transactionValue'))
        ->execute()
        ->fetchObject();
      if ($result) {
        drupal_alter('fortissimmo_transaction_type', $result);
        $transaction_types[$fortissimmo_id] = $result->transactionValue;
      }
    }
  }

  return $transaction_types[$fortissimmo_id];
}

/**
 * Get a list of available titles
 */
function fortissimmo_get_titles($accountid = NULL, $lang = NULL, $reset = FALSE) {
  $titles = &drupal_static(__FUNCTION__);

  global $language;
  if (!$lang) {
    $lang = $language->language;
  }
  if (!$accountid) {
    $accountid = variable_get('fortissimmo_default_account', 0);
  }

  if (!isset($titles[$accountid][$lang]) || $reset) {
    $titles[$accountid][$lang] = db_select('tbl_uzoekt_title', 'title')
      ->condition('title.bedrijfid', $accountid)
      ->condition('title.lang', drupal_strtoupper($lang))
      ->fields('title', array('value', 'name'))
      ->execute()
      ->fetchAllKeyed();
  }

  return $titles[$accountid][$lang];
}

/**
 * Get a list of available regions
 */
function fortissimmo_get_regions($accountid = NULL, $reset = FALSE) {
  $regions = &drupal_static(__FUNCTION__);

  if (!$accountid) {
    $accountid = variable_get('fortissimmo_default_account', 0);
  }

  if (!isset($regions[$accountid]) || $reset) {
    $regions[$accountid] = db_select('tbl_uzoekt_region', 'region')
      ->condition('region.bedrijfid', $accountid)
      ->fields('region', array('value', 'name'))
      ->execute()
      ->fetchAllKeyed();
    array_unshift($regions[$accountid], '--');
  }

  return $regions[$accountid];
}

/**
 * Get city_round values
 */
function fortissimmo_get_city_round() {
  return array(
    0 => ' -- ',
    1 => t('1 km'),
    2 => t('5 km'),
    3 => t('10 km'),
    4 => t('20 km'),
  );
}

/**
 * Get a list of available property types
 */
function fortissimmo_get_types($accountid = NULL, $lang = NULL, $reset = FALSE) {
  $types = &drupal_static(__FUNCTION__);

  global $language;
  if (!$lang) {
    $lang = $language->language;
  }
  if (!$accountid) {
    $accountid = variable_get('fortissimmo_default_account', 0);
  }

  if (!isset($types[$accountid][$lang]) || $reset) {
    $types[$accountid][$lang] = db_select('tbl_uzoekt_type', 'type')
      ->fields('type', array('value', 'name'))
      ->orderBy('value')
      ->execute()
      ->fetchAllKeyed();
  }

  return $types[$accountid][$lang];
}

/**
 * Save a request form submission
 */
function fortissimmo_save_request($data) {
  global $language;
  $accountid = variable_get('fortissimmo_default_account', 0);
  $mediaid = db_query_range("SELECT fortissimmo_mediaid FROM {tbl" . $accountid . "_pand} WHERE fortissimmo_bedrijfid = :accountid", 0, 1, array(':accountid' => $accountid))->fetchField();
  $contactid = fortissimmo_save_request_contact($data, $accountid, $mediaid);
  $requestid = fortissimmo_save_request_data($data, $accountid, $mediaid, $contactid);

  db_insert('tblbedrijf_synchronisatie_request_contact')
  ->fields(array(
    'bedrijfid' => $accountid,
    'mediaid' => $mediaid,
    'contact' => $contactid,
    'remove' => FALSE,
  ))
  ->execute();

  db_insert('tblbedrijf_synchronisatie_request')
  ->fields(array(
    'bedrijfid' => $accountid,
    'mediaid' => $mediaid,
    'requestid' => $requestid,
    'remove' => FALSE,
  ))
  ->execute();
}

/**
 * Save a contact to the database
 */
function fortissimmo_save_request_contact($data, $accountid, $mediaid) {
  global $language;

  return db_insert('tbl' . $accountid . '_uzoekt_contact')
  ->fields(array(
    'fortissimmo' => NULL,
    'fortissimmo_mediaid' => $mediaid,
    'fortissimmo_bedrijfid' => $accountid,
    'taal' => drupal_strtoupper($language->language),
    'email' => $data['email'],
    'title' => $data['title'],
    'lastname' => $data['lastname'],
    'firstname' => $data['firstname'],
    'address' => $data['address'],
    'zipcode' => $data['zipcode'],
    'city' => $data['city'],
    'phone' => $data['phone'],
  ))
  ->execute();
}

/**
 * Save a request to the database
 */
function fortissimmo_save_request_data($data, $accountid, $mediaid, $contactid) {
  $transactions = implode(';', array_filter($data['transaction']));
  $types = implode(';', array_filter($data['typeValue']));

  return db_insert('tbl' . $accountid . '_uzoekt_request')
  ->fields(array(
    'fortissimmo' => NULL,
    'fortissimmo_mediaid' => $mediaid,
    'fortissimmo_bedrijfid' => $accountid,
    'contact' => $contactid,
    'bedroom_max' => $data['bedroom_max'],
    'bedroom_min' => $data['bedroom_min'],
    'city_round' => $data['city_round'],
    'cityValue' => $data['cityValue'],
    'region' => (isset($data['region']) && $data['region'] > 0) ? $data['region'] : NULL,
    'name' => time(),
    'price_max' => $data['price_max'],
    'price_min' => $data['price_min'],
    'transaction' => $transactions,
    'type' => $types,
  ))
  ->execute();
}

/**
 * Save a property-specific information request
 */
function fortissimmo_save_property_request($data) {
  $propertyid = $data['fortissimmo_id'];
  $accountid = variable_get('fortissimmo_default_account', 0);
  $mediaid = db_query_range("SELECT fortissimmo_mediaid FROM {tbl" . $accountid . "_pand} WHERE fortissimmo_bedrijfid = :accountid", 0, 1, array(':accountid' => $accountid))->fetchField();

  return db_insert('tblbedrijf_synchronisatie_pand_contact')
  ->fields(array(
    'bedrijfid' => $accountid,
    'propertyid' => $propertyid,
    'mediaid' => $mediaid,
    'lastname' => $data['lastname'],
    'firstname' => $data['firstname'],
    'phone' => $data['phone'],
    'email' => $data['email'],
    'street' => $data['street'],
    'number' => $data['number'],
    'postalcode' => $data['postalcode'],
    'city' => $data['city'],
    'titleid' => $data['titleid'],


  ))
  ->execute();
}