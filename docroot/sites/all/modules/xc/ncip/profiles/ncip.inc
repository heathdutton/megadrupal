<?php
/**
 * @file
 * NCIP profile functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Implements hook_ncip_info().
 */
function ncip_ncip_info() {
  $info['ncip'] = array(
    'name' => 'NCIP Z39-83',
    'description' => t('NCIP Z39-83'),
  );
  return $info;
}

/**
 * Implements hook_ncip_services().
 */
function ncip_ncip_services(&$servics) {
  $services['ncip'] = array(
    'AuthenticateUserService' => array(
      'name' => t('Authenticate User Service'),
      'description' => t('This service requests authentication of a User presumed to be known to an Agency. Authentication indicates only that the User is known by an Agency. It does not indicate that the responding application is authorizing services for that User.'),
      'type' => NCIP_LOOKUP_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'AuthenticateUser' => array(
            'name' => t('Authenticate User'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'AuthenticateUserResponse' => array(
            'name' => t('Authenticate User Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'LookupAgencyService' => array(
      'name' => t('Lookup Agency Service'),
      'description' => t('This service requests data about a particular Agency known to the responding application. The initiating application provides the unique Id of the Agency and the list of elements for which data is requested.'),
      'type' => NCIP_LOOKUP_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'LookupAgency' => array(
            'name' => t('Lookup Agency'),
            'data elements' => array(
              'required' => array(
                'AgencyElementType' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueAgencyId'),
                  ),
                ),
                'UniqueAgencyId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'LookupAgencyResponse' => array(
            'name' => t('Lookup Agency Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueAgencyId'),
                  ),
                ),
              ),
              'requested' => array(
                'AgencyAddressInformation' => array(
                  'repeatable' => TRUE,
                ),
                'AgencyUserPrivilegeType' => array(
                  'repeatable' => TRUE,
                ),
                'ApplicationProfileSupportedType' => array(
                  'repeatable' => TRUE,
                ),
                'AuthenticationPrompt' => array(
                  'repeatable' => TRUE,
                ),
                'ConsortiumAgreement' => array(
                  'repeatable' => TRUE,
                ),
                'OrganizationNameInformation' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'LookupItemService' => array(
      'name' => t('Lookup Item Service'),
      'description' => t('This service requests data about a particular Item known to the responding application. The initiator provides the unique Id of the Item and a list of elements for which data is requested.'),
      'type' => NCIP_LOOKUP_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'LookupItem' => array(
            'name' => t('Lookup Item'),
            'data elements' => array(
              'required' => array(
                'ItemElementType' => array(
                  'repeatable' => TRUE,
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId', 'VisibleItemId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId', 'VisibleItemId'),
                  ),
                ),
                'VisibleItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId', 'UniqueRequestId'),
                  ),
                ),
              ),
              'optional' => array(
                'CurrentBorrowerDesired' => array(),
                'CurrentRequestersDesired' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'LookupItemResponse' => array(
            'name' => t('Lookup Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId', 'UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_AND_OR => array('Problem', 'UniqueRequestId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'UniqueItemId'),
                  ),
                ),
              ),
              'requested' => array(
                'DateRecalled' => array(),
                'HoldPickupDate' => array(),
                'ItemOptionalFields' => array(),
                'ItemTransaction' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'LookupUserService' => array(
      'name' => t('Lookup User Service'),
      'description' => t('This service requests data about a particular User known to the responding application. The initiator provides the unique Id of the User and a list of elements for which data is requested.'),
      'type' => NCIP_LOOKUP_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'LookupUser' => array(
            'name' => t('Lookup User'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId', 'VisibleUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput', 'VisibleUserId'),
                  ),
                ),
                'VisibleUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput', 'UniqueUserId'),
                  ),
                ),
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
              ),
              'optional' => array(
                'LoanedItemsDesired' => array(),
                'RequestedItemsDesired' => array(),
                'UserFiscalAccountDesired' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'LookupUserResponse' => array(
            'name' => t('Lookup User Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array(),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'LoanedItemsCount' => array(
                  'repeatable' => TRUE,
                ),
                'RequestedItemsCount' => array(
                  'repeatable' => TRUE,
                ),
                'UserFiscalAccount' => array(
                  'repeatable' => TRUE,
                ),
                'UserOptionalFields' => array(),
                'UserTransaction' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'LookupVersionService' => array(
      'name' => t('Lookup Version Service'),
      'description' => t('This service requests data about the NCIP version(s) supported by the responding application.'),
      'type' => NCIP_LOOKUP_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'LookupVersion' => array(
            'name' => t('Lookup Version'),
            'data elements' => array(
              'required' => array(
                'FromAgencyId' => array(),
                'ToAgencyId' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'LookupVersionResponse' => array(
            'name' => t('Lookup Version Response'),
            'data elements' => array(
              'required' => array(
                'FromAgencyId' => array(),
                'ToAgencyId' => array(),
                'VersionSupported' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'AcceptItemService' => array(
      'name' => t('Accept Item Service'),
      'description' => t('This service requests that the responding application accept an Item to be circulated to a User. The responding application may be a third party that has no prior knowledge of either the User or the Item. The initiation message identifies the action the responding agency is requested to take when it receives the item. The request may include a date by which the initiating application requires the Item to be returned, an indication that the user may apply to renew the loan of the Item, and financial data relating to the supply of the Item. If there is a possibility that the responding application has no prior knowledge of either the User or the Item, the request may optionally include data about the User and/or the Item.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'AcceptItem' => array(
            'name' => t('Accept Item'),
            'data elements' => array(
              'required' => array(
                'UniqueRequestId' => array(),
                'RequestedActionType' => array(),
              ),
              'optional' => array(
                'DateForReturn' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('IndeterminateLoanPeriodFlag', 'NonReturnableFlag'),
                  ),
                ),
                'IndeterminateLoanPeriodFlag' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('DateForReturn', 'NonReturnableFlag'),
                  ),
                ),
                'NonReturnableFlag' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('DateForReturn', 'IndeterminateLoanPeriodFlag'),
                  ),
                ),
                'MandatedAction' => array(),
                'ItemOptionalFields' => array(),
                'FiscalTransactionInformation' => array(),
                'RenewalNotPermitted' => array(),
                'UniqueItemId' => array(),
                'UniqueUserId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'AcceptItemResponse' => array(
            'name' => t('Accept Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'UniqueItemId' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'CheckInItemService' => array(
      'name' => t('Check In Item Service'),
      'description' => t('This service requests that the responding application check in an Item. It also permits the initiating application to request data about the User and/or Item involved with this check in.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'CheckInItem' => array(
            'name' => t('Check In Item'),
            'data elements' => array(
              'required' => array(
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'ItemElementType' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CheckInItemResponse' => array(
            'name' => t('Check In Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
                'RoutingInformation' => array(),
                'UniqueUserId' => array(),
              ),
              'requested' => array(
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'CheckOutItemService' => array(
      'name' => t('Check Out Item Service'),
      'description' => t('This service requests that the responding application check out an Item to a User. It also permits the initiating application to acknowledge the fee amount (if any) associated with the check out. The initiating application may also request data about the User and/or Item involved with this check out.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'CheckOutItem' => array(
            'name' => t('Check Out Item'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'AcknowledgedFeeAmount' => array(),
                'AcknowledgedItemUseRestrictionType' => array(
                  'repeatable' => TRUE,
                ),
                'DesiredDateDue' => array(),
                'ItemElementType' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'ResourceDesired' => array(),
                'ShippingInformation' => array(),
                'UniqueRequestId' => array(),
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CheckOutItemResponse' => array(
            'name' => t('Check Out Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('DateDue', 'NonReturnableFlag', 'IndeterminateLoanPeriodFlag', 'UniqueItemId', 'UniqueUserId'),
                  ),
                ),
                'RequiredFeeAmount' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('DateDue', 'NonReturnableFlag', 'IndeterminateLoanPeriodFlag', 'UniqueItemId', 'UniqueUserId'),
                  ),
                ),
                'RequiredItemUseRestrictionType' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('DateDue', 'NonReturnableFlag', 'IndeterminateLoanPeriodFlag', 'UniqueItemId', 'UniqueUserId'),
                  ),
                ),
                'DateDue' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'NonReturnableFlag', 'IndeterminateLoanPeriodFlag'),
                  ),
                ),
                'NonReturnableFlag' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'DateDue', 'IndeterminateLoanPeriodFlag'),
                  ),
                ),
                'IndeterminateLoanPeriodFlag' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'DateDue', 'NonReturnableFlag'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'UndoCheckOutItemService' => array(
      'name' => t('Undo Check Out Item Service'),
      'description' => t('This service requests that the responding application undo the check out that immediately preceded this message. If the responder agrees to the request, processing continues as if the preceding check out had never occurred. This service is typically used by self-check applications.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UndoCheckOutItem' => array(
            'name' => t('Undo Check Out Item'),
            'data elements' => array(
              'required' => array(
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'MandatedAction' => array(),
                'UniqueRequestId' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'UndoCheckOutItemResponse' => array(
            'name' => t('Undo Check Out Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
                'UniqueUserId' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'CreateAgencyService' => array(
      'name' => t('Create Agency Service'),
      'description' => t('This service requests that the responding application create a record for an agency. The initiating application supplies the data elements to be used to create the Agency. It may optionally supply a proposed Unique Agency Id.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'CreateAgency' => array(
            'name' => t('Create Agency'),
            'data elements' => array(
              'required' => array(
                'OrganizationNameInformation' => array(
                  'repeatable' => TRUE,
                ),
              ),
              'optional' => array(
                'AgencyAddressInformation' => array(
                  'repeatable' => TRUE,
                ),
                'AgencyUserPrivilegeType' => array(
                  'repeatable' => TRUE,
                ),
                'ApplicationProfileSupportedType' => array(
                  'repeatable' => TRUE,
                ),
                'AuthenticationPrompt' => array(
                  'repeatable' => TRUE,
                ),
                'ConsortiumAgreement' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'UniqueAgencyId' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CreateAgencyResponse' => array(
            'name' => t('Create Agency Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueAgencyId'),
                  ),
                ),
                'UniqueAgencyId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'CreateItemService' => array(
      'name' => t('Create Item Service'),
      'description' => t('This service requests that the responding application create a record for an item. The initiating application supplies the data elements to be used to create the Item. It may optionally supply a proposed Unique Item Id.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'CreateItem' => array(
            'name' => t('Create Item'),
            'data elements' => array(
              'required' => array(
                'BibliographicDescription' => array(),
              ),
              'optional' => array(
                'CirculationStatus' => array(),
                'ItemDescription' => array(),
                'ItemUseRestrictionType' => array(),
                'Location' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'PhysicalCondition' => array(),
                'SecurityMarker' => array(),
                'SensitizationFlag' => array(),
                'UniqueItemId' => array(),
                'UniqueRequestId' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CreateItemResponse' => array(
            'name' => t('Create Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'CreateUserService' => array(
      'name' => t('Create User Service'),
      'description' => t('This service requests that the responding application create a record for a user. The initiating application supplies the data elements to be used to create the User. It may optionally supply a proposed Unique User Id.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'CreateUser' => array(
            'name' => t('Create User'),
            'data elements' => array(
              'required' => array(
                'NameInformation' => array(),
              ),
              'optional' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'BlockOrTrap' => array(
                  'repeatable' => TRUE,
                ),
                'DateOfBirth' => array(),
                'MandatedAction' => array(),
                'UserAddressInformation' => array(
                  'repeatable' => TRUE,
                ),
                'UserLanguage' => array(
                  'repeatable' => TRUE,
                ),
                'UserPrivilege' => array(
                  'repeatable' => TRUE,
                ),
                'VisibleUserId' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CreateUserResponse' => array(
            'name' => t('Create User Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'CreateUserFiscalTransactionService' => array(
      'name' => t('Create User Fiscal Transaction Service'),
      'description' => t('This service requests that the responding application create a new fiscal transaction for a service provided to a User. The initiating application provides the type of update and fiscal details about the update it wants done.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'CreateUserFiscalTransaction' => array(
            'name' => t('Create User Fiscal Transaction'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'FiscalTransactionInformation' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CreateUserFiscalTransactionResponse' => array(
            'name' => t('Create User Fiscal Transaction Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('FiscalTransactionReferenceId', 'UniqueUserId'),
                  ),
                ),
                'FiscalTransactionReferenceId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'RecallItemService' => array(
      'name' => t('Recall Item Service'),
      'description' => t('This service requests that the responding application recall an Item from a User. The initiating application may propose a new date due for the Item. The initiating application may also request data about the User and/or Item involved with this recall.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'RecallItem' => array(
            'name' => t('Recall Item'),
            'data elements' => array(
              'required' => array(
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'DesiredDateDue' => array(),
                'ItemElementType' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'ShippingInformation' => array(),
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'RecallItemResponse' => array(
            'name' => t('Recall Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'DateDue' => array(),
                'FiscalTransactionInformation' => array(),
                'UniqueUserId' => array(),
              ),
              'requested' => array(
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'CancelRecallItemService' => array(
      'name' => t('Cancel Recall Item Service'),
      'description' => t('This service requests that the responding application cancel the recall of an Item that it had previously been asked to recall. The initiating application may also request data about the User and/or Item involved with this recall cancellation.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'CancelRecallItem' => array(
            'name' => t('Cancel Recall Item'),
            'data elements' => array(
              'required' => array(
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'ItemElementType' => array(
                  'repeateable' => TRUE,
                ),
                'MandatedAction' => array(),
                'UserElementType' => array(
                  'repeateable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CancelRecallItemResponse' => array(
            'name' => t('Cancel Recall Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
                'UniqueUserId' => array(),
              ),
              'requested' => array(
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'RenewItemService' => array(
      'name' => t('Renew Item Service'),
      'description' => t('This service requests that the responding application renew an Item for a User. The initiating application may include a suggested revision of the due date and an acknowledgement of fee amount. The initiating application may also request data about the User and/or Item involved with this renewal.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'RenewItem' => array(
            'name' => t('Renew Item'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'AcknowledgedFeeAmount' => array(),
                'AcknowledgedItemUseRestrictionType' => array(
                  'repeatable' => TRUE,
                ),
                'DesiredDateDue' => array(),
                'DesiredDateForReturn' => array(),
                'ItemElementType' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'RenewItemResponse' => array(
            'name' => t('Renew Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('RequiredItemUseRestrictionType', 'Pending', 'UniqueItemId'),
                  ),
                ),
                'RequiredFeeAmount' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('RequiredItemUseRestrictionType', 'Pending', 'UniqueItemId'),
                  ),
                ),
                'RequiredItemUseRestrictionType' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'RequiredFeeAmount', 'Pending', 'UniqueItemId'),
                  ),
                ),
                'Pending' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'RequiredFeeAmount', 'RequiredItemUseRestrictionType', 'UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'RequiredFeeAmount', 'RequiredItemUseRestrictionType', 'Pending'),
                  ),
                ),
              ),
              'optional' => array(
                'DateDue' => array(),
                'DateForReturn' => array(),
                'FiscalTransactionInformation' => array(),
                'RenewalCount' => array(),
                'UniqueUserId' => array(),
              ),
              'requested' => array(
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'ReportCirculationStatusChangeService' => array(
      'name' => t('Report Circulation Status Change Service'),
      'description' => t('This service requests that the responding application mark an Item as reported returned, missing, lost, or never borrowed. The initiating application may also request data about the User and/or Item involved with this status change.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ReportCirculationStatusChange' => array(
            'name' => t('Report Circulation Status Change'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'ItemReportedLost' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('ItemReportedMissing', 'ItemReportedNeverBorrowed', 'ItemReportedReturned'),
                  ),
                ),
                'ItemReportedMissing' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('ItemReportedLost', 'ItemReportedNeverBorrowed', 'ItemReportedReturned'),
                  ),
                ),
                'ItemReportedNeverBorrowed' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('ItemReportedLost', 'ItemReportedMissing', 'ItemReportedReturned'),
                  ),
                ),
                'ItemReportedReturned' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('ItemReportedLost', 'ItemReportedMissing', 'ItemReportedNeverBorrowed'),
                  ),
                ),
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'ItemElementType' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ReportCirculationStatusChangeResponse' => array(
            'name' => t('Report Circulation Status Change Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'UniqueUserId' => array(),
              ),
              'requested' => array(
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'RequestItemService' => array(
      'name' => t('Request Item Service'),
      'description' => t('This service requests that the responding application place a request on an Item for a User whether or not the Item is immediately available. The initiating application indicates the type of request being made. The initiating application may optionally provide an acknowledgement of the fee to be charged for the service. The initiating application may also request data about the User and/or Item involved with this request.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'RequestItem' => array(
            'name' => t('Request Item'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'RequestScopeType' => array(),
                'RequestType' => array(),
                'UniqueBibliographicId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueBibliographicId'),
                  ),
                ),
              ),
              'optional' => array(
                'AcknowledgedFeeAmount' => array(),
                'AcknowledgedItemUseRestrictionType' => array(
                  'repeatable' => TRUE,
                ),
                'EarliestDateNeeded' => array(),
                'ItemElementType' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'NeedBeforeDate' => array(),
                'PickupExpiryDate' => array(),
                'ShippingInformation' => array(),
                'UniqueRequestId' => array(),
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'RequestItemResponse' => array(
            'name' => t('Request Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('RequestScopeType', 'RequestType', 'UniqueItemId', 'UniqueRequestId', 'UniqueUserId'),
                  ),
                ),
                'RequiredFeeAmount' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('RequestScopeType', 'RequestType', 'UniqueItemId', 'UniqueRequestId', 'UniqueUserId'),
                  ),
                ),
                'RequiredItemUseRestrictionType' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('RequestScopeType', 'RequestType', 'UniqueItemId', 'UniqueRequestId', 'UniqueUserId'),
                  ),
                ),
                'RequestScopeType' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'RequiredFeeAmount', 'RequiredItemUseRestrictionType'),
                  ),
                ),
                'RequestType' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'RequiredFeeAmount', 'RequiredItemUseRestrictionType'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'RequiredFeeAmount', 'RequiredItemUseRestrictionType'),
                    NCIP_USAGE_AND_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'RequiredFeeAmount', 'RequiredItemUseRestrictionType'),
                    NCIP_USAGE_AND_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem', 'RequiredFeeAmount', 'RequiredItemUseRestrictionType'),
                  ),
                ),
              ),
              'optional' => array(
                'DateAvailable' => array(),
                'HoldPickupDate' => array(),
                'FiscalTransactionInformation' => array(),
                'RequiredItemUseRestrictionType' => array(
                  'repeatable' => TRUE,
                ),
                'ShippingInformation' => array(),
              ),
              'requested' => array(
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'CancelRequestItemService' => array(
      'name' => t('Cancel Request Item Service'),
      'description' => t('This service requests that the responding application cancel a previous request for an Item. The initiating application may also request data about the User and/or Item involved with this request cancellation.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          '' => array(
            'name' => t(''),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'RequestType' => array(),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array(),
                    NCIP_USAGE_AND_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array(),
                    NCIP_USAGE_AND_OR => array('UniqueItemId'),
                  ),
                ),
              ),
              'optional' => array(
                'ItemElementType' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'RequestScopeType' => array(),
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CancelRequestItemResponse' => array(
            'name' => t('Cancel Request Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array(),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                    NCIP_USAGE_AND_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                    NCIP_USAGE_AND_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
              ),
              'requested' => array(
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'SendUserNoticeService' => array(
      'name' => t('Send User Notice Service'),
      'description' => t('This service requests that the responding application send a notice to a user. The initiating application provides the unique Id of the User and the type of notice to send. The initiating application may optionally provide a date on which it desires the notice be sent, and the content of the notice.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'SendUserNotice' => array(
            'name' => t('Send User Notice'),
            'data elements' => array(
              'required' => array(
                'UniqueUserId' => array(),
                'UserNoticeDetails' => array(),
              ),
              'optional' => array(
                'DateToSend' => array(),
                'MandatedAction' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'SendUserNoticeResponse' => array(
            'name' => t('Send User Notice Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'DateSent' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('DateWillSend'),
                  ),
                ),
                'DateWillSend' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('DateSent'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'UpdateAgencyService' => array(
      'name' => t('Update Agency Service'),
      'description' => t('This service requests that the responding application update data about an Agency. The initiating application provides elements to be deleted with the value for each element and/or elements to be added with the value for each element. If the initiating application is changing the value for an existing element it must include that element in both the delete and add sections of the message.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UpdateAgency' => array(
            'name' => t('Update Agency'),
            'data elements' => array(
              'required' => array(
                'UniqueAgencyId' => array(),
              ),
              'optional' => array(
                'AddAgencyFields' => array(),
                'DeleteAgencyFields' => array(),
                'MandatedAction' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          '' => array(
            'name' => t(''),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueAgencyId'),
                  ),
                ),
                'UniqueAgencyId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                '' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array(),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'UpdateCirculationStatusService' => array(
      'name' => t('Update Circulation Status Service'),
      'description' => t('This service requests that the responding application update the circulation status of an Item. The initiating application provides the unique Id of the item Id and the new circulation status.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UpdateCirculationStatus' => array(
            'name' => t('Update Circulation Status'),
            'data elements' => array(
              'required' => array(
                'CirculationStatus' => array(),
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'MandatedAction' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'UpdateCirculationStatusResponse' => array(
            'name' => t('Update Circulation Status Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'UpdateItemService' => array(
      'name' => t('Update Item Service'),
      'description' => t('This service requests that the responding application update data about an Item. The initiating application provides elements to be deleted with the value for each element and/or elements to be added with the value for each element. If the initiating application is changing the value for an existing element it must include that element in both the delete and add sections of the message. This service cannot be used to change the circulation status of an Item.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UpdateItem' => array(
            'name' => t('Update Item'),
            'data elements' => array(
              'required' => array(
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'AddItemFields' => array(),
                'DeleteItemFields' => array(),
                'MandatedAction' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'UpdateItemResponse' => array(
            'name' => t('Update Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'UpdateRequestItemService' => array(
      'name' => t('Update Request Item Service'),
      'description' => t('This service asks that the responding application update data about a request for an Item made by a User. The initiating application provides elements to be deleted with the value for each element and/or elements to be added with the value for each element. If the initiating application is changing the value for an existing element it must include that element in both the delete and add sections of the message.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UpdateRequestItem' => array(
            'name' => t('Update Request Item'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId', 'UniqueRequestId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput', 'UniqueRequestId'),
                  ),
                ),
                'RequestType' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput', 'UniqueUserId', 'RequestType', 'UniqueItemId'),
                  ),
                ),
              ),
              'optional' => array(
                'AddRequestFields' => array(),
                'DeleteRequestFields' => array(),
                'ItemElementType' => array(
                  'repeatable' => TRUE,
                ),
                'MandatedAction' => array(),
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'UpdateRequestItemResponse' => array(
            'name' => t('Update Request Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('RequestScopeType', 'RequestType', 'UniqueItemId', 'UniqueUserId'),
                  ),
                ),
                'RequiredFeeAmount' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('RequestScopeType', 'RequestType', 'UniqueItemId', 'UniqueUserId'),
                    NCIP_USAGE_AND_OR => array('Problem'),
                  ),
                ),
                'RequestScopeType' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
                'RequestType' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
              'optional' => array(
                'DateAvailable' => array(),
                'FiscalTransactionInformation' => array(),
                'HoldPickupDate' => array(),
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'UpdateUserService' => array(
      'name' => t('Update User Service'),
      'description' => t('This service requests that the responding application update data about a User. The initiating application provides elements to be deleted with the value for each element and/or elements to be added with the value for each element. If the initiating application is changing the value for an existing element it must include that element in both the delete and add sections of the message.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UpdateUser' => array(
            'name' => t('Update User'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'repeatable' => TRUE,
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
              ),
              'optional' => array(
                'AddUserFields' => array(),
                'DeleteUserFields' => array(),
                'MandatedAction' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'UpdateUserResponse' => array(
            'name' => t('Update User Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'AgencyCreatedService' => array(
      'name' => t('Agency Created Service'),
      'description' => t('This service informs the responding application that a new Agency has been created. The initiating application provides the Unique Agency Id and details about the Agency.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'AgencyCreated' => array(
            'name' => t('Agency Created'),
            'data elements' => array(
              'required' => array(
                'OrganizationNameInformation' => array(
                  'repeatable' => TRUE,
                ),
                'UniqueAgencyId' => array(),
              ),
              'optional' => array(
                'AgencyAddressInformation' => array(
                  'repeatable' => TRUE,
                ),
                'AgencyUserPrivilegeType' => array(
                  'repeatable' => TRUE,
                ),
                'ApplicationProfileSupportedType' => array(
                  'repeatable' => TRUE,
                ),
                'AuthenticationPrompt' => array(
                  'repeatable' => TRUE,
                ),
                'ConsortiumAgreement' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'AgencyCreatedResponse' => array(
            'name' => t('Agency Created Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'AgencyUpdatedService' => array(
      'name' => t('Agency Updated Service'),
      'description' => t('This service informs the responding application that one or more data elements of an Agency have been updated. The initiating application provides elements that have been deleted with the value for each element and/or elements that have been added with the value for each element. If the value for an existing element has changed, the message must include that element in both the delete and add sections of the message.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'AgencyUpdated' => array(
            'name' => t('Agency Updated'),
            'data elements' => array(
              'required' => array(
                'Unique Agency Id' => array(),
              ),
              'optional' => array(
                'AddAgencyFields' => array(),
                'DeleteAgencyFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'AgencyUpdatedResponse' => array(
            'name' => t('Agency Updated Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'CirculationStatusChangeReportedService' => array(
      'name' => t('Circulation Status Change Reported Service'),
      'description' => t('This service informs the responding application that an Item has been reported as returned, lost, missing, or never borrowed. The initiating application provides the Unique Item Id and Unique User Id, and may optionally provide other details associated with the circulation status change.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'CirculationStatusChangeReported' => array(
            'name' => t('Circulation Status Change Reported'),
            'data elements' => array(
              'required' => array(
                'ItemReportedLost' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('ItemReportedMissing', 'ItemReportedNeverBorrowed', 'ItemReportedReturned'),
                  ),
                ),
                'ItemReportedMissing' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('ItemReportedLost', 'ItemReportedNeverBorrowed', 'ItemReportedReturned'),
                  ),
                ),
                'ItemReportedNeverBorrowed' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('ItemReportedLost', 'ItemReportedMissing', 'ItemReportedReturned'),
                  ),
                ),
                'ItemReportedReturned' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('ItemReportedLost', 'ItemReportedMissing', 'ItemReportedNeverBorrowed'),
                  ),
                ),
                'UniqueItemId' => array(),
                'UniqueUserId' => array(),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CirculationStatusChangeReportedResponse' => array(
            'name' => t('Circulation Status Change Reported Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'CirculationStatusUpdatedService' => array(
      'name' => t('Circulation Status Updated Service'),
      'description' => t('This service informs the responding application that the circulation status of an Item has been updated. The initiating application provides the unique Id of the item Id and the new circulation status. Only the following three values from Circulation Status are valid here: Renew Still Pending, Item Not Renewed, or Item Overdue. No other values of Circulation Status are permitted for this service.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'CirculationStatusUpdated' => array(
            'name' => t('Circulation Status Updated'),
            'data elements' => array(
              'required' => array(
                'CirculationStatus' => array(),
                'UniqueItemId' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'CirculationStatusUpdatedResponse' => array(
            'name' => t('Circulation Status Updated Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemCheckedInService' => array(
      'name' => t('Item Checked In Service'),
      'description' => t('This service informs the responding application that an Item has been checked in. The initiating application provides the Unique Item Id and may optionally provide other details associated with the check in.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemCheckedIn' => array(
            'name' => t('Item Checked In'),
            'data elements' => array(
              'required' => array(
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
                'ItemOptionalFields' => array(),
                'RoutingInformation' => array(),
                'UniqueUserId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemCheckedInResponse' => array(
            'name' => t('Item Checked In Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemCheckedOutService' => array(
      'name' => t('Item Checked Out Service'),
      'description' => t('This service informs the responding application that an Item has been checked out to a User. The initiating application provides the Unique Item Id and Unique User Id involved in the check out, and may optionally provide other details associated with the check out.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemCheckedOut' => array(
            'name' => t('Item Checked Out'),
            'data elements' => array(
              'required' => array(
                'DateDue' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('NonReturnableFlag', 'IndeterminateLoanPeriodFlag'),
                  ),
                ),
                'NonReturnableFlag' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('DateDue', 'IndeterminateLoanPeriodFlag'),
                  ),
                ),
                'IndeterminateLoanPeriodFlag' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('DateDue', 'NonReturnableFlag'),
                  ),
                ),
                'UniqueItemId' => array(),
                'UniqueUserId' => array(),
              ),
              'optional' => array(
                'ElectronicResourceProvidedFlag' => array(),
                'FiscalTransactionInformation' => array(),
                'ItemOptionalField' => array(),
                'RenewalCount' => array(),
                'ShippingInformation' => array(),
                'UniqueRequestId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemCheckedOutResponse' => array(
            'name' => t('Item Checked Out Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemCreatedService' => array(
      'name' => t('Item Created Service'),
      'description' => t('This service informs the responding application that a new Item has been created. The initiating application provides the Unique Item Id and details about the Item, and may optionally provide other details associated with the Item.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemCreated' => array(
            'name' => t('Item Created'),
            'data elements' => array(
              'required' => array(
                'BibliographicDescription' => array(),
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'CirculationStatus' => array(),
                'ItemDescription' => array(),
                'ItemUseRestrictionType' => array(),
                'Location' => array(
                  'repeatable' => TRUE,
                ),
                'PhysicalCondition' => array(),
                'SecurityMarker' => array(),
                'SensitizationFlag' => array(),
                'UniqueRequestId' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemCreatedResponse' => array(
            'name' => t('Item Created Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemRecallCancelledService' => array(
      'name' => t('Item Recall Cancelled Service'),
      'description' => t('This service informs the responding application that a recall has been cancelled. The initiating application provides the Unique Item Id, and may optionally provide the Unique User Id and other data related to cancellation of the recall.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemRecallCancelled' => array(
            'name' => t('Item Recall Cancelled'),
            'data elements' => array(
              'required' => array(
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
                'ItemOptionalFields' => array(),
                'UniqueUserId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemRecallCancelledResponse' => array(
            'name' => t('Item Recall Cancelled Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemRecalledService' => array(
      'name' => t('Item Recalled Service'),
      'description' => t('This service informs the responding application that an Item has been recalled. The initiating application provides the Unique Item Id of the recalled item and a new due date, and may optionally provide other details associated with the recall.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemRecalled' => array(
            'name' => t('Item Recalled'),
            'data elements' => array(
              'required' => array(
                'DateDue' => array(),
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'ItemOptionalFields' => array(),
                'ShippingInformation' => array(),
                'UniqueUserId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemRecalledResponse' => array(
            'name' => t('Item Recalled Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemRecallCancelledService' => array(
      'name' => t('Item Recall Cancelled Service'),
      'description' => t('This service informs the responding application that a recall has been cancelled. The initiating application provides the Unique Item Id, and may optionally provide the Unique User Id and other data related to cancellation of the recall.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemRecallCancelled' => array(
            'name' => t('Item Recall Cancelled'),
            'data elements' => array(
              'required' => array(
                'Unique Item Id' => array(),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
                'ItemOptionalFields' => array(),
                'UniqueUserId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemRecallCancelledResponse' => array(
            'name' => t('Item Recall Cancelled Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemRecalledService' => array(
      'name' => t('Item Recalled Service'),
      'description' => t('This service informs the responding application that an Item has been recalled. The initiating application provides the Unique Item Id of the recalled item and a new due date, and may optionally provide other details associated with the recall.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemRecalled' => array(
            'name' => t('Item Recalled'),
            'data elements' => array(
              'required' => array(
                'DateDue' => array(),
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'ItemOptionalFields' => array(),
                'ShippingInformation' => array(),
                'UniqueUserId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemRecalledResponse' => array(
            'name' => t('Item Recalled Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemReceivedService' => array(
      'name' => t('Item Received Service'),
      'description' => t(''),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemReceived' => array(
            'name' => t('Item Received'),
            'data elements' => array(
              'required' => array(
                'DateReceived' => array(),
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'ItemOptionalFields' => array(),
                'UniqueRequestId' => array(),
                'UniqueUserId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemReceivedResponse' => array(
            'name' => t('Item Received Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemRenewedService' => array(
      'name' => t('Item Renewed Service'),
      'description' => t('This service informs the responding application that an Item has been renewed. The initiating application provides the Unique Item Id and the new due date, and may optionally supply other details associated with the renewal.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemRenewed' => array(
            'name' => t('Item Renewed'),
            'data elements' => array(
              'required' => array(
                'DateDue' => array(),
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
                'ItemOptionalFields' => array(),
                'RenewalCount' => array(),
                'UniqueUserId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemRenewedResponse' => array(
            'name' => t('Item Renewed Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemRequestCancelledService' => array(
      'name' => t('Item Request Cancelled Service'),
      'description' => t('This service informs the responding application that a request for an Item has been cancelled. The initiating application provides the Unique Item Id and/or Unique Request Id, and the Unique User Id, and the type of the request being cancelled. It may optionally provide other details associated with the request being cancelled.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemRequestCancelled' => array(
            'name' => t('Item Request Cancelled'),
            'data elements' => array(
              'required' => array(
                'RequestType' => array(),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_AND_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_AND_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueUserId' => array(),
              ),
              'optional' => array(
                'FiscalTransactionInformation' => array(),
                'ItemOptionalFields' => array(),
                'RequestScopeType' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemRequestCancelledResponse' => array(
            'name' => t('Item Request Cancelled Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemRequestUpdatedService' => array(
      'name' => t('Item Request Updated Service'),
      'description' => t('This service informs the responding application that a request for an Item has been updated. The initiating application provides the Unique Item Id and the Unique User Id, or the Unique Request Id. The initiating application provides elements that have been deleted with the value for each element and/or elements that have been added with the value for each element. If the value for an existing element has changed, the message must include that element in both the delete and add sections of the message. Other data may also be provided.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemRequestUpdated' => array(
            'name' => t('Item Request Updated'),
            'data elements' => array(
              'required' => array(
                'RequestType' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('RequestType', 'UniqueItemId', 'UniqueUserId'),
                  ),
                ),
              ),
              'optional' => array(
                'AddRequestFields' => array(),
                'DeleteRequestFields' => array(),
                'ItemOptionalFields' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemRequestUpdatedResponse' => array(
            'name' => t('Item Request Updated Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemRequestedService' => array(
      'name' => t('Item Requested Service'),
      'description' => t('This service informs the responding application that a request for an Item, which may not be immediately available, has been made. The initiating application provides the Unique Item Id, or the Unique Request Id and Unique Bibliographic ID. It also provides the Unique User Id and the type of request being made. It may also optionally provide other details related to the request.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemRequested' => array(
            'name' => t('Item Requested'),
            'data elements' => array(
              'required' => array(
                'RequestScopeType' => array(),
                'RequestType' => array(),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId', 'UniqueBibliographicId'),
                  ),
                ),
                'UniqueBibliographicId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueItemId'),
                  ),
                ),
                'UniqueUserId' => array(),
              ),
              'optional' => array(
                'DateAvailable' => array(),
                'DateOfUserRequest' => array(),
                'EarliestDateNeeded' => array(),
                'ItemOptionalFields' => array(),
                'NeedBeforeDate' => array(),
                'PickupExpiryDate' => array(),
                'ShippingInformation' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemRequestedResponse' => array(
            'name' => t('Item Requested Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemShippedService' => array(
      'name' => t('Item Shipped Service'),
      'description' => t('This service informs the responding application that an Agency has shipped an Item. The date when the Item was shipped and the address to which it was shipped must be included in the message. When shipment supports a user request, User data should be supplied with the message. The initiating application may also provide other details associated with shipment of the Item.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemShipped' => array(
            'name' => t('Item Shipped'),
            'data elements' => array(
              'required' => array(
                'DateShipped' => array(),
                'ShippingInformation' => array(),
                'UniqueItemId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueRequestId'),
                  ),
                ),
                'UniqueRequestId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array(),
                    NCIP_USAGE_AND_OR => array('UniqueItemId'),
                  ),
                ),
              ),
              'optional' => array(
                'ItemOptionalFields' => array(),
                'UniqueUserId' => array(),
                'UserOptionalFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemShippedResponse' => array(
            'name' => t('Item Shipped Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'ItemUpdatedService' => array(
      'name' => t('Item Updated Service'),
      'description' => t('This service informs the responding application that one or more data elements of an Item have been updated. The initiating application provides elements that have been deleted with the value for each element and/or elements that have been added with the value for each element. If the value for an existing element has changed, the message must include that element in both the delete and add sections of the message. This service may not be used to report changes to circulation status.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'ItemUpdated' => array(
            'name' => t('Item Updated'),
            'data elements' => array(
              'required' => array(
                'UniqueItemId' => array(),
              ),
              'optional' => array(
                'AddItemFields' => array(),
                'DeleteItemFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'ItemUpdatedResponse' => array(
            'name' => t('Item Updated Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'UserCreatedService' => array(
      'name' => t('User Created Service'),
      'description' => t('User Created Service'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UserCreated' => array(
            'name' => t('User Created'),
            'data elements' => array(
              'required' => array(
                'NameInformation' => array(),
                'UniqueUserId' => array(),
              ),
              'optional' => array(
                'BlockOrTrap' => array(
                  'repeatable' => TRUE,
                ),
                'DateOfBirth' => array(),
                'UserAddressInformation' => array(
                  'repeatable' => TRUE,
                ),
                'UserAddressInformation' => array(
                  'repeatable' => TRUE,
                ),
                'UserPrivilege' => array(
                  'repeatable' => TRUE,
                ),
                'VisibleUserId' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'UserCreatedResponse' => array(
            'name' => t('User Created Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'UserFiscalTransactionCreatedService' => array(
      'name' => t('User Fiscal Transaction Created Service'),
      'description' => t('This service informs the responding application that a new fiscal transaction has been applied to a User\'s fiscal account.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UserFiscalTransactionCreated' => array(
            'name' => t('User Fiscal Transaction Created'),
            'data elements' => array(
              'required' => array(
                'FiscalTransactionInformation' => array(),
                'UniqueUserId' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'UserFiscalTransactionCreatedResponse' => array(
            'name' => t('User Fiscal Transaction Created Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'UserNoticeSentService' => array(
      'name' => t('User Notice Sent Service'),
      'description' => t('This service informs the responding application that a notice has been sent to a User. The initiating application provides the Unique User Id, the type of notice, and details about the notice.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UserNoticeSent' => array(
            'name' => t('User Notice Sent'),
            'data elements' => array(
              'required' => array(
                'DateSent' => array(),
                'UniqueUserId' => array(),
                'UserNoticeDetails' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'UserNoticeSentResponse' => array(
            'name' => t('User Notice Sent Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
    'UserUpdatedService' => array(
      'name' => t('User Updated Service'),
      'description' => t('This service informs the responding application that one or more data elements of a User have been updated. The initiating application provides elements that have been deleted with the value for each element and/or elements that have been added with the value for each element. If the value for an existing element has changed, the message must include that element in both the delete and add sections of the message.'),
      'type' => NCIP_NOTIFICATION_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'UserUpdated' => array(
            'name' => t('User Updated'),
            'data elements' => array(
              'required' => array(
                'UniqueUserId' => array(),
              ),
              'optional' => array(
                'AddUserFields' => array(),
                'DeleteUserFields' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'UserUpdatedResponse' => array(
            'name' => t('User Updated Response'),
            'data elements' => array(
              'optional' => array(
                'MessagingError' => array(
                  'repeatable' => TRUE,
                ),
              ),
            ),
          ),
        ),
      ),
    ),
  );
  return $services;
}

/**
 * Implements hook_ncip_data_elements().
 */
function ncip_ncip_data_elements(&$data_elements) {

  $unique_agency_id_type = NCIP_OPEN_SCHEME;
  // TODO: check this!!
  $unique_agency_id_data_elements = array();
  $amount_type = NCIP_COMPLEX_ELEMENT;
  $amount_data_elements = array(
    'CurrencyCode' => array(
      'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
    ),
    'MonetaryValue' => array(
      'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
    ),
  );
  $unique_user_id_type = NCIP_COMPLEX_ELEMENT;
  $unique_user_id_data_elements = array(
    'UniqueAgencyId' => array(
      'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
    ),
    'UserIdentifierValue' => array(
      'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
    ),
  );

  /*
   'name' => t('Unique User Id'),
   'description' => t(''),
   'type' => NCIP_COMPLEX_ELEMENT,
   'data elements' => array(
   'UniqueAgencyId' => array(
   'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE
   ),
   'UserIdentifierValue' => array(
   'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE
   ),
   ),
   ),
   */

  $data_elements['ncip'] = array(
    // this is the first, because it is referenced by the AcknowledgedFeeAmount
    'AcknowledgedFeeAmount' => array(
      'name' => t('Acknowledged Fee Amount'),
      'description' => t('Monetary value in a specified currency (i.e., Amount), acknowledged by a User as the fee charged for a requested service.'),
      'type' => $amount_type,
      'data elements' => $amount_data_elements,
    ),
    'AcknowledgedItemUseRestrictionType' => array(
      'name' => t('Acknowledged Item Use Restriction Type'),
      'description' => t('Indicates that the User has acknowledged special rules and restrictions on the use of the Item. This acknowledgement is carried in messages for the request check out, or renewal of the Item.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'AddAgencyFields' => array(
      'name' => t('Add Agency Fields'),
      'description' => t('Used to identify data elements that can be added as part of the Update Agency and Agency Updated services.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AgencyAddressInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'AgencyUserPrivilegeType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'ApplicationProfileSupportedType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'AuthenticationPrompt' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'ConsortiumAgrement' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'OrganizationNameInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
      ),
    ),
    'AddItemFields' => array(
      'name' => t('Add Item Fields'),
      'description' => t('Used to identify data elements that can be added as part of the Update User and User Updated services.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AuthenticationInput' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'BlockOrTrap' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'DateOfBirth' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'NameInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'UserAddressInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'UserLanguage' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'UserPrivilege' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'VisibleUserId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
      ),
    ),
    'AgencyAddressInformation' => array(
      'name' => t('Agency Address Information'),
      'description' => t('Information about an Agency\'s physical or electronic address.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AgencyAddressRoleType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'ElectronicAddress' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('PhysicalAddress'),
          ),
        ),
        'PhysicalAddress' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ElectronicAddress'),
          ),
        ),
        'ValidFromDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ValidToDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'AgencyAddressRoleType' => array(
      'name' => t('Agency Address Role Type'),
      'description' => t('Describes the purpose or role of the address associated with an Agency. An address may serve more than one purpose or role for an Agency.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'AgencyElementType' => array(
      'name' => t('Agency Element Type'),
      'description' => t('Identifies the elements of the Agency Object on which queries (i.e., lookups) can be made.'),
      'type' => NCIP_CLOSED_SCHEME,
      'scheme' => 'http://www.niso.org/ncip/v1_0/schemes/agencyelementtype/agencyelementtype.scm',
    ),
    'AgencyUserPrivilegeType' => array(
      'name' => t('Agency User Privilege Type'),
      'description' => t('Identifies the types of privileges that may be granted by a particular Agency or available to Users of a specific Agency.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'Amount' => array(
      'name' => t('Amount'),
      'description' => t('The amount of a fiscal charge or fee.'),
      'type' => $amount_type,
      'data elements' => $amount_data_elements,
    ),
    'CurrencyCode' => array(
      'name' => t('Currency Code'),
      'description' => t('Indicates the type of currency of a particular sum of money; values are currency codes specified in ISO 4217.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'MonetaryValue' => array(
      'name' => t('Monetary Value'),
      'description' => t('Integer expressed as positive, negative, or zero, multiplied by ten to the power M, where M is the value of the Minor unit for that currency as defined in ISO 4217, Section 6. Gives the Amount in terms of the value of Currency Code.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'ApplicationProfileSupportedType' => array(
      'name' => t('Application Profile Supported Type'),
      'description' => t('Identifies the Application Profiles supported by an Agency.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'ApplicationProfileType' => array(
      'name' => t('Application Profile Type'),
      'description' => t('Identifies for each Circulation Interchange Protocol service the Application Profile under which the message is sent. Application Profiles define the context of the interchange between applications, for example, between circulation and selfservice application; between circulation and ILL applications; or between disparate circulation applications within a consortium.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'AuthenticationDataFormatType' => array(
      'name' => t('Authentication Data Format Type'),
      'description' => t('Identifies the format of the authentication data that an Agency requires.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'AuthenticationInput' => array(
      'name' => t('Authentication Input'),
      'description' => t('Information, such as a password, provided by a User when prompted; used to authenticate the User.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AuthenticationDataFormatType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'AuthenticationInputData' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'AuthenticationInputType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'AuthenticationInputData' => array(
      'name' => t('Authentication Input Data'),
      'description' => t('Input by a User in response to an authentication prompt.'),
      'type' => NCIP_STRING,
    ),
    'AuthenticationInputType' => array(
      'name' => t('Authentication Input Type'),
      'description' => t('Identifies the nature of the data to be supplied to authenticate a User.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'AuthenticationPrompt' => array(
      'name' => t('Authentication Prompt'),
      'description' => t('Agency-specific data; used to prompt a User for entry of authentication data.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'PromptInput' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'PromptOutput' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'PromptInput' => array(
      'name' => t('Prompt Input'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AuthenticationDataFormatType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'AuthenticationInputType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'SensitiveDataFlag' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'PromptOutput' => array(
      'name' => t('Prompt Output'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AuthenticationPromptData' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'AuthenticationPromptType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'SensitiveDataFlag' => array(
      'name' => t('Sensitive Data Flag'),
      'description' => t('Indicates whether or not authentication input should be treated as sensitive data.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'AuthenticationPromptData' => array(
      'name' => t('Authentication Prompt Data'),
      'description' => t('The type specified by Authentication Prompt Type; representing prompts used by an Agency to request input of authentication data.'),
      'type' => NCIP_STRING,
    ),
    'AuthenticationPromptType' => array(
      'name' => t('Authentication Prompt Type'),
      'description' => t('Identifies the format of the authentication prompt.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'BibliographicDescription' => array(
      'name' => t('Bibliographic Description'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'Author' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'AuthorOfComponent' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'BibliographicItemId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'BibliographicLevel' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'BibliographicRecordId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ComponentId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Edition' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ElectronicDataFormatType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Language' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'MediumType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Pagination' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'PlaceOfPublication' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'PublicationDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'PublicationDateOfComponent' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Publisher' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'SeriesTitleNumber' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'SponsoringBody' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Title' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'TitleOfComponent' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'Author' => array(
      'name' => t('Author'),
      'description' => t('Gives the name of person or corporate body responsible for the intellectual or artistic content of an Item, including composers, creators, or originators of an Item.'),
      'type' => NCIP_STRING,
    ),
    'AuthorOfComponent' => array(
      'name' => t('Author Of Component'),
      'description' => t('Gives the name of the author of a bibliographic item that is a component part of another item.'),
      'type' => NCIP_STRING,
    ),
    'BibliographicItemId' => array(
      'type' => NCIP_COMPLEX_ELEMENT,
      'name' => t('Bibliographic Item Id'),
      'data elements' => array(
        'BibliographicItemIdentifierCode' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'BibliographicItemIdentifier' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'BibliographicItemIdentifierCode' => array(
      'name' => t('Bibliographic Item Identifier Code'),
      'description' => t('Identifies the source of resource identifier associated with the bibliographic item.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'BibliographicItemIdentifier' => array(
      'name' => t('Bibliographic Item Identifier'),
      'description' => t('Provides a resource identifier for the bibliographic item.'),
      'type' => NCIP_STRING,
    ),
    'BibliographicLevel' => array(
      'name' => t('Bibliographic Level'),
      'description' => t('Identifies the level of the bibliographic description of the Item.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'BibliographicRecordId' => array(
      'name' => t('Bibliographic Record Id'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'BibliographicRecordIdentifier' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'BibliographicRecordIdentifierCode' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('UniqueAgencyId'),
          ),
        ),
        'UniqueAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('BibliographicRecordIdentifierCode'),
          ),
        ),
      ),
    ),
    'BibliographicRecordIdentifier' => array(
      'name' => t('Bibliographic Record Identifier'),
      'description' => t('Identifies the machine-readable record that describes a bibliographic item.'),
      'type' => NCIP_STRING,
    ),
    'BibliographicRecordIdentifierCode' => array(
      'name' => t('Bibliographic Record Identifier Code'),
      'description' => t('Identifies the numbering scheme that uniquely identifies a bibliographic record. Code values are usually associated with national bibliographies or bibliographic utilities.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'ComponentId' => array(
      'name' => t('Component Id'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ComponentIdentifier' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'ComponentIdentifierType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'ComponentIdentifier' => array(
      'name' => t('Component Identifier'),
      'description' => t('Identifies component part of another bibliographic item.'),
      'type' => NCIP_STRING,
    ),
    'ComponentIdentifierType' => array(
      'name' => t('Component Identifier Type'),
      'description' => t('Identifies a component part of a bibliographic item.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'Edition' => array(
      'name' => t('Edition'),
      'description' => t('Gives edition statement that identifies all the copies of an item produced from one master copy or substantially the same type image, having the same contents, and, in the case of non-book materials, issued by a particular publishing agency or group of such agencies.'),
      'type' => NCIP_STRING,
    ),
    'ElectronicDataFormatType' => array(
      'name' => t('Electronic Data Format Type'),
      'description' => t('Identifies the format in which electronic data is available for access, download, etc.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'MediumType' => array(
      'name' => t('Medium Type'),
      'description' => t('Identifies the medium on which the bibliographic item has been produced.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'Pagination' => array(
      'name' => t('Pagination'),
      'description' => t('Gives number of pages or leaves in an item or a component part of an item.'),
      'type' => NCIP_STRING,
    ),
    'PlaceOfPublication' => array(
      'name' => t('Place Of Publication'),
      'description' => t('Gives geographic location of the publisher, or failing this, of the printer, distributor, or manufacturer.'),
      'type' => NCIP_STRING,
    ),
    'PublicationDate' => array(
      'name' => t('Publication Date'),
      'description' => t('Gives date of issue of an item as designated by the publisher.'),
      'type' => NCIP_STRING,
    ),
    'PublicationDateOfComponent' => array(
      'name' => t('Publication Date Of Component'),
      'description' => t('Gives publication date assigned by the publisher to the component of a item.'),
      'type' => NCIP_STRING,
    ),
    'Publisher' => array(
      'name' => t('Publisher'),
      'description' => t('Gives name of person(s) or organization(s) responsible for the publication of an item.'),
      'type' => NCIP_STRING,
    ),
    'SeriesTitleNumber' => array(
      'name' => t('Series Title Number'),
      'description' => t('Gives name assigned to a group of separate publications related to one another by the fact that each bears a collective title applying to the group or subgroup as a whole as well as its own title, and the number within that series assigned to one of the pieces.'),
      'type' => NCIP_STRING,
    ),
    'SponsoringBody' => array(
      'name' => t('Sponsoring Body'),
      'description' => t('Gives title of an item that is a component part of another Item, such as a chapter of a book or an article in a issue of a journal.'),
      'type' => NCIP_STRING,
    ),
    'Title' => array(
      'name' => t('Title'),
      'description' => t('Gives name of an item (usually assigned by the author or publisher) usually consisting of a word or group of words intended to identify it.'),
      'type' => NCIP_STRING,
    ),
    'TitleOfComponent' => array(
      'name' => t('Title Of Component'),
      'description' => t('Gives title of an item that is a component part of another Item, such as a chapter of a book or an article in a issue of a journal.'),
      'type' => NCIP_STRING,
    ),
    'BlockOrTrap' => array(
      'name' => t('Block Or Trap'),
      'description' => t('Data specifying the nature of the block or trap.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'BlockOrTrapType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'UniqueAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'ValidFromDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ValidToDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'BlockOrTrapType' => array(
      'name' => t('Block Or Trap Type'),
      'description' => t('Identifies the type of block or trap imposed by an Agency on a User.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'CirculationStatus' => array(
      'name' => t('Circulation Status'),
      'description' => t('Indicates the current availability status of a bibliographic item.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'ConsortiumAgreement' => array(
      'name' => t('Consortium Agreement'),
      'description' => t('Identifies a consortium agreement to which an Agency is a party.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'CurrentBorrowerDesired' => array(
      'name' => t('Current Borrower Desired'),
      'description' => t('Indicates whether or not requester desires information about the User who currently has the Item on loan.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'CurrentRequestersDesired' => array(
      'name' => t('Current Requesters Desired'),
      'description' => t('Indicates that the requester desires information about all Users that have requested a specific Item.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'DateAvailable' => array(
      'name' => t('Date Available'),
      'description' => t('Indicates when the requested Item is expected to be available to the User.'),
      'type' => NCIP_DATETIME,
    ),
    'DateDue' => array(
      'name' => t('Date Due'),
      'description' => t('Indicates when the loan of an Item is scheduled to end.'),
      'type' => NCIP_DATETIME,
    ),
    'DateEventOccurred' => array(
      'name' => t('Date Event Occurred'),
      'description' => t('Indicates the date and time that the initiating application took the mandated action.'),
      'type' => NCIP_DATETIME,
    ),
    'DateForReturn' => array(
      'name' => t('Date For Return'),
      'description' => t('Indicates the date and time specified by the lending Agency for the return of an Item. Initiating applications provide this data when requesting that a responding application accept the Item for circulation to or pick-up by a User.'),
      'type' => NCIP_DATETIME,
    ),
    'DateOfBirth' => array(
      'name' => t('Date Of Birth'),
      'description' => t('Indicates a User\'s date of birth.'),
      'type' => NCIP_DATETIME,
    ),
    'DateOfUserRequest' => array(
      'name' => t('Date Of User Request'),
      'description' => t('Indicates when a User has issued a request for an Item.'),
      'type' => NCIP_DATETIME,
    ),
    'DatePlaced' => array(
      'name' => t('Date Placed'),
      'description' => t('Indicates when a User placed a request for an Item.'),
      'type' => NCIP_DATETIME,
    ),
    'DateRecalled' => array(
      'name' => t('Date Recalled'),
      'description' => t('Indicates when a recall for an Item was issued.'),
      'type' => NCIP_DATETIME,
    ),
    'DateReceived' => array(
      'name' => t('Date Received'),
      'description' => t('Indicates when an Agency received an Item.'),
      'type' => NCIP_DATETIME,
    ),
    'DateReportedReturned' => array(
      'name' => t('Date Reported Returned'),
      'description' => t('Indicates when the User purports to have returned the Item.'),
      'type' => NCIP_DATETIME,
    ),
    'DateSent' => array(
      'name' => t('Date Sent'),
      'description' => t('Indicates when an Item or a notice was sent.'),
      'type' => NCIP_DATETIME,
    ),
    'DateShipped' => array(
      'name' => t('Date Shipped'),
      'description' => t('indicates when an Agency shipped an Item.'),
      'type' => NCIP_DATETIME,
    ),
    'DateToSend' => array(
      'name' => t('Date To Send'),
      'description' => t('Indicates when an Item or a notice is to be sent.'),
      'type' => NCIP_DATETIME,
    ),
    'DateWillSend' => array(
      'name' => t('Date Will Send'),
      'description' => t('Indicates when an Agency will send an Item or a notice.'),
      'type' => NCIP_DATETIME,
    ),
    'DeleteAgencyFields' => array(
      'name' => t('Delete Agency Fields'),
      'description' => t('Used to identify data elements that can be deleted as part of the Update Agency and Agency Updated services.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AgencyAddressInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'AgencyUserPrivilegeType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'ApplicationProfileSupportedType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'AuthenticationPrompt' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'ConsortiumAgreement' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'OrganizationNameInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
      ),
    ),
    'DeleteItemFields' => array(
      'name' => t('Delete Item Fields'),
      'description' => t('Used to identify data elements that can be deleted as part of the Update Item and Item Updated services.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'BibliographicDescription' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ItemDescription' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ItemUseRestrictionType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'Location' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'PhysicalCondition' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'SecurityMarker' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'SensitizationFlag' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'DeleteRequestFields' => array(
      'name' => t('Delete Request Fields'),
      'description' => t('Used to identify data elements that can be deleted as part of the Update Request Item and Item Request Updated services.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AcknowledgedFeeAmount' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'DateAvailable' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'DateOfUserRequest' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'EarliestDateNeeded' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'NeedBeforeDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'PickupExpiryDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'RequestScopeType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'RequestStatusType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'RequestType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ShippingInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'UniqueItemId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'UniqueUserId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'DeleteUserFields' => array(
      'name' => t('Delete User Fields'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AuthenticationInput' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'BlockOrTrap' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'DateOfBirth' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'NameInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'UserAddressInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'UserLanguage' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'UserPrivilege' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'VisibleUserId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
      ),
    ),
    'DesiredDateDue' => array(
      'name' => t('Desired Date Due'),
      'description' => t('Indicates the date the User wishes the Item to be due. Initiating applications supply this data at the time that a request is issued for check out, renewal, or recall.'),
      'type' => NCIP_DATETIME,
    ),
    'DesiredDateForReturn' => array(
      'name' => t('Desired Date For Return'),
      'description' => t('Indicates the date desired by the borrowing Agency for the return of an Item. This data is provided when requesting the renewal of an Item for which the circulation activity is being managed by an application other than that of the original lending Agency and the original lending Agency has specified a Date For Return.'),
      'type' => NCIP_DATETIME,
    ),
    'EarliestDateNeeded' => array(
      'name' => t('Earliest Date Needed'),
      'description' => t('Indicates the earliest date at which the User has need of an Item. Implies that the requested Item is not required or desired prior to the date specified.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        '' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'ElectronicAddress' => array(
      'name' => t('Electronic Address'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ElectronicAddressType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'ElectronicAddressData' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'ElectronicAddressType' => array(
      'name' => t('Electronic Address Type'),
      'description' => t('Identifies the type of address used to communicate electronically with an Agency or a User. Values are specified in the IANA Registry of Media Types.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'ElectronicAddressData' => array(
      'name' => t('Electronic Address Data'),
      'description' => t('Provides the electronic address.'),
      'type' => NCIP_STRING,
    ),
    'ElectronicResource' => array(
      'name' => t('Electronic Resource'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ActualResource' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ReferenceToResource'),
          ),
        ),
        'ElectronicDataFormatType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ReferenceToResource'),
          ),
        ),
        'ReferenceToResource' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ActualResource', 'ElectronicDataFormatType'),
          ),
        ),
      ),
    ),
    'ActualResource' => array(
      'name' => t('Actual Resource'),
      'description' => t('Gives the electronic resource entity, such as a computer file or the full text of the Item.'),
      'type' => NCIP_STRING,
    ),
    'ElectronicDataFormatType' => array(
      'name' => t('Electronic Data Format Type'),
      'description' => t('Identifies the data type of the electronic resource. Values are specified in the IANA Registry of Media Types.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'ReferenceToResource' => array(
      'name' => t('Reference To Resource'),
      'description' => t('Provides a pointer to the resource rather than providing the resource itself.'),
      'type' => NCIP_STRING,
    ),
    'ElectronicResourceProvidedFlag' => array(
      'name' => t('Electronic Resource Provided Flag'),
      'description' => t('Indicates whether or not the bibliographic item, in the form of an electronic resource, has been provided as part of the Check Out Item Service.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'FiscalTransactionInformation' => array(
      'name' => t('Fiscal Transaction Information'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'Amount' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'FiscalActionType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'FiscalTransactionDescription' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'FiscalTransactionDescriptionId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'FiscalTransactionType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'ItemDetails' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'PaymentMethodType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'RelatedFiscalTransactionReferenceId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'UniqueRequestId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ValidFromDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ValidToDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'FiscalActionType' => array(
      'name' => t('Fiscal Action Type'),
      'description' => t('Identifies a fiscal action.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'FiscalTransactionDescription' => array(
      'name' => t('Fiscal Transaction Description'),
      'description' => t('Gives supplementary details of the fiscal transaction'),
      'type' => NCIP_STRING,
    ),
    'FiscalTransactionDescriptionId' => array(
      'name' => t('Fiscal Transaction Description Id'),
      'description' => t('Used to identify a specific financial transaction.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'FiscalTransactionIdentifierValue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'Unique Agency Id' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'FiscalTransactionIdentifierValue' => array(
      'name' => t('Fiscal Transaction Identifier Value'),
      'description' => t('Gives value to be used to identify a fiscal transaction.'),
      'type' => NCIP_STRING,
    ),
    'FiscalTransactionType' => array(
      'name' => t('Fiscal Transaction Type'),
      'description' => t('Identifies the type of fiscal charge.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'FromAgencyAuthentication' => array(
      'name' => t('From Agency Authentication'),
      'description' => t('Provides data to be used by recipient to authenticate the Agency initiating the message.'),
      'type' => NCIP_STRING,
    ),
    'FromAgencyId' => array(
      'name' => t('From Agency Id'),
      'description' => t('Unique identification of the Agency (i.e., Unique Agency Id) from which the message was sent.'),
      'type' => $unique_agency_id_type,
      'data elements' => $unique_agency_id_data_elements,
    ),
    'FromSystemAuthentication' => array(
      'name' => t('From System Authentication'),
      'description' => t('Provides data to be used by recipient to authenticate the system from which the message was sent.'),
      'type' => NCIP_STRING,
    ),
    'FromSystemId' => array(
      'name' => t('From System Id'),
      'description' => t('Identifies the system from which a message was sent.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'HoldPickupDate' => array(
      'name' => t('Hold Pickup Date'),
      'description' => t('Indicates when a hold expires for a given User.'),
      'type' => NCIP_DATETIME,
    ),
    'HoldQueueLength' => array(
      'name' => t('Hold Queue Length'),
      'description' => t('Indicates the number of Users who currently have requested a hold on an Item.'),
      'type' => NCIP_NON_NEGATIVE_INTEGER,
    ),
    'HoldQueuePosition' => array(
      'name' => t('Hold Queue Position'),
      'description' => t('Indicates the position in the hold queue of a request by a User for an Item, conditional on the value of Request Type of “hold.”'),
      'type' => NCIP_POSITIVE_INTEGER,
    ),
    'HoldingsInformation' => array(
      'name' => t('Holdings Information'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'StructuredHoldingsData' => array(
          'usage' => NCIP_USAGE_ONE_OR_MORE,
          'conditions' => array(
            NCIP_USAGE_OR => array('UnstructuredHoldingsData'),
          ),
        ),
        'UnstructuredHoldingsData' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('StructuredHoldingsData'),
          ),
        ),
      ),
    ),
    'IndeterminateLoanPeriodFlag' => array(
      'name' => t('Indeterminate Loan Period Flag'),
      'description' => t('Indicates that the loan is for an indefinite period.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'InitiationHeader' => array(
      'name' => t('Initiation Header'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ApplicationProfileType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'FromAgencyAuthentication' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'FromAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'required' => TRUE,
        ),
        'FromSystemAuthentication' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'FromSystemId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'OnBehalfOfAgency' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ToAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'required' => TRUE,
        ),
        'ToSystemId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'ItemDescription' => array(
      'name' => t('Item Description'),
      'description' => t('Description of a specific physical or electronic Item belonging to an Agency\'s collection(s).'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'CallNumber' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'CopyNumber' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'HoldingsInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ItemDescriptionLevel' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'VisibleItemId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'CallNumber' => array(
      'name' => t('Call Number'),
      'description' => t('Indicates permanent relative physical location assigned to a bibliographic item.'),
      'type' => NCIP_STRING,
    ),
    'CopyNumber' => array(
      'name' => t('Copy Number'),
      'description' => t('Gives the copy number of an Item held by an Agency. Occurs 0 or 1 time.'),
      'type' => NCIP_STRING,
    ),
    'ItemDescriptionLevel' => array(
      'name' => t('Item Description Level'),
      'description' => t('Identifies the level at which the Item is described.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'VisibleItemId' => array(
      'name' => t('Visible Item Id'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'VisibleItemIdentifier' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'VisibleItemIdentifierType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'VisibleItemIdentifier' => array(
      'name' => t('Visible Item Identifier'),
      'description' => t('Gives a barcode or another eye-readable identifier that is affixed to or accompanies a physical Item. The value of this identifier may be identical to the Item Identifier Value used in the Unique Item Id.'),
      'type' => NCIP_STRING,
    ),
    'VisibleItemIdentifierType' => array(
      'name' => t('Visible Item Identifier Type'),
      'description' => t('Identifies the type of Visible Item Identifier used.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'ItemDetails' => array(
      'name' => t('Item Details'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'BibliographicDescription' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'DateCheckedOut' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('IndeterminateLoanPeriodFlag', 'NonReturnableFlag'),
          ),
        ),
        'DateDue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'DateRenewed' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'DateReturned' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'IndeterminateLoanPeriodFlag' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('DateCheckedOut', 'NonReturnableFlag'),
          ),
        ),
        'NonReturnableFlag' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('DateCheckedOut', 'IndeterminateLoanPeriodFlag'),
          ),
        ),
        'UniqueItemId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'DateRenewed' => array(
      'name' => t('Date Renewed'),
      'description' => t('Indicates when an Item was renewed.'),
      'type' => NCIP_DATETIME,
    ),
    'DateReturned' => array(
      'name' => t('Date Returned'),
      'description' => t('Indicates when an Item was returned (not necessarily checked in, but no longer in the custody of a User or Agency)'),
      'type' => NCIP_DATETIME,
    ),
    'ItemElementType' => array(
      'name' => t('Item Element Type'),
      'description' => t('Identifies all the elements of the Item Object on which queries (i.e., lookups) can be made.'),
      'type' => NCIP_CLOSED_SCHEME,
      'scheme' => 'http://www.niso.org/ncip/v1_0/schemes/itemelementtype/itemelementtype.scm',
    ),
    'ItemOptionalFields' => array(
      'name' => t('Item Optional Fields'),
      'description' => t('Used in update response messages. Carries data elements describing the Item Object specifically as in the initiation message of an update service. Also used in the initiation message of a notification service to describe the Item Object.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'BibliographicDescription' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'CirculationStatus' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ElectronicResource' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'HoldQueueLength' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ItemDescription' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ItemUseRestrictionType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'Location' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'PhysicalCondition' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'SecurityMarker' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'SensitizationFlag' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'ItemReportedLost' => array(
      'name' => t('Item Reported Lost'),
      'description' => t('Indicates that a User or Agency has reported that the Item has been lost.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'ItemReportedMissing' => array(
      'name' => t('Item Reported Missing'),
      'description' => t('Indicates that a User or Agency has reported that the Item is missing.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'ItemReportedNeverBorrowed' => array(
      'name' => t('Item Reported Never Borrowed'),
      'description' => t('Indicates that a User or Agency has reported that the Item was never borrowed.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'ItemReportedReturned' => array(
      'name' => t('Item Reported Returned'),
      'description' => t('Indicates that a User or Agency has reported that the Item was returned on the date specified.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'DateReportedReturned' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'ItemTransaction' => array(
      'name' => t('Item Transaction'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'CurrentBorrower' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'CurrentRequester' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'CurrentBorrower' => array(
      'name' => t('Current Borrower'),
      'description' => t(''),
      'type' => $unique_user_id_type,
      'data elements' => $unique_user_id_data_elements,
    ),
    'CurrentRequester' => array(
      'name' => t('Current Requester'),
      'description' => t(''),
      'type' => $unique_user_id_type,
      'data elements' => $unique_user_id_data_elements,
    ),
    'ItemUseRestrictionType' => array(
      'name' => t('Item Use Restriction Type'),
      'description' => t('Indicates the circulation usage rules and any special restrictions that apply to an Item at the time of a circulation transaction.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'Language' => array(
      'name' => t('Language'),
      'description' => t('Identifies the language of the Item.'),
      'type' => NCIP_CLOSED_SCHEME,
      'scheme' => 'http://lcweb.loc.gov/standards/iso639-2/bibcodes.html',
    ),
    'LoanedItem' => array(
      'name' => t('Loaned Item'),
      'description' => t('Data describing an Item currently on loan to a User.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'Amount' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'DateDue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('IndeterminateLoanPeriodFlag'),
          ),
        ),
        'IndeterminateLoanPeriodFlag' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('DateDue'),
          ),
        ),
        'ReminderLevel' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'UniqueItemId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'LoanedItemsCount' => array(
      'name' => t('Loaned Items Count'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'CirculationStatus' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ItemUseRestrictionType'),
          ),
        ),
        'ItemUseRestrictionType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('CirculationStatus'),
          ),
        ),
        'LoanedItemCountValue' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'LoanedItemCountValue' => array(
      'name' => t('Loaned Item Count Value'),
      'description' => t('Specifies number of items that a User has on loan at any point in time for a given Circulation Status or Item Use Restriction Type.'),
      'type' => NCIP_NON_NEGATIVE_INTEGER,
    ),
    'LoanedItemsDesired' => array(
      'name' => t('Loaned Items Desired'),
      'description' => t('Indicates that the requester desires information about Items loaned to a specific User.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        '' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'Location' => array(
      'name' => t('Location'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'LocationName' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'LocationType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'ValidFromDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ValidToDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'LocationName' => array(
      'name' => t('Location Name'),
      'description' => t('Contains a sequence of Location Name Instances, which together express the location where an Item is housed at a particular point in time.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'LocationNameInstance' => array(
          'usage' => NCIP_USAGE_ONE_OR_MORE,
        ),
      ),
    ),
    'LocationNameInstance' => array(
      'name' => t('Location Name Instance'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'LocationNameLevel' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'LocationNameValue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'LocationNameLevel' => array(
      'name' => t('Location Name Level'),
      'description' => t('Indicates the level of the Location Name Instance in a sequence within Location Name.'),
      'type' => NCIP_POSITIVE_INTEGER,
    ),
    'LocationNameValue' => array(
      'name' => t('Location Name Value'),
      'description' => t('Provides a value for the Location Name Instance; gives name of an Agency-specific location or sub-location where an Item is housed at a particular point in time.'),
      'type' => NCIP_STRING,
    ),
    'LocationType' => array(
      'name' => t('Location Type'),
      'description' => t('Identifies the nature of the location where an Item is at a particular point in time.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'MandatedAction' => array(
      'name' => t('Mandated Action'),
      'description' => t('Indicates that the responding application shall honor the update request because the initiating application has already acted upon the update. For example, while in \'Offline Recovery\' mode, a self-service unit inserts this element in each of the back-logged update messages to inform the circulation application that the requested action (e.g., Check Out Item) has already taken place during a period of time when the connection between the partner applications was broken.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'DateEventOccurred' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'MessagingError' => array(
      'name' => t('Messaging Error'),
      'description' => t('Indicates that the responding application does not understand the initiation message. Includes identification of location and nature of any messaging errors in an initiation message.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'MessagingErrorType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'MessagingErrorElement' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'MessagingErrorType' => array(
      'name' => t('Messaging Error Type'),
      'description' => t('Indicates the reason for the messaging error.'),
      'type' => NCIP_CLOSED_SCHEME,
      'scheme' => 'http://www.niso.org/ncip/v1_0/schemes/messagingerrortype/messagingerrortype.scm',
    ),
    'MessagingErrorElement' => array(
      'name' => t('Messaging Error Element'),
      'description' => t('Recursive structure used in a response message to identify the element of an initiation message that is the subject of the error. It must fully specify the path from (but not including) the root node of the initiation message to the element that is the subject of the error.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ElementName' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'MessagingErrorElement' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('MessagingErrorValue'),
          ),
        ),
        'MessagingErrorValue' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('MessagingErrorElement'),
          ),
        ),
      ),
    ),
    'ElementName' => array(
      'name' => t('Element Name'),
      'description' => t('Provides name of data element in which error is located.'),
      'type' => NCIP_STRING,
    ),
    'MessagingErrorValue' => array(
      'name' => t('Messaging Error Value'),
      'description' => t('Contains the data element value that was in error.'),
      'type' => NCIP_STRING,
    ),
    'NameInformation' => array(
      'name' => t('Name Information'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'OrganizationNameInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('PersonalNameInformation'),
          ),
        ),
        'PersonalNameInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('OrganizationNameInformation'),
          ),
        ),
      ),
    ),
    'NeedBeforeDate' => array(
      'name' => t('Need Before Date'),
      'description' => t('Indicates when the User needs the requested Item. Implies that the requested Item is no longer required if the Item cannot be supplied by the date specified.'),
      'type' => NCIP_DATETIME,
    ),
    'NonReturnableFlag' => array(
      'name' => t('Non Returnable Flag'),
      'description' => t('Indicates that the Item is returnable.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'NoticeContent' => array(
      'name' => t('Notice Content'),
      'description' => t('Provides the contents of the notice.'),
      'type' => NCIP_STRING,
    ),
    'NoticeItem' => array(
      'name' => t('Notice Item'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'Amount' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ItemDetails' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'NoticeType' => array(
      'name' => t('Notice Type'),
      'description' => t('Identifies the type of notice to be sent or that has been sent to a User.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'OnBehalfOfAgency' => array(
      'name' => t('On Behalf Of Agency'),
      'description' => t('Unique identification of the Agency (i.e., Unique Agency Id) on behalf of which the initiating message is sent.'),
      'type' => $unique_agency_id_type,
      'data elements' => $unique_agency_id_data_elements,
    ),
    'OrganizationNameInformation' => array(
      'name' => t('Organization Name Information'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'OrganizationName' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'OrganizationNameType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'OrganizationName' => array(
      'name' => t('Organization Name'),
      'description' => t('Represents the name of an organization.'),
      'type' => NCIP_STRING,
    ),
    'OrganizationNameType' => array(
      'name' => t('Organization Name Type'),
      'description' => t('Identifies the type of Organization Name.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'Pending' => array(
      'name' => t('Pending'),
      'description' => t('Information on status of a Renew Item request awaiting decision or settlement.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'DateOfExpectedReply' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'DateOfExpectedReply' => array(
      'name' => t('Date Of Expected Reply'),
      'description' => t('Indicates when the sender expects to reply to a request to renew an Item. The sender does not guarantee that a reply will be made at the date/time specified.'),
      'type' => NCIP_DATETIME,
    ),
    'PersonalNameInformation' => array(
      'name' => t('Personal Name Information'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'StructuredPersonalUserName' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('UnstructuredPersonalUserName'),
          ),
        ),
        'UnstructuredPersonalUserName' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('StructuredPersonalUserName'),
          ),
        ),
      ),
    ),
    'PhysicalAddress' => array(
      'name' => t('Physical Address'),
      'description' => t('Designated geographical location recognized by a postal authority or other carrier, to which things may be delivered, as distinguished from Electronic Address.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'PhysicalAddressType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'StructuredAddress' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('UnstructuredAddress'),
          ),
        ),
        'UnstructuredAddress' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('StructuredAddress'),
          ),
        ),
      ),
    ),
    'PhysicalAddressType' => array(
      'name' => t('Physical Address Type'),
      'description' => t('Indicates whether the physical address is a street address or a postal address.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'PhysicalCondition' => array(
      'name' => t('Physical Condition'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'PhysicalConditionDetails' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'PhysicalConditionType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'PhysicalConditionDetails' => array(
      'name' => t('Physical Condition Details'),
      'description' => t('Provides details of the physical condition of an item, identified by Physical Condition Type.'),
      'type' => NCIP_STRING,
    ),
    'PhysicalConditionType' => array(
      'name' => t('Physical Condition Type'),
      'description' => t(''),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'PickupDate' => array(
      'name' => t('Pickup Date'),
      'description' => t('Indicates when a requested Item is available for pickup as a result of fulfillment of a User request (e.g., a hold or a stack retrieval).'),
      'type' => NCIP_DATETIME,
    ),
    'PickupExpiryDate' => array(
      'name' => t('Pickup Expiry Date'),
      'description' => t('Indicates when the physical Item being held for a User is to be returned to a regular circulation status (e.g., available for loan), because the Item has not been picked up (i.e., checked out) by the User in the time allocated. This situation would arise as a result of fulfillment of a request such as a hold or a stack retrieval.'),
      'type' => NCIP_DATETIME,
    ),
    'Problem' => array(
      'name' => t('Problem'),
      'description' => t('Indicates that the responding application cannot perform the service requested by the initiating application. This element is structured to enable a responding application to specify the location and nature of any processing errors in an initiation message.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'MessagingError' => array(
          'usage' => NCIP_USAGE_ONE_OR_MORE,
        ),
        'ProcessingError' => array(
          'usage' => NCIP_USAGE_ONE_OR_MORE,
        ),
      ),
    ),
    'ProcessingError' => array(
      'name' => t('Processing Error'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ProcessingErrorType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'ProcessingErrorElement' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'ProcessingErrorType' => array(
      'name' => t('Processing Error Type'),
      'description' => t(''),
      'type' => NCIP_CLOSED_SCHEME,
      'scheme' => 'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/generalprocessingerror.scm',
    ),
    'ProcessingErrorElement' => array(
      'name' => t('Processing Error Element'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ElementName' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ProcessingErrorElement' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ProcessingErrorValue'),
          ),
        ),
        'ProcessingErrorValue' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ProcessingErrorElement'),
          ),
        ),
      ),
    ),
    'ProcessingErrorValue' => array(
      'name' => t('Processing Error Value'),
      'description' => t('Contains data element value that caused the processing error.'),
      'type' => NCIP_STRING,
    ),
    'RelatedFiscalTransactionReferenceId' => array(
      'name' => t('Related Fiscal Transaction Reference Id'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'UniqueAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'FiscalTransactionIdentifierValue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'FiscalTransactionIdentifierValue' => array(
      'name' => t('Fiscal Transaction Identifier Value'),
      'description' => t('Gives value to be used to identify a fiscal transaction.'),
      'type' => NCIP_STRING,
    ),
    'ReminderLevel' => array(
      'name' => t('Reminder Level'),
      'description' => t('Provides a count of the number of times a reminder has been sent to a User.'),
      'type' => NCIP_POSITIVE_INTEGER,
    ),
    'RenewalCount' => array(
      'name' => t('Renewal Count'),
      'description' => t('Indicates the number of times the Item identified has been renewed. The Renewal Count for an initial check out is “0”.'),
      'type' => NCIP_NON_NEGATIVE_INTEGER,
    ),
    'RenewalNotPermitted' => array(
      'name' => t('Renewal Not Permitted'),
      'description' => t('Indicates that the initiating Agency will accept requests to renew the Item.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'RequestScopeType' => array(
      'name' => t('Request Scope Type'),
      'description' => t('Indicates the level of the Item description being requested.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'RequestStatusType' => array(
      'name' => t('Request Status Type'),
      'description' => t('Identifies the status of the request.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'RequestType' => array(
      'name' => t('Request Type'),
      'description' => t('Identifies the nature of the request.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'RequestedActionType' => array(
      'name' => t('Requested Action Type'),
      'description' => t('Identifies the action the responding Agency is requested to take on receipt of the Accept Item message.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'RequestedItem' => array(
      'name' => t('Requested Item'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'DatePlaced' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'HoldQueuePosition' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'PickupDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'PickupExpiryDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ReminderLevel' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'RequestStatusType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'RequestType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'UniqueItemId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('UniqueRequestId'),
          ),
        ),
        'UniqueRequestId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('UniqueItemId'),
          ),
        ),
      ),
    ),
    'RequestedItemsCount' => array(
      'name' => t('Requested Items Count'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'CirculationStatus' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ItemUseRestrictionType', 'RequestType'),
          ),
        ),
        'ItemUseRestrictionType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('CirculationStatus', 'RequestType'),
          ),
        ),
        'RequestType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('CirculationStatus', 'ItemUseRestrictionType'),
          ),
        ),
        'RequestedItemCountValue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'RequestedItemCountValue' => array(
      'name' => t('Requested Item Count Value'),
      'description' => t('Indicates the number of holds, reserves, or pages a User has in effect at any point in time for a Circulation Status or Item Use Restriction Type.'),
      'type' => NCIP_NON_NEGATIVE_INTEGER,
    ),
    'RequestedItemsDesired' => array(
      'name' => t('Requested Items Desired'),
      'description' => t('Indicates that the requester desires information about items requested by a specific User.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'RequiredFeeAmount' => array(
      'name' => t('Required Fee Amount'),
      'description' => t('Monetary value in a specified currency (i.e., Amount) that a User must pay for a requested service. See Amount for the definition of this complex element.'),
      'type' => $amount_type,
      'data elements' => $amount_data_elements,
    ),
    'RequiredItemUseRestrictionType' => array(
      'name' => t('Required Item Use Restriction Type'),
      'description' => t('Indicates that the Agency must receive an acknowledgement from the User that special rules or restrictions apply to the use of the Item. This requirement is carried, if applicable, in messages responding to the request, check out, or renewal of the Item.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'ResourceDesired' => array(
      'name' => t('Resource Desired'),
      'description' => t('Indicates that the requester desires an electronic resource be transmitted as part of a Check Out Item Response.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'ResponseHeader' => array(
      'name' => t('Response Header'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'FromAgencyAuthentication' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'FromAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'required' => TRUE,
        ),
        'FromSystemAuthentication' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'FromSystemId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ToAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'required' => TRUE,
        ),
        'ToSystemId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'RoutingInformation' => array(
      'name' => t('Routing Information'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'Destination' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'RequestType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'RoutingInstructions' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'UniqueUserId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'Destination' => array(
      'name' => t('Destination'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'BinNumber' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('Location'),
          ),
        ),
        'Location' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('BinNumber'),
          ),
        ),
      ),
    ),
    'BinNumber' => array(
      'name' => t('Bin Number'),
      'description' => t('Identifies an Agency-specific destination to which a physical item is to be routed following check in.'),
      'type' => NCIP_STRING,
    ),
    'RoutingInstructions' => array(
      'name' => t('Routing Instructions'),
      'description' => t('Provides instructions, such as special handling, for the routing of an Item within an Agency following check in.'),
      'type' => NCIP_STRING,
    ),
    'SecurityMarker' => array(
      'name' => t('Security Marker'),
      'description' => t('Specifies the type of security marker protecting an Item, including encryption of electronic resources.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'SensitizationFlag' => array(
      'name' => t('Sensitization Flag'),
      'description' => t('Indicates that an item should be desensitized and re-sensitized during the check out and check in procedures.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'ShippingInformation' => array(
      'name' => t('Shipping Information'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ElectronicAddress' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('PhysicalAddress'),
          ),
        ),
        'PhysicalAddress' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ElectronicAddress'),
          ),
        ),
        'ShippingInstructions' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('ShippingNote'),
          ),
        ),
        'ShippingNote' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('ShippingInstructions'),
          ),
        ),
      ),
    ),
    'ShippingInstructions' => array(
      'name' => t('Shipping Instructions'),
      'description' => t('Provides human-readable data pertaining to the shipment of an Item, such as the preferred carrier, packaging requirements, etc.'),
      'type' => NCIP_STRING,
    ),
    'ShippingNote' => array(
      'name' => t('Shipping Note'),
      'description' => t('Provides data additional to that provided in other data elements that comprise Shipping Information.'),
      'type' => NCIP_STRING,
    ),
    'StructuredAddress' => array(
      'name' => t('Structured Address'),
      'description' => t('Provides information about a physical address; structure permits parsing and allows for the component elements to be assembled in the order and format most suited to a given use.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'CareOf' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Country' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'District' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('Line1', 'Line2'),
          ),
        ),
        'HouseName' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('Line1', 'Line2'),
          ),
        ),
        'Line1' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('District', 'HouseName', 'LocationWithinBuilding', 'PostOfficeBox', 'Street'),
          ),
        ),
        'Line2' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('District', 'HouseName', 'LocationWithinBuilding', 'PostOfficeBox', 'Street'),
          ),
        ),
        'Locality' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'LocationWithinBuilding' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('Line1', 'Line2'),
          ),
        ),
        'PostOfficeBox' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('Line1', 'Line2'),
          ),
        ),
        'PostalCode' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Region' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Street' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('Line1', 'Line2'),
          ),
        ),
      ),
    ),
    'CareOf' => array(
      'name' => t('Care Of'),
      'description' => t('Gives name of a person, organization, or organizational unit to whose attention the addressed item is intended to be directed.'),
      'type' => NCIP_STRING,
    ),
    'Country' => array(
      'name' => t('Country'),
      'description' => t('Gives name of a country.'),
      'type' => NCIP_STRING,
    ),
    'District' => array(
      'name' => t('District'),
      'description' => t('Gives name of a sub-entity within a country such as a parish, county, or district.'),
      'type' => NCIP_STRING,
    ),
    'HouseName' => array(
      'name' => t('House Name'),
      'description' => t('Gives name of a house, building, or building complex.'),
      'type' => NCIP_STRING,
    ),
    'Line1' => array(
      'name' => t('Line 1'),
      'description' => t('Gives one or more of Location Within Building, House Name, Street, Post Office Box, and/or District when a lower level of granularity is not needed.'),
      'type' => NCIP_STRING,
    ),
    'Line2' => array(
      'name' => t('Line 2'),
      'description' => t('Gives one or more of House Name, Street, Post Office Box, and/or District when Line 1 is not sufficient.'),
      'type' => NCIP_STRING,
    ),
    'Locality' => array(
      'name' => t('Locality'),
      'description' => t('Gives name of a city, suburb, or postal area.'),
      'type' => NCIP_STRING,
    ),
    'LocationWithinBuilding' => array(
      'name' => t('Location Within Building'),
      'description' => t('Gives the name of a location within a building or building complex. Used to provide more detailed information when the Street element is not sufficient to identify a particular location.'),
      'type' => NCIP_STRING,
    ),
    'PostOfficeBox' => array(
      'name' => t('Post Office Box'),
      'description' => t('Gives the box number assigned by the postal authority.'),
      'type' => NCIP_STRING,
    ),
    'PostalCode' => array(
      'name' => t('Postal Code'),
      'description' => t('Gives the code assigned by the postal authority of a country that uniquely identifies an address or groups of addresses.'),
      'type' => NCIP_STRING,
    ),
    'Region' => array(
      'name' => t('Region'),
      'description' => t('Gives the name of an area within a country such as a state, province, department, or prefecture.'),
      'type' => NCIP_STRING,
    ),
    'Street' => array(
      'name' => t('Street'),
      'description' => t('Gives the name of the location of a house, building or building complex within a city or district.'),
      'type' => NCIP_STRING,
    ),
    'StructuredHoldingsData' => array(
      'name' => t('Structured Holdings Data'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'HoldingsChronology' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('HoldingsEnumeration'),
          ),
        ),
        'HoldingsEnumeration' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('HoldingsChronology'),
          ),
        ),
      ),
    ),
    'HoldingsChronology' => array(
      'name' => t('Holdings Chronology'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ChronologyLevelInstance' => array(
          'usage' => NCIP_USAGE_ONE_OR_MORE,
        ),
      ),
    ),
    'ChronologyLevelInstance' => array(
      'name' => t('Chronology Level Instance'),
      'description' => t('Provides structured information that describes the hierarchical relationship of the specific part held by an Agency to a whole Item, expressed in chronological terms.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ChronologyCaption' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('ChronologyLevel'),
          ),
        ),
        'ChronologyLevel' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('ChronologyCaption'),
          ),
        ),
        'ChronologyValue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'ChronologyCaption' => array(
      'name' => t('Chronology Caption'),
      'description' => t('Provides a descriptive term identifying the Chronology Level Instance.'),
      'type' => NCIP_STRING,
    ),
    'ChronologyLevel' => array(
      'name' => t('Chronology Level'),
      'description' => t('Indicates the level of a Chronology Level Instance in a sequence of Chronology Level Instances within Holdings Information.'),
      'type' => NCIP_POSITIVE_INTEGER,
    ),
    'ChronologyValue' => array(
      'name' => t('Chronology Value'),
      'description' => t('Provides the value for the Chronology Level Instance.'),
      'type' => NCIP_STRING,
    ),
    'HoldingsEnumeration' => array(
      'name' => t('Holdings Enumeration'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'EnumerationLevelInstance' => array(
          'usage' => NCIP_USAGE_ONE_OR_MORE,
        ),
      ),
    ),
    'EnumerationLevelInstance' => array(
      'name' => t('Enumeration Level Instance'),
      'description' => t('Provides structured information that describes the hierarchical relationship of the specific part held by an Agency to a whole Item, expressed in chronological terms.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'EnumerationCaption' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('EnumerationLevel'),
          ),
        ),
        'EnumerationLevel' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
          'conditions' => array(
            NCIP_USAGE_AND_OR => array('EnumerationCaption'),
          ),
        ),
        'EnumerationValue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'EnumerationCaption' => array(
      'name' => t('Enumeration Caption'),
      'description' => t('Provides a descriptive term identifying the Enumeration Level Instance.'),
      'type' => NCIP_STRING,
    ),
    'EnumerationLevel' => array(
      'name' => t('Enumeration Level'),
      'description' => t('Indicates the level of a Enumeration Level Instance in a sequence of Enumeration Level Instances within Holdings Information.'),
      'type' => NCIP_POSITIVE_INTEGER,
    ),
    'EnumerationValue' => array(
      'name' => t('Enumeration Value'),
      'description' => t('Provides the value for the Enumeration Level Instance.'),
      'type' => NCIP_STRING,
    ),
    'StructuredPersonalUserName' => array(
      'name' => t('Structured Personal UserName'),
      'description' => t('provides components of a personal name; structure permits parsing and allows for the component elements to be manipulated in the order and format most suited to a given use.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'GivenName' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Initials' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Prefix' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Suffix' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'Surname' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'GivenName' => array(
      'name' => t('Given Name'),
      'description' => t('Gives simple, double, or compound name that distinguishes an individual within a family carrying a common name. Christian names and forenames are included as given names.'),
      'type' => NCIP_STRING,
    ),
    'Initials' => array(
      'name' => t('Initials'),
      'description' => t('Provides the initial letter or letters of the given name.'),
      'type' => NCIP_STRING,
    ),
    'Prefix' => array(
      'name' => t('Prefix'),
      'description' => t('Gives name element, placed before other personal name elements such as surname and given names that, in association with these elements, serves to identify an individual, but does not form an integral part of a name.'),
      'type' => NCIP_STRING,
    ),
    'Suffix' => array(
      'name' => t('Suffix'),
      'description' => t('Denotes the family relationship, initials of an academic degree, or initials denoting membership in an organization that, in conjunction with a surname and a given name or names, serves to identify an individual, but does not form an integral part of a name.'),
      'type' => NCIP_STRING,
    ),
    'Surname' => array(
      'name' => t('Surname'),
      'description' => t('gives surname or family name of the User. A surname may consist of a single word, or may be compound, that is, joined by a hyphen or contain more than one word.'),
      'type' => NCIP_STRING,
    ),
    'ToAgencyId' => array(
      'name' => t('To Agency Id'),
      'description' => t('Unique identification of the Agency (i.e., Unique Agency Id) to which a message is sent.'),
      'type' => $unique_agency_id_type,
      'data elements' => $unique_agency_id_data_elements,
    ),
    'ToSystemId' => array(
      'name' => t('To System Id'),
      'description' => t('Identifies the system to which a message is sent.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'UniqueAgencyId' => array(
      'name' => t('Unique Agency Id'),
      'description' => t('Identifies an Agency.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'UniqueBibliographicId' => array(
      'name' => t('Unique Bibliographic Id'),
      'description' => t('Unique identification of a bibliographic item by either the Bibliographic Item Id or the Bibliographic Record Id.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'BibliographicItemId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('BibliographicRecordId'),
          ),
        ),
        'BibliographicRecordId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('BibliographicItemId'),
          ),
        ),
      ),
    ),
    'UniqueItemId' => array(
      'name' => t('Unique Item Id'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'UniqueAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'ItemIdentifierValue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'ItemIdentifierValue' => array(
      'name' => t('Item Identifier Value'),
      'description' => t('Identifies, in context of a specific Agency, the Item involved in a circulation transaction. Identifier may take the form of a barcode, a record number, an accession number, etc.'),
      'type' => NCIP_STRING,
    ),
    'UniqueRequestId' => array(
      'name' => t('Unique Request Id'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'RequestIdentifierValue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'UniqueAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'RequestIdentifierValue' => array(
      'name' => t('Request Identifier Value'),
      'description' => t('Provides the value of a specific request identifier.'),
      'type' => NCIP_STRING,
    ),
    'UniqueUserId' => array(
      'name' => t('Unique User Id'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'UniqueAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'UserIdentifierValue' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'UserIdentifierValue' => array(
      'name' => t('User Identifier Value'),
      'description' => t('Specifies a User within an Agency.'),
      'type' => NCIP_STRING,
    ),
    'UnstructuredAddress' => array(
      'name' => t('Unstructured Address'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'UnstructuredAddressData' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'UnstructuredAddressType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'UnstructuredAddressData' => array(
      'name' => t('Unstructured Address Data'),
      'description' => t('Gives a complete physical address in a single string. String may contain simple processing information such as line breaks to be used when formatting information for output, such as a screen display or address label.'),
      'type' => NCIP_STRING,
    ),
    'UnstructuredAddressType' => array(
      'name' => t('Unstructured Address Type'),
      'description' => t('Identifies the type of formatting present in Unstructured Address Data.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'UnstructuredHoldingsData' => array(
      'name' => t('Unstructured Holdings Data'),
      'description' => t('Gives holdings information in a string. This element could carry, for instance, the non-structured “volume-issue” data supplied by an ISO ILL Protocol-based request.'),
      'type' => NCIP_STRING,
    ),
    'UnstructuredPersonalUserName' => array(
      'name' => t('Unstructured Personal User Name'),
      'description' => t('Gives the complete personal name of a User in a single string.'),
      'type' => NCIP_STRING,
    ),
    'UserAddressInformation' => array(
      'name' => t('User Address Information'),
      'description' => t('Information about the place to which messages and Items destined for a User are directed.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ElectronicAddress' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('PhysicalAddress'),
          ),
        ),
        'PhysicalAddress' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('ElectronicAddress'),
          ),
        ),
        'ValidFromDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ValidToDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'UserElementType' => array(
      'name' => t('User Element Type'),
      'description' => t('Identifies all the elements of the User Object on which queries (i.e., lookups) can be made.'),
      'type' => NCIP_CLOSED_SCHEME,
      'scheme' => 'http://www.niso.org/ncip/v1_0/schemes/userelementtype/userelementtype.scm',
    ),
    'UserFiscalAccount' => array(
      'name' => t('User Fiscal Account'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AccountBalance' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'AccountDetails' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
      ),
    ),
    'AccountBalance' => array(
      'name' => t('Account Balance'),
      'description' => t('Monetary value in a specified currency (i.e., Amount) that is the balance in a User\'s account.'),
      'type' => $amount_type,
      'data elements' => $amount_data_elements,
    ),
    'AccountDetails' => array(
      'name' => t('Account Details'),
      'description' => t('Details of a fiscal transaction affecting the User\'s account.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AccrualDate' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'FiscalTransactionInformation' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'AccrualDate' => array(
      'name' => t('Accrual Date'),
      'description' => t('Indicates when the amount specified in Fiscal Transaction Information was accrued to the User\'s account.'),
      'type' => NCIP_DATETIME,
    ),
    'UserFiscalAccountDesired' => array(
      'name' => t('User Fiscal Account Desired'),
      'description' => t('Indicates that the requester desires information about the User\'s fiscal account.'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'UserLanguage' => array(
      'name' => t('User Language'),
      'description' => t('Identifies one or more languages, in order of preference, in which the User wishes to receive written communication.'),
      'type' => NCIP_CLOSED_SCHEME,
      'scheme' => 'http://lcweb.loc.gov/standards/iso639-2/termcodes.html',
    ),
    'UserNoticeDetails' => array(
      'name' => t('User Notice Details'),
      'description' => t('A message to be displayed or otherwise sent to a User.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'NoticeContent' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'NoticeType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'NoticeItem' => array(
          'usage' => NCIP_USAGE_ONE_OR_MORE,
          'conditions' => array(
            NCIP_USAGE_OR => array('UserFiscalAccount', 'UserPrivilege'),
          ),
        ),
        'UserFiscalAccount' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('NoticeItem', 'UserPrivilege'),
          ),
        ),
        'UserPrivilege' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
          'conditions' => array(
            NCIP_USAGE_OR => array('NoticeItem', 'UserFiscalAccount'),
          ),
        ),
      ),
    ),
    'UserOptionalFields' => array(
      'name' => t('User Optional Fields'),
      'description' => t('Used in update response messages. Carries data elements describing the User Object as requested in the initiation message of an update service. Also used in the initiation message of a notification service to describe the User Object.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'BlockOrTrap' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'DateOfBirth' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'NameInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'UserAddressInformation' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'UserLanguage' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'UserPrivilege' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'VisibleUserId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
      ),
    ),
    'UserPrivilege' => array(
      'name' => t('User Privilege'),
      'description' => t('Contains details concerning a specified privilege accorded to a User by a specific Agency, such as fees charged for the privilege, the status of the privilege, or the dates during which the privilege is in force.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'AgencyUserPrivilegeType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'UniqueAgencyId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'UserPrivilegeDescription' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'UserPrivilegeFee' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'UserPrivilegeStatus' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ValidFromDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'ValidToDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'UserPrivilegeDescription' => array(
      'name' => t('User Privilege Description'),
      'description' => t('Describes a specific user privilege.'),
      'type' => NCIP_STRING,
    ),
    'UserPrivilegeFee' => array(
      'name' => t('User Privilege Fee'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'Amount' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'PaymentMethodType' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'PaymentMethodType' => array(
      'name' => t('Payment Method Type'),
      'description' => t('Indicates the method of payment (actual or proposed) for a specified user privilege fee.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'UserPrivilegeStatus' => array(
      'name' => t('User Privilege Status'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'DateOfUserPrivilegeStatus' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'UserPrivilegeStatusType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'DateOfUserPrivilegeStatus' => array(
      'name' => t('Date Of User Privilege Status'),
      'description' => t('Indicates when the current user privilege status came into effect.'),
      'type' => NCIP_DATETIME,
    ),
    'UserPrivilegeStatusType' => array(
      'name' => t('User Privilege Status Type'),
      'description' => t('Indicates the current status of a user privilege.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
    'UserTransaction' => array(
      'name' => t('User Transaction'),
      'description' => t('Information concerning a User\'s loan or request, including the Item(s) that a User has on loan or on request, together with the Agency to which the Item(s) belong or to which the request was sent.'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'LoanedItem' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
        'RequestedItem' => array(
          'usage' => NCIP_USAGE_ZERO_OR_MORE,
        ),
      ),
    ),
    'ValidFromDate' => array(
      'name' => t('Valid From Date'),
      'description' => t('Indicates when data, such as an address, a user privilege, a block or a trap, became valid or will become valid. If this element is not provided, one may assume that the data is valid, unless otherwise specified in Valid To Date.'),
      'type' => NCIP_DATETIME,
    ),
    'ValidToDate' => array(
      'name' => t('Valid To Date'),
      'description' => t('Indicates when data, such as an address, a user privilege, a block or a trap, will no longer be valid or was no longer valid. If this element is not provided, one may assume that the data is valid, unless otherwise specified in Valid From Date.'),
      'type' => NCIP_DATETIME,
    ),
    'Version' => array(
      'name' => t('Version'),
      'description' => t('Identifies the NCIP version to which the message belongs.'),
      'type' => NCIP_STRING,
    ),
    'VersionSupported' => array(
      'name' => t('Version Supported'),
      'description' => t('Identifies an NCIP version supported by an application.'),
      'type' => NCIP_STRING,
    ),
    'VisibleUserId' => array(
      'name' => t('Visible User Id'),
      'description' => t(''),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'UniqueAgencyId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'VisibleUserIdentifier' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'VisibleUserIdentifierType' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'VisibleUserIdentifier' => array(
      'name' => t('Visible User Identifier'),
      'description' => t('Gives a barcode or another eye-readable identifier affixed to an identification card assigned to a User. The value of the identifier may be identical to the User Identifier Value used in the Unique User Id.'),
      'type' => NCIP_STRING,
    ),
    'VisibleUserIdentifierType' => array(
      'name' => t('Visible User Identifier Type'),
      'description' => t('Identifies the type of Visible User Identifier used.'),
      'type' => NCIP_OPEN_SCHEME,
    ),
  );
  return $data_elements;
}

/**
 * Implements hook_ncip_schemes().
 */
function ncip_ncip_schemes(&$schemes) {
  $schemes['ncip'] = array(
    'http://www.niso.org/ncip/v1_0/schemes/agencyelementtype/agencyelementtype.scm' => array(
      'name' => 'NCIP Agency Element Type Scheme',
      'data elements' => array('AgencyElementType'),
      'type' => NCIP_CLOSED_SCHEME,
      'values' => array(
        'Agency Address Information',
        'Agency User Privilege Type',
        'Application Profile Supported Type',
        'Authentication Prompt',
        'Consortium Agreement',
        'Organization Name Information',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/itemelementtype/itemelementtype.scm' => array(
      'name' => 'NCIP Item Element Type Scheme',
      'data elements' => array('ItemElementType'),
      'type' => NCIP_CLOSED_SCHEME,
      'values' => array(
        'Bibliographic Description',
        'Circulation Status',
        'Electronic Resource',
        'Hold Queue Length',
        'Item Description',
        'Item Use Restriction Type',
        'Location',
        'Physical Condition',
        'Security Marker',
        'Sensitization Flag',
      ),
    ),
    'http://lcweb.loc.gov/standards/iso639-2/bibcodes.html' => array(
      'name' => 'ISO 639-2 Alpha-3 Bibliographic Codes',
      'data elements' => array('Language'),
      'values' => array(),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/messagingerrortype/messagingerrortype.scm' => array(
      'name' => 'NCIP Messaging Error Type Scheme',
      'data elements' => array('MessagingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'values' => array(
        'Invalid Message Syntax Error',
        'Protocol Error',
        'Unknown Element',
        'Unknown Scheme',
        'Unknown Service',
        'Unknown Value From Known Scheme',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/generalprocessingerror.scm' => array(
      'name' => 'NCIP General Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service type' => array(NCIP_LOOKUP_SERVICE, NCIP_UPDATE_SERVICE),
      'values' => array(
        'Agency Authentication Failed',
        'Invalid Amount',
        'Invalid Date',
        'Needed Data Missing',
        'System Authentication Failed',
        'Temporary Processing Failure',
        'Unauthorized Combination Of Element Values For Agency',
        'Unauthorized Combination Of Element Values For System',
        'Unauthorized Service For Agency',
        'Unauthorized Service For System',
        'Unknown Agency',
        'Unknown System',
        'Unsupported Service',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/acceptitemprocessingerror.scm' => array(
      'name' => 'NCIP Accept Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('AcceptItem'),
      'values' => array(
        'Cannot Accept Item',
        'Cannot Guarantee Restrictions On Use',
        'Element Rule Violated',
        'Unknown Item',
        'Unknown Request',
        'Unknown User',
        'User Ineligible To Check Out This Item',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/authenticateuserprocessingerror.scm' => array(
      'name' => 'NCIP Authenticate User Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('AuthenticateUser'),
      'values' => array(
        'Unknown User',
        'User Authentication Failed',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/cancelrecallitemprocessingerror.scm' => array(
      'name' => 'NCIP Cancel Recall Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('CancelRecallItem'),
      'values' => array(
        'Item Not Checked Out',
        'Recall Cannot Be Cancelled At This Time',
        'Unknown Item',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/cancelrequestitemprocessingerror.scm' => array(
      'name' => 'NCIP Cancel Request Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('CancelRequestItem'),
      'values' => array(
        'Element Rule Violated',
        'Request Already Processed',
        'Unknown Item',
        'Unknown Request',
        'Unknown User',
        'User Authentication Failed',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/checkinitemprocessingerror.scm' => array(
      'name' => 'NCIP Check In Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('CheckInItem'),
      'values' => array(
        'Item Not Checked Out',
        'Unknown Item',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/
checkoutitemprocessingerror.scm' => array(
      'name' => 'NCIP Check Out Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('CheckOutItem'),
      'values' => array(
        'Item Does Not Circulate',
        'Maximum Check Outs Exceeded',
        'Resource Cannot Be Provided',
        'Unknown Item',
        'Unknown Request',
        'Unknown User',
        'Unsupported Shipping Address Type',
        'User Authentication Failed',
        'User Blocked',
        'User Ineligible To Check Out This Item',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/createagencyprocessingerror.scm' => array(
      'name' => 'NCIP Create Agency Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('CreateAgency'),
      'values' => array(
        'Duplicate Agency',
        'Element Rule Violated',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/createitemprocessingerror.scm' => array(
      'name' => 'NCIP Create Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('CreateItem'),
      'values' => array(
        'Duplicate Item',
        'Element Rule Violated',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/createuserprocessingerror.scm' => array(
      'name' => 'NCIP Create User Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('CreateUser'),
      'values' => array(
        'Duplicate User',
        'Element Rule Violated',
        'User Authentication Failed',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/createuserfiscaltransactionprocessingerror.scm' => array(
      'name' => 'NCIP Create User Fiscal Transaction Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('CreateUserFiscalTransaction'),
      'values' => array(
        'Account Access Denied',
        'Duplicate Fiscal Transaction',
        'Element Rule Violated',
        'Unable To Add Element',
        'Unknown Item',
        'Unknown Request',
        'Unknown User',
        'User Access Denied',
        'User Authentication Failed',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/lookupagencyprocessingerror.scm' => array(
      'name' => 'NCIP Lookup Agency Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('LookupAgency'),
      'values' => array(
        'Agency Access Denied',
        'Element Rule Violated',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/lookupitemprocessingerror.scm' => array(
      'name' => 'NCIP Lookup Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('LookupItem'),
      'values' => array(
        'Element Rule Violated',
        'Item Access Denied',
        'Non-Unique Item',
        'Unknown Item',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/lookupuserprocessingerror.scm' => array(
      'name' => 'NCIP Lookup User Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('LookupUser'),
      'values' => array(
        'Element Rule Violated',
        'Non-Unique User',
        'Unknown User',
        'User Access Denied',
        'User Authentication Failed',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/recallitemprocessingerror.scm' => array(
      'name' => 'NCIP Recall Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('RecallItem'),
      'values' => array(
        'Item Cannot Be Recalled',
        'Item Not Checked Out',
        'Unknown Item',
        'Unsupported Shipping Address Type',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/renewitemprocessingerror.scm' => array(
      'name' => 'NCIP Renew Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('RenewItem'),
      'values' => array(
        'Item Not Checked Out',
        'Item Not Renewable',
        'Maximum Renewals Exceeded',
        'Renewal Not Allowed - Item Has Outstanding Requests',
        'Unknown Item',
        'Unknown User',
        'User Authentication Failed',
        'User Blocked',
        'User Ineligible To Renew This Item',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/reportcirculationstatuschangeprocessingerror.scm' => array(
      'name' => 'NCIP Report Circulation Status Change Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('ReportCirculationStatusChange'),
      'values' => array(
        'Element Rule Violated',
        'Item Not Checked Out To This User',
        'Unknown Item',
        'Unknown User',
        'User Authentication Failed',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/requestitemprocessingerror.scm' => array(
      'name' => 'NCIP Request Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('RequestItem'),
      'values' => array(
        'Duplicate Request',
        'Element Rule Violated',
        'Item Does Not Circulate',
        'Item Not Available By Need Before Date',
        'Unknown Item',
        'Unknown User',
        'User Authentication Failed',
        'User Blocked',
        'User Ineligible To Request This Item',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/sendusernoticeprocessingerror.scm' => array(
      'name' => 'NCIP Send User Notice Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('SendUserNotice'),
      'values' => array(
        'Element Rule Violated',
        'Item Not Checked Out',
        'Unknown Item',
        'Unknown User',
        'Unsupported Notice',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/undocheckoutitemprocessingerror.scm' => array(
      'name' => 'NCIP Undo Check Out Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('UndoCheckOutItem'),
      'values' => array(
        'Unknown Item',
        'Unknown User',
        'User Authentication Failed',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/updateagencyprocessingerror.scm' => array(
      'name' => 'NCIP Update Agency Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('UpdateAgency'),
      'values' => array(
        'Agency Access Denied',
        'Element Rule Violated',
        'Unable To Add Element',
        'Unable To Delete Element',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/updatecirculationstatusprocessingerror.scm' => array(
      'name' => 'NCIP Update Circulation Status Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('UpdateCirculationStatus'),
      'values' => array(
        'Element Rule Violated',
        'Item Access Denied',
        'Unable To Add Element',
        'Unable To Delete Element',
        'Unknown Item',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/updateitemprocessingerror.scm' => array(
      'name' => 'NCIP Update Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('UpdateItem'),
      'values' => array(
        'Element Rule Violated',
        'Item Access Denied',
        'Unable To Add Element',
        'Unable To Delete Element',
        'Unknown Item',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/updaterequestitemprocessingerror.scm' => array(
      'name' => 'NCIP Update Request Item Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('UpdateRequestItem'),
      'values' => array(
        'Element Rule Violated',
        'Item Access Denied',
        'Unable To Add Element',
        'Unable To Delete Element',
        'Unknown Item',
        'Unknown Request',
        'Unknown User',
        'User Authentication Failed',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/processingerrortype/updateuserprocessingerror.scm' => array(
      'name' => 'NCIP Update User Processing Error Scheme',
      'data elements' => array('ProcessingErrorType'),
      'type' => NCIP_CLOSED_SCHEME,
      'service' => array('UpdateUser'),
      'values' => array(
        'Element Rule Violated',
        'Unable To Add Element',
        'Unable To Delete Element',
        'Unknown User',
        'User Access Denied',
        'User Authentication Failed',
      ),
    ),
    'http://www.niso.org/ncip/v1_0/schemes/userelementtype/userelementtype.scm' => array(
      'name' => 'NCIP User Element Type Scheme',
      'data elements' => array('UserElementType'),
      'type' => NCIP_CLOSED_SCHEME,
      'values' => array(
        'Authentication Input',
        'Block Or Trap',
        'Date Of Birth',
        'Name Information',
        'User Address Information',
        'User Language',
        'User Privilege',
        'Visible User Id',
      ),
    ),
    'http://lcweb.loc.gov/standards/iso639-2/termcodes.html' => array(
      'name' => 'ISO 639-2 Terminological Codes',
      'data elements' => array('UserLanguage'),
      'type' => NCIP_CLOSED_SCHEME,
      'values' => array(),
    ),
    //    '' => array( //      'name' => '', //      'data elements' => array(''), //      'service' => array(''), //      'values' => array( //        '', //      ), //    ),
  );

  return $schemes;
}
