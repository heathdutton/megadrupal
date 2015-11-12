<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * All direct API communications.
 */

/**
 * Ask, if not already, a new session ID from OpenX
 *
 * @param $op
 *   String. The XML-RPC operation that should be called.
 * @param ...
 *   Any. All arguments that will be passed to the XML-RPC call.
 * @return
 *   String. The full session ID, used on every further communication.
 */
function _openx_manager_call($op) {
  // Get all function arguments
  $args = func_get_args();

  // Replace the first two by the URL and the XML-RPC operation and session ID
  array_shift($args);
  array_unshift($args, _openx_manager_session());
  array_unshift($args, $op);
  array_unshift($args, variable_get('openx_manager_server', '') . _openx_manager_call_file($op));

  // Call the XML-RPC
  $result = call_user_func_array('xmlrpc', $args);

  // Show errors, if any
  if ($error_number = xmlrpc_errno()) {
    drupal_set_message($op .': '. $error_number .': '. xmlrpc_error_msg(), 'error');
  }

  return $result;
}

/**
 * Find out what file should be called.
 *
 * @param $op
 *   String. The XML-RPC operation that should be called.
 * @return
 *   String. The full session ID, used on every further communication.
 */
function _openx_manager_call_file($op) {
  switch ($op) {
    case 'addAdvertiser':
    case 'deleteAdvertiser':
    case 'getAdvertiser':
    case 'getAdvertiserListByAgencyId':
    case 'modifyAdvertiser':
    case 'advertiserBannerStatistics':
    case 'advertiserCampaignStatistics':
    case 'advertiserDailyStatistics':
    case 'advertiserPublisherStatistics':
    case 'advertiserZoneStatistics':
      return '/www/api/v1/xmlrpc/AdvertiserXmlRpcService.php';
    case 'addBanner':
    case 'deleteBanner':
    case 'getBanner':
    case 'getBannerListByCampaignId';
    case 'modifyBanner':
    case 'bannerDailyStatistics':
    case 'bannerListByCampaignId':
    case 'bannerPublisherStatistics':
    case 'bannerZoneStatistics':
      return '/www/api/v1/xmlrpc/BannerXmlRpcService.php';
    case 'addCampaign':
    case 'deleteCampaign':
    case 'getCampaign':
    case 'getCampaignListByAdvertiserId':
    case 'modifyCampaign':
    case 'campaignBannerStatistics':
    case 'campaignDailyStatistics':
    case 'campaignListByAdvertiserId':
    case 'campaignPublisherStatistics':
    case 'campaignZoneStatistics':
      return '/www/api/v1/xmlrpc/CampaignXmlRpcService.php';
    case 'addAgency':
    case 'deleteAgency':
    case 'getAgency':
    case 'getAgencyList':
    case 'modifyAgency':
    case 'agencyAdvertiserStatistics':
    case 'agencyBannerStatistics':
    case 'agencyCampaignStatistics':
    case 'agencyDailyStatistics':
    case 'agencyPublisherStatistics':
    case 'agencyZoneStatistics':
      return '/www/api/v1/xmlrpc/AgencyXmlRpcService.php';
    case 'addPublisher':
    case 'deletePublisher':
    case 'getPublisher':
    case 'getPublisherListByAgencyId':
    case 'modifyPublisher':
    case 'publisherAdvertiserStatistics':
    case 'publisherBannerStatistics':
    case 'publisherCampaignStatistics':
    case 'publisherDailyStatistics':
    case 'publisherZoneStatistics':
      return '/www/api/v1/xmlrpc/PublisherXmlRpcService.php';
    case 'addUser':
    case 'deleteUser':
    case 'getUser':
    case 'getUserListByAccountId':
    case 'modifyUser':
    case 'updateSsoUserId':
    case 'updateUserEmailBySsoId':
      return '/www/api/v1/xmlrpc/UserXmlRpcService.php';
    case 'addZone':
    case 'deleteZone':
    case 'generateTags':
    case 'getZone':
    case 'getZoneListByPublisherId':
    case 'linkBanner':
    case 'linkCampaign':
    case 'modifyZone':
    case 'unlinkBanner':
    case 'unlinkCampaign':
    case 'zoneAdvertiserStatistics':
    case 'zoneBannerStatistics':
    case 'zoneCampaignStatistics':
    case 'zoneDailyStatistics':
      return '/www/api/v1/xmlrpc/ZoneXmlRpcService.php';
  }
}
