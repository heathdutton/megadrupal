<?php


/**
 * Client class.
 *
 * TODO: also implements CRMRemoteQueryInterface or somesuch!
 */
class clients_connection_ms_dynamics_soap extends clients_connection_base
  implements
    ClientsConnectionAdminUIInterface,
    ClientsRemoteEntityInterface {

  // ================================= Clients admin UI

  /**
   * Declare an array of properties which should be treated as credentials.
   *
   * This lets the credentials storage plugin know which configuration
   * properties to take care of.
   *
   * @return
   *  A flat array of property names.
   */
  function credentialsProperties() {
    return array('wlid_username', 'wlid_password');
  }

  /**
   * Extra form elements specific to a class's edit form.
   *
   * This is the same pattern as node_form() -- just ignore the object behind
   * the curtain ;)
   *
   * @param $form_state
   *  The form state from the main form, which you probably don't need anyway.
   *
   * @see clients_connection_form()
   * @see clients_connection_form_submit()
   */
  function connectionSettingsFormAlter(&$form, &$form_state) {
    // Change the endpoint element.
    $form['endpoint']['#title'] = t('MS CRM Service endpoint');
    $form['endpoint']['#description'] = t('MS CRM Service endpoint URL');

    $form['configuration']['application_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name of the application'),
      '#size' => 50,
      '#maxlength' => 50,
      '#description' => t('The name of the application connecting to MS CRM'),
      // TODO: uncomment these when out of dev!
      //'#required' => TRUE,
    );
    $form['credentials']['wlid_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Windows Live username'),
      '#size' => 50,
      '#maxlength' => 100,
      '#description' => t('The windows live account username for connecting to MS CRM'),
      //'#required' => TRUE,
    );
    $form['credentials']['wlid_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Windows Live password'),
      '#size' => 50,
      '#maxlength' => 100,
      '#description' => t('The windows live account password for connecting to MS CRM'),
      //'#required' => TRUE,
    );
    $form['configuration']['device_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Device username'),
      '#size' => 30,
      '#maxlength' => 30,
      '#description' => t('The username of the device connecting to MS CRM'),
      //'#required' => TRUE,
    );
    $form['configuration']['device_password'] = array(
      '#type' => 'textfield',
      '#title' => t('Device password'),
      '#size' => 30,
      '#maxlength' => 30,
      '#description' => t('The password of the device connecting to MS CRM'),
      //'#required' => TRUE,
    );
    $form['configuration']['read_only'] = array(
      '#type' => 'checkbox',
      '#title' => t('Read-only'),
      '#description' => t('Marks this connection read-only. This requires associated Resources to support this feature.'),
    );
  }

  // ================================= ClientsRemoteEntityInterface

  /**
   * Load a remote entity.
   *
   * The resource calling this should take care of process the data returned by
   * this into a Drupal entity.
   *
   * @param $entity_type
   *  The entity type to load.
   * @param $id
   *  The (remote) ID of the entity.
   *
   * @return
   *  The remote entity data.
   */
  function remote_entity_load($entity_type, $id) {
    $query = $this->getRemoteEntityQuery('select');
    $query->base($entity_type);
    $query->entityCondition('entity_id', $id);
    $result = $query->execute();
    //dsm($result, 'remote_entity_load $result');

    // There's only one. Same pattern as entity_load_single().
    return reset($result);
  }

  /**
   * Load multiple remote entities.
   *
   * The resource calling this should take care of process the data returned by
   * this into Drupal entities.
   *
   * @param $entity_type
   *  The entity type to load.
   * @param $id
   *  An array of the (remote) entity IDs.
   *
   * @return
   *  The remote entity data. This is a numeric array in the same order as the
   *  given array of remote IDs (at least, observation shows that this is the
   *  case so far! Who knows with Microsoft!).
   */
  function remote_entity_load_multiple($entity_type, $ids) {
    $query = $this->getRemoteEntityQuery('select');
    $query->base($entity_type);
    $query->entityCondition('entity_id', $ids, 'In');
    $result = $query->execute();

    return $result;
  }

  /**
   * Save a remote entity.
   *
   * @param $entity_type
   *  The entity type to save.
   * @param $entity
   *  The entity to save.
   *
   * @return
   *  If the entity is being created remotely, the new remote GUID.
   */
  function remote_entity_save($entity_type, $entity, $remote_properties = array()) {
    // Determine whether this is an insert or an update. An entity not yet
    // saved remotely will have no remote ID property.
    $action = empty($entity->remote_id) ? 'insert' : 'update';
    //dsm($action, '$action');

    $query = $this->getRemoteEntityQuery($action);
    $query->setEntity($entity_type, $entity);

    if ($action == 'insert') {
      $guid = $query->execute();
      return $guid;
    }
    else {
      $query->addFields($remote_properties);

      $query->execute();
    }
  }

  /**
   * Save multiple entities remotely, using XRM's bulk update.
   *
   * @param $entity_type
   *  The entity type to save.
   * @param $entities
   *  An array of entities to save in bulk, keyed by the entity ID.
   * @param $remote_properties
   *  (Optional) An array of properties to save. Values should be names of
   *  properties which are keys in the entity info 'property map' array. Only
   *  applies when updating rather than inserting.
   *
   * @return
   *  An array of the remote GUIDs created by the operation, keyed by the
   *  entity ID. Thus an entity which is updated rather than inserted will not
   *  be present in the array.
   */
  function remote_entity_save_multiple($entity_type, $entities, $remote_properties = array()) {
    $query = $this->getRemoteEntityQuery('bulk_save');
    $query->setEntities($entity_type, $entities);
    $query->addFields($remote_properties);

    // Execute the query. The guids array is keyed by entity ID.
    $guids = $query->execute();

    return $guids;
  }

  /**
   * Provide a map of remote property types to Drupal types.
   *
   * Roughly analogous to _entity_metadata_convert_schema_type().
   *
   * @return
   *  An array whose keys are remote property types as used as types for fields
   *  in hook_remote_entity_query_table_info(), and whose values are types
   *  recognized by the Entity Metadata API (as listed in the documentation for
   *  hook_entity_property_info()).
   *  If a remote property type is not listed here, it will be mapped to 'text'
   *  by default.
   */
  function entity_property_type_map() {
    return array(
      'EntityCollection' => 'list<string>',
    );
  }

  /**
   * Provide setter and getter callbacks for specific remote property types.
   *
   * @return
   *  An array keyed by remote property type, where each value is an array
   *  defining the callbacks for remote properties of this type. Keys in this
   *  must be 'getter callback' and 'setter callback'. The raw setter and getter
   *  callbacks may also be included.
   *
   * @see RemoteEntityAPIDefaultMetadataController::entityPropertyInfo()
   */
  function entity_property_type_callbacks() {
    return array(
      'dateTime' => array(
        'getter callback' => 'clients_ms_dynamics_soap_entity_mapped_property_datetime_get',
        'setter callback' => 'clients_ms_dynamics_soap_entity_mapped_property_datetime_set',
        // Add callbacks for raw values, which can just be the basic ones.
        'raw getter callback' => 'remote_entity_entity_mapped_property_get',
        'raw setter callback' => 'remote_entity_entity_mapped_property_set',
      ),
    );
  }

  /**
   * Get a new RemoteEntityQuery object appropriate for the connection.
   *
   * @param $query_type
   *  (optional) The type of the query. Defaults to 'select'.
   *
   * @return
   *  A remote query object of the type appropriate to the query type.
   */
  function getRemoteEntityQuery($query_type = 'select') {
    // TODO: change class names to be determined automatically, like in core?
    switch ($query_type) {
      case 'select':
        return new MSDynamicsSoapSelectQuery($this);
      case 'fetch_expression':
        return new MSDynamicsSoapSelectFetchExpressionQuery($this);
      case 'insert':
        return new MSDynamicsSoapInsertQuery($this);
      case 'update':
        return new MSDynamicsSoapUpdateQuery($this);
      case 'bulk_save':
        return new MSDynamicsSoapBulkSaveQuery($this);
      case 'set_state':
        return new MSDynamicsSoapSetStateRequestQuery($this);
    }
  }

  /**
   * Helper to create a dummy remote ID, to use in testing.
   *
   * XRM uses GUIDS of the form:
   *  00000000-0000-0000-0000-000000000000
   * We create a dummy of the form:
   *  00000000-0000-FFFF-FFFF-iiiitttttttt
   * where:
   *  - 0 and F are literals. These make our dummy GUIDs in theory easily
   *    recognizable as such, to help prevent head-scratching (such as when
   *    dummy-saved entities fail to update from remote).
   *  - i is an incremented number to ensure uniqueness within a single request
   *    (if you need to create more than 9999 remote entities in a single
   *    request, contact me with your server spec).
   *  - t is the request timestamp converted to hex.
   */
  function generateDummyRemoteID() {
    static $i;
    $i++;

    $prefix = '00000000-0000-FFFF-FFFF';

    $increment = sprintf("%04d", $i);

    // Current timestamps converted to hexadecimal yield 8 characters, and will
    // continue to do so until Sun, 07 Feb 2106 06:28:15 (and the 2038 problem
    // will happen way before then).
    $date_hex = dechex(time());

    $dummy_guid = "$prefix-$increment$date_hex";

    return $dummy_guid;
  }

  // ================================= ClientsCRMQueryInterface

  // More general querying methods?
  // TODO!

  // ================================= SOAP API

  /**
   * Cache lifetime: 1 hour.
   *
   * XRM caches our credentials for 1 day.
   */
  const CACHE_LIFETIME = 3600;

  /**
   * Connect and log in to the remote service.
   */
  function connect() {
    $cid = 'clients_connection_ms_dynamics_soap:' . $this->name;
    $cache = cache_get($cid);
    if ($cache === FALSE || REQUEST_TIME > $cache->expire) {
      $endpoints = $this->fetchEndpoints();

      // login() will set the cache.
      $this->login($endpoints);
      $this->lastRefresh = REQUEST_TIME;
    }
    else {
      $this->credentials = $cache->data;
      $this->isLoggedIn = TRUE;
    }
  }

  /**
   * Try to authenticate with MS Dynamics.
   *
   * Helper for connect().
   *
   * $param $endpoints
   *  An array of endpoints for MS Dynamics as obtained by fetchEndpoints().
   */
  function login($endpoints) {
    $this->credentialsLoad();

    // Test to see if we are using the new Office365 authentication or not
    if ($endpoints['STSEndpoint'] != "https://login.microsoftonline.com/RST2.srf" ) {
      $credentials = array();

      // Urrrrr not sure why we redo these when they are saved in the connection.
      // TODO: investigate earlier version of this!
      $credentials['device_username'] = '11' . $this->random_string();
      $credentials['device_password'] = $this->random_string();

      // Register device
      // registerDevice();
      // TODO: no idea what this is for; nothing uses this!
      $request = $this->serialiseRegistrationRequest($this->configuration['application_name'], $credentials['device_username'], $credentials['device_password']);
      $url = "https://login.live.com/ppsecure/DeviceAddCredential.srf";

      $this->deviceRegistration = $this->postRegistrationRequest($url, $request);

      // Stage 2: Get binaryDAToken.
      $binaryDAToken = $this->getBinaryDAToken($endpoints, $credentials);
      // Check the token is OK.
      $xml = simplexml_load_string($binaryDAToken);
      $this->checkXMLResponse($xml, 'http://www.w3.org/2003/05/soap-envelope');
    }
    else {
      // Looks like we must do this at the start of the session to verify we have a valid login
      $realm_response = $this->getUserRealm($this->credentials['wlid_username'],
        $this->credentials['wlid_password'],
        "https://login.microsoftonline.com/GetUserRealm.srf/");

      $xml = simplexml_load_string($realm_response);
      // Now we want to check if success.
      $xpathresult = $xml->xpath("//RealmInfo/@Success");
      $realm_success = $xpathresult[0]->Success;
      // We should test for true here and throw an error if not true.
      if ($realm_success != "true") {
        dsm("False");
      }

      // Stage 2: Get binaryDAToken.
      // This a new security token method.
      $binaryDAToken = $this->getMSOnlineBinaryDAToken($endpoints, $this->credentials);

      // TODO: use checkXMLResponse(), which needs the namespaces figuring out.
    }

    $domDA  = new DomDocument();
    $domDA->loadXML($binaryDAToken);
    $cipherValues = $domDA->getElementsbyTagName("CipherValue");

    if (!is_object($cipherValues->item(0))) {
      // TODO: This can probably be removed when checkXMLResponse() is used in
      // the second login system above.
      throw new Exception("No cipher value returned");
    }

    $this->cipherValue = $cipherValues->item(0)->textContent;

    if (!empty($this->cipherValue)) {
      // Stage 3.
      $this->securityTokens = $this->getSecurityTokens($endpoints);
      $tokenDom = new DomDocument();
      $tokenDom->loadXML($this->securityTokens);
      $cipherValues = $tokenDom->getElementsbyTagName("CipherValue");
      $this->securityToken0 = $cipherValues->item(0)->textContent;
      $this->securityToken1 = $cipherValues->item(1)->textContent;
      $this->identifierKey  =  $tokenDom->getElementsbyTagName("KeyIdentifier")->item(0)->textContent;
      $this->binarySecret = $tokenDom->getElementsbyTagName("BinarySecret")->item(0)->textContent;
    }

    $credentials['cipher_value']    = $this->cipherValue;
    $credentials['security_token0'] = $this->securityToken0;
    $credentials['security_token1'] = $this->securityToken1;
    $credentials['identifier_key']  = $this->identifierKey;
    $credentials['binary_secret']   = $this->binarySecret;
    $credentials['created']         = $this->tokenCreated;
    $credentials['expires']         = $this->tokenExpires;

    // Our cache ID is our classname + our machine name.
    // @TODO: standardize this in Clients module.
    $cid = 'clients_connection_ms_dynamics_soap:' . $this->name;
    cache_set($cid, $credentials, 'cache',  REQUEST_TIME + clients_connection_ms_dynamics_soap::CACHE_LIFETIME);

    $this->credentials = $credentials;

    $this->isLoggedIn = TRUE;
    $status = TRUE;
    return $status;
  }

  /**
   * Helper for login().
   *
   * @param $username
   *  TODO!
   * @param $password
   * @param $url
   *
   * @return
   *  The response.
   */
  protected function getUserRealm($username, $password, $url) {
    $cURL = curl_init();
    $request = "login=" . $username . "&xml=1";
    curl_setopt($cURL, CURLOPT_URL, $url);
    curl_setopt($cURL, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($cURL, CURLOPT_TIMEOUT, variable_get('clients_connection_ms_dynamics_soap_curl_timeout', 120));
    curl_setopt($cURL, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_setopt($cURL, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($cURL, CURLOPT_POST, 1);
    curl_setopt($cURL, CURLOPT_POSTFIELDS, $request);
    $curl_debug_file = 'temporary://clients_msd_curlrealm.log';
    $fh = fopen($curl_debug_file, "w+");
    curl_setopt($cURL, CURLOPT_STDERR, $fh);
    curl_setopt($cURL, CURLOPT_VERBOSE, true);

    $response = curl_exec($cURL);

    if (curl_exec($cURL) === FALSE)  {
      $curl_error = curl_error($cURL);
      throw new Exception(t('Curl error in getUserRealm(): "@error".', array(
        '@error' => $curl_error,
      )));
    }

    return $response;
  }

  /**
   * Helper for login().
   *
   * Device registration request has to have an XML payload so we serialise it here.
   */
  protected function serialiseRegistrationRequest($clientInfo, $memberName, $devicePassword) {
    $request = '
    <DeviceAddRequest>
      <ClientInfo name="'.$clientInfo.'" version="1.0"/>
      <Authentication>
        <Membername>'.$memberName.'</Membername>
        <Password>'.$devicePassword.'</Password>
      </Authentication>
    </DeviceAddRequest>';

    return $request;
  }

  /**
   * Helper for login().
   *
   * The documentation implies this should be a SOAP request but in reality it
   * is just a normal POST.
   */
  protected function postRegistrationRequest($url, $request) {
    $cURL = curl_init();
    /*
    $headers = array(
        "POST ". $postUrl ." HTTP/1.1",
        "Host: " . $hostname,
        'Connection: Keep-Alive',
        "Content-type: application/soap+xml; charset=UTF-8",
        "Content-length: ".strlen($content),
    );
    */
    curl_setopt($cURL, CURLOPT_URL, $url);
    curl_setopt($cURL, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($cURL, CURLOPT_TIMEOUT, variable_get('clients_connection_ms_dynamics_soap_curl_timeout', 120));
    curl_setopt($cURL, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($cURL, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_setopt($cURL, CURLOPT_POST, 1);
    curl_setopt($cURL, CURLOPT_POSTFIELDS, $request);

    $response = curl_exec($cURL);
    //var_dump($response);
    curl_close($cURL);

    return $response;
  }

  /**
   * Helper for login(). Generates random string.
   *
   * @return
   *  A random string.
   */
  protected function random_string() {
    $charset = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    $str = '';
    $length = 24;
    $count = strlen($charset);
    while ($length--) {
      $str .= $charset[mt_rand(0, $count-1)];
    }
    return $str;
  }

  /**
   * Stage 2 of login process: old authorization method.
   *
   * @param $endpoints
   *  The array of endpoints.
   * @param $credentials
   *  The login credentials.
   *
   * @return
   *  The result of the SOAP request.
   */
  protected function getBinaryDAToken($endpoints, $credentials) {
    $messageID = $this->generateGUID();

    list($currentTime, $nextDayTime) = $this->getDates();

    $deviceCredentialsSoapTemplate = '<s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope"
                    xmlns:a="http://www.w3.org/2005/08/addressing"
                    xmlns:u="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd">

                        <s:Header>
                        <a:Action s:mustUnderstand="1">
                        http://schemas.xmlsoap.org/ws/2005/02/trust/RST/Issue</a:Action>
                        <a:MessageID>
                        urn:uuid:%s</a:MessageID>
                        <a:ReplyTo>
                            <a:Address>
                            http://www.w3.org/2005/08/addressing/anonymous</a:Address>
                        </a:ReplyTo>
                        <VsDebuggerCausalityData xmlns="http://schemas.microsoft.com/vstudio/diagnostics/servicemodelsink">
                        uIDPoy9Ez+P/wJdOhoN2XNauvYcAAAAAK0Y6fOjvMEqbgs9ivCmFPaZlxcAnCJ1GiX+Rpi09nSYACQAA</VsDebuggerCausalityData>
                        <a:To s:mustUnderstand="1">
                        %s</a:To>
                        <o:Security s:mustUnderstand="1"
                        xmlns:o="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd">
                            <u:Timestamp u:Id="_0">
                            <u:Created>%sZ</u:Created>
                            <u:Expires>%sZ</u:Expires>
                            </u:Timestamp>
                            <o:UsernameToken u:Id="devicesoftware">
                            <o:Username>%s</o:Username>
                            <o:Password Type="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordText">%s</o:Password>
                            </o:UsernameToken>
                        </o:Security>
                        </s:Header>
                        <s:Body>
                        <t:RequestSecurityToken xmlns:t="http://schemas.xmlsoap.org/ws/2005/02/trust">
                            <wsp:AppliesTo xmlns:wsp="http://schemas.xmlsoap.org/ws/2004/09/policy">
                            <a:EndpointReference>
                                <a:Address>http://passport.net/tb</a:Address>
                            </a:EndpointReference>
                            </wsp:AppliesTo>
                            <t:RequestType>
                            http://schemas.xmlsoap.org/ws/2005/02/trust/Issue</t:RequestType>
                        </t:RequestSecurityToken>
                        </s:Body>
                    </s:Envelope>';

    $requestXML = sprintf($deviceCredentialsSoapTemplate,
      $messageID,
      $endpoints['STSEndpoint'],
      $currentTime,
      $nextDayTime,
      $credentials['device_username'],
      $credentials['device_password']
    );

    $result = $this->GetSOAPResponse($endpoints['STSEndpoint'], $requestXML);

    //echo 'Result:' . $result;

    return $result;
  }

  /**
   * Stage 2 of login process: new authorization method.
   *
   * @param $endpoints
   *  The array of endpoints.
   * @param $credentials
   *  The login credentials.
   *
   * @return
   *  The result of the SOAP request.
   */
  protected function getMSOnlineBinaryDAToken($endpoints, $credentials) {
    $messageID = $this->generateGUID();

    list($currentTime, $nextDayTime) = $this->getDates();

    $securityTokenSoapTemplate = '<s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope" xmlns:a="http://www.w3.org/2005/08/addressing" xmlns:u="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd">
      <s:Header>
        <a:Action s:mustUnderstand="1">http://schemas.xmlsoap.org/ws/2005/02/trust/RST/Issue</a:Action>
        <a:MessageID>urn:uuid:%s</a:MessageID>
        <a:ReplyTo>
          <a:Address>http://www.w3.org/2005/08/addressing/anonymous</a:Address>
        </a:ReplyTo>
        <a:To s:mustUnderstand="1">%s</a:To>
        <o:Security s:mustUnderstand="1" xmlns:o="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd">
          <u:Timestamp u:Id="_0">
            <u:Created>%sZ</u:Created>
            <u:Expires>%sZ</u:Expires>
          </u:Timestamp>
          <o:UsernameToken u:Id="uuid-cdb639e6-f9b0-4c01-b454-0fe244de73af-1">
            <o:Username>%s</o:Username>
            <o:Password Type="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordText">%s</o:Password>
          </o:UsernameToken>
        </o:Security>
      </s:Header>
      <s:Body>
        <t:RequestSecurityToken xmlns:t="http://schemas.xmlsoap.org/ws/2005/02/trust">
          <wsp:AppliesTo xmlns:wsp="http://schemas.xmlsoap.org/ws/2004/09/policy">
            <a:EndpointReference>
              <a:Address>https://dynamicscrmemea.accesscontrol.windows.net/</a:Address>
            </a:EndpointReference>
          </wsp:AppliesTo>
          <t:RequestType>http://schemas.xmlsoap.org/ws/2005/02/trust/Issue</t:RequestType>
        </t:RequestSecurityToken>
      </s:Body>
    </s:Envelope>';

    $requestXML = sprintf($securityTokenSoapTemplate,
      $messageID,
      $endpoints['STSEndpoint'],
      $currentTime,
      $nextDayTime,
      $credentials['wlid_username'],
      $credentials['wlid_password']
    );

    $result = $this->GetSOAPResponse($endpoints['STSEndpoint'], $requestXML);

    return $result;
  }

  /**
   * Helper for login().
   *
   * Stage 3 of login process.
   *
   * @param $endpoints
   *  The array of endpoints.
   */
  protected function getSecurityTokens($endpoints) {
    $messageID = $this->generateGUID();

    list($currentTime, $nextDayTime) = $this->getDates();

    // TODO: pass $credentials in by reference and add these things in there.
    $this->tokenCreated = time();
    $this->tokenExpires = $this->tokenCreated + (24 * 3600);

    $securityTokenSoapTemplate =
       '<s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope"
        xmlns:a="http://www.w3.org/2005/08/addressing"
        xmlns:u="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd">
          <s:Header>
            <a:Action s:mustUnderstand="1">
            http://schemas.xmlsoap.org/ws/2005/02/trust/RST/Issue</a:Action>
            <a:MessageID>
            urn:uuid:%s</a:MessageID>
            <a:ReplyTo>
              <a:Address>
              http://www.w3.org/2005/08/addressing/anonymous</a:Address>
            </a:ReplyTo>
            <VsDebuggerCausalityData xmlns="http://schemas.microsoft.com/vstudio/diagnostics/servicemodelsink">
            uIDPozBEz+P/wJdOhoN2XNauvYcAAAAAK0Y6fOjvMEqbgs9ivCmFPaZlxcAnCJ1GiX+Rpi09nSYACQAA</VsDebuggerCausalityData>
            <a:To s:mustUnderstand="1">
            %s</a:To>
            <o:Security s:mustUnderstand="1"
            xmlns:o="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd">
              <u:Timestamp u:Id="_0">
                <u:Created>%sZ</u:Created>
                <u:Expires>%sZ</u:Expires>
              </u:Timestamp>
              <o:UsernameToken u:Id="user">
                <o:Username>%s</o:Username>
                <o:Password Type="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordText">%s</o:Password>
              </o:UsernameToken>
              <wsse:BinarySecurityToken ValueType="urn:liveid:device"
              xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd">
                <EncryptedData Id="BinaryDAToken0"
                Type="http://www.w3.org/2001/04/xmlenc#Element"
                xmlns="http://www.w3.org/2001/04/xmlenc#">
                  <EncryptionMethod Algorithm="http://www.w3.org/2001/04/xmlenc#tripledes-cbc">
                  </EncryptionMethod>
                  <ds:KeyInfo xmlns:ds="http://www.w3.org/2000/09/xmldsig#">
                    <ds:KeyName>http://Passport.NET/STS</ds:KeyName>
                  </ds:KeyInfo>
                  <CipherData>
                    <CipherValue>%s</CipherValue>
                  </CipherData>
                </EncryptedData>
              </wsse:BinarySecurityToken>
            </o:Security>
          </s:Header>
          <s:Body>
            <t:RequestSecurityToken xmlns:t="http://schemas.xmlsoap.org/ws/2005/02/trust">
              <wsp:AppliesTo xmlns:wsp="http://schemas.xmlsoap.org/ws/2004/09/policy">
                <a:EndpointReference>
                  <a:Address>%s</a:Address>
                </a:EndpointReference>
              </wsp:AppliesTo>
              <wsp:PolicyReference URI="MBI_FED_SSL"
              xmlns:wsp="http://schemas.xmlsoap.org/ws/2004/09/policy" />
              <t:RequestType>
              http://schemas.xmlsoap.org/ws/2005/02/trust/Issue</t:RequestType>
            </t:RequestSecurityToken>
          </s:Body>
        </s:Envelope>';


    $this->credentialsLoad();
    $requestXML = sprintf($securityTokenSoapTemplate,
      $messageID,
      $endpoints['STSEndpoint'],
      $currentTime,
      $nextDayTime,
      $this->credentials['wlid_username'], // $this->WLIDUsername,
      $this->credentials['wlid_password'], // $this->WLIDPassword,
      $this->cipherValue,
      $endpoints['URNAddress']
    );

    $result = $this->GetSOAPResponse($endpoints['STSEndpoint'], $requestXML);

    return $result;
  }

  /**
   * Helper for connect().
   *
   * @return
   *  An array of endpoints, containing:
   *  - 'URNAddress': The URN address.
   *  - 'STSEndpoint': The STS endpoint (yes, I have no clue what either of
   *    these are, they're just bits of data I am passing around like a monkey).
   */
  public function fetchEndpoints() {
    // Previuosly we fetched $this->endpoint . '?wsdl' and parse the response
    // for the WSDLImportURL but new XRM was generating errors and not reliably
    // resolving this but the underlying wsdl seemed sound so we have basically
    // skipped a step as analysis shows that this equates to $this->endpoint . '?wsdl=wsdl0'
    $WSDLImportURL = $this->endpoint . '?wsdl=wsdl0';
    $WSDKImport = $this->fetchWSDL($WSDLImportURL);
    $xpathresult = $WSDKImport->xpath("//*[local-name()='AuthenticationPolicy' and namespace-uri()='http://schemas.microsoft.com/xrm/2011/Contracts/Services']/*[local-name()='SecureTokenService' and namespace-uri()='http://schemas.microsoft.com/xrm/2011/Contracts/Services']//*[local-name()='AppliesTo' and namespace-uri()='http://schemas.microsoft.com/xrm/2011/Contracts/Services']/text()");
    $URNAddress = $xpathresult[0];

    $xpathresult = $WSDKImport->xpath("//*[local-name()='Issuer' and namespace-uri()='http://docs.oasis-open.org/ws-sx/ws-securitypolicy/200702']/*[local-name()='Address' and namespace-uri()='http://www.w3.org/2005/08/addressing']/text()");
    $STSEndpoint = (string) $xpathresult[0];

    $endpoints = array(
      'URNAddress'  => $URNAddress,
      'STSEndpoint' => $STSEndpoint,
    );
    return $endpoints;
  }

  /**
   * Helper for fetchEndpoints().
   *
   * Uses a curl based fetch, as simplexml_load_file() has SSL issues on newer
   * linux distros and curl provides more control.
   *
   * @param $url
   *  The URL to fetch.
   *
   * @return
   *  A SimpleXMLElement object.
   */
  protected function fetchWSDL($url) {
    $response = $this->fetchWithCurl($url);

    $xml = simplexml_load_string($response);

    $this->checkXMLResponse($xml, 'http://schemas.microsoft.com/ws/2005/05/envelope/none');

    //print_r($xml);
    //dsm($simplexml);

    return $xml;
  }

  /**
   * Helper for fetchWSDL(). Fetches a web resource using curl.
   *
   * @param $url
   *  The URL to fetch data from.
   *
   * @return
   *  The data from the request.
   */
  protected function fetchWithCurl($url) {
    $curl_handle = curl_init();
    $timeout = 60;

    $headers = array(
      'Connection: Keep-Alive',
      "Content-Type: text/xml; charset=utf-8",
    );
    curl_setopt($curl_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_setopt($curl_handle, CURLOPT_HTTPHEADER, $headers);
    curl_setopt($curl_handle, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($curl_handle, CURLOPT_SSLVERSION, 3);
    curl_setopt($curl_handle, CURLOPT_URL, $url);
    curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($curl_handle, CURLOPT_CONNECTTIMEOUT, $timeout);
    curl_setopt($curl_handle, CURLOPT_SSL_VERIFYPEER, FALSE);
    $data = curl_exec($curl_handle);

    if (curl_exec($curl_handle) === FALSE)  {
      $curl_error = curl_error($curl_handle);
      throw new Exception(t('Curl error in fetchWithCurl(): "@error".', array(
        '@error' => $curl_error,
      )));
    }

    curl_close($curl_handle);

    return $data;
  }

  /**
   * Get dates to pass to XRM.
   *
   * @return
   *  A flat array containing two date strings suitable for passing to XRM:
   *  - the current date.
   *  - the date for 1 day from now.
   */
  function getDates() {
    // XRM needs dates in UTC, otherwise it may think that we are giving it
    // dates in the past.
    $current_date = new DateTime('now', new DateTimeZone('UTC'));
    $current_date_string = $current_date->format("Y-m-d\TH:i:s") . ".00";

    $next_day_date = new DateTime('now +1 day', new DateTimeZone('UTC'));
    $next_day_date_string = $next_day_date->format("Y-m-d\TH:i:s"). ".00";

    return array($current_date_string, $next_day_date_string);
  }

  /**
   * Check the XML response to see if there is an error.
   *
   * @todo Expand this so that query classes can use it.
   *
   * @param $xml
   *  A SimpleXMLElement object for the response XML.
   * @param $namespace_uri
   *  The namespace UI to use for looking for the Fault XML elements. Because
   *  @£$%^ Microsoft send us XML back with this in different namespaces
   *  depending on when it's happening.
   *
   * @throws Exception
   *  Throws an exception if there is an error. The message of the exception is
   *  the fault text from the returned XML. It is advised to catch this and
   *  throw another exception with more detail about where the error occurred.
   */
  function checkXMLResponse($xml, $namespace_uri) {
    // See if there is an error.
    // MAY look like this:
    /*
    <Fault xmlns="http://schemas.microsoft.com/ws/2005/05/envelope/none">
      <Code>
        <Value>Receiver</Value>
        <Subcode>
          <Value xmlns:a="http://schemas.microsoft.com/net/2005/12/windowscommunicationfoundation/dispatcher">a:InternalServiceFault</Value>
        </Subcode>
      </Code>
      <Reason>
        <Text xml:lang="en-US">The server was unable to process the request due to an internal error.  For more information about the error, either turn on IncludeExceptionDetailInFaults (either from ServiceBehaviorAttribute or from the &lt;serviceDebug&gt; configuration behavior) on the server in order to send the exception information back to the client, or turn on tracing as per the Microsoft .NET Framework 3.0 SDK documentation and inspect the server trace logs.</Text>
      </Reason>
    </Fault>

    .. but might also be buried inside a SOAP envelope! THANKS MICROSOFT.
    */

    $xml->registerXPathNamespace('ns', $namespace_uri);
    $text_element = $xml->xpath("/ns:Fault/ns:Reason/ns:Text");
    if (!empty($text_element)) {
      $text = (string) $text_element[0];

      throw new Exception($text);
    }
  }

  /**
   * Generate a standard GUID.
   *
   * Cribbed from http://php.net/manual/en/function.com-create-guid.php
   */
  function generateGUID() {
    if (function_exists('com_create_guid') === TRUE) {
      return trim(com_create_guid(), '{}');
    }

    return sprintf('%04X%04X-%04X-%04X-%04X-%04X%04X%04X', mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(16384, 20479), mt_rand(32768, 49151), mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535));
  }

  /**
   * Get the header XML for a query.
   *
   * @param $action
   *  The action to use for the query. One of:
   *    - 'RetrieveMultiple'
   *    - 'Retrieve'
   *
   * @return
   *  An XRML string for the query, starting with the opening 'Envelope' tag,
   *  and containing a complete 'Header' element. It is up to the caller to
   *  close the 'Envelope' element.
   */
  public function get_query_header($action) {
    // REFACTOR NEEDED

    $messageID = $this->generateGUID();

    list($currentTime, $nextDayTime) = $this->getDates();

    // The use of data in $this->credentials requires us to have called
    // connect() on the connection first!

    $headerXML = '
                    <s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope"
                    xmlns:a="http://www.w3.org/2005/08/addressing"
                    xmlns:u="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd">
                      <s:Header>
                        <a:Action s:mustUnderstand="1">
                        http://schemas.microsoft.com/xrm/2011/Contracts/Services/IOrganizationService/' . $action . '</a:Action>
                        <a:MessageID>
                        urn:uuid:' . $messageID . '</a:MessageID>
                        <a:ReplyTo>
                          <a:Address>
                          http://www.w3.org/2005/08/addressing/anonymous</a:Address>
                        </a:ReplyTo>
                        <VsDebuggerCausalityData xmlns="http://schemas.microsoft.com/vstudio/diagnostics/servicemodelsink">
                        uIDPozJEz+P/wJdOhoN2XNauvYcAAAAAK0Y6fOjvMEqbgs9ivCmFPaZlxcAnCJ1GiX+Rpi09nSYACQAA</VsDebuggerCausalityData>
                        <a:To s:mustUnderstand="1">' . $this->endpoint . '</a:To>
                        <o:Security s:mustUnderstand="1"
                        xmlns:o="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd">
                          <u:Timestamp u:Id="_0">
                            <u:Created>' . $currentTime . 'Z</u:Created>
                            <u:Expires>' . $nextDayTime . 'Z</u:Expires>
                          </u:Timestamp>
                          <EncryptedData Id="Assertion0"
                          Type="http://www.w3.org/2001/04/xmlenc#Element"
                          xmlns="http://www.w3.org/2001/04/xmlenc#">
                            <EncryptionMethod Algorithm="http://www.w3.org/2001/04/xmlenc#tripledes-cbc">
                            </EncryptionMethod>
                            <ds:KeyInfo xmlns:ds="http://www.w3.org/2000/09/xmldsig#">
                              <EncryptedKey>
                                <EncryptionMethod Algorithm="http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p">
                                </EncryptionMethod>
                                <ds:KeyInfo Id="keyinfo">
                                  <wsse:SecurityTokenReference xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd">

                                    <wsse:KeyIdentifier EncodingType="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary"
                                    ValueType="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-x509-token-profile-1.0#X509SubjectKeyIdentifier">
                                    ' . $this->credentials['identifier_key'] . '</wsse:KeyIdentifier>
                                  </wsse:SecurityTokenReference>
                                </ds:KeyInfo>
                                <CipherData>
                                  <CipherValue>' . $this->credentials['security_token0'] . '</CipherValue>
                                </CipherData>
                              </EncryptedKey>
                            </ds:KeyInfo>
                            <CipherData>
                              <CipherValue>' . $this->credentials['security_token1'] . '</CipherValue>
                            </CipherData>
                          </EncryptedData>
                        </o:Security>
                      </s:Header>
                      ';

    return $headerXML;
  }

  /**
   * Use curl to fetch POST SOAP request and fetch response
   * amended to force SSL version and set FOLLOWLOCATION
   */
  function GetSOAPResponse($postUrl, $request) {
    // Set up headers.
    $domain = parse_url($postUrl, PHP_URL_HOST);
    $headers = array(
      "Host: " . $domain,
      'Connection: Keep-Alive',
      "Content-type: application/soap+xml; charset=UTF-8",
    );

    $cURLHandle = curl_init();
    // Request that MS Dynamics send us gzipped data, which considerably speeds
    // up data transfer. Should the instance of MSD not support this, it falls
    // back to normal data (supposedly!).
    // Note that curl takes care of unzipping the data.
    curl_setopt($cURLHandle, CURLOPT_ENCODING, "gzip");
    curl_setopt($cURLHandle, CURLOPT_URL, $postUrl);
    curl_setopt($cURLHandle, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($cURLHandle, CURLOPT_TIMEOUT, variable_get('clients_connection_ms_dynamics_soap_curl_timeout', 120));
    curl_setopt($cURLHandle, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($cURLHandle, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($cURLHandle, CURLOPT_SSLVERSION, 3);
    curl_setopt($cURLHandle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_setopt($cURLHandle, CURLOPT_HTTPHEADER, $headers);
    curl_setopt($cURLHandle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_setopt($cURLHandle, CURLOPT_POSTREDIR, 3);
    curl_setopt($cURLHandle, CURLOPT_POSTFIELDS, $request);
    $cookie_file = drupal_tempnam('temporary://', 'curl_cookie');
    curl_setopt($cURLHandle, CURLOPT_COOKIEJAR, $cookie_file);
    $response = curl_exec($cURLHandle);
    curl_close($cURLHandle);

    return $response;
  }

}
