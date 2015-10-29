<?php

/**
 * @file
 * Provides failover feature.
 * Works with Cloud, Cluster and Scripting module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


/* NagiosJSON Class to read and use Nagios2JSON CGI Format.*/

/**
 * Updated by yas   2011/02/02
 */

class NagiosJSON {

  var $type;
  var $ressource;
  var $user;
  var $password;
  var $http_options;

  function NagiosJSON($type, $ressource) {
    $this->type = $type;
    $this->ressource = $ressource;
  }

  function setAuthentification($user, $password) {
    $this->http_options[CURLOPT_USERPWD] = $user . ':' . $password;

  }

  function getJSON() {
    switch ($this->type) {
      case 'url':
        return $this->getJSONfromURL();
        break;
      case 'file':
        return $this->getJSONfromFile();
        break;
      default:
        return NULL;
        break;
    }
  }

  function getJSONfromURL() {
    $curl = curl_init();
    $this->http_options[CURLOPT_URL] = $this->ressource;
    $this->http_options[CURLOPT_RETURNTRANSFER] = 1;
    $this->http_options[CURLOPT_HTTPAUTH] = CURLAUTH_BASIC | CURLAUTH_DIGEST | CURLAUTH_GSSNEGOTIATE | CURLAUTH_NTLM;
    curl_setopt_array( $curl, $this->http_options );
    $str = curl_exec($curl);
    $str = utf8_encode($str);

    //var_dump($str);
    return json_decode($str, 1);
  }

  function getJSONfromFile() {
    if ($file = fopen($this->ressource, "r")) {
      $str = '';
      while (!feof($file)) {
        $str .= fread($file, 1024);
      }
      $str = utf8_encode($str);

      return json_decode($str, 1);

    }
    else {
      return "Error on File";
    }
  }

  function getCompleteJson() {
    return $this->getJSON();
  }

  function getHostlist() {
    $table = $this->getJSON();
    foreach ($table["hosts"] as $hostname => $host) {
      $hosts[] = $hostname;
    }
    return $hosts;
  }

  function getServiceByHost() {
    $table = $this->getJSON();
    foreach ($table["services"] as $hostname => $service) {
      foreach ($service as $servicename   => $v) {
        $services[$hostname][] = $servicename;
      }
    }
    return $services;
  }

  function getHostgroups_Hosts() {
    $table = $this->getJSON();
    return $table["hostgroups"];

  }


  function getHostgroups() {
    $table = $this->getJSON();
    foreach ($table["hostgroups"] as $key) {
      $res[] = $key;
    }
    return $key;

  }

  function getObjectsForNagVis($type, $filter = '') {

    switch ($type) {
      case 'host':
        $tab = $this->getHostlist();
        if (isset($filter['host'])) {
          $key = array_search($filter['host'], $tab);
          if (is_int($key)) {
            $res[] = array(
              'name1' => $tab[$key],
              'name2' => '',
            );
          }
          else {
            return array('f');
          }
        }
        else {
          foreach ($tab as $key => $host) {
            $res[] = array(
              'name1' => $host,
              'name2' => '',
            );
          }
        }

        break;
      case 'service':
        $tab = $this->getServiceByHost();
        if (isset($filter['host'])) {
          if (isset($tab[$filter['host']])) {
            if (isset($filter['service'])) {
              $key = array_search($filter['service'], $tab[$filter['host']]);
              if (is_int($key)) {
                $res[] = array(
                  'name1' => $filter['host'],
                  'name2' => $tab[$filter['host']][$key],
                );
              }
            }
            else {
              foreach ($tab[$filter['host']] as $key => $v) {
                $res[] = array(
                  'name1' => $filter['host'],
                  'name2' => $v,
                );
              }
            }
          }
          else {
            $res[] = array();
          }
        }
        else {
          foreach ($tab as $hostname => $services) {
            foreach ($services as $key ) {
              $res[] = array(
                'name1' => $hostname,
                'name2' => $key,
              );
            }
          }
        }


        break;
      case 'hostgroup':
        $tab = $this->getHostgroups();
        foreach ($tab as $key) {
          $res[] = array(
            'name1' => $key,
            'name2' => NULL,
          );
        }
        break;

      default:
        $res = array();
        break;
    }
    return $res;
  }

  function parseJSON($str) {

    return json_decode($str);
  }
}
