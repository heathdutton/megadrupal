<?php

/**
 * @file
 *  vat_number.inc
 *
 * Validating a VAT by structure
 *
 * VAT structure
 *
 * http://ec.europa.eu/taxation_customs/vies/faq.html  |
 *   official eu page, chosen for inline docs.
 * http://en.wikipedia.org/wiki/VAT_identification_number
 * http://www.advsofteng.com/vatid.html
 * http://pages.ebay.co.uk/help/account/vat-id.html
 * http://www.hmrc.gov.uk/vat/managing/international/esl/country-codes.htm
 *
 * Countries
 *
 * http://europa.eu/about-eu/countries/index_de.htm
 */

/*******************************************************************************
 *                        HELPER FUNCTIONS for EU VAT processing, testing
 ******************************************************************************/

/**
 * Pre check, to output good error messages that can help the user.
 *
 * @param integer $vat_number
 *   VAT number to test e.g. GB123 4567 89
 *
 * @return array
 *   with vat values
 *      county_code, for example DE ES FR
 *      vat_number, the rest after the country code
 *      vat_format, 0 meaning error, > 1 means format valid
 *
 */
function _vat_number_check_vat_format($vat_number) {

  $vat_infos = array();
  $vat_infos = _vat_number_components($vat_number);

  // Check for a country code.
  switch ($vat_infos['country_code']) {

    /* info on the regexes in general. /^(AT){0,1}U[0-9]{8}$/i'; --- we use {0,1} because eventually we want to provide the country later by a second field formatter so tahts no error, but the function           needs to be rewriten for that*/

    // Austria.
    case 'AT':
      // ATU99999999	1 block of 9 characters.
      $example['AT'] = "ATU99999999	,AT + 9 characters, the first position following the prefix is always an U";
      $regex = '/^(AT){0,1}U[0-9]{8}$/i';
      break;

    // Belgium.
    case 'BE':
      // BE0999999999	1 block of 10 digits.
      $example['BE'] = "BE0999999999 ,BE + 10 digits, the first digit following the prefix is always zero (0)";
      $regex = '/^(BE){0,1}[0]{1}[0-9]{9}$/i';
      break;

    // Bukgaria.
    case 'BG':
      // BG999999999 or BG9999999999
      // 1 block of 9 digits or 1 block of 10 digits.
      $example['BG'] = "BG999999999 or BG9999999999 ,BG + 9 or 10 digits";
      $regex = '/^(BG){0,1}[0-9]{9,10}$/i';
      break;

    // Cyprus.
    case 'CY':
      // CY99999999L	1 block of 9 characters, ATTENTION L:	A letter.
      $example['CY'] = "CY99999999L	,CY + 9 characters";
      $regex = '/^(CY){0,1}[0-9]{8}[A-Z]{1}$/i';
      break;

    // Czech Republic.
    case 'CZ':
      // CZ99999999 or CZ999999999 or CZ9999999999
      // 1 block of either 8, 9 or 10 digits.
      $example['CZ'] = "CZ99999999 or CZ999999999 or CZ9999999999	, CZ + 8, 9 or 10 digits";
      $regex = '/^(CZ){0,1}[0-9]{8,10}$/i';
      break;

    // Denmark.
    case 'DK':
      // DK99 99 99 99 4 blocks of 2 digits.
      $example['DK'] = "DK99 99 99 99	,4 blocks of 2 digits";
      $regex = '/^(DK){0,1}([0-9]){8}$/i';
      break;

    // Estonia.
    case 'EE':
      // EE999999999 block of 9 digits.
      $example['EE'] = "EE999999999	1 block of 9 digits";
      $regex = '/^(EE){0,1}[0-9]{9}$/i';
      break;

    // Germany.
    case 'DE':
      // DE999999999	1 block of 9 digits.
      $example['DE'] = "DE999999999	,1 block of 9 digits";
      $regex = '/^(DE){0,1}[0-9]{9}$/i';
      break;

    // Greece.
    case 'EL':
      // EL999999999	1 block of 9 digits.
      $example['EL'] = "EL999999999	,1 block of 9 digits";
      $regex = '/^(EL){0,1}[0-9]{9}$/i';
      break;

    // Protugal.
    case 'PT':
      // PT999999999	1 block of 9 digits.
      $example['PT'] = "PT999999999	,1 block of 9 digits";
      $regex = '/^(PT){0,1}[0-9]{9}$/i';
      break;

    // France.
    case 'FR':
      // FRXX 999999999	1 block of 2 characters, 1 block of 9 digits,
      // X:	A letter or a digit.
      $example['FR'] = "FRXX 999999999	,1 block of 2 characters, 1 block of 9 digits";
      $regex = '/^(FR){0,1}[0-9A-Z]{2}[0-9]{9}$/i';
      break;

    // Finland.
    case 'FI':
      // FI99999999	1 block of 8 digits.
      $example['FI'] = "FI99999999	,1 block of 8 digits  ";
      $regex = '/^(FI){0,1}[0-9]{8}$/i';
      break;

    // Croatia.
    case 'HR':
      // HU12345678901	1 block of 11 digits.
      $example['HU'] = "HU12345678901	,1 block of 8 digits";
      $regex = '/^(HR){0,1}[0-9]{11}$/i';
      break;

    // Hungary.
    case 'HU':
      // HU99999999	1 block of 8 digits.
      $example['HU'] = "HU99999999	,1 block of 8 digits";
      $regex = '/^(HU){0,1}[0-9]{8}$/i';
      break;

    // Luxembourg.
    case 'LU':
      // LU99999999	1 block of 8 digits.
      $example['LU'] = "LU99999999	,1 block of 8 digits";
      $regex = '/^(LU){0,1}[0-9]{8}$/i';
      break;

    // Malta.
    case 'MT':
      // MT99999999	1 block of 8 digits.
      $example['MT'] = "MT99999999	,1 block of 8 digits";
      $regex = '/^(MT){0,1}[0-9]{8}$/i';
      break;

    // Slovenia.
    case 'SI':
      // SI12345678 (8 characters).
      $example['SI'] = "SI12345678 (8 characters)";
      $regex = '/^(SI){0,1}[0-9]{8}$/i';
      break;

    // Ireland.
    case 'IE':
      // IE9S99999L	1 block of 8 characters,
      // S:	A letter; a digit; "+" or "*"  AND L:	A letter.
      $example['IE'] = "IE9S99999L	,1 block of 8 characters";
      $regex = '/^(IE){0,1}[0-9][0-9A-Z\+\*][0-9]{5}[A-Z]$/i';
      break;

    // Italy.
    case 'IT':
      // IT99999999999	1 block of 11 digits.
      $example['IT'] = "IT99999999999	1 block of 11 digits";
      $regex = '/^(IT){0,1}[0-9]{11}$/i';
      break;

    // Latvia.
    case 'LV':
      // LV99999999999	1 block of 11 digits.
      $example['LV'] = "LV99999999999	1 block of 11 digits";
      $regex = '/^(LV){0,1}[0-9]{11}$/i';
      break;

    // Lithuania.
    case 'LT':
      // LT999999999 or LT999999999999
      // 1 block of 9 digits, or 1 block of 12 digits.
      $example['LT'] = "LT999999999 or LT999999999999 ,1 block of 9 digits, or 1 block of 12 digits";
      $regex = '/^(LT){0,1}([0-9]{9}|[0-9]{12})$/i';
      break;

    // Netherlands.
    case 'NL':
      // NL999999999B99	,1 block of 12 characters,
      // The 10th position following the prefix is always "B".
      $example['NL'] = "NL999999999B99	,1 block of 12 characters";
      $regex = '/^(NL){0,1}[0-9]{9}B[0-9]{2}$/i';
      break;

    // Poland.
    case 'PL':
      // PL9999999999	1 block of 10 digits.
      $example['PL'] = "PL9999999999	,1 block of 10 digits";
      $regex = '/^(PL){0,1}[0-9]{10}$/i';
      break;

    // Slovakia.
    case 'SK':
      // SK9999999999	1 block of 10 digits.
      $example['SK'] = "SK9999999999	1 block of 10 digits";
      $regex = '/^(SK){0,1}[0-9]{10}$/i';
      break;

    // Romania.
    case 'RO':
      // RO999999999	1 block of minimum 2 digits and maximum 10 digits.
      $example['RO'] = "RO999999999	,1 block of minimum 2 digits and maximum 10 digits";
      $regex = '/^(RO){0,1}[0-9]{2,10}$/i';
      break;

    // Sweden.
    case 'SE':
      // SE999999999999	1 block of 12 digits.
      $example['SE'] = "SE999999999999	,1 block of 12 digits";
      $regex = '/^(SE){0,1}[0-9]{12}$/i';
      break;

    // Spain.
    case 'ES':
      // ESX9999999X	1 block of 9 characters,
      // ATTENTION X:	A letter or a digit,
      // The first and last characters may be alpha or numeric;
      // but they may not both be numeric.
      $example['ES'] = "ESX9999999X	,1 block of 9 characters";
      $regex = '/^(ES){0,1}([0-9A-Z][0-9]{7}[A-Z])|([A-Z][0-9]{7}[0-9A-Z])$/i';
      break;

    // United Kingdom.
    case 'GB':
      // GB999 9999 99      standard:
      // - 9 digits (block of 3, block of 4, block of 2).
      // GB999 9999 99 999  branch traders: 12 digits
      // - (as for 9 digits, followed by a block of 3 digits).
      // GBGD999            government departments:
      // - the letters GD then 3 digits orom 000 to 499 (e.g. GBGD001).
      // GBHA999            health authorities:
      // - the letters HA then 3 digits from 500 to 999 (e.g. GBHA599).
      $example['GB'] = "GB999 9999 99 ,9 digits (block of 3, block of 4, block of 2)";
      $regex = '/^(GB){0,1}(([0-9]{9})|([0-9]{12})|((GD|HA)[0-9]{3}))$/i';
      break;

    default:
      // No valid country code, return all invalid data.
      $vat_infos['vat_format'] = 0;
      $vat_infos['message'] = t("The country of the vat can not be detected. Please do not remove the language Prefix. Example: DE123456789 (where DE is the country prefix)");
      return $vat_infos;
      break;
  }

  // OK now check if the regex matched the supplied vat.
  $vat_number = $vat_infos['country_code'] . $vat_infos['vat_number'];
  $vat_infos['vat_format'] = preg_match($regex, $vat_number);

  // Output a message with an info about wrong frong format if a
  // country code was in front but regex validation does not match
  // in case the user added too many numbers or forgot something else.
  // Help hi mand display a message about formatting information.
  $eu_countries = _vat_number_eu_countries();

  // Valid eu country code.
  $valid_eu = isset($eu_countries[$vat_infos['country_code']]);
  if ($valid_eu && !$vat_infos['vat_format']) {
    $vat_infos['message'] = t('You input an %country VAT, but it is not a vaild format. <br> It must have the following format, example: <b>%format</b>',
     array(
       '%country' => $eu_countries[$vat_infos['country_code']],
       '%format' => $example[$vat_infos['country_code']],
     )
    );
  }

  return $vat_infos;

}

/**
 * A list of valid countrys of the EU
 *
 * @return array
 *   a list of eu countrys, key is country code, value is readable name
 */
function _vat_number_eu_countries() {

  // Necessary for country_get_list().
  require_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();

  // ISO 3166.
  $eu_country_codes = array(
    "AT",
    "BE",
    "BG",
    "CY",
    "CZ",
    "DE",
    "DK",
    "EE",
    "ES",
    "FI",
    "FR",
    "GB",
    "GR",
    "HR",
    "HU",
    "IE",
    "IT",
    "LT",
    "LU",
    "LV",
    "MT",
    "NL",
    "PL",
    "PT",
    "RO",
    "SE",
    "SI",
    "SK",
  );

  // Merge in country names from country_get_list().
  foreach ($eu_country_codes as $key => $value) {
    $eu_countries[$value] = $countries[$value];
  }
  return $eu_countries;
}


/**
 * Check a VAT number. Return error message on error, FALSE on success.
 */
function _vat_number_validate_vat($vat_number, $skip_validation_on_service_failure = FALSE) {
  $vat_infos = _vat_number_components($vat_number);
  $client = _vat_number_connect_vies_database();
  if ($client) {
    $params = array('countryCode' => $vat_infos['country_code'], 'vatNumber' => $vat_infos['vat_number']);
    try{
      $r = $client->checkVat($params);
      return ($r->valid == TRUE) ? TRUE : FALSE;
    } catch(SoapFault $e) {
      watchdog('vat_number', $e->faultstring, NULL, WATCHDOG_ERROR);
      if ($skip_validation_on_service_failure) {
        // Something is wrong, but that's fine. Pass the validation..
        return TRUE;
      }
    }
  }
  elseif ($skip_validation_on_service_failure) {
    // We couldn't connect but we are allowed to pass the validation.
    return TRUE;
  }
  else {
    // No $client and can't skip.
    drupal_set_message(t('The connection to the European VAT database server failed and the VAT could not be validated, please contact us about this problem.'), 'error');
    return FALSE;
  }
}

/**
 * Try to connect with the VIES database. Catch PHP warnings if connection with
 * the host is not possible.
 */
function _vat_number_connect_vies_database() {
  try {
    return @new SoapClient("http://ec.europa.eu/taxation_customs/vies/checkVatService.wsdl", array("exceptions" => 1));
  } catch (SoapFault $e) {
    // Connection to host not possible, europe.eu down?
    watchdog('vat_number', $e->faultstring, NULL, WATCHDOG_ERROR);
  }
}

/**
 * Splitting up a VAT to country prefix and number and clean it from spaces etc.
 */
function _vat_number_components($vat_number) {

  // Some countries like DK use spaces in the formatting.
  // Maybe someone does that too for readability or uses dots.
  // We remove all dots, spaces and dashes because they are not
  // important for the validation operations and we do NOT regex spaces.
  $vatid = preg_replace('/[ .-]/', '', $vat_number);

  // First two letters are always country code.
  $vat_infos['country_code'] = drupal_strtoupper(drupal_substr($vatid, 0, 2));
  $vat_infos['vat_number'] = drupal_strtoupper(drupal_substr($vatid, 2));

  return $vat_infos;
}
