<?php

/**
 * Implementation of hook_banking_iban_format_info().
 */
function banking_banking_iban_format_info() {
  return array(
    new BankingIBANFormat(array(
      'countryCode' => 'AD',
      'pattern'  => '/^AD(\d{2})(\d{4})(\d{4})([A-Za-z0-9]{12})$/i',
      'example' => 'AD1200012030200359100100',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'AL',
      'pattern'  => '/^AL(\d{2})(\d{8})([A-Za-z0-9]{16})$/i',
      'example' => 'AL47212110090000000235698741',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'AT',
      'pattern'  => '/^AT(\d{2})(\d{5})(\d{11})$/i',
      'example' => 'AT611904300234573201',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'BA',
      'pattern'  => '/^BA(\d{2})(\d{3})(\d{3})(\d{8})(\d{2})$/i',
      'example' => 'BA391290079401028494',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'BE',
      'pattern'  => '/^BE(\d{2})(\d{3})(\d{7})(\d{2})$/i',
      'example' => 'BE68539007547034',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'BG',
      'pattern'  => '/^BG(\d{2})([A-Z]{4})(\d{4})(\d{2})([A-Za-z0-9]{8})$/i',
      'example' => 'BG80BNBG96611020345678',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'CH',
      'pattern'  => '/^CH(\d{2})(\d{5})([A-Za-z0-9]{12})$/i',
      'example' => 'CH9300762011623852957',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'CY',
      'pattern'  => '/^CY(\d{2})(\d{3})(\d{5})([A-Za-z0-9]{16})$/i',
      'example' => 'CY17002001280000001200527600',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'CZ',
      'pattern'  => '/^CZ(\d{2})(\d{4})(\d{6})(\d{10})$/i',
      'example' => 'CZ6508000000192000145399',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'DE',
      'pattern'  => '/^DE(\d{2})(\d{8})(\d{10})$/i',
      'example' => 'DE89370400440532013000',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'DK',
      'pattern'  => '/^DK(\d{2})(\d{4})(\d{9})(\d{1})$/i',
      'example' => 'DK5000400440116243',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'EE',
      'pattern'  => '/^EE(\d{2})(\d{2})(\d{2})(\d{11})(\d{1})$/i',
      'example' => 'EE382200221020145685',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'ES',
      'pattern'  => '/^ES(\d{2})(\d{4})(\d{4})(\d{1})(\d{1})(\d{10})$/i',
      'example' => 'ES9121000418450200051332',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'FI',
      'pattern'  => '/^FI(\d{2})(\d{6})(\d{7})(\d{1})$/i',
      'example' => 'FI2112345600000785',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'FR',
      'pattern'  => '/^FR(\d{2})(\d{5})(\d{5})([A-Za-z0-9]{11})(\d{2})$/i',
      'example' => 'FR1420041010050500013M02606',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'GB',
      'pattern'  => '/^GB(\d{2})([A-Z]{4})(\d{6})(\d{8})$/i',
      'example' => 'GB29NWBK60161331926819',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'GI',
      'pattern'  => '/^GI(\d{2})([A-Z]{4})([A-Za-z0-9]{15})$/i',
      'example' => 'GI75NWBK000000007099453',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'GR',
      'pattern'  => '/^GR(\d{2})(\d{3})(\d{4})([A-Za-z0-9]{16})$/i',
      'example' => 'GR1601101250000000012300695',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'HR',
      'pattern'  => '/^HR(\d{2})(\d{7})(\d{10})$/i',
      'example' => 'HR1210010051863000160',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'HU',
      'pattern'  => '/^HU(\d{2})(\d{3})(\d{4})(\d{1})(\d{15})(\d{1})$/i',
      'example' => 'HU42117730161111101800000000',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'IE',
      'pattern'  => '/^IE(\d{2})([A-Z]{4})(\d{6})(\d{8})$/i',
      'example' => 'IE29AIBK93115212345678',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'IL',
      'pattern'  => '/^IL(\d{2})(\d{3})(\d{3})(\d{13})$/i',
      'example' => 'IL620108000000099999999',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'IS',
      'pattern'  => '/^IS(\d{2})(\d{4})(\d{2})(\d{6})(\d{10})$/i',
      'example' => 'IS140159260076545510730339',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'IT',
      'pattern'  => '/^IT(\d{2})([A-Z]{1})(\d{5})(\d{5})([A-Za-z0-9]{12})$/i',
      'example' => 'IT60X0542811101000000123456',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'LI',
      'pattern'  => '/^LI(\d{2})(\d{5})([A-Za-z0-9]{12})$/i',
      'example' => 'LI21088100002324013AA',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'LT',
      'pattern'  => '/^LT(\d{2})(\d{5})(\d{11})$/i',
      'example' => 'LT121000011101001000',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'LU',
      'pattern'  => '/^LU(\d{2})(\d{3})([A-Za-z0-9]{13})$/i',
      'example' => 'LU280019400644750000',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'LV',
      'pattern'  => '/^LV(\d{2})([A-Z]{4})([A-Za-z0-9]{13})$/i',
      'example' => 'LV80BANK0000435195001',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'MC',
      'pattern'  => '/^MC(\d{2})(\d{5})(\d{5})([A-Za-z0-9]{11})(\d{2})$/i',
      'example' => 'MC1112739000700011111000h79',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'ME',
      'pattern'  => '/^ME(\d{2})(\d{3})(\d{13})(\d{2})$/i',
      'example' => 'ME25505000012345678951',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'MK',
      'pattern'  => '/^MK(\d{2})(\d{3})([A-Za-z0-9]{10})(\d{2})$/i',
      'example' => 'MK07250120000058984',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'MT',
      'pattern'  => '/^MT(\d{2})([A-Z]{4})(\d{5})([A-Za-z0-9]{18})$/i',
      'example' => 'MT84MALT011000012345MTLCAST001S',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'MU',
      'pattern'  => '/^MU(\d{2})([A-Z]{4})(\d{2})(\d{2})(\d{12})(\d{3})([A-Z]{3})$/i',
      'example' => 'MU17BOMM0101101030300200000MUR',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'NC',
      'pattern'  => '/^NC(\d{2})(\d{5})(\d{5})([A-Za-z0-9]{11})(\d{2})$/i',
      'example' => 'NC1420041010050500013M02606',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'NL',
      'pattern'  => '/^NL(\d{2})([A-Z]{4})(\d{10})$/i',
      'example' => 'NL91ABNA0417164300',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'NO',
      'pattern'  => '/^NO(\d{2})(\d{4})(\d{6})(\d{1})$/i',
      'example' => 'NO9386011117947',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'PF',
      'pattern'  => '/^PF(\d{2})(\d{5})(\d{5})([A-Za-z0-9]{11})(\d{2})$/i',
      'example' => 'PF1420041010050500013M02606',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'PL',
      'pattern'  => '/^PL(\d{2})(\d{8})(\d{16})$/i',
      'example' => 'PL61109010140000071219812874',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'PM',
      'pattern'  => '/^PM(\d{2})(\d{5})(\d{5})([A-Za-z0-9]{11})(\d{2})$/i',
      'example' => 'PM1420041010050500013M02606',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'PT',
      'pattern'  => '/^PT(\d{2})(\d{4})(\d{4})(\d{11})(\d{2})$/i',
      'example' => 'PT50000201231234567890154',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'RO',
      'pattern'  => '/^RO(\d{2})([A-Z]{4})([A-Za-z0-9]{16})$/i',
      'example' => 'RO49AAAA1B31007593840000',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'RS',
      'pattern'  => '/^RS(\d{2})(\d{3})(\d{13})(\d{2})$/i',
      'example' => 'RS35260005601001611379',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'SA',
      'pattern'  => '/^SA(\d{2})(\d{2})([A-Za-z0-9]{18})$/i',
      'example' => 'SA0380000000608010167519',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'SE',
      'pattern'  => '/^SE(\d{2})(\d{3})(\d{16})(\d{1})$/i',
      'example' => 'SE1212312345678901234561',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'SI',
      'pattern'  => '/^SI(\d{2})(\d{5})(\d{8})(\d{2})$/i',
      'example' => 'SI56191000000123438',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'SK',
      'pattern'  => '/^SK(\d{2})(\d{4})(\d{6})(\d{10})$/i',
      'example' => 'SK3112000000198742637541',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'SM',
      'pattern'  => '/^SM(\d{2})([A-Z]{1})(\d{5})(\d{5})([A-Za-z0-9]{12})$/i',
      'example' => 'SM86U0322509800000000270100',
     )),
    new BankingIBANFormat(array(
      'countryCode' => 'TF',
      'pattern'  => '/^TF(\d{2})(\d{5})(\d{5})([A-Za-z0-9]{11})(\d{2})$/i',
      'example' => 'TF1420041010050500013M02606',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'TN',
      'pattern'  => '/^TN(\d{2})(\d{2})(\d{3})(\d{13})(\d{2})$/i',
      'example' => 'TN5910006035183598478831',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'TR',
      'pattern'  => '/^TR(\d{2})(\d{5})([A-Za-z0-9]{1})([A-Za-z0-9]{16})$/i',
      'example' => 'TR330006100519786457841326',
    )),
    new BankingIBANFormat(array(
      'countryCode' => 'WF',
      'pattern'  => '/^WF(\d{2})(\d{5})(\d{5})([A-Za-z0-9]{11})(\d{2})$/i',
      'example' => 'WF1420041010050500013M02606',
    )),
    new BankingIBANFormat(array(
      'pattern'  => '/^YT(\d{2})(\d{5})(\d{5})([A-Za-z0-9]{11})(\d{2})$/i',
      'example' => 'YT1420041010050500013M02606',
    )),
  );
}