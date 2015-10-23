<?php


function wsrp_admin_settings()
{
  $form['consumer-options'] = array(
    '#type' => 'fieldset',
    '#title' => t('WSRP Consumer'),
    '#description' => t('<strong>NOTE:</strong> These settings are used to register this site with WSRP producers.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['consumer-options']['wsrp_consumer_name'] = array(
   '#type' => 'textfield',
   '#title' => t('Consumer name'),
   '#description' => t("A name (preferably unique) that identifies the Consumer (this site). An example of such a name would be the site URL (".  url(NULL, array('absolute' => TRUE)) .")"),
   '#default_value' => variable_get('wsrp_consumer_name',''),
   '#required' => TRUE
  );
  
  $form['consumer-options']['httpsPort'] = array(
   '#type' => 'textfield',
   '#title' => t('HTTPS port'),
   '#description' => t("The port used for https comunication (default: 443)"),
   '#default_value' => variable_get('wsrp_consumer_httpsPort','443'),
   '#required' => TRUE
  );
  
  $form['consumer-options']['debug'] = array(
   '#type' => 'checkbox',
   '#title' => t('Debug?'),
   '#description' => t("output SOAP comunications with syslog (in the http server error log)"),
   '#default_value' => variable_get('wsrp_consumer_debug','0')
  );
  
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type'  => 'submit',
    '#value' => t('Reset to defaults'),
  );
  return $form;
}

function wsrp_admin_settings_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (((int)$values['httpsPort'])==0) {
      form_set_error('name', t('Invalid HTTPS port.'));
  }
  
}

function wsrp_admin_settings_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case "Save configuration":
      $values = $form_state['values'];
      
      variable_set('wsrp_consumer_name', $values['wsrp_consumer_name']);
      variable_set('wsrp_consumer_httpsPort', $values['httpsPort']);
      
      syslog(LOG_ERR,"debug: ".print_r($values,true));
      
      if (isset($values['debug'])&&$values['debug']=='1')
        variable_set('wsrp_consumer_debug', '1');
      else
        variable_set('wsrp_consumer_debug', '0');
      
      drupal_set_message(t('The configuration options have been saved.'));
      
      break;
    case "Reset to defaults":
      variable_del('wsrp_consumer_name');

      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }

  // Rebuild the menu router.
//  menu_rebuild();
}

/**
 * Implements the wsrp servers list.
 *
 * @return
 *   The HTML table with the servers list.
 */
function wsrp_admin_list() {
  $rows = array();
  $result = db_query("SELECT producer_id, name, status FROM {wsrp_producer} ORDER BY name");
  while ($row = $result->fetchObject()) {
    $rows[] = array(
      $row->name,
      l(t('edit'), 'admin/config/services/wsrp/edit/'. $row->producer_id),
      l($row->status ? t('de-activate') : t('activate'), 'admin/config/services/wsrp/'. ($row->status ? 'deactivate' : 'activate') .'/'. $row->producer_id),
      l(t('delete'), 'admin/config/services/wsrp/delete/'. $row->producer_id)
    );
  }

  $header = array(
    t('Producer'),
    array('data' => t('Operations'), 'colspan' => 3),
  );

  return theme('table', array( 'header' => $header, 'rows' => $rows));
}

/**
 * Implements the wsrp server edit page.
 *
 * @param $form_state
 *   A form state array.
 * @param $op
 *   An operatin - add or edit.
 * @param $sid
 *   A wsrp producer ID.
 *
 * @return
 *   The form structure.
 */
function wsrp_admin_form(&$form_state, $args = NULL, $op = NULL, $producer_id = NULL) {
  if ($op == "edit" && $producer_id) {
    $edit = db_query("SELECT * FROM {wsrp_producer} WHERE producer_id = :producer_id", array( 'producer_id' => $producer_id ))->fetchAssoc();
    $form['producer_id'] = array(
      '#type' => 'hidden',
      '#value' => $producer_id,
    );
  }
  else {
    $edit = array(
      'name' => '',
      'url' => '',
      'version' => ''
    );
  }

  $form['producer-settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Producer settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['producer-settings']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Producer name'),
    '#default_value' => $edit['name'],
    '#description' => t('Choose a <em><strong>unique</strong></em> name for this producer.'),
    '#size' => 50,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['producer-settings']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Producer WSDL url'),
    '#default_value' => $edit['url'],
    '#size' => 70,
    '#maxlength' => 255,
    '#description' => t('The WSDL url of this producer.'),
    '#required' => TRUE,
  );

  $version_options = array(
    '1' => '1.0',
    '2' => '2.0'
  );
  
  $form['producer-settings']['version'] = array(
    '#type' => 'select',
    '#title' => t('Producer WSRP version'),
    '#default_value' => $edit['version'],
    '#options' => $version_options,
    '#description' => t('The WSRP protocol version of this producer.'),
    '#required' => TRUE
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validate hook for the wsrp server form.
 */
function wsrp_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (!isset($values['producer_id'])) {
    if (db_query("SELECT name FROM {wsrp_producer} WHERE name = :name", array( 'name' => $values['name']))->fetchObject()) {
      form_set_error('name', t('A WSRP producer with the name "%name" already exists.', array('%name' => $values['name'])));
    }
  }
  
  try
  {
    $client= new SoapClient($values['url'],array("trace"=>1));
    $serviceDescription= (array)$client->getServiceDescription();
  }
  catch (Exception $e)
  {
  	form_set_error('url', t('"%url" seems not a valid WSDL url for a WSRP producer: '. $e->getMessage(), array('%url' => $values['url'])));
  	return;
  }
  
  
  if (!array_key_exists("requiresRegistration", $serviceDescription))
      form_set_error('url', t('"%url" seems not a valid WSDL url for a WSRP producer.', array('%url' => $values['url'])));
  
}

/**
 * Submit hook for the wsrp server form.
 */
function wsrp_admin_form_submit($form, &$form_state)
{
  $values = $form_state['values'];
  
  try
  {
  
	  $consumer= wsrp_get_consumer($values['url'],$values['version']);
	  $registrationContext= $consumer->register();
  	  $serviceDescription= $consumer->getServiceDescription($registrationContext);

	  if (!isset($values['producer_id']))
	  {
	  	db_query("INSERT INTO {wsrp_producer} (name, url, version, registration, init_cookie) VALUES (:name,:url,:version,:registration,:init_cookie)", 
                         array('name'         => $values['name'], 
                               'url'          => $values['url'], 
                               'version'      => $values['version'], 
                               'registration' => serialize($registrationContext), 
                               'init_cookie'  => (isset($serviceDescription->requiresInitCookie ) ? $serviceDescription->requiresInitCookie : null)));

                drupal_set_message(t('WSRP producer "%name" has been added.', array('%name' => $values['name'])));
	        watchdog('wsrp', 'WSRP producer "%name" has been added.', array('%name' => $values['name']));
	  }
	  else 
	  {
	     if ($row = db_query("SELECT url, version, registration FROM {wsrp_producer} WHERE producer_id = :producer_id", array( 'producer_id' => $values['producer_id']))->fetchObject()) 
	     {
	     	if ($row->registration)
	     	{
			  	$oldconsumer= wsrp_get_consumer($row->url,$row->version);
			  	$oldconsumer->deregister(unserialize($row->registration));
	     	}
	     }
	  	  	   
	     db_query("UPDATE {wsrp_producer} SET name = :name, url = :url, version= :version, registration= :registration, init_cookie= :init_cookie WHERE producer_id = :producer_id", 
                     array( 'name'          => $values['name'], 
                            'url'           => $values['url'], 
                            'version'       => $values['version'], 
                            'registration'  => serialize($registrationContext), 
                            'init_cookie'   => (isset($serviceDescription->requiresInitCookie ) ? $serviceDescription->requiresInitCookie : null),
                            'producer_id'   => $values['producer_id']));

	     drupal_set_message(t('WSRP producer "%name" has been updated.', array('%name' => $values['name'])));
	     watchdog('wsrp', 'WSRP producer "%name" has been updated.', array('%name' => $values['name']));
	  }

  	  
  	  $producer_id= db_query("select producer_id from {wsrp_producer} where name= :name",array( 'name' => $values['name']))->fetchColumn();
  	  
  	  if (!is_array($serviceDescription->offeredPortlets)) $serviceDescription->offeredPortlets= array($serviceDescription->offeredPortlets);
  	  
	  foreach ($serviceDescription->offeredPortlets as $portlet)
	  {
		if ($portlet_id= db_query("select portlet_id from {wsrp_portlet} where handle= :handle and producer_id= :producer_id",array( 'handle' => $portlet->portletHandle, 'producer_id' => $producer_id))->fetchColumn())
		{
		   db_query("UPDATE {wsrp_portlet} SET description = :description WHERE portlet_id = :portlet_id", 
			    array( 'description' => serialize($portlet), 'portlet_id' => $portlet_id ));
		}
		else
		{
		   db_query("INSERT INTO {wsrp_portlet} (producer_id,handle,description) values (:producer_id,:handle,:description)", 
			    array( 'producer_id' => $producer_id, 
				   'handle'      => $portlet->portletHandle, 
				   'description' => serialize($portlet)));
		}
	  }
	  
  }
  catch (Exception $e)
  {
  	 form_set_error('url', t('"%url" seems not a valid WSDL url for a WSRP producer: '. $e->getMessage(), array('%url' => $values['url'])));
  }

  $form_state['redirect'] = 'admin/config/services/wsrp/list';
}

/**
 * De-activates the wsrp server.
 *
 * @param $sid
 *   A wsrp server ID.
 *
 * @return
 */
function wsrp_admin_deactivate($producer_id) {
  syslog(LOG_ERR,'wsrp_admin_deactivate: '.$producer_id);
  $result = db_query("SELECT name, url, version, registration from {wsrp_producer} WHERE producer_id = :producer_id", array( 'producer_id' => $producer_id ));
  
  if ($row = $result->fetchObject()) 
  {
  	
    db_query("UPDATE {wsrp_producer} SET status = 0 WHERE producer_id = :producer_id", array( 'producer_id' => $producer_id ));
    drupal_set_message(t('WSRP producer "%name" has been de-activated.', array('%name' => $row->name)));
    watchdog('wsrp', 'WSRP producer "%name" was de-activated.', array('%name' => $row->name));
    
  }
  
  drupal_goto('admin/config/services/wsrp/list');
}

/**
 * Activates the WSRP producer.
 *
 * @param $producer_id
 *   A WSRP producer ID.
 *
 * @return
 */
function wsrp_admin_activate($producer_id) {
  $result = db_query("SELECT name, url, version, registration from {wsrp_producer} WHERE producer_id = :producer_id", array( 'producer_id' => $producer_id ));
  
  if ($row = $result->fetchObject())
  try 
  {
  	$consumer= wsrp_get_consumer($row->url,$row->version);
    $registrationContext= unserialize($row->registration);
    $serviceDescription= $consumer->getServiceDescription($registrationContext);
	
	$handles= array();
	
	if (!is_array($serviceDescription->offeredPortlets)) $serviceDescription->offeredPortlets= array($serviceDescription->offeredPortlets);
		
	foreach ($serviceDescription->offeredPortlets as $portlet)
	{
		//syslog(LOG_ERR,print_r($portlet->portletHandle,true));
		
		$handles[]= "'". str_replace("'","''",$portlet->portletHandle) ."'";
		
        if ($portlet_id=db_query("select portlet_id from {wsrp_portlet} where handle= :handle and producer_id= :producer_id",array( 'handle' => $portlet->portletHandle, 'producer_id' => $producer_id))->fetchColumn())
        {
           db_query("UPDATE {wsrp_portlet} SET description = :description WHERE portlet_id = :portlet_id", array( 'description' => serialize($portlet), 'portlet_id' => $portlet_id));
        }
        else
        {
           db_query("INSERT INTO {wsrp_portlet} (producer_id,handle,description) values (:producer_id,:handle,:description)", array( 'producer_id' =>  $producer_id, 'handle' => $portlet->portletHandle, 'description' => serialize($portlet)));
        }
	}
	
	//syslog(LOG_ERR,print_r($handles,true));
		
  	db_query("DELETE FROM {wsrp_portlet} WHERE handle not in (". implode(',',$handles) .") and producer_id = :producer_id", array( 'producer_id' => $producer_id ));
	db_query("UPDATE {wsrp_producer} SET status = '1', registration = :registration WHERE producer_id = :producer_id", array( 'registration' => serialize($registrationContext), 'producer_id' => $producer_id));
    drupal_set_message(t('WSRP producer "%name" has been activated.', array('%name' => $row->name)));
    watchdog('wsrp', 'WSRP producer "%name" was activated.', array('%name' => $row->name));
  }
  catch (Exception $e)
  {
  	form_set_error('',t('Cannot activate WSRP Producer: %message', array('%message' => $e->getMessage())));
  }
  
  drupal_goto('admin/config/services/wsrp/list');
}

/**
 * Implements the WSRP producer delete page.
 *
 * @param $form_state
 *   A form state array.
 * @param $producer_id
 *   A WSRP producer ID.
 *
 * @return
 *   The form structure.
 */
function wsrp_admin_delete(&$form_state, $args, $producer_id) {
  if ($row = db_query("SELECT name FROM {wsrp_producer} WHERE producer_id = :producer_id", array( 'producer_id' => $producer_id ))->fetchObject()) {
    $form['producer_id'] = array(
      '#type' => 'hidden',
      '#value' => $producer_id,
    );
    $form['name'] = array(
      '#type' => 'hidden',
      '#value' => $row->name,
    );
    return confirm_form(
      $form,
      t('Are you sure you want to delete the WSRP producer named <em><strong>%name</strong></em>?', array('%name' => $row->name)),
      'admin/config/services/wsrp/list',
      t('<p>This action cannot be undone.</p>'),
      t('Delete'),
      t('Cancel')
    );
  }
  drupal_goto('admin/config/services/wsrp/list');
}

/**
 * Submit hook for the WSRP producer delete page.
 */
function wsrp_admin_delete_submit($form, &$form_state) {
  $values = $form_state['values'];
  
  if ($values['confirm'] && $values['producer_id'])
  {
  	
     if ($row = db_query("SELECT url, version, registration FROM {wsrp_producer} WHERE producer_id = :producer_id", array( 'producer_id' => $values['producer_id']))->fetchObject()) 
     {
     	if ($row->registration)
     	{
                 try
                 {
		  	$consumer= wsrp_get_consumer($row->url,$row->version);
		  	$consumer->deregister(unserialize($row->registration));
                 }
                 catch (Exception $ex)
                 {
                     drupal_set_message(t('Cannot deregister WSRP Producer "%name" on the remote site.', array('%name' => $values['name'])));
                 }
     	}
     }
  	
    db_query("DELETE FROM {wsrp_portlet_instance} WHERE portlet_id in (select portlet_id FROM {wsrp_portlet} WHERE producer_id = :producer_id)", array ( 'producer_id' => $values['producer_id'] ));
    db_query("DELETE FROM {wsrp_portlet} WHERE producer_id = :producer_id", array ( 'producer_id' => $values['producer_id'] ));
    db_query("DELETE FROM {wsrp_producer} WHERE producer_id = :producer_id", array ( 'producer_id' => $values['producer_id'] ));
    drupal_set_message(t('WSRP Producer "%name" has been deleted.', array('%name' => $values['name'])));
    watchdog('wsrp', 'WSRP Producer "%name" has been deleted.', array('%name' => $values['name']));
    
  }
  
}


?>
