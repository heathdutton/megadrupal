<?php

/**
 * @file
 * Cablecast API
 */

/**
 * Cablecast server query
 *
 * @param $method
 * @param array $data
 * @return mixed
 */
function cablecast_api_server_query($method, $data) {
  
  try {
    $client = cablecast_api_get_client();
    //add username and password of $data
    $data['username'] = variable_get('cablecast_api_server_username', NULL);
    $data['password'] = variable_get('cablecast_api_server_password', NULL);
    // perform query
    $return = $client->$method($data);

    return $return;

  } // end try

  catch (SoapFault $fault) {
    trigger_error("The following SOAP Error was generated, SOAP Fault: (faultcode: {$fault->faultcode}, faultstring: {$fault->faultstring})", E_USER_ERROR);
  }// end catch
}

/**
 * Creates a new Producer on Cablecast
 *
 * @param $data
 *
 * @return $result
 */
function cablecast_api_add_producer($user) {
  /*
      <LocationID>int</LocationID>
      <ProducerName>string</ProducerName>
      <Phone1>string</Phone1>
      <Phone2>string</Phone2>
      <Address>string</Address>
      <Notes>string</Notes>
      <Active>boolean</Active>
      <ContactName>string</ContactName>
      <Email>string</Email>
      <Website>string</Website>
      <username>string</username>
      <password>string</password>
  */

   // setup data array for SOAP call
  $data = array(
    'LocationID'    => 1,
    'ProducerName'  => $user->display_name,
    'Phone1'        => '',
    'Phone2'        => '',
    'Address'       => '',
    'Notes'         => '',
    'Active'        => 1,
    'ContactName'   => $user->display_name,
    'Email'         => $user->mail,
    'Website'       => '',
  );

  $producer_id = cablecast_api_server_query('CreateNewProducer', $data);

  return $producer_id->CreateNewProducerResult;

}

/**
 * Updates a Producer on Cablecast
 *
 * @param $data
 * @return $result
 */
function cablecast_api_update_producer($user) {
  /*
      <ProducerID>int</ProducerID>
      <ProducerName>string</ProducerName>
      <Phone1>string</Phone1>
      <Phone2>string</Phone2>
      <Address>string</Address>
      <Notes>string</Notes>
      <Active>boolean</Active>
      <ContactName>string</ContactName>
      <Email>string</Email>
      <Website>string</Website>
      <username>string</username>
      <password>string</password>
  */

  // setup data array for SOAP call
  $data = array(
    'ProducerID'    => $user->field_cablecast_producer_id[LANGUAGE_NONE][0]['value'],
    'ProcuerName'   => $user->display_name,
    'Phone1'        => '',
    'Phone2'        => '',
    'Address'       => '',
    'Notes'         => '',
    'Active'        => 1,
    'ContactName'   => $user->display_name,
    'Email'         => $user->mail,
    'Website'       => '',
  );

  $producer_id = cablecast_api_server_query('UpdateProducer', $data);

  return $producer_id->UpdateProducerResult;
}

/**
 * Retrieve a Producer from Cablecast
 *
 * @param $data
 * @return $return
 */
function cablecast_api_get_producer($user) {
  /*
      <ProducerID>int</ProducerID>
      <username>string</username>
      <password>string</password>
  */

  // setup data array for SOAP call
  $data = array(
    'ProducerID'    => $user->field_cablecast_producer_id[LANGUAGE_NONE][0]['value'],
  );

  $producer_id = cablecast_api_server_query('GetProducerInfo', $data);

  return $producer_id->GetProducerInfoResult->ProducerID;

}

/**
 * Retrieve a Producer ID from Cablecast
 *
 * @param $data
 * @return $return
 */
function cablecast_api_get_producer_from_id($uid) {
  /*
      <ProducerID>int</ProducerID>
      <username>string</username>
      <password>string</password>
  */

  // setup data array for SOAP call
  $data = array(
    'ProducerID'    => $uid,
  );

  $producer = cablecast_api_server_query('GetProducerInfo', $data);

  return $producer->GetProducerInfoResult->ProducerID;

}

/**
 * Create a Project on Cablecast
 *
 * @param $data
 * @return $return
 */
function cablecast_api_add_project($node) {
 /*
   <LocationID>int</LocationID>
   <Name>string</Name>
   <Description>string</Description>
   <ProducerID>int</ProducerID>
   <Podcast>boolean</Podcast>
   <PodcastName>string</PodcastName>
   <PodcastDescription>string</PodcastDescription>
   <username>string</username>
   <password>string</password>
 */

  // setup data array for SOAP call
  $data = array(
    'LocationID'          => 1,
    'Name'                => check_plain($node->title),
    'Description'         => '',
    'ProducerID'          => cablecast_api_get_producer_from_id($node->uid),
    'Podcast'             => FALSE,
    'PodcastName'         => '',
    'PodcastDescription'  => '',
  );

  $project_id = cablecast_api_server_query('CreateNewProject', $data);

  return $project_id->CreateNewProjectResult;
}

/**
 * Updates a Project on Cablecast
 *
 * @param $data
 * @return $return
 */
function cablecast_api_update_project($node) {
  /*
    <ProjectID>int</ProjectID>
    <Name>string</Name>
    <Description>string</Description>
    <ProducerID>int</ProducerID>
    <Podcast>boolean</Podcast>
    <PodcastName>string</PodcastName>
    <PodcastDescription>string</PodcastDescription>
    <username>string</username>
    <password>string</password>
  */

  // setup data array for SOAP call
  $data = array(
    'ProjectID'           => $node->field_cablecast_project_id[LANGUAGE_NONE][0]['value'],
    'Name'                => check_plain($node->title),
    'Description'         => '',
    'ProducerID'          => cablecast_api_get_producer_from_id($node->uid),
    'Podcast'             => FALSE,
    'PodcastName'         => '',
    'PodcastDescription'  => '',
  );
  $project_id = cablecast_api_server_query('UpdateProject', $data);

  return $project_id->UpdateProjectResult;
}

/**
 * Retrieves all Projects from Cablecast
 * Cablecast lacks a method to get a single Project
 *
 * @param $data
 *
 * @return bool $return
 */
function cablecast_api_get_projects($node) {
  /*
    <ChannelID>int</ChannelID>
  */

  $project_id = $node->field_cablecast_project_id[LANGUAGE_NONE][0]['value'];

  // setup data array for SOAP call
  $data = array(
    'ChannelID' => 1,
  );

  $projects = cablecast_api_server_query('GetProjects', $data);

  // return project_id if the Project is found
  foreach ($projects->GetProjectsResult->Project as $project) {
    if ($project->ProjectID == $project_id) {
      return $project_id;
    }
  }

  return FALSE;
}

/**
 * Retrieves all Projects from Cablecast
 * Cablecast lacks a method to get a single Project
 *
 * @param $data
 *
 * @return bool $return
 */
function cablecast_api_get_project_from_id($project_id) {
  /*
    <ChannelID>int</ChannelID>
  */

  // setup data array for SOAP call
  $data = array(
    'ChannelID' => 1,
  );

  $projects = cablecast_api_server_query('GetProjects', $data);

  // return project_id if the Project is found
  foreach ($projects->GetProjectsResult->Project as $project) {
    if ($project->ProjectID == $project_id) {
      return $project_id;
    }
  }

  return FALSE;
}

/**
 *
 * Adds a Show to Cablecast
 *
 * @param $data
 * @return $return
 */
function cablecast_api_add_show($node) {
  /*
      <LocalID>string</LocalID>
      <LocationID>int</LocationID>
      <Title>string</Title>
      <CGTitle>string</CGTitle>
      <ProjectID>int</ProjectID>
      <CGExempt>boolean</CGExempt>
      <ProducerID>int</ProducerID>
      <Reels>
        <NewReel>
          <FormatID>int</FormatID>
          <CueSeconds>int</CueSeconds>
          <LengthSeconds>int</LengthSeconds>
          <Title>int</Title>
          <Chapter>int</Chapter>
        </NewReel>
        <NewReel>
          <FormatID>int</FormatID>
          <CueSeconds>int</CueSeconds>
          <LengthSeconds>int</LengthSeconds>
          <Title>int</Title>
          <Chapter>int</Chapter>
        </NewReel>
      </Reels>
      <CategoryID>int</CategoryID>
      <EventDate>dateTime</EventDate>
      <Comments>string</Comments>
      <CustomFields>
        <CustomField>
          <Name>string</Name>
          <Value>string</Value>
        </CustomField>
        <CustomField>
          <Name>string</Name>
          <Value>string</Value>
        </CustomField>
      </CustomFields>
      <OnDemand>boolean</OnDemand>
      <OnDemandStatus>string</OnDemandStatus>
      <BugText>string</BugText>
      <CrawlText>string</CrawlText>
      <CrawlLengthInSeconds>int</CrawlLengthInSeconds>
      <username>string</username>
      <password>string</password>
   */

  // All Cablecast shows need at least 1 Reel
  $reels = new stdClass();
  $reels->NewReel = new stdClass();
  $reels->NewReel->FormatID = 1;
  $reels->NewReel->CueSeconds = 0;
  $reels->NewReel->LengthSeconds = 0;
  $reels->NewReel->Title = 1;
  $reels->NewReel->Chapter = 1;
  
  // setup data array for SOAP call
  $data = array(
    'LocalID'               => "Added by API",
    'LocationID'            => 1,
    'Title'                 => check_plain($node->title),
    'CGTitle'               => check_plain($node->title),
    'ProjectID'             => cablecast_api_get_project_from_id($node->og_group_ref[LANGUAGE_NONE][0]['target_id']),
    'CGExempt'              => FALSE, // TODO: may vary based on project type or other criteria
    'ProducerID'            => cablecast_api_get_producer_from_id($node->uid),
    'Reels'                 => $reels,
    'CategoryID'            => 1,
    'EventDate'             => $time = date("c", time()),
    'Comments'              => '',
    'OnDemand'              => FALSE,
    'OnDemandStatus'        => FALSE,
    'CustomFields'          => FALSE,
    'BugText'               => '',
    'CrawlText'             => '',
    'CrawlLengthInSeconds'  => '',
  );

  $show_id = cablecast_api_server_query('CreateNewShowRecord', $data);

  return $show_id->CreateNewShowRecordResult;
}

/**
 * Creates a Show record on Cablecast
 *
 * Updates an existing Show Record. All fields are required.
 * Blank or empty fields will overwrite existing show data.
 *
 * @param $data
 * @return $return
 */
function cablecast_api_update_show($node) {
  /*
    <ShowID>int</ShowID>
    <LocalID>string</LocalID>
    <Title>string</Title>
    <CGTitle>string</CGTitle>
    <ProjectID>int</ProjectID>
    <CGExempt>boolean</CGExempt>
    <ProducerID>int</ProducerID>
    <CategoryID>int</CategoryID>
    <EventDate>dateTime</EventDate>
    <Comments>string</Comments>
    <OnDemand>boolean</OnDemand>
    <OnDemandStatus>string</OnDemandStatus>
    <BugText>string</BugText>
    <CrawlText>string</CrawlText>
    <CrawlLengthInSeconds>int</CrawlLengthInSeconds>
    <username>string</username>
    <password>string</password>
   */

  // setup data array for SOAP call
  $data = array(
    'ShowID'                => $node->field_cablecast_show_id[LANGUAGE_NONE][0]['value'],
    'LocalID'               => "Added by API",
    'LocationID'            => 1,
    'Title'                 => check_plain($node->title),
    'CGTitle'               => check_plain($node->title),
    'ProjectID'             => cablecast_api_get_project_from_id($node->og_group_ref[LANGUAGE_NONE][0]['target_id']),
    'CGExempt'              => FALSE, // TODO: may vary based on project type or other criteria
    'ProducerID'            => cablecast_api_get_producer_from_id($node->uid),
    'CategoryID'            => 1,
    'EventDate'             => $time = date("c", time()),
    'Comments'              => '',
    'OnDemand'              => FALSE,
    'OnDemandStatus'        => FALSE,
    'CustomFields'          => FALSE,
    'BugText'               => '',
    'CrawlText'             => '',
    'CrawlLengthInSeconds'  => '',
  );


  $show_id = cablecast_api_server_query('UpdateShowRecord', $data);

  return $show_id->UpdateShowRecordResult;
}

/**
 * Creates a Schedule record on Cablecast
 *
 * Updates an existing Schedule Record. All fields are required.
 * Blank or empty fields will overwrite existing show data.
 *
 * @param $data
 * @return $return
 */
function cablecast_api_add_airing($airing, $cablecast_show_id, $channel_name) {
  /*
    <s:element minOccurs="1" maxOccurs="1" name="ChannelID" type="s:int"/>
		<s:element minOccurs="1" maxOccurs="1" name="LocationID" type="s:int"/>
		<s:element minOccurs="1" maxOccurs="1" name="ShowID" type="s:int"/>
		<s:element minOccurs="1" maxOccurs="1" name="RunDateTime" type="s:dateTime"/>
		<s:element minOccurs="1" maxOccurs="1" name="RunLock" type="s:boolean"/>
		<s:element minOccurs="1" maxOccurs="1" name="RunBump" type="s:int"/>
		<s:element minOccurs="1" maxOccurs="1" name="CGExempt" type="s:boolean"/>
		<s:element minOccurs="0" maxOccurs="1" name="BugText" type="s:string"/>
		<s:element minOccurs="0" maxOccurs="1" name="CrawlText" type="s:string"/>
		<s:element minOccurs="1" maxOccurs="1" name="CrawlLength" type="s:int"/>
		<s:element minOccurs="0" maxOccurs="1" name="username" type="s:string"/>
		<s:element minOccurs="0" maxOccurs="1" name="password" type="s:string"/>
   */
  
  dsm($cablecast_show_id);
  dsm($airing);
  
  $rundatetime = date("c", strtotime($airing->field_airing_date[LANGUAGE_NONE][0]['value']));
  
 //field_airing_channel 
 
  // setup data array for SOAP call
  $data = array(
    'ChannelID'             => $airing->field_airing_channel[LANGUAGE_NONE][0]['tid'],
    'LocationID'            => 1,
    'ShowID'           		  => $cablecast_show_id,
    'RunDateTime'           => $rundatetime,
    'RunLock'               => '',
    'RunBump'               => '',
    'CGExempt'              => FALSE, // TODO: may vary based on project type or other criteria
    'BugText'               => '',
    'CrawlText'             => '',
    'CrawlLength'           => '',
  );

  $airing_id = cablecast_api_server_query('AddScheduleEvent', $data);

  return $airing_id->AddScheduleEventResult;
}

/**
 * Updates a Show on Cablecast
 *
 * @param $data
 * @return $return
 */
function cablecast_api_get_show($node) {
  /*
    <ShowID>int</ShowID>
   */
  
  // setup data array for SOAP call
  $data = array(
    'ShowID'                => $node->field_cablecast_show_id[LANGUAGE_NONE][0]['value'],
  );
  
  $return = cablecast_api_server_query('GetShowInformation', $data);

  return $return->GetShowInformationResult->ShowID;
}

/**
 * Returns shows at this site that were modified in the range requested by the
 * SearchDate and DateComarator. DateComparator is a string consisting of
 * "<", ">", "<=", or ">="
 */
function cablecast_api_last_modified_search($data) {
  /*
         <LocationID>int</LocationID>
         <SearchDate>dateTime</SearchDate>
         <DateComparator>string</DateComparator>
   */

  $return = cablecast_api_server_query('LastModifiedSearch', $data);
  return $return->LastModifiedSearchResult->ShowInfo;
}

/**
 * Returns a list of all the channels on this Cablecast server.
 *
 * @return mixed
 */
function cablecast_api_get_channels() {
  /*
     <GetChannels xmlns="http://www.trms.com/CablecastWS/" />
   */

  $return = cablecast_api_server_query('GetChannels', $params = array());

  return $return->GetChannelsResult->Channel;

}

function cablecast_api_get_schedule($data) {
  /*
       <GetScheduleInformation xmlns="http://www.trms.com/CablecastWS/">
         <ChannelID>int</ChannelID>
         <FromDate>dateTime</FromDate>
         <ToDate>dateTime</ToDate>
         <restrictToShowID>int</restrictToShowID>
       </GetScheduleInformation>
   */

  $return = cablecast_api_server_query('GetScheduleInformation', $data);
  return $return->GetScheduleInformationResult->ScheduleInfo;

}

function cablecast_api_get_version() {
  if (!variable_get('cablecast_api_server_address', NULL)) {
    drupal_set_message(t('<a href="@url">Cablecast API must be configured.</a>', array('@url' => url('admin/config/cablecast-api'))), 'error');
    return;
  }
  return cablecast_api_server_query('DataVersion', $data = array())->DataVersionResult;
}

function cablecast_api_get_locations() {
  return cablecast_api_server_query('GetLocations', $data = array());
}