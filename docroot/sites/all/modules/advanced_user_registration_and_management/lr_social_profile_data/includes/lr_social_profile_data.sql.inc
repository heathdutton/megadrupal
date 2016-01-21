<?php

/**
 * @file
 * lr_social_profile_data
 */

/**
 * basic profile data table schema
 *
 * @return array Schema of basic profile data table
 */
function lr_social_profile_data_loginradius_basic_profile_data() {
  return array(
    'description' => 'Stores Basic User Profile data.',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'provider' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'The provider\'s machine name.',
      ),
      'provider_id' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'The provider\'s unique id.',
      ),
      'prefix' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Prefix of user',
      ),
      'first_name' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'First name of user',
      ),
      'middle_name' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'Middle name of user',
      ),
      'last_name' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'Last name of user',
      ),
      'suffix' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Suffix of user.',
      ),
      'full_name' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'full name of user',
      ),
      'nick_name' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'Nick name of user',
      ),
      'profile_name' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'Profile name of user.',
      ),
      'birth_date' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'Date of Birth of User.',
      ),
      'gender' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'Gender of user',
      ),
      'country_code' => array(
        'type' => 'varchar',
        'length' => 10,
        'not null' => FALSE,
        'description' => 'Country code of user.',
      ),
      'country_name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Country Name of User.',
      ),
      'thumbnail_image_url' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'Square/Small image of user.',
      ),
      'image_url' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'Large Iamge of user.',
      ),
      'local_country' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'Local country of user.',
      ),
      'profile_country' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'Profile Country of user.',
      ),
    ),
    'primary key' => array('user_id'),
  );
}

/**
 * email table schema
 *
 * @return array Schema of email table
 */
function lr_social_profile_data_loginradius_emails() {
  return array(
    'description' => 'Stores Emails information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'email_type' => array(
        'type' => 'varchar',
        'length' => 10,
        'not null' => FALSE,
        'description' => 'email_type of user',
      ),
      'email' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'email of user',
      ),
    ),
  );
}

/**
 * extended location schema
 *
 * @return array Schema of table
 */
function lr_social_profile_data_loginradius_extended_location_data() {
  return array(
    'description' => 'Stores Extended location data',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'main_address' => array(
        'type' => 'varchar',
        'length' => 500,
        'not null' => FALSE,
        'description' => 'MAin address of user',
      ),
      'hometown' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Home Town of user',
      ),
      'state' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'State of user',
      ),
      'city' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'City  of user',
      ),
      'local_city' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'Local city of user',
      ),
      'profile_city' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'Profile City of user',
      ),
      'profile_url' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'Profile URL of user.',
      ),
      'local_language' => array(
        'type' => 'varchar',
        'length' => 10,
        'not null' => FALSE,
        'description' => 'Local Language of user',
      ),
      'language' => array(
        'type' => 'varchar',
        'length' => 10,
        'not null' => FALSE,
        'description' => 'Language of user',
      ),
    ),
    'primary key' => array('user_id'),
  );
}

/**
 * Extended profile data table schema
 *
 * @return array Schema of Extended profile data table
 */
function lr_social_profile_data_loginradius_extended_profile_data() {
  return array(
    'description' => 'Extended Profile data.',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'website' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'website of user.',
      ),
      'favicon' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'favicon of user.',
      ),
      'industry' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'Industry of user',
      ),
      'about' => array(
        'type' => 'text',
        'not null' => FALSE,
        'description' => 'About of user',
      ),
      'timezone' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Timezone of user',
      ),
      'verified' => array(
        'type' => 'varchar',
        'length' => 10,
        'not null' => FALSE,
        'description' => 'Verified of user',
      ),
      'last_profile_update' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Last Profile update of user.',
      ),
      'created' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Created date of user',
      ),
      'relationship_status' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'Nick name of user',
      ),
      'quote' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'quote of user.',
      ),
      'interested_in' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'interested in of User.',
      ),
      'interests' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'interests of user',
      ),
      'religion' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'religion of user.',
      ),
      'political_view' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'political view of User.',
      ),
      'https_image_url' => array(
        'type' => 'varchar',
        'length' => 500,
        'not null' => FALSE,
        'description' => 'https image url of user.',
      ),
      'followers_count' => array(
        'type' => 'int',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'Large Iamge of user.',
      ),
      'friends_count' => array(
        'type' => 'varchar',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'friends count of user.',
      ),
      'is_geo_enabled' => array(
        'type' => 'varchar',
        'length' => 10,
        'not null' => FALSE,
        'description' => 'is geo enabled of user.',
      ),
      'total_status_count' => array(
        'type' => 'int',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'total status count of User.',
      ),
      'number_of_recommenders' => array(
        'type' => 'int',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'number of recommenders of user.',
      ),
      'honors' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'honors of user.',
      ),
      'associations' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'associations of user.',
      ),
      'hirable' => array(
        'type' => 'int',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'hirable of User.',
      ),
      'repository_url' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'repository url of user.',
      ),
      'age' => array(
        'type' => 'int',
        'length' => 3,
        'not null' => FALSE,
        'description' => 'age of user.',
      ),
      'professional_headline' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'professional headline of user.',
      ),
      'provider_access_token' => array(
        'type' => 'blob',
        'not null' => FALSE,
        'description' => 'provider access token of user.',
      ),
      'provider_token_secret' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'provider token secret  of user.',
      ),
    ),
    'primary key' => array('user_id'),
  );
}

/**
 * position table schema
 *
 * @return array Schema of position table
 */
function lr_social_profile_data_loginradius_positions() {
  return array(
    'description' => 'Stores Extended location data',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'position' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'position of user',
      ),
      'summary' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'summary of user',
      ),
      'start_date' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'start date  of user',
      ),
      'end_date' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'end date  user',
      ),
      'is_current' => array(
        'type' => 'varchar',
        'length' => 10,
        'not null' => FALSE,
        'description' => 'is current  of user',
      ),
      'company' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'company of user',
      ),
      'location' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'location of user.',
      ),
    ),
  );
}

/**
 * companies table schema
 *
 * @return array Schema of companies table
 */
function lr_social_profile_data_loginradius_companies() {
  return array(
    'description' => 'Stores Compenies of user',
    'fields' => array(
      'id' => array(
        'type' => 'serial',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'ID of the user table.',
      ),
      'company_name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'company_name of user',
      ),
      'company_type' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'company_type of user',
      ),
      'industry' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'industry  of user',
      ),
    ),
    'primary key' => array('id'),
  );
}

/**
 * education table schema
 *
 * @return array Schema of education table
 */
function lr_social_profile_data_loginradius_education() {
  return array(
    'description' => 'Stores education Information of data',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'school' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'position of user',
      ),
      'year' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'year of user',
      ),
      'type' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'type  of user',
      ),
      'notes' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'notes  user',
      ),
      'activities' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'activities  of user',
      ),
      'degree' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'degree of user',
      ),
      'field_of_study' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'field_of_study of user.',
      ),
      'start_date' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'start_date of user.',
      ),
      'end_date' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'end_date of user.',
      ),
    ),
  );
}

/**
 * phone number table schema
 *
 * @return array Schema of phone number table
 */
function lr_social_profile_data_loginradius_phone_numbers() {
  return array(
    'description' => 'Stores Compenies of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'number_type' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'number_type of user',
      ),
      'phone_number' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'phone_number of user',
      ),
    ),
  );
}

/**
 * im account table schema
 *
 * @return array Schema of  im account table
 */
function lr_social_profile_data_loginradius_IMaccounts() {
  return array(
    'description' => 'Stores Compenies of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'account_type' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'account_type of user',
      ),
      'account_username' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'account_username of user',
      ),
    ),
  );
}

/**
 * address table schema
 *
 * @return array Schema of address table
 */
function lr_social_profile_data_loginradius_addresses() {
  return array(
    'description' => 'Stores education Information of data',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'type' => array(
        'type' => 'varchar',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'type of user',
      ),
      'address_line1' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'address_line of user',
      ),
      'address_line2' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'address_line2  of user',
      ),
      'city' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'city of  user',
      ),
      'state' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'state  of user',
      ),
      'postal_code' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'postal_code of user',
      ),
      'region' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'region of user.',
      ),
    ),
  );
}

/**
 * sport table schema
 *
 * @return array Schema of sport table
 */
function lr_social_profile_data_loginradius_sports() {
  return array(
    'description' => 'Stores SPorts information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'sport_id' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'sport_id of user',
      ),
      'sport' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'sport of user',
      ),
    ),
  );
}

/**
 * inspirational peorple table schema
 *
 * @return array Schema of inspirational table
 */
function lr_social_profile_data_loginradius_inspirational_people() {
  return array(
    'description' => 'Stores Inspirational people information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'social_id' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'social_id of user',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'name of user',
      ),
    ),
  );
}

/**
 * skill table schema
 *
 * @return array Schema of skill table
 */
function lr_social_profile_data_loginradius_skills() {
  return array(
    'description' => 'Stores Skills information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'skill_id' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'skill_id of user',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'name of user',
      ),
    ),
  );
}

/**
 * current status table schema
 *
 * @return array Schema of current status table
 */
function lr_social_profile_data_loginradius_current_status() {
  return array(
    'description' => 'Stores current status information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'status_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'status_id of user',
      ),
      'status' => array(
        'type' => 'text',
        'not null' => FALSE,
        'description' => 'status of user',
      ),
      'source' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'source of user',
      ),
      'created_date' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'created_date of user',
      ),
    ),
  );
}

/**
 * certification table schema
 *
 * @return array Schema of certification table
 */
function lr_social_profile_data_loginradius_certifications() {
  return array(
    'description' => 'Stores certifications information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'certification_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'certification_id of user',
      ),
      'certification_name' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'certification_name of user',
      ),
      'authority' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'authority of user',
      ),
      'license_number' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'license_number of user',
      ),
      'start_date' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'start_date of user',
      ),
      'end_date' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'end_date of user',
      ),
    ),
  );
}

/**
 * courses table schema
 *
 * @return array Schema of courses table
 */
function lr_social_profile_data_loginradius_courses() {
  return array(
    'description' => 'Stores courses information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'course_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'course_id of user',
      ),
      'course' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'course of user',
      ),
      'course_number' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'course_number of user',
      ),
    ),
  );
}

/**
 * volunteer table schema
 *
 * @return array Schema of volunteer table
 */
function lr_social_profile_data_loginradius_volunteer() {
  return array(
    'description' => 'Stores volunteer information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'volunteer_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'volunteer_id of user',
      ),
      'role' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'role of user',
      ),
      'organization' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'organization of user',
      ),
      'cause' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'cause of user',
      ),
    ),
  );
}

/**
 * recommdention received table schema
 *
 * @return array Schema of recommdention received table
 */
function lr_social_profile_data_loginradius_recommendations_received() {
  return array(
    'description' => 'Stores recommendations received information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'recommendation_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'recommendation_id of user',
      ),
      'recommendation_type' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'recommendation_type of user',
      ),
      'recommendation_text' => array(
        'type' => 'text',
        'not null' => FALSE,
        'description' => 'recommendation_text of user',
      ),
      'recommender' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'recommender of user',
      ),
    ),
  );
}

/**
 * language table schema
 *
 * @return array Schema of language table
 */
function lr_social_profile_data_loginradius_languages() {
  return array(
    'description' => 'Stores language information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'language_id' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'language_id of user',
      ),
      'language' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'language of user',
      ),
    ),
  );
}

/**
 * patent table schema
 *
 * @return array Schema of patent table
 */
function lr_social_profile_data_loginradius_patents() {
  return array(
    'description' => 'Stores patents information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'patent_id' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'patent_id of user',
      ),
      'title' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'title of user',
      ),
      'date' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'date of user',
      ),
    ),
  );
}

/**
 * game table schema
 *
 * @return array Schema of game table
 */
function lr_social_profile_data_loginradius_games() {
  return array(
    'description' => 'Stores games Information of data',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'game_id' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'game_id of game',
      ),
      'category' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'category of game',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'name  of game',
      ),
      'created_date' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Created date of user',
      ),
    ),
  );
}

/**
 * television table schema
 *
 * @return array Schema of television table
 */
function lr_social_profile_data_loginradius_television_show() {
  return array(
    'description' => 'Stores television Information of data',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'television_id' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'television_id of television',
      ),
      'category' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'category of television',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'name  of television',
      ),
      'created_date' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Created date of television',
      ),
    ),
  );
}

/**
 * movie table schema
 *
 * @return array Schema of movie table
 */
function lr_social_profile_data_loginradius_movies() {
  return array(
    'description' => 'Stores movie Information of data',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'movie_id' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'movie_id of movie',
      ),
      'category' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'category of movie',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'name  of movie',
      ),
      'created_date' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Created date of movie',
      ),
    ),
  );
}

/**
 * book table schema
 *
 * @return array Schema of book table
 */
function lr_social_profile_data_loginradius_books() {
  return array(
    'description' => 'Stores book Information of data',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'book_id' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => FALSE,
        'description' => 'television_id of book',
      ),
      'category' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'category of book',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'name  of book',
      ),
      'created_date' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'Created date of book',
      ),
    ),
  );
}

/**
 * favorites table schema
 *
 * @return array Schema of favorites table
 */
function lr_social_profile_data_loginradius_favorites() {
  return array(
    'description' => 'Stores favorites information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'social_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'social_id of user',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'name of user',
      ),
      'type' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'type of user',
      ),
    ),
  );
}

/**
 * linkedin companes table schema
 *
 * @return array Schema of linkedin companes table
 */
function lr_social_profile_data_loginradius_linkedin_companies() {
  return array(
    'description' => 'Stores followed companies of LinkedIn information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'company_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'company_id of user',
      ),
      'company_name' => array(
        'type' => 'varchar',
        'length' => 200,
        'not null' => FALSE,
        'description' => 'company_name of user',
      ),
    ),
  );
}

/**
 * facebook events table schema
 *
 * @return array Schema of facebook events table
 */
function lr_social_profile_data_loginradius_facebook_events() {
  return array(
    'description' => 'Stores facebook events information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'event_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'event_id ID of the user table.',
      ),
      'owner_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'owner_id ID of the user table.',
      ),
      'owner_name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'event_id ID of the user table.',
      ),
      'event' => array(
        'type' => 'varchar',
        'length' => 500,
        'not null' => FALSE,
        'description' => 'event of user',
      ),
      'description' => array(
        'type' => 'text',
        'not null' => FALSE,
        'description' => 'description of event',
      ),
      'start_time' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'start_time of user',
      ),
      'updated_date' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'updated_time of user',
      ),
      'end_time' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'end_time of user',
      ),
      'privacy' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'privacy',
      ),
      'rsvp_status' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'rsvp_status of user',
      ),
      'location' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'location of user',
      ),
    ),
  );
}

/**
 * status table schema
 *
 * @return array Schema of status table
 */
function lr_social_profile_data_loginradius_status() {
  return array(
    'description' => 'Stores status information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'provider' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => TRUE,
        'description' => 'provider of the user table.',
      ),
      'status_id' => array(
        'type' => 'varchar',
        'length' => 500,
        'not null' => FALSE,
        'description' => 'status_id of user',
      ),
      'status' => array(
        'type' => 'text',
        'not null' => FALSE,
        'description' => 'status of user',
      ),
      'date_time' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'date_time of user',
      ),
      'likes' => array(
        'type' => 'int',
        'length' => 8,
        'not null' => FALSE,
        'description' => 'likes of user',
      ),
      'place' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'place of user',
      ),
      'source' => array(
        'type' => 'varchar',
        'length' => 500,
        'not null' => FALSE,
        'description' => 'source of user',
      ),
      'image_url' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'image_url of user',
      ),
      'link_url' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'link_url of user',
      ),
    ),
  );
}

/**
 * facebook posts table schema
 *
 * @return array Schema of facebook posts table
 */
function lr_social_profile_data_loginradius_facebook_posts() {
  return array(
    'description' => 'Stores facebook post information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'post_id' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'post_id of the user table.',
      ),
      'from_name' => array(
        'type' => 'varchar',
        'length' => 500,
        'not null' => FALSE,
        'description' => 'from_name of user',
      ),
      'title' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'title of user',
      ),
      'start_time' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'start_time of user',
      ),
      'update_time' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'update_time of user',
      ),
      'message' => array(
        'type' => 'text',
        'not null' => FALSE,
        'description' => 'message of user',
      ),
      'place' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'place of user',
      ),
      'picture' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'picture of user',
      ),
      'likes' => array(
        'type' => 'int',
        'length' => 8,
        'not null' => FALSE,
        'description' => 'likes of user',
      ),
      'shares' => array(
        'type' => 'int',
        'length' => 8,
        'not null' => FALSE,
        'description' => 'shares of user',
      ),
    ),
  );
}

/**
 * twitter mention table schema
 *
 * @return array Schema of twitter mention table
 */
function lr_social_profile_data_loginradius_twitter_mentions() {
  return array(
    'description' => 'Stores twitter mentions information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'mention_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'mention_id of the user table.',
      ),
      'tweet' => array(
        'type' => 'text',
        'not null' => FALSE,
        'description' => 'tweet of user',
      ),
      'date_time' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'date_time of user',
      ),
      'likes' => array(
        'type' => 'int',
        'length' => 8,
        'not null' => FALSE,
        'description' => 'likes of user',
      ),
      'place' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'place of user',
      ),
      'source' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'source of user',
      ),
      'image_url' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'image_url of user',
      ),
      'link_url' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'link_url of user',
      ),
      'mentioned_by' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'mentioned_by of user',
      ),
    ),
  );
}

/**
 * group table schema
 *
 * @return array Schema of group table
 */
function lr_social_profile_data_loginradius_groups() {
  return array(
    'description' => 'Stores groups information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'provider' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'provider of user',
      ),
      'group_id' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'group_id of user',
      ),
      'group_name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'group_name of user',
      ),
      'email' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'email of group user',
      ),
      'group_description' => array(
        'type' => 'text',
        'not null' => FALSE,
        'description' => 'description of group user',
      ),
      'type' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'type of group user',
      ),
      'country' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'country of group user',
      ),
      'postal_code' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'postal_code of group user',
      ),
      'logo' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'logo of group user',
      ),
      'image_url' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'image_url of group user',
      ),
      'member_count' => array(
        'type' => 'int',
        'length' => 11,
        'not null' => FALSE,
        'description' => 'members count of group user.',
      ),
    ),
  );
}

/**
 * contacts table schema
 *
 * @return array Schema of contacts table
 */
function lr_social_profile_data_loginradius_contacts() {
  return array(
    'description' => 'Stores contacts information of user',
    'fields' => array(
      'user_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'User ID of the user table.',
      ),
      'provider' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'provider of user',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'name of user',
      ),
      'email' => array(
        'type' => 'varchar',
        'length' => 100,
        'not null' => FALSE,
        'description' => 'email of user',
      ),
      'phone_number' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'phone_number of user',
      ),
      'social_id' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'social_id of user',
      ),
      'profile_url' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'profile_url of user',
      ),
      'image_url' => array(
        'type' => 'varchar',
        'length' => 1000,
        'not null' => FALSE,
        'description' => 'image_url of user',
      ),
      'status' => array(
        'type' => 'text',
        'not null' => FALSE,
        'description' => 'status of user',
      ),
      'industry' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'industry of user',
      ),
      'country' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'country of user',
      ),
      'gender' => array(
        'type' => 'varchar',
        'length' => 10,
        'not null' => FALSE,
        'description' => 'gender of user',
      ),
      'location' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => FALSE,
        'description' => 'gender of user',
      ),
      'birth_date' => array(
        'type' => 'varchar',
        'length' => 30,
        'not null' => FALSE,
        'description' => 'Date of Birth of User.',
      ),
    ),
  );
}
