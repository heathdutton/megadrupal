<?php

/**
 * @file
 * PMB reader account summary template.
 */

$header = array();
$rows = array();

$rows[] = array(
  t('Last Name'),
  check_plain($reader->personal_information->lastname),
);
$rows[] = array(
  t('First Name'),
  check_plain($reader->personal_information->firstname),
);
$rows[] = array(
  t('Bar code'),
  check_plain($reader->cb),
);
$rows[] = array(
  t('Join date'),
  $reader->adhesion_date,
);
$rows[] = array(
  t('Expiration date'),
  $reader->expiration_date,
);

$template .= theme('table', array('header' => $header, 'rows' => $rows));
