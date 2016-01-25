<?php

/**
* Exception thrown by PAINT when the PureResponse application terminates
* unexpectedly.
*/
class PaintSystemException extends Exception {  	
}

/**
* Exception thrown by PAINT when your code attempts to access an action
* for which it has insufficient privileges
*/
class PaintSecurityException extends Exception {  	
}

/**
* Exception thrown when PAINT encounters validation errors during an update or store.
* Errors are stored in a hashtable keyed by a unique reference for the field
* or particular validation error.
*/
class PaintValidationException extends Exception {
	/** Array of errors keyed on the error field/name **/
	protected $errors;
	/**
	 * Construct the exception with a hashtable of errors
	 */
  public function __construct($errors) {
		$this->errors = $errors;
		parent::__construct("Validation error");
	}
	/**
	 * Return the hash table or errors 
	 */
	public function getErrors()	{
			return $this->errors;
	}
}

/**
*
* Utility class for creating and re-using a session within PAINT.  Utility
* methods are included to implement the standard actions e.g. creating and
* email or uploading a list.
*/
class PaintSession {
	/** WSDL URL **/
	protected $wsdlUrl = "http://paint.pure360.com/paint.pure360.com/ctrlPaint.wsdl";

	/** Id to link requests together between logging in and out **/
	protected $contextId;

	public $loginName;
	public $password;
	
	/* For proxy */
	public $proxy_host;
	public $proxy_port;
	public $proxy_username;
	public $proxy_password;

	/** Hashtable containing the data for this context **/
	protected $contextData;

	// Construct the class with the relevant credentials
  public function __construct()	{
			// ** ENTER YOUR CREDENTIALS HERE **
			$this->loginName = "username.sys";
			$this->password = "password";
	}

	/**
	* Log into the Pure system and obtain a context id.
	* This is automatically called from the class constructor 
	* and so is probably only required if manually logging
	* out and then back in again. 
	*/    
	public function login() {
		$entityInput = null;
		$resultOutput = null;
		// Sanity check that the user name and password have been set correctly
		if ($this->loginName == "yourUsername" && $this->password == "yourPassword") {
				throw new PaintSystemException("You have not set the user name and password ".
																				"for your account.  Please see the PaintSession ".
																				"class in the com.pure360.paint namespace and ".
																				"update the values set in the constructor.");
		}

		// Create argument data into a hashtable
		$entityInput = array();
		$entityInput["userName"] = $this->loginName;
		$entityInput["password"] = $this->password;

		// Login 
		$resultOutput = $this->sendRequest("bus_facade_context", "login", $entityInput, null);

		// Store the context id on the class
		$this->contextData = $resultOutput;
		$this->contextId = $resultOutput["bus_entity_context"]["beanId"];        
	}

	/**
	* Log out of the current context.  This will remove the context id and you won't be
	* able to issue any other requests after this other than login.
	*/
	public function logout() {
		// No data needs to be sent to this request
		$this->sendRequest("bus_facade_context", "logout", null, null);

		$this->contextId = null;
	}

	/**
	* Search for an entity by a set of search parameters and return the key fields for
	* the entity or entities found.
	*/
	public function search($facadeBean, $searchParameters) {
		$resultOutput = null;
		$searchBean = str_replace("bus_facade", "bus_search", $facadeBean);

		// First search to see if an email already exists with this name (assumes no SMS on the account)
		$resultOutput = $this->sendRequest($facadeBean, "search", $searchParameters, null);

		// Access the data using the search bean name NOT the facade bean name
		$resultOutput = $resultOutput[$searchBean];
		$resultOutput = $resultOutput["idData"];

		return $resultOutput;
	}
    
	/**
	* Search for an entity using search parameters but ensure that only an exact match
	* for all parameters is returned.
	*/
  public function searchExactMatch($facadeBean, $searchParameters) {
		$searchResults = null;
		$exactMatchData = null;
		$entityBean = str_replace("bus_facade", "bus_entity", $facadeBean);
			
		// Perform the general search to obtain a list of ids
		$searchResults = $this->search($facadeBean, $searchParameters);
		
		// Loop through the ids and call the load method until we find an exact match
		foreach($searchResults as $loadInput) {
			$beanInst = $this->sendRequest($facadeBean, "load", $loadInput, null);
			
			if(!empty($beanInst)) {
				$exactMatch = true;
				
				foreach($searchParameters as $paramName=>$paramValue) {
					$beanData = $beanInst[$entityBean];
					
					if(!isset($beanData[$paramName]) || $beanData[$paramName] !== $paramValue) {
						$exactMatch = false;
					}
				}
				
				if($exactMatch) {
					$exactMatchData = $beanData;
				}
			}
		}
		
		if(empty($exactMatchData)) {
			throw new PaintValidationException(array("searchExactMatch"=>"No exact match found for $facadeBean"));
		}
		
		return $exactMatchData;
	}

	/**
	* Send a request to PAINT passing the required parameters
	* and returning a hashtable of hashtables as the result
	* if successful, or throw an exception if unsuccessful.
	*/
	public function sendRequest($className, $processName, $entityInput, $processInput) {
		$client = null;
		$resultOutput = null;

		// Check that the context is valid
		if ($processName != "login" && $this->contextId == null) {
			throw new PaintSystemException("No context available for this request");
		}        
		if (!empty($this->proxy_host)) {
			$soapOptions = array (
				"proxy_host"     => $this->proxy_host,
				"proxy_port"     => $this->proxy_port,
				"proxy_login"    => $this->proxy_username,
				"proxy_password" => $this->proxy_password,
				"trace"          => "0"
			);
		}
		else {
			$soapOptions = array("trace" => "0");
		}
		$client = new SoapClient($this->wsdlUrl, $soapOptions);
		$resultOutput = $client->handleRequest($this->contextId,$className,$processName,$entityInput,$processInput);   
		switch ($resultOutput["result"]) {
			case "success":
				if (!empty($resultOutput["resultData"])) {
					$resultOutput = $resultOutput["resultData"];
				}
				else {
					// Update requests return no data back
					$resultOutput = array();
				}
				break;
			case "bean_exception_validation":
				throw new PaintValidationException($resultOutput["resultData"]);
			case "bean_exception_security":
				throw new PaintSecurityException($resultOutput["resultData"]);
			case "bean_exception_system":
				throw new PaintSystemException($resultOutput["resultData"]);
			default:
				throw new Exception("Unhandled exception thrown from PAINT");
		}
		return $resultOutput;
	}

	/**
	* Return the data from the context.  This data is loaded whe
	* logging in and contains details about the login, profile and group
	*/
	public function getContextData() {
			return $this->contextData;
	}

	/**
	* Convert a result hashtable into a string for debug purposes
	*/
	public function convertResultToDebugString($result) {
		$resultStr = "";
		foreach($result as $tmpKey=>$tmpValue) {
				if ($tmpValue != null && is_array($tmpValue)) {
					$resultStr = $resultStr."<BR/>---><BR/>[Nested Hashtable Key] [".$tmpKey."]".$this->convertResultToDebugString($tmpValue);
				}
				else {
					$resultStr = $resultStr."<BR/>Key [".$tmpKey."] Value [".$tmpValue."]";
				}
		}
		$resultStr = $resultStr."<BR/><---<BR/>";
		return $resultStr;
	}
}
/**
* Class holding short-cuts to the different operations this 
* application will make using PAINT.  This inherits from PaintSession
* which handles the login and context id plus provides handler methods.
*/
class PaintMethods extends PaintSession {
	/**
	* Create a new message on the account.  This function isolates some of the basic features
	* of a message.  More complicated features must be accessed using the sendRequest
	* function and using the data dictionary to discover the required data fields
	*/
	public function createEmail($messageName, $subject, $bodyPlain, $bodyHtml) {
		$messageFound = false;
		$searchInput = array();
		$deleteInput = array();
		$emailInput = array();
		$resultOutput = null;

		// Put the data for the email into a hashtable keyed on the field names taken from the 
		// data dictionary
		$emailInput["messageName"]	= $messageName;
		$emailInput["subject"]		= $subject;
		$emailInput["bodyPlain"]	= $bodyPlain;
		$emailInput["bodyHtml"]		= $bodyHtml;
		// Search to see if an email already exists with this name (assumes no SMS on the account)
		if(!empty($messageName)) {
			$searchInput["messageName"] = $messageName;
			$resultOutput = $this->search("bus_facade_campaign_email", $searchInput);
		}

		if (!empty($resultOutput)) {
			// Loop through the results in case there are other messages that contain the 
			// same string within their message name
			for ($index = 0; $index < count($resultOutput) && !$messageFound; $index++) {
				$loadOutput = null;
				$loadOutputFields = null;
				$loadInput = $resultOutput[$index];

				// Use the id data returned from the search to load the specific email
				$loadOutput = $this->sendRequest("bus_facade_campaign_email", "load", $loadInput, null);
				$loadOutputFields = $loadOutput["bus_entity_campaign_email"];
				
				if ($loadOutputFields["messageName"] == $messageName) {
					$resultOutput = $loadOutput;
					$messageFound = true;
				}
			}
		}
			
		if(!$messageFound) {
			// No existing message found so we'll create a new one
			$resultOutput = $this->sendRequest("bus_facade_campaign_email", "create", null, null);
		}
		// Whether we loaded the bean or created a new one, we'll have a bean id now. 
		// Put the bean id along with the rest of the data and request to store. After
		// this the bean will have been cleared away.
		$emailInput["beanId"] = $resultOutput["bus_entity_campaign_email"]["beanId"];
		$resultOutput = $this->sendRequest("bus_facade_campaign_email", "store", $emailInput, null);
		return $resultOutput;
	}

	/**
	* Send a request to upload a new list.  If the list already exists then we'll
	* remove it first and create a new one.  Note that the list data should be a CSV
	* string starting with a comma separated list of field names. At least one header 
	* must be either "email" or "mobile".
	*/
	public function createList($listName, $listDataSource, $notifyUri) {
		$listFound = false;
		$searchInput = array();
		$listInput = array();
		$resultOutput = null;

		// Search to see if an email already exists with this name (assumes no SMS on the account)
		$searchInput["listName"] = $listName;
		$resultOutput = $this->search("bus_facade_campaign_list", $searchInput);

		// If we found the correct list then remove it first
		if (!empty($resultOutput)) {
			// Loop through the results in case there are other messages that contain the 
			// same string within their message name
			for ($index = 0; $index < count($resultOutput) && !$listFound; $index++)
			{
				$loadOutput = null;
				$loadOutputFields = null;
				$loadInput = $resultOutput[$index];

				// Use the id data returned from the search to load the specific email
				$loadOutput = $this->sendRequest("bus_facade_campaign_list", "load", $loadInput, null);
				$loadOutputFields = $loadOutput["bus_entity_campaign_list"];
				if ($loadOutputFields["listName"] == $listName) {
					$removeInput = array();

					// Remove the existing list
					$removeInput["beanId"] = $loadOutputFields["beanId"];
					$this->sendRequest("bus_facade_campaign_list", "remove", $removeInput, null);
					$listFound = true;
				}
			}
		}

		// Put the data for the list into the hashtable.  Note that the header row needs to
		// be split out and is used to create the custom field names.  
		$listInput["listName"] = $listName;

		if ($notifyUri != null) {
			$listInput["uploadFileNotifyEmail"] = $notifyUri;
		}

		if ($listDataSource != null) {
			$endFirstRowPos = 0;
			$customFieldCount = 0;
			$firstRow = null;
			$fieldNames = null;
			
			// Extract the first row from the list data
			$endFirstRowPos = strpos($listDataSource, "\n");
			
			if ($endFirstRowPos !== false) {
					$firstRow = substr($listDataSource, 0, $endFirstRowPos);
			}

			// Split this into the different column names
			$fieldNames = explode(",", $firstRow);

			// Loop through each column name and add them to the custom field
			// names list until all have been added or we have reached the maximum 
			// allowed
			for ($index = 0; ($index < count($fieldNames) & $customFieldCount <= 10); $index++) {
				$fieldName = $fieldNames[$index];

				switch ($fieldName) {
					case "email":
						$listInput["emailCol"] = $index;
						break;

					case "mobile":
						$listInput["mobileCol"] = $index;
						break;

					default:
						$fieldColStr = "field".$index."Col";
						$fieldNameStr = "field".$index."Name";

						// Replace illegal spaces
						$fieldName = str_replace(' ', '_', $fieldName);

						// Add data to the list so PAINT knows about the fields
						$listInput[$fieldColStr] = $index;
						$listInput[$fieldNameStr] = $fieldName;

						// Keep count so we don't go over ten (PAINT would ignore them)
						$customFieldCount++;
						break;
				}
			}
		}

		// Use the "paste" field to pass in the string of data.  File uploads are not currently
		// supported via PAINT.
		$listInput["pasteFile"] = $listDataSource;

		// Now create the new list bean for us to reference and load with data
		$resultOutput = $this->sendRequest("bus_facade_campaign_list", "create", null, null);

		// Set the data onto the list and save to the system.  Note that the bean will
		// bean cleared away from the session after this
		$listInput["beanId"] = $resultOutput["bus_entity_campaign_list"]["beanId"];
		$resultOutput = $this->sendRequest("bus_facade_campaign_list", "store", $listInput, null);

		return $resultOutput;
	}

	/**
	* Schedule a delivery to the named list and message to run immediately
	*/
	public function createDelivery($listName, $messageName) {
		$deliveryDtTmStr = null;
		$deliveryDtTm = null;
		$deliveryInput = array();
		$listSearchInput = array();
		$msgSearchInput = array();
		$resultOutput = null;
		$listData = null;
		$messageData = null;

		// Request to create a new delivery record.  This wil return with a list of 
		// messages and lists so we can use those lists to get the ids of the 
		// list and message we want to send to
		$resultOutput = $this->sendRequest("bus_facade_campaign_delivery", "create", null, null);
		$resultOutput = $resultOutput["bus_entity_campaign_delivery"];

		// Find the list id based on the name
		$listSearchInput["listName"] = $listName;
		$listData = $this->searchExactMatch("bus_facade_campaign_list", $listSearchInput);
		$deliveryInput["listIds"] = array($listData["listId"]);

		// Loop through the messages to find the ID that matches the name we've received
		$msgSearchInput["messageName"] = $messageName;
		$messageData = $this->searchExactMatch("bus_facade_campaign_email", $msgSearchInput);
		$deliveryInput["messageId"] = $messageData["messageId"];

		// Finally, add the a time five minutes into the future as the scheduled time
		$deliveryDtTm = strtotime("5 minutes", time());        
		$deliveryDtTmStr = date("d/m/Y H:i", $deliveryDtTm);
		$deliveryInput["deliveryDtTm"] = $deliveryDtTmStr;

		// Set the data onto the list and save to the system.  Note that the bean will
		// bean cleared away from the session after this
		$deliveryInput["beanId"] = $resultOutput["beanId"];
		$resultOutput = $this->sendRequest("bus_facade_campaign_delivery", "store", $deliveryInput, null);

		return $resultOutput;
	}

	/**
	* Load an existing delivery using a reference number.  High level report data will be returned
	*/
	public function loadDelivery($deliveryId)	{
		$entityInput	= null;
		$resultOutput	= null;
		
		$entityInput = array("deliveryId" => $deliveryId);

		// Use the unique id to retrieve the delivery and return the bean data
		$resultOutput = $this->sendRequest("bus_facade_campaign_delivery", "load", $entityInput, null);
		$resultOutput = $resultOutput["bus_entity_campaign_delivery"];		
		
		return $resultOutput;
	}
	
	/**
	* Create a new one-to-one delivery to a specified email address and passing
	* any custom data that should merge into the message.  Note that the message must 
	* already exist in the account.
	*/
	public function createOne2One($emailTo, $messageName, $customData) {
		$entityInput	= array();
		$processInput	= array();
		$resultOutput	= null;

		// Put the data for the email into a hashtable keyed on the field names taken from the 
		// data dictionary
		$processInput["message_messageName"]	= $messageName;
		$entityInput["toAddress"]				= $emailTo;

		// Load the string of custom data as separate arguments into the 
		// customData parameter
		if($customData != null) {
			$customDataAll = array();
			$customDataRows = null;

			// Split into rows and load into the input hashtable
			$customDataRows = explode("\r\n", $customData);

			for($index=0; $index<count($customDataRows); $index++) {
				$fieldName = null;
				$customDataField = null;

				// Split name value and load
				$customDataField = explode("=", $customDataRows[$index]);
				$fieldName = $customDataField[0];

				// Add the value to pass in as custom data for the message
				if (count($customDataField) == 2 && $fieldName!="")
				{
						$fieldValue = $customDataField[1];
						$customDataAll[$fieldName] = $fieldValue;
				}
			}

			if (!empty($customDataAll)) {
					$entityInput["customData"] = $customDataAll;
			}
		}

		// Create a blank one2one
		$resultOutput	= $this->sendRequest("bus_facade_campaign_one2one", "create", $entityInput, $processInput);
		$resultOutput	= $resultOutput["bus_entity_campaign_one2one"];
		$entityInput	= array("beanId" => $resultOutput["beanId"]);

		// Update with data and save
		$resultOutput = $this->sendRequest("bus_facade_campaign_one2one", "store", $entityInput, null);

		return $resultOutput;
	}
    
	/**
	* Retrieve a batch of event notifications from a PureResponse profile.  Note that the profile must be set-up 
	* to capture these events, and for click and open event notifications, the campaign email must be set-up
	* to capture these events too.
	*
	* Data is returned in a base 64 encoded cvs string so it requires decoding before it can be used.
	*
	*/
	public function retrieveEventNotifications($notificationTypes, $maxNotifications, $markAsReadInd, $customFieldNames) {
		$processInput		= null;
		$resultOutput		= null;
		$customFieldNames	= (!empty($customFieldNames)? $customFieldNames: array());
		
			$processInput = array(	"notificationTypes" => $notificationTypes,
							"maxNotifications"	=> $maxNotifications,
							"markAsReadInd"		=> $markAsReadInd,
							"customFieldNames"	=> $customFieldNames);

			$resultOutput = $this->sendRequest("bus_facade_eventNotification", "getBatch", null, $processInput);
			$resultOutput = $resultOutput["bus_entity_eventNotificationBatch"];

			return $resultOutput;
	}    
	/**
	* Append ONE person to ONE list
	*/
	public function appendList($listName, $listDataSource, $notifyUri) {
		$listFound = false;
		$searchInput = array();
		$listInput = array();
		$resultOutput = null;

		// Put the data for the list into the hashtable.  Note that the header row needs to
		// be split out and is used to create the custom field names.  
		$listInput["listName"] = $listName;
		$listInput["uploadTransactionType"] = "APPEND";

		if ($notifyUri != null) {
			$listInput["uploadFileNotifyEmail"] = $notifyUri;
		}

		if ($listDataSource != null) {
			$endFirstRowPos = 0;
			$customFieldCount = 0;
			$firstRow = null;
			$fieldNames = null;
			
			// Extract the first row from the list data
			$endFirstRowPos = strpos($listDataSource, "\n");
				
			if ($endFirstRowPos !== false) {
				$firstRow = substr($listDataSource, 0, $endFirstRowPos);
				$listDataSource = substr($listDataSource, $endFirstRowPos+1);
			}

			// Split this into the different column names
			$fieldNames = explode(",", $firstRow);

			// Loop through each column name and add them to the custom field
			// names list until all have been added or we have reached the maximum 
			// allowed
			for ($index = 0; ($index < count($fieldNames) & $customFieldCount <= 10); $index++) {
				$fieldName = $fieldNames[$index];

				switch ($fieldName) {
					case "email":
						$listInput["emailCol"] = $index;
						break;

					case "mobile":
						$listInput["mobileCol"] = $index;
						break;

					default:
						$fieldColStr = "field".$index."Col";
						$fieldNameStr = "field".$index."Name";

						// Replace illegal spaces
						$fieldName = str_replace(' ', '_', $fieldName);

						// Add data to the list so PAINT knows about the fields
						$listInput[$fieldColStr] = $index;
						$listInput[$fieldNameStr] = $fieldName;

						// Keep count so we don't go over ten (PAINT would ignore them)
						$customFieldCount++;
						break;
				}
			}
		}

		// Use the "paste" field to pass in the string of data.  File uploads are not currently
		// supported via PAINT.
		$listInput["pasteFile"] = $listDataSource;

		// Now create the new list bean for us to reference and load with data
		$resultOutput = $this->sendRequest("bus_facade_campaign_list", "create", null, null);

		// Set the data onto the list and save to the system.  Note that the bean will
		// bean cleared away from the session after this
		$listInput["beanId"] = $resultOutput["bus_entity_campaign_list"]["beanId"];

		//echo "<h2>List Input Array:</h2><pre>".print_r($listInput, true)."</pre>";

		$resultOutput = $this->sendRequest("bus_facade_campaign_list", "store", $listInput, null);

		return $listInput;
	}
	
	/**
	* Return all dynamic content 
	*/
	public function loadContentItems() {
		$resultOutput	= null;

		$searchInput["contentItemId"] = "";
		$foundItems = $this->search("bus_facade_campaign_contentItem", $searchInput);

		if (!empty($foundItems)) {
			// Loop through the results in case there are other matching items
			for ($index = 0; $index < count($foundItems); $index++) {
				$loadInput = $foundItems[$index];

				// Use the id data returned from the search to load the specific email
				$contentItemData = $this->sendRequest("bus_facade_campaign_contentItem", "load", $loadInput, null);
				$resultOutput[$index] = $contentItemData['bus_entity_campaign_contentItem'];
			}
		}

		return  $resultOutput;
	}

	/**
	* Return content item data
	*/
	public function getContentItem($contentItemId) {
		$resultOutput	= null;

		$loadInput["contentItemId"] = $contentItemId;

		$contentItemData = $this->sendRequest("bus_facade_campaign_contentItem", "load", $loadInput, null);
		$resultOutput = $contentItemData['bus_entity_campaign_contentItem'];

		return $resultOutput;
	}

	/**
	* Delete a dynamic content item
	*/
	public function deleteContentItem($contentItemId) {
		$resultOutput	= null;
		$resultRaw	= null;
		$loadRaw = null;

		// First load the content item so we have a beanId for it
		$loadInput["contentItemId"] = $contentItemId;
		$contentItemData = $this->sendRequest("bus_facade_campaign_contentItem", "load", $loadInput, null);
		
		// Delete the content item by using its beanId
		$removeInput["beanId"] = $contentItemData["bus_entity_campaign_contentItem"]["beanId"];
		$resultRaw = $this->sendRequest("bus_facade_campaign_contentItem", "remove", $removeInput, null);

		$resultOutput = print_r($resultRaw, true);
		
		return $resultOutput;
	}

	/**
	* Creates a dynamic content item
	*/
	public function createContent($clientReference, $contentHtml, $contentPlain) {
		$contentFound = false;
		$searchInput = array();
		$deleteInput = array();
		$contentInput = array();
		$resultOutput = null;

		// Put the data for the email into a hashtable keyed on the field names taken from the 
		// data dictionary
		$contentInput["clientReference"]	= $clientReference;
		$contentInput["contentHtml"]		= $contentHtml;
		$contentInput["contentPlain"]		= $contentPlain;

		// Search to see if an email already exists with this name (assumes no SMS on the account)
		if(!empty($messageName)) {
			$searchInput["clientReference"] = $clientReference;
			$resultOutput = $this->search("bus_facade_campaign_contentItem", $searchInput);
		}

		if (!empty($resultOutput)) {
			// Loop through the results in case there are other dynamic content items with the same reference name
			for ($index = 0; $index < count($resultOutput) && !$contentFound; $index++) {
				$loadOutput = null;
				$loadOutputFields = null;
				$loadInput = $resultOutput[$index];

				// Use the id data returned from the search to load the specific email
				$loadOutput = $this->sendRequest("bus_facade_campaign_contentItem", "load", $loadInput, null);
				$loadOutputFields = $loadOutput["bus_entity_campaign_contentItem"];
				
				if ($loadOutputFields["clientReference"] == $clientReference) {
						$resultOutput = $loadOutput;
						$contentFound = true;
				}
			}
		}
        
		if(!$contentFound) {
				// No existing message found so we'll create a new one
				$resultOutput = $this->sendRequest("bus_facade_campaign_contentItem", "create", null, null);
		}

		// Whether we loaded the bean or created a new one, we'll have a bean id now. 
		// Put the bean id along with the rest of the data and request to store. After
		// this the bean will have been cleared away.
		$contentInput["beanId"] = $resultOutput["bus_entity_campaign_contentItem"]["beanId"];

		$resultOutput = $this->sendRequest("bus_facade_campaign_contentItem", "store", $contentInput, null);

		return $resultOutput;
	}
}
