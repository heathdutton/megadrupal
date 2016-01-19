<?php

namespace Drupal\cointools_fiat;

/**
 * @file
 * Contains information about fiat currencies.
 */

class FiatCurrencies {

  public static function currencies() {
    $currencies = [];
    $currencies['AED'] = [
      'label' => t("United Arab Emirates Dirham"),
      'symbol' => 'د.إ',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['AFN'] = [
      'label' => t("Afghan Afghani"),
      'symbol' => '؋',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['ALL'] = [
      'label' => t("Albanian Lek"),
      'symbol' => 'L',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['AMD'] = [
      'label' => t("Armenian Dram"),
      'symbol' => '֏',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['ANG'] = [
      'label' => t("Netherlands Antillean Guilder"),
      'symbol' => 'ƒ',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['AOA'] = [
      'label' => t("Angolan Kwanza"),
      'symbol' => 'Kz',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['ARS'] = [
      'label' => t("Argentine Peso"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['AUD'] = [
      'label' => t("Australian Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['AWG'] = [
      'label' => t("Aruban Florin"),
      'symbol' => 'ƒ',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['AZN'] = [
      'label' => t("Azerbaijani Manat"),
      'symbol' => '₼',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['BAM'] = [
      'label' => t("Bosnia and Herzegovina Convertible Mark"),
      'symbol' => 'KM',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['BBD'] = [
      'label' => t("Barbadian Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['BDT'] = [
      'label' => t("Bangladeshi Taka"),
      'symbol' => '৳',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['BGN'] = [
      'label' => t("Bulgarian Lev"),
      'symbol' => 'лв',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['BHD'] = [
      'label' => t("Bahraini Dinar"),
      'symbol' => 'د.ب',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 3,
    ];
    $currencies['BIF'] = [
      'label' => t("Burundian Franc"),
      'symbol' => 'FBu',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['BND'] = [
      'label' => t("Brunei Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['BOB'] = [
      'label' => t("Bolivian Boliviano"),
      'symbol' => 'Bs.',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['BRL'] = [
      'label' => t("Brazilian Real"),
      'symbol' => 'R$',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['BSD'] = [
      'label' => t("Bahamian Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['BTN'] = [
      'label' => t("Bhutanese Ngultrum"),
      'symbol' => 'Nu.',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['BWP'] = [
      'label' => t("Botswana Pula"),
      'symbol' => 'P',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['BYR'] = [
      'label' => t("Belarusian Ruble"),
      'symbol' => 'Br',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['BZD'] = [
      'label' => t("Belize Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['CAD'] = [
      'label' => t("Canadian Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['CDF'] = [
      'label' => t("Congolese Franc"),
      'symbol' => 'FC',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['CHF'] = [
      'label' => t("Swiss Franc"),
      'symbol' => 'Fr.',
      'symbol_after' => TRUE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['CLP'] = [
      'label' => t("Chilean Peso"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['CNY'] = [
      'label' => t("Chinese Renminbi"),
      'symbol' => '¥',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['COP'] = [
      'label' => t("Colombian Peso"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['CRC'] = [
      'label' => t("Costa Rican Colón"),
      'symbol' => '₡',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['CUP'] = [
      'label' => t("Cuban Peso"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['CVE'] = [
      'label' => t("Cape Verdean Escudo"),
      'symbol' => 'Esc',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['CZK'] = [
      'label' => t("Czech Koruna"),
      'symbol' => 'Kč',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['DJF'] = [
      'label' => t("Djiboutian Franc"),
      'symbol' => 'Fdj',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['DKK'] = [
      'label' => t("Danish Krone"),
      'symbol' => 'kr',
      'symbol_after' => TRUE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['DOP'] = [
      'label' => t("Dominican Peso"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['DZD'] = [
      'label' => t("Algerian Dinar"),
      'symbol' => 'د.ج',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['EGP'] = [
      'label' => t("Egyptian Pound"),
      'symbol' => 'ج.م',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['EUR'] = [
      'label' => t("Euro"),
      'symbol' => '€',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['ERN'] = [
      'label' => t("Eritrean Nakfa"),
      'symbol' => 'ናቕፋ',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['ETB'] = [
      'label' => t("Ethiopian Birr"),
      'symbol' => 'ብር',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['FJD'] = [
      'label' => t("Fijian Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['GBP'] = [
      'label' => t("British Pound Sterling"),
      'symbol' => '£',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['GEL'] = [
      'label' => t("Georgian Lari"),
      'symbol' => 'ლ',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['GHS'] = [
      'label' => t("Ghana Cedi"),
      'symbol' => '₵',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['GMD'] = [
      'label' => t("Gambian Dalasi"),
      'symbol' => 'D',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['GNF'] = [
      'label' => t("Guinean Franc"),
      'symbol' => 'FG',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['GTQ'] = [
      'label' => t("Guatemalan Quetzal"),
      'symbol' => 'Q',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['GYD'] = [
      'label' => t("Guyanese Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['HKD'] = [
      'label' => t("Hong Kong Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['HNL'] = [
      'label' => t("Honduran Lempira"),
      'symbol' => 'L',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['HRK'] = [
      'label' => t("Croatian Kuna"),
      'symbol' => 'kn',
      'symbol_after' => TRUE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['HTG'] = [
      'label' => t("Haitian Gourde"),
      'symbol' => 'G',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['IDR'] = [
      'label' => t("Indonesian Rupiah"),
      'symbol' => 'Rp',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['ILS'] = [
      'label' => t("Israeli New Shekel"),
      'symbol' => '₪',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['INR'] = [
      'label' => t("Indian Rupee"),
      'symbol' => '₹',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['IQD'] = [
      'label' => t("Iraqi Dinar"),
      'symbol' => 'د.ع',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['IRR'] = [
      'label' => t("Iranian Rial"),
      'symbol' => '﷼',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['ISK'] = [
      'label' => t("Icelandic Króna"),
      'symbol' => 'kr',
      'symbol_after' => TRUE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['JMD'] = [
      'label' => t("Jamaican Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['JOD'] = [
      'label' => t("Jordanian Dinar"),
      'symbol' => 'JD',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 3,
    ];
    $currencies['JPY'] = [
      'label' => t("Japanese Yen"),
      'symbol' => '¥',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['KES'] = [
      'label' => t("Kenyan Shilling"),
      'symbol' => 'KSh',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['KGS'] = [
      'label' => t("Kyrgyzstani Som"),
      'symbol' => 'лв',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['KHR'] = [
      'label' => t("Cambodian Riel"),
      'symbol' => '៛',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['KMF'] = [
      'label' => t("Comorian Franc"),
      'symbol' => 'CF',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['KPW'] = [
      'label' => t("North Korean Won"),
      'symbol' => '₩',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['KRW'] = [
      'label' => t("South Korean Won"),
      'symbol' => '₩',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['KWD'] = [
      'label' => t("Kuwaiti Dinar"),
      'symbol' => 'د.ك',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 3,
    ];
    $currencies['KYD'] = [
      'label' => t("Cayman Islands Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['KZT'] = [
      'label' => t("Kazakhstani Tenge"),
      'symbol' => '₸',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['LAK'] = [
      'label' => t("Lao Kip"),
      'symbol' => '₭',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['LBP'] = [
      'label' => t("Lebanese Pound"),
      'symbol' => 'ل.ل',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['LKR'] = [
      'label' => t("Sri Lankan Rupee"),
      'symbol' => 'රු',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['LRD'] = [
      'label' => t("Liberian Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['LSL'] = [
      'label' => t("Lesotho Loti"),
      'symbol' => 'L',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['LTL'] = [
      'label' => t("Lithuanian Litas"),
      'symbol' => 'Lt',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['LYD'] = [
      'label' => t("Libyan Dinar"),
      'symbol' => 'ل.د',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 3,
    ];
    $currencies['MAD'] = [
      'label' => t("Moroccan Dirham"),
      'symbol' => 'د.م.',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['MDL'] = [
      'label' => t("Moldovan Leu"),
      'symbol' => 'L',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['MGA'] = [
      'label' => t("Malagasy Ariary"),
      'symbol' => 'Ar',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['MKD'] = [
      'label' => t("Macedonian Denar"),
      'symbol' => 'ден',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['MMK'] = [
      'label' => t("Burmese Kyat"),
      'symbol' => 'K',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['MNT'] = [
      'label' => t("Mongolian Tögrög"),
      'symbol' => '₮',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['MOP'] = [
      'label' => t("Macanese Pataca"),
      'symbol' => 'MOP$',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['MRO'] = [
      'label' => t("Mauritanian Ouguiya"),
      'symbol' => 'UM',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['MUR'] = [
      'label' => t("Mauritian Rupee"),
      'symbol' => '₨',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['MWK'] = [
      'label' => t("Malawian Kwacha"),
      'symbol' => 'MK',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['MXN'] = [
      'label' => t("Mexican Peso"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['MYR'] = [
      'label' => t("Malaysian Ringgit"),
      'symbol' => 'RM',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['MZN'] = [
      'label' => t("Mozambican Metical"),
      'symbol' => 'MT',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['NAD'] = [
      'label' => t("Namibian Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['NGN'] = [
      'label' => t("Nigerian Naira"),
      'symbol' => '₦',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['NIO'] = [
      'label' => t("Nicaraguan Córdoba"),
      'symbol' => 'C$',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['NOK'] = [
      'label' => t("Norwegian Krone"),
      'symbol' => 'kr',
      'symbol_after' => TRUE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['NPR'] = [
      'label' => t("Nepalese Rupee"),
      'symbol' => 'रू',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['NZD'] = [
      'label' => t("New Zealand Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['OMR'] = [
      'label' => t("Omani Rial"),
      'symbol' => 'ر.ع.',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 3,
    ];
    $currencies['PEN'] = [
      'label' => t("Peruvian Nuevo Sol"),
      'symbol' => 'S/.',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['PHP'] = [
      'label' => t("Philippine Peso"),
      'symbol' => '₱',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['PKR'] = [
      'label' => t("Pakistani Rupee"),
      'symbol' => '₨',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['PLN'] = [
      'label' => t("Polish Złoty"),
      'symbol' => 'zł',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['PYG'] = [
      'label' => t("Paraguayan Guaraní"),
      'symbol' => '₲',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['QAR'] = [
      'label' => t("Qatari Riyal"),
      'symbol' => 'ر.ق',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['RON'] = [
      'label' => t("Romanian Leu"),
      'symbol' => 'lei',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['RSD'] = [
      'label' => t("Serbian Dinar"),
      'symbol' => 'РСД',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['RUB'] = [
      'label' => t("Russian Ruble"),
      'symbol' => '₽',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['RWF'] = [
      'label' => t("Rwandan Franc"),
      'symbol' => 'R₣',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['SAR'] = [
      'label' => t("Saudi Riyal"),
      'symbol' => 'ر.س',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['SCR'] = [
      'label' => t("Seychellois Rupee"),
      'symbol' => '₨',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['SDG'] = [
      'label' => t("Sudanese Pound"),
      'symbol' => 'ج.س.',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['SEK'] = [
      'label' => t("Swedish Krona"),
      'symbol' => 'kr',
      'symbol_after' => TRUE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['SGD'] = [
      'label' => t("Singapore Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['SLL'] = [
      'label' => t("Sierra Leonean Leone"),
      'symbol' => 'Le',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['SOS'] = [
      'label' => t("Somali Shilling"),
      'symbol' => 'Sh.So.',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['SRD'] = [
      'label' => t("Surinamese Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['SSP'] = [
      'label' => t("South Sudanese Pound"),
      'symbol' => 'PT.',
      'symbol_after' => TRUE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['STD'] = [
      'label' => t("São Tomé and Príncipe Dobra"),
      'symbol' => 'Db',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['SYP'] = [
      'label' => t("Syrian Pound"),
      'symbol' => 'ل.س',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['SZL'] = [
      'label' => t("Swazi Lilangeni"),
      'symbol' => 'L',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['THB'] = [
      'label' => t("Thai Baht"),
      'symbol' => '฿',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['TJS'] = [
      'label' => t("Tajikistani Somoni"),
      'symbol' => 'ЅМ',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['TMT'] = [
      'label' => t("Turkmenistan Manat"),
      'symbol' => 'm',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['TND'] = [
      'label' => t("Tunisian Dinar"),
      'symbol' => 'د.ت',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 3,
    ];
    $currencies['TRY'] = [
      'label' => t("Turkish Lira"),
      'symbol' => '₺',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['TTD'] = [
      'label' => t("Trinidad and Tobago Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['TWD'] = [
      'label' => t("New Taiwan Dollar"),
      'symbol' => '塊',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['TZS'] = [
      'label' => t("Tanzanian Shilling"),
      'symbol' => 'TSh',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['UAH'] = [
      'label' => t("Ukrainian Hryvnia"),
      'symbol' => '₴',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['UGX'] = [
      'label' => t("Ugandan Shilling"),
      'symbol' => 'USh',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['USD'] = [
      'label' => t("United States Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['UYU'] = [
      'label' => t("Uruguayan Peso"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['UZS'] = [
      'label' => t("Uzbekistani Som"),
      'symbol' => 'лв',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['VEF'] = [
      'label' => t("Venezuelan Bolívar"),
      'symbol' => 'Bs.F.',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['VND'] = [
      'label' => t("Vietnamese Dong"),
      'symbol' => '₫',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['XAF'] = [
      'label' => t("Central African CFA Franc"),
      'symbol' => 'FCFA',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['XCD'] = [
      'label' => t("East Caribbean Dollar"),
      'symbol' => '$',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 2,
    ];
    $currencies['XOF'] = [
      'label' => t("West African CFA Franc"),
      'symbol' => 'CFA',
      'symbol_after' => TRUE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 0,
    ];
    $currencies['YER'] = [
      'label' => t("Yemeni Rial"),
      'symbol' => '﷼',
      'symbol_after' => FALSE,
      'symbol_distinct' => FALSE,
      'decimal_places' => 0,
    ];
    $currencies['ZAR'] = [
      'label' => t("South African Rand"),
      'symbol' => 'R',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    $currencies['ZMW'] = [
      'label' => t("Zambian Kwacha"),
      'symbol' => 'ZK',
      'symbol_after' => FALSE,
      'symbol_distinct' => TRUE,
      'decimal_places' => 2,
    ];
    return $currencies;
  }

}
