<?php
/**
 * @file
 * XC profile functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Implementation of hook_ncip_info()
 */
function xc_ncip_info() {
  $info['xc'] = array(
    'name' => 'XC NCIP Implementation',
    'description' => t('XC NCIP provider'),
    'required' => array('ncip'),
  );
  return $info;
}

/**
 * Implementation of hook_ncip_services()
 */
function xc_ncip_services(&$services) {
  $services['xc'] = array(
    'XCLookupUserService' => array(
      'name' => t('XC Lookup User Service'),
      'description' => t('This service is used to look up information about a user. It is almost identical to the Lookup User Service, but allows additional information to be queried which is not exposed by the NCIP standard. The only required parameter is the user\'s unique identifier.'),
      'type' => NCIP_LOOKUP_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'XCLookupUser' => array(
            'name' => t('XC Lookup User'),
            'data elements' => array(
              'required' => array(
                'UniqueUserId' => array(),
              ),
              'optional' => array(
                'UserElementType' => array(
                  'repeatable' => TRUE,
                ),
                'LoanedItemsDesired' => array(),
                'RequestedItemsDesired' => array(),
                'RecalledItemsDesired' => array(),
                'UserFiscalAccountDesired' => array(),
                'MessagesDesired ' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'XCLookupUserResponse' => array(
            'name' => t('XC Lookup User Response'),
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
              'requested' => array(
                'LoanedItem' => array(),
                'RequestedItem' => array(),
                'RecalledItem' => array(),
                'UserFiscalAccount' => array(),
                'RecalledItem' => array(),
              ),
            ),
          ),
        ),
      ),
    ),
    'XCGetAvailabilityService' => array(
      'name' => t('XC Get Availability Service'),
      'description' => t('This service is used to check the availability of a list of items.  The XC Get Availability request must contain one or more UniqueItemId elements which contain the item IDs of the items whose availability is being queried.'),
      'type' => NCIP_LOOKUP_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'XCGetAvailability' => array(
            'name' => t('XC Get Availability'),
            'data elements' => array(
              'required' => array(
                'UniqueItemId' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'XCGetAvailabilityResponse' => array(
            'name' => t('XC Get Availability Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('XCItemAvailability'),
                  ),
                ),
                'XCItemAvailability' => array(
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
    'XCOpenURLRequestItemService' => array(
      'name' => t('XC Open URL Request Item Service'),
      'description' => t('This service is used to place a request for an item on behalf of a user.  The user who is requesting the item must have authenticated before invoking this service, otherwise an error will be generated.  The NCIP request calling this service must provide either a UniqueUserId element or AuthenticationInput elements in the format described for the Authenticate User Service for the user placing the request.  It must also provide an open URL query string identifying the item being requested.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'XCOpenURLRequestItem' => array(
            'name' => t('XC Open URL Request Item'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'OpenUrlQueryString' => array(),
              ),
              'optional' => array(
                'PickupExpiryDate' => array(),
                'ShippingInformation' => array(),
                'ShippingInstructions' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'XCOpenURLRequestItemResponse' => array(
            'name' => t('XC Open URL Request Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('XCItemAvailability'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('Problem'),
                  ),
                ),
                'UniqueRequestId' => array(
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
    'XCOpenURLRenewItemService' => array(
      'name' => t('XC Open URL Renew Item Service'),
      'description' => t('This service is used to renew an item which has already been checked out to the requesting user.'),
      'type' => NCIP_UPDATE_SERVICE,
      'messages' => array(
        NCIP_INITIATION_MESSAGE => array(
          'XCOpenURLRenewItem' => array(
            'name' => t('XC Open URL Renew Item'),
            'data elements' => array(
              'required' => array(
                'AuthenticationInput' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('UniqueUserId'),
                  ),
                ),
                'UniqueUserId' => array(
                  'conditions' => array(
                    NCIP_USAGE_OR => array('AuthenticationInput'),
                  ),
                ),
                'OpenUrlQueryString' => array(),
              ),
              'optional' => array(
                'UniqueItemId' => array(),
                'DesiredDateForReturn' => array(),
                'DesiredDateDue' => array(),
              ),
            ),
          ),
        ),
        NCIP_RESPONSE_MESSAGE => array(
          'XCOpenURLRenewItemResponse' => array(
            'name' => t('XC Open URL Renew Item Response'),
            'data elements' => array(
              'required' => array(
                'Problem' => array(),
              ),
              'requested' => array(
                'DateDue' => array(),
                'DesiredDateForReturn' => array(),
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
 * Implementation of hook_ncip_data_elements()
 */
function xc_ncip_data_elements(&$data_elements) {
  $data_elements['xc'] = array(
    'LoanedItemsDesired' => array(
      'name' => t('Loaned Items Desired'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'RequestedItemsDesired' => array(
      'name' => t('Requested Items Desired'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'RecalledItemsDesired' => array(
      'name' => t('Recalled Items Desired'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'UserFiscalAccountDesired' => array(
      'name' => t('User Fiscal Account Desired'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'MessagesDesired' => array(
      'name' => t('Messages Desired'),
      'type' => NCIP_EMPTY_ELEMENT,
    ),
    'RecalledItem' => array(
      'name' => t('Recalled Item'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'DatePlaced' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'PickupDate' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
        'UniqueItemId' => array(
          'usage' => NCIP_USAGE_ZERO_OR_ONE,
        ),
      ),
    ),
    'XCItemAvailability' => array(
      'name' => t('XC Item Availability'),
      'type' => NCIP_COMPLEX_ELEMENT,
      'data elements' => array(
        'ItemId' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
        'Availability' => array(
          'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
        ),
      ),
    ),
    'ItemId' => array(
      'name' => t('Item Id'),
      'type' => NCIP_STRING,
    ),
    'Availability' => array(
      'name' => t('Availability'),
      'type' => NCIP_STRING,
    ),
    'OpenUrlQueryString' => array(
      'name' => t('Open Url Query String'),
      'type' => NCIP_STRING,
    ),
    'ErrorCode' => array(
      'name' => t('Error Code'),
      'type' => NCIP_STRING,
    ),
    'ErrorMessage' => array(
      'name' => t('Error Message'),
      'type' => NCIP_STRING,
    ),
  );

  $data_elements['Problem']['data elements']['ErrorCode'] = array(
    'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
  );

  $data_elements['Problem']['data elements']['ErrorMessage'] = array(
    'usage' => NCIP_USAGE_ONE_AND_ONLY_ONE,
  );

  return $data_elements;
}

/**
 * Implementation of hook_ncip_schemes()
 */
function xc_ncip_schemes(&$schemes) {
  $schemes['xc'] = array();
  return $schemes;
}
