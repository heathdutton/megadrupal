<?php
/**
 * @file
 * Default configuration for the Format Currency project.
 *
 * NOTE: The 'name' and 'links' fields are NOT used - they're present for simply for information.
 *
 * Whilst every effort has been made to ensure this list is correct, it's a LONG
 * list and mistakes *may* have been made. If you find one PLEASE report it to
 * the issue queue for this module so the community can benefit from the change.
 */

/**
 * Implements hook_default_currency().
 */
function format_currency_default_currency() {
  $export = array();

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'United Arab Emirates, Dirhams';
  $currency->links = array(
    'http://www.xe.com/currency/aed-emirati-dirham',
    'http://www.gocurrency.com/countries/uae.htm',
    'http://en.wikipedia.org/wiki/United_Arab_Emirates_dirham',
  );
  $currency->code = 'AED';
  $currency->num = 784;
  $currency->base = 100;
  $currency->symbol = 'د.إ';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['AED'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Afghanistan, Afghanis';
  $currency->links = array(
    'http://www.xe.com/currency/afn-afghan-afghani',
    'http://www.gocurrency.com/countries/afghanistan.htm',
    'http://en.wikipedia.org/wiki/Afghan_afghani',
  );
  $currency->code = 'AFN';
  $currency->num = 971;
  $currency->base = 100;
  $currency->symbol = '؋';
  $currency->html_entity = '&#1547;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['AFN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Albania, Leke';
  $currency->links = array(
    'http://www.xe.com/currency/all-albanian-lek',
    'http://en.wikipedia.org/wiki/Albanian_lek',
  );
  $currency->code = 'ALL';
  $currency->num = 8;
  $currency->base = 100;
  $currency->symbol = 'Lek';
  $currency->html_entity = '&#76;&#101;&#107;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ALL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Armenia, Drams';
  $currency->links = array(
    'http://www.xe.com/currency/amd-armenian-dram',
    'http://www.gocurrency.com/countries/armenia.htm',
    'http://en.wikipedia.org/wiki/Armenian_dram',
  );
  $currency->code = 'AMD';
  $currency->num = 51;
  $currency->base = 100;
  $currency->symbol = 'Դ';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['AMD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Netherlands Antilles, Guilders (also called Florins)';
  $currency->links = array(
    'http://www.xe.com/currency/ang-dutch-guilder',
    'http://www.gocurrency.com/countries/netherland_antilles.htm',
    'http://en.wikipedia.org/wiki/Netherlands_Antillean_guilder',
  );
  $currency->code = 'ANG';
  $currency->num = 532;
  $currency->base = 100;
  $currency->symbol = 'ƒ';
  $currency->html_entity = '&fnof;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ANG'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Angola, Kwanza';
  $currency->links = array(
    'http://www.xe.com/currency/aoa-angolan-kwanza',
    'http://www.gocurrency.com/countries/angola.htm',
    'http://en.wikipedia.org/wiki/Angolan_kwanza',
  );
  $currency->code = 'AOA';
  $currency->num = 973;
  $currency->base = 100;
  $currency->symbol = 'Kzr.';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['AOA'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Argentina, Pesos';
  $currency->links = array(
    'http://www.xe.com/currency/ars-argentine-peso',
    'http://www.gocurrency.com/countries/argentina.htm',
    'http://en.wikipedia.org/wiki/Argentine_peso',
  );
  $currency->code = 'ARS';
  $currency->num = 32;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ARS'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Australian Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/aud-australian-dollar',
    'http://www.gocurrency.com/countries/australia.htm',
    'http://en.wikipedia.org/wiki/Australian_dollar',
  );
  $currency->code = 'AUD';
  $currency->num = 36;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ' ';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['AUD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Aruba, Guilders (also called Florins)';
  $currency->links = array(
    'http://www.xe.com/currency/awg-aruban-or-dutch-guilder',
    'http://www.gocurrency.com/countries/aruba.htm',
    'http://en.wikipedia.org/wiki/Aruban_florin',
  );
  $currency->code = 'AWG';
  $currency->num = 533;
  $currency->base = 100;
  $currency->symbol = 'ƒ';
  $currency->html_entity = '&fnof;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['AWG'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Azerbaijan, New Manats';
  $currency->links = array(
    'http://www.xe.com/currency/azn-azerbaijani-new-manat',
    'http://www.gocurrency.com/countries/azerbaijan.htm',
    'http://en.wikipedia.org/wiki/Azerbaijani_manat',
  );
  $currency->code = 'AZN';
  $currency->num = 944;
  $currency->base = 100;
  $currency->symbol = 'ман';
  $currency->html_entity = '&#1084;&#1072;&#1085;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['AZN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Bosnia and Herzegovina, Convertible Marka';
  $currency->links = array(
    'http://www.xe.com/currency/bam-bosnian-convertible-marka',
    'http://www.gocurrency.com/countries/bosnia.htm',
    'http://en.wikipedia.org/wiki/Bosnia_and_Herzegovina_convertible_mark',
  );
  $currency->code = 'BAM';
  $currency->num = 977;
  $currency->base = 100;
  $currency->symbol = 'KM';
  $currency->html_entity = '&#75;&#77;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BAM'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Barbados, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/bbd-barbadian-or-bajan-dollar',
    'http://www.gocurrency.com/countries/barbados.htm',
    'http://en.wikipedia.org/wiki/Barbadian_dollar',
  );
  $currency->code = 'BBD';
  $currency->num = 52;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BBD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Bangladesh, Taka';
  $currency->links = array(
    'http://www.xe.com/currency/bdt-bangladeshi-taka',
    'http://en.wikipedia.org/wiki/Bangladeshi_taka',
  );
  $currency->code = 'BDT';
  $currency->num = 50;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '&#2547;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BDT'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Bulgaria, Leva';
  $currency->links = array(
    'http://www.xe.com/currency/bgn-bulgarian-lev',
    'http://www.gocurrency.com/countries/bulgaria.htm',
    'http://en.wikipedia.org/wiki/Bulgarian_lev',
  );
  $currency->code = 'BGN';
  $currency->num = 975;
  $currency->base = 100;
  $currency->symbol = 'лв';
  $currency->html_entity = '&#1083;&#1074;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BGN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Bahrain, Dinars';
  $currency->links = array(
    'http://www.xe.com/currency/bhd-bahraini-dinar',
    'http://www.gocurrency.com/countries/bahrain.htm',
    'http://en.wikipedia.org/wiki/Bahraini_dinar',
  );
  $currency->code = 'BHD';
  $currency->num = 48;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BHD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Burundi, Francs';
  $currency->links = array(
    'http://www.xe.com/currency/bif-burundian-franc',
    'http://www.gocurrency.com/countries/burundi.htm',
    'http://en.wikipedia.org/wiki/Burundian_franc',
  );
  $currency->code = 'BIF';
  $currency->num = 108;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BIF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Bermuda, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/bmd-bermudian-dollar',
    'http://www.gocurrency.com/countries/bermuda.htm',
    'http://en.wikipedia.org/wiki/Bermudian_dollar',
  );
  $currency->code = 'BMD';
  $currency->num = 60;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BMD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Brunei Darussalam, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/bnd-bruneian-dollar',
    'http://www.gocurrency.com/countries/brunei.htm',
    'http://en.wikipedia.org/wiki/Brunei_dollar',
  );
  $currency->code = 'BND';
  $currency->num = 96;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BND'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Bolivia, Bolivianos';
  $currency->links = array(
    'http://www.xe.com/currency/bob-bolivian-boliviano',
    'http://www.gocurrency.com/countries/bolivia.htm',
    'http://en.wikipedia.org/wiki/Boliviano',
  );
  $currency->code = 'BOB';
  $currency->num = 68;
  $currency->base = 100;
  $currency->symbol = '$b';
  $currency->html_entity = '&#36;&#98;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BOB'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Brazil, Brazil Real';
  $currency->links = array(
    'http://www.xe.com/currency/brl-brazilian-real',
    'http://www.gocurrency.com/countries/brazil.htm',
    'http://en.wikipedia.org/wiki/Brazilian_real',
  );
  $currency->code = 'BRL';
  $currency->num = 986;
  $currency->base = 100;
  $currency->symbol = 'R$';
  $currency->html_entity = '&#82;&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BRL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Bahamas, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/bsd-bahamian-dollar',
    'http://www.gocurrency.com/countries/bahamas.htm',
    'http://en.wikipedia.org/wiki/Bahamian_dollar',
  );
  $currency->code = 'BSD';
  $currency->num = 44;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BSD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Bhutan, Ngultrum';
  $currency->links = array(
    'http://www.xe.com/currency/btn-bhutanese-ngultrum',
    'http://www.gocurrency.com/countries/bhutan.htm',
    'http://en.wikipedia.org/wiki/Bhutanese_ngultrum',
  );
  $currency->code = 'BTN';
  $currency->num = 64;
  $currency->base = 100;
  $currency->symbol = 'Nu.';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BTN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Botswana, Pulas';
  $currency->links = array(
    'http://www.xe.com/currency/bwp-batswana-pula',
    'http://www.gocurrency.com/countries/botswana.htm',
    'http://en.wikipedia.org/wiki/Botswana_pula',
  );
  $currency->code = 'BWP';
  $currency->num = 72;
  $currency->base = 100;
  $currency->symbol = 'P';
  $currency->html_entity = '&#80;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BWP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Belarus, Rubles';
  $currency->links = array(
    'http://www.xe.com/currency/byr-belarusian-ruble',
    'http://www.gocurrency.com/countries/belarus.htm',
    'http://en.wikipedia.org/wiki/Belarusian_ruble',
  );
  $currency->code = 'BYR';
  $currency->num = 974;
  $currency->base = 100;
  $currency->symbol = 'p.';
  $currency->html_entity = '&#112;&#46;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BYR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Belize, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/bzd-belizean-dollar',
    'http://www.gocurrency.com/countries/belize.htm',
    'http://en.wikipedia.org/wiki/Belize_dollar',
  );
  $currency->code = 'BZD';
  $currency->num = 84;
  $currency->base = 100;
  $currency->symbol = 'BZ$';
  $currency->html_entity = '&#66;&#90;&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['BZD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Canada, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/cad-canadian-dollar',
    'http://www.gocurrency.com/countries/canada.htm',
    'http://en.wikipedia.org/wiki/Canadian_dollar',
  );
  $currency->code = 'CAD';
  $currency->num = 124;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['CAD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Congo/Kinshasa, Congolese Francs';
  $currency->links = array(
    'http://www.xe.com/currency/cdf-congolese-franc',
    'http://www.gocurrency.com/countries/congo.htm',
    'http://en.wikipedia.org/wiki/Congolese_franc',
  );
  $currency->code = 'CDF';
  $currency->num = 976;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['CDF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Switzerland, Francs';
  $currency->links = array(
    'http://www.xe.com/currency/chf-swiss-franc',
    'http://www.gocurrency.com/countries/switzerland.htm',
    'http://en.wikipedia.org/wiki/Swiss_franc',
  );
  $currency->code = 'CHF';
  $currency->num = 756;
  $currency->base = 100;
  $currency->symbol = 'CHF';
  $currency->html_entity = '&#67;&#72;&#70;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '\'';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['CHF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Chile, Pesos';
  $currency->links = array(
    'http://www.xe.com/currency/clp-chilean-peso',
    'http://www.gocurrency.com/countries/chile.htm',
    'http://en.wikipedia.org/wiki/Chilean_peso',
  );
  $currency->code = 'CLP';
  $currency->num = 152;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['CLP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'China, Yuan Renminbi';
  $currency->links = array(
    'http://www.xe.com/currency/cny-chinese-yuan-renminbi',
    'http://www.gocurrency.com/countries/china.htm',
    'http://en.wikipedia.org/wiki/Renminbi',
  );
  $currency->code = 'CNY';
  $currency->num = 156;
  $currency->base = 100;
  $currency->symbol = '¥';
  $currency->html_entity = '&yen;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['CNY'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Colombia, Pesos';
  $currency->links = array(
    'http://www.xe.com/currency/cop-colombian-peso',
    'http://www.gocurrency.com/countries/colombia.htm',
    'http://en.wikipedia.org/wiki/Colombian_peso',
  );
  $currency->code = 'COP';
  $currency->num = 170;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['COP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Costa Rica, Colones';
  $currency->links = array(
    'http://www.xe.com/currency/crc-costa-rican-colon',
    'http://www.gocurrency.com/countries/costa_rica.htm',
    'http://en.wikipedia.org/wiki/Costa_Rican_colon',
  );
  $currency->code = 'CRC';
  $currency->num = 188;
  $currency->base = 100;
  $currency->symbol = '₡';
  $currency->html_entity = '&#8353;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['CRC'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Cuba, Pesos';
  $currency->links = array(
    'http://www.xe.com/currency/cup-cuban-peso',
    'http://www.gocurrency.com/countries/cuba.htm',
    'http://en.wikipedia.org/wiki/Cuban_peso',
  );
  $currency->code = 'CUP';
  $currency->num = 192;
  $currency->base = 100;
  $currency->symbol = '₱';
  $currency->html_entity = '&#8369;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['CUP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Cape Verde, Escudos';
  $currency->links = array(
    'http://www.xe.com/currency/cve-cape-verdean-escudo',
    'http://en.wikipedia.org/wiki/Cape_Verde_escudo',
  );
  $currency->code = 'CVE';
  $currency->num = 132;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['CVE'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Cyprus, Pounds (expires 2008-Jan-31)';
  $currency->links = array(
    'http://www.gocurrency.com/countries/cyprus.htm',
  );
  $currency->code = 'CYP';
  $currency->num = 196;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_EXPIRED;
  $export['CYP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Czech Republic, Koruny';
  $currency->links = array(
    'http://www.xe.com/currency/czk-czech-koruna',
    'http://en.wikipedia.org/wiki/Czech_koruna',
  );
  $currency->code = 'CZK';
  $currency->num = 203;
  $currency->base = 100;
  $currency->symbol = 'Kč';
  $currency->html_entity = '&#75;&#269;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_RIGHT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['CZK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Djibouti, Francs';
  $currency->links = array(
    'http://www.xe.com/currency/djf-djiboutian-franc',
    'http://www.gocurrency.com/countries/djibouti.htm',
    'http://en.wikipedia.org/wiki/Djiboutian_franc',
  );
  $currency->code = 'DJF';
  $currency->num = 262;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['DJF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Denmark, Kroner';
  $currency->links = array(
    'http://www.xe.com/currency/dkk-danish-krone',
    'http://www.gocurrency.com/countries/denmark.htm',
    'http://en.wikipedia.org/wiki/Danish_krone',
  );
  $currency->code = 'DKK';
  $currency->num = 208;
  $currency->base = 100;
  $currency->symbol = 'kr';
  $currency->html_entity = '&#107;&#114;';
  $currency->coinage_symbol = 'Øre';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['DKK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Dominican Republic, Pesos';
  $currency->links = array(
    'http://www.xe.com/currency/dop-dominican-peso',
    'http://www.gocurrency.com/countries/dominican_republic.htm',
    'http://en.wikipedia.org/wiki/Dominican_peso',
  );
  $currency->code = 'DOP';
  $currency->num = 214;
  $currency->base = 100;
  $currency->symbol = 'RD$';
  $currency->html_entity = '&#82;&#68;&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['DOP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Algeria, Algeria Dinars';
  $currency->links = array(
    'http://www.xe.com/currency/dzd-algerian-dinar',
    'http://www.gocurrency.com/countries/algeria.htm',
    'http://en.wikipedia.org/wiki/Algerian_dinar',
  );
  $currency->code = 'DZD';
  $currency->num = 12;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['DZD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Estonia, Krooni';
  $currency->links = array(
    'http://www.xe.com/currency/eek-estonian-kroon',
    'http://www.gocurrency.com/countries/estonia.htm',
    'http://en.wikipedia.org/wiki/Estonian_kroon',
  );
  $currency->code = 'EEK';
  $currency->num = 233;
  $currency->base = 100;
  $currency->symbol = 'kr';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['EEK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Egypt, Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/egp-egyptian-pound',
    'http://www.gocurrency.com/countries/egypt.htm',
    'http://en.wikipedia.org/wiki/Egyptian_pound',
  );
  $currency->code = 'EGP';
  $currency->num = 818;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['EGP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Eritrea, Nakfa';
  $currency->links = array(
    'http://www.xe.com/currency/ern-eritrean-nakfa',
    'http://www.gocurrency.com/countries/eritrea.htm',
    'http://en.wikipedia.org/wiki/Eritrean_nakfa',
  );
  $currency->code = 'ERN';
  $currency->num = 232;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ERN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Ethiopia, Birr';
  $currency->links = array(
    'http://www.xe.com/currency/etb-ethiopian-birr',
    'http://www.gocurrency.com/countries/ethiopia.htm',
    'http://en.wikipedia.org/wiki/Ethiopian_birr',
  );
  $currency->code = 'ETB';
  $currency->num = 230;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ETB'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Euro Member Countries, Euro';
  $currency->links = array(
    'http://www.xe.com/currency/eur-euro',
    'http://www.gocurrency.com/countries/euro.htm',
    'http://en.wikipedia.org/wiki/Euro',
  );
  $currency->code = 'EUR';
  $currency->num = 978;
  $currency->base = 100;
  $currency->symbol = '€';
  $currency->html_entity = '&euro;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['EUR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Fiji, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/fjd-fijian-dollar',
    'http://www.gocurrency.com/countries/fiji.htm',
    'http://en.wikipedia.org/wiki/Fiji_dollar',
  );
  $currency->code = 'FJD';
  $currency->num = 242;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['FJD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Falkland Islands (Malvinas), Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/fkp-falkland-island-pound',
    'http://www.gocurrency.com/countries/falkland_islands.htm',
    'http://en.wikipedia.org/wiki/Falkland_Islands_pound',
  );
  $currency->code = 'FKP';
  $currency->num = 238;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['FKP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'United Kingdom, Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/gbp-british-pound',
    'http://www.gocurrency.com/countries/united_kingdom.htm',
    'http://en.wikipedia.org/wiki/Pound_sterling',
  );
  $currency->code = 'GBP';
  $currency->num = 826;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = 'p';
  $currency->coinage_html_entity = '&#112;';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['GBP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Georgia, Lari';
  $currency->links = array(
    'http://www.xe.com/currency/gel-georgian-lari',
    'http://en.wikipedia.org/wiki/Georgian_lari',
  );
  $currency->code = 'GEL';
  $currency->num = 981;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['GEL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Guernsey, Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/ggp-guernsey-pound',
    'http://en.wikipedia.org/wiki/Guernsey_pound',
  );
  $currency->code = 'GGP';
  $currency->num = 0;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['GGP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Ghana, Cedis';
  $currency->links = array(
    'http://www.gocurrency.com/countries/ghana.htm',
    'http://en.wikipedia.org/wiki/Ghanaian_cedi',
  );
  $currency->code = 'GHC';
  $currency->num = 288;
  $currency->base = 100;
  $currency->symbol = '¢';
  $currency->html_entity = '&cent;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_EXPIRED;
  $export['GHC'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Ghana, Cedis';
  $currency->links = array(
    'http://www.xe.com/currency/ghs-ghanaian-cedi',
    'http://www.gocurrency.com/countries/ghana.htm',
    'http://en.wikipedia.org/wiki/Ghanaian_cedi',
  );
  $currency->code = 'GHS';
  $currency->num = 936;
  $currency->base = 100;
  $currency->symbol = '¢';
  $currency->html_entity = '&cent;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['GHS'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Gibraltar, Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/gip-gibraltar-pound',
    'http://en.wikipedia.org/wiki/Gibraltar_pound',
  );
  $currency->code = 'GIP';
  $currency->num = 292;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['GIP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Gambia, Dalasi';
  $currency->links = array(
    'http://www.xe.com/currency/gmd-gambian-dalasi',
    'http://www.gocurrency.com/countries/gambia.htm',
    'http://en.wikipedia.org/wiki/Gambian_dalasi',
  );
  $currency->code = 'GMD';
  $currency->num = 270;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['GMD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Guinea, Francs';
  $currency->links = array(
    'http://www.xe.com/currency/gnf-guinean-franc',
    'http://en.wikipedia.org/wiki/Guinean_franc',
  );
  $currency->code = 'GNF';
  $currency->num = 324;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['GNF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Guatemala, Quetzales';
  $currency->links = array(
    'http://www.xe.com/currency/gtq-guatemalan-quetzal',
    'http://www.gocurrency.com/countries/guatemala.htm',
    'http://en.wikipedia.org/wiki/Guatemalan_quetzal',
  );
  $currency->code = 'GTQ';
  $currency->num = 320;
  $currency->base = 100;
  $currency->symbol = 'Q';
  $currency->html_entity = '&#81;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['GTQ'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Guyana, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/gyd-guyanese-dollar',
    'http://www.gocurrency.com/countries/guyana.htm',
    'http://en.wikipedia.org/wiki/Guyanese_dollar',
  );
  $currency->code = 'GYD';
  $currency->num = 328;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['GYD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Hong Kong, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/hkd-hong-kong-dollar',
    'http://www.gocurrency.com/countries/hong_kong.htm',
    'http://en.wikipedia.org/wiki/Hong_Kong_dollar',
  );
  $currency->code = 'HKD';
  $currency->num = 344;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['HKD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Honduras, Lempiras';
  $currency->links = array(
    'http://www.xe.com/currency/hnl-honduran-lempira',
    'http://www.gocurrency.com/countries/honduras.htm',
    'http://en.wikipedia.org/wiki/Honduran_lempira',
  );
  $currency->code = 'HNL';
  $currency->num = 340;
  $currency->base = 100;
  $currency->symbol = 'L';
  $currency->html_entity = '&#76;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['HNL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Croatia, Kuna';
  $currency->links = array(
    'http://www.xe.com/currency/hrk-croatian-kuna',
    'http://www.gocurrency.com/countries/croatia.htm',
    'http://en.wikipedia.org/wiki/Croatian_kuna',
  );
  $currency->code = 'HRK';
  $currency->num = 191;
  $currency->base = 100;
  $currency->symbol = 'kn';
  $currency->html_entity = '&#107;&#110;';
  $currency->coinage_symbol = 'lipa';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['HRK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Haiti, Gourdes';
  $currency->links = array(
    'http://www.xe.com/currency/htg-haitian-gourde',
    'http://www.gocurrency.com/countries/haiti.htm',
    'http://en.wikipedia.org/wiki/Haitian_gourde',
  );
  $currency->code = 'HTG';
  $currency->num = 332;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['HTG'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Hungary, Forint';
  $currency->links = array(
    'http://www.xe.com/currency/huf-hungarian-forint',
    'http://www.gocurrency.com/countries/hungary.htm',
    'http://en.wikipedia.org/wiki/Hungarian_forint',
  );
  $currency->code = 'HUF';
  $currency->num = 348;
  $currency->base = 100;
  $currency->symbol = 'Ft';
  $currency->html_entity = '&#70;&#116;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_RIGHT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['HUF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Indonesia, Rupiahs';
  $currency->links = array(
    'http://www.xe.com/currency/idr-indonesian-rupiah',
    'http://www.gocurrency.com/countries/indonesia.htm',
    'http://en.wikipedia.org/wiki/Indonesian_rupiah',
  );
  $currency->code = 'IDR';
  $currency->num = 360;
  $currency->base = 100;
  $currency->symbol = 'Rp';
  $currency->html_entity = '&#82;&#112;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['IDR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Israel, New Shekels';
  $currency->links = array(
    'http://www.xe.com/currency/ils-israeli-shekel',
    'http://www.gocurrency.com/countries/israel.htm',
    'http://en.wikipedia.org/wiki/Israeli_new_sheqel',
  );
  $currency->code = 'ILS';
  $currency->num = 376;
  $currency->base = 100;
  $currency->symbol = '₪';
  $currency->html_entity = '&#8362;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_RIGHT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ILS'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Isle of Man, Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/imp-isle-of-man-pound',
    'http://en.wikipedia.org/wiki/Isle_of_Man_pound',
  );
  $currency->code = 'IMP';
  $currency->num = 0;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['IMP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'India, Rupees';
  $currency->links = array(
    'http://www.xe.com/currency/inr-indian-rupee',
    'http://www.gocurrency.com/countries/india.htm',
    'http://en.wikipedia.org/wiki/Indian_rupee',
  );
  $currency->code = 'INR';
  $currency->num = 356;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['INR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Iraq, Dinars';
  $currency->links = array(
    'http://www.xe.com/currency/iqd-iraqi-dinar',
    'http://en.wikipedia.org/wiki/Iraqi_dinar',
  );
  $currency->code = 'IQD';
  $currency->num = 368;
  $currency->base = 100;
  $currency->symbol = 'د.ع';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['IQD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Iran, Rials';
  $currency->links = array(
    'http://www.xe.com/currency/irr-iranian-rial',
    'http://www.gocurrency.com/countries/iran.htm',
    'http://en.wikipedia.org/wiki/Iranian_rial',
  );
  $currency->code = 'IRR';
  $currency->num = 364;
  $currency->base = 100;
  $currency->symbol = '﷼';
  $currency->html_entity = '&#65020;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['IRR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Iceland, Kronur';
  $currency->links = array(
    'http://www.xe.com/currency/isk-icelandic-krona',
    'http://www.gocurrency.com/countries/iceland.htm',
    'http://en.wikipedia.org/wiki/Icelandic_króna',
  );
  $currency->code = 'ISK';
  $currency->num = 352;
  $currency->base = 100;
  $currency->symbol = 'kr';
  $currency->html_entity = '&#107;&#114;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_RIGHT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ISK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Jersey, Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/jep-jersey-pound',
    'http://en.wikipedia.org/wiki/Jersey_pound',
  );
  $currency->code = 'JEP';
  $currency->num = 0;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['JEP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Jamaica, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/jmd-jamaican-dollar',
    'http://www.gocurrency.com/countries/jamaica.htm',
    'http://en.wikipedia.org/wiki/Jamaican_dollar',
  );
  $currency->code = 'JMD';
  $currency->num = 388;
  $currency->base = 100;
  $currency->symbol = 'J$';
  $currency->html_entity = '&#74;&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['JMD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Jordan, Dinars';
  $currency->links = array(
    'http://www.xe.com/currency/jod-jordanian-dinar',
    'http://www.gocurrency.com/countries/jordan.htm',
    'http://en.wikipedia.org/wiki/Jordanian_dinar',
  );
  $currency->code = 'JOD';
  $currency->num = 400;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['JOD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Japan, Yen';
  $currency->links = array(
    'http://www.xe.com/currency/jpy-japanese-yen',
    'http://www.gocurrency.com/countries/japan.htm',
    'http://en.wikipedia.org/wiki/Japanese_yen',
  );
  $currency->code = 'JPY';
  $currency->num = 392;
  $currency->base = 100;
  $currency->symbol = '¥';
  $currency->html_entity = '&yen;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['JPY'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Kenya, Shillings';
  $currency->links = array(
    'http://www.xe.com/currency/kes-kenyan-shilling',
    'http://www.gocurrency.com/countries/kenya.htm',
    'http://en.wikipedia.org/wiki/Kenyan_shilling',
  );
  $currency->code = 'KES';
  $currency->num = 404;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['KES'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Kyrgyzstan, Soms';
  $currency->links = array(
    'http://www.xe.com/currency/kgs-kyrgyzstani-som',
    'http://en.wikipedia.org/wiki/Kyrgyzstani_som',
  );
  $currency->code = 'KGS';
  $currency->num = 417;
  $currency->base = 100;
  $currency->symbol = 'лв';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['KGS'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Cambodia, Riels';
  $currency->links = array(
    'http://www.xe.com/currency/khr-cambodian-riel',
    'http://www.gocurrency.com/countries/cambodia.htm',
    'http://en.wikipedia.org/wiki/Cambodian_riel',
  );
  $currency->code = 'KHR';
  $currency->num = 116;
  $currency->base = 100;
  $currency->symbol = '៛';
  $currency->html_entity = '&#6107;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['KHR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Comoros, Francs';
  $currency->links = array(
    'http://www.xe.com/currency/kmf-comoran-franc',
    'http://www.gocurrency.com/countries/comoros.htm',
    'http://en.wikipedia.org/wiki/Comoro_franc',
  );
  $currency->code = 'KMF';
  $currency->num = 174;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['KMF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Korea (North), Won';
  $currency->links = array(
    'http://www.xe.com/currency/kpw-korean-won',
    'http://www.gocurrency.com/countries/north_korea.htm',
    'http://en.wikipedia.org/wiki/North_Korean_won',
  );
  $currency->code = 'KPW';
  $currency->num = 408;
  $currency->base = 100;
  $currency->symbol = '₩';
  $currency->html_entity = '&#8361;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['KPW'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Korea (South), Won';
  $currency->links = array(
    'http://www.xe.com/currency/krw-korean-won',
    'http://www.gocurrency.com/countries/south_korea.htm',
    'http://en.wikipedia.org/wiki/South_Korean_won',
  );
  $currency->code = 'KRW';
  $currency->num = 410;
  $currency->base = 100;
  $currency->symbol = '₩';
  $currency->html_entity = '&#8361;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_RIGHT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['KRW'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Kuwait, Dinars';
  $currency->links = array(
    'http://www.xe.com/currency/kwd-kuwaiti-dinar',
    'http://www.gocurrency.com/countries/kuwait.htm',
    'http://en.wikipedia.org/wiki/Kuwaiti_dinar',
  );
  $currency->code = 'KWD';
  $currency->num = 414;
  $currency->base = 1000;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['KWD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Cayman Islands, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/kyd-caymanian-dollar',
    'http://www.gocurrency.com/countries/cayman_islands.htm',
    'http://en.wikipedia.org/wiki/Cayman_Islands_dollar',
  );
  $currency->code = 'KYD';
  $currency->num = 136;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['KYD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Kazakhstan, Tenge';
  $currency->links = array(
    'http://www.xe.com/currency/kzt-kazakhstani-tenge',
    'http://www.gocurrency.com/countries/kazakhstan.htm',
    'http://en.wikipedia.org/wiki/Kazakhstani_tenge',
  );
  $currency->code = 'KZT';
  $currency->num = 398;
  $currency->base = 100;
  $currency->symbol = 'лв';
  $currency->html_entity = '&#1083;&#1074;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['KZT'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Laos, Kips';
  $currency->links = array(
    'http://www.xe.com/currency/lak-lao-or-laotian-kip',
    'http://www.gocurrency.com/countries/laos.htm',
    'http://en.wikipedia.org/wiki/Lao_kip',
  );
  $currency->code = 'LAK';
  $currency->num = 418;
  $currency->base = 100;
  $currency->symbol = '₭';
  $currency->html_entity = '&#8365;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['LAK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Lebanon, Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/lbp-lebanese-pound',
    'http://www.gocurrency.com/countries/lebanon.htm',
    'http://en.wikipedia.org/wiki/Lebanese_pound',
  );
  $currency->code = 'LBP';
  $currency->num = 422;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ' ';
  $currency->symbol_position = FORMAT_CURRENCY_RIGHT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['LBP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Sri Lanka, Rupees';
  $currency->links = array(
    'http://www.xe.com/currency/lkr-sri-lankan-rupee',
    'http://www.gocurrency.com/countries/sri_lanka.htm',
    'http://en.wikipedia.org/wiki/Sri_Lankan_rupee',
  );
  $currency->code = 'LKR';
  $currency->num = 144;
  $currency->base = 100;
  $currency->symbol = '₨';
  $currency->html_entity = '&#8360;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['LKR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Liberia, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/lrd-liberian-dollar',
    'http://www.gocurrency.com/countries/liberia.htm',
    'http://en.wikipedia.org/wiki/Liberian_dollar',
  );
  $currency->code = 'LRD';
  $currency->num = 430;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['LRD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Lesotho, Maloti';
  $currency->links = array(
    'http://www.xe.com/currency/lsl-basotho-loti',
    'http://www.gocurrency.com/countries/lesotho.htm',
    'http://en.wikipedia.org/wiki/Lesotho_loti',
  );
  $currency->code = 'LSL';
  $currency->num = 426;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['LSL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Lithuania, Litai';
  $currency->links = array(
    'http://www.xe.com/currency/ltl-lithuanian-litas',
    'http://www.gocurrency.com/countries/lithuania.htm',
    'http://en.wikipedia.org/wiki/Lithuanian_litas',
  );
  $currency->code = 'LTL';
  $currency->num = 440;
  $currency->base = 100;
  $currency->symbol = 'Lt';
  $currency->html_entity = '&#76;&#116;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['LTL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Latvia, Lati';
  $currency->links = array(
    'http://www.xe.com/currency/lvl-latvian-lat',
    'http://www.gocurrency.com/countries/latvia.htm',
    'http://en.wikipedia.org/wiki/Latvian_lats',
  );
  $currency->code = 'LVL';
  $currency->num = 428;
  $currency->base = 100;
  $currency->symbol = 'Ls';
  $currency->html_entity = '&#76;&#115;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['LVL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Libya, Dinars';
  $currency->links = array(
    'http://www.xe.com/currency/lyd-libyan-dinar',
    'http://www.gocurrency.com/countries/libya.htm',
    'http://en.wikipedia.org/wiki/Libyan_dinar',
  );
  $currency->code = 'LYD';
  $currency->num = 434;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['LYD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Morocco, Dirhams';
  $currency->links = array(
    'http://www.xe.com/currency/mad-moroccan-dirham',
    'http://www.gocurrency.com/countries/morocco.htm',
    'http://en.wikipedia.org/wiki/Moroccan_dirham',
  );
  $currency->code = 'MAD';
  $currency->num = 504;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MAD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Moldova, Lei';
  $currency->links = array(
    'http://www.xe.com/currency/mdl-moldovan-leu',
    'http://www.gocurrency.com/countries/moldova.htm',
    'http://en.wikipedia.org/wiki/Moldovan_leu',
  );
  $currency->code = 'MDL';
  $currency->num = 498;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MDL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Madagascar, Ariary';
  $currency->links = array(
    'http://www.xe.com/currency/mga-malagasy-ariary',
    'http://www.gocurrency.com/countries/madagascar.htm',
    'http://en.wikipedia.org/wiki/Malagasy_ariary',
  );
  $currency->code = 'MGA';
  $currency->num = 969;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MGA'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Macedonia, Denars';
  $currency->links = array(
    'http://www.xe.com/currency/mkd-macedonian-denar',
    'http://www.gocurrency.com/countries/macedonia.htm',
    'http://en.wikipedia.org/wiki/Macedonian_denar',
  );
  $currency->code = 'MKD';
  $currency->num = 807;
  $currency->base = 100;
  $currency->symbol = 'ден';
  $currency->html_entity = '&#1076;&#1077;&#1085;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MKD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Myanmar (Burma), Kyats';
  $currency->links = array(
    'http://www.xe.com/currency/mmk-burmese-kyat',
    'http://www.gocurrency.com/countries/myanmar.htm',
    'http://en.wikipedia.org/wiki/Myanma_kyat',
  );
  $currency->code = 'MMK';
  $currency->num = 104;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MMK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Mongolia, Tugriks';
  $currency->links = array(
    'http://www.xe.com/currency/mnt-mongolian-tughrik',
    'http://www.gocurrency.com/countries/mongolia.htm',
    'http://en.wikipedia.org/wiki/Mongolian_tögrög',
  );
  $currency->code = 'MNT';
  $currency->num = 496;
  $currency->base = 100;
  $currency->symbol = '₮';
  $currency->html_entity = '&#8366;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MNT'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Macau, Patacas';
  $currency->links = array(
    'http://www.xe.com/currency/mop-macau-pataca',
    'http://en.wikipedia.org/wiki/Macanese_pataca',
  );
  $currency->code = 'MOP';
  $currency->num = 446;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MOP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Mauritania, Ouguiyas';
  $currency->links = array(
    'http://www.xe.com/currency/mro-mauritian-ouguiya',
    'http://en.wikipedia.org/wiki/Mauritanian_ouguiya',
  );
  $currency->code = 'MRO';
  $currency->num = 478;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MRO'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Malta, Liri (expires 2008-Jan-31)';
  $currency->links = array(
    'http://en.wikipedia.org/wiki/Maltese_scudo',
  );
  $currency->code = 'MTL';
  $currency->num = 470;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MTL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Mauritius, Rupees';
  $currency->links = array(
    'http://www.xe.com/currency/mur-mauritian-rupee',
    'http://en.wikipedia.org/wiki/Mauritian_rupee',
  );
  $currency->code = 'MUR';
  $currency->num = 480;
  $currency->base = 100;
  $currency->symbol = '₨';
  $currency->html_entity = '&#8360;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MUR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Maldives (Maldive Islands), Rufiyaa';
  $currency->links = array(
    'http://www.xe.com/currency/mvr-maldivian-rufiyaa',
    'http://en.wikipedia.org/wiki/Maldivian_rufiyaa',
  );
  $currency->code = 'MVR';
  $currency->num = 462;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MVR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Malawi, Kwachas';
  $currency->links = array(
    'http://www.xe.com/currency/mwk-malawian-kwacha',
    'http://www.gocurrency.com/countries/malawi.htm',
    'http://en.wikipedia.org/wiki/Malawian_kwacha',
  );
  $currency->code = 'MWK';
  $currency->num = 454;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MWK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Mexico, Pesos';
  $currency->links = array(
    'http://www.xe.com/currency/mxn-mexican-peso',
    'http://www.gocurrency.com/countries/mexico.htm',
    'http://en.wikipedia.org/wiki/Mexican_peso',
  );
  $currency->code = 'MXN';
  $currency->num = 484;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MXN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Malaysia, Ringgits';
  $currency->links = array(
    'http://www.xe.com/currency/myr-malaysian-ringgit',
    'http://www.gocurrency.com/countries/malaysia.htm',
    'http://en.wikipedia.org/wiki/Malaysian_ringgit',
  );
  $currency->code = 'MYR';
  $currency->num = 458;
  $currency->base = 100;
  $currency->symbol = 'RM';
  $currency->html_entity = '&#82;&#77;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MYR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Mozambique, Meticais';
  $currency->links = array(
    'http://www.xe.com/currency/mzn-mozambican-metical',
    'http://www.gocurrency.com/countries/mozambique.htm',
    'http://en.wikipedia.org/wiki/Mozambican_metical',
  );
  $currency->code = 'MZN';
  $currency->num = 943;
  $currency->base = 100;
  $currency->symbol = 'MT';
  $currency->html_entity = '&#77;&#84;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['MZN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Namibia, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/nad-namibian-dollar',
    'http://www.gocurrency.com/countries/namibia.htm',
    'http://en.wikipedia.org/wiki/Namibian_dollar',
  );
  $currency->code = 'NAD';
  $currency->num = 516;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['NAD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Nigeria, Nairas';
  $currency->links = array(
    'http://www.xe.com/currency/ngn-nigerian-naira',
    'http://www.gocurrency.com/countries/nigeria.htm',
    'http://en.wikipedia.org/wiki/Nigerian_naira',
  );
  $currency->code = 'NGN';
  $currency->num = 566;
  $currency->base = 100;
  $currency->symbol = '₦';
  $currency->html_entity = '&#8358;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['NGN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Nicaragua, Cordobas';
  $currency->links = array(
    'http://www.xe.com/currency/nio-nicaraguan-cordoba',
    'http://www.gocurrency.com/countries/nicaragua.htm',
    'http://en.wikipedia.org/wiki/Cordoba_oro',
  );
  $currency->code = 'NIO';
  $currency->num = 558;
  $currency->base = 100;
  $currency->symbol = 'C$';
  $currency->html_entity = '&#67;&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['NIO'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Norway, Krone';
  $currency->links = array(
    'http://www.xe.com/currency/nok-norwegian-krone',
    'http://www.gocurrency.com/countries/norway.htm',
    'http://en.wikipedia.org/wiki/Norwegian_krone',
  );
  $currency->code = 'NOK';
  $currency->num = 578;
  $currency->base = 100;
  $currency->symbol = 'kr';
  $currency->html_entity = '&#107;&#114;';
  $currency->coinage_symbol = 'Øre';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['NOK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Nepal, Nepal Rupees';
  $currency->links = array(
    'http://www.xe.com/currency/npr-nepalese-rupee',
    'http://www.gocurrency.com/countries/nepal.htm',
    'http://en.wikipedia.org/wiki/Nepalese_rupee',
  );
  $currency->code = 'NPR';
  $currency->num = 524;
  $currency->base = 100;
  $currency->symbol = '₨';
  $currency->html_entity = '&#8360;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['NPR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'New Zealand, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/nzd-new-zealand-dollar',
    'http://www.gocurrency.com/countries/new_zealand.htm',
    'http://en.wikipedia.org/wiki/New_Zealand_dollar',
  );
  $currency->code = 'NZD';
  $currency->num = 554;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['NZD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Oman, Rials';
  $currency->links = array(
    'http://www.xe.com/currency/omr-omani-rial',
    'http://www.gocurrency.com/countries/oman.htm',
    'http://en.wikipedia.org/wiki/Omani_rial',
  );
  $currency->code = 'OMR';
  $currency->num = 512;
  $currency->base = 100;
  $currency->symbol = '﷼';
  $currency->html_entity = '&#65020;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['OMR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Panama, Balboa';
  $currency->links = array(
    'http://www.xe.com/currency/pab-panamanian-balboa',
    'http://www.gocurrency.com/countries/panama.htm',
    'http://en.wikipedia.org/wiki/Panamanian_balboa',
  );
  $currency->code = 'PAB';
  $currency->num = 590;
  $currency->base = 100;
  $currency->symbol = 'B/.';
  $currency->html_entity = '&#66;&#47;&#46;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['PAB'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Peru, Nuevos Soles';
  $currency->links = array(
    'http://www.xe.com/currency/pen-peruvian-nuevo-sol',
    'http://www.gocurrency.com/countries/peru.htm',
    'http://en.wikipedia.org/wiki/Peruvian_nuevo_sol',
  );
  $currency->code = 'PEN';
  $currency->num = 604;
  $currency->base = 100;
  $currency->symbol = 'S/.';
  $currency->html_entity = '&#83;&#47;&#46;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['PEN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Papua New Guinea, Kina';
  $currency->links = array(
    'http://www.xe.com/currency/pgk-papua-new-guinean-kina',
    'http://www.gocurrency.com/countries/papua_new_guinea.htm',
    'http://en.wikipedia.org/wiki/Papua_New_Guinean_kina',
  );
  $currency->code = 'PGK';
  $currency->num = 598;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['PGK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Philippines, Pesos';
  $currency->links = array(
    'http://www.xe.com/currency/php-philippine-peso',
    'http://en.wikipedia.org/wiki/Philippine_peso',
  );
  $currency->code = 'PHP';
  $currency->num = 608;
  $currency->base = 100;
  $currency->symbol = 'Php';
  $currency->html_entity = '&#80;&#104;&#112;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['PHP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Pakistan, Rupees';
  $currency->links = array(
    'http://www.xe.com/currency/pkr-pakistani-rupee',
    'http://www.gocurrency.com/countries/pakistan.htm',
    'http://en.wikipedia.org/wiki/Pakistani_rupee',
  );
  $currency->code = 'PKR';
  $currency->num = 586;
  $currency->base = 100;
  $currency->symbol = '₨';
  $currency->html_entity = '&#8360;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['PKR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Poland, Zlotych';
  $currency->links = array(
    'http://www.xe.com/currency/pln-polish-zloty',
    'http://en.wikipedia.org/wiki/Polish_złoty',
  );
  $currency->code = 'PLN';
  $currency->num = 985;
  $currency->base = 100;
  $currency->symbol = 'zł';
  $currency->html_entity = '&#122;&#322;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = ' ';
  $currency->symbol_position = FORMAT_CURRENCY_RIGHT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['PLN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Paraguay, Guarani';
  $currency->links = array(
    'http://www.xe.com/currency/pyg-paraguayan-guarani',
    'http://www.gocurrency.com/countries/paraguay.htm',
    'http://en.wikipedia.org/wiki/Paraguayan_guaran%C3%AD',
  );
  $currency->code = 'PYG';
  $currency->num = 600;
  $currency->base = 100;
  $currency->symbol = 'Gs';
  $currency->html_entity = '&#71;&#115;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['PYG'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Qatar, Rials';
  $currency->links = array(
    'http://www.xe.com/currency/qar-qatari-riyal',
    'http://www.gocurrency.com/countries/qatar.htm',
    'http://en.wikipedia.org/wiki/Qatari_rial',
  );
  $currency->code = 'QAR';
  $currency->num = 634;
  $currency->base = 100;
  $currency->symbol = '﷼';
  $currency->html_entity = '&#65020;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['QAR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Romania, New Lei';
  $currency->links = array(
    'http://www.xe.com/currency/ron-romanian-new-leu',
    'http://www.gocurrency.com/countries/romania.htm',
    'http://en.wikipedia.org/wiki/Romanian_Leu',
  );
  $currency->code = 'RON';
  $currency->num = 946;
  $currency->base = 100;
  $currency->symbol = 'lei';
  $currency->html_entity = '&#108;&#101;&#105;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['RON'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Serbia, Dinars';
  $currency->links = array(
    'http://www.xe.com/currency/rsd-serbian-dinar',
    'http://www.gocurrency.com/countries/serbia.htm',
    'http://en.wikipedia.org/wiki/Serbian_dinar',
  );
  $currency->code = 'RSD';
  $currency->num = 941;
  $currency->base = 100;
  $currency->symbol = 'Дин.';
  $currency->html_entity = '&#1044;&#1080;&#1085;&#46;';
  $currency->coinage_symbol = 'paras';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['RSD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Russia, Rubles';
  $currency->links = array(
    'http://www.xe.com/currency/rub-russian-ruble',
    'http://www.gocurrency.com/countries/russia.htm',
    'http://en.wikipedia.org/wiki/Russian_rouble',
  );
  $currency->code = 'RUB';
  $currency->num = 643;
  $currency->base = 100;
  $currency->symbol = 'руб';
  $currency->html_entity = '&#1088;&#1091;&#1073;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['RUB'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Rwanda, Rwanda Francs';
  $currency->links = array(
    'http://www.xe.com/currency/rwf-rwandan-franc',
    'http://www.gocurrency.com/countries/rwanda.htm',
    'http://en.wikipedia.org/wiki/Rwandan_franc',
  );
  $currency->code = 'RWF';
  $currency->num = 646;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['RWF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Saudi Arabia, Riyals';
  $currency->links = array(
    'http://www.xe.com/currency/sar-saudi-or-saudi-arabian-riyal',
    'http://www.gocurrency.com/countries/saudi_arabia.htm',
    'http://en.wikipedia.org/wiki/Saudi_riyal',
  );
  $currency->code = 'SAR';
  $currency->num = 682;
  $currency->base = 100;
  $currency->symbol = '﷼';
  $currency->html_entity = '&#65020;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SAR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Solomon Islands, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/sbd-solomon-islander-dollar',
    'http://www.gocurrency.com/countries/solomon_islands.htm',
    'http://en.wikipedia.org/wiki/Solomon_Islands_dollar',
  );
  $currency->code = 'SBD';
  $currency->num = 90;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SBD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Seychelles, Rupees';
  $currency->links = array(
    'http://www.xe.com/currency/scr-seychellois-rupee',
    'http://www.gocurrency.com/countries/seychelles.htm',
    'http://en.wikipedia.org/wiki/Seychelles_rupee',
  );
  $currency->code = 'SCR';
  $currency->num = 690;
  $currency->base = 100;
  $currency->symbol = '₨';
  $currency->html_entity = '&#8360;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SCR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Sudan, Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/sdg-sudanese-pound',
    'http://www.gocurrency.com/countries/sudan.htm',
    'http://en.wikipedia.org/wiki/Sudanese_pound',
  );
  $currency->code = 'SDG';
  $currency->num = 938;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SDG'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Sweden, Kronor';
  $currency->links = array(
    'http://www.xe.com/currency/sek-swedish-krona',
    'http://www.gocurrency.com/countries/sweden.htm',
    'http://en.wikipedia.org/wiki/Swedish_krona',
  );
  $currency->code = 'SEK';
  $currency->num = 752;
  $currency->base = 100;
  $currency->symbol = 'kr';
  $currency->html_entity = '&#107;&#114;';
  $currency->coinage_symbol = 'öre';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ' ';
  $currency->symbol_position = FORMAT_CURRENCY_RIGHT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SEK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Singapore, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/sgd-singapore-dollar',
    'http://www.gocurrency.com/countries/singapore.htm',
    'http://en.wikipedia.org/wiki/Singapore_dollar',
  );
  $currency->code = 'SGD';
  $currency->num = 702;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SGD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Saint Helena, Pounds';
  $currency->links = array(
    'http://www.gocurrency.com/countries/saint_helena.htm',
    'http://en.wikipedia.org/wiki/Saint_Helena_pound',
  );
  $currency->code = 'SHP';
  $currency->num = 654;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SHP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Sierra Leone, Leones';
  $currency->links = array(
    'http://www.xe.com/currency/sll-sierra-leonean-leone',
    'http://www.gocurrency.com/countries/sierra_leone.htm',
    'http://en.wikipedia.org/wiki/Sierra_Leonean_leone',
  );
  $currency->code = 'SLL';
  $currency->num = 694;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SLL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Somalia, Shillings';
  $currency->links = array(
    'http://www.xe.com/currency/sos-somali-shilling',
    'http://www.gocurrency.com/countries/somalia.htm',
    'http://en.wikipedia.org/wiki/Somali_shilling',
  );
  $currency->code = 'SOS';
  $currency->num = 706;
  $currency->base = 100;
  $currency->symbol = 'S';
  $currency->html_entity = '&#83;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SOS'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Seborga, Luigini';
  $currency->links = array(
    'http://www.xe.com/currency/spl-seborgan-luigino',
    'http://en.wikipedia.org/wiki/Seborga_luigino',
  );
  $currency->code = 'SPL';
  $currency->num = 0;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SPL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Suriname, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/srd-surinamese-dollar',
    'http://www.gocurrency.com/countries/suriname.htm',
    'http://en.wikipedia.org/wiki/Surinamese_dollar',
  );
  $currency->code = 'SRD';
  $currency->num = 968;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SRD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'São Tome and Principe, Dobras';
  $currency->links = array(
    'http://www.xe.com/currency/std-sao-tomean-dobra',
    'http://en.wikipedia.org/wiki/São_Tomé_and_Pr%C3%ADncipe_dobra',
  );
  $currency->code = 'STD';
  $currency->num = 678;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['STD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'El Salvador, Colones';
  $currency->links = array(
    'http://www.xe.com/currency/svc-salvadoran-colon',
    'http://www.gocurrency.com/countries/el_salvador.htm',
  );
  $currency->code = 'SVC';
  $currency->num = 222;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_EXPIRED;
  $export['SVC'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Syria, Pounds';
  $currency->links = array(
    'http://www.xe.com/currency/syp-syrian-pound',
    'http://www.gocurrency.com/countries/syria.htm',
    'http://en.wikipedia.org/wiki/Syrian_pound',
  );
  $currency->code = 'SYP';
  $currency->num = 760;
  $currency->base = 100;
  $currency->symbol = '£';
  $currency->html_entity = '&pound;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SYP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Swaziland, Emalangeni';
  $currency->links = array(
    'http://www.xe.com/currency/szl-swazi-lilangeni',
    'http://en.wikipedia.org/wiki/Lilangeni',
  );
  $currency->code = 'SZL';
  $currency->num = 748;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['SZL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Thailand, Baht';
  $currency->links = array(
    'http://www.xe.com/currency/thb-thai-baht',
    'http://www.gocurrency.com/countries/thailand.htm',
    'http://en.wikipedia.org/wiki/Thai_baht',
  );
  $currency->code = 'THB';
  $currency->num = 764;
  $currency->base = 100;
  $currency->symbol = '฿';
  $currency->html_entity = '';
  $currency->coinage_symbol = 'satang';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['THB'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Tajikistan, Somoni';
  $currency->links = array(
    'http://www.xe.com/currency/tjs-tajikistani-somoni',
    'http://en.wikipedia.org/wiki/Tajikistani_somoni',
  );
  $currency->code = 'TJS';
  $currency->num = 972;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['TJS'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Turkmenistan, Manats';
  $currency->links = array(
    'http://www.gocurrency.com/countries/turkmenistan.htm',
    'http://en.wikipedia.org/wiki/Turkmenistani_manat',
  );
  $currency->code = 'TMM';
  $currency->num = 795;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_EXPIRED;
  $export['TMM'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Turkmenistan, New Manats';
  $currency->links = array(
    'http://www.xe.com/currency/tmt-turkmenistani-manat',
    'http://www.gocurrency.com/countries/turkmenistan.htm',
    'http://en.wikipedia.org/wiki/Turkmenistani_manat',
  );
  $currency->code = 'TMT';
  $currency->num = 934;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['TMT'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Tunisia, Dinars';
  $currency->links = array(
    'http://www.xe.com/currency/tnd-tunisian-dinar',
    'http://www.gocurrency.com/countries/tunisia.htm',
    'http://en.wikipedia.org/wiki/Tunisian_dinar',
  );
  $currency->code = 'TND';
  $currency->num = 788;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['TND'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Tonga, Pa\'anga';
  $currency->links = array(
    "http://www.xe.com/currency/top-tongan-pa'anga",
    'http://www.gocurrency.com/countries/tonga.htm',
    'http://en.wikipedia.org/wiki/Tongan_paʻanga',
  );
  $currency->code = 'TOP';
  $currency->num = 776;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['TOP'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Turkey, New Lira';
  $currency->links = array(
    'http://www.xe.com/currency/try-turkish-lira',
    'http://www.gocurrency.com/countries/turkey.htm',
    'http://en.wikipedia.org/wiki/Turkish_lira',
  );
  $currency->code = 'TRY';
  $currency->num = 949;
  $currency->base = 100;
  $currency->symbol = 'TL';
  $currency->html_entity = '&#84;&#76;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['TRY'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Trinidad and Tobago, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/ttd-trinidadian-dollar',
    'http://www.gocurrency.com/countries/trinidad_and_tobago.htm',
    'http://en.wikipedia.org/wiki/Trinidad_and_Tobago_dollar',
  );
  $currency->code = 'TTD';
  $currency->num = 780;
  $currency->base = 100;
  $currency->symbol = 'TT$';
  $currency->html_entity = '&#84;&#84;&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['TTD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Tuvalu, Tuvalu Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/tvd-tuvaluan-dollar',
  );
  $currency->code = 'TVD';
  $currency->num = 0;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['TVD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Taiwan, New Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/twd-taiwan-new-dollar',
    'http://www.gocurrency.com/countries/taiwan.htm',
    'http://en.wikipedia.org/wiki/New_Taiwan_dollar',
  );
  $currency->code = 'TWD';
  $currency->num = 901;
  $currency->base = 100;
  $currency->symbol = 'NT$';
  $currency->html_entity = '&#78;&#84;&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['TWD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Tanzania, Shillings';
  $currency->links = array(
    'http://www.xe.com/currency/tzs-tanzanian-shilling',
    'http://www.gocurrency.com/countries/tanzania.htm',
    'http://en.wikipedia.org/wiki/Tanzanian_shilling',
  );
  $currency->code = 'TZS';
  $currency->num = 834;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['TZS'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Ukraine, Hryvnia';
  $currency->links = array(
    'http://www.xe.com/currency/uah-ukrainian-hryvna',
    'http://www.gocurrency.com/countries/ukraine.htm',
    'http://en.wikipedia.org/wiki/Ukrainian_hryvnia',
  );
  $currency->code = 'UAH';
  $currency->num = 980;
  $currency->base = 100;
  $currency->symbol = '₴';
  $currency->html_entity = '&#8372;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['UAH'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Uganda, Shillings';
  $currency->links = array(
    'http://www.xe.com/currency/ugx-ugandan-shilling',
    'http://www.gocurrency.com/countries/uganda.htm',
    'http://en.wikipedia.org/wiki/Ugandan_shilling',
  );
  $currency->code = 'UGX';
  $currency->num = 800;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['UGX'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'United States of America, Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/usd-us-dollar',
    'http://www.gocurrency.com/countries/united_states.htm',
    'http://en.wikipedia.org/wiki/United_States_dollar',
  );
  $currency->code = 'USD';
  $currency->num = 840;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '¢';
  $currency->coinage_html_entity = '&cent;';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['USD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Uruguay, Pesos';
  $currency->links = array(
    'http://www.xe.com/currency/uyu-uruguayan-peso',
    'http://www.gocurrency.com/countries/uruguay.htm',
    'http://en.wikipedia.org/wiki/Uruguayan_peso',
  );
  $currency->code = 'UYU';
  $currency->num = 858;
  $currency->base = 100;
  $currency->symbol = '$U';
  $currency->html_entity = '&#36;&#85;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['UYU'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Uzbekistan, Sums';
  $currency->links = array(
    'http://www.xe.com/currency/uzs-uzbekistani-som',
    'http://www.gocurrency.com/countries/uzbekistan.htm',
    'http://en.wikipedia.org/wiki/Uzbekistan_som',
  );
  $currency->code = 'UZS';
  $currency->num = 860;
  $currency->base = 100;
  $currency->symbol = 'лв';
  $currency->html_entity = '&#1083;&#1074;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['UZS'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Venezuela, Bolivares (expires 2008-Jun-30)';
  $currency->links = array(
    'http://www.xe.com/currency/veb-venezuelan-bolivar',
    'http://www.gocurrency.com/countries/venezuela.htm',
    'http://en.wikipedia.org/wiki/Venezuelan_bol%C3%ADvar',
  );
  $currency->code = 'VEB';
  $currency->num = 862;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = ',';
  $currency->thousands_separator = '.';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_EXPIRED;
  $export['VEB'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Venezuela, Bolivares Fuertes';
  $currency->links = array(
    'http://www.xe.com/currency/vef-venezuelan-bolivar-fuerte',
    'http://www.gocurrency.com/countries/venezuela.htm',
    'http://en.wikipedia.org/wiki/Venezuelan_bol%C3%ADvar',
  );
  $currency->code = 'VEF';
  $currency->num = 937;
  $currency->base = 100;
  $currency->symbol = 'Bs';
  $currency->html_entity = '&#66;&#115;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['VEF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Viet Nam, Dong';
  $currency->links = array(
    'http://www.xe.com/currency/vnd-vietnamese-dong',
    'http://www.gocurrency.com/countries/vietnam.htm',
    'http://en.wikipedia.org/wiki/Vietnamese_đồng',
  );
  $currency->code = 'VND';
  $currency->num = 704;
  $currency->base = 100;
  $currency->symbol = '₫';
  $currency->html_entity = '&#8363;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['VND'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Vanuatu, Vatu';
  $currency->links = array(
    'http://www.xe.com/currency/vuv-ni-vanuatu-vatu',
    'http://www.gocurrency.com/countries/vanuatu.htm',
    'http://en.wikipedia.org/wiki/Vanuatu_vatu',
  );
  $currency->code = 'VUV';
  $currency->num = 548;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['VUV'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Samoa, Tala';
  $currency->links = array(
    'http://www.xe.com/currency/wst-samoan-tala',
    'http://www.gocurrency.com/countries/samoa.htm',
    'http://en.wikipedia.org/wiki/Samoan_tala',
  );
  $currency->code = 'WST';
  $currency->num = 882;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['WST'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Communauté Financière Africaine BEAC, Francs';
  $currency->links = array(
    'http://www.xe.com/currency/xaf-central-african-cfa-franc-beac',
    'http://en.wikipedia.org/wiki/Central_African_CFA_franc',
  );
  $currency->code = 'XAF';
  $currency->num = 950;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['XAF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Silver, Ounces';
  $currency->links = array(
    'http://en.wikipedia.org/wiki/Silver',
  );
  $currency->code = 'XAG';
  $currency->num = 961;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['XAG'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Gold, Ounces';
  $currency->links = array(
    'http://en.wikipedia.org/wiki/Gold',
  );
  $currency->code = 'XAU';
  $currency->num = 959;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['XAU'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'East Caribbean Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/xcd-east-caribbean-dollar',
    'http://www.gocurrency.com/countries/east_caribbean.htm',
    'http://en.wikipedia.org/wiki/East_Caribbean_dollar',
  );
  $currency->code = 'XCD';
  $currency->num = 951;
  $currency->base = 100;
  $currency->symbol = '$';
  $currency->html_entity = '&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['XCD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'International Monetary Fund (IMF) Special Drawing Rights';
  $currency->links = array(
    'http://www.xe.com/currency/xdr-imf-special-drawing-rights',
    'http://en.wikipedia.org/wiki/Special_Drawing_Rights',
  );
  $currency->code = 'XDR';
  $currency->num = 960;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['XDR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Communauté Financière Africaine BCEAO, Francs';
  $currency->links = array(
    'http://www.xe.com/currency/xof-cfa-franc',
    'http://en.wikipedia.org/wiki/West_African_CFA_franc',
  );
  $currency->code = 'XOF';
  $currency->num = 952;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['XOF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Palladium Ounces';
  $currency->links = array(
    'http://en.wikipedia.org/wiki/Palladium',
  );
  $currency->code = 'XPD';
  $currency->num = 964;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['XPD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Comptoirs Français du Pacifique Francs';
  $currency->links = array(
    'http://www.xe.com/currency/xpf-cfp-franc',
    'http://en.wikipedia.org/wiki/CFP_franc',
  );
  $currency->code = 'XPF';
  $currency->num = 953;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['XPF'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Platinum, Ounces';
  $currency->links = array(
    'http://en.wikipedia.org/wiki/Platinum',
  );
  $currency->code = 'XPT';
  $currency->num = 962;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['XPT'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Yemen, Rials';
  $currency->links = array(
    'http://www.xe.com/currency/yer-yemeni-rial',
    'http://www.gocurrency.com/countries/yemen.htm',
    'http://en.wikipedia.org/wiki/Yemeni_rial',
  );
  $currency->code = 'YER';
  $currency->num = 886;
  $currency->base = 100;
  $currency->symbol = '﷼';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['YER'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'South Africa, Rand';
  $currency->links = array(
    'http://www.xe.com/currency/zar-south-african-rand',
    'http://www.gocurrency.com/countries/south_africa.htm',
    'http://en.wikipedia.org/wiki/South_African_rand',
  );
  $currency->code = 'ZAR';
  $currency->num = 710;
  $currency->base = 100;
  $currency->symbol = 'R';
  $currency->html_entity = '&#82;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ZAR'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Zambia, Kwacha';
  $currency->links = array(
    'http://www.xe.com/currency/zmk-zambian-kwacha',
    'http://www.gocurrency.com/countries/zambia.htm',
    'http://en.wikipedia.org/wiki/Zambian_kwacha',
  );
  $currency->code = 'ZMK';
  $currency->num = 894;
  $currency->base = 100;
  $currency->symbol = '';
  $currency->html_entity = '';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 0;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ZMK'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Zimbabwe, Zimbabwe Dollars';
  $currency->links = array(
    'http://www.xe.com/currency/zwd-zimbabwean-dollar',
    'http://www.gocurrency.com/countries/zimbabwe.htm',
    'http://en.wikipedia.org/wiki/Zimbabwe_dollar',
  );
  $currency->code = 'ZWD';
  $currency->num = 716;
  $currency->base = 100;
  $currency->symbol = 'Z$';
  $currency->html_entity = 'Z&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_EXPIRED;
  $export['ZWD'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Zimbabwe, Zimbabwe Dollars';
  $currency->links = array(
    'http://www.gocurrency.com/countries/zimbabwe.htm',
    'http://en.wikipedia.org/wiki/Zimbabwe_dollar',
  );
  $currency->code = 'ZWL';
  $currency->num = 932;
  $currency->base = 100;
  $currency->symbol = 'Z$';
  $currency->html_entity = 'Z&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_ACTIVE;
  $export['ZWL'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Zimbabwe, Zimbabwe Dollars';
  $currency->links = array(
    'http://www.gocurrency.com/countries/zimbabwe.htm',
    'http://en.wikipedia.org/wiki/Zimbabwe_dollar',
  );
  $currency->code = 'ZWN';
  $currency->num = 942;
  $currency->base = 100;
  $currency->symbol = 'Z$';
  $currency->html_entity = 'Z&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_EXPIRED;
  $export['ZWN'] = $currency;

  $currency = new stdClass();
  $currency->disabled = FALSE; /* Edit this to true to make a default currency disabled initially */
  $currency->api_version = 1;
  $currency->name = 'Zimbabwe, Zimbabwe Dollars';
  $currency->links = array(
    'http://www.gocurrency.com/countries/zimbabwe.htm',
    'http://en.wikipedia.org/wiki/Zimbabwe_dollar',
  );
  $currency->code = 'ZWR';
  $currency->num = 935;
  $currency->base = 100;
  $currency->symbol = 'Z$';
  $currency->html_entity = 'Z&#36;';
  $currency->coinage_symbol = '';
  $currency->coinage_html_entity = '';
  $currency->decimal_places = 2;
  $currency->decimal_separator = '.';
  $currency->thousands_separator = ',';
  $currency->symbol_position = FORMAT_CURRENCY_LEFT;
  $currency->coinage_position = FORMAT_CURRENCY_RIGHT;
  $currency->status = FORMAT_CURRENCY_EXPIRED;
  $export['ZWR'] = $currency;

  return $export;
}
