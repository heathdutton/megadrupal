<?php

function visitorinfo_views_data()  {
  // Basic table information.


  $data = array();
  $data['visitorinfo']['table']['group']  = t('Visitorinfo');
  $data['visitorinfo']['table']['base'] = array(
   'field' => 'csid',
   'title' => t('Visitor Info'),
   'help' => t('Stores site visitor information.'),
 );



  $data['visitorinfo']['csid'] = array(
   'title' => t('CSID'),
   'help' => t("Record ID"), // The help that appears on the UI,
   'field' => array(
     'handler' => 'views_handler_field',
   ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
  );
  
  $data['visitorinfo']['ip'] = array(
   'title' => t('IP Address'),
   'help' => t("IP Address"), // The help that appears on the UI,
   'field' => array(
     'handler' => 'views_handler_field',
   ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
  );  
  
  $data['visitorinfo']['ccode'] = array(
   'title' => t('Country Code'),
   'help' => t("Country Code"), // The help that appears on the UI,
   'field' => array(
     'handler' => 'views_handler_field',
   ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
  );
  
  $data['visitorinfo']['cname'] = array(
   'title' => t('Country Name'),
   'help' => t("Country Name"), // The help that appears on the UI,
   'field' => array(
     'handler' => 'views_handler_field',
   ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
  );  
  
  
  $data['visitorinfo']['zip'] = array(
   'title' => t('Zip Code'),
   'help' => t("Zip Code"), // The help that appears on the UI,
   'field' => array(
     'handler' => 'views_handler_field',
   ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
  );  
    
  $data['visitorinfo']['state'] = array(
   'title' => t('State'),
   'help' => t("The state"), // The help that appears on the UI,
   'field' => array(
     'handler' => 'views_handler_field',
   ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
  'sort' => array(
    'handler' => 'views_handler_sort',
  ),
);

  
  $data['visitorinfo']['city'] = array(
   'title' => t('City'),
   'help' => t("The city"), // The help that appears on the UI,
   'field' => array(
     'handler' => 'views_handler_field',
   ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
 );
  
  
  $data['visitorinfo']['latitude'] = array(
   'title' => t('Latitude'),
   'help' => t("Latitude"), // The help that appears on the UI,
   'field' => array(
     'handler' => 'views_handler_field',
   ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
  );  

  $data['visitorinfo']['longitude'] = array(
   'title' => t('Longitude'),
   'help' => t("Longitude"), // The help that appears on the UI,
   'field' => array(
     'handler' => 'views_handler_field',
   ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
  );
  
 return $data;
}

function visitorinfo_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'visitorinfo'),
    ),
    'handlers' => array(
      // filter handlers
      'visitorinfo_handler_filter_field_exists' => array(
        'parent' => 'views_handler_filter',
      ),
    ),
  );
}
