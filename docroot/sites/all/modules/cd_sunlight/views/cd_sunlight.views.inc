<?php

/*
 * @file
 * Views stuff for cd_sunlight.
 */

/**
 * Implements hook_views_data().
 */
function cd_sunlight_views_data() {
  $data['cd_sunlight_legislators']['table']['group'] = t('Congress');
  $data['cd_sunlight_legislators']['table']['base'] = array(
    'field' => 'legislator_id',
    'title' => t('Congress'),
    'help' => t("The current members of Congress as returned by the Sunlight API."),
  );
  $data['cd_sunlight_legislators']['firstname'] = array(
    'title' => t('First Name'),
    'help' => ' ',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['middlename'] = array(
    'title' => t('Middle Name'),
    'help' => ' ',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['lastname'] = array(
    'title' => t('Last Name'),
    'help' => ' ',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['name_suffix'] = array(
    'title' => t('Name Suffix'),
    'help' => t('"Jr." for example.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['nickname'] = array(
    'title' => t('Nickname'),
    'help' => ' ',
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_nickname',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['title'] = array(
    'title' => t('Title'),
    'help' => t('Usually "Sen" or "Rep".'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_title',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'cd_sunlight_handler_filter_title',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['cd_sunlight_legislators']['chamber'] = array(
    'real field' => 'title',
    'title' => t('Chamber'),
    'help' => t('"Senate" or "House".'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_chamber',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['party'] = array(
    'title' => t('Party'),
    'help' => ' ',
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_party',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'cd_sunlight_handler_filter_party',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['cd_sunlight_legislators']['state'] = array(
    'title' => t('State'),
    'help' => ' ',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'cd_sunlight_handler_filter_state',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['cd_sunlight_legislators']['district'] = array(
    'title' => t('District'),
    'help' => t('District number.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'cd_sunlight_handler_argument_district',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'cd_sunlight_handler_filter_district',
      'help' => t('Not useful unless used in combination with the state filter.'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['cd_sunlight_legislators']['gender'] = array(
    'title' => t('Gender'),
    'help' => ' ',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'cd_sunlight_handler_filter_gender',
      'allow empty' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['phone'] = array(
    'title' => t('Phone'),
    'help' => ' ',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['fax'] = array(
    'title' => t('Fax'),
    'help' => ' ',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['website'] = array(
    'title' => t('Website'),
    'help' => ' ',
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_website',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['webform'] = array(
    'title' => t('Webform'),
    'help' => t("URL of the congress person's web-based contact form."),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_webform',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['email'] = array(
    'title' => t('Email'),
    'help' => ' ',
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['congress_office'] = array(
    'title' => t('Congress Office'),
    'help' => 'Office address',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['bioguide_id'] = array(
    'title' => t('BioGuide page'),
    'help' => t('http://bioguide.congress.gov/'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_bioguide',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['votesmart_id'] = array(
    'title' => t('VoteSmart page'),
    'help' => t('http://votesmart.org/'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_votesmart',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['fec_id'] = array(
    'title' => t('FEC ID'),
    'help' => ' ',
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['govtrack_id'] = array(
    'title' => t('Govtrack page'),
    'help' => t('http://govtrack.us/'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_govtrack',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['crp_id'] = array(
    'title' => t('OpenSecrets page'),
    'help' => t('http://www.opensecrets.org/'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_opensecrets',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['congresspedia_url'] = array(
    'title' => t('OpenCongress page'),
    'help' => t('http://www.opencongress.org/'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_congresspedia',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['twitter_id'] = array(
    'title' => t('Twitter Page'),
    'help' => t('http://twitter.com/'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_twitter',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['youtube_url'] = array(
    'title' => t('YouTube Page'),
    'help' => t('http://youtube.com/'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_youtube',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['facebook_url'] = array(
    'real field' => 'facebook_id',
    'title' => t('Facebook URL'),
    'help' => t('http://www.facebook.com/'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_facebook',
      'click sortable' => TRUE,
    ),
  );
  $data['cd_sunlight_legislators']['picture'] = array(
    'title' => t('Picture'),
    'help' => t('Profile picture.'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_congress_picture',
    ),
  );
  $data['cd_sunlight_legislators']['progressive_punch'] = array(
    'title' => t('Progressive Punch Page'),
    'help' => t('http://progressivepunch.org'),
    'field' => array(
      'handler' => 'cd_sunlight_handler_field_url_favicon_progressive_punch',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function cd_sunlight_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'cd_sunlight') . '/views',
    ),
    'handlers' => array(
      'cd_sunlight_handler_field_congress_picture' => array(
        'parent' => 'views_handler_field',
      ),
      'cd_sunlight_handler_field_title' => array(
        'parent' => 'views_handler_field',
      ),
      'cd_sunlight_handler_field_chamber' => array(
        'parent' => 'views_handler_field',
      ),
      'cd_sunlight_handler_field_nickname' => array(
        'parent' => 'views_handler_field',
      ),
      'cd_sunlight_handler_field_party' => array(
        'parent' => 'views_handler_field',
      ),

      // URLs.
      'cd_sunlight_handler_field_url_favicon' => array(
        'parent' => 'views_handler_field_url',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_facebook' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_youtube' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_bioguide' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_votesmart' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_govtrack' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_opensecrets' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_congresspedia' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_twitter' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_generic' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_webform' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon_generic',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_website' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon_generic',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),
      'cd_sunlight_handler_field_url_favicon_progressive_punch' => array(
        'parent' => 'cd_sunlight_handler_field_url_favicon',
        'file' => 'cd_sunlight_handler_field_url.inc',
      ),

      // Filters.
      'cd_sunlight_handler_filter_title' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'cd_sunlight_handler_filter_party' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'cd_sunlight_handler_filter_gender' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'cd_sunlight_handler_filter_state' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'cd_sunlight_handler_filter_district' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),

      // Arguments.
      'cd_sunlight_handler_argument_district' => array(
        'parent' => 'views_handler_argument_numeric',
      ),

    ),
  );
}
