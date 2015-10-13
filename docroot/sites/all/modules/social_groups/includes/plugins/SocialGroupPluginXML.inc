<?php

/**
 * @file
 *  Definition of SocialGroupPluginXML class.
 */

/**
 * Base class for a social group.
 */
abstract class SocialGroupPluginXML extends SocialGroupPlugin {
  
  /**
   * Retreive data from a social group via XML. 
   */
  protected function retreiveData() {
    $url = $this->getUrl();
    if ($url) {
      $curl = curl_init();
      curl_setopt($curl, CURLOPT_URL, $url);
      curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
      $data = curl_exec($curl);
      curl_close($curl);
      
      $xml = new SimpleXMLElement($data);
      $children = $xml->children('http://gdata.youtube.com/schemas/2007');
			$stats = $children->statistics->attributes();
      
      return (string)$stats['subscriberCount'];
    }   
  }
  
}