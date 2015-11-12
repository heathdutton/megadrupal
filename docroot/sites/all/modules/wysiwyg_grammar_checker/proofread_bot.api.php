<?php
	
	function proofread_bot_check_api($text) {
		
		global $base_url;
		global $user;
		
		//print_r($user);
		if ($base_url == "http://proofreadbot.com" OR $base_url == "https://proofreadbot.com") 	{
			$api = new DrupalREST($base_url.'/server/');
			$api->debug = TRUE;
			
			if (user_is_logged_in())	{
				$api->session = session_name()."=".$user->sid;
				$api->login();
			}
			//print_r($api);
		}
		else {
			if (!variable_get('proofread_bot_username'))
				return "Login failed, please check your Proofread Bot username and password in the <a href=\"". $base_url."/admin/config/content/proofread_bot\">admin screen</a>.";	
				
			if (!variable_get('proofread_bot_password'))
				return "Login failed, please check your Proofread Bot username and password in the <a href=\"". $base_url."/admin/config/content/proofread_bot\">admin screen</a>.";					

			$api = new DrupalREST('http://proofreadbot.com/server/', variable_get('proofread_bot_username'), variable_get('proofread_bot_password'), TRUE);
			$api->login();
			
			//session is set to "=" if login failed :D, kind of hackish
			//print_r($api);
			if ($api->session == "=")
			return "Login failed, please check your Proofread Bot username and password in the <a href=\"". $base_url."/admin/config/system/proofread_bot\">admin screen</a>.";
			
		}
		
		$node = new stdClass();
		$node->type = 'proofreading';
		//$node->language = "und";
		//if ($passed_data["type"] == "Full")
		//	$node->field_full[$node->language][0] = 1;
		//$node->field_depth[$node->language][0] = 2;
		
		$node->body["und"][0]["value"] = $text;
		$node->body["und"][0]["format"] = "filtered_html";
		
		
		$result = $api->createNode(array('node' => $node));

		//print_r($result);
		//$result->nid = test nid here (must be integer;
		
		/*
			lets test this crappy encoding issue...	
			$result = $api->retrieveNode("700977");
		*/
		//print_r($result);
		//$result_body = json_decode($result->body);
		if ($result->ErrorCode && $result->body) {
			$form_error = json_decode($result->body);
			//print_r($result_body);
			return addslashes($form_error->form_errors->content);
			//result body without json is only used for system errors?
			//return $result->body;
		}
		else {
			$node = $api->retrieveNode($result->nid);
			
			/*
				print "<pre>";
				print_r($node);
				print "</pre>";
			*/
			return $node;
		}
		
	}
	
	
	class DrupalREST {
		var $username;
		var $password;
		var $session;
		var $endpoint;
		var $debug;
		
		function __construct($endpoint, $username, $password, $debug)
		{
			$this->username = $username;
			$this->password = $password;
			//TODO: Check for trailing slash and fix if needed
			$this->endpoint = $endpoint;
			$this->debug = $debug;
		}
		
		function login()
		{
			if (!isset($this->session)) {
				$ch = curl_init($this->endpoint . 'user/login.json');
				$post_data = array(
				'username' => $this->username,
				'password' => $this->password,
				);
				$post = http_build_query($post_data, '', '&');
				curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
				curl_setopt($ch, CURLOPT_HEADER, false);
				curl_setopt($ch, CURLOPT_POST, true);
				curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
				curl_setopt($ch, CURLOPT_HTTPHEADER,array (
				"Accept: application/json",
				"Content-type: application/x-www-form-urlencoded"
				));
				$response = json_decode(curl_exec($ch));
				
				curl_close($ch);
				
				//Save Session information to be sent as cookie with future calls
				$this->session = $response->session_name . '=' . $response->sessid;
				}
				
			// GET CSRF Token
			$ch = curl_init();
			curl_setopt_array($ch, array(
			CURLOPT_RETURNTRANSFER => 1,
			CURLOPT_URL => 'http://proofreadbot.com/services/session/token',
			));
			curl_setopt($ch, CURLOPT_COOKIE, "$this->session"); 
			// $csrf_token = curl_exec($ch);
			//print_r($csrf_token);
			//print "hehe";
			
			$ret = new stdClass;
			
			$ret->response = curl_exec($ch);
			$ret->error    = curl_error($ch);
			$ret->info     = curl_getinfo($ch);
			
			//curl_close($ch);
			
			//print_r($ret->response);
			$this->csrf_token = $ret->response;
		}
		
		// Retrieve a node from a node id
		function retrieveNode($nid)
		{
			//Cast node id as integer
			$nid = (int) $nid;
			$ch = curl_init($this->endpoint . 'node/' . $nid );
			curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
			curl_setopt($ch, CURLOPT_HEADER, TRUE);
			curl_setopt($ch, CURLINFO_HEADER_OUT, TRUE);
			curl_setopt($ch, CURLOPT_HTTPHEADER,array (
			"Accept: application/json",
			"Cookie: $this->session"
			));
			$result = $this->_handleResponse($ch);
			
			curl_close($ch);
			
			return $result;
		}
		
		function createNode($node)
		{
			$post = http_build_query($node, '', '&');
			$ch = curl_init($this->endpoint . 'node/');
			curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
			curl_setopt($ch, CURLOPT_HEADER, TRUE);
			curl_setopt($ch, CURLOPT_POST, TRUE);
			curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
			curl_setopt($ch, CURLOPT_HTTPHEADER,
			array (
			"Accept: application/json",
			"Content-type: application/x-www-form-urlencoded",
			"Cookie: $this->session",
			'X-CSRF-Token: ' .$this->csrf_token
			));
			
			$result = $this->_handleResponse($ch);
			
			curl_close($ch);
			
			return $result;
		}
		
		// Private Helper Functions
		private function _handleResponse($ch)
		{
			$response = curl_exec($ch);
			$info = curl_getinfo($ch);
			
			//break apart header & body
			$header = substr($response, 0, $info['header_size']);
			$body = substr($response, $info['header_size']);
			
			$result = new stdClass();
			
			if ($info['http_code'] != '200')
			{
				$header_arrray = explode("\n",$header);
				$result->ErrorCode = $info['http_code'];
				$result->ErrorText = $header_arrray['0'];
				} else {
				$result->ErrorCode = NULL;
				$decodedBody= json_decode($body);
				$result = (object) array_merge((array) $result, (array) $decodedBody );
			}
			
			if ($this->debug)
			{
				$result->header = $header;
				$result->body = $body;
			}
			
			return $result;
		}
	}

	
?>