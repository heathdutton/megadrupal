<?php

/**
 * Implements hook_services_resources().
 */
function services_contact_services_resources() {
  $resources = array(
    'contact' => array(
      'actions' => array(
        'site' => array(
          'help' => t('Submits the site-wide contact form.'),
          'file' => array(
            'type' => 'inc',
            'module' => 'services_contact',
            'name' => 'services_contact.resource',
          ),
          'callback' => 'services_contact_site',
          'args' => array(
            array(
              'name' => 'name',
              'type' => 'string',
              'description' => t('The contact name.'),
              'source' => array('data' => 'name'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'mail',
              'type' => 'string',
              'description' => t('The contact e-mail.'),
              'source' => array('data' => 'mail'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'subject',
              'type' => 'string',
              'description' => t('The contact subject.'),
              'source' => array('data' => 'subject'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'cid',
              'type' => 'int',
              'description' => t('The contact category id.'),
              'source' => array('data' => 'cid'),
              'optional' => TRUE,
            ),
            array(
              'name' => 'message',
              'type' => 'string',
              'description' => t('The contact message.'),
              'source' => array('data' => 'message'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'copy',
              'type' => 'int',
              'description' => t('Send a copy to yourself.'),
              'source' => array('data' => 'copy'),
              'optional' => TRUE,
            )
          ),
          'access callback' => '_contact_resource_access',
          'access callback file' => array(
            'type' => 'inc',
            'module' => 'services_contact',
            'name' => 'services_contact.resource',
          ),
          'access arguments' => array('access site-wide contact form'),
          'access arguments append' => TRUE,
        ),
        'personal' => array(
          'help' => t('Submits a personal contact form.'),
          'file' => array(
            'type' => 'inc',
            'module' => 'services_contact',
            'name' => 'services_contact.resource',
          ),
          'callback' => 'services_contact_personal',
          'args' => array(
            array(
              'name' => 'name',
              'type' => 'string',
              'description' => t('The contact name.'),
              'source' => array('data' => 'name'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'mail',
              'type' => 'string',
              'description' => t('The contact e-mail.'),
              'source' => array('data' => 'mail'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'to',
              'type' => 'string',
              'description' => t('The recipient user name.'),
              'source' => array('data' => 'to'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'subject',
              'type' => 'string',
              'description' => t('The contact subject.'),
              'source' => array('data' => 'subject'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'message',
              'type' => 'string',
              'description' => t('The contact message.'),
              'source' => array('data' => 'message'),
              'optional' => FALSE,
            ),
            array(
              'name' => 'copy',
              'type' => 'int',
              'description' => t('Send a copy to yourself.'),
              'source' => array('data' => 'copy'),
              'optional' => TRUE,
            )
          ),
          'access callback' => '_contact_resource_access',
          'access callback file' => array(
            'type' => 'inc',
            'module' => 'services_contact',
            'name' => 'services_contact.resource',
          ),
          'access arguments' => array('access user contact forms'),
          'access arguments append' => TRUE,
        ),
      ),
      'operations' => array(
        'index' => array(
          'help' => t('Get the contact categories.'),
          'file' => array(
            'type' => 'inc',
            'module' => 'services_contact',
            'name' => 'services_contact.resource',
          ),
          'callback' => 'services_contact_categories',
          'access callback' => '_contact_resource_access',
          'access callback file' => array(
            'type' => 'inc',
            'module' => 'services_contact',
            'name' => 'services_contact.resource',
          ),
          'access arguments' => array('access site-wide contact form'),
          'access arguments append' => TRUE,
        ),
      ),
    ),
  );
  return $resources;
}

