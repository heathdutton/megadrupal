<?php

use \StructuredDynamics\osf\php\api\framework\ServerIDQuery;
use \StructuredDynamics\osf\php\api\ws\dataset\create\DatasetCreateQuery;
use \StructuredDynamics\osf\php\api\ws\auth\registrar\access\AuthRegistrarAccessQuery;
use \StructuredDynamics\osf\php\api\framework\CRUDPermission;


/**
 * Form builder; Form for editing a OSF for Drupal endpoint.
 * Used by CTools export ui
 *
 * @ingroup forms
 */
function osf_configure_dataset_form(&$form, &$form_state) {
  // Get URI and Info from query string when present
  if($form_state['op'] == 'add' && isset($_GET['uri'])) {
    $uri = check_plain(urldecode($_GET['uri']));
    
    // This is a cached call specific to the user
    $info = osf_configure_api_fetch_dataset_info($uri);    
    $form['info']['label']['#default_value'] = $info['prefLabel'];
  }

  $preset = $form_state['item'];
            
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'osf_configure') . '/osf_configure.css',
  );
  
  $form['uri'] = array(
    '#type' => 'textfield',
    '#title' => t('URI'),
    '#description' => t('The full URI of the dataset'),
    '#default_value' => (isset($_GET['uri']) ? urldecode($_GET['uri']) : $preset->uri),
    '#required' => true,
    '#disabled' => (empty($preset->uri) && !isset($_GET['uri']) ? FALSE : TRUE),
  );
   
  $endpointOptions = array();
  
  foreach(osf_configure_get_endpoints(NULL, FALSE, TRUE) as $endpoint)
  {
    $endpointOptions[$endpoint->sceid] = $endpoint->label;
  }
  
  $defaultEndpointSceid = '';
  
  if(isset($_GET['uri']))
  {
    $defaultEndpointSceid = current(osf_configure_get_default_endpoint())->sceid;
  } 
  elseif(!empty($preset->sceid))
  {
    $defaultEndpointSceid = $preset->sceid;
  }

  $form['sceid'] = array(
    '#type' => 'select',
    '#title' => t('Endpoint'),
    '#description' => t('Endpoint where the dataset should be created'),
    '#options' => $endpointOptions,
    '#default_value' => $defaultEndpointSceid,
    '#disabled' => (empty($preset->sceid) && !isset($_GET['uri']) ? FALSE : TRUE),
  ); 
  
  if(empty($preset->uri) && !isset($_GET['uri']))
  {
    global $user;
    
    $rolesOptions = array();
    
    foreach($user->roles as $role_id => $role_name)
    {
      $groupURI = osf_get_group_uri($role_id, $role_name);
      
      $rolesOptions[$groupURI] = $role_name;
    }  
    
    $form['role'] = array(
      '#type' => 'select',
      '#title' => t('Role access'),
      '#description' => t('The role that will have full CRUD permissions on the new dataset you are about to create'),
      '#options' => $rolesOptions,
      '#default_value' => $preset->role,
    ); 
  }
  
  $form['active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Dataset is searchable'),
    '#description' => t('The dataset will be exposed to the OSF SearchAPI module by checking this option. If the dataset is not marked as <em>searchable</em>, it will be be of type <em>normal</em> which mean that it will be available to other modules.'),
    '#default_value' => $preset->active,
  );    

  
  if(isset($_GET['uri']))
  {
    $form['origin'] = array(
      '#type' => 'hidden',
      '#value' => 'shared',
    );    
    
    $form['new_dataset'] = array(
      '#type' => 'hidden',
      '#value' => FALSE,
    );
  }
  elseif(empty($preset->uri))
  {  
    $form['origin'] = array(
      '#type' => 'hidden',
      '#value' => 'local',
    );    
    
    $form['new_dataset'] = array(
      '#type' => 'hidden',
      '#value' => TRUE,
    );
  }
  else
  {
    $form['new_dataset'] = array(
      '#type' => 'hidden',
      '#value' => FALSE,
    );
  } 

  return $form;
}

function osf_configure_dataset_form_validate(&$form, &$form_state) {
  
  // Create the dataset in the validation form.
  // If we cannot create the dataset, or the permissions, we throw an error and this form won't validate (be created)
  if($form['new_dataset']['#value'])
  {
    global $base_url;
    global $user;
    
    $endpoint = current(osf_configure_get_endpoints($form['sceid']['#value']));
    
    $webservices = osf_configure_osf_get_registered_webservices($endpoint);      
    
    $datasetCreate = new DatasetCreateQuery($endpoint->uri, $endpoint->app_id, $endpoint->api_key, osf_configure_get_current_user_uri());
    
    $datasetCreate->title($form['info']['label']['#value'])
                  ->description('')
                  ->uri($form['uri']['#value'])
                  ->creator($base_url . "/user/" . $user->uid)
                  ->targetWebservices($webservices)
                  ->send(new DrupalQuerierExtension());
    
    if(!$datasetCreate->isSuccessful())      
    {
      if($datasetCreate->error->id == 'WS-DATASET-CREATE-202')
      {
        form_set_error('uri', t('The dataset is already existing. Please change the new dataset\'s URI'));
      }
      else
      {
        form_set_error('uri', t('An error occured when we tried to create the new dataset: '.$datasetCreate->error->id.' ('.$datasetCreate->error->name.')'));
      }
      
      return;
    } 
    else
    {
      // Create FULL CRUD permissions for the user that created it.
      $crudPermissions = new CRUDPermission(TRUE, TRUE, TRUE, TRUE);

      $authRegistrarAccess = new AuthRegistrarAccessQuery($endpoint->uri, $endpoint->app_id, $endpoint->api_key, osf_configure_get_current_user_uri());
      
      $authRegistrarAccess->create($form['role']['#value'], $form['uri']['#value'], $crudPermissions, $webservices)
                          ->mime('text/xml')
                          ->send(new DrupalQuerierExtension());
                           
      if(!$authRegistrarAccess->isSuccessful())
      {
        form_set_error('uri', t('An error occured when we tried to create the permissions for the new dataset: '.$authRegistrarAccess->error->id.' ('.$datasetCreate->error->name.')'));
        
        return;
      } 
      
      if($form['role']['#value'] != osf_configure_get_administrator_group_uri())
      {
        $authRegistrarAccess = new AuthRegistrarAccessQuery($endpoint->uri, $endpoint->app_id, $endpoint->api_key, osf_configure_get_first_user_uri());
        
        $authRegistrarAccess->create(osf_configure_get_administrator_group_uri(), $form['uri']['#value'], $crudPermissions, $webservices)
                            ->mime('text/xml')
                            ->send(new DrupalQuerierExtension());
                             
        if(!$authRegistrarAccess->isSuccessful())
        {
          drupal_set_message(t("Couldn't create permissions on the created dataset for the administrator group."), 'warning', TRUE);        
        }            
      }      
    }
  }
}

function osf_configure_dataset_form_submit(&$form, &$form_state) {
  // Get the role of this dataset
  if($form_state['values']['new_dataset'])
  {
    preg_match('/role\/(.*)\//', $form_state['input']['role'], $matches);
    
    $role = $matches[1];
    
    // get the scdid of the dataset
    $tableStatus = db_query('SHOW TABLE STATUS LIKE \'osf_configure_datasets\'')->fetchObject();
    $scdid = $tableStatus->Auto_increment;

    // Register these permissions in drupal's permissions system
    // Note: here we cannot use the user_role_grant_permissions() API call
    //       because the new dataset is not yet created in drupal.
    db_merge('role_permission')->key(array(
      'rid' => $role,
      'permission' => 'create dataset '.$scdid.' '.$form_state['input']['sceid'],
    ))->fields(array(
      'module' => 'osf_permissions',
    ))->execute();  
    
    db_merge('role_permission')->key(array(
      'rid' => $role,
      'permission' => 'read dataset '.$scdid.' '.$form_state['input']['sceid'],
    ))->fields(array(
      'module' => 'osf_permissions',
    ))->execute();  
    
    db_merge('role_permission')->key(array(
      'rid' => $role,
      'permission' => 'update dataset '.$scdid.' '.$form_state['input']['sceid'],
    ))->fields(array(
      'module' => 'osf_permissions',
    ))->execute();  
    
    db_merge('role_permission')->key(array(
      'rid' => $role,
      'permission' => 'delete dataset '.$scdid.' '.$form_state['input']['sceid'],
    ))->fields(array(
      'module' => 'osf_permissions',
    ))->execute();  
    
    if($role != 3)
    {
      db_merge('role_permission')->key(array(
        'rid' => 3,
        'permission' => 'create dataset '.$scdid.' '.$form_state['input']['sceid'],
      ))->fields(array(
        'module' => 'osf_permissions',
      ))->execute();  
      
      db_merge('role_permission')->key(array(
        'rid' => 3,
        'permission' => 'read dataset '.$scdid.' '.$form_state['input']['sceid'],
      ))->fields(array(
        'module' => 'osf_permissions',
      ))->execute();  
      
      db_merge('role_permission')->key(array(
        'rid' => 3,
        'permission' => 'update dataset '.$scdid.' '.$form_state['input']['sceid'],
      ))->fields(array(
        'module' => 'osf_permissions',
      ))->execute();  
      
      db_merge('role_permission')->key(array(
        'rid' => 3,
        'permission' => 'delete dataset '.$scdid.' '.$form_state['input']['sceid'],
      ))->fields(array(
        'module' => 'osf_permissions',
      ))->execute();  
    }
    
    drupal_static_reset('user_access');
    drupal_static_reset('user_role_permissions');  
  }
}

/**
 * Form builder; Form for editing a OSF for Drupal endpoint.
 * Used by CTools export ui
 *
 * @ingroup forms
 */
function osf_configure_endpoint_form(&$form, &$form_state) {
  $preset = $form_state['item'];

  $form['uri'] = array(
    '#type' => 'textfield',
    '#title' => t('URI'),
    '#description' => t('The full URI of the endpoint'),
    '#default_value' => $preset->uri,
    '#required' => true,
  );

  $form['access'] = array(
    '#type' => 'fieldset',
    '#title' => t('OSF Endpoint Access Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,    
  );  
  
  $form['access']['app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#description' => t('Application ID to be used by Drupal to communicate with the OSF instance.'),
    '#default_value' => $preset->app_id,
    '#required' => true,
  );  
  
  $form['access']['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API KEY'),
    '#description' => t('API key associated with the Application ID.'),
    '#default_value' => $preset->api_key,
    '#required' => true,
  );  
  
  // @TODO Use testfield when color picker module isn't present
  $form['color'] = array(
    '#type' => 'jquery_colorpicker',
    '#title' => t('Color'),
    '#default_value' => $preset->color,
    '#description' => t('Color coding for managing this endpoint.'),
    '#required' => true,
  );

  $form['is_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Default'),
    '#description' => t('If set, this endpoint will be used as the default.'),
    '#default_value' => $preset->is_default,
  );


  return $form;
}

function osf_configure_endpoint_form_validate(&$form, &$form_state) {
  // Get the SID of the endpoint and save it into OSF Configure
  // This also act as a way to make sure that the endpoint is up, running and operational  
  $serverIDQuery = new ServerIDQuery($form_state['values']['uri']);
  
  $serverIDQuery->send(new DrupalQuerierExtension());
  
  if($serverIDQuery->isSuccessful())
  {
    $sid = $serverIDQuery->getResultset();
    if(empty($sid))
    {
      form_set_error('sid', t('The server ID (SID) returned by the server is empty which means that the endpoint is not properly configured. Make sure the endpoint is properly configured and try again.'));
      return;
    }
    else
    {
      $form_state['item']->{'sid'} = $sid;
    }    
  }
  else
  {
    form_set_error('uri', t('It is not possible to get the server ID (SID) for that endpoint. Make sure the endpoint is properly functioning and try again to register it.'));
    return;
  }

  // Be a bit less strict with this one
  if (!valid_url($form_state['values']['uri'])) {
    form_set_error('uri', t('Improperly formatted URI.'));
  }
}

function osf_configure_endpoint_form_submit(&$form, &$form_state) {
  
  $form['uri']['#value'] = trim($form['uri']['#value'], '/').'/';
  $form_state['values']['uri'] = trim($form_state['values']['uri'], '/').'/';
  
  if($form_state['input']['is_default']) {
    foreach (osf_configure_get_endpoints() as $endpoint) {
      if ($endpoint->is_default === "1" && $endpoint->sceid !== OSF_CONFIGURE_ENDPOINT_DEFAULT_SCEID) {
        $endpoint->label = str_replace('Active Default:', '', $endpoint->label);
        $endpoint->is_default = "0";
        osf_configure_endpoint_save($endpoint);
      }
    }
  }

  $endpoint = new stdClass;
  $endpoint->uri = $form['uri']['#value'];
  $endpoint->app_id = $form['access']['app_id']['#value'];
  $endpoint->api_key = $form['access']['api_key']['#value'];
  
  // Synchronize roles as groups, and users assignation in OSF
  osf_configure_sync_groups($endpoint);
  
  // Clear the Ctools export API cache.
  ctools_include('export');
  ctools_export_load_object_reset('osf_configure_endpoints');
  ctools_export_load_object_reset('osf_configure_datasets');
  
  // Clear all cache
  drupal_flush_all_caches();
}