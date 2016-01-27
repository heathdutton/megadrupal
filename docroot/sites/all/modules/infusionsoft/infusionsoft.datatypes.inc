<?php
/**
 * @file
 */

/**
 * Infusionsoft datatype callback.
 *
 * Describe the service resources to WS Client.
 *
 * @return mixed
 */
function _infusionsoft_datatypes_info() {
  return array(
    'infusionsoft_contact' => array(
      'label' => 'Contact',
      'property info' => array(
        'Address1Type' => array('type' => 'text', 'label' => 'Address1 Type'),
        'Address2Street1' => array('type' => 'text', 'label' => 'Address2 Street1'),
        'Address2Street2' => array('type' => 'text', 'label' => 'Address2 Street2'),
        'Address2Type' => array('type' => 'text', 'label' => 'Address2 Type'),
        'Address3Street1' => array('type' => 'text', 'label' => 'Address3 Street1'),
        'Address3Street2' => array('type' => 'text', 'label' => 'Address3 Street2'),
        'Address3Type' => array('type' => 'text', 'label' => 'Address3 Type'),
        'Anniversary' => array('type' => 'date', 'label' => 'Anniversary'),
        'AssistantName' => array('type' => 'text', 'label' => 'Assistant Name'),
        'AssistantPhone' => array('type' => 'text', 'label' => 'Assistant Phone'),
        'BillingInformation' => array('type' => 'text', 'label' => 'Billing Information'),
        'Birthday' => array('type' => 'date', 'label' => 'Birthday'),
        'City' => array('type' => 'text', 'label' => 'City'),
        'City2' => array('type' => 'text', 'label' => 'City2'),
        'City3' => array('type' => 'text', 'label' => 'City3'),
        'Company' => array('type' => 'text', 'label' => 'Company'),
        'AccountId' => array('type' => 'integer', 'label' => 'AccountId'),
        'CompanyID' => array('type' => 'integer', 'label' => 'Company ID'),
        'ContactNotes' => array('type' => 'text', 'label' => 'Contac tNotes'),
        'ContactType' => array('type' => 'text', 'label' => 'Contact Type'),
        'Country' => array('type' => 'text', 'label' => 'Country'),
        'Country2' => array('type' => 'text', 'label' => 'Country 2'),
        'Country3' => array('type' => 'text', 'label' => 'Country 3'),
        'CreatedBy' => array('type' => 'integer', 'label' => 'Created By'),
        'DateCreated' => array('type' => 'date', 'label' => 'date Created'),
        'Email' => array('type' => 'text', 'label' => 'Email'),
        'EmailAddress2' => array('type' => 'text', 'label' => 'Email Address2'),
        'EmailAddress3' => array('type' => 'text', 'label' => 'Email Address3'),
        'Fax1' => array('type' => 'text', 'label' => 'Fax1'),
        'Fax1Type' => array('type' => 'text', 'label' => 'Fax1 Type'),
        'Fax2' => array('type' => 'text', 'label' => 'Fax2'),
        'Fax2Type' => array('type' => 'text', 'label' => 'Fax2 Type'),
        'FirstName' => array('type' => 'text', 'label' => 'First Name'),
        'Groups' => array('type' => 'text', 'label' => 'Groups'),
        'Id' => array('type' => 'integer', 'label' => 'Id'),
        'JobTitle' => array('type' => 'text', 'label' => 'Job Title'),
        'LastName' => array('type' => 'text', 'label' => 'Last Name'),
        'LastUpdated' => array('type' => 'date', 'label' => 'Last Updated'),
        'LastUpdatedBy' => array('type' => 'integer', 'label' => 'Last Updated By'),
        'Leadsource' => array('type' => 'text', 'label' => 'Leadsource'),
        'LeadSourceId' => array('type' => 'integer', 'label' => 'LeadSource Id'),
        'MiddleName' => array('type' => 'text', 'label' => 'Middle Name'),
        'Nickname' => array('type' => 'text', 'label' => 'Nickname'),
        'OwnerID' => array('type' => 'integer', 'label' => 'Owner ID'),
        'Password' => array('type' => 'text', 'label' => 'Password'),
        'Phone1' => array('type' => 'text', 'label' => 'Phone1'),
        'Phone1Ext' => array('type' => 'text', 'label' => 'Phone1 Ext'),
        'Phone1Type' => array('type' => 'text', 'label' => 'Phone1 Type'),
        'Phone2' => array('type' => 'text', 'label' => 'Phone2'),
        'Phone2Ext' => array('type' => 'text', 'label' => 'Phone2 Ext'),
        'Phone2Type' => array('type' => 'text', 'label' => 'Phone2 Type'),
        'Phone3' => array('type' => 'text', 'label' => 'Phone3'),
        'Phone3Ext' => array('type' => 'text', 'label' => 'Phone3 Ext'),
        'Phone3Type' => array('type' => 'text', 'label' => 'Phone3 Type'),
        'Phone4' => array('type' => 'text', 'label' => 'Phone4'),
        'Phone4Ext' => array('type' => 'text', 'label' => 'Phone4 Ext'),
        'Phone4Type' => array('type' => 'text', 'label' => 'Phone4 Type'),
        'Phone5' => array('type' => 'text', 'label' => 'Phone 5'),
        'Phone5Ext' => array('type' => 'text', 'label' => 'Phone5 Ext'),
        'Phone5Type' => array('type' => 'text', 'label' => 'Phone5 Type'),
        'PostalCode' => array('type' => 'text', 'label' => 'Postal Code'),
        'PostalCode2' => array('type' => 'text', 'label' => 'Postal Code2'),
        'PostalCode3' => array('type' => 'text', 'label' => 'Postal Code3'),
        'ReferralCode' => array('type' => 'text', 'label' => 'Referral Code'),
        'SpouseName' => array('type' => 'text', 'label' => 'Spouse Name'),
        'State' => array('type' => 'text', 'label' => 'State'),
        'State2' => array('type' => 'text', 'label' => 'State2'),
        'State3' => array('type' => 'text', 'label' => 'State3'),
        'StreetAddress1' => array('type' => 'text', 'label' => 'Street Address1'),
        'StreetAddress2' => array('type' => 'text', 'label' => 'Street Address2'),
        'Suffix' => array('type' => 'text', 'label' => 'Suffix'),
        'Title' => array('type' => 'text', 'label' => 'Title'),
        'Username' => array('type' => 'text', 'label' => 'Username'),
        'Validated' => array('type' => 'text', 'label' => 'Validated'),
        'Website' => array('type' => 'text', 'label' => 'Website'),
        'ZipFour1' => array('type' => 'text', 'label' => 'ZipFour1'),
        'ZipFour2' => array('type' => 'text', 'label' => 'ZipFour2'),
        'ZipFour3' => array('type' => 'text', 'label' => 'ZipFour3'),
      ),
    ),
    'infusionsoft_group' => array(
      'label' => 'Group',
      'property info' => array(
        'Id' => array('type' => 'integer', 'label' => 'Id'),
        'GroupName' => array('type' => 'text', 'label' => 'Group Name'),
        'GroupCategoryId' => array('type' => 'integer', 'label' => 'Group Category Id'),
        'GroupDescription' => array('type' => 'text', 'label' => 'Group Description'),
      ),
    ),
    'infusionsoft_campaign' => array(
      'label' => 'Campaign',
      'property info' => array(
        'Id' => array('type' => 'integer', 'label' => 'Id'),
        'Name' => array('type' => 'text', 'label' => 'Campaign Name'),
        'Status' => array('type' => 'text', 'label' => 'Status'),
      ),
    ),
    'infusionsoft_contact_action' => array(
      'label' => 'Contact Action',
      'property info' => array(
        'Id' => array('type' => 'integer', 'label' => 'Id'),
        'ContactId' => array('type' => 'integer', 'label' => 'Contact Id'),
        'OpportunityId' => array('type' => 'integer', 'label' => 'Opportunity Id'),
        'ActionType' => array('type' => 'text', 'label' => 'Action Type'),
        'ActionDescription' => array('type' => 'text', 'label' => 'Action Description'),
        'CreationDate' => array('type' => 'date', 'label' => 'Creation Date'),
        'CreationNotes' => array('type' => 'text', 'label' => 'Creation Notes'),
        'CompletionDate' => array('type' => 'date', 'label' => 'Completion Date'),
        'ActionDate' => array('type' => 'date', 'label' => 'Action Date'),
        'EndDate' => array('type' => 'date', 'label' => 'End Date'),
        'PopupDate' => array('type' => 'date', 'label' => 'Popup Date'),
        'UserID' => array('type' => 'integer', 'label' => 'User ID'),
        'Accepted' => array('type' => 'integer', 'label' => 'Accepted'),
        'CreatedBy' => array('type' => 'integer', 'label' => 'Created By'),
        'LastUpdated' => array('type' => 'date', 'label' => 'Last Updated'),
        'LastUpdatedBy' => array('type' => 'integer', 'label' => 'Last Updated By'),
        'Priority' => array('type' => 'integer', 'label' => 'Priority'),
        'IsAppointment' => array('type' => 'integer', 'label' => 'Is Appointment'),
      ),
    ),
    'infusionsoft_lead' => array(
      'label' => 'Lead',
      'property info' => array(
        'Id' => array('type' => 'integer', 'label' => 'Id'),
        'OpportunityTitle' => array('type' => 'text', 'label' => 'Opportunity Title'),
        'ContactID' => array('type' => 'integer', 'label' => 'Contact ID'),
        'AffiliateId' => array('type' => 'integer', 'label' => 'Affiliate Id'),
        'UserID' => array('type' => 'integer', 'label' => 'User ID'),
        'StageID' => array('type' => 'integer', 'label' => 'Stage ID'),
        'StatusID' => array('type' => 'integer', 'label' => 'Status ID'),
        'Leadsource' => array('type' => 'text', 'label' => 'Leadsource'),
        'Objection' => array('type' => 'text', 'label' => 'Objection'),
        'ProjectedRevenueLow' => array('type' => 'float', 'label' => 'Projected Revenue Low'),
        'ProjectedRevenueHigh' => array('type' => 'float', 'label' => 'Projected Revenue High'),
        'OpportunityNotes' => array('type' => 'text', 'label' => 'Opportunity Notes'),
        'DateCreated' => array('type' => 'date', 'label' => 'Date Created'),
        'LastUpdated' => array('type' => 'date', 'label' => 'Last Updated'),
        'LastUpdatedBy' => array('type' => 'integer', 'label' => 'Last Updated By'),
        'CreatedBy' => array('type' => 'integer', 'label' => 'Created By'),
        'EstimatedCloseDate' => array('type' => 'date', 'label' => 'Estimated Close Date'),
        'NextActionDate' => array('type' => 'date', 'label' => 'Next Action Date'),
        'NextActionNotes' => array('type' => 'text', 'label' => 'Next Action Notes'),
      ),
    ),
  );
}

/**
 * Register Methods
 *
 * @param type $service
 */
function _infusionsoft_register_methods(&$service) {
  $operation = array();
  $operation['label'] = 'Add a Contact';
  $operation['url'] = 'ContactService.add';
  $operation['parameter']['data'] = array(
    'type' => 'infusionsoft_contact',
    'label' => 'Contact',
    'description' => 'The array of data for the new contact.',
  );
  $service->operations['ContactService.add'] = $operation;

  $operation = array();
  $operation['label'] = 'Update a Contact';
  $operation['url'] = 'ContactService.update';
  $operation['parameter']['contactId'] = array('type' => 'integer', 'label' => 'Contact ID');
  $operation['parameter']['data'] = array(
    'type' => 'infusionsoft_contact',
    'label' => 'Contact',
    'description' => 'The array of data for the updated contact.',
  );
  $service->operations['ContactService.update'] = $operation;

  $operation = array();
  $operation['label'] = 'Load a Contact';
  $operation['url'] = 'ContactService.load';
  $operation['parameter']['contactId'] = array('type' => 'integer', 'label' => 'Contact ID');
  $operation['parameter']['selectedFields'] = array('type' => 'list<text>', 'label' => 'Fields to Fetch');
  $operation['result'] = array('type' => 'infusionsoft_contact', 'label' => 'Infusionsoft Contact');
  $service->operations['ContactService.load'] = $operation;

  $operation = array();
  $operation['label'] = 'Get Opt In/Out Status of a User to Email Service';
  $operation['url'] = 'APIEmailService.getOptStatus';
  $operation['parameter']['email'] = array('type' => 'string', 'label' => 'Email');
  $operation['result'] = array('type' => 'integer', 'label' => 'Infusionsoft Opt Status Result');
  $service->operations['APIEmailService.getOptStatus'] = $operation;

  $operation = array();
  $operation['label'] = 'Opt In a User to Email Service';
  $operation['url'] = 'APIEmailService.optIn';
  $operation['parameter']['email'] = array('type' => 'string', 'label' => 'Email');
  $operation['parameter']['optInReason'] = array('type' => 'string', 'label' => 'Opt In Reason');
  $operation['result'] = array('type' => 'boolean', 'label' => 'True or False');
  $service->operations['APIEmailService.optIn'] = $operation;

  $operation = array();
  $operation['label'] = 'Opt Out a User from Email Service';
  $operation['url'] = 'APIEmailService.optOut';
  $operation['parameter']['email'] = array('type' => 'string', 'label' => 'Email');
  $operation['parameter']['optOutReason'] = array('type' => 'string', 'label' => 'Opt Out Reason');
  $operation['result'] = array('type' => 'boolean', 'label' => 'True or False');
  $service->operations['APIEmailService.optOut'] = $operation;

  $operation = array();
  $operation['label'] = 'Find a Contact by Email';
  $operation['url'] = 'ContactService.findByEmail';
  $operation['parameter']['email'] = array('type' => 'string', 'label' => 'Email');
  $operation['parameter']['selectedFields'] = array('type' => 'list<text>', 'label' => 'Fields to Fetch');
  $operation['result'] = array('type' => 'infusionsoft_contact', 'label' => 'Infusionsoft Contact');
  $service->operations['ContactService.findByEmail'] = $operation;

  $operation = array();
  $operation['label'] = 'Query';
  $operation['url'] = 'DataService.query';
  $operation['parameter']['table'] = array('type' => 'text', 'label' => 'Table');
  $operation['parameter']['limit'] = array('type' => 'integer', 'label' => 'Count');
  $operation['parameter']['page'] = array('type' => 'integer', 'label' => 'Pager');
  $operation['parameter']['queryData'] = array('type' => 'string', 'label' => 'Query String');
  $operation['parameter']['selectedFields'] = array('type' => 'list<text>', 'label' => 'Fields to Fetch');
  $service->operations['DataService.query'] = $operation;

  $operation = array();
  $operation['label'] = 'Add to Campaign';
  $operation['url'] = 'ContactService.addToCampaign';
  $operation['parameter']['contactId'] = array('type' => 'integer', 'label' => 'Contact ID');
  $operation['parameter']['campaignId'] = array('type' => 'integer', 'label' => 'Campaign ID');
  $service->operations['ContactService.addToCampaign'] = $operation;

  $operation = array();
  $operation['label'] = 'Remove from Campaign';
  $operation['url'] = 'ContactService.removeFromCampaign';
  $operation['parameter']['contactId'] = array('type' => 'integer', 'label' => 'Contact ID');
  $operation['parameter']['followUpSequenceId'] = array('type' => 'integer', 'label' => 'Campaign ID');
  $service->operations['ContactService.removeFromCampaign'] = $operation;

  $operation = array();
  $operation['label'] = 'Add to Group';
  $operation['url'] = 'ContactService.addToGroup';
  $operation['parameter']['contactId'] = array('type' => 'integer', 'label' => 'Contact ID');
  $operation['parameter']['campaignId'] = array('type' => 'integer', 'label' => 'Group ID');
  $service->operations['ContactService.addToGroup'] = $operation;

  $operation = array();
  $operation['label'] = 'Remove from Group';
  $operation['url'] = 'ContactService.removeFromGroup';
  $operation['parameter']['contactId'] = array('type' => 'integer', 'label' => 'Contact ID');
  $operation['parameter']['tagId'] = array('type' => 'integer', 'label' => 'Group ID');
  $service->operations['ContactService.removeFromGroup'] = $operation;

  $operation = array();
  $operation['label'] = 'Add Data';
  $operation['url'] = 'DataService.add';
  $operation['parameter']['table'] = array('type' => 'text', 'label' => 'Table Name');
  $operation['parameter']['values'] = array('type' => 'list<text>', 'label' => 'Field Values');
  $service->operations['DataService.add'] = $operation;

  $operation = array();
  $operation['label'] = 'Delete Data';
  $operation['url'] = 'DataService.delete';
  $operation['parameter']['table'] = array('type' => 'text', 'label' => 'Table Name');
  $operation['parameter']['Id'] = array('type' => 'integer', 'label' => 'Rercord ID');
  $service->operations['DataService.delete'] = $operation;
}
