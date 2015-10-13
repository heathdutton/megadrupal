<?php
/**
 * @file
 * Admin page callback file for the lr_social_profile_data module.
 */
module_load_include('inc', 'lr_social_login', 'includes/lr_social_login.admin');
/**
 * Function that build an abstract representation of a HTML form for Social Sharing settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 * @return array A renderable form array for the respective request.
 */
function lr_social_profile_data_settings_form($form, &$form_state) {
  $group = 'social_profile_data';
  $form[$group] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Profile Data'),
    '#group' => 'settings',
  );
  $form[$group]['social_profile_data_selection'] = array(
    '#type' => 'item',
    '#markup' => '<b>' . t('User Profile Data Selection') . '</b>',
  );
  $form[$group]['lr_social_profile_data_profile_selection'] = array(
    '#title' => t('Please select the user profile data fields you would like to save in your database:'),
    '#description' => t('For a list of all fields <a href="!loginradius" target="_blank"> https://secure.loginradius.com/datapoints </a>', array('!loginradius' => 'https://secure.loginradius.com/datapoints')),
    '#type' => 'checkboxes',
    '#options' => array(
      'basic_profile_data' => t('Basic Profile Data<a title="Data fields include: Social ID, Social ID Provider, Prefix, First Name, Middle Name, Last Name, Suffix, Full Name, Nick Name, Profile Name, Birthdate, Gender, Country Code, Country Name, Thumbnail Image Url, Image Url, Local Country, Profile Country"  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      'extended_location_data' => t('Extended Location Data<a title="Data fields include: Main Address, Hometown, State, City, Local City, Profile City, Profile Url, Local Language, Language" style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      'extended_profile_data' => t('Extended Profile Data<a title="Data fields include: Website, Favicon, Industry, About, Timezone, Verified, Last Profile Update, Created, Relationship Status, Favorite Quote, Interested In, Interests, Religion, Political View, HTTPS Image Url, Followers Count, Friends Count, Is Geo Enabled, Total Status Count, Number of Recommenders, Honors, Associations, Hirable, Repository Url, Age, Professional Headline, Provider Access Token, Provider Token Secret, Positions, Companies, Education, Phone Numbers, IM Accounts, Addresses, Sports, Inspirational People, Skills, Current Status, Certifications, Courses, Volunteer, Recommendations Received, Languages, Patents, Favorites" style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      'followed_companies_on' => t('Followed Companies on LinkedIn<a title="A list of all the companies this user follows on LinkedIn."  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      'facebook_profile_events' => t('Facebook Profile Events<a title="A list of events (birthdays, invitation, etc.) on the Facebook profile of user"  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      'status_message' => t('Status Messages<a title="Facebook wall activity, Twitter tweets and LinkedIn status of the user, including links"  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      'facebook_posts' => t('Facebook Posts<a title="Facebook posts of the user, including links"  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      'twitter_mentions' => t('Twitter Mentions<a title="A list of tweets that the user is mentioned in."  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      'lr_groups' => t('Groups<a title="A list of the Facebook groups of user."  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
      'lr_contacts' => t('Contacts/Friend Data<a title="For email providers (Google and Yahoo), a list of the contacts of user in his/her address book. For social networks (Facebook, Twitter, and LinkedIn), a list of the people in the network of user."  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
    ),
    '#default_value' => variable_get('lr_social_profile_data_profile_selection', array('')),
  );
  $form[$group]['lr_social_profile_data_show_profile'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to show all the saved user profile data for each user in the Drupal admin panel?<a title="If enabled, you will be able to see a list of the saved data collected from each user on the PEOPLE page in the Drupal admin panel."  style="text-decoration:none"> (<span style="color:#3CF;">?</span>)</a>'),
    '#default_value' => variable_get('lr_social_profile_data_show_profile', 0),
    '#options' => array(
      1 => t('YES, display the option to view profile data on the  <a href="@block_lr_social_profile_data" target="_blank">People</a> page', array('@block_lr_social_profile_data' => url('admin/people'))),
      0 => t('No'),
    ),
  );
  $form['#submit'][] = 'lr_social_profile_data_settings_form_submit';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit social profile data settings and create social profiles tables.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 *
 * @return bool
 */
/** @noinspection PhpInconsistentReturnPointsInspection */
/** @noinspection PhpInconsistentReturnPointsInspection */
function lr_social_profile_data_settings_form_submit($form, &$form_state) {
  if (!lr_social_login_save_settings($form_state)) {
    return FALSE;
  }
  $lr_social_profile_data_profile_selection = explode(',', implode(',', variable_get('lr_social_profile_data_profile_selection', '')));
  module_load_include('inc', 'lr_social_profile_data', 'includes/lr_social_profile_data.sql');

  if (in_array('basic_profile_data', $lr_social_profile_data_profile_selection)) {
    lr_social_profile_data_check_table_exists_and_create('loginradius_basic_profile_data');
    lr_social_profile_data_check_table_exists_and_create('loginradius_emails');
  }

  if (in_array('extended_profile_data', $lr_social_profile_data_profile_selection)) {
    $extended_profile_data_array = array(
      'loginradius_extended_profile_data',
      'loginradius_positions',
      'loginradius_companies',
      'loginradius_education',
      'loginradius_phone_numbers',
      'loginradius_IMaccounts',
      'loginradius_addresses',
      'loginradius_sports',
      'loginradius_inspirational_people',
      'loginradius_skills',
      'loginradius_current_status',
      'loginradius_certifications',
      'loginradius_courses',
      'loginradius_volunteer',
      'loginradius_recommendations_received',
      'loginradius_languages',
      'loginradius_patents',
      'loginradius_favorites',
      'loginradius_books',
      'loginradius_games',
      'loginradius_television_show',
      'loginradius_movies',
    );

    foreach ($extended_profile_data_array as $table_name) {
      lr_social_profile_data_check_table_exists_and_create($table_name);
    }
  }

  $create_table = array(
    'extended_location_data' => 'loginradius_extended_location_data',
    'followed_companies_on' => 'loginradius_linkedin_companies',
    'facebook_profile_events' => 'loginradius_facebook_events',
    'status_message' => 'loginradius_status',
    'facebook_posts' => 'loginradius_facebook_posts',
    'twitter_mentions' => 'loginradius_twitter_mentions',
    'lr_groups' => 'loginradius_groups',
    'lr_contacts' => 'loginradius_contacts',
  );

  foreach ($create_table as $table_key => $table_name) {
    lr_social_profile_data_check_table_selected_in_database($table_key, $table_name, $lr_social_profile_data_profile_selection);
  }
}

/**
 * Check table exist int database and get scheme of that table and create table.
 *
 * @param string $table_name Database table name
 */
function lr_social_profile_data_check_table_exists_and_create($table_name) {
  if (!db_table_exists($table_name)) {
    $function_table_name = 'lr_social_profile_data_' . $table_name;
    $schema = $function_table_name();
    db_create_table($table_name, $schema);

  }
}

/**
 * Check admin selected this option to create table in your database.
 *
 * @param string $key Stored key value of extended data in database.
 * @param string $value Database table name
 * @param array $profile_selection Array of values that contain the extended data selection
 */
function lr_social_profile_data_check_table_selected_in_database($key, $value, $profile_selection) {
  if (in_array($key, $profile_selection)) {
    lr_social_profile_data_check_table_exists_and_create($value);
  }
}
