<?php
/**
 * @file
 * Describe key table to views.
 */

function pgp_contact_views_data() {
  $data = array();

  $data['pgp_contact_pub_keys'] = array(
    'table' => array(
      'group' => t('User'),
      'join' => array(
        'users' => array(
          'left_field' => 'uid',
          'field' => 'uid',
        ),
      ),
    ),
    'algorithm' => array(
      'title' => t('Algorithm'),
      'help' => t('User\'s PGP key algorithm.'),
      'field' => array(
        'handler' => 'views_handler_field'
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'fingerprint' => array(
      'title' => t('Fingerprint'),
      'help' => t('User\'s PGP key fingerprint.'),
      'field' => array(
        'handler' => 'views_handler_field'
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'public_key' => array(
      'title' => t('Public key'),
      'help' => t('User\'s public PGP key.'),
      'field' => array(
        'handler' => 'views_handler_field'
      ),
    ),
  );

  return $data;
}
