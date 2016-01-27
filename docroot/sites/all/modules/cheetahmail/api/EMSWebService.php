<?php
/**
 * @file
 * Cheetahmail class EMSWebService
 */

define('EMS_CHEETAHMAIL_SUBSCRIBER_IDUSER_FIELD_ID', '1');
define('EMS_CHEETAHMAIL_SUBSCRIBER_STATUS_FIELD_ID', '4');
define('EMS_CHEETAHMAIL_UNREGISTERED', -1);
define('EMS_CHEETAHMAIL_SUBSCRIBED', 0);
define('EMS_CHEETAHMAIL_UNSUBSCRIBED', 1);
define('EMS_CHEETAHMAIL_DEBUG_MODE', FALSE);
define('EMS_CHEETAHMAIL_CAMPAIGN_TERMINE_STATE', 'TERMINE');
define('EMS_CHEETAHMAIL_CAMPAIGN_WISHDATE_DEFAULT', '0001-01-01T00:00:00');
define('EMS_CHEETAHMAIL_DEFAULT_SUBSCRIBER', 'support@experian-cheetahmail.fr');

/**
 * Universal EMS API webservice wrapper
 */
class EMSWebService {
  protected $webservice_name;
  public $_client;
  protected $_result = NULL;

  /**
   * EMSWebService constructor initialize SOAP Client.
   */
  public function __construct($webservice_name, $configs = array()) {
    if (empty($webservice_name)) {
      throw new Exception('EMSWebService Undefined $webservice_name');
    }
    $this->webservice_name = $webservice_name;
    $this->_client = $this->_get_soap_client($this->webservice_name, $configs);
  }

  /**
   * Get SOAP EMS API Client.
   * Initialize client and prepare SoapHeaders.
   *
   * @param string $webservice_name
   *   Name of the SOAP webservice.
   * @param array $configs
   *   Optional EMS API Connection params
   *   - used in SoapHeader for identification.
   */
  public function _get_soap_client($webservice_name = 'subscribers', $configs = array()) {
    try {
      $_defaults = cheetahmail_api_get_soap_configs();

      $configs = array_merge($_defaults, $configs);

      $context = stream_context_create(array(
          'ssl' => array('verify_peer' => FALSE, 'allow_self_signed' => TRUE),
      ));
      $options = array();
      if (EMS_CHEETAHMAIL_DEBUG_MODE) {
        $options = array('trace' => TRUE);
      }

      // , array('stream_context' => $context);
      $client = new SoapClient('https://ws1.ems6.net/' . $webservice_name . '.asmx?WSDL', $options);

      if (empty($client) || !is_object($client)) {
        throw new Exception("Can't create the webservice SoapClient:" . $webservice_name);
      }

      // Namespace of the WS. !very important!
      $ns = 'http://ws.ems6.net/';

      // Body of the Soap Header.
      $headerbody = $configs;

      // Create Soap Header.
      $header = new SOAPHeader($ns, 'AuthHeader', $headerbody);

      // Set the Headers of Soap Client.
      $client->__setSoapHeaders($header);

      return $client;
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), $type = 'error');
    }
  }

  /**
   * Process request to SOAP API
   */
  public function __call($name, $arguments) {
    try {
      if (empty($this->_client) || !is_object($this->_client)) {
        throw new Exception("Can't create the webservice SoapClient:" . $this->webservice_name);
      }
      $this->_result = $this->_client->__soapCall($name, $arguments);

      if (EMS_CHEETAHMAIL_DEBUG_MODE) {
        watchdog('cheetahmail_api', 'SOAPRequest Details  %webservice->%method : `%details`', 
          array(
            '%webservice' => $this->webservice_name,
            '%method' => $name,
            '%details' => $this->_client->__getLastRequest()
          ),
          WATCHDOG_DEBUG
        );
      }

      $resultNodeName = $name . 'Result';

      if (EMS_CHEETAHMAIL_DEBUG_MODE) {
        watchdog('cheetahmail_api', 'Result %webservice->%method : `%details`',
          array(
            '%webservice' => $this->webservice_name,
            '%method' => $name,
            '%details' => print_r($this->_result, TRUE)
          ),
          WATCHDOG_DEBUG
        );
      }
      
      if(!isset($this->_result->$resultNodeName)) {
        return NULL;
      }
      return $this->_result->$resultNodeName;
    }
    catch (Exception $e) {
      drupal_set_message(t('Request to EMS API failed.'), $type = 'error');
      if (EMS_CHEETAHMAIL_DEBUG_MODE) {
        drupal_set_message(t('Error Details %webservice->%method : `%details`', array(
          '%webservice' => $this->webservice_name,
          '%method' => $name,
          '%details' => $e->getMessage())), $type = 'error');
      }
      else {
        watchdog('cheetahmail_api', 'Error Details %webservice->%method : `%details`', array(
          '%webservice' => $this->webservice_name,
          '%method' => $name,
          '%details' => $e->getMessage(),
          ), WATCHDOG_ERROR);
      }
      return FALSE;
    }
    return FALSE;
  }

  /**
   * Return SOAP responce.
   */
  public function getSOAPResult() {
    return $this->_result;
  }
}


class Experian_Cheetahmail_Helper {
  /**
   * subscribers->Find() - get subscribers id by $email
   */
  public function ems_subscribers_FindByEmail($email) {
    $client = new EMSWebService('subscribers');
    $result = $client->Find(array('criteria' => array(array('1', $email))));
    if (!isset($result->int)) {
      return NULL;
    }
    return $result->int;
  }

  /**
   * subscribers->Add() - Subscribe new email
   */
  public function ems_subscribers_Add($email) {
    $client = new EMSWebService('subscribers');
    $result = $client->Add(array('email' => $email));
    return $result;
  }

  /**
   * subscribers->Get() - Retrieves a subscriber's information by $subscriberId
   */
  public function ems_subscribers_Get($subscriberId) {
    $client = new EMSWebService('subscribers');
    $result = $client->Get(array('subscriberId' => $subscriberId));
    return $result;
  }

  /**
   * subscribers->Update() - Updates a subscriber's information
   */
  public function ems_subscribers_Update($subscriberId = '', $data = array()) {
    if (empty($data)) {
      return;
    }
    $client = new EMSWebService('subscribers');
    $result = $client->Update(array('subscriberId' => $subscriberId, 'data' => $data));
    return $result;
  }

  /**
   * subscribers->Unsubscribe() - Subscribe new email
   */
  public function ems_subscribers_Unsubscribe($subscriberId) {
    $client = new EMSWebService('subscribers');
    $result = $client->Unsubscribe(array('subscriberId' => $subscriberId));
    return $result;
  }

  /**
   * CREATE EMAIL CAMPAIGN and CHRONOMAIL
   */
  /**
   * filers->create() - 1.3.1 Create target Filter
   */
  public function ems_filters_Create() {
    $client = new EMSWebService('filters');
    $result = $client->Create(array('isPrivate' => TRUE, 'description' => 'Target All Subscribers'));
    return $result;
  }

  /**
   * filers->SetFields() - Methode pour definir les criteres du filtre
   */
  public function ems_filters_SetFields($filterId, $Criterion) {
    $client = new EMSWebService('filters');
    $result = $client->SetFields(array('queryId' => $filterId, 'criteria' => array('Criterion' => $Criterion)));
    return $result;
  }

  /**
   * filers->GetFields() - Methode permettant de selectionner les criteres du filtre
   */
  public function ems_filters_GetFields($filterId) {
    $client = new EMSWebService('filters');
    $result = $client->GetFields(array('queryId' => $filterId));
    return $result;
  }

  /**
   * confgis->Create() - 1.3.2 Create/set Configs
   *<_config>
   *   <idMlist>int</idMlist>
   *   <idConf>int</idConf>
   *   <name>string</name>
   *   <mailFrom>string</mailFrom>
   *   <mailFromAddr>string</mailFromAddr>
   *   <mailRetNpai>string</mailRetNpai>
   *   <mailReply>string</mailReply>
   *   <mailTo>string</mailTo>
   *   <mailDep>string</mailDep>
   *   <htmlUnsubs>string</htmlUnsubs>
   *   <txtUnsubs>string</txtUnsubs>
   *   <htmlFooter>string</htmlFooter>
   *   <txtFooter>string</txtFooter>
   *   <htmlHeader>string</htmlHeader>
   *   <txtHeader>string</txtHeader>
   *   <customHeaderKey>string</customHeaderKey>
   *   <customHeaderValue>string</customHeaderValue>
   *   <verpSend>Complet or Sauf_ReplyTo or ReturnPath_Seulement or Pas_de_traitement</verpSend>
   *   <charset>ISO_2022_jp or ISO_2022_kr or ISO_8859_1 or ISO_8859_2 or ISO_8859_3 or ISO_8859_4 or ISO_8859_5 or ISO_8859_6 or ISO_8859_7 or ISO_8859_8 or ISO_ir_58 or ISO_8859_11 or UTF_8</charset>
   *   <miroir>boolean</miroir>
   * </_config>
   */
  public function ems_configs_Create($configs = array()) {
    $client = new EMSWebService('configs');
    $result = $client->Create(array('_config' => $configs));
    return $result;
  }

  /**
   * confgis->UpdateConfig() - Updates an email configurations
   */
  public function ems_configs_UpdateConfig($configs = array()) {
	$domains = $this->ems_configs_ListDomain();
	$count_domain = count($domains);
	
	if($count_domain > 1)
	{
		rsort($domains);
	} 
	
	
	
    $client = new EMSWebService('configs');
    $_default_configs = array(
            'idMlist'   => variable_get('experian/cheetahmail/api/idmlist'),
            'idConf'    => NULL,
            'name'      => 'Drupal Module',
            'mailFrom'  => 'Experian Cheetahmail',
            'mailFromAddr'  => 'contact@' . array_shift($domains),
            'mailRetNpai'   => 'contact@yourcompany.com',
            'mailReply'     => 'contact@yourcompany.com',
            'mailTo'        => 'contact@yourcompany.com',
            'mailDep'       => 'RET',
            'htmlUnsubs'    => '',
            'txtUnsubs'     => '',
            'htmlFooter'    => '<div align="center"><font face="arial" size="1">Message envoye avec<br><a href="$H(9)" target="_blank"><img src="http://web.emailingsolution.com/ems/signature/ems.gif" border="0"></a><br></div>$H(0)',
            'txtFooter'     => '---------------------------------------------------------------
Message envoye avec Experian Cheetahmail  $h(9) $H(0)',
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

    $result = $client->UpdateConfig(array('_config' => $configs));
    return $result;
  }

  /**
   * confgis->GetConfig() - 1.3.2 Get an email configuration
   */
  public function ems_configs_GetConfig($configId) {
    $client = new EMSWebService('configs');
    $result = $client->GetConfig(array('_IdConf' => $configId));
    return $result;
  }

  /**
   * confgis->List() - 1.3.2 Retrieves a list of email configurations
   */
  public function ems_configs_List($configId) {
    $client = new EMSWebService('configs');
    $result = $client->List();
    return $result;
  }

  /**
   * confgis->Delete() - Deletes an email configurations
   */
  public function ems_configs_Delete($configId) {
    $client = new EMSWebService('configs');
    $result = $client->Delete(array('_IdConf' => $configId));
    return $result;
  }

  /**
   * confgis->ListDomain() - 1.3.2 Create/set Configs
   */
    public function ems_configs_ListDomain()
    {
        static $domains = array();
        if (!empty($domains))
        {
            return $domains;
        }

        $client = new EMSWebService('configs');
        $result = $client->ListDomain();

        // //@deprecated//
        // $result = (array)$result;
        // foreach ($result as $domain)
        // {
        //     $domains[$domain->idDomain] = $domain->DomainName;
        // }
        // //end @deprecated//

        if(count($result->Domain) == 1){
            $domain = $result->Domain;
            $domains[$domain->idDomain] = $domain->DomainName;
        }
        else{
            //$nb_domaine = count($result->Domain);
            //$i = 1;
            foreach ($result->Domain as $domain) {
        	   //on ne garde que le dernier domaine
        	   //if($i == $nb_domaine)
        	   //{
        	       $domains[$domain->idDomain] = $domain->DomainName;	   
        	   //}
        	   //$i++;
        	}
        }

        return $domains;
    } 

  /**
   * confgis->SetListunsubscribe() - 1.3.2 SetListUnsubscribe
   */
  public function ems_configs_SetListunsubscribe($data = array()) {
    $data['link'] = $this->trackBody($data['link'], FALSE);

    $client = new EMSWebService('configs');
    $result = $client->SetListunsubscribe($data);
    return $result;
  }

  /**
   * confgis->GetListUnsubscribe() - 1.3.2 GetListUnsubscribe
   */
  public function ems_configs_GetListUnsubscribe($configId) {
    $client = new EMSWebService('configs');
    $result = $client->GetListUnsubscribe(array('idconf' => $configId));
    return $result;
  }

  /**
   * configs->SetDomain().
   */
  public function ems_configs_SetDomain($configId, $domain_id) {
    $client = new EMSWebService('configs');
    $result = $client->SetDomain(array('idConf' => $configId, 'idDomain' => $domain_id));
    return $result;
  }

  /**
   * configs->GetDomain().
   */
  public function ems_configs_GetDomain($configId) {
    $client = new EMSWebService('configs');
    $result = $client->GetDomain(array('idconf' => $configId));
    return $result;
  }

  /**
   * bodymanager->TrackBody() - 1.3.3 Create Tracking HTML code
   */
  public function ems_bodymanager_TrackBody($emailBody, $isHtml = TRUE) {
    $client = new EMSWebService('bodymanager');
    $result = $client->TrackBody(array('_strBody' => $emailBody, '_firstCategory' => '', '_secondCategory' => '', '_bisHtml' => $isHtml));
    return $result;
  }

  /**
   * campaigns->Create - 1.3.5 Create EmailCampaign
   */
  public function ems_campaigns_Create($description, $filter_id, $config_id, $messageParams = array('subject' => '', 'format' => 0, 'priority' => 3, 'htmlSrc' => '', 'txtSrc' => '')) {
    $client = new EMSWebService('campaigns');
    
    $campaign_data = array(
      'campaignParams' => array(
            'isPrivate' => FALSE,
            'description' => $description,
            'filters' => array('fieldFilterId' => $filter_id, 'behavioralFilterId' => '', 'sqlQueryFilterId' => '', 'targetId' => ''), 'folderId' => 0),
      'sentParams' => array('speed' => 'SPEED_MAX', 'configId' => $config_id, 'wishdate' => EMS_CHEETAHMAIL_CAMPAIGN_WISHDATE_DEFAULT),
      'messageParams' => $messageParams);

    $result = $client->Create($campaign_data);

    return $result;
  }

  /**
   * campaigns->Start - 1.3.5 Create EmailCampaign
   */
  public function ems_campaigns_Start($campaignId) {
    $client = new EMSWebService('campaigns');
    $result = $client->Start(array('campaignId' => $campaignId));
    return $result;
  }

  /**
   * campaigns->GetCampaign - 1.3.5 Create EmailCampaign
   */
  public function ems_campaigns_GetCampaign($campaignId) {
    $client = new EMSWebService('campaigns');
    $result = $client->GetCampaign(array('campaignId' => $campaignId));
    return $result;
  }

  /**
   * campaigns->Delete - Methode pour supprimer une campagne email
   */
  public function ems_campaigns_Delete($campaignId) {
    $client = new EMSWebService('campaigns');
    $result = $client->Delete(array('campaignId' => $campaignId));
    return $result;
  }
  
  /**
   * campaigns->Delete - Methode pour supprimer une campagne email
   */
  public function ems_campaigns_CloseCampaign($campaignId) {
    $client = new EMSWebService('campaigns');
    $result = $client->CloseCampaign(array('campaignId' => $campaignId));
    return $result;
  }

  /**
   * campaigns->UpdateMessage - Methode pour modifier le message d'une campagne email
   */
  public function ems_campaigns_UpdateMessage($campaignId, $message_params) {
    $client = new EMSWebService('campaigns');
    $result = $client->UpdateMessage(array('campaignId' => $campaignId, 'parameters' => $message_params));
    return $result;
  }
  
  /**
   * campaigns->DuplicateCampaign - Methode permettant de dupliquer une campagne
   */
  public function ems_campaigns_DuplicateCampaign($campaignId) {
    $client = new EMSWebService('campaigns');
    $result = $client->DuplicateCampaign(array('campaignId' => $campaignId));
    return $result;
  }

  /**
   * chronocontact->CreateTemplate - 1.3.4 Create an Email template
   */
  public function ems_chronocontact_CreateTemplate($templateName, $sourceHTML, $sourceTXT, $subject = '') {
    $client = new EMSWebService('chronocontact');
    $result = $client->CreateTemplate(array('TemplateName' => $templateName, 'SourceHTML' => $sourceHTML, 'SourceTXT' => $sourceTXT, 'subject' => $subject));
    return $result;
  }

  /**
   * chronocontact->ModifyTemplate - Methode permettant de modifier un template en fonction de son id, renvoi true si la modification s'est bien effectuee
   */
  public function ems_chronocontact_ModifyTemplate($templateId, $templateName, $sourceHTML, $sourceTXT, $subject = '') {
    $client = new EMSWebService('chronocontact');
    $result = $client->ModifyTemplate(array('IdTemplate' => $templateId, 'TemplateName' => $templateName, 'SourceHTML' => $sourceHTML, 'SourceTXT' => $sourceTXT, 'subject' => $subject));
    return $result;
  }

  /**
   * chronocontact->CreateChrono() - 1.3.6 Create ChronoContact
   */
  public function ems_chronocontact_CreateChrono($ChronoName, $IdCampagne, $IdTemplate) {
    $client = new EMSWebService('chronocontact');
    $result = $client->CreateChrono(array('ChronoName' => $ChronoName, 'IdCampagne' => $IdCampagne, 'IdTemplate' => $IdTemplate));
    return $result;
  }

  /**
   * chronocontact->getChrono() - Methode permettant d'obtenir un Chrono
   */
  public function ems_chronocontact_getChrono($chronoId) {
    $client = new EMSWebService('chronocontact');
    $result = $client->getChrono(array('IdChrono' => $chronoId));
    return $result;
  }

  /**
   * chronocontact->SendMail() - 1.3.7 Send Mail
   */
  public function ems_chronocontact_SendMail($chronoId, $userId) {
    $client = new EMSWebService('chronocontact');
    $result = $client->SendMail(array('chronoId' => $chronoId, 'userId' => $userId, 'deleteAttachementFile' => ''));
    return $result;
  }

  /**
   * chronocontact->DeleteTemplate() - Methode permettant de supprimer un template mail
   */
  public function ems_chronocontact_DeleteTemplate($templateId) {
    $client = new EMSWebService('chronocontact');
    $result = $client->DeleteTemplate(array('templateId' => $templateId));
    return $result;
  }

  /**
   * chronocontact->GetTemplate() - Methode permettant de recuperer un template mail
   */
  public function ems_chronocontact_GetTemplate($templateId) {
    $client = new EMSWebService('chronocontact');
    $result = $client->GetTemplate(array('templateId' => $templateId));
    return $result;
  }
}
