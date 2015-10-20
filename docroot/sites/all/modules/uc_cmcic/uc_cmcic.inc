<?php

/**
 * @file uc_cmcic.inc
 * Written by Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
 * http://www.absyx.fr
 *
 * This file is the API-like part of the module.
 * It is separated from the module file so that it can be imported from another module, even without Ubercart.
 * So if you want to use these utilities in a custom module, just add:
 *   require_once drupal_get_path('module', 'uc_cmcic') .'/uc_cmcic.inc;
 * at the beginning of your module.
 */



function uc_cmcic_complete_request($key, $fields) {
  $fields = array_merge(array(
    'version' => '3.0',
    'texte-libre' => '',
    'lgue' => 'EN',
  ), $fields);

  $ordered_fields = array();
  foreach (array(
    'TPE',
    'date',
    'montant',
    'reference',
    'texte-libre',
    'version',
    'lgue',
    'societe',
    'mail',
    'nbrech',
    'dateech1',
    'montantech1',
    'dateech2',
    'montantech2',
    'dateech3',
    'montantech3',
    'dateech4',
    'montantech4',
    'options',
  ) as $name) {
    $ordered_fields[$name] = isset($fields[$name]) ? $fields[$name] : '';
  }
  $fields['MAC'] = uc_cmcic_hmac_sha1(uc_cmcic_usable_key($key), implode('*', $ordered_fields));

  return $fields;
}



function uc_cmcic_validate_response($key, $fields) {
  if (empty($fields['MAC'])) {
    return FALSE;
  }

  $ordered_fields = array();
  foreach (array(
    'TPE',
    'date',
    'montant',
    'reference',
    'texte-libre',
    'version',
    'code-retour',
    'cvx',
    'vld',
    'brand',
    'status3ds',
    'numauto',
    'motifrefus',
    'originecb',
    'bincb',
    'hpancb',
    'ipclient',
    'originetr',
    'veres',
    'pares',
  ) as $name) {
    $ordered_fields[$name] = isset($fields[$name]) ? $fields[$name] : '';
  }
  $ordered_fields['version'] = '3.0';
  $ordered_fields[] = '';

  $mac = uc_cmcic_hmac_sha1(uc_cmcic_usable_key($key), implode('*', $ordered_fields));
  return (strtolower($mac) == strtolower($fields['MAC']));
}



function uc_cmcic_receipt($mac_ok) {
  return sprintf("version=2\ncdr=%s\n", ($mac_ok) ? '0' : '1');
}



function uc_cmcic_usable_key($key) {
  if (strlen($key) != 20) {
    $suffix = ''. substr($key, 38, 2) .'00';
    $key = substr($key, 0, 38);
    $cca0 = ord($suffix);
    if (($cca0 > 70) && ($cca0 < 97)) {
      $key .= chr($cca0 - 23) . substr($suffix, 1, 1);
    }
    elseif (substr($suffix, 1, 1) == 'M') {
      $key .= substr($suffix, 0, 1) .'0';
    }
    else {
      $key .= substr($suffix, 0, 2);
    }
    $key = pack('H*', $key);
  }
  return $key;
}



/**
 * RFC 2104 HMAC implementation for PHP >= 4.3.0 - Creates a SHA1 HMAC.
 * Eliminates the need to install mhash to compute a HMAC.
 * Adjusted from the md5 version by Lance Rushing.
 */
function uc_cmcic_hmac_sha1($key, $data) {
  $length = 64; // block length for SHA1
  if (strlen($key) > $length) {
    $key = pack('H*', sha1($key));
  }
  $key  = str_pad($key, $length, chr(0x00));
  $ipad = str_pad('', $length, chr(0x36));
  $opad = str_pad('', $length, chr(0x5c));
  $k_ipad = $key ^ $ipad;
  $k_opad = $key ^ $opad;
  return sha1($k_opad . pack('H*', sha1($k_ipad . $data)));
}



function uc_cmcic_languages() {
  return array('FR', 'EN', 'DE', 'IT', 'ES', 'NL');
}
