<?php
/**
 * @file * Admin include file. 
*/

/** 
 * Displays the form for the standard settings tab.
 * 
 * @return array 
 * A structured array for use with Forms API. 
*/ 
function intel_mailchimp_admin_config($form, $form_state) {
  $form = array_merge($form, intel_mailchimp_admin_config_form($form, $form_state));
  return system_settings_form($form);
}

function intel_mailchimp_admin_config_form($form, $form_state) {
  $form = array();
 
  $api_connection = 0;
  $options = array('none' => 'Select list');
  $q = mailchimp_get_api_object();
  if ($q) {
    $lists = mailchimp_get_lists();
    if (!empty($lists)) {
      foreach ($lists as $key => $list) {
        $options[$list['id']] = $list['name'];
      }
    }
    else {
      drupal_set_message(t('You do not have any Mailchimp lists defined.'), 'error');
    }
  }
  else {
    drupal_set_message(t('Could not get valid Mailchimp API object'), 'error');
  }
  $list_id = variable_get('intel_mailchimp_default_list_id', '');
  $form['intel_mailchimp_default_list_id'] = array(
    '#type' => 'select',
    '#title' => t('Default list'),
    '#options' => $options,
    '#default_value' => $list_id,
    '#description' => t('Select the mailchimp list you want to sync Intelligence data.'),
  ); 
  $options = array(
    'update' => t('Only sync if contact already exists in MailChimp'),
    'create,update' => t('Create contact in MailChimp if one does not already exist'),
  );
  $form['intel_mailchimp_sync_crud'] = array(
    '#type' => 'radios',
    '#title' => t('Sync mode'),
    '#options' => $options,
    '#default_value' => variable_get('intel_mailchimp_sync_crud', 'update'),
    //'#description' => t('Select the mailchimp list you want to sync Intelligence data.'),
  );  
  if ($list_id && isset($lists[$list_id])) {
    $list = $lists[$list_id];
    $mvars = $list['mergevars'];
    $prop_info = intel_intel_visitor_property_info();
    $options = intel_visitor_property_select_options();
    $system_vars = intel_mailchimp_system_mergevars();
    $form['property-map'] = array(
      '#type' => 'fieldset',
      '#title' => t('Property map'),
      '#collapsible' => FALSE,
    );
    foreach ($mvars AS $mvar) {
      $tag = $mvar['tag'];
      // skip system variables
      if (in_array($tag, $system_vars)) {
        continue;
      }
      $key = 'intel_mailchimp_property_map_' . $list_id . '_' . $mvar['tag'];
      $form['property-map'][$key] = array(
        '#type' => 'select',
        '#title' => "{$mvar['name']} ({$mvar['tag']})",
        '#options' => $options,
        '#default_value' => variable_get($key, ''),
        '#description' => t('Select an intel property to map to MailChimp merge vars.'),
      );       
    }
  }
  return $form;
}

function intel_mailchimp_contact($visitor, $action = '') {
  $visitor_name = (!empty($visitor->name)) ? $visitor->name : '[No Name]';
  //drupal_set_title(t('MailChimp contact: @title', array('@type' => $visitor_name, '@title' => $visitor_name)), PASS_THROUGH);
  drupal_set_title(t('MailChimp contact'));
  $list_id = variable_get('intel_mailchimp_default_list_id', '');
  
  $output = '';  

  if (!empty($_GET['debug'])) {
    dpm($visitor->getProperties());//
  }
  $ext_data = $visitor->ext_data;
  if (($action == 'resync')
      && (!empty($ext_data['mailchimp']['email']) || !empty($visitor->email))
    ) {
    intel_mailchimp_intel_sync_visitordata($visitor);
    $visitor->load();
    $ext_data = $visitor->ext_data;
    drupal_set_message(t('MailChimp data resynced.'));
    drupal_goto('visitor/' . arg(1) . '/mailchimp');
  }
  elseif (empty($ext_data['mailchimp']['email']) 
      && (!empty($visitor->email))
    ) {
    intel_mailchimp_intel_sync_visitordata($visitor);
    $visitor->load();
    $ext_data = $visitor->ext_data;
    drupal_set_message(t('MailChimp data resynced.'));
  }
  
  if (empty($ext_data['mailchimp']['leid']) || empty($list_id)) {
    drupal_set_message('This contact has not been linked to a MailChimp contact.', 'error');
    $output = 'Unable to fetch contact from MailChimp.';
    return $output;
  }
  $profile_url = 'https://us3.admin.mailchimp.com/lists/members/view?id=' . $ext_data['mailchimp']['leid'];
  if ($action == 'launch') {
    //$script = "window.open('{$visitor->ext_data['hubspot']['profile_url']}', 'hubspot');";
    //drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
    drupal_goto($profile_url);
  }
  
  $list_id = variable_get('intel_mailchimp_default_list_id', '');
  if (!$list_id || !$visitor->email) {
    return TRUE;
  }
  $email = $visitor->getVar('ext', 'mailchimp', 'email');
  if (!$email) {
    $email = $visitor->email;
  }
  $api = mailchimp_get_api_object();
  $member = mailchimp_get_memberinfo($list_id, $email);
  $merge_vars = $api->listMergeVars($list_id);
  $fields = array();
  foreach ($merge_vars AS $i => $var) {
    $fields[$var['tag']] = $var;
  }

if (!empty($_GET['debug'])) {
  dpm($member);//
  dpm($fields);//
}
 
  require_once drupal_get_path('module', 'intel') . "/admin/intel.admin_theming.inc";
  $ext_link_options = array(
    'attributes' => array(
      'target' => '_blank',
    ),
  );
  $vdata = $visitor->data;
  $vars = array(
    'sidebar' => array(),
    'header' => array(),
    'main' => array(),
  );
  $name = $member['merges']['FNAME'] . ' ' . $member['merges']['LNAME'];
  $link = l(t('View in MailChimp'), $profile_url, $ext_link_options);
  $h1 = '<div style="float: left;">' . $name . '</div><div style="float: right; font-size: .4em;">' . $link . '</div>';
  $vars['h1'] = $h1;
  $vars['h2'] = $member['email'];
  
  /*
  $elem = array(
    '#type' => 'markup',
    '#markup' => '<span style="font-weight: bold;">' . l(t('View in MailChimp'), $profile_url, $ext_link_options) . '</span>', 
  );
  $vars['main']['contact-link'] = $elem;
  */
  
  $elem = array(
    '#type' => 'intel_profile_value',
    '#label' => t('Status'),
    '#value' => $member['status'], 
    '#theme' => 'intel_profile_value',
  );
  $vars['header']['status'] = $elem;
  
  $elem = array(
    '#type' => 'intel_profile_value',
    '#label' => t('Rating'),
    '#value' => $member['member_rating'] . ' ' . t('out of 5'), 
    '#theme' => 'intel_profile_value',
  );
  $vars['header']['member-rating'] = $elem;
  
  $elem = array(
    '#type' => 'intel_profile_value',
    '#label' => t('Subscribed'),
    '#value' => format_date(strtotime($member['timestamp']), 'long'), 
    '#theme' => 'intel_profile_value',
  );
  $vars['header']['subscribe-date'] = $elem;

  $elem = array(
    '#type' => 'intel_profile_value',
    '#label' => t('Profile updated'),
    '#value' => format_date(strtotime($member['info_changed']), 'long'), 
    '#theme' => 'intel_profile_value',
  );
  $vars['header']['profile-updated'] = $elem;

  
  $items = array();
  foreach ($fields AS $key => $field) {
    $value = $member['merges'][$key];
    if ($field['field_type'] == 'link') {
      $value = l($value, $value, $ext_link_options);
    } 
    $items[] = array(
      '#label' => $field['name'],
      '#value' => $value,
      '#theme' => 'intel_profile_value',
    );
  }
  $elem = array(
    '#type' => 'intel_profile_fieldset',
    '#title' => t('Subscriber details'),
    '#items' => $items,
    '#theme' => 'intel_profile_fieldset',
  );
  $vars['main']['details'] = $elem;
  
  if (!empty($member['geo']['latitude'])) {
    $loc = array(
      'lat' => $member['geo']['latitude'],
      'lon' => $member['geo']['longitude'],
    );
    $mvars = array(
      'locations' => array($loc),
    );
    $content = theme('intel_location_map', $mvars);
    /*
    $content .= '<div class="value">';
    $content .= $vdata['location']['city'] . ', ' . $vdata['location']['region'] . "<br />";
    $content .= $vdata['location']['country'];
    if ($vdata['location']['metro'] != '(not set)') {
      $content .= '<br><br>';
      $content .= $vdata['location']['metro'];
    }
    $content .= '</div>';
    */
    $elem = array(
      '#title' => t('Location'),
      '#content' => $content,
      '#theme' => 'intel_profile_block'
    );
    $vars['sidebar']['location'] = $elem;
  }
  
  $output = theme('intel_visitor_profile', $vars);

  return $output;
}