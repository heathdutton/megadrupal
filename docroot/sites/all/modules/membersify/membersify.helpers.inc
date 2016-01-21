<?php

/**
 * Formats a price string.
 *
 * @param array $payment_plan
 *   The payment plan to get the string for.
 * @param bool $adjustment
 *   (optional) Whether or not this is an adjustment. Defaults to FALSE.
 *
 * @return string
 *   The formatted string for payment plan..
 */
function membersify_get_price_string($payment_plan, $adjustment = FALSE) {
  $schedule = (object) $payment_plan;
  $currency = !empty($schedule->currency) ? $schedule->currency : variable_get('membersify_default_currency', 'USD');

  if (!empty($schedule->qty)) {
    $schedule->main_length *= $schedule->qty;
  }

  // Set the amount to 0 if it is negative, unless this is an adjustment.
  if (!$adjustment) {
    if ($schedule->main_amount < 0) {
      $schedule->main_amount = 0;
    }
    if (empty($schedule->trial_amount) || $schedule->trial_amount < 0) {
      $schedule->trial_amount = 0;
    }
  }

  if ($schedule->recurring) {
    if ($schedule->has_trial) {
      if (($schedule->total_occurrences > 1)) {
        if (!empty($schedule->fixed_date)) {
          return t('!trial_amount for now until !date, then !main_amount !every_period for !time_length',
            array(
              '!trial_amount' => ($schedule->trial_amount != 0) ? membersify_format_money($schedule->trial_amount, $currency) : t('Free'),
              '!main_amount' => ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free'),
              '!every_period' => membersify_format_unit($schedule->main_unit, $schedule->main_length, TRUE),
              '!time_length' => membersify_format_unit($schedule->main_unit, $schedule->total_occurrences),
              '!date' => membersify_format_next_fixed_date($schedule->fixed_date_string, $schedule->fixed_date_type),
            )
          );
        }
        else {
          return t('!trial_amount for !trial_period, then !main_amount !every_period for !time_length',
            array(
              '!trial_amount' => ($schedule->trial_amount != 0) ? membersify_format_money($schedule->trial_amount, $currency) : t('Free'),
              '!trial_period' => membersify_format_unit($schedule->trial_unit, $schedule->trial_length),
              '!main_amount' => ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free'),
              '!every_period' => membersify_format_unit($schedule->main_unit, $schedule->main_length, TRUE),
              '!time_length' => membersify_format_unit($schedule->main_unit, $schedule->total_occurrences),
            )
          );
        }
      }
      else {
        if (!empty($schedule->fixed_date)) {
          return t('!trial_amount for now until !date, then !main_amount !every_period',
            array(
              '!trial_amount' => ($schedule->trial_amount != 0) ? membersify_format_money($schedule->trial_amount, $currency) : t('Free'),
              '!main_amount' => ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free'),
              '!every_period' => membersify_format_unit($schedule->main_unit, $schedule->main_length, TRUE),
              '!date' => membersify_format_next_fixed_date($schedule->fixed_date_string, $schedule->fixed_date_type),
            )
          );
        }
        else {
          return t('!trial_amount for !trial_period, then !main_amount !every_period',
            array(
              '!trial_amount' => ($schedule->trial_amount != 0) ? membersify_format_money($schedule->trial_amount, $currency) : t('Free'),
              '!trial_period' => membersify_format_unit($schedule->trial_unit, $schedule->trial_length),
              '!main_amount' => ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free'),
              '!every_period' => membersify_format_unit($schedule->main_unit, $schedule->main_length, TRUE),
            )
          );
        }
      }
    }
    else {
      if (($schedule->total_occurrences > 1)) {
        if (!empty($schedule->fixed_date)) {
          return t('!main_amount for now until !date, then !main_amount !every_period for !time_length',
            array(
              '!main_amount' => ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free'),
              '!every_period' => membersify_format_unit($schedule->main_unit, $schedule->main_length, TRUE),
              '!time_length' => membersify_format_unit($schedule->main_unit, $schedule->total_occurrences),
              '!date' => membersify_format_next_fixed_date($schedule->fixed_date_string, $schedule->fixed_date_type),
            )
          );
        }
        else {
          return t('!main_amount !every_period for !time_length',
            array(
              '!main_amount' => ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free'),
              '!every_period' => membersify_format_unit($schedule->main_unit, $schedule->main_length, TRUE),
              '!time_length' => membersify_format_unit($schedule->main_unit, $schedule->total_occurrences),
            )
          );
        }
      }
      else {
        if (!empty($schedule->fixed_date)) {
          return t('!main_amount for now until !date, then !main_amount !every_period',
            array(
              '!main_amount' => ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free'),
              '!every_period' => membersify_format_unit($schedule->main_unit, $schedule->main_length, TRUE),
              '!date' => membersify_format_next_fixed_date($schedule->fixed_date_string, $schedule->fixed_date_type),
            )
          );
        }
        else {
          return t('!main_amount !every_period',
            array(
              '!main_amount' => ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free'),
              '!every_period' => ($schedule->main_amount != 0) ? membersify_format_unit($schedule->main_unit, $schedule->main_length, TRUE) : '',
            )
          );
        }
      }
    }
  }
  else {
    if (!empty($schedule->fixed_date)) {
      $amount = ($schedule->trial_amount != 0) ? $schedule->trial_amount : $schedule->main_amount;
      return t('!main_amount for now until !date',
        array(
          '!main_amount' => ($amount != 0) ? membersify_format_money($amount, $currency) : t('Free'),
          '!date' => membersify_format_next_fixed_date($schedule->fixed_date_string, $schedule->fixed_date_type),
        )
      );
    }
    else {
      if (!empty($schedule->main_length)) {
        return t('!main_amount for !main_period',
          array(
            '!main_amount' => ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free'),
            '!main_period' => membersify_format_unit($schedule->main_unit, $schedule->main_length),
          )
        );
      }
      else {
        return ($schedule->main_amount != 0) ? membersify_format_money($schedule->main_amount, $currency) : t('Free');
      }
    }
  }
}

/**
 * Gets the configuration area for all currencies.
 */
function membersify_get_currencies_config() {
  $default = array(
    'AED' => array('name' => t('United Arab Emirates Dirham')),
    'AFN' => array('name' => t('Afghan Afghani')),
    'ALL' => array('name' => t('Albanian Lek')),
    'AMD' => array('name' => t('Armenian Dram')),
    'ANG' => array('name' => t('Netherlands Antillean Gulden')),
    'AOA' => array('name' => t('Angolan Kwanza')),
    'ARS' => array('name' => t('Argentine Peso')),
    'AUD' => array('name' => t('Australian Dollar'), 'sign' => '$',),
    'AWG' => array('name' => t('Aruban Florin')),
    'AZN' => array('name' => t('Azerbaijani Manat')),
    'BAM' => array('name' => t('Bosnia & Herzegovina Convertible Mark')),
    'BBD' => array('name' => t('Barbadian Dollar'), 'sign' => '$',),
    'BDT' => array('name' => t('Bangladeshi Taka')),
    'BGN' => array('name' => t('Bulgarian Lev')),
    'BIF' => array('name' => t('Burundian Franc')),
    'BMD' => array('name' => t('Bermudian Dollar'), 'sign' => '$',),
    'BND' => array('name' => t('Brunei Dollar'), 'sign' => '$',),
    'BOB' => array('name' => t('Bolivian Boliviano')),
    'BRL' => array('name' => t('Brazilian Real'), 'sign' => 'R$',),
    'BSD' => array('name' => t('Bahamian Dollar'), 'sign' => '$',),
    'BWP' => array('name' => t('Botswana Pula')),
    'BZD' => array('name' => t('Belize Dollar'), 'sign' => '$',),
    'CAD' => array('name' => t('Canadian Dollar'), 'sign' => '$',),
    'CDF' => array('name' => t('Congolese Franc')),
    'CFA' => array('name' => t('CFA franc'), 'sign' => 'CFA francs',),
    'CHF' => array('name' => t('Swiss Franc'), 'sign' => 'francs',),
    'CLP' => array('name' => t('Chilean Peso')),
    'CNY' => array('name' => t('Chinese Renminbi Yuan')),
    'COP' => array('name' => t('Colombian Peso')),
    'CRC' => array('name' => t('Costa Rican Colón')),
    'CVE' => array('name' => t('Cape Verdean Escudo')),
    'CZK' => array('name' => t('Czech Koruna'), 'sign' => 'Kc',),
    'DJF' => array('name' => t('Djiboutian Franc')),
    'DKK' => array('name' => t('Danish Krone'), 'sign' => 'kr',),
    'DOP' => array('name' => t('Dominican Peso')),
    'DZD' => array('name' => t('Algerian Dinar')),
    'EEK' => array('name' => t('Estonian Kroon')),
    'EGP' => array('name' => t('Egyptian Pound')),
    'ETB' => array('name' => t('Ethiopian Birr')),
    'EUR' => array('name' => t('Euro'), 'sign' => '€',),
    'FJD' => array('name' => t('Fijian Dollar'), 'sign' => '$',),
    'FKP' => array('name' => t('Falkland Islands Pound')),
    'GBP' => array('name' => t('British Pound'), 'sign' => '£',),
    'GEL' => array('name' => t('Georgian Lari')),
    'GIP' => array('name' => t('Gibraltar Pound')),
    'GMD' => array('name' => t('Gambian Dalasi')),
    'GNF' => array('name' => t('Guinean Franc')),
    'GTQ' => array('name' => t('Guatemalan Quetzal')),
    'GYD' => array('name' => t('Guyanese Dollar'), 'sign' => '$',),
    'HKD' => array('name' => t('Hong Kong Dollar'), 'sign' => '$',),
    'HNL' => array('name' => t('Honduran Lempira')),
    'HRK' => array('name' => t('Croatian Kuna')),
    'HTG' => array('name' => t('Haitian Gourde')),
    'HUF' => array('name' => t('Hungarian Forint'), 'sign' => 'forint',),
    'IDR' => array('name' => t('Indonesian Rupiah')),
    'ILS' => array('name' => t('Israeli New Sheqel'), 'sign' => 'new shekalim',),
    'INR' => array('name' => t('Indian Rupee'), 'sign' => 'INR',),
    'ISK' => array('name' => t('Icelandic Króna')),
    'JMD' => array('name' => t('Jamaican Dollar'), 'sign' => '$',),
    'JPY' => array('name' => t('Japanese Yen'), 'sign' => '¥',),
    'KES' => array('name' => t('Kenyan Shilling')),
    'KGS' => array('name' => t('Kyrgyzstani Som')),
    'KHR' => array('name' => t('Cambodian Riel')),
    'KMF' => array('name' => t('Comorian Franc')),
    'KRW' => array('name' => t('South Korean Won'), 'sign' => '₩',),
    'KYD' => array('name' => t('Cayman Islands Dollar'), 'sign' => '$',),
    'KZT' => array('name' => t('Kazakhstani Tenge')),
    'LAK' => array('name' => t('Lao Kip')),
    'LBP' => array('name' => t('Lebanese Pound')),
    'LKR' => array('name' => t('Sri Lankan Rupee')),
    'LRD' => array('name' => t('Liberian Dollar'), 'sign' => '$',),
    'LSL' => array('name' => t('Lesotho Loti')),
    'LTL' => array('name' => t('Lithuanian Litas')),
    'LVL' => array('name' => t('Latvian Lats')),
    'MAD' => array('name' => t('Moroccan Dirham')),
    'MDL' => array('name' => t('Moldovan Leu')),
    'MGA' => array('name' => t('Malagasy Ariary')),
    'MKD' => array('name' => t('Macedonian Denar')),
    'MNT' => array('name' => t('Mongolian Tögrög')),
    'MOP' => array('name' => t('Macanese Pataca')),
    'MRO' => array('name' => t('Mauritanian Ouguiya')),
    'MUR' => array('name' => t('Mauritian Rupee')),
    'MVR' => array('name' => t('Maldivian Rufiyaa')),
    'MWK' => array('name' => t('Malawian Kwacha')),
    'MXN' => array('name' => t('Mexican Peso'), 'sign' => '$',),
    'MYR' => array('name' => t('Malaysian Ringgit'), 'sign' => 'RM',),
    'MZN' => array('name' => t('Mozambican Metical')),
    'NAD' => array('name' => t('Namibian Dollar'), 'sign' => '$',),
    'NGN' => array('name' => t('Nigerian Naira'), 'sign' => '₦',),
    'NIO' => array('name' => t('Nicaraguan Córdoba')),
    'NOK' => array('name' => t('Norwegian Krone'), 'sign' => 'kr',),
    'NPR' => array('name' => t('Nepalese Rupee')),
    'NZD' => array('name' => t('New Zealand Dollar'), 'sign' => '$',),
    'PAB' => array('name' => t('Panamanian Balboa')),
    'PEN' => array('name' => t('Peruvian Nuevo Sol')),
    'PGK' => array('name' => t('Papua New Guinean Kina')),
    'PHP' => array('name' => t('Philippine Peso'), 'sign' => 'piso',),
    'PKR' => array('name' => t('Pakistani Rupee')),
    'PLN' => array('name' => t('Polish Złoty'), 'sign' => 'zl',),
    'PYG' => array('name' => t('Paraguayan Guaraní')),
    'QAR' => array('name' => t('Qatari Riyal')),
    'RON' => array('name' => t('Romanian Leu')),
    'RSD' => array('name' => t('Serbian Dinar')),
    'RUB' => array('name' => t('Russian Ruble')),
    'RWF' => array('name' => t('Rwandan Franc')),
    'SAR' => array('name' => t('Saudi Riyal')),
    'SBD' => array('name' => t('Solomon Islands Dollar'), 'sign' => '$',),
    'SCR' => array('name' => t('Seychellois Rupee')),
    'SEK' => array('name' => t('Swedish Krona'), 'sign' => 'kr',),
    'SGD' => array('name' => t('Singapore Dollar'), 'sign' => '$',),
    'SHP' => array('name' => t('Saint Helenian Pound')),
    'SLL' => array('name' => t('Sierra Leonean Leone')),
    'SOS' => array('name' => t('Somali Shilling')),
    'SRD' => array('name' => t('Surinamese Dollar'), 'sign' => '$',),
    'STD' => array('name' => t('São Tomé and Príncipe Dobra')),
    'SVC' => array('name' => t('Salvadoran Colón')),
    'SZL' => array('name' => t('Swazi Lilangeni')),
    'THB' => array('name' => t('Thai Baht'), 'sign' => '?',),
    'TJS' => array('name' => t('Tajikistani Somoni')),
    'TOP' => array('name' => t('Tongan Paʻanga')),
    'TRY' => array('name' => t('Turkish Lira')),
    'TTD' => array('name' => t('Trinidad and Tobago Dollar'), 'sign' => '$',),
    'TWD' => array('name' => t('New Taiwan Dollar'), 'sign' => '$',),
    'TZS' => array('name' => t('Tanzanian Shilling')),
    'UAH' => array('name' => t('Ukrainian Hryvnia')),
    'UGX' => array('name' => t('Ugandan Shilling')),
    'USD' => array('name' => t('United States Dollar'), 'sign' => '$',),
    'UYI' => array('name' => t('Uruguayan Peso')),
    'UZS' => array('name' => t('Uzbekistani Som')),
    'VEF' => array('name' => t('Venezuelan Bolívar')),
    'VND' => array('name' => t('Vietnamese Đồng')),
    'VUV' => array('name' => t('Vanuatu Vatu')),
    'WST' => array('name' => t('Samoan Tala')),
    'XAF' => array('name' => t('Central African Cfa Franc')),
    'XCD' => array('name' => t('East Caribbean Dollar'), 'sign' => '$',),
    'XOF' => array('name' => t('West African Cfa Franc')),
    'XPF' => array('name' => t('Cfp Franc')),
    'YER' => array('name' => t('Yemeni Rial')),
    'ZAR' => array('name' => t('South African Rand'), 'sign' => 'R ',),
    'ZMW' => array('name' => t('Zambian Kwacha')),
  );

  $default_values = array(
    'name' => '',
    'sign' => '',
    'format' => variable_get('membersify_format_money_template', '[sign][number].[decimal]'),
    'decimal' => '.',
    'thousands' => ',',
  );

  foreach ($default as &$info) {
    if (!is_array($info)) {
      $info = array('name' => $info);
    }
    $info = array_merge($default_values, $info);
  }

  return $default;
}

/**
 * Formats a money string based on currency and amount.
 *
 * @param float|int $amount
 *   (optional) The amount to format. Defaults to 0.
 * @param string $currency_code
 *   (optional) The currency to use for the formatting. Defaults to NULL.
 * @param string $context
 *   (optional) The context of the display. Defauls to 'HTML'.
 *
 * @return string
 *   The formatted string for the money.
 *
 * @ingroup membersify_api
 */
function membersify_format_money($amount = 0, $currency_code = NULL, $context = 'HTML') {
  $pos_sign = '';
  if (!$currency_code) {
    $currency_code = variable_get('membersify_default_currency', 'USD');
  }
  if ($amount < 0) {
    $pos_sign = '- ';
    $amount = abs($amount);
  }

  $currencies = membersify_get_currencies_config();
  $currency_info = array(
    'name' => '',
    'sign' => '',
    'decimal' => '.',
    'thousands' => ',',
    'format' => variable_get('membersify_format_money_template', '[sign][number].[decimal]'),
  );

  if (isset($currencies[$currency_code])) {
    $currency_info = $currencies[$currency_code];
  }

  $f_amount = number_format(floatval($amount), 2, $currency_info['decimal'], $currency_info['thousands']);

  $amount_string = explode('.', $f_amount);

  $whole = $amount_string[0];
  $decimal = $amount_string[1];

  $sign = !empty($currency_info['sign']) ? $currency_info['sign'] : $currency_code . ' ';

  $format = $pos_sign . $currency_info['format'];

  $search = array('[sign]', '[number]', '[decimal]', '[currency_code]');
  $replace = array($sign, $whole, $decimal, $currency_code);

  return str_replace($search, $replace, $format);
}

/**
 * Formats units in a plural form to faciliate translation.
 *
 * @param string $unit
 *   The unit. Can be 'day', 'week', 'month', or 'year'.
 * @param int $occurrences
 *   (optional) The number of occurrences. Defaults to 1.
 * @param bool $every
 *   (optional) Whether this is used in an 'every' context. Defaults to FALSE.
 *
 * @return string
 *   The translated string.
 */
function membersify_format_unit($unit, $occurrences = 1, $every = FALSE) {
  switch ($unit) {
    case 'hour':
      return ($every) ? format_plural($occurrences, t('per hour'), t('every @count hours')) :
        format_plural($occurrences, t('1 hour'), t('@count hours'));

    case 'day':
      return ($every) ? format_plural($occurrences, t('per day'), t('every @count days')) :
        format_plural($occurrences, t('1 day'), t('@count days'));

    case 'week':
      return ($every) ? format_plural($occurrences, t('per week'), t('every @count weeks')) :
        format_plural($occurrences, t('1 week'), t('@count weeks'));

    case 'month':
      return ($every) ? format_plural($occurrences, t('per month'), t('every @count months')) :
        format_plural($occurrences, t('1 month'), t('@count months'));

    case 'year':
      return ($every) ? format_plural($occurrences, t('per year'), t('every @count years')) :
        format_plural($occurrences, t('1 year'), t('@count years'));

    default:
      $timestamp = membersify_get_string_time($occurrences, $unit, 0);
      return ($every) ? t('every @unit', array('@unit' => format_interval($timestamp))) : format_interval($timestamp);
  }
}

/**
 * Gets a string for a date.
 */
function membersify_get_string_time($trial_length, $trial_unit) {
  switch ($trial_unit) {
    case 'hour':
      $unit = ($trial_length > 1) ? 'hours' : 'hour';
      break;
    case 'day':
      $unit = ($trial_length > 1) ? 'days' : 'day';
      break;
    case 'week':
      $unit = ($trial_length > 1) ? 'weeks' : 'week';
      break;
    case 'month':
      $unit = ($trial_length > 1) ? 'months' : 'month';
      break;
    case 'year':
      $unit = ($trial_length > 1) ? 'years' : 'year';
      break;
    default:
      $unit = $trial_unit;
      break;
  }
  return "+$trial_length $unit";
}

/**
 * Generates the text for the next fixed date payment date.
 *
 * @param string $fixed_date_string
 *   The string used to generate the next payment date.
 * @param string $fixed_date_type
 *   The fixed date type. Could be 'weekly', 'monthly', or 'yearly'.
 *
 * @return string
 *   The short date for the next payment date.
 */
function membersify_format_next_fixed_date($fixed_date_string, $fixed_date_type) {
  return format_date(membersify_calculate_next_fixed_date($fixed_date_string, $fixed_date_type), 'short');
}

/**
 * Calcuates the unix timestamp of the next payment date for fixed date schedules.
 *
 * @param string $fixed_date_string
 *   The string used to generate the next payment date.
 * @param string $fixed_date_type
 *   The fixed date type. Could be 'weekly', 'monthly', or 'yearly'.
 * @param int $last_payment
 *   (Optional) The last payment time that the calculated date should be after.
 *   Defaults to now.
 *
 * @return int
 *   The unix timestamp of the next date.
 */
function membersify_calculate_next_fixed_date($fixed_date_string, $fixed_date_type, $last_payment = 0) {
  if (empty($last_payment)) {
    $last_payment = REQUEST_TIME;
  }
  switch ($fixed_date_type) {
    case 'yearly':
      $raw_time = strtotime($fixed_date_string);
      if ($raw_time < $last_payment) {
        $raw_time = strtotime('+1 Year', $raw_time);
      }
      break;

    case 'monthly':
      $raw_time = strtotime(date('M ' . $fixed_date_string . ', Y'));
      if ($raw_time < $last_payment) {
        $raw_time = strtotime('+1 Month', $raw_time);
      }
      break;

    case 'weekly':
      $raw_time = strtotime($fixed_date_string);
      if ($raw_time < $last_payment) {
        $raw_time = strtotime('+1 Week', $raw_time);
      }
      break;

    default:
      $raw_time = strtotime($fixed_date_string);
      break;
  }

  return $raw_time;
}

/**
 * Calculates the total from items and adjustments.
 *
 * @param $items
 *   The items, which are each an associated array with the following keys:
 *     -payment_plan: An associate array with the following keys:
 *       -has_trial: Whether or not there is a trial.
 *       -trial_amount: The amount charged for trial.
 *       -main_amount: The regular amount.
 * @param Membersify_Adjustment[] $adjustments
 *   An array of adjustments to apply to the total.
 *
 * @return float
 */
function membersify_get_total($items, &$adjustments) {
  $total = 0;

  foreach ($items as $item) {
    if ($item['payment_plan']['has_trial']) {
      $total += $item['payment_plan']['trial_amount'];
    }
    else {
      $total += $item['payment_plan']['main_amount'];
    }
  }

  foreach ($adjustments as &$adjustment) {
    $adjustment->amount = membersify_get_adjusted_price($adjustment, $total);
    $total += $adjustment->amount;
  }

  return $total;
}

/**
 * Returns an Adjusted Price based on the adjustment.
 *
 * @param Membersify_Adjustment $adjustment
 *   The adjustment object to use to adjust the price.
 * @param float $price
 *   The price before the adjustment.
 *
 * @return float
 *   The adjusted price.
 */
function membersify_get_adjusted_price($adjustment, $price) {
  switch ($adjustment->type) {
    case 'percentage':
      return round($price * ($adjustment->value / 100), 2);
    case 'fixed':
      return $adjustment->value;
    default:
      return $adjustment->value;
  }
}

/**
 * Creates a list from an array of items.
 */
function membersify_list($items, $value = 'name', $key = null) {
  $list = array();
  foreach ($items as $item_key => $item) {
    if (is_array($item)) {
      $t_key = !empty($item[$key]) ? $item[$key] : $item_key;
      $list[$t_key] = $item[$value];
    }
    else {
      $t_key = !empty($item->$key) ? $item->$key : $item_key;
      $list[$t_key] = $item->$value;
    }
  }

  return $list;
}

/**
 * Saves a record given the object and the table name, while handling existing records.
 *
 * @param mixed $record
 *   The record to save.
 * @param string $table
 *   The database table.
 * @param string $field
 *   The primary key field.
 *
 * @return mixed
 *   The result of drupal_write_record() on the record.
 */
function membersify_save_record($table, $record, $field = 'id') {
  membersify_delete_record($table, $record, $field);
  drupal_write_record($table, $record);

  return $record;
}

/**
 * Deletes a record given the object and the table name.
 *
 * @param mixed $record
 *   The record to save.
 * @param string $table
 *   The database table.
 * @param string $field
 *   The primary key field.
 *
 * @return mixed
 *   The result of db_delete() on the record.
 */
function membersify_delete_record($table, $record, $field = 'id') {
  return db_delete($table)
    ->condition($field, is_array($record) ? $record[$field] : $record->$field)
    ->execute();
}

/**
 * Calculates how much credit a subscription has left.
 *
 * @param Membersify_Subscription $subscription
 *   The subscription object.
 *
 * @return
 *   The credit that is still left in a subscription.
 */
function membersify_calculate_credit(Membersify_Subscription $subscription) {
  $credit = 0;

  switch ($subscription->status) {
    case 'active':
      if ($subscription->next_payment > 0 && $subscription->next_payment > REQUEST_TIME) {
        $time_left = $subscription->next_payment - REQUEST_TIME;
        $total_time = $subscription->next_payment - $subscription->current_period_start; // FIXME - The current_period_start isn't being set properly. It is 0.
        $credit = ($time_left / $total_time) * $subscription->payment_plan['main_amount'];
      }
      break;

    case 'canceled':
    case 'expiring_soon':
      if ($subscription->expiration > 0 && $subscription->expiration > REQUEST_TIME) {
        $time_left = $subscription->expiration - REQUEST_TIME;
        $total_time = $subscription->expiration - $subscription->current_period_start;
        $credit = ($time_left / $total_time) * $subscription->payment_plan['main_amount'];
      }
      break;
  }

  return ($credit > 0) ? round($credit, 2) : 0;
}