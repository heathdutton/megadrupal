<?php
require_once 'EMSWebService.php';

class Experian_Cheetahmail_Helper_Data extends Experian_Cheetahmail_Helper {
  /**
   * Check if the module is activated
   *
   * @return boolean
   */
  public function is_activated() {
    $is_activated = variable_get('experian/cheetahmail/isactivated');
    return !empty($is_activated);
  }
  
  /**
   * Delete specified config by $path.
   */
  protected function deleteConfig($path, $scope = 'default', $scopeId = 0, $flush_cache = TRUE) {
    variable_del($path);
  }

  /**
   * Self test - check that all required items are created in EMS
   */
  public function self_test($show_success_msg = TRUE, $recheck_in_EMS = FALSE) {
    if (!$this->is_activated()) {
      drupal_set_message(t("Please, configure Module settings at first."), $type= 'error');
      return FALSE;
    }
    $subscriberId = $this->_get_default_subscriberId();
    if (empty($subscriberId)) {
      drupal_set_message(t("Default subscriber is not set."), $type= 'error');
      return FALSE;
    }

    $configId = $this->_get_default_configId();
    if (empty($configId)) {
      drupal_set_message(t("ConfigId is not set. Can't create it."), $type = 'error');
      return FALSE;
    }

    $domain_id = $this->ems_configs_GetDomain($configId);
    if (empty($domain_id)) {
      drupal_set_message(t("DomainId is not set for the configuration."), $type = 'error');
      return FALSE;
    }

    $filterId  = $this->_get_filter_id();
    if (empty($filterId)) {
      drupal_set_message(t("Filter is not set for the configuration."), $type = 'error');
      return FALSE;
    }

    $campaignId = $this->_get_campaignId();
    if (empty($campaignId)) {
      drupal_set_message(t("CampaignId is not set for the configuration."), $type = 'error');
      return FALSE;
    }
    
    $campaignId2 = $this->_get_campaignId(2);
    if (empty($campaignId2)) {
      drupal_set_message(t("CampaignId2 is not set for the configuration."), $type = 'error');
      return FALSE;
    }       
    
    $templateid = $this->_get_subscribing_emailTemplate();
    if (empty($templateid)) {
      drupal_set_message(t("Subscribing Template is not set."), $type = 'error');
      return FALSE;
    }
    $utemplateid = $this->_get_unsubscribing_emailTemplate();
    if (empty($utemplateid)) {
      drupal_set_message(t("Unsubscribing Template is not set."), $type = 'error');
      return FALSE;
    }

    $is_campaign_finished = $this->_is_campaign_finished($campaignId);
    if (empty($is_campaign_finished)) {
      drupal_set_message(t("The EmailCampaign for Drupal Integration hasn't finished yet. Please, recheck in a minute."), $type = 'warning', $repeat = FALSE);
      //$this->ems_campaigns_Start($campaignId);
      return FALSE;
    }

    $chronoid = $this->_get_subscribing_Chrono();
    if (empty($chronoid)) {
      drupal_set_message(t("The Subcribing ChronoContact is not set."), $type = 'error');
      return FALSE;
    }
    $uchronoid = $this->_get_unsubscribing_Chrono();
    if (empty($uchronoid)) {
      drupal_set_message(t("The Unsubscribing ChronoContact is not set."), $type = 'error');
      return FALSE;
    }

    if (!empty($recheck_in_EMS)) {
      // Check if exist ChronoContact(s) in EMST.
      $result = $this->ems_chronocontact_getChrono($chronoid);
      if (empty($result)) {
          $this->deleteConfig('experian/cheetahmail/campaign/chronoid', $scope = 'default', $scopeId = 0);
          drupal_set_message(t("The Subcribing ChronoContact doesn't exist in EMS. Reinitializing."), $type = 'error');
      }
      $result = $this->ems_chronocontact_getChrono($uchronoid);
      if (empty($result)) {
          $this->deleteConfig('experian/cheetahmail/campaign/uchronoid', $scope = 'default', $scopeId = 0);
          drupal_set_message(t("The UnSubcribing ChronoContact doesn't exist in EMS. Reinitializing."), $type = 'error');
      }

      // Check if exist Campaign in EMST.
      $result = $this->ems_campaigns_GetCampaign($campaignId);
      if (empty($result)) {
        $this->deleteConfig('experian/cheetahmail/campaign/campaignid', $scope = 'default', $scopeId = 0, $flush_cache = FALSE);
        drupal_set_message(t("CampaignId doesn't exist in EMS. Reinitializing."), $type = 'error');
        // also need to delete
        $this->deleteConfig('experian/cheetahmail/campaign/chronoid', $scope = 'default', $scopeId = 0, $flush_cache = FALSE);
        $this->deleteConfig('experian/cheetahmail/campaign/uchronoid', $scope = 'default', $scopeId = 0, $flush_cache = TRUE);
      }

      // Check if exist Template(s) in EMST.
      $result = $this->ems_chronocontact_GetTemplate($templateid);
      if (empty($result)) {
        $this->deleteConfig('experian/cheetahmail/campaign/templateid', $scope = 'default', $scopeId = 0, $flush_cache = FALSE);
        drupal_set_message(t("Subscribing Template doesn't exist in EMS. Reinitializing."), $type = 'error');
        // also need to delete
        $this->deleteConfig('experian/cheetahmail/campaign/chronoid', $scope = 'default', $scopeId = 0, $flush_cache = TRUE);
      }
      $result = $this->ems_chronocontact_GetTemplate($utemplateid);
      if (empty($result)) {
        $this->deleteConfig('experian/cheetahmail/campaign/utemplateid', $scope = 'default', $scopeId = 0, $flush_cache = FALSE);
        drupal_set_message(t("Unsubscribing Template doesn't exist in EMS. Reinitializing."), $type = 'error');
        // also need to delete
        $this->deleteConfig('experian/cheetahmail/campaign/uchronoid', $scope = 'default', $scopeId = 0, $flush_cache = TRUE);
      }

      // Check if exist Config in EMST.
      $result = $this->ems_configs_GetConfig($configId);
      if (empty($result)) {
        $this->deleteConfig('experian/cheetahmail/campaign/configid', $scope = 'default', $scopeId = 0, $flush_cache = FALSE);
        drupal_set_message(t("ConfigId doesn't exist in EMS. Reinitializing."), $type = 'error');
        // also need to delete
        $this->deleteConfig('experian/cheetahmail/campaign/chronoid', $scope = 'default', $scopeId = 0, $flush_cache = FALSE);
        $this->deleteConfig('experian/cheetahmail/campaign/uchronoid', $scope = 'default', $scopeId = 0, $flush_cache = FALSE);
        $this->deleteConfig('experian/cheetahmail/campaign/campaignid', $scope = 'default', $scopeId = 0, $flush_cache = TRUE);
      }

      // Check if exist Filter in EMST.
      $result = $this->ems_filters_GetFields($filterId);
      if (empty($result)) {
        $this->deleteConfig('experian/cheetahmail/campaign/filterid', $scope = 'default', $scopeId = 0);
        drupal_set_message(t("Filter doesn't exist in EMS. Reinitializing."), $type = 'error');
      }

      //Check if default subscriberId exists
      $this->deleteConfig('experian/cheetahmail/campaign/defaultsubscriberid', $scope = 'default', $scopeId = 0);
      $subscriberId = $this->_get_default_subscriberId();

      return TRUE;
    }

    if (!empty($show_success_msg)) {
      drupal_set_message(t("Self TEST completed."), $type = 'status');
    }
    return TRUE;
  }

  /**
   * Get the EMS API connection params.
   *
   * @return array
   *    Connection param as assosiated array of key->value pairs.
   */
  public function _get_soap_configs() {
    return cheetahmail_api_get_soap_configs();
  }

  /**
   * Get the default subscriberId.
   *
   * @return integer subscriberId
   */
  public function _get_default_subscriberId() {
    $subscriberId = variable_get('experian/cheetahmail/campaign/defaultsubscriberid');
    if (!empty($subscriberId)) {
      return $subscriberId;
    }

    $subscriberId = $this->ems_subscribers_FindByEmail(EMS_CHEETAHMAIL_DEFAULT_SUBSCRIBER);

    if(empty($subscriberId) || (EMS_CHEETAHMAIL_UNREGISTERED == $subscriberId)) {
      $subscriberId = $this->ems_subscribers_Add(EMS_CHEETAHMAIL_DEFAULT_SUBSCRIBER);
    }

    variable_set('experian/cheetahmail/campaign/defaultsubscriberid', $subscriberId);
    return $subscriberId;
  }

  /**
   * Get an email configuration from EMS.
   *
   * @return array
   *    Connection param as assosiated array of key->value pairs.
   */
  public function _get_campaign_configs() {
    $configId = $this->_get_default_configId();

    $configs = (array)$this->ems_configs_GetConfig($configId);
    $listUnsubscribe = $this->_get_default_listUnsubscribe($configId);

    // Subscribe / Unsubscribe Email content
    $configs['subscribeSubject'] = variable_get('experian/cheetahmail/campaign/subscribe_subject');
    $configs['unsubscribeSubject'] = variable_get('experian/cheetahmail/campaign/unsubscribe_subject');
    $configs['subscribeEmailhtml'] = variable_get('experian/cheetahmail/campaign/subscribe_email_html');
    $configs['subscribeEmailtxt'] = variable_get('experian/cheetahmail/campaign/subscribe_email_txt');
    $configs['unsubscribeEmailhtml'] = variable_get('experian/cheetahmail/campaign/unsubscribe_email_html');
    $configs['unsubscribeEmailtxt'] = variable_get('experian/cheetahmail/campaign/unsubscribe_email_txt');

    return array_merge($configs, $listUnsubscribe);
  }

  /**
   * Get the configuration id
   */
  public function _get_default_configId() {
    $configId = variable_get('experian/cheetahmail/campaign/configid');
    if (empty($configId)) {
      $configId = $this->_create_default_campaign_configs();
    }
    return $configId;
  }

  /**
   * Track all links in the $str.
   * All the params that can contains HTML link or TXT link must be tracked with the bodymanager API,
   * it must be applicated to the configurations params and the campaign content param.
   */
  public function trackBody($str='', $is_html = TRUE) {
    return $this->ems_bodymanager_TrackBody($str, $is_html);
  }

  /**
   * Create a default email configuration in EMS.
   */
  public function _create_default_campaign_configs() {
    $config_id = $this->create_campaign_configs();
    variable_set('experian/cheetahmail/campaign/configid', $config_id);
    return $config_id;    
  }

  /**
   * Create an email configuration in EMS.
   */
  public function create_campaign_configs($configs = array()) {
 
        $domains = $this->ems_configs_ListDomain();
        $count_domain = count($domains);

        if($count_domain > 1)
        {
                $list_id = array_keys($domains);
                $domain_id = $list_id[$count_domain-1];
        }
        else
        {
                $domain_id = key($domains);
        }


    $_default_configs = array(
      'idMlist'   => variable_get('experian/cheetahmail/api/idmlist'),
      'idConf'    => -1,
      'name'      => 'Drupal Module',
      'mailFrom'  => 'Experian Cheetahmail',
      'mailFromAddr'  => 'contact',
      'mailRetNpai'   => 'contact@yourcompany.com',
      'mailReply'     => 'contact@yourcompany.com',
      'mailTo'        => 'contact@yourcompany.com',
      'mailDep'       => 'RET',
      'htmlUnsubs'    => '',
      'txtUnsubs'     => '',
      'htmlFooter'    => '<br /> To unsubscribe, please click <a href="' . url('cheetahmail/index/unsubscribe', array( 'absolute' => TRUE)) . '?email=$U(1)' . '" target="_blank">here</a>.<br />',
      'txtFooter'     => '
Unsubscribe :  ' . url('cheetahmail/index/unsubscribe', array( 'absolute' => TRUE)) . '?email=$U(1)',
      'htmlHeader'    => '<div align="center"><font face="arial" size="1">Si ce message ne s\'affiche pas correctement, <a href="$H(1)" target="_blank">cliquez ici</a></font></div>',
      'txtHeader'     => 'Pour visualiser cet email en Html, cliquez ici :
$H(1)',
      'customHeaderKey' => '',
      'customHeaderValue' => '',
      'verpSend'      => 'Complet',//?Complet or Sauf_ReplyTo or ReturnPath_Seulement or Pas_de_traitement
      'charset'       => 'UTF_8',
      'miroir' => 1,
      'isDkim' => 1
    );
    
    $configs = array_merge($_default_configs, $configs);

    $configs['htmlFooter'] = $this->trackBody($configs['htmlFooter'], TRUE);
    $configs['txtFooter'] = $this->trackBody($configs['txtFooter'], FALSE);
    $configs['htmlHeader'] = $this->trackBody($configs['htmlHeader'], TRUE);
    $configs['txtHeader'] = $this->trackBody($configs['txtHeader'], FALSE);

    $configId = $this->ems_configs_Create($configs);
    if (empty($configId) || !is_numeric($configId)) {
      return FALSE;
    }

    // Set Config's ListUnsubscribe.
    $listUnsubscribe = $this->_get_default_listUnsubscribe($configId, TRUE);

    // Set Config's Domain.
    $this->ems_configs_SetDomain($configId, $domain_id);

    return $configId;
  }

  /**
   * Check whether the campaign has status 'TERMINE'.
   */
  public function _is_campaign_finished($campaign_id) {
    if (!is_numeric($campaign_id)) {
      return FALSE;
    }

    $record = db_select('cheetahmail_campaigns', 'cc_list')
      ->fields('cc_list', array('campaign_id', 'status', 'is_finished'))
      ->condition('campaign_id', $campaign_id, '=')
      ->execute()
      ->fetchAssoc();

    if (!empty($record['status']) && ($record['status'] == EMS_CHEETAHMAIL_CAMPAIGN_TERMINE_STATE)) {
      return TRUE;
    }

    $is_finished = variable_get('experian/cheetahmail/campaign/is_finished_' . $campaign_id);
    if (!empty($is_finished)) {
      return $is_finished;
    }

    $campaign = $this->ems_campaigns_GetCampaign($campaign_id);

    // Important Note! Use $campaign->sents->SentDetails->state (not $campaign->state);
    if (!isset($campaign->sents->SentDetails->state)) {
      // Status is UNKNOWN.
      return FALSE;
    }
    
    // Update status in local DB.
    $record = array('campaign_id' => $campaign_id, 'is_finished' => 1, 'status' => $campaign->sents->SentDetails->state);
    if(!drupal_write_record('cheetahmail_campaigns', $record, 'campaign_id')) {
      watchdog('cheetahmail_api', "Can't update the campaign status: (%details) in DB.", array('%details' => print_r($record, TRUE)), WATCHDOG_ERROR, $link = NULL);
    }

    if (EMS_CHEETAHMAIL_CAMPAIGN_TERMINE_STATE == $campaign->sents->SentDetails->state) {
      // @TODO . remove variables for status they are deprecated.
      //variable_set('experian/cheetahmail/campaign/is_finished_' . $campaign_id, TRUE);

      return TRUE;
    }
    return FALSE;
  }

  /**
   * Get "readonly" listUnsubscribe params.
   */
  public function _get_default_listUnsubscribe($configId, $generate = FALSE) {
    if (empty($generate)) {
      $listUnsubscribe = $this->ems_configs_GetListUnsubscribe($configId);

      return array(
        'idconf' => $configId,
        'link' => $listUnsubscribe->link,
        'isEnable' => TRUE,
      );
    }

    $listUnsubscribe = array(
        'idconf' => $configId,
        'link' => url("cheetahmail/index/unsubscribe", array('absolute' => TRUE)),
        'isEnable' => TRUE,
      );

    $result = $this->ems_configs_SetListunsubscribe($listUnsubscribe);

    return $listUnsubscribe;
  }

  /**
   * Get Campaign Params cached on Drupal site.
   */
  public function _get_ems_params() {
    return array(
        'configId' => variable_get('experian/cheetahmail/campaign/configid'),
        'IdMlist'  => variable_get('experian/cheetahmail/api/idmlist'),
        'filterId' => variable_get('experian/cheetahmail/campaign/filterid'),
        'templateid' => variable_get('experian/cheetahmail/campaign/templateid'),
        'utemplateid' => variable_get('experian/cheetahmail/campaign/utemplateid'),
        'campaignid' => variable_get('experian/cheetahmail/campaign/campaignid'),
        'ucampaignid' => variable_get('experian/cheetahmail/campaign/ucampaignid'),
        'chronoid' => variable_get('experian/cheetahmail/campaign/chronoid'),
        'uchronoid' => variable_get('experian/cheetahmail/campaign/uchronoid'),
        'htmlsource' => variable_get('experian/cheetahmail/campaign/htmlsource'),
      );
  }

  /**
   * 1.3.1 Create target Filter with 
   * Filters all user to one target EMS_CHEETAHMAIL_DEFAULT_SUBSCRIBER.
   */
  public function _get_filter_id() {
    $filterid = variable_get('experian/cheetahmail/campaign/filterid');
    if (!empty($filterid)) {
      return $filterid;
    }

    $filterid = $this->ems_filters_Create();
    if (!empty($filterid)) {
      $filterFields = array('IdField' => EMS_CHEETAHMAIL_SUBSCRIBER_IDUSER_FIELD_ID, 'Operation' => 'EQUAL', 'Value' => EMS_CHEETAHMAIL_DEFAULT_SUBSCRIBER);
      $result = $this->ems_filters_SetFields($filterid, $filterFields);
      if (empty($result)) {
        return FALSE;
      }
    }
    variable_set('experian/cheetahmail/campaign/filterid', $filterid);
    return $filterid;
  }
  
  /**
   * 2.x.x Create target Filter
   * Filters all users by specified $filter_fields:
   * <Criterion>
   *       <IdField>int</IdField>
   *       <Operation>NONE or FILLED or NOTFILLED or EQUAL or DIFFERENT or INFERIOR or SUPERIOR or BETWEEN or AGO or IN</Operation>
   *       <Value>string</Value>
   * </Criterion>.
   *
   * @return integer
   *   ID of the created filter.
   */
  public function create_filter($filter_fields = array()) {
    $filterid = $this->ems_filters_Create();
    
    if (empty($filterid)) {
      return FALSE;
    }
    
    if (empty($filter_fields)) {
      return $filterid;
    }
    
    // $filter_fields = array('IdField' => EMS_CHEETAHMAIL_SUBSCRIBER_IDUSER_FIELD_ID, 'Operation' => 'EQUAL', 'Value' => EMS_CHEETAHMAIL_DEFAULT_SUBSCRIBER);
    $result = $this->ems_filters_SetFields($filterid, $filter_fields);
    if (empty($result)) {
      return FALSE;
    }

    return $filterid;
  }

  /**
   * 1.3.3 Create Tracking HTML code + 1.3.4 Create an Email template
   */
  public function _get_subscribing_emailTemplate() {
    $templateid = variable_get('experian/cheetahmail/campaign/templateid');
    if (!empty($templateid)) {
      return $templateid;
    }

    $subject = t("Subcription");
    $emailHtmlBody = t('Thank you for subscribing, <a href="!url">%Site</a>', array('!url' => url('<front>', array('absolute' => TRUE)), '%Site' => variable_get('site_name')));
    $emailTxtBody = t('Thank you for subscribing, !url', array('!url' => url('<front>', array('absolute' => TRUE))));

    $tracked_emailHtmlBody = $this->ems_bodymanager_TrackBody($emailHtmlBody, TRUE);
    $tracked_emailTxtBody = $this->ems_bodymanager_TrackBody($emailTxtBody, FALSE);

    $templateid = $this->ems_chronocontact_CreateTemplate($templateName = variable_get('site_name') . " Subscribing Mail", $tracked_emailHtmlBody, $tracked_emailTxtBody, $subject);
    variable_set('experian/cheetahmail/campaign/templateid', $templateid);

    variable_set('experian/cheetahmail/campaign/subscribe_subject', $subject);
    variable_set('experian/cheetahmail/campaign/subscribe_email_html', $tracked_emailHtmlBody);
    variable_set('experian/cheetahmail/campaign/subscribe_email_txt', $tracked_emailTxtBody);
    variable_set('experian/cheetahmail/campaign/subscribe_email_subject', $subject);
    return $templateid;
  }

  /**
   * Modify Subscription Email content
   */
  public function _set_subscribing_emailTemplate($configs) {
    $emailHtmlBody = $configs['subscribeEmailhtml'];
    $emailTxtBody = $configs['subscribeEmailtxt'];

    $subject = $configs['subscribeSubject']; 
    $tracked_emailHtmlBody = $this->ems_bodymanager_TrackBody($emailHtmlBody, TRUE);
    $tracked_emailTxtBody = $this->ems_bodymanager_TrackBody($emailTxtBody, FALSE);

    $templateid = variable_get('experian/cheetahmail/campaign/templateid');
    if (empty($templateid)) {
      $templateid = $this->ems_chronocontact_CreateTemplate($templateName = variable_get('site_name') . " Subscribing Mail", $tracked_emailHtmlBody, $tracked_emailTxtBody, $subject);
      variable_set('experian/cheetahmail/campaign/templateid', $templateid);
    }
    else
    {
      $this->ems_chronocontact_ModifyTemplate($templateid, $templateName = variable_get('site_name') . " Subscribing Mail", $tracked_emailHtmlBody, $tracked_emailTxtBody, $subject);
    }

    variable_set('experian/cheetahmail/campaign/subscribe_subject', $subject);
    variable_set('experian/cheetahmail/campaign/subscribe_email_html', $tracked_emailHtmlBody);
    variable_set('experian/cheetahmail/campaign/subscribe_email_txt', $tracked_emailTxtBody);
    return $templateid;
  }

  /**
   * 1.3.3 Create Tracking HTML code + 1.3.4 Create an Email template
   */
  public function _get_unsubscribing_emailTemplate() {
    $utemplateid = variable_get('experian/cheetahmail/campaign/utemplateid');
    if (!empty($utemplateid)) {
      return $utemplateid;
    }

    $subject = t("Unsubcription");
    $emailHtmlBody = t('You have succesfully unsubscribed, <a href="!url">%Site</a>', array('!url' => url('<front>', array('absolute' => TRUE)), '%Site' => variable_get('site_name')));
    $emailTxtBody = t('You have succesfully unsubscribed, !url',  array('!url' => url('<front>', array('absolute' => TRUE))));

    $tracked_emailHtmlBody = $this->ems_bodymanager_TrackBody($emailHtmlBody, TRUE);
    $tracked_emailTxtBody = $this->ems_bodymanager_TrackBody($emailTxtBody, FALSE);

    $utemplateid = $this->ems_chronocontact_CreateTemplate($templateName = variable_get('site_name') . " Unsubscribing Mail", $tracked_emailHtmlBody, $tracked_emailTxtBody, $subject);
    variable_set('experian/cheetahmail/campaign/utemplateid', $utemplateid);

    variable_set('experian/cheetahmail/campaign/unsubscribe_subject', $subject);
    variable_set('experian/cheetahmail/campaign/unsubscribe_email_html', $tracked_emailHtmlBody);
    variable_set('experian/cheetahmail/campaign/unsubscribe_email_txt', $tracked_emailTxtBody);
    variable_set('experian/cheetahmail/campaign/unsubscribe_email_subject', $subject);
    return $utemplateid;
  }

  /**
   * Modify UnSubscription Email content
   */
  public function _set_unsubscribing_emailTemplate($configs) {
    $emailHtmlBody = $configs['unsubscribeEmailhtml'];
    $emailTxtBody = $configs['unsubscribeEmailtxt'];

    $subject = $configs['unsubscribeSubject'];
    $tracked_emailHtmlBody = $this->ems_bodymanager_TrackBody($emailHtmlBody, TRUE);
    $tracked_emailTxtBody = $this->ems_bodymanager_TrackBody($emailTxtBody, FALSE);

    $utemplateid = variable_get('experian/cheetahmail/campaign/utemplateid');
    if (empty($utemplateid)) {
      $utemplateid = $this->ems_chronocontact_CreateTemplate($templateName = variable_get('site_name') . " Unsubscribing Mail", $tracked_emailHtmlBody, $tracked_emailTxtBody, $subject);
      variable_set('experian/cheetahmail/campaign/utemplateid', $utemplateid);
    }
    else {
      $this->ems_chronocontact_ModifyTemplate($utemplateid, $templateName = variable_get('site_name') . " Unsubscribing Mail", $tracked_emailHtmlBody, $tracked_emailTxtBody, $subject);
    }

    variable_set('experian/cheetahmail/campaign/unsubscribe_subject', $subject);
    variable_set('experian/cheetahmail/campaign/unsubscribe_email_html', $tracked_emailHtmlBody);
    variable_set('experian/cheetahmail/campaign/unsubscribe_email_txt', $tracked_emailTxtBody);
    return $utemplateid;
  }

  /**
   * 1.3.5 Create EmailCampaign (a default Drupal Campaign)
   */
  public function _get_campaignId($nb = 1) {

	$variable_drupal = 'experian/cheetahmail/campaign/campaignid';
	if($nb <> 1)
	{
		$variable_drupal = 'experian/cheetahmail/campaign/campaignid2';
	}
  
    $campaignid = variable_get($variable_drupal);
    if (!empty($campaignid)) {
      return $campaignid;
    }

    $campaignid = $this->ems_campaigns_Create($description = 'Drupal EMS Campaign (' . url('<front>', array('absolute' => TRUE)) . ')',
      $filter_id = $this->_get_filter_id(),
      $config_id = $this->_get_default_configId(),
      $messageParams = array(
          'subject' => 'Drupal Integration',
          'format' => 1,
          'priority' => 3,
          'htmlSrc' => t('The EmailCampaign for Drupal integration created from site <a href="!url" title="Store">Drupal</a>.', array('!url' => url('<front>', array('absolute' => TRUE)))),
          'txtSrc' => t('The EmailCampaign for Drupal integration created from site !url .', array('!url' => url('<front>', array('absolute' => TRUE)))),
          )
      );

    /*if (!empty($campaignid)) {
      $this->ems_campaigns_Start($campaignid);
    }*/
    variable_set($variable_drupal, $campaignid);
    return $campaignid;
  }

  /**
   * _get_subscribing_Chrono()
   */
  public function _get_subscribing_Chrono() {
    $chronoid = variable_get('experian/cheetahmail/campaign/chronoid');
    if (!empty($chronoid)) {
      return $chronoid;
    }

    $chronoid = $this->ems_chronocontact_CreateChrono($ChronoName = 'Drupal Subscribing Chrono', $IdCampagne = $this->_get_campaignId(), $IdTemplate = $this->_get_subscribing_emailTemplate());

    variable_set('experian/cheetahmail/campaign/chronoid', $chronoid);
    return $chronoid;
  }

  /**
   * _get_unsubscribing_Chrono()
   */
  public function _get_unsubscribing_Chrono() {
    $uchronoid = variable_get('experian/cheetahmail/campaign/uchronoid');
    if (!empty($uchronoid)) {
        return $uchronoid;
    }

    $uchronoid = $this->ems_chronocontact_CreateChrono($ChronoName = 'Drupal Unsubscribing Chrono', $IdCampagne = $this->_get_campaignId(2), $IdTemplate = $this->_get_unsubscribing_emailTemplate());

    variable_set('experian/cheetahmail/campaign/uchronoid', $uchronoid);
    return $uchronoid;
  }
  
  /**
   * campaigns->Create - 1.3.5 Create EmailCampaign
   */
  public function ems_campaigns_Create($description, $filter_id, $config_id, $messageParams = array('subject' => '', 'format' => 0, 'htmlSrc' => '', 'txtSrc' => '')) {
    $record = array();
    $record['description'] = $description;
    $record['filter_id'] = $filter_id;
    $record['config_id'] = $config_id;
    $record['createdat'] = REQUEST_TIME;
    
    $campaign_id = parent::ems_campaigns_Create($description, $filter_id, $config_id, $messageParams);
    if (empty($campaign_id)) {
      watchdog('cheetahmail_api', "Can't create campaign: (%details) in EMS.", array('%details' => print_r($record, TRUE)), WATCHDOG_ERROR, $link = NULL);
      return $campaign_id;
    }

    $record['campaign_id'] = $campaign_id;
    $record['parent_id'] = $campaign_id;
  
    if (!drupal_write_record('cheetahmail_campaigns', $record)) {
      // watchdog...
      watchdog('cheetahmail_api', "Can't save campaign: (%details) in DB.", array('%details' => print_r($record, TRUE)), WATCHDOG_ERROR, $link = NULL);
    }
    return $campaign_id;
  }

  /**
   * campaigns->Start - 1.3.5 Create EmailCampaign
   */
  public function ems_campaigns_Start($campaign_id) {
    $res = parent::ems_campaigns_Start($campaign_id);
    if (empty($res)) {
      return $res;
    }
    
    $record = array('campaign_id' => $campaign_id, 'is_finished' => 1, 'startedat' => REQUEST_TIME);
    if(!drupal_write_record('cheetahmail_campaigns', $record, 'campaign_id')) {
      watchdog('cheetahmail_api', "Can't update the campaign: (%details) in DB.", array('%details' => print_r($record, TRUE)), WATCHDOG_ERROR, $link = NULL);
    }
    return $res;
  }
  
  /**
   * campaigns->DuplicateCampaign - Methode permettant de dupliquer une campagne
   */
  public function ems_campaigns_DuplicateCampaign($campaign_id) {
    $record = db_select('cheetahmail_campaigns', 'cc_list')
      ->fields('cc_list', array('campaign_id', 'description', 'filter_id', 'config_id'))
      ->condition('campaign_id', $campaign_id, '=')
      ->execute()
      ->fetchAssoc();

    if (empty($record['campaign_id'])) {
      $record['description'] = 'Copy of ' . $campaign_id;
      watchdog('cheetahmail_api', "Can't find the campaign: (%details) in DB.", array('%details' => $campaign_id), WATCHDOG_NOTICE, $link = NULL);
    }
    else {
      $record['description'] .= '(copy)';
    }

    $record['createdat'] = REQUEST_TIME;
    $record['parent_id'] = $campaign_id;

    $duplicated_campaign_id = parent::ems_campaigns_DuplicateCampaign($campaign_id);
    if (empty($duplicated_campaign_id)) {
      watchdog('cheetahmail_api', "Can't duplicate campaign: (%details) in EMS.", array('%details' => print_r($record, TRUE)), WATCHDOG_ERROR, $link = NULL);
      return $duplicated_campaign_id;
    }
    
    $record['campaign_id'] = $duplicated_campaign_id;

    if (!drupal_write_record('cheetahmail_campaigns', $record)) {
      // watchdog...
      watchdog('cheetahmail_api', "Can't save campaign: (%details) in DB.", array('%details' => print_r($record, TRUE)), WATCHDOG_ERROR, $link = NULL);
    }
    
    return $duplicated_campaign_id;
  }
}


class Experian_Cheetahmail_NL_Helper_Data extends Experian_Cheetahmail_Helper {
/*
Finally, there is :
* a single configuration
* a single filter
* a single initial campaign (that is duplicated each time)

Create first the campaign and for each sent do : 
* Campaigns->duplicate
* Campaigns ->UpdateMessage
* Campaigns ->start
*/
  public function __construct(){
  }
  /**
   * Get an email configuration from EMS.
   *
   * @return array
   *    Connection param as assosiated array of key->value pairs.
   */
  public function get_configs() {
    $config_id = $this->_get_default_configId();

    $configs = (array)$this->ems_configs_GetConfig($config_id);
    $listUnsubscribe = $this->_get_default_listUnsubscribe($config_id);

    // Subscribe / Unsubscribe Email content
    /*
    $configs['subscribeSubject'] = variable_get('experian/cheetahmail/campaign/subscribe_subject');
    $configs['unsubscribeSubject'] = variable_get('experian/cheetahmail/campaign/unsubscribe_subject');
    $configs['subscribeEmailhtml'] = variable_get('experian/cheetahmail/campaign/subscribe_email_html');
    $configs['subscribeEmailtxt'] = variable_get('experian/cheetahmail/campaign/subscribe_email_txt');
    $configs['unsubscribeEmailhtml'] = variable_get('experian/cheetahmail/campaign/unsubscribe_email_html');
    $configs['unsubscribeEmailtxt'] = variable_get('experian/cheetahmail/campaign/unsubscribe_email_txt');
    
     <messageParams>
        <subject>string</subject>
        <format>int</format>
        <priority>int</priority>
        <htmlSrc>string</htmlSrc>
        <txtSrc>string</txtSrc>
      </messageParams>
   */
    $configs['messageParams'] = $this->get_message_params();

    return array_merge($configs, $listUnsubscribe);
  }
  /**
   * Set an email configuration from EMS.
   *
   * @return array
   *    Connection param as assosiated array of key->value pairs.
   */
  public function set_configs($configs) {
  }  
}