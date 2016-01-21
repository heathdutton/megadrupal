<?php

namespace Drupal\cointools;

use Symfony\Component\Yaml\Yaml;
use Drupal\Component\Utility\UrlHelper;
use BitWasp\BitcoinLib\BitcoinLib;
use BitWasp\BitcoinLib\BIP32;

class CoinTools {

  /**
   * Converts a bitcoin amount to a satoshi amount.
   *
   * @param float $amount
   *   A bitcoin amount.
   *
   * @return integer
   *   A satoshi amount.
   *
   * @see https://en.bitcoin.it/wiki/Proper_Money_Handling_%28JSON-RPC%29
   */
  public static function bitcoinToSatoshi($amount) {
    return (int) round($amount * 1e8);
  }

  /**
   * Converts a satoshi amount to a bitcoin amount.
   *
   * @param integer $amount
   *   A satoshi amount.
   *
   * @return float
   *   A bitcoin amount.
   *
   * @see https://en.bitcoin.it/wiki/Proper_Money_Handling_%28JSON-RPC%29
   */
  public static function satoshiToBitcoin($amount) {
    return $amount / 1e8;
  }

  public static function coins() {
    $yaml = file_get_contents(composer_manager_vendor_dir() . '/bluedroplet/coins/coins.yaml');
    return Yaml::parse($yaml, TRUE);
  }

  public static function testnet() {
    return variable_get('cointools.testnet', 0);
  }

  public static function prefixes($coin = 'BTC') {
    $coins = self::coins();
    return $coins[$coin]['base58_prefixes'][self::testnet() ? 'testnet' : 'mainnet'];
  }

  public static function validateAddress($address) {
    // Check the address is decoded correctly.
    $decode = BitcoinLib::base58_decode($address);
    if (strlen($decode) !== 50) {
      return FALSE;
    }
    // Compare the checksums.
    if (substr($decode, -8) != substr(BitcoinLib::hash256(substr($decode, 0, 42)), 0, 8)) {
      return FALSE;
    }
    // Compare the version.
    $version = hexdec(substr($decode, 0, 2));
    $prefixes = self::prefixes();
    if (!isset($prefixes['p2pkh']) || $version != $prefixes['p2pkh']) {
      if (!isset($prefixes['p2sh']) || $version != $prefixes['p2sh']) {
        return FALSE;
      }
    }
    return TRUE;
  }

  public static function interrogateAddress($address) {
    if (!self::validateAddress($address)) {
      return;
    }
    $address_prefix = hexdec(substr(BitcoinLib::base58_decode($address), 0, 2));
    $coins = self::coins();
    $networks = $coins['BTC']['base58_prefixes'];

    foreach ($network as $network => $types) {
      foreach ($types as $type => $type_prefix) {
        if ($address_prefix == $type_prefix) {
          return [
            'testnet' => $network == 'testnet',
            'type' => $type,
          ];
        }
      }
    }
  }

  public static function bitcoinUri(array $components) {
    $uri = 'bitcoin:';
    if (isset($components['address'])) {
      $uri .= $components['address'];
      unset($components['address']);
    }
    if (isset($components['amount'])) {
      $components['amount'] = self::satoshiToBitcoin($components['amount']);
    }
    // Remove any empty components.
    foreach ($components as $key => $value) {
      if (!$value) {
        unset($components[$key]);
      }
    }
    if (count($components)) {
      $uri .= '?' . UrlHelper::buildQuery($components);
    }
    return $uri;
  }

  public static function getHdAddress($xpub, $i = NULL) {
    if (!isset($i)) {
      // Get the current index to use for this xpub.
      $i = \Drupal::state()->get('cointools.xpubs.' . $xpub, 0);
      // Store the next index.
      \Drupal::state()->set('cointools.xpubs.' . $xpub, $i + 1);
    }
    // Calculate the address.
    $extended_key = BIP32::build_key($xpub, '0/' . $i);
    $import = BIP32::import($extended_key[0]);
    if ($import['type'] == 'public') {
      $public = $import['key'];
    }
    elseif ($import['type'] == 'private') {
      $public = BitcoinLib::private_key_to_public_key($import['key'], TRUE);
    }
    else {
      return FALSE;
    }
    $prefixes = self::prefixes();
    $prefix = str_pad(dechex($prefixes['p2pkh']), 2, '0');
    return BitcoinLib::public_key_to_address($public, $prefix);
  }

}
