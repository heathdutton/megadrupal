<?php
/**
 * @file
 * This file registries new communication service.
 */

/**
 * Returns registered services as array.
 *
 * @return array $services
 */
/**
 * This file registries new communication service.
 */
function reach_us_services() {
  $moreservices = module_invoke_all('reach_us_add_services');
  $services = array();
  // Phone call.
  $services['phone']['label'] = t('Phone Number');
  $services['phone']['linktext'] = t('Call us');
  $services['phone']['description'] = t('Enter your phone number to receive call from visitor.');
  $services['phone']['schema'] = 'tel:!id';
  $services['phone']['weight'] = 1;
  $services['phone']['valid'] = '_reach_us_validate_phone';

  // SMS / Text.
  $services['sms']['label'] = t('SMS / Text');
  $services['sms']['linktext'] = t('SMS / Text us');
  $services['sms']['description'] = t('Enter your phone number to receive SMS from visitor.');
  $services['sms']['schema'] = 'sms:!id';
  $services['sms']['schemamsg'] = 'sms:!id?body=!message';
  $services['sms']['weight'] = 2;
  $services['sms']['valid'] = '_reach_us_validate_sms';

  // Email.
  $services['email']['label'] = t('Email');
  $services['email']['linktext'] = t('Email us');
  $services['email']['description'] = t('Email ID to contact. It will open email client when you click email icon');
  $services['email']['schema'] = 'mailto:!id';
  $services['email']['schemamsg'] = 'mailto:!id?subject=!message';
  $services['email']['weight'] = 3;
  $services['email']['valid'] = '_reach_us_validate_email';

  // Facebook Chat messenger.
  $services['fbchat']['label'] = t('Facebook Chat');
  $services['fbchat']['linktext'] = t('Chat with us in facebook');
  $services['fbchat']['description'] = t('Enter your facebook unique User id, its not your username or email. Get your facebook ID <a href = "@facebookid" target = "_blank">here.</a>', array('@facebookid' => 'http://findmyfacebookid.com/'));
  $services['fbchat']['schema'] = 'fb://messaging/compose/!id';
  $services['fbchat']['weight'] = 4;
  $services['fbchat']['valid'] = '_reach_us_validate_fbchat';

  // Whatsapp.
  $services['whatsapp']['label'] = t('Whatsapp');
  $services['whatsapp']['linktext'] = t('Whatsapp with us');
  $services['whatsapp']['description'] = t('Enter your whatsapp phone number. It works only in IOS');
  $services['whatsapp']['schema'] = 'whatsapp://send?abid=!id';
  $services['whatsapp']['schemamsg'] = 'whatsapp://send?abid=!id&text=!message';
  $services['whatsapp']['weight'] = 5;
  $services['whatsapp']['valid'] = '_reach_us_validate_whatsapp';

  // Apple facetime.
  $services['facetime']['label'] = t('Facetime');
  $services['facetime']['linktext'] = t('Facetime with us');
  $services['facetime']['description'] = t('Apple Facetime app for calling and messaging, Enter your facetime id.  <strong>address</strong>  or  <strong>MSISDN</strong>  or <strong>mobile number</strong>');
  $services['facetime']['schema'] = 'facetime://!id';
  $services['facetime']['weight'] = 6;
  $services['facetime']['valid'] = '_reach_us_validate_facetime';

  // Skype call.
  $services['skypecall']['label'] = t('Skype Call');
  $services['skypecall']['linktext'] = t('Skype Call with us');
  $services['skypecall']['description'] = t('Enter your <strong>Skype ID</strong> or <strong>phone number</strong> to receive call from visitor.');
  $services['skypecall']['schema'] = 'skype:!id?call';
  $services['skypecall']['weight'] = 6;
  $services['skypecall']['valid'] = '_reach_us_validate_skype_call';

  // Skype chat.
  $services['skypechat']['label'] = t('Skype Chat');
  $services['skypechat']['linktext'] = t('Skype Chat with us');
  $services['skypechat']['description'] = t('Enter your Skype ID  to receive message from visitor.');
  $services['skypechat']['schema'] = 'skype:!id?chat';
  $services['skypechat']['weight'] = 7;
  $services['skypechat']['valid'] = '_reach_us_validate_skype_chat';

  // Yahoo Messenger.
  $services['ymsgr']['label'] = t('Yahoo Messenger');
  $services['ymsgr']['linktext'] = t('Yahoo chat with us');
  $services['ymsgr']['description'] = t('Enter your Yahoo Messenger ID  to receive message from visitor.');
  $services['ymsgr']['schema'] = 'ymsgr:sendIM?!id';
  $services['ymsgr']['weight'] = 8;
  $services['ymsgr']['valid'] = '_reach_us_validate_ymsgr';

  // AIM.
  $services['aim']['label'] = t('AIM Chat');
  $services['aim']['linktext'] = t('AIM Chat with us');
  $services['aim']['description'] = t('Enter your AIM screen name to receive message from visitor.');
  $services['aim']['schema'] = 'aim:goim?screenname=!id';
  $services['aim']['schemamsg'] = 'aim:goim?screenname=!id&message=!message';
  $services['aim']['weight'] = 9;
  $services['aim']['valid'] = '_reach_us_validate_aim';

  // Add more services from modules hooked their own services.
  foreach ($moreservices as $key => $moreservice) {
    $services[$key] = $moreservice;
  }
  return $services;
}

/**
 * Validate the phone number to call.
 */
function _reach_us_validate_phone($id) {
  $regex = '/^([0-9\(\)\/\+ \-]*)$/';
  if (preg_match($regex, $id)) {
    return FALSE;
  }
  else {
    return t('Enter the valid phone number to call');
  }
}

/**
 * Validate the phone number for SMS.
 */
function _reach_us_validate_sms($id) {
  $regex = '/^([0-9\(\)\/\+ \-]*)$/';
  if (preg_match($regex, $id)) {
    return FALSE;
  }
  else {
    return t('Enter the valid phone number for sending SMS');
  }
}

/**
 * Validate the phone number for whatsapp.
 */
function _reach_us_validate_whatsapp($id) {
  $regex = '/^([0-9\(\)\/\+ \-]*)$/';
  if (preg_match($regex, $id)) {
    return FALSE;
  }
  else {
    return t('Whatsapp number should be a valid phone number');
  }
}

/**
 * Validate Email ID for AIM chat.
 */
function _reach_us_validate_aim($id) {
  if (valid_email_address($id)) {
    return FALSE;
  }
  else {
    return t('AIM id should be a valid email address');
  }
}

/**
 * Validate Email ID for Yahoo chat.
 */
function _reach_us_validate_ymsgr($id) {
  if (valid_email_address($id)) {
    return FALSE;
  }
  else {
    return t('Yahoo Messenger id should be a valid email address');
  }
}

/**
 * Validate Skype ID, Phone number and Microsoft email ID for skype call.
 */
function _reach_us_validate_skype_call($id) {
  $skypeid = '/^[a-z][a-z0-9\.,\-_]{5,31}$/i';
  $phone = '/^([0-9\(\)\/\+ \-]*)$/';
  if (preg_match($skypeid, $id) || preg_match($phone, $id) || valid_email_address($id)) {
    return FALSE;
  }
  else {
    return t('Skype call id should be either skype ID or Microsoft ID or a valid Phone Number');
  }
}

/**
 * Validate Skype ID and microsoft email ID for skype chat.
 */
function _reach_us_validate_skype_chat($id) {
  $skypeid = '/^[a-z][a-z0-9\.,\-_]{5,31}$/i';
  if (preg_match($skypeid, $id) || valid_email_address($id)) {
    return FALSE;
  }
  else {
    return t('Skype chat id should be either skype ID or Microsoft ID.');
  }
}

/**
 * Validate Phone number and email ID for Apple Facetime.
 */
function _reach_us_validate_facetime($id) {
  $phone = '/^([0-9\(\)\/\+ \-]*)$/';
  if (preg_match($phone, $id) || valid_email_address($id)) {
    return FALSE;
  }
  else {
    return t('Facetime ID should be either phone number or email ID.');
  }
}

/**
 * Validate Facebook User ID.
 */
function _reach_us_validate_fbchat($id) {
  $regex = '/^\d+$/';
  if (preg_match($regex, $id)) {
    return FALSE;
  }
  else {
    return t('Enter FB ID which is only numerical. Get your facebook ID <a href = "@facebookid" target = "_blank">here.</a>', array('@facebookid' => 'http://findmyfacebookid.com/'));
  }
}

/**
 * Validate Email Address.
 */
function _reach_us_validate_email($id) {
  if (valid_email_address($id)) {
    return FALSE;
  }
  else {
    return t('Enter valid email ID');
  }
}
