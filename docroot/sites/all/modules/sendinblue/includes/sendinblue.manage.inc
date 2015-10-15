<?php
/**
 * @file
 * Manage class file.
 */

/**
 * Basic manager of module.
 */
class SendinblueManager {
  /**
   * The request url of Sendinblue api.
   */
  const API_URL = 'https://api.sendinblue.com/v2.0';

  /**
   * Variable name of Sendinblue access key.
   */
  const ACCESS_KEY = 'sendinblue_access_key';

  /**
   * Variable name of Sendinblue account email.
   */
  const ACCOUNT_EMAIL = 'sendinblue_account_email';

  /**
   * Variable name of Sendinblue account user name.
   */
  const ACCOUNT_USERNAME = 'sendinblue_account_username';

  /**
   * Variable name of Sendinblue account data.
   */
  const ACCOUNT_DATA = 'sendinblue_account_data';

  /**
   * Variable name of access_token.
   */
  const ACCESS_TOKEN = 'sendinblue_access_token';

  /**
   * Variable name of attribute lists.
   */
  const ATTRIBUTE_LISTS = 'sendinblue_attribute_lists';

  /**
   * Variable name of smtp details.
   */
  const SMTP_DETAILS = 'sendinblue_smtp_details';

  /**
   * Generate sidebar.
   *
   * @return string
   *   A html of sidebar.
   */
  public static function generateSidebar() {
    $print_html = '<div class="panel panel-default text-left box-border-box  small-content">';
    $print_html .= '<div class="panel-heading"><strong>' . t('About SendinBlue') . '</strong></div>';
    $print_html .= '<div class="panel-body"><p>';
    $print_html .= t('SendinBlue is an online software that allows you to send emails and SMS. Easily manage your Marketing campaigns, transactional emails and SMS.');
    $print_html .= '</p><ul class="sib-widget-menu"><li>';
    $print_html .= t('<a href="@sendinblue-about" target="_blank">> Who we are</a>', array('@sendinblue-about' => 'https://www.sendinblue.com/about/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li><li>';
    $print_html .= t('<a href="@sendinblue-price" target="_blank">> Pricing</a>', array('@sendinblue-price' => 'https://www.sendinblue.com/pricing/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li><li>';
    $print_html .= t('<a href="@sendinblue-feature" target="_blank">> Features</a>', array('@sendinblue-feature' => 'https://www.sendinblue.com/features/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li></ul></div></div>';
    $print_html .= '<div class="panel panel-default text-left box-border-box  small-content">';
    $print_html .= '<div class="panel-heading"><strong>' . t('Need Help ?') . '</strong></div>';
    $print_html .= '<div class="panel-body"><p>' . t('You have a question or need more information ?') . '</p><ul class="sib-widget-menu"><li>';
    $print_html .= t('<a href="@sendinblue-tutorial" target="_blank">> Tutorials</a>', array('@sendinblue-tutorial' => 'https://resources.sendinblue.com/category/tutorials/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li><li>';
    $print_html .= t('<a href="@sendinblue-faq" target="_blank">> FAQ</a>', array('@sendinblue-faq' => 'https://resources.sendinblue.com/category/faq/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</li></ul></div></div>';
    return $print_html;
  }

  /**
   * Generate Home layout of Log out.
   *
   * @return string
   *   A html of home page when log out.
   */
  public static function generateHomeLogout() {
    $print_html = '<div id="main-content"><div class="panel panel-default row small-content">';
    $print_html .= '<div class="page-header">';
    $print_html .= t('<span style="color: #777777;">Step1&nbsp;|&nbsp;</span><strong>Create a SendinBlue Account</strong>');
    $print_html .= '</div><div class="panel-body"><div class="col-md-9 row">';
    $print_html .= '<p>' . t('By Creating a free SendinBlue account, you will have access to send a confirmation message.') . '</p>';
    $print_html .= '<ul class="sib-home-feature">';
    $print_html .= '<li>-&nbsp;&nbsp;' . t('Collect your contacts and upload your lists') . '</li>';
    $print_html .= '<li>-&nbsp;&nbsp;' . t('Use SendinBlue SMTP to send your transactional emails') . '</li>';
    $print_html .= '<li class="home-read-more-content">-&nbsp;&nbsp;' . t('Email marketing builders') . '</li>';
    $print_html .= '<li class="home-read-more-content">-&nbsp;&nbsp;' . t('Create and schedule your email marketing campaigns') . '</li>';
    $print_html .= '<li class="home-read-more-content">-&nbsp;&nbsp;' . t('See all of') . '&nbsp;';
    $print_html .= t('<a href="@sendinblue-feature" target="_blank">SendinBlue\'s features</a></li>', array('@sendinblue-feature' => 'https://www.sendinblue.com/features/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</ul>' . t('<a href="@sendinblue-signup" target="_blank" style="margin-top: 10px;display: block;">>&nbsp;Create an account</a>', array('@sendinblue-signup' => 'https://www.sendinblue.com/users/signup/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</div></div></div><div class="panel panel-default row small-content"><div class="page-header">';
    $print_html .= '<span style="color: #777777;">' . t('Step2') . '&nbsp;|&nbsp;</span>';
    $print_html .= '<strong>' . t('Activate your account with your API key') . '</strong></div>';
    $print_html .= '<div class="panel-body"><div class="col-md-9 row"><div id="success-alert" class="alert alert-success" role="alert" style="display: none;">';
    $print_html .= t('You successfully activate your account.');
    $print_html .= '</div>';
    $print_html .= '<div id="failure-alert" class="alert alert-danger" role="alert" style="display: none;">' . t('Please input correct information.') . '</div>';
    $print_html .= '<p>' . t('Once your have created your SendinBlue account, activate this plugin to send all your transactional emails by using SendinBlue SMTP to make sure all of your emails get to your contacts inbox.') . '<br />';
    $print_html .= t('To activate your plugin, enter your API key.') . '<br /></p><p>';
    $print_html .= t('<a href="@sendinblue-apikey" target="_blank">>&nbsp;Get your API key from your account</a>', array('@sendinblue-apikey' => 'https://my.sendinblue.com/advanced/apikey/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</p><p><div class="col-md-7 row">';
    $form = drupal_get_form('sendinblue_login_form');
    $print_html .= drupal_render($form);
    $print_html .= '</div></p></div></div></div></div>';
    return $print_html;
  }

  /**
   * Generate Home layout of Log out.
   *
   * @return string
   *   A html of home page when login.
   */
  public static function generateHomeLogin() {

    $accesss_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $accesss_key);

    // Calculate total count of subscribers.
    $list_response = $mailin->getLists();
    if ($list_response['code'] != 'success') {
      $total_subscribers = 0;
    }
    else {
      $list_datas = $list_response['data'];
      $list_ids = array();
      foreach ($list_datas as $list_data) {
        $list_ids[] = $list_data['id'];
      }
      $user_response = $mailin->displayListUsers($list_ids, 1, 500);
      $total_subscribers = intval($user_response['data']['total_list_records']);
    }

    // Get account details.
    $account_email = variable_get(self::ACCOUNT_EMAIL, '');
    $account_username = variable_get(self::ACCOUNT_USERNAME, '');
    $account_data = variable_get(self::ACCOUNT_DATA, array());

    $print_html = '<div id="main-content">';
    $print_html .= '<div class="panel panel-default row small-content">';
    $print_html .= '<div class="page-header">';
    $print_html .= '<strong>' . t('My Account') . '</strong>';
    $print_html .= '</div><div class="panel-body"><span class="col-md-12"><b>' . t('You are currently logged in as :') . '</b></span>';
    $print_html .= '<div class="col-md-8 row" style="margin-bottom: 10px;"><p class="col-md-12" style="margin-top: 5px;">';
    $print_html .= check_plain($account_username) . ' - ' . check_plain($account_email) . '<br />';
    $count = count($account_data);
    for ($i = 0; $i < $count - 1; $i++) {
      $print_html .= check_plain($account_data[$i]['plan_type']) . ' - ' . check_plain($account_data[$i]['credits']) . ' ' .  t('credits') . '<br />';
    }
    $print_html .= '</p></div>';
    $print_html .= '<span class="col-md-12"><b>' . t('Contacts') . '</b></span><div class="col-md-8 row" style="margin-bottom: 10px;">';
    $print_html .= '<p class="col-md-7" style="margin-top: 5px;">' . t('You have') . '<span id="sendinblue_total_contacts">';
    $print_html .= check_plain($total_subscribers) . '</span> ' . t('contacts.') . '<br />';
    $print_html .= t('<a id="sendinblue_list_link" href="@sendinblue_list" target="_blank">>&nbsp;Access to the list of all my contacts</a>', array('@sendinblue_list' => 'https://my.sendinblue.com/users/list/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link'));
    $print_html .= '</p></div></div></div>';
    $print_html .= '<div class="panel panel-default row small-content"><div class="page-header">';
    $print_html .= '<strong>' . t('Transactional emails') . '</strong></div>';
    $print_html .= '<div class="panel-body">';
    $form = drupal_get_form('sendinblue_send_email_form');
    $print_html .= drupal_render($form);
    $print_html .= '</div></div></div>';
    return $print_html;
  }

  /**
   * Generate List page when log in.
   *
   * @return string
   *   A html of list page.
   */
  public static function generateListLogin() {
    $access_token = self::updateAccessToken();
    $print_html = '<iframe id="datamain" src="https://my.sendinblue.com/lists/index/access_token/' . check_plain($access_token) . '" width="100%" height="750" scrolling="yes"></iframe>';
    return $print_html;
  }

  /**
   * Generate Campaign page when log in.
   *
   * @return string
   *   A html of campaign.
   */
  public static function generateCampaignLogin() {
    $access_token = self::updateAccessToken();
    $print_html = '<iframe id="datamain" src="https://my.sendinblue.com/camp/listing/access_token/' . check_plain($access_token) . '" width="100%" height="750" scrolling="yes"></iframe>';
    return $print_html;
  }

  /**
   * Generate Statistic page when log in.
   *
   * @return string
   *   A html of statistic page.
   */
  public static function generateStatisticLogin() {
    $access_token = self::updateAccessToken();
    $print_html = '<iframe id="datamain" src="https://my.sendinblue.com/camp/message/access_token/' . check_plain($access_token) . '" width="100%" height="750" scrolling="yes"></iframe>';
    return $print_html;
  }

  /**
   * Check if current state is logged in.
   *
   * @return bool
   *   A status of login of user.
   */
  public static function isLoggedInState() {
    $access_key = variable_get(self::ACCESS_KEY, '');
    if ($access_key != '') {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Update access token.
   *
   * @return string
   *   An access token information.
   */
  public static function updateAccessToken() {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);

    // If exist old access_token, delete it.
    $old_access_token = variable_get(self::ACCESS_TOKEN, '');
    $mailin->deleteToken($old_access_token);

    // Get new access_token.
    $access_response = $mailin->getAccessTokens();
    $access_token = $access_response['data']['access_token'];
    variable_set(self::ACCESS_TOKEN, $access_token);
    return $access_token;
  }

  /**
   * Get email template by type.
   *
   * @param string $type
   *   A type of email.
   *
   * @return array
   *   An array of email content.
   */
  public static function getEmailTemplate($type = 'test') {
    $file = 'temp';
    $file_path = drupal_get_path('module', 'sendinblue') . '/asset/email-templates/' . $type . '/';
    // Get html content.
    $html_content = file_get_contents($file_path . $file . '.html');
    // Get text content.
    $text_content = file_get_contents($file_path . $file . '.txt');
    $templates = array('html_content' => $html_content, 'text_content' => $text_content);
    return $templates;
  }

  /**
   * Send mail.

   * @param string $type
   *   A type of email.
   * @param string $to_email
   *   A recipe address.
   * @param string $code
   *   A activate code.
   * @param int $list_id
   *   A list identification.
   * @param string $sender_id
   *   A sender identification.
   * @param string $template_id
   *   A template identification.
   */
  public static function sendEmail($type, $to_email, $code, $list_id, $sender_id = '-1', $template_id = '-1') {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);

    $account_email = variable_get(self::ACCOUNT_EMAIL, '');
    $account_username = variable_get(self::ACCOUNT_USERNAME, '');

    // Set subject info.
    if ($type == 'confirm') {
      $subject = t('Subscription confirmed');
    }
    elseif ($type == "double-optin") {
      $subject = t('Please confirm subscription');
    }
    elseif ($type == 'test') {
      $subject = t('[SendinBlue SMTP] test email');
    }

    $sender_email = $account_email;
    $sender_name = $account_username;

    if ($sender_email == '') {
      $sender_email = t('no-reply@sendinblue.com');
      $sender_name = t('SendinBlue');
    }

    // Get template html and text.
    $template_contents = self::getEmailTemplate($type);
    $html_content = $template_contents['html_content'];
    $text_content = $template_contents['text_content'];

    if ($type == "confirm" && $template_id != '-1') {
      $response = $mailin->getCampaign($template_id);
      if ($response['code'] == 'success') {
        $html_content = $response['data'][0]['html_content'];
        $subject = $response['data'][0]['subject'];
        if (($response['data'][0]['from_name'] != '[DEFAULT_FROM_NAME]') &&
          ($response['data'][0]['from_email'] != '[DEFAULT_FROM_EMAIL]') &&
          ($response['data'][0]['from_email'] != '')) {
          $sender_name = $response['data'][0]['from_name'];
          $sender_email = $response['data'][0]['from_email'];
        }
      }
    }

    // Send mail.
    $to = array($to_email => '');
    $from = array($sender_email, $sender_name);
    $null_array = array();
    $base_url = self::getBaseUrl();
    $site_domain = str_replace('https://', '', $base_url);
    $site_domain = str_replace('http://', '', $site_domain);

    $html_content = str_replace('{title}', $subject, $html_content);
    $html_content = str_replace('{site_domain}', $site_domain, $html_content);

    $text_content = str_replace('{site_domain}', self::getBaseUrl(), $text_content);
    $activate_email = variable_get('sendinblue_on', 0);
    if ($activate_email == '1') {
      $headers = array();
      $mailin->sendEmail($to, $subject, $from, $html_content, $text_content, $null_array, $null_array, $from, $null_array, $headers);
    }
    else {
      $headers  = 'MIME-Version: 1.0' . "\r\n";
      $headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
      $headers .= 'From: ' . $sender_name . ' <' . $sender_email . '>' . "\r\n";
      mail($to_email, $subject, $html_content, $headers);
    }
  }

  /**
   * Get Base URL.
   *
   * @return string
   *   A base url of the site.
   */
  public static function getBaseUrl() {
    global $base_url;
    return $base_url;
  }

  /**
   * Get Attribute lists.
   *
   * @return array
   *   An array of attributes.
   */
  public static function getAttributeLists() {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);
    $response = $mailin->getAttributes();
    if (($response['code'] == 'success') && (is_array($response['data']))) {
      $attributes = array_merge($response['data']['normal_attributes'], $response['data']['category_attributes']);
      variable_set(self::ATTRIBUTE_LISTS, $attributes);
      return $attributes;
    }
    return array();
  }

  /**
   * Get template list.
   *
   * @return array
   *   An array of template.
   */
  public static function getTemplateList() {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);
    $response = $mailin->getCampaigns('template');
    $templates = array(array(
      'id' => '-1',
      'name' => 'Default',
      ),
    );
    if (($response['code'] == 'success') && (is_array($response['data']))) {
      foreach ($response['data']['campaign_records'] as $template) {
        $templates[] = array(
          'id' => $template['id'],
          'name' => $template['campaign_name'],
        );
      }
    }
    return $templates;
  }

  /**
   * Get lists.
   *
   * @return array
   *   An array of lists.
   */
  public static function getLists() {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);
    $response = $mailin->getLists();
    if (($response['code'] == 'success') && (is_array($response['data']))) {
      return $response['data'];
    }
    return array();
  }

  /**
   * Get list name form id.
   *
   * @param int $list_id
   *   A list id.
   *
   * @return string
   *   A list name.
   */
  public static function getListNameById($list_id) {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);
    $response = $mailin->getList($list_id);
    if (($response['code'] == 'success') && (is_array($response['data']))) {
      return $response['data']['name'];
    }
    return '';
  }

  /**
   * Get sender list.
   *
   * @return array
   *   An array of senders.
   */
  public static function getSenderList() {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);
    $response = $mailin->getSenders('');
    $senders = array(array('id' => '-1', 'name' => 'Default'));
    if (($response['code'] == 'success') && (is_array($response['data']))) {
      foreach ($response['data'] as $sender) {
        $senders[] = array(
          'id' => $sender['from_email'],
          'name' => $sender['from_email'],
        );
      }
    }
    return $senders;
  }

  /**
   * Check the email address of subscriber.
   *
   * @param string $email
   *   An email address.
   * @param string $list_id
   *   A list id.
   *
   * @return array
   *   A response information.
   */
  public static function validationEmail($email, $list_id) {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);
    $response = $mailin->getUser($email);
    if ($response['code'] == 'failure') {
      $ret = array(
        'code' => 'success',
        'listid' => array(),
      );
      return $ret;
    }

    $listid = $response['data']['listid'];
    if (!is_array($listid)) {
      $listid = array();
    }
    if ($response['data']['blacklisted'] == 1) {
      $ret = array(
        'code' => 'update',
        'listid' => $listid,
      );
    }
    else {
      if (!in_array($list_id, $listid)) {
        $ret = array(
          'code' => 'success',
          'listid' => $listid,
        );
      }
      else {
        $ret = array(
          'code' => 'already_exist',
          'listid' => $listid,
        );
      }
    }
    return $ret;
  }

  /**
   * Subscriber user.
   *
   * @param string $email
   *   An email address of subscriber.
   * @param array $info
   *   A data of subscriber.
   * @param array $listids
   *   An array of list id.
   *
   * @return string
   *   A response information.
   */
  public static function subscribeUser($email, $info = array(), $listids = array()) {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);
    $response = $mailin->createUpdateUser($email, $info, 0, $listids, NULL);
    return $response['code'];
  }

  /**
   * Get subscriber data by email on drupal table.
   *
   * @param string $email
   *   An email address.
   *
   * @return string
   *   A details of subscriber.
   */
  public static function getSubscriberByEmail($email) {
    $record = db_select('sendinblue_contact', 'c')->fields('c')->condition('c.email', $email, '=')->execute()->fetchAssoc();
    return $record;
  }

  /**
   * Add subscriber on drupal table.
   *
   * @param array $data
   *   A data to add in table.
   */
  public static function addSubscriberTable($data = array()) {
    db_insert('sendinblue_contact')->fields(array(
      'email' => $data['email'],
      'info' => $data['info'],
      'code' => $data['code'],
      'is_active' => $data['is_active'],
    ))->execute();
  }

  /**
   * Update smtp details.
   *
   * @return string|bool
   *   A access token if exist, else 0.
   */
  public static function updateSmtpDetails() {
    $access_key = variable_get(self::ACCESS_KEY, '');
    $mailin = new SendinblueMailin(self::API_URL, $access_key);
    $response = $mailin->getSmtpDetails();
    if ($response['code'] == 'success') {
      if ($response['data']['relay_data']['status'] == 'enabled') {
        $smtp_details = $response['data']['relay_data']['data'];
        variable_set(self::SMTP_DETAILS, $smtp_details);
        return $smtp_details;
      }
      else {
        $smtp_details = array(
          'relay' => FALSE,
        );
        variable_set('sendinblue_on', 0);
        variable_set(self::SMTP_DETAILS, $smtp_details);
        return $smtp_details;
      }
    }

    return FALSE;
  }
}
