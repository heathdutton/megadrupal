<?php

/**
 * @file
 * Module providing openid attribute exchange API schema
 */

/**
 * Create an array containing the identifiers as listed in
 * http://www.axschema.org/types/
 *
 * @return
 *   Array containing the schema definitions
 */
function _openid_ax_api_schema_definitions() {

  $attributes = array(
    'http://axschema.org/namePerson' => array(
      'identifier' => 1,
      'label' => t('Full Name'),
      'description' => t('Full name of subject)'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'sreg' => 'openid.sreg.fullname',
      'section' => t('Name'),
    ),
    'http://axschema.org/namePerson/prefix' => array(
      'identifier' => 2,
      'label' => t('Name prefix'),
      'description' => t('Honorific prefix for the subject\'s name; i.e. "Mr.", "Mrs.", "Dr."'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Name'),
    ),
    'http://axschema.org/namePerson/first' => array(
      'identifier' => 3,
      'label' => t('First name'),
      'description' => t('First or given name of subject'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Name'),
    ),
    'http://axschema.org/namePerson/last' => array(
      'identifier' => 4,
      'label' => t('Last name'),
      'description' => t('Last name or surname of subject'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Name'),
    ),
    'http://axschema.org/namePerson/middle' => array(
      'identifier' => 5,
      'label' => t('Middle name'),
      'description' => t('Middle name(s) of subject'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Name'),
    ),
    'http://axschema.org/namePerson/suffix' => array(
      'identifier' => 6,
      'label' => t('Name suffix'),
      'description' => t('Suffix of subject\'s name'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Name'),
    ),
    'http://axschema.org/namePerson/friendly' => array(
      'identifier' => 7,
      'label' => t('Alias'),
      'description' => t('Subject\'s alias or "screen" name'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'sreg' => 'openid.sreg.nickname',
      'section' => t('Name'),
    ),
    'http://axschema.org/company/name' => array(
      'identifier' => 8,
      'label' => t('Company'),
      'description' => t('Company name (employer)'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Work'),
    ),
    'http://axschema.org/company/title' => array(
      'identifier' => 9,
      'label' => t('Title'),
      'description' => t('Employee title'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Work'),
    ),
    'http://axschema.org/birthDate' => array(
      'identifier' => 10,
      'label' => t('Birth Date'),
      'description' => t('Date of Birth'),
      'type' => 'http://www.w3.org/2001/XMLSchema#date',
      'sreg' => 'openid.sreg.dob',
      'section' => t('Date-Of-Birth'),
    ),
    'http://axschema.org/birthDate/birthYear' => array(
      'identifier' => 11,
      'label' => t('Birth year'),
      'description' => t('Year of birth (four digits)'),
      'type' => 'http://www.w3.org/2001/XMLSchema#gYear',
      'section' => t('Date-Of-Birth'),
    ),
    'http://axschema.org/birthDate/birthMonth' => array(
      'identifier' => 12,
      'label' => t('Birth month'),
      'description' => t('Month of birth (1-12)'),
      'type' => 'http://www.w3.org/2001/XMLSchema#gMonth',
      'section' => t('Date-Of-Birth'),
    ),
    'http://axschema.org/birthDate/birthday' => array(
      'identifier' => 13,
      'label' => t('Birth day'),
      'description' => t('Day of birth'),
      'type' => 'http://www.w3.org/2001/XMLSchema#gDay',
      'section' => t('Date-Of-Birth'),
    ),
    'http://axschema.org/contact/phone/default' => array(
      'identifier' => 14,
      'label' => t('Phone (preferred) '),
      'description' => t('Main phone number (preferred)'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Telephone'),
    ),
    'http://axschema.org/contact/phone/home' => array(
      'identifier' => 15,
      'label' => t('Phone (home) '),
      'description' => t('Home phone number'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Telephone'),
    ),
    'http://axschema.org/contact/phone/business' => array(
      'identifier' => 16,
      'label' => t('Phone (work) '),
      'description' => t('Business phone number'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Telephone'),
    ),
    'http://axschema.org/contact/phone/cell' => array(
      'identifier' => 17,
      'label' => t('Phone (mobile) '),
      'description' => t('Cellular (or mobile) phone number'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Telephone'),
    ),
    'http://axschema.org/contact/phone/fax' => array(
      'identifier' => 18,
      'label' => t('Phone (fax) '),
      'description' => t('Fax number'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Telephone'),
    ),
    'http://axschema.org/contact/postalAddress/home' => array(
      'identifier' => 19,
      'label' => t('Address'),
      'description' => t('Home postal address: street number, name and apartment number'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Address'),
    ),
    'http://axschema.org/contact/postalAddressAdditional/home' => array(
      'identifier' => 20,
      'label' => t('Address 2'),
      'description' => t('Home postal address: supplementary information'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Address'),
    ),
    'http://axschema.org/contact/city/home' => array(
      'identifier' => 21,
      'label' => t('City'),
      'description' => t('Home city name'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Address'),
    ),
    'http://axschema.org/contact/state/home' => array(
      'identifier' => 22,
      'label' => t('State/Province'),
      'description' => t('Home state or province name'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Address'),
    ),
    'http://axschema.org/contact/country/home' => array(
      'identifier' => 23,
      'label' => t('Country'),
      'description' => t('Home country code'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Address'),
    ),
    'http://axschema.org/contact/postalCode/home' => array(
      'identifier' => 24,
      'label' => t('Postal code'),
      'description' => t('Home postal code'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Address'),
    ),
    'http://axschema.org/contact/postalAddress/business' => array(
      'identifier' => 25,
      'label' => t('Business Address'),
      'description' => t('Business postal address: street number, name and apartment number'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Business-Address'),
    ),
    'http://axschema.org/contact/postalAddressAdditional/business' => array(
      'identifier' => 26,
      'label' => t('Business Address 2'),
      'description' => t('Business postal address: supplementary information'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Business-Address'),
    ),
    'http://axschema.org/contact/city/business' => array(
      'identifier' => 27,
      'label' => t('City'),
      'description' => t('Business city name'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Business-Address'),
    ),
    'http://axschema.org/contact/state/business' => array(
      'identifier' => 28,
      'label' => t('State/Province'),
      'description' => t('Business state or province name'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Business-Address'),
    ),
    'http://axschema.org/contact/country/business' => array(
      'identifier' => 29,
      'label' => t('Country'),
      'description' => t('Business country code'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Business-Address'),
    ),
    'http://axschema.org/contact/postalCode/business' => array(
      'identifier' => 30,
      'label' => t('Postal code'),
      'description' => t('Business postal code'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Business-Address'),
    ),
    'http://axschema.org/contact/email' => array(
      'identifier' => 31,
      'label' => t('Email'),
      'description' => t('Internet SMTP email address as per RFC2822'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'sreg' => 'openid.sreg.email',
      'section' => t('Email'),
    ),
    'http://axschema.org/contact/IM/AIM' => array(
      'identifier' => 32,
      'label' => t('AOL IM'),
      'description' => t('AOL instant messaging service handle'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Instant-Messaging'),
    ),
    'http://axschema.org/contact/IM/ICQ' => array(
      'identifier' => 33,
      'label' => t('ICQ IM'),
      'description' => t('ICQ instant messaging service handle'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Instant-Messaging'),
    ),
    'http://axschema.org/contact/IM/MSN' => array(
      'identifier' => 34,
      'label' => t('MSN IM'),
      'description' => t('MSN instant messaging service handle'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Instant-Messaging'),
    ),
    'http://axschema.org/contact/IM/Yahoo' => array(
      'identifier' => 35,
      'label' => t('Yahoo! IM'),
      'description' => t('Yahoo! instant messaging service handle'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Instant-Messaging'),
    ),
    'http://axschema.org/contact/IM/Jabber' => array(
      'identifier' => 36,
      'label' => t('Jabber IM'),
      'description' => t('Jabber instant messaging service handle'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Instant-Messaging'),
    ),
    'http://axschema.org/contact/IM/Skype' => array(
      'identifier' => 37,
      'label' => t('Skype IM'),
      'description' => t('Skype instant messaging service handle'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Instant-Messaging'),
    ),
    'http://axschema.org/contact/web/default' => array(
      'identifier' => 38,
      'label' => t('Web page'),
      'description' => t('Web site URL'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Web-Sites'),
    ),
    'http://axschema.org/contact/web/blog' => array(
      'identifier' => 39,
      'label' => t('Blog'),
      'description' => t('Blog URL'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Web-Sites'),
    ),
    'http://axschema.org/contact/web/Linkedin' => array(
      'identifier' => 40,
      'label' => t('LinkedIn URL'),
      'description' => t('LinkedIn URL'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Web-Sites'),
    ),
    'http://axschema.org/contact/web/Amazon' => array(
      'identifier' => 41,
      'label' => t('Amazon URL'),
      'description' => t('Amazon URL'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Web-Sites'),
    ),
    'http://axschema.org/contact/web/Flickr' => array(
      'identifier' => 42,
      'label' => t('Flickr URL'),
      'description' => t('Flickr URL'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Web-Sites'),
    ),
    'http://axschema.org/contact/web/Delicious' => array(
      'identifier' => 43,
      'label' => t('del.icio.us URL'),
      'description' => t('del.icio.us URL'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Web-Sites'),
    ),
    'http://axschema.org/media/spokenname' => array(
      'identifier' => 44,
      'label' => t('Spoken name'),
      'description' => t('Spoken name (web URL)'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Audio/Video-Greetings'),
    ),
    'http://axschema.org/media/greeting/audio' => array(
      'identifier' => 45,
      'label' => t('Audio greeting'),
      'description' => t('Audio greeting (web URL)'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Audio/Video-Greetings'),
    ),
    'http://axschema.org/media/greeting/video' => array(
      'identifier' => 46,
      'label' => t('Video greeting'),
      'description' => t('Video greeting (web URL)'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Audio/Video-Greetings'),
    ),
    'http://axschema.org/media/image/default' => array(
      'identifier' => 47,
      'label' => t('Image'),
      'description' => t('Image (web URL); unspecified dimension'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Images'),
    ),
    'http://axschema.org/media/image/aspect11' => array(
      'identifier' => 48,
      'label' => t('Square image'),
      'description' => t('Image (web URL) with equal width and height'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Images'),
    ),
    'http://axschema.org/media/image/aspect43' => array(
      'identifier' => 49,
      'label' => t('4:3 aspect image'),
      'description' => t('Image (web URL) 4:3 aspect ratio - landscape'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Images'),
    ),
    'http://axschema.org/media/image/aspect34' => array(
      'identifier' => 50,
      'label' => t('3:4 aspect image'),
      'description' => t('Image (web URL) 3:4 aspect ratio - portrait'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Images'),
    ),
    'http://axschema.org/media/image/favicon' => array(
      'identifier' => 51,
      'label' => t('Favicon image'),
      'description' => t('Image (web URL); favicon format'),
      'type' => 'http://www.w3.org/2001/XMLSchema#anyURI',
      'section' => t('Images'),
    ),
    'http://axschema.org/person/gender' => array(
      'identifier' => 52,
      'label' => t('Gender'),
      'description' => t('Gender, either "M" or "F"'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Extra-Information'),
    ),
    'http://axschema.org/pref/language' => array(
      'identifier' => 53,
      'label' => t('Language'),
      'description' => t('Preferred language'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Extra-Information'),
    ),
    'http://axschema.org/pref/timezone' => array(
      'identifier' => 54,
      'label' => t('Time zone'),
      'description' => t('Home time zone information'),
      'type' => 'http://www.w3.org/2001/XMLSchema#normalizedString',
      'section' => t('Extra-Information'),
    ),
    'http://axschema.org/media/biography' => array(
      'identifier' => 55,
      'label' => t('Biography'),
      'description' => t('Biography (text)'),
      'type' => 'http://www.w3.org/2001/XMLSchema#string',
      'section' => t('Extra-Information'),
    ),
  );
  return $attributes;
}
