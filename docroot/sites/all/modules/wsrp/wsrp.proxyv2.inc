<?php

   /*
    * WARNING: HTTP_RAW_POST_DATA empty on multipart requests.....
    * */
  function wsrp_proxy_v2 ()
  {
  	 global $wsrprequest, $user;
  	 
	 $portletInstanceKey= $wsrprequest['portletInstanceKey'];
  	 $portletInstance= wsrp_get_portletinstance($portletInstanceKey);
	      
     if ($wsrprequest['wsrp-urlType']=='resource')
     {
//         syslog(LOG_ERR,"wsrprequest resource: ". print_r($_SERVER,true));
         
         if (isset($wsrprequest['wsrp-preferOperation']))
           $preferResourceID= (boolean)$wsrprequest['wsrp-preferOperation'];
         else
           $preferResourceID= false;
         
         
     	 if (!isset($wsrprequest['wsrp-resourceID'])||(!$preferResourceID&&isset($wsrprequest['wsrp-url']))) // use HTTP
     	 {
     	 	
	         if ($_SERVER['REQUEST_METHOD']=='GET')
	         {
                        $queryString= '';

                        while (list($paramName,$paramValue)=each($_GET))
		            if ($paramName!='wsrprequest'&&$paramName!='q')
			        $queryString.='&'.$paramName.'='.urlencode($paramValue);

			    if (substr_count($wsrprequest['wsrp-url'],'?')==0)
			     $queryString= '?'.substr($queryString,1);

                        //syslog(LOG_ERR,'proxy v2 url: '.print_r($wsrprequest,true));
			     
	         	$response= http_parse_message(http_get($wsrprequest['wsrp-url'].$queryString));
	         }
	         elseif ($_SERVER['REQUEST_METHOD']=='POST')
	         {
	         	 $fields= array();
			     $files= array();
	         	 
			     while (list($paramName,$paramValue)=each($_POST))
			        if ($paramName!='wsrprequest'&&$paramName!='q')
			          $fields[$paramName]= $paramValue;
			        
                 if (isset($_FILES)&&count($_FILES)>0)
                 {
			   	    
			   	    foreach ($_FILES as $name => $file)
			   	       if ($file["name"])
			   	       {
			   	          if (!$file["error"])
			   	          {
			   	          	rename($file["tmp_name"],$file["tmp_name"].':'.$file["name"]);
			   	          	
			                $files[]= array('name' => $name,
							   		        'type' => $file["type"],
									        'file' => $file["tmp_name"].':'.$file["name"]);
			   	          }
			              else
			                 form_set_error('', t('Cannot upload file (may be it exceeds the maximum allowed upload size): %file', array('%file' => $file["name"])));
			              
			           }
                 }
                 	        
	         	 $response= http_parse_message(http_post_fields($wsrprequest['wsrp-url'],$fields,$files));
	         }
	
	         if (isset($wsrprequest['wsrp-requiresRewrite'])&&$wsrprequest['wsrp-requiresRewrite'])
	         {
	            $body= $portletInstance->consumer->urlRewrite($response->body,$portletInstance->key);
	         }
	         else
	            $body= $response->body;
	         
	         header('Content-Length: '.strlen($body));
	         header('Content-Type: '.$response->headers["Content-Type"]);

                 if (isset($response->headers["Content-Disposition"]))
	           header('Content-Disposition: '.$response->headers["Content-Disposition"]);
	         
	         echo $body;
	           	
     	 }
     	 else // use getResource
     	 {
			  try
			  {
					  
				   	  $resourceParams= wsrp_get_markupparams($portletInstance);
				   	  
				      $resourceParams["resourceID"]= $wsrprequest['wsrp-resourceID'];
				      
				   	  if (isset($wsrprequest['wsrp-resourceState']))
				   	   $resourceParams["resourceState"]= $wsrprequest['wsrp-resourceState'];
				   	  
				   	  if (isset($wsrprequest['wsrp-resourceCacheability']))
				   	   $resourceParams["resourceCacheability"]= $wsrprequest['wsrp-resourceCacheability'];
				   	   
					  while (list($paramName,$paramValue)=each($_GET))
					    if ($paramName!='wsrprequest')
					      $resourceParams["formParameters"][]=array("name"=> $paramName, "value"=>$paramValue);
				      
					  while (list($paramName,$paramValue)=each($_POST))
					    if ($paramName!='wsrprequest')
						  $resourceParams["formParameters"][]=array("name"=> $paramName, "value"=>$paramValue);
				   	   				      
		   	          $uploadContexts= array();
						  
				      $response= $portletInstance->consumer->getResource($portletInstance->consumer->registrationContext, 
				                                                         $portletInstance->portletContext, 
				                                                         wsrp_get_runtimecontext($portletInstance),
				                                                         wsrp_get_usercontext(),
				                                                         $resourceParams);
				      

                                  syslog(LOG_ERR,"wsrprequest response: ". print_r($response,true));

		                  if (isset($response->sessionContext))
 		   	            $portletInstance->sessionContext=  $response->sessionContext;
				      
			      	  if (isset($response->portletContext))
			   	    $portletInstance->portletContext=  $response->portletContext;

			   	  if (isset($response->resourceContext->itemString))
			   	    $body= $response->resourceContext->itemString;
			   	  else
			   	    $body= $response->resourceContext->itemBinary;

                              if (isset($response->resourceContext->requiresRewriting)&&$response->resourceContext->requiresRewriting) 
                                $body= $portletInstance->consumer->urlRewrite($body,$portletInstance->key);
				   	    
  			  wsrp_set_portletinstance($portletInstance);

			      header('Content-Length: '. strlen($body));
		              header('Content-Type: '. $response->resourceContext->mimeType);
				   	  
		              echo $body;  	
			  }
			  catch (Exception $e)
			  {
			  	 syslog(LOG_ERR,"cannot get resource for portlet_id: ". $portlet_id ." error: ". $e->getMessage() . " wsrprequest: " . print_r($wsrprequest,true));
			  }
	  	
     	 }
     }
  }

?>
