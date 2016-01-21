<?php
/**
 * A class to aid with accessing the Wordstream API
 *
 * Based on the code by John Gadbois - DomainSuperstar.com
 * 
 */
class WordStream {
   private $username;
   private $password;
   private $session;
   public static $apiUrl = "https://api.wordstream.com/";

   public function __construct($username, $password)
   {
      $this->username = $username;
      $this->password = $password;

      $loginResult = $this->login();

      if($loginResult !== false)
      {
         $this->session = $loginResult;
      }
   }

   public function login()
   {
      $response = $this->callAPI('authentication/login', array( 'username' => $this->username, 'password' => $this->password));

      if($response->code != 'OK')
      {
         return false;
      }
      else
      {
         return $response->data->session_id;
      }
   }

   public function logout()
   {
      $response = $this->callAPI('authentication/logout', array('session' => $this->session));
      return $response->code == 'OK';
   }

   public function getKeywords($seeds, $maxResults = 100)
   {
      $response = $this->callAPI('keywordtool/get_keywords', array('seeds' =>$seeds, 'max_results' => $maxResults, 'session_id' => $this->session));

      if($response->code != 'OK')
         return false;
      else
         return $response->data;
   }

   public function getRelatedKeywords($seeds, $maxResults = 100)
   {
      $response = $this->callAPI('keywordtool/get_related_keywords', array('seeds' =>$seeds, 'max_results' => $maxResults, 'session_id' => $this->session));

      if($response->code != 'OK')
         return false;
      else
      {
         return $response->data;
      }
   }

   public function getQuestionKeywords($seeds, $maxResults = 100)
   {
      $response = $this->callAPI('keywordtool/get_question_keywords', array('seeds' =>$seeds, 'max_results' => $maxResults, 'session_id' => $this->session));

      if($response->code != 'OK')
         return false;
      else
         return $response->data;
   }

   public function getKeywordNiches($seeds, $maxResults = 100)
   {
      $response = $this->callAPI('keywordtool/get_keyword_niches', array('seeds' =>$seeds, 'max_results' => $maxResults, 'session_id' => $this->session));

      if($response->code != 'OK')
         return false;
      else
         return $response->data;
   }

   public function getKeywordVolumes($seeds, $maxResults = 100)
   {
      $response = $this->callAPI('keywordtool/get_keyword_volumes', array('seeds' =>$seeds, 'max_results' => $maxResults, 'session_id' => $this->session));

      if($response->code != 'OK')
         return false;
      else
         return $response->data;
   }


   public function getAPICredits()
   {
      $response = $this->callAPI('/authentication/get_api_credits', array('session_id' => $this->session));

      if($response->code != 'OK')
         return false;
      else
         return $response->data;
   }

   private function callAPI($apiMethod, $params = null)
   {
      $paramStr = '';

      if($params)
      {
         foreach($params as $key=>$value)
         {
            $paramStr .= '&' . $key . '=' . urlencode($value);
         }
      }

      //initialize a new curl resource
      $queryStr = "?1=1" . $paramStr;
      $ch = curl_init();

      curl_setopt($ch, CURLOPT_URL, self::$apiUrl . $apiMethod . $queryStr);
      curl_setopt($ch, CURLOPT_HEADER, 0);
      curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      $content = curl_exec($ch);
      curl_close($ch);

      if($content === FALSE) {
         return false;     //Content couldn't be retrieved... Do something
      } else {
         return json_decode($content);   //Content was retrieved do something with it.
      }
   }
}

?>