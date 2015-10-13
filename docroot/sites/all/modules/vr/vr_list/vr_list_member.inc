<?php

/**
 * This is a basic class for manupilating VerticalResponse member data.  It
 * provides methods for converting a member object to an array and vice-versa
 * and a form builder for generating a drupal form.
 */
class vr_list_member {
  // Provide an array of form properties for VerticalResponse list members
  var $form_info = array();
  var $member_array = array();
  var $member_object = NULL;
  var $list = NULL;

  /**
   * Set up default form values for title, type, etc..
   *
   * @param mixed
   *  A member object, array or hash
   * @param list_id
   *  optional list_id, required when member is a hash
   */
  function __construct($list_id, $member = NULL) {
    $this->list = vr_list_load($list_id);
    if (!empty($list_id) AND !empty($member) AND is_string($member)) {
      $param['hash'] = $member;
      $param['list_id'] = $list_id;
      $member = vr_api('getListMemberByHash', $param);
      $this->rebuild($member);
    }
    elseif (is_object($member) OR is_array($member)) {
      $this->rebuild($member);
    }
    $this->init_form();
  }

  /**
   * Rebuild member information based on input.
   *
   * @param mixed
   *  A member array or member object.
   */
  function rebuild($member) {
    $this->member_object = $this->make_object($member);
    $this->member_array = $this->make_array($member);
  }

  /**
   *  Convert VerticalResponse member data into an array.
   */
  function make_array($object) {
    if (is_array($object)) {
      return $object;
    }
    $member['list_id'] = $object->list_id;
    $member['list_name'] = $object->list_name;
    $member['list_type'] = $object->list_type;
    if (!is_array($object->member_data)) {
      return FALSE;
    }
    else {
      foreach ($object->member_data as $property) {
        $member[$property->name] = check_plain($property->value);
      }
      return $member;
    }
  }

  /**
   * Convert vr_list member array into a VerticalResponse member
   * object.
   */
  function make_object($member_array) {
    if (is_object($member_array)) {
      return $member_array;
    }
    $outter = array(
      'list_id',
      'list_name',
      'list_type',
    );
    $member = (object) NULL;
    foreach ($member_array as $key => $value) {
      if (in_array($key, $outter)) {
        $member->$key = $value;
      }
      else {
        $property = (object) NULL;
        $property->name = $key;
        $property->value = $value;
        $member->member_data[] = $property;
      }
    }
    return $member;
  }

  /**
   * Return a drupal form array.
   *
   * @param array
   *   A VerticalResponse mailing list object that has been converted
   *   to an array using the member_array method.
   */
  function form($member_info = NULL) {
    if (empty($member_info)) {
      $member_info = $this->member_array;
    }
    // Some fields should be skipped when creating or editing a user.
    $skip_fields = array(
      'ip_address',
      'id',
      'hash',
      'created_date',
      'last_updated',
    );
    foreach ($this->form_info as $key => $value) {
      if (empty($member_info) and in_array($key, $skip_fields)) {
        continue;
      }
      if (empty($value['#type'])) {
        $this->set_form($key, '#type', 'textfield');
        $this->set_form($key, '#default_value', $member_info[$key]);
      }
      elseif ($value['type'] == 'value' OR $value['#type'] == 'item') {
        $this->set_form($key, '#value', $member_info[$key]);
      }
      else {
        $this->set_form($key, '#default_value', $member_info[$key]);
      }
    }
    return $this->form_info;
  }

  /**
   * Update VerticalResponse mailing list member record from an
   * object.
   *
   * It is crutial to exclude the hash and id fields when creating
   * a new user.
   *
   * @param mixed
   *  NULL to use the current member object.
   */
  function save($member = NULL) {
    if (empty($member)) {
      $member = $this->member_object;
    }
    elseif (is_array($member)) {
      $member = $this->make_object($member);
    }
    $param['list_member'] = $member;
    if (empty($this->member_array['hash'])) {
      vr_api('addListMember', $param, TRUE);
    }
    else {
      vr_api('editListMember', $param, TRUE);
    }
  }

  /**
   * Provide title, description, type etc. for the member form.
   */
  function set_form($item, $property, $value) {
    $this->form_info[$item][$property] = $value;
  }

  /**
   * Provide a list of acceptable marital statuses as provided by VerticalResponse
   */
  function get_marital_status() {
    return array(
      'null' => t('Not Provided'),
      'Single' => t('Single'),
      'Married' => t('Married'),
      'Divorced' => t('Divorced'),
      'Widowed' => t('Widowed'),
    );
  }
  /**
   * Provide a list of gender options as provided by VerticalResponse.
   */
  function get_gender() {
    return array(
      'null' => t('Not Provided'),
      'Male' => t('Male'),
      'Female' => t('Female'),
    );
  }

  /**
   * Search drupal for a user map, and update VerticalResponse information
   * if one exists.
   *
   * @return boolean
   *  TRUE if the mailing list has a field mapping, FALSE if not.
   */
  function map_fields() {
    $default_view = variable_get('vr_list_view_default', NULL);
    $view_name = variable_get('vr_list_view_' . $this->list->id, $default_view);

    $email = $this->member_array['email_address'];
    $uid = db_query('SELECT uid FROM {users} WHERE mail = :mail', array(':mail' => $email))->fetchField();
    // Return FALSE if there is no view designated as a field map or if there is
    // no Drupal user with a given email address.

    if (empty($view_name) OR empty($uid)) {
      return FALSE;
    }
    // Load default view, pass arguments and retrieve the result.
    $view = views_get_view($view_name);
    $view->set_display('default');
    $view->pre_execute(array($uid));
    $view->display_handler->preview();
    // Exit if the result is empty.
    if (empty($view->result)) {
      return FALSE;
    }
    // Retrieve system field map from drupal.
    $default_field_map = variable_get('vr_list_view_map_default', array());
    $field_map = variable_get('vr_list_view_map_' . $this->list->id, $default_field_map);
    foreach ($field_map as $key => $field) {
      $property = $this->list->fields[$key];
      if (empty($field) OR $property == 'optin_status') {
        continue;
      }
      $alias = $view->field[$field]->field_alias;
      $value = $view->result[0]->$alias;
      $this->member_array[$property] = $value;
    }
    // Set empty string to NULL for optin_status, otherwise VR will
    // mark the record as unsubscribed.
    $list_status = $this->member_array['optin_status'];
    if (is_string($list_status) AND empty($list_status)) {
      $this->member_array['optin_status'] = NULL;
    }
    $this->rebuild($this->member_array);
    return TRUE;
  }

  /**
   * Provide a list of countries per VerticalResponse.
   */
  function get_countries() {
    return array(
      'null' => 'None Provided',
      'US' => 'United States',
      'CA' => 'Canada',
      'UK' => 'United Kindgom',
      'AU' => 'Australia',
      'AF' => 'Afghanistan',
      'AX' => 'Aland Islands',
      'AL' => 'Albania',
      'DZ' => 'Algeria',
      'AS' => 'American Samoa',
      'AD' => 'Andorra',
      'AO' => 'Angola',
      'AI' => 'Anguilla',
      'AQ' => 'Antarctica',
      'AG' => 'Antigua And Barbuda',
      'AR' => 'Argentina',
      'AM' => 'Armenia',
      'AW' => 'Aruba',
      'AT' => 'Austria',
      'AZ' => 'Azerbaijan',
      'BS' => 'Bahamas, The',
      'BH' => 'Bahrain',
      'BD' => 'Bangladesh',
      'BB' => 'Barbados',
      'BY' => 'Belarus',
      'BE' => 'Belgium',
      'BZ' => 'Belize',
      'BJ' => 'Benin',
      'BM' => 'Bermuda',
      'BT' => 'Bhutan',
      'BO' => 'Bolivia',
      'BA' => 'Bosnia and Herzegovina',
      'BW' => 'Botswana',
      'BV' => 'Bouvet Island',
      'BR' => 'Brazil',
      'IO' => 'British Indian Ocean Territory',
      'BN' => 'Brunei',
      'BG' => 'Bulgaria',
      'BF' => 'Burkina Faso',
      'BI' => 'Burundi',
      'KH' => 'Cambodia',
      'CM' => 'Cameroon',
      'CV' => 'Cape Verde',
      'KY' => 'Cayman Islands',
      'CF' => 'Central African Republic',
      'TD' => 'Chad',
      'CL' => 'Chile',
      'CN' => 'China',
      'CX' => 'Christmas Island',
      'CC' => 'Cocos (Keeling) Islands',
      'CO' => 'Colombia',
      'KM' => 'Comoros',
      'CG' => 'Congo',
      'CD' => 'Congo, Democractic Republic of the',
      'CK' => 'Cook Islands',
      'CR' => 'Costa Rica',
      'CI' => 'Cote D\'Ivoire (Ivory Coast)',
      'HR' => 'Croatia (Hrvatska)',
      'CU' => 'Cuba',
      'CY' => 'Cyprus',
      'CZ' => 'Czech Republic',
      'DK' => 'Denmark',
      'DJ' => 'Djibouti',
      'DM' => 'Dominica',
      'DO' => 'Dominican Republic',
      'TP' => 'East Timor',
      'EC' => 'Ecuador',
      'EG' => 'Egypt',
      'SV' => 'El Salvador',
      'GQ' => 'Equatorial Guinea',
      'ER' => 'Eritrea',
      'EE' => 'Estonia',
      'ET' => 'Ethiopia',
      'FK' => 'Falkland Islands (Islas Malvinas)',
      'FO' => 'Faroe Islands',
      'FJ' => 'Fiji Islands',
      'FI' => 'Finland',
      'FR' => 'France',
      'GF' => 'French Guiana',
      'PF' => 'French Polynesia',
      'TF' => 'French Southern Territories',
      'GA' => 'Gabon',
      'GM' => 'Gambia',
      'GE' => 'Georgia',
      'DE' => 'Germany',
      'GH' => 'Ghana',
      'GI' => 'Gibraltar',
      'GR' => 'Greece',
      'GL' => 'Greenland',
      'GD' => 'Grenada',
      'GP' => 'Guadeloupe',
      'GU' => 'Guam',
      'GT' => 'Guatemala',
      'GN' => 'Guinea',
      'GW' => 'Guinea-Bissau',
      'GY' => 'Guyana',
      'HT' => 'Haiti',
      'HM' => 'Heard and McDonald Islands',
      'HN' => 'Honduras',
      'HK' => 'Hong Kong',
      'HU' => 'Hungary',
      'IS' => 'Iceland',
      'IN' => 'India',
      'ID' => 'Indonesia',
      'IR' => 'Iran',
      'IQ' => 'Iraq',
      'IE' => 'Ireland',
      'IL' => 'Israel',
      'IT' => 'Italy',
      'JM' => 'Jamaica',
      'JP' => 'Japan',
      'JO' => 'Jordan',
      'KZ' => 'Kazakhstan',
      'KE' => 'Kenya',
      'KI' => 'Kiribati',
      'KR' => 'Korea',
      'KP' => 'Korea, North',
      'KW' => 'Kuwait',
      'KG' => 'Kyrgyzstan',
      'LA' => 'Laos',
      'LV' => 'Latvia',
      'LB' => 'Lebanon',
      'LS' => 'Lesotho',
      'LR' => 'Liberia',
      'LY' => 'Libya',
      'LI' => 'Liechtenstein',
      'LT' => 'Lithuania',
      'LU' => 'Luxembourg',
      'MO' => 'Macau',
      'MK' => 'Macedonia, Former Yugoslav Republic of',
      'MG' => 'Madagascar',
      'MW' => 'Malawi',
      'MY' => 'Malaysia',
      'MV' => 'Maldives',
      'ML' => 'Mali',
      'MT' => 'Malta',
      'MH' => 'Marshall Islands',
      'MQ' => 'Martinique',
      'MR' => 'Mauritania',
      'MU' => 'Mauritius',
      'YT' => 'Mayotte',
      'MX' => 'Mexico',
      'FM' => 'Micronesia',
      'MD' => 'Moldova',
      'MC' => 'Monaco',
      'MN' => 'Mongolia',
      'ME' => 'Montenegro',
      'MS' => 'Montserrat',
      'MA' => 'Morocco',
      'MZ' => 'Mozambique',
      'MM' => 'Myanmar',
      'NA' => 'Namibia',
      'NR' => 'Nauru',
      'NP' => 'Nepal',
      'AN' => 'Netherlands Antilles',
      'NL' => 'Netherlands, The',
      'NC' => 'New Caledonia',
      'NZ' => 'New Zealand',
      'NI' => 'Nicaragua',
      'NE' => 'Niger',
      'NG' => 'Nigeria',
      'NU' => 'Niue',
      'NF' => 'Norfolk Island',
      'MP' => 'Northern Mariana Islands',
      'NO' => 'Norway',
      'OM' => 'Oman',
      'PK' => 'Pakistan',
      'PW' => 'Palau',
      'PA' => 'Panama',
      'PG' => 'Papua new Guinea',
      'PY' => 'Paraguay',
      'PE' => 'Peru',
      'PH' => 'Philippines',
      'PN' => 'Pitcairn Island',
      'PL' => 'Poland',
      'PT' => 'Portugal',
      'PR' => 'Puerto Rico',
      'QA' => 'Qatar',
      'RE' => 'Reunion',
      'RO' => 'Romania',
      'RU' => 'Russia',
      'RW' => 'Rwanda',
      'SH' => 'Saint Helena',
      'KN' => 'Saint Kitts And Nevis',
      'LC' => 'Saint Lucia',
      'PM' => 'Saint Pierre and Miquelon',
      'VC' => 'Saint Vincent And The Grenadines',
      'WS' => 'Samoa',
      'SM' => 'San Marino',
      'ST' => 'Sao Tome and Principe',
      'SA' => 'Saudi Arabia',
      'SN' => 'Senegal',
      'RS' => 'Serbia',
      'SC' => 'Seychelles',
      'SL' => 'Sierra Leone',
      'SG' => 'Singapore',
      'SK' => 'Slovakia',
      'SI' => 'Slovenia',
      'SB' => 'Solomon Islands',
      'SO' => 'Somalia',
      'ZA' => 'South Africa',
      'GS' => 'South Georgia And The South Sandwich Islands',
      'ES' => 'Spain',
      'LK' => 'Sri Lanka',
      'SD' => 'Sudan',
      'SR' => 'Suriname',
      'SJ' => 'Svalbard And Jan Mayen Islands',
      'SZ' => 'Swaziland',
      'SE' => 'Sweden',
      'CH' => 'Switzerland',
      'SY' => 'Syria',
      'TW' => 'Taiwan',
      'TJ' => 'Tajikistan',
      'TZ' => 'Tanzania',
      'TH' => 'Thailand',
      'TG' => 'Togo',
      'TK' => 'Tokelau',
      'TO' => 'Tonga',
      'TT' => 'Trinidad And Tobago',
      'TN' => 'Tunisia',
      'TR' => 'Turkey',
      'TM' => 'Turkmenistan',
      'TC' => 'Turks And Caicos Islands',
      'TV' => 'Tuvalu',
      'UG' => 'Uganda',
      'UA' => 'Ukraine',
      'AE' => 'United Arab Emirates',
      'UM' => 'United States Minor Outlying Islands',
      'UY' => 'Uruguay',
      'UZ' => 'Uzbekistan',
      'VU' => 'Vanuatu',
      'VA' => 'Vatican City State (Holy See)',
      'VE' => 'Venezuela',
      'VN' => 'Vietnam',
      'VG' => 'Virgin Islands (British)',
      'VI' => 'Virgin Islands (US)',
      'WF' => 'Wallis And Futuna Islands',
      'YE' => 'Yemen',
      'YU' => 'Yugoslavia',
      'ZM' => 'Zambia',
      'ZW' => 'Zimbabwe',
    );
  }

  /**
   * Initialize form.
   */
  function init_form() {
    foreach ($this->list->fields as $field) {
      if ($field != 'optin_status' AND $field != 'optin_status_last_updated') {
        $title = str_replace('_', ' ', $field);
        $this->set_form($field, '#title', ucwords($title));
      }
    }
    $this->set_form('list_id', '#type', 'value');
    $this->set_form('list_name', '#type', 'value');
    $this->set_form('list_type', '#type', 'value');
    $this->set_form('address_hash', '#type', 'value');
    $this->set_form('ip_address', '#type', 'value');
    $this->set_form('hash', '#type', 'value');
    $this->set_form('id', '#type', 'value');
    $this->set_form('create_date', '#type', 'value');
    $this->set_form('address_hash', '#type', 'value');
    $this->set_form('last_updated', '#type', 'value');
    $this->set_form('postalcode', '#title', t('Postal Code'));
    $this->set_form('country', '#type', 'select');
    $this->set_form('country', '#options', $this->get_countries());
    $this->set_form('mobile_phone', '#title', t('Mobile Phone'));
    $this->set_form('fax', '#title', t('Fax'));
    $this->set_form('marital_status', '#type', 'select');
    $this->set_form('marital_status', '#options', $this->get_marital_status());
    $this->set_form('gender', '#type', 'select');
    $this->set_form('gender', '#options', $this->get_gender());
  }
}
