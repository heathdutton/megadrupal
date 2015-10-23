<?php

function services_addressfield_services_resources() {
  $resources = array(
    'services_addressfield' => array(
      'actions' => array(

        'country_get_list' => array(
          'help' => t('Get list of all predefined and custom countries.'),
          'file' => array(
            'type' => 'inc',
            'module' => 'services_addressfield',
            'name' => 'services_addressfield.resource',
          ),
          'callback' => 'services_addressfield_country_get_list',
          'args' => array(),
          'access callback' => '_services_addressfield_resource_access',
          'access callback file' => array(
            'type' => 'inc',
            'module' => 'services_addressfield',
            'name' => 'services_addressfield.resource',
          ),
          'access arguments' => array('access content'),
          'access arguments append' => TRUE,
        ),

        'get_address_format' => array(
          'help' => t('Returns the address format for the given country code.'),
          'file' => array(
            'type' => 'inc',
            'module' => 'services_addressfield',
            'name' => 'services_addressfield.resource',
          ),
          'callback' => 'services_addressfield_get_address_format',
          'args' => array(
            array(
              'name'         => 'country_code',
              'type'         => 'string',
              'description'  => t('The country code.'),
              'source'       => array('data' => 'country_code'),
              'optional'     => FALSE,
            ),
          ),
          'access callback' => '_services_addressfield_resource_access',
          'access callback file' => array(
            'type' => 'inc',
            'module' => 'services_addressfield',
            'name' => 'services_addressfield.resource',
          ),
          'access arguments' => array('access content'),
          'access arguments append' => TRUE,
        ),
        
        'get_administrative_areas' => array(
          'help' => t('Returns the predefined administrative areas for the given country code'),
          'file' => array(
            'type' => 'inc',
            'module' => 'services_addressfield',
            'name' => 'services_addressfield.resource',
          ),
          'callback' => 'services_addressfield_get_administrative_areas',
          'args' => array(
            array(
              'name'         => 'country_code',
              'type'         => 'string',
              'description'  => t('The country code.'),
              'source'       => array('data' => 'country_code'),
              'optional'     => FALSE,
            ),
          ),
          'access callback' => '_services_addressfield_resource_access',
          'access callback file' => array(
            'type' => 'inc',
            'module' => 'services_addressfield',
            'name' => 'services_addressfield.resource',
          ),
          'access arguments' => array('access content'),
          'access arguments append' => TRUE,
        ),
        
        'get_address_format_and_administrative_areas' => array(
          'help' => t('Returns the address format and administrative areas for the given country code.'),
          'file' => array(
            'type' => 'inc',
            'module' => 'services_addressfield',
            'name' => 'services_addressfield.resource',
          ),
          'callback' => 'services_addressfield_get_address_format_and_administrative_areas',
          'args' => array(
            array(
              'name'         => 'country_code',
              'type'         => 'string',
              'description'  => t('The country code.'),
              'source'       => array('data' => 'country_code'),
              'optional'     => FALSE,
            ),
          ),
          'access callback' => '_services_addressfield_resource_access',
          'access callback file' => array(
            'type' => 'inc',
            'module' => 'services_addressfield',
            'name' => 'services_addressfield.resource',
          ),
          'access arguments' => array('access content'),
          'access arguments append' => TRUE,
        ),

      ),
    ),
  );
  return $resources;
}

