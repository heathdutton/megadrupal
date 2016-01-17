<?php
/**
 * @file
 * Contains functions for administration use of the kaltura core module.
 */

/**
 * Helper function that processes the partner registration with kaltura.
 *
 * If registration successful, the partner details are saved as system variables
 * (drupal DB) and the user is being redirected to the modules settings page
 * where he should see the details inserted and a configuration test which says
 * that the configuraion works.
 *
 * TODO set up for new API.
 * @changes:
 *  removed references to categories
 * @questions:
 *  this method created problems within getServiceConfiguration wrt the Logger
 */
function kaltura_register_partner($values) {
  // Get module version.
  $info = system_get_info('module', 'kaltura');
  $module_version = !empty($info['version']) ? $info['version'] : 'N/A';

  $partner_name = ($values['server_url'] == 'http://www.kaltura.com') ? NULL : $values['first_name'] . '_' . $values['last_name'];
  $cms_pass = kaltura_generate_cms_password();

  // Maybe mark fields them in the form.
  $addit_val_array = array('title', 'vertical', 'would_you_like_to_be_contacted');
  $additional = array();
  foreach ($addit_val_array as $val) {
    $parm = new KalturaKeyValue();
    $parm->key = $val;
    $parm->value = $values[$val];
    $additional[] = $parm;
  }

  $partner = new KalturaPartner();
  // What is this field?
  $partner->name = $values['company'];
  $partner->phone = $values['phone'];
  $partner->website = $values['website'];
  $partner->notificationUrl = $values['kaltura_partner_url2'] . '/?q=kaltura/notification_handler';

  if (!empty($partner_name)) {
    $partner->adminName = $partner_name;
    $partner->firstName = NULL;
    $partner->lastName = NULL;
  }
  else {
    $partner->firstName = $values['first_name'];
    $partner->lastName = $values['last_name'];
  }

  $partner->adminEmail = $values['email'];
  $partner->country = $values['country'];
  $partner->state = $values['state'];
  $partner->description = $values['k_description'] . '|DRUPAL' . VERSION . '|module ver ' . $module_version;
  $partner->commercialUse = "non-commercial_use";
  $partner->type = 102;
  $partner->appearInSearch = '1';
  $partner->additionalParams = $additional;

  $config = KalturaHelpers::getServiceConfiguration();
  $config->serviceUrl = $values['server_url'];
  $kaltura_client = new KalturaClient($config);
  // $kaltura_client->setConfig($config);

  watchdog('regis', print_r($partner, TRUE));

  try {
    $res = $kaltura_client->partner->register($partner, $cms_pass);
    $res->cms_pass = $cms_pass;
  }
  catch (Exception $e) {
    $res = $e;
  }

  return $res;
}

/**
 * Helper function to retrieve partner's details from kaltura.
 *
 * @changes
 *  - kaltura client config set up to reflect changes in the API;
 *  - getsecrets now called instead of getPartner;
 *  - return array built using Kaltura Partner object;
 */
function kaltura_get_partner_details($partner_id, $cms_email, $cms_password, $server_url) {
  libraries_load('KalturaClient');

  $config = KalturaHelpers::getServiceConfiguration();
  $config->serviceUrl = $server_url;
  $config->partnerId = $partner_id;
  $kaltura_client = new KalturaClient($config);

  try {
    // Here we are trying to retrieve the partner details from the kaltura
    // server.
    $result = $kaltura_client->partner->getsecrets($partner_id, $cms_email, $cms_password);
  }
  catch (Exception $e) {
    $result = $e;
  }

  return $result;
}

/**
 * Helper function that uses Kaltura's getDefaultWidget service to create some
 * default widgets for the partner, in case he does not have these yet.
 *
 * TODO: Useless unfinished function.
 *
 * @changes
 *  - changed client initialisation;
 *  - changed the getDefaultWidget function call to
 *    $kaltura_client->widget->get;
 * @questions
 *  - what do we do with the widget when we get them?
 */
function kaltura_create_widgets_for_partner($partner_id, $secret) {
  try {
    $helpers = new KalturaHelpers();
    $kaltura_client = $helpers->getKalturaClient(1);
  }
  catch (Exception $e) {
    watchdog_exception('kaltura', $e);
  }

  $widgets = new KalturaSettings();

  foreach ($widgets->kdp_widgets as $type => $uiconfs) {
    foreach ($uiconfs as $skin => $confs) {
    // That doesn't work for me yet.
    // $result = $kaltura_client->widget->get("_1_".$confs['view_uiconf']);
    // $result = $kaltura_client->widget->get("_1_".$confs['remix_uiconf']);
    }
  }
}
/**
 * Determines how the general settings form will look like.
 *
 * @return mixed
 *   one of 2 states of forms:
 *    1) registration form (if variables do not exist in the DB) either as SaaS
 *       or Kaltura CE
 *    2) notifications settings + configuration test + settings form
 */
function kaltura_registration_form() {
  global $base_url;

  drupal_add_css(drupal_get_path('module', 'kaltura') . '/style/kaltura_reg_form.css');
  drupal_add_js(drupal_get_path('module', 'kaltura') . '/js/jquery.validate.min.js', array('group' => JS_LIBRARY));
  drupal_add_js(drupal_get_path('module', 'kaltura') . '/js/additional-methods.js', array('group' => JS_LIBRARY));

  // Check if we are already registered if so show status page and partner info.
  if (variable_get('kaltura_partner_id')) {
    return kaltura_status_form();
  }

  // Load locale include file to get a country list from it.
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  $form['first_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['first_set']['server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server URL'),
    '#default_value' => variable_get('kaltura_server_url', 'http://www.kaltura.com'),
    '#description' => t('If you are working against Kaltura SaaS, leave this setting as it is. Otherwise, if you are working against a Kaltura CE or Kaltura On-Prem server, type the server URL here.'),
  );

  $form['first_set']['kaltura_partner_url2'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification URL'),
    '#default_value' => variable_get('kaltura_partner_url2', $base_url),
    '#description' => t('server notifications allows the Kaltura video platform to update your Drupal server about the status of the media content. This is especially useful when uploading content in various formats that needs to be transcoded. In order to support server side notifications, your server must be publicly available on the internet. The notification URL is where notifications are sent to and where a notification handler script resides.'),
  );

  $form['first_set']['new_partner'] = array(
    '#type' => 'radios',
    '#title' => t('Partner'),
    '#options' => array(
      'new' => t('New'),
      'existing' => t('Existing'),
    ),
    '#default_value' => t('new'),
  );

  $form['ex_partner'] = array(
    '#type' => 'fieldset',
    '#title' => t('Existing partner registration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="new_partner"]' => array('value' => t('existing')),
      ),
    ),
  );

  $form['ex_partner']['ex_partner_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner ID'),
    '#size' => 10,
  );

  $form['ex_partner']['ex_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
  );

  $form['ex_partner']['ex_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#maxlength' => 64,
  );

  $form['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('New partner registration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="new_partner"]' => array('value' => t('new')),
      ),
    ),
  );

  $form['registration']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-Mail'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['company'] = array(
    '#type' => 'textfield',
    '#title' => t('Company'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website'),
    '#size' => 30,
  );

  $form['registration']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Job Title'),
    '#size' => 30,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['vertical'] = array(
    '#type' => 'select',
    '#title' => t('Describe yourself'),
    // '#description' => t('/* description */'),
    '#options' => array(
      'Enterprise' => t('Enterprise / Small Business / Government Agency'),
      'Education' => t('Education Organization'),
      'Media' => t('Media Company / Agency'),
      'Service Provider' => t('CDN / ISP / Integrator / Hosting'),
      'Other' => t('Other'),
    ),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['country'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    // '#description' => t('/* description */'),
    '#options' => country_get_list(),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['registration']['state'] = array(
    '#type' => 'select',
    '#title' => t('State / Province'),
    // '#description' => t('/* description */'),
    '#options' => kaltura_us_states(),
    '#states' => array(
      'visible' => array(
        ':input[name="country"]' => array('value' => 'US'),
      ),
    ),
    '#default_value' => 'Not Applicable',
  );

  $form['registration']['would_you_like_to_be_contacted'] = array(
    '#type' => 'select',
    '#title' => t('Would you like a Kaltura Video Expert to contact you?'),
    // '#description' => t('/* description */'),
    '#options' => array(
      'yes' => t('Yes'),
      'no'  => t('Not right now'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="server_url"]' => array('value' => 'http://www.kaltura.com'),
      ),
    ),
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
    '#default_value' => 'yes',
  );

  $form['registration']['k_description'] = array(
    '#type' => 'textarea',
    '#title' => t('How do you plan to use Kaltura\'s video platform?'),
    '#cols' => 60,
  );

  $form['terms'] = array(
    '#type' => 'checkbox',
    '#title' => t('I accept !terms_of_use', array('!terms_of_use' => l(t('The Kaltura service Terms of Use'), 'http://corp.kaltura.com/terms_of_use_drupal'))),
    '#required' => TRUE,
    '#attributes' => array(
      'class' => array('required'),
      'title' => t('This field is required.'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('submit'),
  );

  return $form;
}

/**
 * Validate callback for kaltura_registration_form().
 *
 * @param $form
 * @param $form_state
 */
function kaltura_registration_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  if ($values['new_partner'] == 'new') {
    $required = array('first_name', 'last_name', 'email', 'phone', 'company', 'title', 'country', 'would_you_like_to_be_contacted');
    foreach ($required as $field) {
      if (empty($values[$field])) {
        form_set_error('', t('The field !field_name is required', array('!field_name' => str_replace('_', ' ', $field))));
      }
    }
  }
  else {
    $required = array('ex_partner_id', 'ex_email', 'ex_password');
    foreach ($required as $field) {
      if (empty($values[$field])) {
        form_set_error('', t('The field !field_name is required', array('!field_name' => str_replace('ex_', ' ', $field))));
      }
    }
  }
}

/**
 * Submit callback for kaltura_registration_form().
 *
 * @param $form
 * @param $form_state
 */
function kaltura_registration_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  libraries_load('KalturaClient');

  if ($values['new_partner'] == 'existing') {
    $res = kaltura_get_partner_details($values['ex_partner_id'], $values['ex_email'], $values['ex_password'], $values['server_url']);
  }
  else {
    $res = kaltura_register_partner($values);
  }

  if (!empty($res->id)) {
    variable_set('kaltura_partner_id', $res->id);
    variable_set('kaltura_subp_id', $res->id . '00');
    variable_set('kaltura_secret', $res->secret);
    variable_set('kaltura_admin_secret', $res->adminSecret);
    variable_set('kaltura_local_registration', 1);
    variable_set('kaltura_server_url', $values['server_url']);
    variable_set('kaltura_partner_url2', $values['kaltura_partner_url2']);
    variable_set('kaltura_notification_type', 1);
    kaltura_update_kaltura_partner(2, $res->notificationUrl);
    kaltura_create_widgets_for_partner($res->id, $res->secret);
    drupal_set_message(t('Congratulations! You have successfully installed the Kaltura Video Module and registered for a Kaltura Partner ID. Now you may want to <a href="!url">import all media entries from Kaltura</a>.', array('!url' => url('admin/config/media/kaltura/entries/import'))));
    drupal_goto('admin/config/media/kaltura');
  }
  else {
    $code = $res->getCode();
    $msg = $res->getMessage();
    if ($code == 'PARTNER_REGISTRATION_ERROR') {
      $msg = t('It seems you already have an account with Kaltura (the email you provided is in our records). If you want to create another Kaltura account, please enter a different email address.');
    }
    drupal_set_message($msg, 'error');
  }
}

/**
 * Helper function that returns a list of US states.
 *
 * TODO: Do we need to keep such a big array of data here?
 *
 * @access public
 * @return array
 */
function kaltura_us_states() {
  $state_list = array(
    'NA' => 'Not Applicable',
    'AL' => 'Alabama',
    'AK' => 'Alaska',
    'AZ' => 'Arizona',
    'AR' => 'Arkansas',
    'CA' => 'California',
    'CO' => 'Colorado',
    'CT' => 'Connecticut',
    'DE' => 'Delaware',
    'DC' => 'District Of Columbia',
    'FL' => 'Florida',
    'GA' => 'Georgia',
    'HI' => 'Hawaii',
    'ID' => 'Idaho',
    'IL' => 'Illinois',
    'IN' => 'Indiana',
    'IA' => 'Iowa',
    'KS' => 'Kansas',
    'KY' => 'Kentucky',
    'LA' => 'Louisiana',
    'ME' => 'Maine',
    'MD' => 'Maryland',
    'MA' => 'Massachusetts',
    'MI' => 'Michigan',
    'MN' => 'Minnesota',
    'MS' => 'Mississippi',
    'MO' => 'Missouri',
    'MT' => 'Montana',
    'NE' => 'Nebraska',
    'NV' => 'Nevada',
    'NH' => 'New Hampshire',
    'NJ' => 'New Jersey',
    'NM' => 'New Mexico',
    'NY' => 'New York',
    'NC' => 'North Carolina',
    'ND' => 'North Dakota',
    'OH' => 'Ohio',
    'OK' => 'Oklahoma',
    'OR' => 'Oregon',
    'PA' => 'Pennsylvania',
    'RI' => 'Rhode Island',
    'SC' => 'South Carolina',
    'SD' => 'South Dakota',
    'TN' => 'Tennessee',
    'TX' => 'Texas',
    'UT' => 'Utah',
    'VT' => 'Vermont',
    'VA' => 'Virginia',
    'WA' => 'Washington',
    'WV' => 'West Virginia',
    'WI' => 'Wisconsin',
    'WY' => 'Wyoming',
  );

  return $state_list;
}

/**
 * Implements hook_form().
 */
function kaltura_status_form() {
  global $base_url;

  $form['kaltura_notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Kaltura Notifications'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['kaltura_notifications']['kaltura_partner_url2'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification URL'),
    '#default_value' => variable_get('kaltura_partner_url2', $base_url),
    '#disabled' => TRUE,
    '#description' => t('Server notifications allows the Kaltura video platform to update your Drupal server about the status of the media content. This is especially useful when uploading content in various formats that needs to be transcoded. In order to support server side notifications, your server must be publicly available on the internet.'),
  );

  $form['kaltura_server_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server Status'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  list($admin_session_status, $admin_session_error) = kaltura_check_server_status($admin = 1);
  list($normal_session_status, $normal_session_error) = kaltura_check_server_status($admin = 0);

  $status_class = ($admin_session_status && $normal_session_status) ? 'ok' : 'bad';
  $session_status = ($admin_session_status && $normal_session_status) ? 'OK' : 'ERROR';

  $error_status = FALSE;
  $error = ' kaltura_error: ';

  if ($admin_session_error) {
    $error .= '<br />admin session: ' . $admin_session_error;
    $error_status = TRUE;
  }
  if ($normal_session_error) {
    $error .= '<br />normal session: ' . $normal_session_error;
    $error_status = TRUE;
  }

  // TODO: Rewrite prefixes.
  $form['kaltura_server_status']['kaltura_Admin_test_server'] = array(
    '#type' => 'item',
    '#title' => t('Drupal to Kaltura Session Test'),
    '#prefix' => '<div class="kaltura_status_' . $status_class . '">',
    '#markup' => 'Session status: ' . $session_status . (($error_status) ? $error : ''),
    '#suffix' => '</div>',
  );

  // TODO: Do we need this commented stuff??
  // Function kaltura_test_notification_config() is not implemented yet!
  // if (kaltura_variable_get_real('kaltura_notification_type', 0) == 1) {
  //   list($status, $text) = kaltura_test_notification_config();
  //     $prefix = '<div class="kaltura_status_'. (($status)? 'ok': 'bad') .'">';
  //     $form['kaltura_server_status']['kaltura_notification_status'] = array(
  //       '#type' => 'item',
  //       '#title' => t('Kaltura to Drupal Notification Status'),
  //       '#prefix' => $prefix,
  //       '#markup' => $text,
  //       '#suffix' => '</div>'
  //    );
  //  }

  // TODO: Rewrite prefixes.
  list($status, $text) = kaltura_test_cron_job_status();
  $prefix = '<div class="kaltura_status_' . (($status) ? 'ok' : 'bad') . '">';
  $form['kaltura_server_status']['kaltura_cron_job_status'] = array(
    '#type' => 'item',
    '#title' => t('Cron-Job Status'),
    '#prefix' => $prefix,
    '#markup' => $text,
    '#suffix' => '</div>',
  );

  list($status, $text) = kaltura_test_crossdomain();
  $prefix = '<div class="kaltura_status_' . (($status) ? 'ok' : 'bad') . '">';
  $form['kaltura_server_status']['kaltura_crossdomain_status'] = array(
    '#type' => 'item',
    '#title' => t('CrossDomain.xml Status'),
    '#prefix' => $prefix,
    '#markup' => $text,
    '#suffix' => '</div>',
  );

  $form['kaltura_partner_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Partner Info'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['kaltura_partner_settings']['kaltura_partner_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner ID'),
    '#default_value' => variable_get('kaltura_partner_id'),
    '#size' => 20,
    '#maxlength' => 10,
    '#disabled' => TRUE,
  );

  return $form;
}

/**
 * Page callback: Constructs a form for importing entries from Kaltura.
 *
 * @see kaltura_import_submit()
 *
 * @ingroup forms
 */
function kaltura_import($form, &$form_state) {
  try {
    $helpers = new KalturaHelpers();
    $client = $helpers->getKalturaClient(TRUE);

    if ($last_imported = variable_get('kaltura_last_imported')) {
      $filter = new KalturaMediaEntryFilter();
      $filter->updatedAtGreaterThanOrEqual = $last_imported + 1;

      $count = $client->media->count($filter) + count($helpers->filterOutUpToDateEntries($last_imported));
    }
    else {
      $count = $client->media->count();
    }

    if ($count) {
      $form['count']['#markup'] = format_plural($count, 'There is <strong>1</strong> media entry to be imported.', 'There are <strong>@count</strong> media entries to be imported.');

      $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Import'),
      );
    }
    else {
      $form['count']['#markup'] = t('The are no media entries that need to be imported.');
    }
  }
  catch (Exception $e) {
    watchdog_exception('kaltura', $e);

    if (module_exists('dblog')) {
      $message = t('The website encountered an unexpected error. More info in <a href="!url">logs</a>.', array('!url' => url('admin/reports/dblog')));
    }
    else {
      $message = t('The website encountered an unexpected error. More info in logs.');
    }
    drupal_set_message($message, 'error');
  }

  return $form;
}

/**
 * Form submission handler for kaltura_import().
 */
function kaltura_import_submit($form, &$form_state) {
  try {
    $last_updated = variable_get('kaltura_last_imported');

    $helpers = new KalturaHelpers();
    $client = $helpers->getKalturaClient(TRUE);

    // Use order by updated date (DESC) because if some entries will be
    // updated at the same time while we fetching them then those entries will
    // be fetched on the next cron run.
    $filter = new KalturaMediaEntryFilter();
    $filter->orderBy = KalturaMediaEntryOrderBy::UPDATED_AT_DESC;
    $filter->updatedAtGreaterThanOrEqual = $last_updated ? ($last_updated + 1) : NULL;

    $pager = new KalturaFilterPager();
    // @todo Make this value configurable.
    $pager->pageSize = 100;

    $max_time = NULL;
    $entry_ids = $last_updated ? $helpers->filterOutUpToDateEntries($last_updated) : array();

    $queue = DrupalQueue::get('kaltura_import_entries');
    // Cron won't do the same.
    $queue->deleteQueue();

    while (TRUE) {
      ++$pager->pageIndex;
      $result = $client->media->listAction($filter, $pager);

      if (empty($result->objects)) {
        break;
      }

      foreach ($result->objects as $entry) {
        $entry_ids[] = $entry->id;
      }

      // Store the maximum of 'updatedAt' property.
      if (!isset($max_time)) {
        $max_time = reset($result->objects)->updatedAt;
      }
    }

    $batch = array(
      'operations' => array(
        array('kaltura_batch_import', array($entry_ids, $max_time)),
      ),
      'title' => t('Importing Kaltura Media Entries'),
      'init_message' => t('Importing...'),
      'progress_message' => t('@estimate remained.'),
      'finished' => 'kaltura_batch_import_finished',
      'file' => drupal_get_path('module', 'kaltura') . '/includes/kaltura.admin.inc',
    );
    batch_set($batch);
  }
  catch (Exception $e) {
    watchdog_exception('kaltura', $e);

    if (module_exists('dblog')) {
      $message = t('The website encountered an unexpected error. More info in <a href="!url">logs</a>.', array('!url' => url('admin/reports/dblog')));
    }
    else {
      $message = t('The website encountered an unexpected error. More info in logs.');
    }
    drupal_set_message($message, 'error');
  }
}

/**
 * Imports Kaltura media entries in batch.
 *
 * @param array $entry_ids
 *   Entry IDs of items to be imported.
 * @param int $new_last_updated
 *   Timestamp of maximum of 'updatedAt' properties.
 */
function kaltura_batch_import(array $entry_ids, $new_last_updated, &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($entry_ids);

    // Store other needed information for 'finished' callback.
    $context['results']['new last updated'] = $new_last_updated;
  }

  // @todo Make the length value configurable.
  $ids = array_slice($entry_ids, $context['sandbox']['progress'], 100);
  kaltura_import_entries($ids);

  $context['message'] = t('Completed @current of @total.', array(
    '@current' => $context['sandbox']['progress'] + count($ids),
    '@total' => $context['sandbox']['max'],
  ));
  $context['sandbox']['progress'] += 100;

  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch finish callback.
 *
 * @param bool $success
 *   Means no fatal PHP errors were detected. All other error management
 *   should be handled using $results.
 * @param array $results
 *   Array of results returned from operations callbacks.
 * @param array $operations
 */
function kaltura_batch_import_finished($success, $results, $operations) {
  if ($success) {
    if (!empty($results['new last updated'])) {
      variable_set('kaltura_last_imported', $results['new last updated']);
    }

    drupal_set_message(t('Import completed.'));
  }
  else {
    drupal_set_message(t('Finished with an error.'));
  }
}

/**
 * Helper function that checks the drupal cron job status.
 *
 * Cron job is required to collect statictics about kaltura items.
 *
 * @return array
 */
function kaltura_test_cron_job_status() {
  if (variable_get('cron_last')) {
    return array(TRUE, t('Cron has run. Be sure that you have a cron job configured, so you don\'t need to run it manually. Statistics about kaltura items will only be updated at cron run.'));
  }
  else {
    return array(FALSE, t('Cron has not run. If you don\'t configure cron job, statistics about kaltura items will not be updated.'));
  }
}

/**
 * Helper function that tests the notification status of the module.
 *
 * This function tries to close a loop with the kaltura server in a "real-time"
 * test.
 * The notification handler sets a variable in the DB before this function run
 * ends.
 * Therefore we need to use the kaltura_variable_get_real() function to check
 * the variable status in the DB and not in the $_GLOBALS which are set before
 * this function even start.
 *
 * TODO: Not yet implemented.
 */
function kaltura_test_notification_config() {
  return array(TRUE, t('Test call not yet implemented, sorry'));

  if (kaltura_variable_get_real('kaltura_notification_status', 0) == 0) {
    $helpers = new KalturaHelpers();

    $session_user = $helpers->getSessionUser();
    $kaltura_client = $helpers->getKalturaClient();
    $result = $kaltura_client->testNotification($session_user);
    sleep(3);
  }

  if (kaltura_variable_get_real('kaltura_notification_status', 0) === 1) {
    return array(TRUE, t('Drupal server receives notification from Kaltura server.'));
  }

  return array(FALSE, t('Drupal server fails to receive notifications from Kaltura. Please fix your configuration or disable server notifications.'));
}

/**
 * Replace default Drupal's variable_get() function with a "real-time" one.
 *
 * This function checks a variable status in the DB and not in the $_GLOBALS.
 * TODO: why do we need this one????
 */
function kaltura_variable_get_real($var, $default) {
  $result = db_select('variable', 'v')
    ->fields('v')
    ->condition('name', $var, '=')
    ->execute()
    ->fetchAssoc();
  if (!$result) {
    return $default;
  }

  return unserialize($result['value']);
}

/**
 * Helper function that checks if crossdomain.xml is in the right place.
 *
 * @return array
 */
function kaltura_test_crossdomain() {
  global $base_url;
  $cd_req = drupal_http_request($base_url . '/crossdomain.xml');
  if ($cd_req->code == '200') {
    return kaltura_parse_crossdomain($cd_req->data);
  }
  else {
    // Fall back to fetching and retrieving file contents (should we
    // succeed if HTTP request has failed, though?
    $filename = $_SERVER['DOCUMENT_ROOT'] . '/crossdomain.xml';
    if ($cd_xml = @file_get_contents($filename)) {
      return kaltura_parse_crossdomain($cd_xml);
    }
    else {
      return array(
        FALSE,
        t('crossdomain.xml file could not be found in your site\'s root directory') . ' [' . $filename . ']' .
        t('Please read !help_link.', array('!help_link' => l(t('here'), 'admin/help/kaltura'))),
      );
    }
  }
}

/**
 * Helper function that checks if crossdomain.xml says the right things.
 *
 * @param $crossdomain_xml
 *
 * @return array
 */
function kaltura_parse_crossdomain($crossdomain_xml) {
  if (!empty($crossdomain_xml)) {
    $cd_xml_content = simplexml_load_string($crossdomain_xml);
    foreach ($cd_xml_content->children() as $child) {
      $kaltura_ok = FALSE;
      $headers_ok = FALSE;

      $atts = $child->attributes();
      if ($atts['domain'] == '*' || $atts['domain'] == '*.kaltura.com') {
        $kaltura_ok = TRUE;
      }
      if ($atts['headers'] == '*' && ($atts['domain'] == '*' || $atts['domain'] == '*.kaltura.com')) {
        $headers_ok = TRUE;
      }
      if ($kaltura_ok && $headers_ok) {
        return array(TRUE, t('crossdomain.xml is in place and seems to be OK'));
      }
    }
  }
  return array(
    FALSE,
    t('crossdomain.xml is in place, but it is probably not configured properly. Please read !help_link.', array('!help_link' => l(t('here'), 'admin/help/kaltura'))
    ),
  );
}

/**
 * Helper function that updates partner notification settings in kaltura's
 * server.
 *
 * @param int $notifications_config
 * @param string $notification_url
 *
 * @return null
 */
function kaltura_update_kaltura_partner($notifications_config = 2, $notification_url = '') {

  try {
    $partner = new KalturaPartner();
    $partner->allowMultiNotification = '1';
    $partner->notify = 1;
    $partner->notificationsConfig = '*=' . $notifications_config;
    $partner->notificationUrl = $notification_url ? $notification_url : url('kaltura/notification_handler', array('absolute' => TRUE));

    $helpers = new KalturaHelpers();
    $kaltura_client = $helpers->getKalturaClient(1);

    $result = $kaltura_client->partner->update($partner, TRUE);
    return $result;
  }
  catch (Exception $e) {
    watchdog_exception('kaltura', $e);
  }
}

/**
 * Helper function that tries to open session with the kaltura server.
 *
 * Used to verify partner details, and display errors if there are.
 *
 * @changed
 *  - added new variable_get statements required for the api calls;
 *  - updated KalturaClient creation process;
 *  - changed API calls;
 *
 * TODO: check correct format for error checking of result.
 */
function kaltura_check_server_status($admin) {
  try {
    $k_helpers = new KalturaHelpers();
    $k_helpers->getKalturaClient($admin);
    $kaltura_status = TRUE;
    $kaltura_error = '';
  }
  catch(Exception $ex) {
    $kaltura_status = FALSE;
    $kaltura_error = $ex->getMessage();
  }
  return array($kaltura_status, $kaltura_error);
}

/**
 * Helper function to generate a cms password.
 *
 * CMS is the "administration panel" for kaltura's partners in kaltura's
 * website.
 *
 * @todo Use Drupal methods.
 */
function kaltura_generate_cms_password() {
  $pass = md5(time() . ip_address());
  $pass = drupal_substr($pass, 0, 8);
  return $pass;
}

/**
 * Page callback: Constructs a form for the mapping of remote to local fields.
 *
 * @see kaltura_manage_fields_mapping_submit()
 * @see kaltura_menu()
 *
 * @ingroup forms
 */
function kaltura_manage_fields_mapping($form, &$form_state) {
  $form['kaltura_fields_map'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $map = variable_get('kaltura_fields_map', array());

  // Prepare the options array. It is the list of remote fields.
  // @todo Different options for different field types and cardinality.
  $options = array();
  $helpers = new KalturaHelpers();
  foreach ($helpers->getMetadataFieldList() as $profile_id => $data) {
    $profile_name = $data['metadata_profile']->name;

    foreach ($data['field_info'] as $field_name => $instance) {
      // Get the field type.
      $type = (string) $instance->attributes()->type;
      if (!$type && !empty($instance->simpleType)) {
        $type = (string) $instance->simpleType->restriction->attributes()->base;
      }
      if (!$type) {
        // This is some complex type of the field.
        continue;
      }
      if (substr($type, -4) == 'Type') {
        $type = substr($type, 0, -4);
      }

      $label = (string) $instance->annotation->appinfo->children()->label;
      $cardinality = (string) $instance->attributes()->maxOccurs;

      $value = $profile_id . '/' . $field_name;
      // Each option group corresponds to one profile. Options will be escaped
      // by form_select_options() so do not escape them here.
      $options[$profile_name][$value] = t('!label (type: !type; max num of values: !cardinality)', array('!label' => $label, '!type' => $type, '!cardinality' => $cardinality));
    }
  }

  // Create a select element for each of Kaltura Media Entry fields.
  $field_instances = field_info_instances('kaltura_entry', 'kaltura_entry');
  foreach ($field_instances as $field_name => $instance) {
    $field = field_info_field($field_name);
    $field_type = field_info_field_types($field['type']);

    if (!isset($field['columns']['value'])) {
      // Field types without 'value' column are not supported currently.
      continue;
    }

    $hints = array();
    $hints[] = t('The type of this field is <strong>@type</strong>.', array('@type' => $field_type['label']));

    if ($field['cardinality'] != FIELD_CARDINALITY_UNLIMITED) {
      $hints[] = t('This field can contain maximum of <strong>@cardinality</strong> value(s).', array('@cardinality' => $field['cardinality']));
    }

    $form['kaltura_fields_map'][$field_name] = array(
      '#type' => 'select',
      '#title' => $instance['label'] . ' <em>(' . $field_name . ')</em>',
      '#description' => implode("<br/>", $hints),
      '#options' => $options,
      '#empty_value' => '',
      '#default_value' => isset($map[$field_name]) ? $map[$field_name] : '',
      '#weight' => $instance['widget']['weight'],
    );
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'kaltura_manage_fields_mapping_submit';

  return $form;
}

/**
 * Form submission handler for kaltura_manage_fields_mapping().
 */
function kaltura_manage_fields_mapping_submit($form, &$form_state) {
  // Unset the timestamp of last imported entries so next cron run will also
  // fetch fields newly added to the map.
  variable_del('kaltura_last_imported');

  drupal_set_message(t('Now you may want to <a href="!url">update all media entries</a> so they will have new field values.', array('!url' => url('admin/config/media/kaltura/entries/import'))));
}
