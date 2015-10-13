<?php

/**
* Implements hook_rules_event_info().
*/
function site_expire_rules_event_info() {
  $items = array(
    'site_expire_expiration' => array(
      'label' => t('After site has expired and is disabled'), 
      'group' => t('Site expire'), 
    ),
  ); 

  return $items;
}
