<?php

/**
 * @file
 *  Definition of SocialGroupPluginJSON class.
 */

/**
 * Base class for a social group.
 */
abstract class SocialGroupPluginJSON extends SocialGroupPlugin {
  
  /**
   * Retreive data from a social group via JSON. 
   */
  protected function retreiveData() {
    $url = $this->getUrl();
    if ($url) {
      $curl = curl_init();
      curl_setopt($curl, CURLOPT_URL, $url);
      curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
      $data = curl_exec($curl);
      curl_close($curl);
      $data = json_decode($data);

      return $data;
    }  
  }
  
}