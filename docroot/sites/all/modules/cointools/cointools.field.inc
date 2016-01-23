<?php

use Drupal\cointools\CoinTools;
use BitWasp\BitcoinLib\BitcoinLib;

/**
 * @file
 * Field module functionality for the Coin Tools module.
 */

/**
 * Implements hook_field_info().
 */
function cointools_field_info() {
  return array(
    'cointools_address' => array(
      'label' => t("Coin Address"),
      'description' => t("This field stores coin addresses in the database."),
      'default_widget' => 'cointools_address_textfield',
      'default_formatter' => 'cointools_address_text',
    ),
    'cointools_amount' => array(
      'label' => t("Coin Amount"),
      'description' => t("This field stores coin amounts in the database."),
      'default_widget' => 'cointools_amount',
      'default_formatter' => 'cointools_amount',
    ),
    'cointools_transaction' => array(
      'label' => t("Coin Transaction"),
      'description' => t("This field stores coin addresses in the database."),
      'default_widget' => 'cointools_transaction_textfield',
      'default_formatter' => 'cointools_transaction_text',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function cointools_field_is_empty($item, $field) {
  return $item['value'] === NULL || $item['value'] === '';
}

/**
 * Implements hook_field_schema().
 */
function cointools_field_schema($field) {
  switch ($field['type']) {
    case 'cointools_address':
      $columns = array(
        'value' => array(
          'type' => 'blob',
          'length' => 25,
          'mysql_type' => 'BINARY(25)',
        ),
      );
      $indexes = array(
        'value' => array('value'),
      );
      break;

    case 'cointools_amount':
      $columns = array(
        'value' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'size' => 'big',
        ),
      );
      $indexes = array();
      break;

    case 'cointools_transaction':
      $columns = array(
        'value' => array(
          'type' => 'blob',
          'length' => 32,
          'mysql_type' => 'BINARY(32)',
        ),
      );
      $indexes = array(
        'value' => array('value'),
      );
      break;
  }
  
  return array(
    'columns' => $columns,
    'indexes' => $indexes,
  );
}

/**
 * Implements hook_field_presave().
 */
function cointools_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  switch ($field['type']) {
    case 'cointools_address':
      foreach ($items as &$item) {
        $item['value'] = hex2bin(BitcoinLib::base58_decode($item['value']));
      }
      break;

    case 'cointools_amount':
      foreach ($items as &$item) {
        $units = array_keys(cointools_units());
        $exp = array_search($item['unit'], $units);
        $item['value'] *= pow(1000, $exp) * 100;
      }
      break;

    case 'cointools_transaction':
      foreach ($items as &$item) {
        $item['value'] = hex2bin($item['value']);
      }
      break;
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function cointools_field_formatter_info() {
  return array(
    'cointools_address_qrcode' => array(
      'label' => t("QR Code"),
      'field types' => array('cointools_address'),
      'settings' => array(
        'width' => 200,
      ),
    ),
    'cointools_address_text' => array(
      'label' => t("Text"),
      'field types' => array('cointools_address'),
    ),
    'cointools_amount' => array(
      'label' => t("Amount"),
      'field types' => array('cointools_amount'),
      'settings' => array(
        'unit' => 'auto',
        'unit_display' => TRUE,
        'decimal_places' => NULL,
        'fiat' => FALSE,
      ),
    ),
    'cointools_transaction_info' => array(
      'label' => t("Info"),
      'field types' => array('cointools_transaction'),
    ),
    'cointools_transaction_text' => array(
      'label' => t("Text"),
      'field types' => array('cointools_transaction'),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function cointools_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  switch ($display['type']) {

    case 'cointools_address_qrcode':
      $element['width'] = array(
        '#title' => t("Width"),
        '#type' => 'numberfield',
        '#default_value' => $settings['width'],
        '#required' => TRUE,
      );
      break;
      
    case 'cointools_amount':
      $options['auto'] = t("Auto");
      $units = cointools_units();
      foreach ($units as $key => $value) {
        $options[$key] = $key;
      }
      $element['unit'] = array(
        '#title' => t("Unit"),
        '#type' => 'select',
        '#default_value' => $settings['unit'],
        '#options' => $options,
      );
      $element['unit_display'] = array(
        '#title' => t("Display unit"),
        '#type' => 'checkbox',
        '#default_value' => $settings['unit_display'],
      );
      $element['decimal_places'] = array(
        '#title' => t("Decimal places"),
        '#type' => 'numberfield',
        '#default_value' => $settings['decimal_places'],
        '#min' => 0,
        '#max' => 15,
      );
      $element['fiat'] = array(
        '#title' => t("Fiat"),
        '#type' => 'checkbox',
        '#default_value' => $settings['fiat'],
      );
      break;
  }
  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function cointools_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  switch ($display['type']) {

   case 'cointools_address_qrcode':
      $summary[] = t("Width: @width", array('@width' => $settings['width']));
      break;

    case 'cointools_amount':
      $summary[] = t("Unit: @unit", array('@unit' => $settings['unit']));
      $summary[] = t("Display unit: @unit_display", array('@unit_display' => $settings['unit_display'] ? t("True") : t("False")));
      if (is_numeric($settings['decimal_places'])) {
        $summary[] = t("Decimal places: @decimal_places", array('@decimal_places' => $settings['decimal_places']));
      }
      $summary[] = t("Fiat: @fiat", array('@fiat' => $settings['fiat'] ? t("True") : t("False")));
      break;
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function cointools_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $elements = array();

  switch ($display['type']) {

    case 'cointools_address_qrcode':
      // Generate QR code for each item.
      foreach ($items as $delta => $item) {
        $elements[$delta] = array(
          '#theme' => 'cointools_qr',
          '#address' => BitcoinLib::base58_encode(bin2hex($item['value'])),
          '#width' => $display['settings']['width'],
          '#height' => $display['settings']['width'],
        );
      }
      break;

    case 'cointools_address_text':
      foreach ($items as $delta => $item) {
        $address = BitcoinLib::base58_encode(bin2hex($item['value']));
        $elements[$delta] = array(
          '#type' => 'link',
          '#title' => $address,
          '#href' => 'bitcoin:' . $address,
          '#options' => array(
            'external' => TRUE,
          ),
          '#prefix' => '<span class="cointools-monospace">',
          '#suffix' => '</span>',
        );
      }
      break;

    case 'cointools_amount':
      foreach ($items as $delta => $item) {
        $elements[$delta] = array(
          '#theme' => 'cointools_amount',
          '#amount' => $item['value'],
          '#unit' => $display['settings']['unit'],
          '#unit_display' => $display['settings']['unit_display'],
          '#decimal_places' => $display['settings']['decimal_places'],
          '#fiat' => $display['settings']['fiat'],
        );
      }
      break;

    case 'cointools_transaction_info':
      foreach ($items as $delta => $item) {
        $transaction = array(
          '#theme' => 'cointools_transaction',
          '#txn_id' => bin2hex($item['value']),
        );
        $elements[$delta] = $transaction;
      }
      break;

    case 'cointools_transaction_text':
      foreach ($items as $delta => $item) {
        $elements[$delta] = array(
          '#type' => 'markup',
          '#markup' => bin2hex($item['value']),
        );
      }
      break;
  }

  return $elements;
}

/**
 * Implements hook_field_widget_info().
 */
function cointools_field_widget_info() {
  return array(
    'cointools_address_textfield' => array(
      'label' => t("Text Field"),
      'field types' => array('cointools_address'),
    ),
    'cointools_amount' => array(
      'label' => t("Text Field"),
      'field types' => array('cointools_amount'),
    ),
    'cointools_transaction_textfield' => array(
      'label' => t("Text Field"),
      'field types' => array('cointools_transaction'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function cointools_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  switch ($instance['widget']['type']) {
    case 'cointools_address_textfield':
      $element['value'] = $element + array(
        '#type' => 'textfield',
        '#default_value' => isset($items[$delta]) ? BitcoinLib::base58_encode(bin2hex($items[$delta]['value'])) : '',
        '#attributes' => array('class' => array('cointools_address')),
        '#size' => 34,
      );
      break;

    case 'cointools_amount':
      if (isset($items[$delta]) && $items[$delta]['value'] !== '' && $items[$delta]['value'] !== NULL) {
        list ($unit, $amount) = cointools_unit_amount($items[$delta]['value']);
      }
      else {
        $unit = 'BTC';
        $amount = '';
      }

      $units = cointools_units();

      foreach ($units as $key => &$value) {
        $value = $key;
      }

      $element['unit'] = $element + array(
        '#type' => 'select',
        '#options' => $units,
        '#default_value' => $unit,
        '#weight' => 0,
      );

      $element['value'] = array(
        '#type' => 'numberfield',
        '#default_value' => $amount,
        '#weight' => 1,
        '#step' => 'any',
        '#min' => 0,
      );
      break;

    case 'cointools_transaction_textfield':
      $element['value'] = $element + array(
        '#type' => 'textfield',
        '#default_value' => isset($items[$delta]['value']) ? bin2hex($items[$delta]['value']) : '',
      );
      break;
  }

  return $element;
}

/**
 * Implements hook_field_validate().
 */
function cointools_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {

  switch ($instance['widget']['type']) {
    case 'cointools_address_textfield':
      foreach ($items as $delta => $item) {
        // Core does not seem to check if the field is empty.
        if (cointools_field_is_empty($item, $field)) {
          continue;
        }
        if (!CoinTools::validateAddress($item['value'])) {
          $errors[$field['field_name']][$langcode][$delta][] = array(
            'error' => 'invalid_address',
            'message' => t("Bitcoin address is invalid."),
          );
        }
      }
      break;
  }
}
