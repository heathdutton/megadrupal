<?php

	class wsrpconsumerv1
	{
		
		private $client;
		private $wsdl;
		private $httpsPort;
		public $version= '1';
		public $registrationContext;
		
		function __construct($wsdl)
		{
			$this->wsdl= $wsdl;
			$this->httpsPort= (int)variable_get('wsrp_consumer_httpsPort','443');
			$this->client= new SoapClient($this->wsdl,array("trace"=>1));
		}



		public function register()
		{
			$args= array();
			$args["registrationData"]["consumerName"]= variable_get('wsrp_consumer_name','');
			$args["registrationData"]["consumerAgent"]= "drupalwsrp1.0";
			$args["registrationData"]["methodGetSupported"]= true;       
			$args["registrationData"]["consumerModes"] = array('wsrp:view','wsrp:edit','wsrp:help','wsrp:preview','wsrp:solo');  
			$args["registrationData"]["consumerWindowStates"] = array('wsrp:normal');
			$args["registrationData"]["consumerUserScopes"] = array();
			$args["registrationData"]["customUserProfileData"] = array();
			$args["registrationData"]["registrationProperties"] = array();
			$args["registrationData"]["extensions"] = array();
			return $this->client->register($args["registrationData"]);
		}

		public function getLastRequest()
		{
		   return 	$this->client->__getLastRequest();
		}
		
		public function deregister($registrationContext)
		{
            return $this->client->deregister($registrationContext);
		}


		public function getServiceDescription($registrationContext = FALSE, $desiredLocales = FALSE,  $portletHandles = FALSE, $userContext = FALSE) 
		{
			//get available services
			$args= array();

            if ($registrationContext)
              $args["registrationContext"]= $registrationContext;
              
            if ($desiredLocales)
              $args["desiredLocales"]= $desiredLocales;
              
            if ($portletHandles)
              $args["portletHandles"]= $portletHandles;
              
            if ($userContext)
              $args["userContext"]= $userContext;
              
              
			return $this->client->getServiceDescription($args);
		}

        public function initCookie($registrationContext)
        {
			$args= array();
            $args["registrationContext"]= $registrationContext;

			return $this->client->initCookie($args);
        }

		public function getMarkup($registrationContext, $portletContext, $runtimeContext, $userContext, $markupParams)
		{
			$request["registrationContext"]= $registrationContext;
			$request["portletContext"]= $portletContext;
			$request["runtimeContext"]= $runtimeContext;
			$request["userContext"]= $userContext;
			$request["markupParams"]= $markupParams;
			return $this->client->getMarkup($request);
		}
		
		public function getResource($registrationContext, $portletContext, $runtimeContext, $userContext, $resourceParams)
		{
			$request["registrationContext"]= $registrationContext;
			$request["portletContext"]= $portletContext;
			$request["runtimeContext"]= $runtimeContext;
			$request["userContext"]= $userContext;
			$request["resourceParams"]= $resourceParams;
			return $this->client->getResource($request);
		}
		

		public function validateMode($mode)
		{
			switch ($mode)
			{
				case "wsrp:edit":
				case "edit":	
					$mode="wsrp:edit";
					break;
				case "wsrp:view":
				case "view":
					$mode="wsrp:view";
					break;
				case "wsrp:help":
				case "help":
					$mode="wsrp:help";
					break;
				default:
					$mode="wsrp:view";
					break;
			}
			return $mode;
		}


		public function performBlockingInteraction($registrationContext,$portletContext,$runtimeContext,$userContext,$markupParams,$interactionParams)
		{
			$request["registrationContext"]= $registrationContext;
			$request["portletContext"]= $portletContext;
			$request["runtimeContext"]= $runtimeContext;
			$request["userContext"]= $userContext;
			$request["markupParams"]= $markupParams;
			$request["interactionParams"]= $interactionParams;
			
			$response= $this->client->performBlockingInteraction($request);
			
			//syslog(LOG_ERR,"response: ". print_r($response,true));
			
			return $response; 
		}
		
		/**
		*  urlRewrite - rewrite urls
		*  @param	string	markup with url placeholders
		*  @return	string	markup with rewritten urls
		**/
		public function urlRewrite($markup,$portletInstanceKey)
		{
			$this->translateResource($portletInstanceKey,true);
			return preg_replace_callback('"wsrp_rewrite\?(.*)/wsrp_rewrite"', array($this,'translateResource'),$markup);
		}


		/**
		*  translateResource - translate url placeholders to real urls
		*  @param	string	placeholder url
		*  @return	string	rewritten url
		**/
		private function translateResource($match, $init= FALSE)
		{
			static $portletInstanceKey;
			
			if ($init)
			{
				$portletInstanceKey= $match;
				return;
			}
			
			$resourceUri= $match[1];
			//translate encoded wsrp-url to a valid url
			$request["portletInstanceKey"]= $portletInstanceKey;
			
			$paramList=explode("&amp;",$resourceUri);
			while (list($index,$data)=each($paramList))
			{
				$keyvalue=explode("=",$data);
				$parameters[$keyvalue[0]]=urldecode($keyvalue[1]);
			}
			
			//syslog(LOG_ERR, print_r($parameters,true));
			
			$request["wsrp-urlType"]= $parameters["wsrp-urlType"];
			
			if (isset($parameters["wsrp-interactionState"]))
			   $request["wsrp-interactionState"]= $parameters["wsrp-interactionState"];
			
			if (isset($parameters["wsrp-navigationalState"]))
			   $request["wsrp-navigationalState"]= $parameters["wsrp-navigationalState"];
			   
			if (isset($parameters["wsrp-navigationalValues"]))
			{
			    $request["wsrp-navigationalValues"]= array();
				$paramList=explode("&",$parameters["wsrp-navigationalValues"]);
				while (list($index,$data)=each($paramList))
				{
					$keyvalue=explode("=",$data);
					$request["wsrp-navigationalValues"][]= array("name"=> $keyvalue[0], "value"=> $keyvalue[1]);
				}
			}

			//syslog(LOG_ERR, print_r($request,true));
			$currentUrl= split("\\?",$_SERVER["REQUEST_URI"]);
			$currentUrl= $currentUrl[0];
			//syslog(LOG_ERR, $currentUrl);
			
			switch ($parameters["wsrp-urlType"])
			{
				case "resource": 
					$this->setRequestParam($request,$parameters,"wsrp-url");
					$this->setRequestParam($request,$parameters,"wsrp-resourceID");
					$this->setRequestParam($request,$parameters,"wsrp-requiresRewrite");
					$this->setRequestParam($request,$parameters,"wsrp-preferOperation");
					$this->setRequestParam($request,$parameters,"wsrp-resourceState");
					$this->setRequestParam($request,$parameters,"wsrp-requiresRewrite");
					$this->setRequestParam($request,$parameters,"wsrp-resourceCacheability");
					$resourceUrl= base_path(). 'wsrp/proxy/v2?wsrprequest='.urlencode(base64_encode(serialize($request)));
					break;
				
				case "render":
					$this->setRequestParam($request,$parameters,"wsrp-mode");
					$this->setRequestParam($request,$parameters,"wsrp-windowState");
					$resourceUrl= $currentUrl. '?wsrprequest='.urlencode(base64_encode(serialize($request)));
					break;
					
				case "blockingAction":
					$this->setRequestParam($request,$parameters,"wsrp-mode");
					$this->setRequestParam($request,$parameters,"wsrp-windowState");
					$resourceUrl= $currentUrl. '?wsrprequest='.urlencode(base64_encode(serialize($request)));
					break;
					
				default:
					$resourceUrl=$resourceUri;
					break;
			}
			//syslog(LOG_ERR, $resourceUrl);
			
			if (isset($parameters["wsrp-fragmentID"]))
			  $resourceUrl.= "#".$parameters["wsrp-fragmentID"];

			if (isset($parameters["wsrp-secureURL"]))
			{
			   if ($parameters["wsrp-secureURL"]=="true")
			   {
			   	 $request["wsrp-secureURL"]= $parameters["wsrp-secureURL"];
			     $resourceUrl= "https://". $SERVER["HTTP_HOST"] . ($this->httpsPort==443 ? "" : ":".$this->httpsPort) . $resourceUrl;
			   }
			}
			  
			return $resourceUrl;
		}

	    private function setRequestParam(&$request,&$parameters,$name)
	    {
	       if (isset($parameters[$name]))
		      $request[$name]= $parameters[$name];
	    }
	    
	}

?>
