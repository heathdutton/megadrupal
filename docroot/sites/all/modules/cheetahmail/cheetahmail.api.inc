<?php
/**
 * @file
 * Cheetahmail API func
 */

require_once 'api/Data.php';

/**
 * Get the EMS API connection params.
 *
 * @return array
 *   Connection param as assosiated array of key->value pairs.
 */
function cheetahmail_api_get_soap_configs() {
  return array(
    'IdMlist'   => variable_get('experian/cheetahmail/api/idmlist', ''),
    'UserName'  => variable_get('experian/cheetahmail/api/username', ''),
    'Password'  => variable_get('experian/cheetahmail/api/password', ''),
  );
}


/**
 * Delete all the module Configs stored in Drupal Variables
 * and delete EMS items.
 */
function cheetahmail_api_delete_all_configs() {
  variable_del('experian/cheetahmail/isactivated');

  cheetahmail_api_delete_emst_elements();

  variable_del('experian/cheetahmail/api/idmlist');
  variable_del('experian/cheetahmail/api/username');
  variable_del('experian/cheetahmail/api/password');

  variable_del('experian/cheetahmail/campaign/templateid');
  variable_del('experian/cheetahmail/campaign/utemplateid');
  variable_del('experian/cheetahmail/campaign/campaignid');
  variable_del('experian/cheetahmail/campaign/chronoid');
  variable_del('experian/cheetahmail/campaign/uchronoid');
  variable_del('experian/cheetahmail/campaign/configid');
  variable_del('experian/cheetahmail/campaign/filterid');
  variable_del('experian/cheetahmail/campaign/defaultsubscriberid');
  variable_del('experian/cheetahmail/campaign/subscribe_email_html');
  variable_del('experian/cheetahmail/campaign/subscribe_email_txt');
  variable_del('experian/cheetahmail/campaign/unsubscribe_email_html');
  variable_del('experian/cheetahmail/campaign/unsubscribe_email_txt');

  return TRUE;
}

/**
 * Delete EMST elements (configurations, templates, campaigns)
 */
function cheetahmail_api_delete_emst_elements() {
  // Remove ChronoContact(s).
  // @NOTE: No methods to delete Chronocontact directly.
  // Remove Template(s).
  $templateid = variable_get('experian/cheetahmail/campaign/templateid');
  if (!empty($templateid)) {
      cheetahmail_api_get_helper()->ems_chronocontact_DeleteTemplate($templateid);
  }

  $utemplateid = variable_get('experian/cheetahmail/campaign/utemplateid');
  if (!empty($utemplateid)) {
      cheetahmail_api_get_helper()->ems_chronocontact_DeleteTemplate($utemplateid);
  }
  // Remove Campaign.
  // ! Impossible to remove a campaign already sent or programmed:
  // at EMSConnectEngine.CampaignEngine.Delete(Int32 campaignId).
  $campaignid = variable_get('experian/cheetahmail/campaign/campaignid');
  if (!empty($campaignid)) {
    // ! Impossible to remove a campaign already sent or programmed.
    // at EMSConnectEngine.CampaignEngine.Delete(Int32 campaignId).
    // cheetahmail_api_get_helper()->ems_campaigns_Delete($campaignid);
  }
  // Remove Config.
  $config_id = variable_get('experian/cheetahmail/campaign/configid');

  if (!empty($config_id)) {
    cheetahmail_api_get_helper()->ems_configs_Delete($config_id);
  }
  // Remove Filter.
  // @NOTE: No methods to delete Filter directly.
}

/**
 * Get EMSAPIHelper.
 * @return object
 *   instance of EMSAPIHelper class
 */
function cheetahmail_api_get_helper() {
  static $ems_api_helper;

  if (!empty($ems_api_helper)) {
    return $ems_api_helper;
  }

  $ems_api_helper = new Experian_Cheetahmail_Helper_Data();
  return $ems_api_helper;
}

/**
 * Check if the module is activated
 *
 * @return boolean
 *   TRUE if module is activated and API connection settings are configured
 */
function cheetahmail_api_is_activated() {
  $is_activated = variable_get('experian/cheetahmail/isactivated');
  return !empty($is_activated);
}

/**
 * Helper for Campaign configuration form.
 */
function cheetahmail_api_get_campaign_configuration_form($configs) {
  $form = array();
  
  $form['setup_of_sending'] = array(
    '#type' => 'fieldset',
    '#title' => t('Set-up of sending'),
    '#collapsible' => TRUE,
  );

  $domains = cheetahmail_api_get_helper()->ems_configs_ListDomain();
  $domain = cheetahmail_api_get_helper()->ems_configs_GetDomain($configs['idConf']);
  $configs['domain'] = empty($domain->idDomain) ? key($domains) : $domain->idDomain;

  $form['setup_of_sending']['domain'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Sending domain'),
    '#options' => $domains,
    '#default_value' => $configs['domain'],
  );

  $form['setup_of_sending']['sender'] = array(
    '#type' => 'markup',
    '#markup' => t('Sender'),
  );

  $form['setup_of_sending']['mailFrom'] = array(
    '#type' => 'textfield',
    '#title' => t('Source address(posting)'),
    '#required' => TRUE,
    '#default_value' => $configs['mailFrom'],
  );

  $form['setup_of_sending']['mailFromAddr'] = array(
    '#type' => 'textfield',
    '#title' => t('Email source'),
    '#required' => TRUE,
    '#default_value' => $configs['mailFromAddr'],
    '#size' => 20,
  );
  $form['setup_of_sending']['mailFromAddrupdater'] = array(
    '#type' => 'markup',
    '#markup' => "<script>
      if(jQuery('#mailFromDomain').size() < 1) {
        jQuery('#edit-mailfromaddr').after('<span id=\"mailFromDomain\">@</span>');
      }
      jQuery('#edit-domain').change(function(){
        jQuery('#mailFromDomain').text('@' + jQuery('#edit-domain option:selected').text());
      }).change();
      </script>",
  );
  $form['setup_of_sending']['addresses'] = array(
    '#type' => 'markup',
    '#markup' => t('Addresses handling bounces'),
  );

  $form['setup_of_sending']['mailReply'] = array(
    '#type' => 'textfield',
    '#title' => t('Answer Email'),
    '#required' => TRUE,
    '#default_value' => $configs['mailReply'],
  );

  $form['setup_of_sending']['mailRetNpai'] = array(
    '#type' => 'textfield',
    '#title' => t('Email storing return bounces'),
    '#required' => TRUE,
    '#default_value' => $configs['mailRetNpai'],
  );

  $form['setup_of_sending']['unsubscribe'] = array(
    '#type' => 'markup',
    '#markup' => t('List Unsubscribe'),
  );

  $form['setup_of_sending']['link'] = array(
    '#type' => 'textfield',
    '#title' => t('Unsubscribe Link'),
    '#disabled' => TRUE,
    '#attributes' => array('readonly' => 'readonly'),
    '#default_value' => $configs['link'],
  );

  $form['setup_of_sending']['isEnable'] = array(
    '#type' =>'checkbox',
    '#title' => t('Activate List Unsubscribe'),
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );

  $form['message_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  /*$form['message_options']['Header'] = array(
    '#type' => 'markup',
    '#markup' => '<h3>' . t('Header') . '</h3><hr/>',
  );*/

  $form['message_options']['htmlHeader'] = array(
    '#type' => 'textarea',
    '#title' => t('Header HTML'),
    '#default_value' => $configs['htmlHeader'],
  );

  $form['message_options']['txtHeader'] = array(
    '#type' => 'textarea',
    '#title' => t('Header TEXT'),
    '#default_value' => $configs['txtHeader'],
  );

  /*$form['message_options']['Footer'] = array(
    '#type' => 'markup',
    '#markup' => '<h3>' . t('Footer') . '</h3><hr/>',
  );*/

  $form['message_options']['htmlFooter'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer HTML'),
    '#default_value' => $configs['htmlFooter'],
  );

  $form['message_options']['txtFooter'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer TEXT'),
    '#default_value' => $configs['txtFooter'],
  );

  // Message Params.
  $form['messageParams'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Campaign Message'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if (empty($configs['messageParams'])) {
    $configs['messageParams'] = array(
      'subject' => '',
      'format' => 1,
      'priority' => 3,
      'htmlSrc' => '',
      'txtSrc' => '',
    );
  }

  $form['messageParams']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Subject'),
    '#default_value' => $configs['messageParams']['subject'],
    '#required' => TRUE,
  );

  $form['messageParams']['format'] = array(
    '#type' => 'value',
    '#default_value' => $configs['messageParams']['format'],
  );

  $form['messageParams']['priority'] = array(
    '#type' => 'value',
    '#value' => $configs['messageParams']['priority'],
  );

  $form['messageParams']['htmlSrc'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Content HTML'),
    '#description' => t('Email Content HTML will be tracked automatically. TXT version is automatically generated based on the HTML version.'),
    '#default_value' => $configs['messageParams']['htmlSrc'],
  );

  $form['messageParams']['txtSrc'] = array(
    '#type' => 'value',
    // '#default_value' => $configs['messageParams']['txtSrc'],
    '#default_value' => '',
  );

  $form_with_vtabs = array();
  $form_with_vtabs['vtabs'] = $form;
  $form_with_vtabs['vtabs']['#type'] = 'vertical_tabs';
  $form = $form_with_vtabs;

  return $form;
}
