<?php

use \StructuredDynamics\osf\php\api\ws\auth\lister\AuthListerQuery;
use \StructuredDynamics\osf\framework\Namespaces;
use \StructuredDynamics\osf\framework\Resultset;
use \StructuredDynamics\osf\framework\Subject;
use \StructuredDynamics\osf\php\api\ws\ontology\update\OntologyUpdateQuery;
use \StructuredDynamics\osf\php\api\ws\search\SearchQuery;
use \StructuredDynamics\osf\php\api\ws\crud\read\CrudReadQuery;
use \StructuredDynamics\osf\php\api\ws\search\ExtendedFiltersBuilder;
use \StructuredDynamics\osf\php\api\ws\ontology\update\CreateOrUpdateEntityFunction;
use \StructuredDynamics\osf\php\api\ws\revision\lister\RevisionListerQuery;
use \StructuredDynamics\osf\php\api\ws\revision\update\RevisionUpdateQuery;
use \StructuredDynamics\osf\php\api\ws\revision\delete\RevisionDeleteQuery;
use \StructuredDynamics\osf\php\api\ws\revision\diff\RevisionDiffQuery;



/**
 * Generate the resource_type edit for; this form edit properties of the resource_type
 * bundles.
 */
function resource_type_form($form, &$form_state, $resource_type, $op = 'edit') 
{
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $resource_type->label,
    '#description' => t('The human-readable name of this resource.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($resource_type->type) ? $resource_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $resource_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'resource_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this resource type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($resource_type->description) ? $resource_type->description : '',
    '#description' => t('Description about the resource type.'),
  );

  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save resource type'),
    '#weight' => 40,
  );
  
  // Add the field for ignoring this resource type
  $form['ignoreResource'] = array(
    '#title' => t('Ignore synchronized resource type'),
    '#description' => t('Use this option to ignore a synchronized class. Ingored classes are not used anywhere in Drupal'),
    '#default_value' => db_query('SELECT ignoreResource FROM {resource_type} WHERE type = :type LIMIT 1', array(':type' => $form['type']['#default_value']))->fetchField(),
    '#type' => 'checkbox'
  );    
  
  return $form;
}

/**
* Manage the "Add new resource type" menu action link
*/
function osf_entities_resource_add_new()
{
  return drupal_get_form('osf_entities_resource_add_new_form');
}

/**
* Manage the "Add new resource type field" menu action link
*/
function osf_entities_resource_fields_add_new()
{
  return drupal_get_form('osf_entities_resource_fields_add_new_form');
}

function osf_entities_resource_fields_add_new_form()
{
  $form = array();
   
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => (isset($_GET['uri']) ? get_label_from_uri($_GET['uri']) : ''),
    '#description' => t('The human-readable name of this resource type field. This text will be 
                         displayed as part of the list on the Add new resource type field page. It is 
                         recommended that this name begin with a lower-case letter and contain 
                         only letters, numbers, and spaces. This name must be unique.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE          
  );
  
  $form['fieldtype'] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#title' => t('Field value'),
    '#default_value' => 'datatype',
    '#options' => array(
                    'datatype' => t('Literal'),
                    'object' => t('Reference')
                  ),
    '#description' => t('Specify the value to be expected for this field. If the expected value is a literal, 
                         it means that all its values will be interpreted as literal values. If the value if 
                         a reference, it means that the value will be considered a unique identifier that 
                         refer to another entity in the system.'),
    '#attributes' => array(
      'onchange' => "if(this.value == 'object') {jQuery('.form-item-range').fadeIn('slow');} else {jQuery('.form-item-range').fadeOut('slow');}"
    ),
  );    
  
  $form['range'] = array(
    '#type' => 'textfield',
    '#title' => t('Reference type'),
    '#default_value' => '',
    '#description' => t('Specify the type of entities that we can refer with this field. If this field is empty, 
                         then this field will be able to reference any type of entities.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#autocomplete_path' => 'osf/entities/get/classes/include_used',
    '#states' => array(
      'invisible' => array(
        '#edit-fieldtype-datatype' => array('checked' => TRUE),
      ),
    )
  );  
  
  // If object, specify domain/ranges   
  $accessibleOntologies = osf_get_accessible_datasets(TRUE, TRUE);
  $loadedOntologies = osf_get_loaded_ontologies();
    
  foreach($accessibleOntologies as $key => $value)
  {
    if(isset($loadedOntologies[$key]))
    {
      $accessibleOntologies[$key] = $loadedOntologies[$key]['prefLabel'];
    }
  }

  $form['ontology'] = array(
    '#type' => 'select',
    '#title' => t('Ontology'),
    '#options' => $accessibleOntologies,
    '#default_value' => $accessibleOntologies[key($accessibleOntologies)],
    '#description' => t('Specify in which ontology you want to save that new field.'),
    '#required' => TRUE,
  );     
  
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#value' => '',
    '#description' => t('Describe this resource type field. The text will be displayed on the Add new 
                         resource field page.'),   
  );     

  $form['submit'] = array(  
    '#type' => 'submit', 
    '#value' => t('Save resource type'),
  );                                
  
  $form['#submit'][] = 'osf_entities_resource_fields_add_new_submit';
  $form['#validate'][] = 'osf_entities_resource_fields_add_new_validate';
  
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'osf_entities') . '/js/autocomplete.js',
  );  
  
  return($form);
}

function osf_entities_resource_fields_add_new_validate($form, &$form_state)
{ 
  $loadedOntologies = osf_get_loaded_ontologies();
  
  if(!isset($loadedOntologies[$form_state['input']['ontology']]))
  {
    form_set_error('ontology', t('Selected ontology not loaded in the system: '.$form_state['input']['ontology']));   
    return; 
  }
  
  $baseURI = $loadedOntologies[$form_state['input']['ontology']]['http://www.w3.org/2000/01/rdf-schema#isDefinedBy'][0]['uri'];
  
  $uri = $baseURI.preg_replace("/[^a-zA-Z0-9]/", "_", str_replace(' ', '', $form_state['input']['name']));
  
  $fieldID = resource_type_get_id($uri);
  
  $entities = entity_load('resource_type', array($fieldID));

  // Make sure that the name is valid and unique
  if(!empty($entities))
  {
    form_set_error('name', t('The name of the resource type field you are trying to create already exists. Please choose another unique name.'));   
    return;         
  }
  
  if(isset($_GET['uri']))
  {
    $form_state['new_property_uri'] = $_GET['uri'];
  }
  else
  {
    $form_state['new_property_uri'] = $uri;    
  }
}

function osf_entities_resource_fields_add_new_submit($form, &$form_state)
{  
  // Create new class in OSF  
  $defaultEndpoint = current(osf_configure_get_default_endpoint());
  
  $ontologyUpdate = new OntologyUpdateQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
  
  $createOrUpdateEntityFunction = new CreateOrUpdateEntityFunction();
  
  // Add the class represented by the bundle in the range.
  
  $propertyN3 = '';
  
  // Define RDF type
  if($form['fieldtype']['#value'] == 'datatype')
  {
    $propertyN3 = '<'.$form_state['new_property_uri'].'> a <'.Namespaces::$owl.'DatatypeProperty> ;';
  }
  else
  {
    $propertyN3 = '<'.$form_state['new_property_uri'].'> a <'.Namespaces::$owl.'ObjectProperty> ;';
    
    if($form['range']['#value'] == '')
    {
      $propertyN3 .= '    rdfs:range <'.Namespaces::$owl.'Thing> ;';  
    }
    else
    {
      $bundle = substr($form['range']['#value'], strpos($form['range']['#value'], '(') + 1, strpos($form['range']['#value'], ')') - (strpos($form['range']['#value'], '(') + 1));
      
      $namespaces = new Namespaces();
      $propertyN3 .= '    rdfs:range <'.$namespaces->getUnprefixedUri($bundle).'> ;';  
    }
  }
  
  // Define property domain
  preg_match('/.*manage\/(.*)\/fields.*/', drupal_get_path_alias(), $matches);
  
  $bundle = $matches[1];
  
  $classesByBundles = variable_get('osf_entities_classes_by_bundles', array());
  
  if(!isset($classesByBundles[$bundle]))
  {
    drupal_set_message(t("The class related to the bundle where you want to create that new field is not 
                          existing or unsynchronized."), 
                       "error", 
                       TRUE);      
    return;
  }
  
  $propertyN3 .= '    rdfs:domain <'.$classesByBundles[$bundle].'> ;';

  $propertyN3 .= '       <'.Namespaces::$iron.'prefLabel> """'.$form_state['input']['name'].'""" ;
                         <'.Namespaces::$iron.'description> """'.$form_state['input']['description'].'""" .';

  $createOrUpdateEntityFunction->enableAdvancedIndexation()
                               ->document($propertyN3);
  
  $ontologyUpdate->createOrUpdateEntity($createOrUpdateEntityFunction)
                 ->enableReasoner()
                 ->ontology($form_state['input']['ontology'])
                 ->send(new DrupalQuerierExtension());
                               
  if(!$ontologyUpdate->isSuccessful())  
  {
    drupal_set_message(t("Can't get the list of  registered web service endpoints: [@error] @errorMsg - @errorDescription", 
                       array("@error" => $ontologyUpdate->getStatus(),
                             "@errorMsg" => $ontologyUpdate->getStatusMessage(),
                             "@errorDescription" => $ontologyUpdate->getStatusMessageDescription())), 
                       "error", 
                       TRUE);  
                       
    watchdog('osf_entities', 
             'Can\'t get the list of  registered web service endpoints: [@error] @errorMsg - @errorDescription.', 
             array("@error" => $ontologyUpdate->getStatus(),
                   "@errorMsg" => $ontologyUpdate->getStatusMessage(),
                   "@errorDescription" => $ontologyUpdate->getStatusMessageDescription()));                       
    return;  
  }  
    
  // Create new field & register it in different registries variables
  _osf_entities_create_field_from_property($form_state['new_property_uri']);
  
  // Create new field instance for that bundle
  _osf_entities_create_field_instance_by_property_class($form_state['new_property_uri'], $classesByBundles[$bundle]);

  // Since this is a new field/property, we make sure that it appears in the "unused" section of the
  // bundle in the synchronization form.  
  $unusedProperties = variable_get('osf_entities_unused_properties_to_synchronize', array());    
  
  $namespaces = new Namespaces();
  
  for($i = 0; $i < 256; $i++)
  {
    if(!isset($unusedProperties[$bundle.'_'.$i]))  
    {
      $unusedProperties[$bundle.'_'.$i] = array(t(get_label_from_uri($form_state['new_property_uri'])).'   ('.$namespaces->getPrefixedUri($form_state['new_property_uri']).')');  
      break;
    }
  }

  variable_set('osf_entities_unused_properties_to_synchronize', $unusedProperties);   
  
  // Make sure this property get added to the list of unused properties for this class when synchronizing
  $unusedPropertiesToSynchronize = variable_get('osf_entities_unused_properties_to_synchronize', array());
  
  $newUnusedProperty = get_label_from_uri($form_state['new_property_uri']).'   ('.$namespaces->getPrefixedUri($form_state['new_property_uri']).')';
  
  if(!in_array($newUnusedProperty, $unusedPropertiesToSynchronize))
  {
    // Find the last unused properties field for that bundle
    $id = -1;
    foreach($unusedPropertiesToSynchronize as $numberedBundle => $value)
    {
      if(strpos($numberedBundle, $bundle.'_') !== FALSE)
      {
        $newId = (int)str_replace($bundle.'_', '', $numberedBundle);
        
        if($newId > $id)
        {
          $id = $newId;
          
          if($value[0] == '')
          {
            $id--;
            break;
          }
        }
      }
    }
    
    $id++;    
    
    $unusedPropertiesToSynchronize[$bundle.'_'.$id] = array($newUnusedProperty);
    
    variable_set('osf_entities_unused_properties_to_synchronize', $unusedPropertiesToSynchronize);
  }        
  
  drupal_flush_all_caches();
  
  // Redirect to the resource types management page
  drupal_goto('admin/structure/resource-types/manage/'.$bundle.'/fields');
}

function osf_entities_resource_add_new_form()
{
  $form = array();
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => (isset($_GET['uri']) ? get_label_from_uri($_GET['uri']) : ''),
    '#description' => t('The human-readable name of this resource type. This text will be 
                         displayed as part of the list on the Add new resource type page. It is 
                         recommended that this name begin with a capital letter and contain 
                         only letters, numbers, and spaces. This name must be unique.'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE          
  );  
   
  $accessibleOntologies = osf_get_accessible_datasets(TRUE, TRUE);
  $loadedOntologies = osf_get_loaded_ontologies();
    
  foreach($accessibleOntologies as $key => $value)
  {
    if(isset($loadedOntologies[$key]))
    {
      $accessibleOntologies[$key] = $loadedOntologies[$key]['prefLabel'];
    }
  }

  $form['ontology'] = array(
    '#type' => 'select',
    '#title' => t('Ontology'),
    '#options' => $accessibleOntologies,
    '#default_value' => $accessibleOntologies[key($accessibleOntologies)],
    '#description' => t('Specify in which ontology you want to save that new resource type.'),
    '#required' => TRUE          
  );  
  
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#value' => '',
    '#description' => t('Describe this resource type. The text will be displayed on the Add new 
                         resource page.'),   
  );     

  $form['submit'] = array(  
    '#type' => 'submit', 
    '#value' => t('Save resource type'),
  );  
  
  /*
  $entities = entity_load('resource_type', array('owl_class'));
  
  $output = drupal_get_form('osf_entities_resource_type_form', $entities[key($entities)]);

  foreach($output as $property => $field)
  {
    if(is_array($field))
    {
      if($field['#type'] == 'container')
      {
        $form[$property] = $field;
      }
    }
  }
  */
              
  $form['#submit'][] = 'osf_entities_resource_add_new_submit';
  $form['#validate'][] = 'osf_entities_resource_add_new_validate';
  
  return($form);
}

function osf_entities_resource_add_new_validate($form, &$form_state)
{
  $loadedOntologies = osf_get_loaded_ontologies();
  
  if(!isset($loadedOntologies[$form_state['input']['ontology']]))
  {
    form_set_error('ontology', t('Selected ontology not loaded in the system: '.$form_state['input']['ontology']));   
    return; 
  }
  
  $baseURI = $loadedOntologies[$form_state['input']['ontology']]['http://www.w3.org/2000/01/rdf-schema#isDefinedBy'][0]['uri'];
  
  $uri = $baseURI.preg_replace("/[^a-zA-Z0-9]/", "_", str_replace(' ', '', $form_state['input']['name']));
  
  $bundle = resource_type_get_id($uri);
  
  $entities = entity_load('resource_type', array($bundle));
  
  // Make sure that the name is valid and unique
  if(!empty($entities))
  {
    form_set_error('name', t('The name of the resource type you are trying to create already exists. Please choose another unique name.'));   
    return;         
  }
  
  if(isset($_GET['uri']))
  {
    $form_state['new_class_uri'] = $_GET['uri'];
  }
  else
  {
    $form_state['new_class_uri'] = $uri;    
  }
}

function osf_entities_resource_add_new_submit($form, &$form_state)
{
  // First thing to do: make sure that the Class bundle is created.
  osf_entities_sync_core_structure();
    
  // Create new class in OSF  
  $defaultEndpoint = current(osf_configure_get_default_endpoint());
  
  $ontologyUpdate = new OntologyUpdateQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
  
  $createOrUpdateEntityFunction = new CreateOrUpdateEntityFunction();
  
  $classN3 = '<'.$form_state['new_class_uri'].'> a <'.Namespaces::$owl.'Class> ;
                                                 <'.Namespaces::$iron.'prefLabel> """'.$form_state['input']['name'].'""" ;
                                                 <'.Namespaces::$iron.'description> """'.$form_state['input']['description'].'""" .';

  $createOrUpdateEntityFunction->enableAdvancedIndexation()
                               ->document($classN3);
  
  $ontologyUpdate->createOrUpdateEntity($createOrUpdateEntityFunction)
                 ->enableReasoner()
                 ->ontology($form_state['input']['ontology'])
                 ->send(new DrupalQuerierExtension());
                               
  if(!$ontologyUpdate->isSuccessful())  
  {
    drupal_set_message(t("Can't get the list of  registered web service endpoints: [@error] @errorMsg - @errorDescription", 
                       array("@error" => $ontologyUpdate->getStatus(),
                             "@errorMsg" => $ontologyUpdate->getStatusMessage(),
                             "@errorDescription" => $ontologyUpdate->getStatusMessageDescription())), 
                       "error", 
                       TRUE);  
                       
    watchdog('osf_entities', 
             'Can\'t get the list of  registered web service endpoints: [@error] @errorMsg - @errorDescription.', 
             array("@error" => $ontologyUpdate->getStatus(),
                   "@errorMsg" => $ontologyUpdate->getStatusMessage(),
                   "@errorDescription" => $ontologyUpdate->getStatusMessageDescription()));                       
                       
    return;  
  }  
    
  // Create new bundle & register it in different registries variables
  _osf_entities_create_bundle_from_class($form_state['new_class_uri']);
  
  // Make sure this class get added to the list of unused classes when synchronizing
  $unusedClassesToSynchronize = variable_get('osf_entities_unused_classes_to_synchronize', array());
  
  $namespaces = new Namespaces();
  $newUnusedClass = get_label_from_uri($form_state['new_class_uri']).'   ('.$namespaces->getPrefixedUri($form_state['new_class_uri']).')';
  
  if(!in_array($newUnusedClass, $unusedClassesToSynchronize))
  {
    $unusedClassesToSynchronize[] = $newUnusedClass;
    variable_set('osf_entities_unused_classes_to_synchronize', $unusedClassesToSynchronize);
  }  
  
  drupal_flush_all_caches();
  
  // Redirect to the resource types management page
  drupal_goto('admin/structure/resource-types');
}

/**
* Manage the "Synchronize existing resource type" menu action link
*/
function osf_entities_resource_synchronize_existing()
{
  drupal_goto('osf/entities/ontologies/sync/classes');
}

/**
* Manage the "Synchronize existing fields" menu action link
*/
function osf_entities_resource_fields_synchronize_existing()
{
  drupal_goto('osf/entities/ontologies/sync/properties');
}

/**
 * Submit handler for creating/editing resource_type bundles
 */
function resource_type_form_submit(&$form, &$form_state) {
  $resource_type = entity_ui_form_submit_build_entity($form, $form_state);
  
  // Save and go back.
  resource_type_save($resource_type);

  // Redirect user back to list of resource types.
  $form_state['redirect'] = 'admin/structure/resource-types';
}
              
/**
* Handle to confirm entity record deletation in the OSF instance
*/
function osf_entities_form_delete_confirm($form, &$form_state, $entity) {

  $form['resource'] = array('#type' => 'hidden', '#value' => serialize($entity));
  
  return confirm_form($form,
    t('Are you sure you want to delete resource type %title?', array('%title' => ($entity[key($entity)]->label != "" ? $entity[key($entity)]->label : $entity[key($entity)]->iron_preflabel[0]))),
    'resource/' . rawurldecode($entity[key($entity)]->drid),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Resource type delete form submit handler.
 */
function osf_entities_form_delete_confirm_submit($form, &$form_state) {
  
  $entity = unserialize($form['resource']['#value']);
  $entity = $entity[key($entity)];
  
  resource_type_delete($entity->uri);

  watchdog('resource_type', '@type: deleted %title.', array('@type' => $entity->type[0], '%title' => ($entity->label != "" ? $entity->label : $entity->iron_preflabel[0])));
  drupal_set_message(t('%title has been deleted.', array('%title' => ($entity->label != "" ? $entity->label : $entity->iron_preflabel[0]))));

  $form_state['redirect'] = 'admin/resource';
}


/**
 * Add new resource type page callback.
 */
function osf_entities_resource_add($type) {
  $resource_type = resource_types($type);

  $resource = entity_create('resource_type', array('type' => $type));
  drupal_set_title(t('Create @name', array('@name' => entity_label('resource_type', $resource_type))));

  $output = drupal_get_form('osf_entities_resource_type_form', $resource);

  return $output;
}

/**
* Handle the "delete" menu action click for an entity instance
*/
function osf_entities_resource_delete($entity) {
  return(drupal_get_form('osf_entities_form_delete_confirm', $entity));
}

/**
* Handle the "Revisions" menu item click for an entity instance
*/
function osf_entities_resource_revisions($entity)
{               
  $entity = $entity[key($entity)];

  drupal_set_title(t('Revisions for %title', array('%title' => $entity->label)), PASS_THROUGH);


  $header = array(t('Revision'), 
                  t('Compare'),
                  array('data' => t('Operations'), 'colspan' => 2));           
                  
  // Get all revisions
  $defaultEndpoint = osf_configure_get_endpoint_by_uri(osf_entities_get_network_from_dataset_uri($entity->dataset));
  
  $revisionLister = new RevisionListerQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());

  $revisionLister->longResults()
                 ->uri($entity->uri)
                 ->dataset($entity->dataset)
                 ->send(new DrupalQuerierExtension());
                 
  if($revisionLister->isSuccessful())
  {
    // each of them that are not published get deleted
    $revisions = $revisionLister->getResultset()->getResultset();
    
    $revisions = $revisions['unspecified'];
  } 
  
  $rows = array();
  
  $revert_permission = FALSE;  
  if(user_access('resource entity revert revisions'))
  {
    $revert_permission = TRUE;
  }
  
  $delete_permission = FALSE;
  if(user_access('resource entity delete revisions')) 
  {
    $delete_permission = TRUE;
  }
  
  // Check if we are comparing revisions
  $compareRev = '';
  if(stripos(drupal_get_path_alias(), '/compare/') !== FALSE)
  {
    preg_match('/\/compare\/(.*)\/?/', drupal_get_path_alias(), $matches);
    $compareRev = $matches[1];    
  }
  
  foreach($revisions as $revuri => $revision) 
  {                                                                                   
    $row = array();
    $operations = array();
    
    $status = $revision[Namespaces::$wsf.'status'][0]['uri'];
    $revisionTime = $revision[Namespaces::$wsf.'revisionTime'][0]['value'];
    $revisionTimeInt = substr($revisionTime, 0, strpos($revisionTime, '.'));
    $performerUserID = $revision[Namespaces::$wsf.'performer'][0]['uri'];
    
    if($status == Namespaces::$wsf.'published') 
    {
      // @TODO Specify the user that created the revision. This will require some new improvements of CRUD: Update
      $row[] = array(
        'data' => t('!date by !username', array('!date' => l(format_date($revisionTimeInt, 'short'), 'resources/'.rawurldecode($entity->drid)), '!username' => theme('username', array('account' => user_load(1))))),
        'class' => array('revision-current'),
      );
      
      if($compareRev == '')
      {
        $row[] = array(
          'data' => l(t('compare'), 'resources/'.rawurldecode($entity->drid).'/revisions/compare/'.$revisionTime),
          'class' => array('revision-current')
        );
      }
      else
      {
        if($revisionTime == $compareRev)
        {
          $row[] = array(
            'data' => t('comparing ').l(t('(stop)'), 'resources/'.rawurldecode($entity->drid).'/revisions/'),
            'class' => array('revision-current')
          );
        }
        else
        {
          $row[] = array(
            'data' => l(t('with'), 'resources/'.rawurldecode($entity->drid).'/revisions/compare/'.$compareRev.'/with/'.$revisionTime),
            'class' => array('revision-current')
          );          
        }
      }
      
      $operations[] = array(
        'data' => drupal_placeholder(t('current revision')),
        'class' => array('revision-current'),
        'colspan' => 2
      );
      
      /*
      
      // The unpublish operations link needs to be better integrated in a Drupal moderation flow.
      // The issue is that when we unpublish it, it remains as revisions, but since we use entity_load()
      // it can't find it in the dataset (only in the revisions dataset). This means that we would have
      // to change _load() to check in the revisions graph if CrudRead() fails to try to get a unpublished
      // version of an entity. Then, we would have to make sure that only the proper people to have access
      // to the unpublished versions of that entity.      
      
      $operations[] = array(
        'data' => l(t('unpublish'), 'resources/'.rawurldecode($entity->drid).'/revisions/'.$revisionTime.'/unpublish'),
        'class' => array('revision-current'),
      );
      */
    }
    else 
    {
      // @TODO Specify the user that created the revision. This will require some new improvements of CRUD: Update
      $row[] = t('!date by !username', array('!date' => l(format_date($revisionTimeInt, 'short'), 'resources/'.rawurldecode($entity->drid).'/revisions/'.$revisionTime.'/view'), '!username' => theme('username', array('account' => user_load(1)))));

      if($compareRev == '')
      {      
        $row[] = l(t('compare'), 'resources/'.rawurldecode($entity->drid).'/revisions/compare/'.$revisionTime);
      }
      else
      {
        if($revisionTime == $compareRev)
        {
          $row[] = t('comparing ').l(t('(stop)'), 'resources/'.rawurldecode($entity->drid).'/revisions/');
        }        
        else
        {
          $row[] = l(t('with'), 'resources/'.rawurldecode($entity->drid).'/revisions/compare/'.$compareRev.'/with/'.$revisionTime);
        }
      }
      
      if($revert_permission)       
      {
        $operations[] = l(t('revert'), 'resources/'.rawurldecode($entity->drid).'/revisions/'.$revisionTime.'/revert');
      }
      
      if($delete_permission) 
      {
        $operations[] = l(t('delete'), 'resources/'.rawurldecode($entity->drid).'/revisions/'.$revisionTime.'/delete');
      }
    }
    $rows[] = array_merge($row, $operations);
  }

  $build['resource_revisions_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );

  return $build;  
}

/**
* Handle the list of translations available for a resource
*/
function osf_entities_resource_translate($entity)
{               
  $entity = $entity[key($entity)];

  drupal_set_title(t('Translations of %title', array('%title' => $entity->label)), PASS_THROUGH);


  $header = array(t('Language'), 
                  t('Title'),
                  t('Operations'));           
  
  $rows = array();
  
  
  $languages = language_list('enabled');
  foreach($languages[1] as $lang => $language)
  {
    $row = array();
    
    if($lang == get_default_language())
    {
      $row[] = '<strong>'.$language->name.'</strong> (source)';  
    }
    else
    {
      $row[] = $language->name;  
    }

    if(isset($entity->{resource_type_get_id(Namespaces::$iron."prefLabel")}[$lang]))
    {
      $row[] = $entity->{resource_type_get_id(Namespaces::$iron."prefLabel")}[$lang][0]['value'];
    }
    else
    {
      $row[] = '<em>'.t('untranslated').'</em>';
    }
    
    $row[] = '<a href="'.($lang != get_default_language() ? '/'.$lang.'/' : '/').'resources/'.rawurldecode($entity->drid).'/edit">'.t('edit').'</a>';
    
    $rows[] = $row;
  }
  
  $build['resource_translations_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );

  return $build;  
}
  
/**
* Handle the "Edit" menu item click for an entity instance
*/
function osf_entities_resource_edit($entity)
{
  $entity = $entity[key($entity)];  
  
  // Clear that entity from the cache. That way, if it changed in the OSF instance
  // we will see the changes in the edit form.
  cache_clear_all($entity->uri, 'cache_entity_resource_type');
  
  $entity = current(entity_load('resource_type', array($entity->uri)));

  // Make sure there is only one type for form selection...
  // Make sure the type is existing as a bundle
  $bundleExists = FALSE;
   
  if(is_array($entity->type))
  {
    foreach($entity->type as $type)
    {
      if(db_query('SELECT id FROM {resource_type} WHERE type = :type LIMIT 1', 
               array(':type' => $type))->fetchField() !== FALSE)
      {
        $entity->type = $type;
        $bundleExists = TRUE;
        break;
      }
    }
  }
  
  if(is_array($entity->type))
  {
    $entity->type = $entity->type[0];
    
    if(db_query('SELECT id FROM {resource_type} WHERE type = :type LIMIT 1', 
             array(':type' => $entity->type))->fetchField() !== FALSE)
    {
      $bundleExists = TRUE;
    }    
  }
  
  if(!$bundleExists)
  {
    drupal_set_message(t('No bundle existing for the resource you are trying to edit: %bundle. <a href="/osf/entities/ontologies/sync/classes?full_destination='.drupal_get_path_alias().'">Go synchronize this bundle</a>', array('%bundle' => $entity->type)), 'warning');
    return('');
  }  

  $output = drupal_get_form('osf_entities_resource_type_form', $entity);

  return $output;   
}

/**
* Handle the "view" menu item click for an entity instance
*/
function osf_entities_resource_page_view($entity)
{
  $content = array( '#theme' => 'resource_type',
                    '#element'   => $entity[key($entity)],
                    '#view_mode' => 'full',
                    '#language'  => NULL);                   
                    
  return($content);  
}

function osf_entities_resource_page_redirect_expose($entity)
{
  $entity = current($entity);
  
  drupal_goto('osf/entities/ontologies/sync/classes/expose/'.urlencode($entity->rdf_type[0]));
}

function osf_entities_resource_page_redirect_expose_menu_access($entity)
{  
  if(user_access('Edit resource'))
  {
    $entity = current($entity);
    
    $classesByBundles = variable_get('osf_entities_classes_by_bundles', array());
    
    if(array_search($classesByBundles[$entity->type[0]], $entity->rdf_type) === FALSE)
    {
      return(TRUE);
    }    
    else
    {
      return(FALSE);
    }
  }
}

/**
* Handle the "export" menu item click for an entity instance
*/
function osf_entities_resource_page_export($entity)
{
  $form['formats'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export the <em>'.current($entity)->label.'</em> resource entity'),
    '#description' => t('Export the structured data of that resource in different formats'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,    
  );  
  
  $links = array();
  
  $links[] = '<a href="/'.str_replace('/export', '', drupal_get_path_alias()).'/rdf_xml">RDF+XML</a>';
  $links[] = '<a href="/'.str_replace('/export', '', drupal_get_path_alias()).'/rdf_n3">RDF+N3</a>';
  $links[] = '<a href="/'.str_replace('/export', '', drupal_get_path_alias()).'/xml">structXML</a>';
  $links[] = '<a href="/'.str_replace('/export', '', drupal_get_path_alias()).'/json">structJSON</a>';
  $links[] = '<a href="/'.str_replace('/export', '', drupal_get_path_alias()).'/iron_json">irON JSON</a>';
  $links[] = '<a href="/'.str_replace('/export', '', drupal_get_path_alias()).'/iron_common">irON commON (CSV)</a>';  
  
  $form['formats']['list'] = array(
    '#theme' => 'item_list',
    '#items' => $links,
  );  
  
  return($form);  
}

/**
* Handle the "export" menu item click for an entity instance
*/
function osf_entities_resource_page_export_format($entity, $format)
{
  $exportResultset = new Resultset(current($entity)->dataset);
  
  $subject = current($entity)->obj;

  $exportResultset->addSubject($subject, current($entity)->dataset);
  
  switch($format)
  {
    case 'rdf_xml':
      drupal_add_http_header('Content-Type','application/rdf+xml');
      
      die($exportResultset->getResultsetRDFXML());
    break;
    case 'rdf_n3':
      drupal_add_http_header('Content-Type','application/rdf+n3');
      
      die($exportResultset->getResultsetRDFN3());
    break;
    case 'xml':
      drupal_add_http_header('Content-Type','text/xml');
      
      die($exportResultset->getResultsetXML());    
    break;
    case 'json':
      drupal_add_http_header('Content-Type','application/json');
      
      die($exportResultset->getResultsetJSON());
    break;
    case 'iron_json':
      drupal_add_http_header('Content-Type','application/iron+json');
      
      $entity = current($entity);
      
      // Special usecase. Since we are not sure if we have access to the OSF Web Services
      // classes on that server, we have to use the CRUD: Read endpoint to get the description
      // of that entity for the ironJSON format.
      $endpoint = current(osf_configure_get_endpoint_from_dataset_uri($entity->dataset));
      
      $crudRead = new CrudReadQuery($endpoint->uri, $endpoint->app_id, $endpoint->api_key, osf_configure_get_current_user_uri());

      $crudRead->dataset($entity->dataset)
               ->uri($entity->uri)
               ->lang('')
               ->mime('application/iron+json')
               ->send(new DrupalQuerierExtension());
      
      if($crudRead->isSuccessful())
      {
        die($crudRead->getResultset());
      }
      else
      {
        die('');
      }
    break;
    case 'iron_common':
      drupal_add_http_header('Content-Type','application/iron+csv');
      
      $entity = current($entity);
      
      // Special usecase. Since we are not sure if we have access to the OSF Web Services
      // classes on that server, we have to use the CRUD: Read endpoint to get the description
      // of that entity for the ironJSON format.
      $endpoint = current(osf_configure_get_endpoint_from_dataset_uri($entity->dataset));
      
      $crudRead = new CrudReadQuery($endpoint->uri, $endpoint->app_id, $endpoint->api_key, osf_configure_get_current_user_uri());

      $crudRead->dataset($entity->dataset)
               ->uri($entity->uri)
               ->lang('')
               ->mime('application/iron+csv')
               ->send(new DrupalQuerierExtension());
      
      if($crudRead->isSuccessful())
      {
        die($crudRead->getResultset());
      }
      else
      {
        die('');
      }
    break;
    
    default:
      // unsupported format
    break;
  }
  return('');
}

/**
 * Page to select resource Type to add new resource.
 */
function osf_entities_resource_add_page() {
  
  $form = array();
  $items = array();
  
  $path_alias = trim(str_replace('admin/resource/add', '', drupal_get_path_alias()), '/');
  
  // If there is something at the end of the path, it means that the user entered an URL
  // where the DRID is not existing. This means that we suggest him to create a new record
  // of one of the existing type for that new entity
  if(!empty($path_alias))
  {
    $exposedResourceTypes = osf_entities_get_exposed_resource_type_ids();
    
    foreach (resource_types() as $resource_type_key => $resource_type) {
      if(array_search($resource_type_key, $exposedResourceTypes) !== FALSE)
      {
        $items[] = l(entity_label('resource_type', $resource_type), 'resource/add/' . $resource_type_key . '/'. $path_alias);
      }
    }
  
    $form['add_resources'] = array('#theme' => 'item_list', '#items' => $items, '#title' => t('<em>The requested resource is not existing.</em><br /><br /> Select the type of new resource you would like to create for that page:<br />'));
  }
  else
  {
    $exposedResourceTypes = osf_entities_get_exposed_resource_type_ids();
    
    foreach (resource_types() as $resource_type_key => $resource_type) {
      if(array_search($resource_type_key, $exposedResourceTypes) !== FALSE)
      {
        $items[] = l(entity_label('resource_type', $resource_type), 'resource/add/' . $resource_type_key);
      }
    }
  
    $form['add_resources'] = array('#theme' => 'item_list', '#items' => $items, '#title' => t('Select the type of new resource you would like to create:'));
  }
  
  return($form);
}

/**
 * Implements hook_menu_local_tasks_alter().
 */
function osf_entities_menu_local_tasks_alter(&$data, $router_item, $root_path) {
  // Add action link to 'node/add' on 'admin/resource' page.
  if ($root_path == 'admin/resource') {
    $item = menu_get_item('admin/resource/add');
    if ($item['access']) {
      $data['actions']['output'][] = array(
        '#theme' => 'menu_local_action',
        '#link' => $item,
      );
    }
  }
}

function osf_entities_admin_content($form, $form_state)
{
  $page = 0;
  $orderBy = 'Title';
  $sort = 'asc';
  $itemsPerPage = 50;
  $nbRecords = 0;
  $namespaces = new Namespaces();
  
  if(isset($_GET['page']))
  {
    $page = $_GET['page'];
  }
  
  if(isset($_GET['order']))
  {
    $orderBy = $_GET['order'];
  }
  
  if(isset($_GET['sort']))
  {
    $sort = $_GET['sort'];
  } 
  
  if(!isset($_SESSION['resource_overview_columns']))
  {
    $_SESSION['resource_overview_columns'] = array('title' => 'title', 
                                                   'type' => 'type', 
                                                   'dataset' => 'dataset', 
                                                   'status' => 'status', 
                                                   'operations' => 'operations',
                                                   'uri' => 0);
  }
  
  if(!isset($_SESSION['resource_overview_show_available']))
  {
    $_SESSION['resource_overview_show_available'] = TRUE;
  }
  
  $columns = $_SESSION['resource_overview_columns'];
  $showAvailable = $_SESSION['resource_overview_show_available'];
  
  // Add the types and datasets to the Drupal static cache such that they become available
  // to the filters hooks that creates the list of possible filters to apply to this query
  $filterTypes = &drupal_static('osf_entities_filters_types');
  if(!isset($filterTypes)) 
  {
    if($cache = cache_get('osf_entities_filters_types_'.session_id())) 
    {
      $filterTypes = $cache->data;
    }
    else
    {
      $filterTypes = array();
    }
  }    
  
  $filterDatasets = &drupal_static('osf_entities_filters_datasets');
  if(!isset($filterDatasets)) 
  {
    if($cache = cache_get('osf_entities_filters_datasets_'.session_id())) 
    {
      $filterDatasets = $cache->data;
    }
    else 
    {
      $filterDatasets = array();
    }
  } 
    
  $searchTerms = '';
  if(isset($_SESSION['resource_overview_filter_search_terms'])) 
  {
    $searchTerms = $_SESSION['resource_overview_filter_search_terms'];
  }   
  
  // Build the sortable table header.
  $header = array();
  
  if($columns['title'] == 'title')
  {
    $header['title'] = array('data' => t('Title'),
                             'field' => 'r.title',
                             'sort' => 'asc');
  }
  
  if($columns['uri'] !== 0)
  {
    $header['uri'] = array('data' => t('URI (Unique Identifier))'));
  }
  
  if($columns['type'] !== 0)
  {
    $header['type'] = array('data' => t('Type'), 'field' => 'r.type');
  }
  
  if($columns['dataset'] !== 0)
  {
    $header['dataset'] = array('data' => t('Dataset'), 'field' => 'r.dataset');
  }
  
  if($columns['status'] !== 0)
  {
    $header['status'] = array('data' => t('Status'));
  }
  
  if($columns['operations'] !== 0)
  {
    $header['operations'] = array('data' => t('Operations'));
  }
  
  $form['search'] = array(
    '#type' => 'fieldset',
    '#weight' => '0',
    '#title' => t('Search'),
    '#attributes' => array('class' => array('container-inline')),
    '#access' => 'access osf search',
  );  
  
  $form['search']['searchfield'] = array(
    '#type' => 'textfield',
    '#title' => t('Search for'),
    '#size' => 100,
    '#default_value' => $searchTerms,
  );  
  
  $form['search']['status']['actions']['search'] = array(
    '#type' => 'submit',
    '#value' => 'Search',
  );
  
  if($searchTerms != '')
  {
    $form['search']['status']['actions']['resetsearch'] = array(
      '#type' => 'submit',
      '#value' => 'Remove',
    );
  }
  
  $accessibleOntologies = osf_get_accessible_datasets(TRUE, TRUE);

  if(empty($accessibleOntologies))
  {
    drupal_set_message(t('Ontologies not loaded. <a href="/admin/ontology">Please make sure that ontologies are loaded</a>. If none appears in that list, then make sure the Ontologies web services are operational, and then click the "Reload all" button to reload all of them into Drupal.'), 'error');
    return;
  }
  
  $extendedSearch = new ExtendedFiltersBuilder();

  // Remove the ontologies datasets
  foreach($accessibleOntologies as $uri => $ontology)
  {
    $extendedSearch->not_()
                   ->datasetFilter($uri);
  }
  
  $defaultEndpoint = current(osf_configure_get_default_endpoint());
  
  $search = new SearchQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
  
  $search->items($itemsPerPage)
         ->page($itemsPerPage * $page)
         ->extendedFilters($extendedSearch->getExtendedFilters())
         ->includeAggregates()         
         ->sourceInterface(variable_get("osf_searchapi_settings_interface_name", ''))
         ->mime('resultset');
         
  if($showAvailable === TRUE)
  {
    $search->enableInference();
  }
  else
  {
    $search->disableInference()
           ->includeAttribute('uri');
  }

  // Only include the types that got synchronized
  $filters = osf_entities_admin_content_filters();  
  $rdfMapping = osf_entities_get_rdf_mappings();

  if((empty($_SESSION['resource_overview_filter']) || empty($_SESSION['resource_overview_filter']['type'])) && $showAvailable === FALSE)
  {
    // If no types filters exists, then we constrain the search query to the types
    // that have been synchronized with OSF Entities
    
    $appliedFilters = 0;
    
    foreach($rdfMapping as $machine_name => $map)
    {
      if($machine_name != 'owl_class' && 
         $machine_name != 'owl_annotationproperty' && 
         $machine_name != 'owl_datatypeproperty' && 
         $machine_name != 'owl_objectproperty' && 
         $machine_name != 'owl_thing')
      {
        foreach($map['mapping']['rdftype'] as $rdftype)
        {
          $search->typeFilter($namespaces->getUnprefixedUri($rdftype));
          $appliedFilters++;
        }
      }
    }
    
    if($appliedFilters == 0)
    {
      // There is a special usecase when the "show available" option is *unchecked* and when
      // no bundles have been mapped into Drupal.
      // What need to be done here is add a dummy type filter that will return no results.
      $search->typeFilter('unexisting-type-filter');
    }
  }
  
  // Check if we have to add some ordering criterias         
  if($orderBy != '')
  {
    switch(strtolower($orderBy))
    {
      case 'title':
        $search->sort(Namespaces::$iron.'prefLabel', $sort);
      break;
      case 'type':
        $search->sort('type', $sort);
      break;
      case 'dataset':
        $search->sort('dataset', $sort);
      break;
    }
  }       
  
  // Add possible filters
  if(!empty($_SESSION['resource_overview_filter']))
  {                  
    foreach($_SESSION['resource_overview_filter'] as $filter)
    {      
      $type = $filter[0];
      $value = $filter[1];
      
      switch($type)
      {
        case 'dataset':
           $search->datasetFilter($value);
        break;
        
        case 'type':
           $search->typeFilter($value);
        break;
      }
    }
  }
  
  // Add possible search terms
  if($searchTerms != '')
  {    
    $search->attributeValuesFilters('prefLabel', $searchTerms);
  }
  
  $search->send(new DrupalQuerierExtension());  
  
  $options = array();  
  $destination = drupal_get_destination();

  if($search->isSuccessful())
  {
    $resultset = $search->getResultset()->getResultset();

    if(!empty($resultset))
    {
      // Get the total number of results for that Search query
      foreach($resultset['unspecified'] as $record)
      {
        if($record['type'][0] == Namespaces::$aggr.'Aggregate' &&
           $record[Namespaces::$aggr.'property'][0]['uri'] == Namespaces::$void.'Dataset')
        {        
          $nbRecords += $record[Namespaces::$aggr.'count'][0]['value'];
          
          if(!in_array($record[Namespaces::$aggr.'object'][0]['uri'], $filterDatasets))
          {
            $datasetInfo = osf_configure_api_fetch_dataset_info($record[Namespaces::$aggr.'object'][0]['uri']);
                  
            if($datasetInfo !== FALSE)
            {
              $filterDatasets[$record[Namespaces::$aggr.'object'][0]['uri']] = check_plain($datasetInfo['prefLabel']);
            }
            else
            {
              $filterDatasets[$record[Namespaces::$aggr.'object'][0]['uri']] = $record[Namespaces::$aggr.'object'][0]['uri'];
            }                                    
          }
        }
      }
      
      // Get the list of all the types that match this current search query
      foreach($resultset['unspecified'] as $record)
      {
        if($record['type'][0] == Namespaces::$aggr.'Aggregate' &&
           $record[Namespaces::$aggr.'property'][0]['uri'] == Namespaces::$rdf.'type')
        {        
          if(!in_array($record[Namespaces::$aggr.'object'][0]['uri'], $filterTypes))
          {
            $t = entity_load('resource_type', array($record[Namespaces::$aggr.'object'][0]['uri']));
            
            if(!empty($t) && !empty($t[key($t)]->label))
            {
              $filterTypes[$record[Namespaces::$aggr.'object'][0]['uri']] = check_plain($t[key($t)]->label);                    
            }
            else
            {
              // Keep URI ending only
              $uriLabel = '';
              if(strrpos($record[Namespaces::$aggr.'object'][0]['uri'], '#') !== FALSE)
              {
                $uriLabel = substr($record[Namespaces::$aggr.'object'][0]['uri'], strrpos($record[Namespaces::$aggr.'object'][0]['uri'], '#') + 1);
              }
              else
              {
                $uriLabel = substr($record[Namespaces::$aggr.'object'][0]['uri'], strrpos($record[Namespaces::$aggr.'object'][0]['uri'], '/') + 1);
              }
              
              $uriLabel = preg_replace('/[^a-zA-Z0-1]/i', '', $uriLabel);
              
              $filterTypes[$record[Namespaces::$aggr.'object'][0]['uri']] = check_plain($uriLabel);
            }              
          }
        }
      }
      
      // Add the datasets and types filters to the cache
      asort($filterTypes, SORT_STRING);
      asort($filterDatasets, SORT_STRING);
      
      cache_set('osf_entities_filters_types_'.session_id(), $filterTypes, 'cache', CACHE_TEMPORARY);
      cache_set('osf_entities_filters_datasets_'.session_id(), $filterDatasets, 'cache', CACHE_TEMPORARY);
      
      // Get the list of URIs that will be used to load the ResourceType entities using entity_load()  
      $uris = array();
      
      foreach($resultset as $dataset => $records)
      {
        if($dataset != 'unspecified')
        {
          foreach($records as $uri => $record)
          {
            $uris[] = $uri;
          }
        }
      }

      // Load all the entities from the search query
      $loadedResources = entity_load('resource_type', $uris);    
      $classesByBundles = variable_get("osf_entities_classes_by_bundles", NULL);
      $resources = array('exposed' => array(),
                         'available' => array());
                       
      // Create the array of resources that is composed of the 'exposed' and 'available' resources
      foreach($resultset as $dataset => $records)
      {
        if($dataset == 'unspecified')
        {
          continue;
        }
        
        foreach($records as $uri => $record)
        {
          // If for the record, its type !== rdf_type then it means that its
          // rdf_type is not currently exposed to Drupal as a bundle.
          if(!isset($classesByBundles[$loadedResources[$uri]->type[0]]) || array_search($classesByBundles[$loadedResources[$uri]->type[0]], $loadedResources[$uri]->rdf_type) === FALSE)
          {
            $resources['available'][$uri] = $loadedResources[$uri];
          }
          else
          {
            $resources['exposed'][$uri] = $loadedResources[$uri];
          }
        }
      }
      
      $accessibleDatasets = osf_get_accessible_datasets();
      
      foreach($resources as $status => $res)
      {
        switch($status)
        {
          case 'exposed':
            foreach($resources[$status] as $uri => $resource)
            {
              if(is_object($resource) && get_class($resource) == 'ResourceType')
              {
                $options[$resource->drid] = array();
                
                if($columns['title'] !== 0)
                {
                  $options[$resource->drid]['title'] = array('data' => array(
                                                               '#type' => 'link',
                                                               '#title' => $resource->label,
                                                               '#href' => osf_entities_get_resource_page_url_from_entity_uri($resource->uri)));
                }
                
                if($columns['uri'] !== 0)
                {
                  $options[$resource->drid]['uri'] = check_plain($uri);
                }
                
                if($columns['type'] !== 0)
                {
                  $t = entity_load('resource_type', array($resource->type[0]));
                  
                  if(!empty($t))
                  {
                    $options[$resource->drid]['type'] = check_plain($t[key($t)]->label);                    
                  }
                  else
                  {
                    $options[$resource->drid]['type'] = check_plain($resource->type[0]);
                  }                  
                }
                
                if($columns['dataset'] !== 0)
                {
                  $datasetInfo = osf_configure_api_fetch_dataset_info($resource->dataset);
                  
                  if($datasetInfo !== FALSE)
                  {
                    $options[$resource->drid]['dataset'] = check_plain($datasetInfo['prefLabel']);
                  }
                  else
                  {
                    $options[$resource->drid]['dataset'] = check_plain($resource->dataset);
                  }
                }
                
                if($columns['status'] !== 0)
                {
                  $options[$resource->drid]['status'] = check_plain((isset($classesByBundles[$resource->type[0]]) ? 'exposed' : 'available'));
                }
                
                if($columns['operations'] !== 0)
                {
                  // Build a list of all the accessible operations for the current node.
                  $operations = array();

                  if(in_array($resource->dataset, $accessibleDatasets))
                  {
                    if(user_access('Edit resource')) 
                    {
                      $operations['edit'] = array(
                        'title' => t('edit'),
                        'href' => 'resources/' . rawurldecode($resource->drid) . '/edit',
                        'query' => $destination,
                      );
                    }
                    
                    if(user_access('Delete resource')) {
                      $operations['delete'] = array(
                        'title' => t('delete'),
                        'href' => 'resources/' . rawurldecode($resource->drid) . '/delete',
                        'query' => $destination,
                      );
                    }
                  }
                  else
                  {
                    $operations['configure'] = array(
                      'title' => t('configure dataset for editing'),
                      'href' => 'admin/config/osf/networks_and_dataset/datasets',
                      'query' => $destination,
                    );
                  }
                  
                  $options[$resource->drid]['operations'] = array();
                  
                  if (count($operations) > 1) {
                    // Render an unordered list of operations links.
                    $options[$resource->drid]['operations'] = array(
                      'data' => array(
                        '#theme' => 'links__node_operations',
                        '#links' => $operations,
                        '#attributes' => array('class' => array('links', 'inline')),
                      ),
                    );
                  }
                  elseif (!empty($operations)) {
                    // Render the first and only operation as a link.
                    $link = reset($operations);
                    $options[$resource->drid]['operations'] = array(
                      'data' => array(
                        '#type' => 'link',
                        '#title' => $link['title'],
                        '#href' => $link['href'],
                        '#options' => array('query' => $link['query']),
                      ),
                    );
                  } 
                }  
              }       
            }          
          break;
          
          case 'available':
            foreach($resources[$status] as $uri => $resource)
            {          
                $options[$uri] = array();
                
                if($columns['title'] !== 0)
                {
                  $options[$uri]['title'] = array('data' => array(
                                              '#type' => 'link',
                                              '#title' => $resource->label,
                                              '#href' => osf_entities_get_resource_page_url_from_entity_uri($resource->uri)));
                  
                }
                
                if($columns['uri'] !== 0)
                {
                  $options[$uri]['uri'] = check_plain($uri);
                }
                
                if($columns['type'] !== 0)
                {
                  $t = entity_load('resource_type', array($resource->type[0]));
                  
                  if(!empty($t))
                  {
                    $options[$uri]['type'] = check_plain($t[key($t)]->label);                    
                  }
                  else
                  {
                    // Keep URI ending only
                    $uriLabel = '';
                    if(strrpos($resource->type[0], '#') !== FALSE)
                    {
                      $uriLabel = substr($resource->type[0], strrpos($resource->type[0], '#') + 1);
                    }
                    else
                    {
                      $uriLabel = substr($resource->type[0], strrpos($resource->type[0], '/') + 1);
                    }
                    
                    $options[$uri]['type'] = check_plain($uriLabel);
                  }              
                }
                
                if($columns['dataset'] !== 0)
                {
                  if(isset($resource->dataset))
                  {
                    
                    $datasetInfo = osf_configure_api_fetch_dataset_info($resource->dataset);
                    
                    if($datasetInfo !== FALSE)
                    {
                      $options[$uri]['dataset'] = check_plain($datasetInfo['prefLabel']);
                    }
                    else
                    {
                      $options[$uri]['dataset'] = check_plain($resource->dataset);
                    }                                    
                  }
                  else
                  {
                    $options[$uri]['dataset'] = t('unknown provenance dataset');
                  }
                }
                
                if($columns['status'] !== 0)
                {
                  $options[$uri]['status'] = check_plain((isset($classesByBundles[$resource->type[0]]) ? 'exposed' : 'available'));
                }
                
                if($columns['operations'] !== 0)
                {              
                  // Build a list of all the accessible operations for the current node.
                  $operations = array();

                  if(user_access('Edit resource')) 
                  {
                    $operations['expose'] = array(
                      'title' => t('expose'),
                      'href' => 'osf/entities/ontologies/sync/classes/expose/'.urlencode($resource->rdf_type[0]),
                    );
                  }

                  if(in_array($resource->dataset, $accessibleDatasets))
                  {
                    if(user_access('Edit resource')) 
                    {
                      $operations['edit'] = array(
                        'title' => t('edit'),
                        'href' => 'resources/' . rawurldecode($resource->drid) . '/edit',
                        'query' => $destination,
                      );
                    }
                    
                    if(user_access('Delete resource')) {
                      $operations['delete'] = array(
                        'title' => t('delete'),
                        'href' => 'resources/' . rawurldecode($resource->drid) . '/delete',
                        'query' => $destination,
                      );
                    }
                  }
                  else
                  {
                    $operations['configure'] = array(
                      'title' => t('configure dataset for editing'),
                      'href' => 'admin/config/osf/networks_and_dataset/datasets',
                      'query' => $destination,
                    );
                  }                  
                  
                  $options[$uri]['operations'] = array();
                  
                  // Render an unordered list of operations links.
                  $options[$uri]['operations'] = array(
                    'data' => array(
                      '#theme' => 'links__node_operations',
                      '#links' => $operations,
                      '#attributes' => array('class' => array('links', 'inline')),
                    ),
                  );
                }
            }        
          break;
        }        
      }
    }
  }
  else
  {
    drupal_set_message(
      t('Couldn\'t perform the search query. Error [%status] %msg (%msg_desc).',
        array(
          '%status' => $search->getStatus(),
          '%msg' => $search->getStatusMessage(),
          '%msg_desc' => $search->getStatusMessageDescription(),
        )
      ),
      'error'
    );      
  }

  $form['organize_columns'] = array(
    '#type' => 'fieldset',
    '#weight' => '2',
    '#title' => t('Organize Columns'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('class' => array('container-inline')),
    '#access' => 'access osf search',
  );  
  
  $form['organize_columns']['columns'] = array(
    '#type' => 'checkboxes',
    '#options' => array('title' => t('Title'),
                        'uri' => t('URI (Unique Identifier)'),
                        'type' => t('Type'),
                        'dataset' => t('Dataset'),
                        'status' => t('Status'),
                        'operations' => t('Operations')),
    '#default_value' => array_values($columns),
    '#title' => t('Results columns to expose in the table:'),
  );
  
  $form['organize_columns']['status']['actions']['show_available'] = array(
    '#type' => 'submit',
    '#value' => t('Reorganize'),
  );    

  $form['availability'] = array(
    '#type' => 'fieldset',
    '#weight' => '2',
    '#title' => t('Availability'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('class' => array('container-inline')),
    '#access' => 'access osf search',
  );  
  
  $form['availability']['available'] = array(
    '#type' => 'checkboxes',
    '#options' => array('available' => ''),
    '#default_value' => array(($showAvailable === TRUE ? 'available' : '')),
    '#title' => t('Show available records that are not currently exposed in the portal:'),
  );
  
  $form['availability']['status']['actions']['show_available'] = array(
    '#type' => 'submit',
    '#value' => t('Show available'),
  );  
  
  $form['resources'] = array(
    '#type' => 'tableselect',
    '#weight' => '3',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content available. Try exposing available, un-exposed, records from the <em>Availability</em> options.'),
  );  

  $form['filter'] = osf_entities_admin_content_filters_form();
  $form['#submit'][] = 'osf_entities_admin_content_filter_form_submit';
    
  pager_default_initialize($nbRecords, $itemsPerPage);
    
  $form['pager'] = array('#markup' => theme('pager'));
  $form['pager']['#weight'] = '4';
  return $form;  
}

function osf_entities_form_osf_entities_admin_content_alter(&$form, &$form_state, $form_id)
{
  // Disable the checkboxes until we start doing something with them
  $form['resources']['#disabled'] = true;
}

function osf_entities_admin_content_filter_form_submit($form, &$form_state) {
  $filters = osf_entities_admin_content_filters();
  switch (strtolower($form_state['values']['op'])) {
    case t('filter'):
    case t('refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['resource_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case t('undo'):
      array_pop($_SESSION['resource_overview_filter']);
      break;
    case t('reset'):
      $_SESSION['resource_overview_filter'] = array();
      break;
    case t('show available'):
      if(isset($form_state['values']['available']) && $form_state['values']['available']['available'] !== 0)
      {
        $_SESSION['resource_overview_show_available'] = TRUE;  
      }
      else
      {
        $_SESSION['resource_overview_show_available'] = FALSE;  
      }
      
      // Clean up sessions variables and cache to re-initialize the search for taking
      // this new setting into account
      $_SESSION['resource_overview_filter_search_terms'] = '';
      $_SESSION['resource_overview_filter'] = array();
      cache_clear_all('osf_entities_filters_datasets_'.session_id(), 'cache', TRUE); 
      cache_clear_all('osf_entities_filters_types_'.session_id(), 'cache', TRUE); 
      
      break;
    case t('remove'):
      $_SESSION['resource_overview_filter_search_terms'] = '';
      break;
    case t('search'):
      $_SESSION['resource_overview_filter_search_terms'] = $form_state['values']['searchfield'];
      break;
    case t('reorganize'):
      $_SESSION['resource_overview_columns'] = $form_state['values']['columns'];
      break;
  }
}

function osf_entities_admin_content_filters_form() {
  $session = isset($_SESSION['resource_overview_filter']) ? $_SESSION['resource_overview_filter'] : array();
  $filters = osf_entities_admin_content_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#weight' => '0',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
  );
  
  foreach ($session as $filter) {
    list($type, $value) = $filter;

    $value = $filters[$type]['options'][$value];

    $t_args = array(
      '%property' => $filters[$type]['title'],
      '%value' => $value,
    );
    
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array(
      '#type' => 'submit',
      '#value' => t('Undo'),
    );
    $form['filters']['status']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }

  drupal_add_js('misc/form.js');

  return $form;
}

function osf_entities_admin_content_filters() {

  $filters = array();
  $filterTypes = &drupal_static('osf_entities_filters_types');
  $filterDatasets = &drupal_static('osf_entities_filters_datasets');
  
  $filters['dataset'] = array(
    'title' => t('dataset'),
    'options' => array(
      '[any]' => t('any'),
    ) + $filterDatasets,
  );

  $filters['type'] = array(
    'title' => t('type'),
    'options' => array(
      '[any]' => t('any'),
    ) + $filterTypes,
  );
     
  return $filters;
}

/**
 * Resource Form.
 */
function osf_entities_resource_type_form($form, &$form_state, $resource) {  
  // Overwrite possible theme CSS such that we get a padding at the left of the title of each property
  drupal_add_css('form .field-multiple-table th.field-label {padding-left: 5px;}',$option['type'] = 'inline');

  global $language;
  
  if(is_site_multilingual() && $_GET['q'] != 'system/ajax')
  {
    drupal_set_message(t('You are editing the <strong>%lang</strong> version of this website', array('%lang' => $language->name)), 'warning');
  }  
  
  if(!isset($resource->uri))
  {
    $form_state['is_new_entity'] = TRUE;
  }
  else
  {
    $form_state['is_new_entity'] = FALSE;
  }

  // Check if the user has access to any dataset
  if(empty($resource->dataset))
  {
    // If this is a new entity, we check if the user can at least save it in one dataset
    $accessibleDatasets = osf_get_accessible_datasets();

    if(empty($accessibleDatasets))
    {
      form_set_error('osf_DatasetAccess', t('You do not have the ability to save any records in any dataset'));   
      return; 
    } 
  }
  else
  {
    // If this is an existing entity, we check if the user has the rights to modify it into that dataset
    $accessibleDatasets = osf_get_accessible_datasets(TRUE);

    if(!isset($accessibleDatasets[$resource->dataset]))
    { 
      form_set_error('osf_DatasetAccess', t('You do not have the ability to save this record in this dataset: '.$resource->dataset));   
      return; 
    }    
  }

  foreach($accessibleDatasets as $key => $accessibleDataset)
  {
    $info = osf_configure_api_fetch_dataset_info($accessibleDataset);
    
    $accessibleDatasets[$key] = $info['prefLabel'];
  }
  
  $datasets = array_merge(array('Select a dataset'), array_values($accessibleDatasets));
  $datasetIndex = array_merge(array('Select a dataset'), array_keys($accessibleDatasets));
  
  $form_state['dataset_index'] = $datasetIndex;
  
  $form_state['resource'] = $resource;
  
  // Add all the fields and possible values to the $form.  
  field_attach_form('resource', $resource, $form, $form_state);
  
  // Delete the iron_preflabel and iron_description fields if they got generated
  unset($form['iron_preflabel']);
  unset($form['iron_description']);
  unset($form_state['field']['iron_preflabel']);
  unset($form_state['field']['iron_description']);
  
  $defaultPrefLabel = '';
  $uriFragment = '';
        
  if(strpos(drupal_get_path_alias(), 'resource/add') !== FALSE)
  {
    $path_alias = trim(str_replace('resource/add', '', drupal_get_path_alias()), '/');
    
    $pos = strpos($path_alias, '/');
    
    if($pos !== FALSE)
    {
      $path_alias = substr($path_alias, $pos + 1);
      
      $defaultPrefLabel = $path_alias;
      
      $uriFragment = osf_entities_get_drid_from_string($path_alias);
    }
  }
  else
  {
    if(isset($resource->dataset) && $resource->dataset != "")
    {
      $uriFragment = str_replace($resource->dataset, "", $resource->uri);
    }    
  }
    
  // Replace them by the general and generic version
  $form['iron_preflabel'] = array(
    '#weight' => -108,
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Preferred label'),
    '#description' => t('Preferred label to use for referring to this new entity. This preferred label will appear everywhere in the user interface.'),
    '#default_value' => $defaultPrefLabel,
  );

  $form['iron_description'] = array(
    '#weight' => -107,
    '#type' => 'textarea',
    '#required' => FALSE,
    '#title' => t('Description'),
    '#description' => t('Description of this new entity. The description is a short abstract describing the entity.'),
    '#default_value' => '',
  );  
  
  $properties = variable_get('osf_entities_properties', array());
  $propertiesByField = variable_get('osf_entities_properties_by_field', array());
  
  $namespaces = new Namespaces();
  
  // Special hangling of possible lat/long values
  // If the lat/long of an entity is specified in two properties
  // then we have to display a single geolocation widget.
  if(module_exists('geolocation'))
  {
    if(isset($form['geo_lat']) &&
       isset($form['geo_long']))
    {
      unset($form['geo_long']);
    }
  }
  
  // If we are editing an existing resource, we populate the forms fields
  // with the values of that entity
  if(isset($resource->dataset) && $resource->dataset != "")
  { 
    // Populate the fields with the current values of the entity we are editing. 
    foreach($form as $field => $fieldValue)
    {
      // Check if the field existing in the entity's description
      if(isset($resource->{$field}))
      {
        foreach($resource->{$field} as $propertyLang =>  $propertyValues)
        {
          if($propertyLang != $language->language)
          {
            continue;
          }          
          
          foreach($propertyValues as $key => $propertyValue)
          {
            $propertyValue = $propertyValue['value'];
            
            // Populate the generic preflabel & description fields
            if($field == 'iron_preflabel')
            {              
              $form['iron_preflabel']['#default_value'] = $propertyValue;
              continue;
            }
            
            if($field == 'iron_description')
            {
              $form['iron_description']['#default_value'] = $propertyValue;
              continue;
            }
            
            $lang = $form[$field]['#language'];
            
            // Try to get the preferred label for the record referenced by this property
            if(isset($propertiesByField[$field]) && isset($properties[$propertiesByField[$field]]))
            {
              $property = unserialize($properties[$propertiesByField[$field]]);
            }

            // Define the proper default value depending on the type of the field
            $defaultValue = $propertyValue;            
            
            // Check if a drupal:value is reified for this triple. If there is one
            // the use it. Otherwise define a default one.            
            
            $fieldInfo = field_info_field($field);
            
            $existingValues = $resource->obj->getDataPropertyValues($property->getUri());
            if(!empty($existingValues) && isset($existingValues[$key]['reify']))
            {
              $defaultValue = unserialize(current($existingValues[$key]['reify']['http://purl.org/ontology/drupal#value']));
            }
            else
            {            
              switch($fieldInfo['type'])
              {
                case 'link_field':
                  $defaultValue = array('url' => $propertyValue,
                                        'title' => '');                                      
                break;
                
                case 'geolocation_latlng':
                  switch($propertiesByField[$field])
                  {
                    case Namespaces::$geo.'lat':
                      $defaultValue = array('lat' => $defaultValue,
                                            'lng' => current($resource->geo_long['value']),
                                            'field' => '');                                      
                    break;
                    case Namespaces::$geo.'lat_long':
                      $latLong = explode(',', $propertyValue);
                      
                      $defaultValue = array('lat' => $latLong[0],
                                            'lng' => $latLong[1],
                                            'field' => '');                                      
                    break;
                  }
                break;
                
                default:
                  $defaultValue = array('value' => $propertyValue);                                      
                break;
              }
            }
            
            // Check to add more values if more values exists for a field.
            // This should be checked if the type of the field is not
            // one of the following ones            
            if($fieldInfo['type'] != 'list_boolean' &&
               $fieldInfo['type'] != 'list_float' )
            {
              if(!isset($form[$field][$lang][$key]))
              {
                $form[$field][$lang][] = $form[$field][$lang][($key - 1)];          
                
                // Properly setup the deltas of the new item
                $form[$field][$lang][$key]['#delta'] = $key;
                
                $form[$field][$lang][$key]['field_remove_item']['#name'] = 'op-remove-'.str_replace('_', '-', $field).'-'.$lang.'-'.$key;
              }
            }            
            
            // Now do populate the field, within the form, such that the field will
            // display the value to the user.            
            if($fieldInfo['type'] == 'list_boolean')
            {
              if(is_array($form[$field][$lang]['#default_value']))
              {
                $form[$field][$lang]['#default_value'] = array(filter_var($propertyValue, FILTER_VALIDATE_BOOLEAN)); 
              }
              else
              {
                $form[$field][$lang]['#default_value'] = filter_var($propertyValue, FILTER_VALIDATE_BOOLEAN); 
              }
            }
            elseif($fieldInfo['type'] == 'geolocation_latlng')
            {                                             
              $form[$field][$lang][$key]['lat']['#default_value'] = $defaultValue['lat']; 
              $form[$field][$lang][$key]['lng']['#default_value'] = $defaultValue['lng']; 
              $form[$field][$lang][$key]['address']['field']['#value'] = $defaultValue['field']; 
            }
            elseif($fieldInfo['type'] == 'number_decimal' ||
                   $fieldInfo['type'] == 'number_float' ||
                   $fieldInfo['type'] == 'number_integer')
            {
              $form[$field][$lang][$key]['value']['#default_value'] = $propertyValue;
            }
            elseif($fieldInfo['type'] == 'list_float' ||
                   $fieldInfo['type'] == 'list_integer' ||
                   $fieldInfo['type'] == 'list_text')
            {
              if($fieldInfo['cardinality'] == FIELD_CARDINALITY_UNLIMITED)
              {
                if(!is_array($form[$field][$lang]['#default_value']))
                {
                  $form[$field][$lang]['#default_value'] = array();
                }
                
                $form[$field][$lang]['#default_value'][] = array_search($propertyValue, $form[$field][$lang]['#options']);
              }
              else
              {
                $form[$field][$lang]['#default_value'] = array_search($propertyValue, $form[$field][$lang]['#options']);  
              }
            }
            elseif($fieldInfo['type'] == 'date' || 
                   $fieldInfo['type'] == 'datetime' ||
                   $fieldInfo['type'] == 'datestamp')
            {
              $form[$field][$lang][$key]['#default_value'] = $defaultValue;
            }
            elseif($fieldInfo['type'] == 'file' ||
                   $fieldInfo['type'] == 'image' ||
                   $fieldInfo['type'] == 'link_field')
            {
              $form[$field][$lang][$key]['#default_value'] = $defaultValue;
            }
            elseif($fieldInfo['type'] == 'text_long')
            {
              $form[$field][$lang][$key]['value']['#default_value'] = $defaultValue['value'];
            }
            elseif($fieldInfo['type'] == 'text_with_summary')
            {
              $form[$field][$lang][$key]['#format'] = $defaultValue['format'];
              $form[$field][$lang][$key]['summary']['#default_value'] = $defaultValue['summary'];
              $form[$field][$lang][$key]['#default_value'] = $defaultValue['value'];
            }
            elseif($fieldInfo['type'] == 'text')
            {
              $form[$field][$lang][$key]['value']['#default_value'] = $defaultValue['value'];
            }
            else
            {
              $form[$field][$lang][$key]['#default_value'] = $defaultValue['value'];
            }
          }
        }
      }
    }
  } 
  
  // Here we check if a field is disabled. If it is disabled, when we hide it from the user.
  // The trick used to be able to hide the field is by using a form element state.
  // It checks if the table where the fields values are is "checked". However, since a table
  // cannot be checked, it always returns false. Then the state hide the table element to the user.
  // More information about states here: 
  //   http://api.drupal.org/api/drupal/includes!common.inc/function/drupal_process_states/7
  //
  // Note: we just want to hide them, because we still want their values when we will save the
  //       entity into the OSF instance.
  foreach($form as $field => $fieldValue)
  {
    if(is_array($form[$field]) && 
       isset($form[$field]['#language']) && 
       isset($form[$field]['#disabled']) && $form[$field]['#disabled'])
    {
      $lang = $form[$field]['#language'];
      
      $id = str_replace('_', '-', $form[$field][$lang]['#field_name']).'-values';
      
      $form[$field]['#states'] =   array(
        'invisible' => array(
          '#'.$id => array('checked' => TRUE),
        ),
      );
    }    
  }  
  
  // Let's do the same for the field instances that have been specified in the 'Manage Display' section
  // to be hidden
  foreach($form_state['field'] as $field => $fieldValue)
  {
    // Check if the field is hidden in the default display
    // @TODO maybe in the future we should update that such that it handles other (non default) displays
    $displayType = $fieldValue[key($fieldValue)]['instance']['display']['default']['type'];
    
    if($displayType == 'hidden')
    {
      // If it is hidden, then we hide it it in the form
      if(is_array($form[$field]) && 
         isset($form[$field]['#language']))
      {
        $lang = $form[$field]['#language'];
        
        $id = str_replace('_', '-', $form[$field][$lang]['#field_name']).'-values';
        
        $form[$field]['#states'] =   array(
          'invisible' => array(
            '#'.$id => array('checked' => TRUE),
          ),
        );
      }       
    }
  }
  
  $form['uriFragment'] = array(
    '#weight' => -110,
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Unique identifier'),
    '#description' => t('The final unique identifier (URI) is generated from the concatenation of this Unique identifier and the URI of the dataset where the record get saved.'),
    '#default_value' => $uriFragment,
  );

  if(isset($resource->uri))
  {
    $form['fullUri'] = array(
      '#type' => 'hidden',
      '#value' => $resource->uri,
    );
  }
  
  $form['dataset'] = array(
    '#weight' => -109,
    '#type' => 'select',
    '#options' => $datasets,
    '#required' => TRUE,
    '#title' => t('Dataset'),
    '#default_value' => 0,
    '#description' => t('Dataset where to save that record')
  );   
  
  if(isset($resource->dataset) && $resource->dataset != "")
  {
    $form['dataset']['#disabled'] = TRUE;
    $form['dataset']['#default_value'] = array_search($resource->dataset, $datasetIndex);
  }  
  
  if(isset($resource->uri) && $resource->uri != "")
  {
    $form['uriFragment']['#disabled'] = TRUE;
  }  
  
  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 10000,
  );           
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save resource'),
    '#submit' => $submit + array('osf_entities_resource_form_submit'),
  );
  
  // Adding moderation features
  $form['moderations'] = array(
    '#type' => 'fieldset',
    '#collapsable' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Resource Moderation'),
    '#weight' => 100
  );
  
  $moderationOptions = array(
    'no_revision_no_moderation' => t('Modify current resource, no revision, no moderation'),
    'revision_no_moderation' => t('Create new revision, no moderation'),
    'revision_moderation' => t('Create new revision and moderate'),
  );
  
  $form['moderations']['moderation_options'] = array(
    '#type' => 'radios',
    '#options' => $moderationOptions,
    '#default_value' => 'revision_no_moderation',
    '#title' => t('Revision creation and moderation options'),
    '#description' => t('Moderation means that the new revision is not publicly visible until approved by someone with the appropriate permissions.'),
  );
  
  $form['#validate'][] = 'osf_entities_resource_form_validate';

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'osf_entities') . '/js/autocomplete.js',
  );  

  return $form;
}   

/**
* Validate a resource-type form when an entity form is being saved
*/
function osf_entities_resource_form_validate($form, &$form_state) {
  if($form_state['is_new_entity'])
  {
    // Make sure that if the user is trying to create a new entity, that this entity's
    // URI is not already existing in this dataset
    $dataset = $form_state['dataset_index'][$form_state['complete form']['dataset']['#value']];

    $uri = $dataset.$form_state['values']['uriFragment'];

    $entity = entity_load('resource_type', array($uri));

    if(!empty($entity))
    {
      form_set_error('uriFragment', t("The new resource you are trying to create is already existing in the system. Please choose a new unique identifier for this new resource you are creating."));
    }
    
    // Make sure that a dataset has been selected by the user.
    if($form_state['complete form']['dataset']['#value'] == 0)
    {
      form_set_error('dataset', t("Please select the dataset where you want to save that entity."));      
    }
    
    // Check if we have to modify the unique ID that as been provided
    if(osf_entities_get_drid_from_string($form_state['values']['uriFragment']) !== $form_state['values']['uriFragment'])
    {
      $form_state['complete form']['uriFragment']['#value'] = osf_entities_get_drid_from_string($form_state['values']['uriFragment']);
      form_set_error('uriFragment', t("The unique identifier you provided was not valid. We modified it such that it become valid. Please review it and save the entity again."));      
    }
  }
}

/**
 * Save an entity instance in OSF upon saving
 */
function osf_entities_resource_form_submit($form, &$form_state) {  
  $resource = $form_state['resource'];
  
  // Change the reference to the URIs of the object properties
  $properties = variable_get('osf_entities_properties', array());
  $propertiesByField = variable_get('osf_entities_properties_by_field', array());  
  
  $namespaces = new Namespaces();
  
  // Fix some values we want to save as RDF using osf_entities_get_rdf_entity() down the road
  foreach($form as $field => $fieldValue)
  {
    $fieldInfo = field_info_field($field);
    
    // What we do here is to transform the #value of a list
    // such that the value of the list is saved into OSF
    // instead of the key of the list
    if($fieldInfo['type'] == 'list_float' ||
       $fieldInfo['type'] == 'list_integer' || 
       $fieldInfo['type'] == 'list_text')
    {
      $lang = $form[$field]['#language'];
      $options = $form[$field][$lang]['#options'];
      $value = $form[$field][$lang]['#value'];
      
      if(is_array($value))
      {
        $newValues = array();
        
        foreach($value as $val)
        {
          $newValues[] = array('value' => $options[$val]);  
        }
        
        $form_state['values'][$field][$lang] = $newValues;
      }
      else
      {
        $form_state['values'][$field][$lang] = array(array('value' => $options[$value]));
      }
    }
  }  

  
  // Let's take the values from the form, and populate the $resource according to the changes.
  // Then we will save the $resource into OSF once the changes are applied.
  entity_form_submit_build_entity('resource', $resource, $form, $form_state);

  // Now let's replace the values from the form that defines the current language with the
  // previous values defined for that resource and that language.
  foreach(get_object_vars($resource) as $property => $values)
  {
    if(is_array($resource->{$property}) && 
       /*isset($resource->{$property}[get_lang()]) &&*/
       isset($resource->{$property}[LANGUAGE_NONE]))
    {
      $resource->{$property}[get_lang()] = $resource->{$property}[LANGUAGE_NONE];
      unset($resource->{$property}[LANGUAGE_NONE]);
    }
  }
  
  // Properly setup the dataset URI
  $resource->dataset = $form_state['dataset_index'][$resource->dataset];

  // Create the URI property in our resource entity
  if(!empty($resource->fullUri))
  {
    $resource->{'uri'} = $resource->fullUri;
  }
  else
  {
    $resource->{'uri'} = $resource->dataset.$resource->uriFragment;
  }
  
  // Populate prefLabel & description
  if(!empty($form['iron_preflabel']['#value']))
  {
    $resource->{'iron_preflabel'}[get_lang()] = array(array('value' => $form['iron_preflabel']['#value']));
    $resource->preflabel[get_lang()][0]['value'] = $form['iron_preflabel']['#value'];
    $resource->label = $form['iron_preflabel']['#value'];
  }
  
  if(!empty($form['iron_description']['#value']))
  {
    $resource->{'iron_description'}[get_lang()] = array(array('value' => $form['iron_description']['#value']));
    $resource->description[get_lang()][0]['value'] = $form['iron_description']['#value'];
  }

  // Remove unnecessary entity information for Resource
  unset($resource->uriFragment);
  
  $resource->{'moderation'} = $form_state['input']['moderation_options'];
  
  resource_type_save($resource);
  
  global $base_url;
  
  $form_state['redirect'] = osf_entities_get_resource_page_url_from_entity_uri($resource->uri);

  $messages = drupal_get_messages('error', FALSE);
  
  if(!empty($messages['error']))
  {
    drupal_set_message(t('Resource %title couln\'d be saved.', array('%title' => entity_label('resource', $resource))), 'warning');
  }
  else
  {
    drupal_set_message(t('Resource %title saved.', array('%title' => entity_label('resource', $resource))));
  }
}

/**
* Handle resource-type entity deletation when the request is submitted
*/
function osf_entities_resource_form_submit_delete($form, &$form_state) {
  $resource = $form_state['resource'];
  $resource_uri = entity_uri('resource', $resource);
  $form_state['redirect'] = $resource_uri['path'] . '/delete';
}

/**
 * Delete confirmation form.
 */
function osf_entities_resource_delete_form($form, &$form_state, $resource) {
  $form_state['resource'] = $resource;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['resource_type_id'] = array('#type' => 'value', '#value' => entity_id('resource' ,$resource));
  $resource_uri = entity_uri('resource', $resource);
  return confirm_form($form,
    t('Are you sure you want to delete resource %title?', array('%title' => entity_label('resource', $resource))),
    $resource_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function osf_entities_resource_delete_form_submit($form, &$form_state) {
  $resource = $form_state['resource'];
  resource_delete($resource);

  drupal_set_message(t('Resource %title deleted.', array('%title' => entity_label('resource', $resource))));

  $form_state['redirect'] = '<front>';
}

function osf_entities_resource_revisions_view($entity, $revisionTime)
{
  $revisionTimeInt = substr($revisionTime, 0, strpos($revisionTime, '.'));
  
  $entityRevisionUri = $entity[key($entity)]->dataset.'revisions/'.$revisionTime;
  
  $revisionEntity = entity_load('resource_type', array($entityRevisionUri.'::'.$entity[key($entity)]->dataset));

  drupal_set_title(t('Revision of %title from %date', array('%title' => $revisionEntity[key($revisionEntity)]->label, '%date' => format_date($revisionTimeInt))), PASS_THROUGH);

  return(osf_entities_resource_page_view($revisionEntity));
}

function osf_entities_resource_republish($form, $form_state, $entityURI, $datasetURI) 
{
  $form['#republishEntityURI'] = urldecode($entityURI);
  $form['#republishDatasetURI'] = urldecode($datasetURI);
  
  return confirm_form($form,
    t('Are you sure you want to republish the latest revision of the record %uri from this %dataset?', array('%uri' => urldecode($entityURI),
                                                                                                  '%dataset' => urldecode($datasetURI))),
    'admin/resource',
    t('You are trying to re-publish an unpublished record.'),
    t('Republish'),
    t('Cancel')
  );
}

function osf_entities_revision_revert_confirm($form, $form_state, $entity, $revisionTime) 
{
  $revisionTimeInt = substr($revisionTime, 0, strpos($revisionTime, '.'));
  
  $entityRevisionUri = $entity[key($entity)]->dataset.'revisions/'.$revisionTime;
  
  $revisionEntity = entity_load('resource_type', array($entityRevisionUri.'::'.$entity[key($entity)]->dataset));

  $form['#resource_entity'] = $entity[key($entity)];
  $form['#revisionned_resource_entity'] = $revisionEntity[key($revisionEntity)];
  $form['#revisionTime'] = $revisionTimeInt;
  $form['#revisionUri'] = $entityRevisionUri;
  
  return confirm_form($form, t('Are you sure you want to revert to the revision from %revision-date?', array('%revision-date' => format_date($revisionTimeInt))), 'resources/' . rawurldecode($entity[key($entity)]->drid) . '/revisions', '', t('Revert'), t('Cancel'));
}

function osf_entities_resource_republish_submit($form, &$form_state) 
{
  $defaultEndpoint = osf_configure_get_endpoint_by_uri(osf_entities_get_network_from_dataset_uri($form['#republishDatasetURI']));
  
  $revisionLister = new RevisionListerQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());

  $revisionLister->longResults()
                 ->uri($form['#republishEntityURI'])
                 ->dataset($form['#republishDatasetURI'])
                 ->send(new DrupalQuerierExtension());
                 
  if($revisionLister->isSuccessful())
  {
    // each of them that are not published get deleted
    $revisions = $revisionLister->getResultset()->getResultset();
    
    $revisions = $revisions['unspecified'];
    
    $latestRevisionURI = key($revisions);
  
    $revisionUpdate = new RevisionUpdateQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());

    $revisionUpdate->dataset($form['#republishDatasetURI'])
                   ->revisionUri($latestRevisionURI)
                   ->isPublished()
                   ->send(new DrupalQuerierExtension());

    if(!$revisionUpdate->isSuccessful())                   
    {
      drupal_set_message(t("Can't republish the record URI (@uri)): [@error] @errorMsg - @errorDescription",
                         array("@uri" => $form['#republishEntityURI'],
                               "@error" => $revisionUpdate->getStatus(),
                               "@errorMsg" => $revisionUpdate->getStatusMessage(),
                               "@errorDescription" => $revisionUpdate->getStatusMessageDescription())),
                         "error",
                         TRUE); 
                         
      $form_state['redirect'] = 'admin/resource';      
    }
    else
    {               
      $form_state['redirect'] = 'resources/' . urlencode($form['#republishEntityURI']);
    }
  }
  else
  {
    drupal_set_message(t("Can't list the revisions for the record URI (@uri)): [@error] @errorMsg - @errorDescription",
                       array("@uri" => $form['#republishEntityURI'],
                             "@error" => $revisionLister->getStatus(),
                             "@errorMsg" => $revisionLister->getStatusMessage(),
                             "@errorDescription" => $revisionLister->getStatusMessageDescription())),
                       "error",
                       TRUE); 
    $form_state['redirect'] = 'admin/resource';      
  }
}

function osf_entities_revision_revert_confirm_submit($form, &$form_state) {
  $entity = $form['#resource_entity'];
  $revisionEntity = $form['#revisionned_resource_entity'];

  $defaultEndpoint = osf_configure_get_endpoint_by_uri(osf_entities_get_network_from_dataset_uri($entity->dataset));

  $revisionUpdate = new RevisionUpdateQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
  
  $revisionUpdate->dataset($entity->dataset)
                 ->isPublished()
                 ->revisionUri($form['#revisionUri'])
                 ->send(new DrupalQuerierExtension());
                 
  if($revisionUpdate->isSuccessful())
  {
    watchdog('osf_entities', 'Resource type: reverted %title revision %revision.', array('%title' => $revisionEntity->label, '%revision' => $form['#revisionUri']));
    drupal_set_message(t('%title has been reverted back to the revision from %revision-date.', array('%title' => $revisionEntity->label, '%revision-date' => format_date($form['#revisionTime']))));
    
    // delete cache for this entity
    cache_clear_all($entity->uri, 'cache_entity_resource_type');
  }
  else
  {
    drupal_set_message(t("Can't revert to that revision: [@error] @errorMsg - @errorDescription",
                       array("@error" => $revisionUpdate->getStatus(),
                             "@errorMsg" => $revisionUpdate->getStatusMessage(),
                             "@errorDescription" => $revisionUpdate->getStatusMessageDescription())),
                       "error",
                       TRUE);
                       
    watchdog('osf_entities', 
             'Can\'t revert to that revision: [@error] @errorMsg - @errorDescription.', 
             array("@error" => $revisionUpdate->getStatus(),
                   "@errorMsg" => $revisionUpdate->getStatusMessage(),
                   "@errorDescription" => $revisionUpdate->getStatusMessageDescription()));                       
  }
  
  $form_state['redirect'] = 'resources/' . rawurldecode($entity->drid) . '/revisions';
}

function osf_entities_revision_delete_confirm($form, $form_state, $entity, $revisionTime) {

  $revisionTimeInt = substr($revisionTime, 0, strpos($revisionTime, '.'));
  
  $entityRevisionUri = $entity[key($entity)]->dataset.'revisions/'.$revisionTime;
  
  $revisionEntity = entity_load('resource_type', array($entityRevisionUri.'::'.$entity[key($entity)]->dataset));

  $form['#resource_entity'] = $entity[key($entity)];
  $form['#revisionned_resource_entity'] = $revisionEntity[key($revisionEntity)];
  $form['#revisionTime'] = $revisionTimeInt;
  $form['#revisionUri'] = $entityRevisionUri;

  return confirm_form($form, t('Are you sure you want to delete the revision from %revision-date?', array('%revision-date' => format_date($revisionTimeInt))), 'resources/' . rawurldecode($entity[key($entity)]->drid) . '/revisions', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}


function osf_entities_revision_delete_confirm_submit($form, &$form_state) {
  $entity = $form['#resource_entity'];
  $revisionEntity = $form['#revisionned_resource_entity'];
  
  // Delete revision
  $defaultEndpoint = osf_configure_get_endpoint_by_uri(osf_entities_get_network_from_dataset_uri($entity->dataset));
  
  $revisionDelete = new RevisionDeleteQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
  
  $revisionDelete->dataset($entity->dataset)
                 ->revisionUri($form['#revisionUri'])
                 ->send(new DrupalQuerierExtension());
                 
  if($revisionDelete->isSuccessful())
  {
    watchdog('osf_entities', 'Resource type: deleted %title revision %revision.', array('%title' => $revisionEntity->label, '%revision' => $form['#revisionUri']));
    drupal_set_message(t('Revision from %revision-date of %title has been deleted.', array('%revision-date' => format_date($form['#revisionTime']), '%title' => $revisionEntity->label)));
    
    // delete cache for this entity
    cache_clear_all($entity->uri, 'cache_entity_resource_type');    
    $form_state['redirect'] .= 'resources/'.rawurldecode($entity->drid).'/revisions';    
  }
  else
  {
    drupal_set_message(t("Can't delete that revision: [@error] @errorMsg - @errorDescription",
                       array("@error" => $revisionDelete->getStatus(),
                             "@errorMsg" => $revisionDelete->getStatusMessage(),
                             "@errorDescription" => $revisionDelete->getStatusMessageDescription())),
                       "error",
                       TRUE);
                       
    watchdog('osf_entities', 
             'Can\'t delete that revision: [@error] @errorMsg - @errorDescription.', 
             array("@error" => $revisionDelete->getStatus(),
                   "@errorMsg" => $revisionDelete->getStatusMessage(),
                   "@errorDescription" => $revisionDelete->getStatusMessageDescription()));          
  }  
}

function osf_entities_revisions_compare($entity, $right, $left)
{
  $rightDate = substr($right, 0, strpos($right, '.'));
  $leftDate = substr($left, 0, strpos($left, '.'));
  
  $entity = $entity[key($entity)];
  
  $defaultEndpoint = osf_configure_get_endpoint_by_uri(osf_entities_get_network_from_dataset_uri($entity->dataset));
  
  $diff = new RevisionDiffQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
  
  $diff->dataset($entity->dataset)
       ->leftRevisionUri($entity->dataset.'revisions/'.$left)
       ->rightRevisionUri($entity->dataset.'revisions/'.$right)
       ->mime('resultset')
       ->send(new DrupalQuerierExtension());
  
  if($diff->isSuccessful())
  {
    $resultset = $diff->getResultset()->getResultset();
    $resultset = $resultset['unspecified'];

    $header = array(array('data' => t(t('!date', array('!date' => l(format_date($leftDate, 'short'), 'resources/'.rawurldecode($entity->drid).'/revisions/'.$left.'/view')))), 'colspan' => 2),
                    array('data' => t(t('!date', array('!date' => l(format_date($rightDate, 'short'), 'resources/'.rawurldecode($entity->drid).'/revisions/'.$right.'/view')))), 'colspan' => 2));      
                    
    $rows = array();    
    $changes = array();

    foreach($resultset as $uri => $change)
    {
      if(!isset($change[Namespaces::$rdf.'predicate']))
      {
        continue;
      }
      
      $property = $change[Namespaces::$rdf.'predicate'][0]['uri'];
      
      if(isset($change[Namespaces::$rdf.'object'][0]['value']))
      {
        $object = $change[Namespaces::$rdf.'object'][0]['value'];
      }
      else
      {
        $object = get_label_from_uri($change[Namespaces::$rdf.'object'][0]['uri']);
      }

      if(!isset($changes[$property]))
      {
        $changes[$property] = array(
                                'removals' => array(),
                                'additions' => array()
                              );
      }
      
      if(strpos($uri, 'removal-') !== FALSE)
      {
        $changes[$property]['removals'][] = $object;        
      }
      elseif(strpos($uri, 'addition-') !== FALSE)
      {
        $changes[$property]['additions'][] = $object;        
      }      
    }
    
    foreach($changes as $property => $change)
    {
      $rows[] = array(array(
                  'data' => t(get_label_from_uri($property)),
                  'style' => array('font-weight: bolder'),  
                  'colspan' => 4
                ));                  
      
      if(count($change['removals']) == 1 && count($change['additions']) == 1)
      {
        $row = array();
        
        $row[] = array(
                    'data' => t('-'),
                  );
        
        $row[] = array(
                    'data' => t($change['removals'][0]),
                    'style' => array('background-color: #FF4848; font-weight: bold;'),
                  );    
        
        $row[] = array(
                    'data' => t('+'),
                  );
        
        $row[] = array(
                    'data' => t(htmlDiff($change['removals'][0], $change['additions'][0])),
                    'style' => array('background-color: #79FC4E; font-weight: bold;'),
                  );
                  
        $rows[] = $row;        
      }
      else
      {
        // If there are multiple removals and additions for a single property, it means that
        // we are facing a multi-valued property for which there has been multiple changes.
        // In this case, we cannot highlight the differences since we don't know what as been
        // replaced by what
        
        $additionsPadding = 0;
        $removalsPadding = 0;
        
        if(count($change['removals']) > count($change['additions']))
        {
          foreach($change['removals'] as $key => $removal)
          {
            $row = array();
            
            $row[] = array(
                        'data' => t('-'),
                      );
            
            $row[] = array(
                        'data' => t($removal),
                        'style' => array('background-color: #FF4848; font-weight: bold;'),
                      );    
            
            if(isset($change['additions'][$key]))
            {
              $row[] = array(
                          'data' => t('+'),
                        );
              
              $row[] = array(
                          'data' => t($change['additions'][$key]),
                          'style' => array('background-color: #79FC4E; font-weight: bold;'),
                        );  
            }
            else
            {
              $row[] = '';
              $row[] = '';                  
            }
            
            $rows[] = $row;  
          }
        }
        else
        {
          foreach($change['additions'] as $key => $addition)
          {
            $row = array();
            
            if(isset($change['removals'][$key]))
            {
              $row[] = array(
                          'data' => t('-'),
                        );
              
              $row[] = array(
                          'data' => t($change['removals'][$key]),
                          'style' => array('background-color: #FF4848; font-weight: bold;'),
                        );  
            }
            else
            {
              $row[] = '';
              $row[] = '';                  
            }   
                        
            $row[] = array(
                        'data' => t('+'),
                      );
            
            $row[] = array(
                        'data' => t($addition),
                        'style' => array('background-color: #79FC4E; font-weight: bold;'),
                      ); 
                      
            $rows[] = $row;  
          }
        }
      }
    }
    
    $form['back'] = array('#type' => 'link',
                          '#title' => t('<< Back to revisions'),
                          '#href' => substr(drupal_get_path_alias(), 0, strpos(drupal_get_path_alias(), '/with/')));
    
    $form['diff_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => $header,
    );    
    
    return($form);
    
  }
  else
  {
    drupal_set_message(t("Can't compare revisions: [@error] @errorMsg - @errorDescription",
                       array("@error" => $diff->getStatus(),
                             "@errorMsg" => $diff->getStatusMessage(),
                             "@errorDescription" => $diff->getStatusMessageDescription())),
                       "error",
                       TRUE);
                       
    watchdog('osf_entities', 
             'Can\'t compare revisions: [@error] @errorMsg - @errorDescription.', 
             array("@error" => $diff->getStatus(),
                   "@errorMsg" => $diff->getStatusMessage(),
                   "@errorDescription" => $diff->getStatusMessageDescription()));      
  }
}

/*
    Paul's Simple Diff Algorithm v 0.1
    (C) Paul Butler 2007 <http://www.paulbutler.org/>
    May be used and distributed under the zlib/libpng license.

    This code is intended for learning purposes; it was written with short
    code taking priority over performance. It could be used in a practical
    application, but there are a few ways it could be optimized.

    Given two arrays, the function diff will return an array of the changes.
    I won't describe the format of the array, but it will be obvious
    if you use print_r() on the result of a diff on some test data.

    htmlDiff is a wrapper for the diff command, it takes two strings and
    returns the differences in HTML. The tags used are <ins> and <del>,
    which can easily be styled with CSS.
*/

function diff($old, $new){
    $maxlen = 0;
    foreach($old as $oindex => $ovalue){
        $nkeys = array_keys($new, $ovalue);
        foreach($nkeys as $nindex){
            $matrix[$oindex][$nindex] = isset($matrix[$oindex - 1][$nindex - 1]) ?
                $matrix[$oindex - 1][$nindex - 1] + 1 : 1;
            if($matrix[$oindex][$nindex] > $maxlen){
                $maxlen = $matrix[$oindex][$nindex];
                $omax = $oindex + 1 - $maxlen;
                $nmax = $nindex + 1 - $maxlen;
            }
        }
    }
    if($maxlen == 0) return array(array('d'=>$old, 'i'=>$new));
    return array_merge(
        diff(array_slice($old, 0, $omax), array_slice($new, 0, $nmax)),
        array_slice($new, $nmax, $maxlen),
        diff(array_slice($old, $omax + $maxlen), array_slice($new, $nmax + $maxlen)));
}

function htmlDiff($old, $new){
    $ret = '';
    $diff = diff(explode(' ', $old), explode(' ', $new));
    foreach($diff as $k){
        if(is_array($k))
            $ret .= (!empty($k['d'])?'<del>'.implode(' ',$k['d']).'</del> ':'').
                (!empty($k['i'])?'<ins>'.implode(' ',$k['i']).'</ins> ':'');
        else $ret .= $k . ' ';
    }
    return $ret;
}

function osf_entities_revision_unpublish_confirm($form, $form_state, $entity, $revisionTime) {

  $revisionTimeInt = substr($revisionTime, 0, strpos($revisionTime, '.'));
  
  $entityRevisionUri = $entity[key($entity)]->dataset.'revisions/'.$revisionTime;
  
  $revisionEntity = entity_load('resource_type', array($entityRevisionUri.'::'.$entity[key($entity)]->dataset));

  $form['#resource_entity'] = $entity[key($entity)];
  $form['#revisionned_resource_entity'] = $revisionEntity[key($revisionEntity)];
  $form['#revisionTime'] = $revisionTimeInt;
  $form['#revisionUri'] = $entityRevisionUri;

  return confirm_form($form, t('Are you sure you want to unpublish the revision from %revision-date?', array('%revision-date' => format_date($revisionTimeInt))), 'resources/' . rawurldecode($entity[key($entity)]->drid) . '/revisions', t(''), t('Unpublish'), t('Cancel'));
}

function osf_entities_revision_unpublish_confirm_submit($form, &$form_state) {
  $entity = $form['#resource_entity'];
  $revisionEntity = $form['#revisionned_resource_entity'];
  
  // Unpublish revision
  $defaultEndpoint = osf_configure_get_endpoint_by_uri(osf_entities_get_network_from_dataset_uri($entity->dataset));
  
  $revisionUpdate = new RevisionUpdateQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
  
  $revisionUpdate->dataset($entity->dataset)
                 ->revisionUri($form['#revisionUri'])
                 ->isArchive()
                 ->send(new DrupalQuerierExtension());
  
 if($revisionUpdate->isSuccessful())
  {
    watchdog('osf_entities', 'Resource type: unpublished %title revision %revision.', array('%title' => $revisionEntity->label, '%revision' => $form['#revisionUri']));
    drupal_set_message(t('Revision from %revision-date of %title has been unpublished.', array('%revision-date' => format_date($form['#revisionTime']), '%title' => $revisionEntity->label)));
    
    // delete cache for this entity
    cache_clear_all($entity->uri, 'cache_entity_resource_type');    
    $form_state['redirect'] .= 'resources/'.rawurldecode($entity->drid).'/revisions';    
  }
  else
  {
    drupal_set_message(t("Can't update the publication status of this entity (@uri): [@error] @errorMsg - @errorDescription",
                       array("@uri" => $uri,
                             "@error" => $revisionUpdate->getStatus(),
                             "@errorMsg" => $revisionUpdate->getStatusMessage(),
                             "@errorDescription" => $revisionUpdate->getStatusMessageDescription())),
                       "error",
                       TRUE);
                       
    watchdog('osf_fieldstorage', 
             'Can\'t update the publication status of this entity: [@error] @errorMsg - @errorDescription.', 
             array("@uri" => $uri,
                   "@error" => $revisionUpdate->getStatus(),
                   "@errorMsg" => $revisionUpdate->getStatusMessage(),
                   "@errorDescription" => $revisionUpdate->getStatusMessageDescription()));             
  }          
}
