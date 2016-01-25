<?php
/**
 * @file
 * A sample for countries config file.
 */

/**
 * Gets the countries for a certain continent.
 *
 * @param string $continent
 *   The continent machine name.
 *
 * @return array
 *   The array with countries if there are any for the required continent, empty
 *   array if not.
 */
function forms_ajax_test_get_countries($continent) {
  $countries = array(
    'europe' => array(
      'uk' => t('United Kingdom of Great Britain and Northern Ireland'),
      'france' => t("France"),
      'germany' => t('Germany'),
      'austria' => t('Austria'),
      'finland' => t('Republic of Finland'),
      'romania' => t('Romania'),
      'czech' => t('Czech Republic'),
    ),
    'south_america' => array(
      'chile' => t('Republic of Chile'),
      'peru' => t('Republic of Peru'),
      'bolivia' => t('Plurinational State of Bolivia'),
      'argentina' => t('Argentine Republic'),
      'brasil' => t('Federative Republic of Brazil'),
      'venezuela' => t('Bolivarian Republic of Venezuela'),
      'ecuador' => t('Republic of Ecuador'),
    ),
    'north_america' => array(
      'usa' => t('United States of America'),
      'mexico' => t('United Mexican States'),
      'canada' => t('Canada'),
      'cuba' => t('Republic of Cuba'),
      'panama' => t('Republic of Panama'),
    ),
    'africa' => array(
      'egipt' => t('Arab Republic of Egypt'),
      'morroco' => t('Kingdom of Morocco'),
      'niger' => t('Republic of Niger'),
      'nigeria' => t('Federal Republic of Nigeria'),
      'south_africa' => t('Republic of South Africas'),
    ),
    'asia' => array(
      'china' => t("People's Republic of China"),
      'japan' => t('Japan'),
      'mongolia' => t('Mongolia'),
      'india' => t('Republic of India'),
      'south_korea' => t('Republic of Korea'),
    ),
    'australia' => array(
      'australia' => t('Commonwealth of Australia'),
    )
  );

  return isset($countries[$continent]) ? $countries[$continent] : array();
}