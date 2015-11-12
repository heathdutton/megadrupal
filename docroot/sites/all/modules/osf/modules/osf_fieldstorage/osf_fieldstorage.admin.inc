<?php
  
include_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'osf') . '/framework/ClassHierarchy.php';

use \StructuredDynamics\osf\php\api\ws\search\SearchQuery;
use \StructuredDynamics\osf\framework\Namespaces;
use \StructuredDynamics\osf\framework\Resultset;
use \StructuredDynamics\osf\framework\Subject;
use \StructuredDynamics\osf\ws\framework\ClassHierarchy;


/**
* 
*  Code related to the UI part of this module used to do the mapping between fields and OWL properties
* 
*  Most of this code come from the RDFUI module part of the RDFX project
* 
*/

/**
 * Implements hook_help().
 */
function osf_fieldstorage_help($path, $arg) {
  switch ($path) {
    case 'admin/structure/types/manage/%/rdf':
      return '<p>' . t('Manage the way this bundle and its fields are represented in RDF. The mappings defined here will be used to save the bundle\'s content into a OSF instance', array()) . '</p>';
  }
}

/**
 * Implements hook_theme().
 */
function osf_fieldstorage_theme($path, $arg) {
  return array(
  'osf_fieldstorage_term_autocomplete' => array(
      'render element' => 'term',
    ),
  );
}

/**
 * Menu callback; listing of field RDF mappings for a content type.
 *
 * Allows the content type to be mapped to a RDF class and
 * fields to be mapped to RDF properties.
 */
function osf_fieldstorage_admin_rdf_overview_form($form, &$form_state, $entity_type, $bundle, $instance) {
  $bundle = field_extract_bundle($entity_type, $bundle);
  $fields = field_info_instances($entity_type, $bundle);
  $mapping = rdf_mapping_load($entity_type, $bundle);
  
  // Don't make assumptions about entities and only get the type if dealing with a node entity
  $type = ($entity_type == 'node') ? node_type_get_type($bundle) : '';
  $form['rdf_mappings'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'osf_fieldstorage') . '/js/osf_fieldstorage.js',
        drupal_get_path('module', 'osf_fieldstorage') . '/js/autocomplete.js'
      ),
    ),
  );
  
  // @TODO Add linkage that come from OSF Entities here.

  // Declare the fieldset and field for RDF type.
  $form['type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Type'),
    '#group' => 'rdf_mappings',
    '#attributes' => array('class' => array('rdf-type')),
  );
  $form['type']['rdf_rdftype'] = array(
    '#type' => 'textarea',
    '#title' => t('RDF Type'),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'osf_fieldstorage') . '/osf_fieldstorage.js'),
    ),
  );

  // Declare the fieldset and field for RDF type.
  $form['type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Type'),
    '#group' => 'rdf_mappings',
    '#attributes' => array('class' => array('rdf-field')),
  );
  $form['type']['rdf_rdftype'] = array(
    '#type' => 'textarea',
    '#title' => t('RDF Type'),
    '#attributes' => array('class' => array('rdf-field')),
  );

  // Add the field for RDF type.
  osf_fieldstorage_rdftype_fieldset($form['type']['rdf_rdftype'], $mapping);

  // Add the options for all other fields.
  foreach ($fields as $field_name => $field) {

    // skip title & body core fields, there are handled automatically later in the process
    if($field_name == 'title' || $field_name == 'body')
    {
      continue;
    }
    
    // Make sure this field does use the "osf_fieldstorage" storage type
    $storageType = db_query('SELECT storage_type FROM {field_config} WHERE field_name = :field_name LIMIT 1', array(':field_name' => $field_name))->fetchField();

    if($storageType != 'osf_fieldstorage' && !variable_get('osf_fieldstorage_show_all_fields_rdf_mapping', 0))
    {
      continue;
    }
    
    $label = $field['label'];
    
    $form[$field_name] = array(
      '#type' => 'fieldset',
      '#group' => 'rdf_mappings',
      '#title' => t('@label', array('@label' => $label)),
      '#attributes' => array('class' => array('rdf-field')),
    );
    
    osf_fieldstorage_predicate_fieldset($form[$field_name], $mapping, $field_name, $label);
  }

  $form['field_names'] = array('#type' => 'value', '#value' => array_keys($fields));
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save mappings'));

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function osf_fieldstorage_form_field_ui_field_edit_form_alter(&$form, &$form_state) {

  $field_name = $form['#field']['field_name'];
  $instance = $form['instance'];
  $label = isset($instance['label']) ? $instance['label']['#default_value'] : $instance['field_name'];
  $entity_type = $instance['entity_type']['#value'];
  $mapping = rdf_mapping_load($entity_type, $instance['bundle']['#value']);

  $form['rdf'] = array(
    '#type' => 'fieldset',
    '#title' => t('@label RDF Mapping', array('@label' => $label)),
  );

  // add the predicate, etc fields
  osf_fieldstorage_predicate_fieldset($form['rdf'], $mapping, $field_name, $label);

  $form['submit']['#weight'] = 1;

  // add submit and validate handlers
  $form['#validate'] = array_merge($form['#validate'], array('osf_fieldstorage_form_field_ui_field_edit_form_validate'));
  $form['#submit'] = array_merge($form['#submit'], array('osf_fieldstorage_form_field_ui_field_edit_form_submit'));
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function osf_fieldstorage_form_node_type_form_alter(&$form, &$form_state) {
  $mapping = array();
  // If we are editing an existing content type, the mapping will be available
  // via the bundles in entity info.
  if ($bundle = $form['type']['#default_value']) {
    $entity = entity_get_info('node');
    $mapping = $entity['bundles'][$bundle]['rdf_mapping'];
  }

  $form['rdf_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('RDF Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings'
  );
  $form['rdf_settings']['rdf_rdftype'] = array(
    '#type' => 'fieldset',
    '#title' => t('RDF Type'),
  );
  $form['rdf_settings']['rdf_title'] = array(
    '#type' => 'fieldset',
    '#title' => t('RDF Title Predicate'),
  );

  // Add the RDF type field for the bundle.
  osf_fieldstorage_rdftype_fieldset($form['rdf_settings']['rdf_rdftype'], $mapping);
  
  //Add the predicate, datatype, property fields for the bundle title.
  osf_fieldstorage_predicate_fieldset($form['rdf_settings']['rdf_title'], $mapping, 'title', 'title');

  // add submit and validate handlers
  $form['#validate'] = array_merge($form['#validate'], array('osf_fieldstorage_form_node_type_form_validate'));
  $form['#submit'] = array_merge($form['#submit'], array('osf_fieldstorage_form_node_type_form_submit'));
}

/*
function osf_fieldstorage_form_field_ui_field_edit_form_validate($form, &$form_state) {
  $field_name = $form['#field']['field_name'];

  //_osf_fieldstorage_validate_terms($form_state, $field_name);
  //_osf_fieldstorage_validate_datatype($form_state, $field_name);
}
*/
/*
function osf_fieldstorage_admin_rdf_overview_form_validate($form, &$form_state) {
  // Validate bundle's RDF type(s).
  _osf_fieldstorage_validate_terms($form_state);

  // Validate title predicate(s).
  if ((isset($form_state['input']['rdf_title_type']))) {
    $field_name = 'title';
    _osf_fieldstorage_validate_terms($form_state, $field_name);
    //_osf_fieldstorage_validate_datatype($form_state, $field_name);
  }

  // Validate predicates for all fields.
  foreach ($form_state['values']['field_names'] as $field_name) {
    _osf_fieldstorage_validate_terms($form_state, $field_name);
    //_osf_fieldstorage_validate_datatype($form_state, $field_name);
  }
}
*/
/*
function osf_fieldstorage_form_node_type_form_validate($form, &$form_state) {
  // Validate bundle's RDF type(s).
  _osf_fieldstorage_validate_terms($form_state);

  // Validate title predicate(s).
  if ((isset($form_state['input']['rdf_title_type']))) {
    $field_name = 'title';
    _osf_fieldstorage_validate_terms($form_state, $field_name);
    //_osf_fieldstorage_validate_datatype($form_state, $field_name);
  }
}
*/

/**
 * Saves RDF mapping for all fields and node type.
 */
function osf_fieldstorage_admin_rdf_overview_form_submit($form, &$form_state) {
  $entity_type = $form_state['build_info']['args'][0];
  // @todo somehow $form_state['build_info']['args'][1] is an object for the
  // node RDF mapping but a string 'user' for the user RDF mapping.
  if (isset($form_state['build_info']['args'][1]->type)) {
    $bundle = $form_state['build_info']['args'][1]->type;
  }
  elseif ($form_state['build_info']['args'][0] == 'taxonomy_term') {
    $bundle = $form_state['build_info']['args'][1]->machine_name;
  }
  else {
    $bundle = $form_state['build_info']['args'][1];
  }
  _osf_fieldstorage_mapping_save($form_state, $entity_type, $bundle, 'title');

  foreach ($form_state['values']['field_names'] as $field_name) {
    _osf_fieldstorage_mapping_save($form_state, $entity_type, $bundle, $field_name);
  }
  drupal_set_message(t('RDF mappings have been saved.'), 'status');
}

/**
 * Saves RDF mapping for individual field.
 */
function osf_fieldstorage_form_field_ui_field_edit_form_submit($form, &$form_state) {
  $entity_type = $form['instance']['entity_type']['#value'];
  $bundle = $form['instance']['bundle']['#value'];
  $field_name = $form['#field']['field_name'];

  _osf_fieldstorage_mapping_save($form_state, $entity_type, $bundle, $field_name);
}

/**
 * Saves RDF mapping for Title field.
 */
function osf_fieldstorage_form_node_type_form_submit($form, &$form_state) {
  $entity_type = 'node';
  $bundle = $form_state['input']['type'];
  // We only need to call _osf_fieldstorage_mapping_save once because it checks whether
  // rdf_type is set in the form before saving the field value.
  _osf_fieldstorage_mapping_save($form_state, $entity_type, $bundle, 'title');
}

/**
 * Menu callback for classes autocomplete
 */
 
function osf_fieldstorage_classes_autocomplete($query) {
  $networks = osf_configure_get_endpoints(NULL, FALSE, TRUE);

  $suggestions = array();
  
  $ontologies = variable_get('osf_osf_entities_ontologies_selected', array());
  
  $ontologiesFilters = array();
  
  foreach($ontologies as $ontology)
  {
    if($ontology != NULL)
    {
      array_push($ontologiesFilters, $ontology);
    }
  }

  foreach($networks as $network)
  {
    $defaultEndpoint = osf_configure_get_endpoint_by_uri($network->uri);
    
    $search = new SearchQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
    
    if($query == '')
    {
      $search->excludeAggregates()
             ->sort('prefLabel', 'asc')
             ->items(15)
             ->attributeValuesFilters(Namespaces::$iron.'prefLabel', '*');
    }
    else
    {
      $search->excludeAggregates()
             ->sort('prefLabel', 'asc')
             ->items(15)
             ->attributeValuesFilters(Namespaces::$iron.'prefLabel', '*'.$query.'**');
    } 
    
    $search->datasetsFilters($ontologiesFilters)
           ->typeFilter(Namespaces::$owl.'Class')
           ->sourceInterface(variable_get("osf_searchapi_settings_interface_name", ''))
           ->send(new DrupalQuerierExtension());
    
    if(!$search->isSuccessful())
    {
      watchdog('osf_entities', 
               'Can\'t get the list of things to autocomplete: [@error] @errorMsg - @errorDescription.', 
               array("@error" => $search->getStatus(),                 
                     "@errorMsg" => $search->getStatusMessage(),
                     "@errorDescription" => $search->getStatusMessageDescription()));                       
                         
    }
    else
    {           
      $resultset = $search->getResultset();
     
      $namespaces = new Namespaces();
     
      if(isset($resultset))
      {
        foreach($resultset->getResultset() as $dataset => $results)
        {
          foreach($results as $uri => $result)
          {
            $suggestions[$result['prefLabel'] . '   ('.$namespaces->getPrefixedUri($uri).')'] = '<b>'.$result['prefLabel'] . '</b>   <em>('.$namespaces->getPrefixedUri($uri).')</em>';
          }
        }
      }
      
      // We we got more than 10 results, we stop querying for more networks
      if(count($suggestions) > 10)
      {
        break;
      } 
    }   
  }

  drupal_json_output($suggestions);
}  

/**
 * Menu callback for datatype autocomplete
 *//*
function osf_fieldstorage_datatype_autocomplete($string) {

  //@todo Make sure datatype is only active if property is the relationship.

  $datatypes = 'xsd:string, xsd:boolean, xsd:decimal, xsd:float, xsd:double, xsd:dateTime, xsd:time, xsd:date, xsd:gYearMonth, xsd:gYear, xsd:gMonthDay, xsd:gDay, xsd:gMonth, xsd:hexBinary, xsd:base64Binary, xsd:anyURI, xsd:normalizedString, xsd:token, xsd:language, xsd:NMTOKEN, xsd:Name, xsd:NCName, xsd:integer, xsd:nonPositiveInteger, xsd:negativeInteger, xsd:long, xsd:int, xsd:short, xsd:byte, xsd:nonNegativeInteger, xsd:unsignedLong, xsd:unsignedInt, xsd:unsignedShort, xsd:unsignedByte, xsd:positiveInteger';

  $datatypes = explode(', ', $datatypes);
  $matches = array();

  foreach($datatypes as $datatype) {
    if (preg_match("/^$string/", $datatype))
      $matches[$datatype] = $datatype;
  }

  drupal_json_output($matches);
} */

/**
 * Menu callback for predicates autocomplete
 */
function osf_fieldstorage_literal_predicates_autocomplete($query) {
  $networks = osf_configure_get_endpoints(NULL, FALSE, TRUE);

  $suggestions = array();
  
  $ontologies = variable_get('osf_osf_entities_ontologies_selected', array());
  
  $ontologiesFilters = array();
  
  foreach($ontologies as $ontology)
  {
    if($ontology != NULL)
    {
      array_push($ontologiesFilters, $ontology);
    }
  }

  foreach($networks as $network)
  {
    $defaultEndpoint = osf_configure_get_endpoint_by_uri($network->uri);
    
    $search = new SearchQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
    
    if($query == '')
    {
      $search->excludeAggregates()
             ->items(15)
             ->sort('prefLabel', 'asc')
             ->attributeValuesFilters(Namespaces::$iron.'prefLabel', '*');
    }
    else
    {
      $search->excludeAggregates()
             ->items(15)
             ->sort('prefLabel', 'asc')
             ->attributeValuesFilters(Namespaces::$iron.'prefLabel', '*'.$query.'**');
    } 
    
    $search->datasetsFilters($ontologiesFilters)
           ->typeFilter(Namespaces::$owl.'DatatypeProperty')
           ->typeFilter(Namespaces::$owl.'AnnotationProperty')
           ->sourceInterface(variable_get("osf_searchapi_settings_interface_name", ''))
           ->send(new DrupalQuerierExtension());

    if(!$search->isSuccessful())
    {
      watchdog('osf_entities', 
               'Can\'t get the list of things to autocomplete: [@error] @errorMsg - @errorDescription.', 
               array("@error" => $search->getStatus(),                 
                     "@errorMsg" => $search->getStatusMessage(),
                     "@errorDescription" => $search->getStatusMessageDescription()));                       
                         
    }
    else
    {           
      $resultset = $search->getResultset();
     
      $namespaces = new Namespaces();
      
      if(isset($resultset))
      {
        foreach($resultset->getResultset() as $dataset => $results)
        {  
          foreach($results as $uri => $result)
          {
            $valueType = 'text';
            
            switch($result['type'][0])
            {
              case Namespaces::$owl.'DatatypeProperty':
                $valueType = 'text';
              break;
              case Namespaces::$owl.'AnnotationProperty':
                $valueType = 'text';
              break;
              case Namespaces::$owl.'ObjectProperty':
                $valueType = 'relation';
              break;
            }            
            
            $suggestions[$result['prefLabel'] /*. ' ['.$valueType.'] '*/ .' ('.$namespaces->getPrefixedUri($uri).')'] = '<b>'.$result['prefLabel'] . '</b> '. /*'<b>['.$valueType.']</b>'.*/ ' <em>('.$namespaces->getPrefixedUri($uri).')</em>';
          }
        }
      }
      
      // We we got more than 10 results, we stop querying for more networks
      if(count($suggestions) > 10)
      {
        break;
      } 
    }   
  }

  drupal_json_output($suggestions);
}

/**
 * Menu callback for predicates autocomplete
 */
function osf_fieldstorage_relation_predicates_autocomplete($query) {
  $networks = osf_configure_get_endpoints(NULL, FALSE, TRUE);

  $suggestions = array();
  
  $ontologies = variable_get('osf_osf_entities_ontologies_selected', array());
  
  $ontologiesFilters = array();
  
  foreach($ontologies as $ontology)
  {
    if($ontology != NULL)
    {
      array_push($ontologiesFilters, $ontology);
    }
  }

  foreach($networks as $network)
  {
    $defaultEndpoint = osf_configure_get_endpoint_by_uri($network->uri);
    
    $search = new SearchQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
    
    if($query == '')
    {
      $search->excludeAggregates()
             ->items(15)
             ->sort('prefLabel', 'asc')
             ->attributeValuesFilters(Namespaces::$iron.'prefLabel', '*');
    }
    else
    {
      $search->excludeAggregates()
             ->items(15)
             ->sort('prefLabel', 'asc')
             ->attributeValuesFilters(Namespaces::$iron.'prefLabel', '*'.$query.'**');
    } 
    
    $search->datasetsFilters($ontologiesFilters)
           ->typeFilter(Namespaces::$owl.'ObjectProperty')
           ->sourceInterface(variable_get("osf_searchapi_settings_interface_name", ''))
           ->send(new DrupalQuerierExtension());

    if(!$search->isSuccessful())
    {
      watchdog('osf_entities', 
               'Can\'t get the list of things to autocomplete: [@error] @errorMsg - @errorDescription.', 
               array("@error" => $search->getStatus(),                 
                     "@errorMsg" => $search->getStatusMessage(),
                     "@errorDescription" => $search->getStatusMessageDescription()));                       
                         
    }
    else
    {           
      $resultset = $search->getResultset();
     
      $namespaces = new Namespaces();
      
      if(isset($resultset))
      {
        foreach($resultset->getResultset() as $dataset => $results)
        {  
          foreach($results as $uri => $result)
          {
            $valueType = 'text';
            
            switch($result['type'][0])
            {
              case Namespaces::$owl.'DatatypeProperty':
                $valueType = 'text';
              break;
              case Namespaces::$owl.'AnnotationProperty':
                $valueType = 'text';
              break;
              case Namespaces::$owl.'ObjectProperty':
                $valueType = 'relation';
              break;
            }            
            
            $suggestions[$result['prefLabel'] /*. ' ['.$valueType.'] '*/ .' ('.$namespaces->getPrefixedUri($uri).')'] = '<b>'.$result['prefLabel'] . '</b> '. /*'<b>['.$valueType.']</b>'.*/ ' <em>('.$namespaces->getPrefixedUri($uri).')</em>';
          }
        }
      }
      
      // We we got more than 10 results, we stop querying for more networks
      if(count($suggestions) > 10)
      {
        break;
      } 
    }   
  }

  drupal_json_output($suggestions);
}


/**
 * appends the rdf form fields to a fieldset dedicated to a fields.module field
 */
function osf_fieldstorage_predicate_fieldset(&$fieldset, $mapping, $field_name, $label) {
  $fieldInfo = field_info_field($field_name);
  $fieldInstance = field_read_instance('node', $field_name, $fieldInfo['bundles']['node'][0]);
    
  // Check if the field type and/or the field widget that is used is the kind of field
  // that reference another entity. If it is the case, then we make sure that the
  // RDF Mapping user interface only shows owl:ObjectProperty to the user. Otherwise
  // we display owl:DatatypeProperty and owl:AnnotationProperty to the users.
  if($fieldInfo['type'] == 'entityreference' ||
     $fieldInfo['type'] == 'node_reference' ||
     ($fieldInfo['type'] == 'text' && $fieldInstance['widget']['type'] == 'osf_field_concept_reference') ||
     ($fieldInfo['type'] == 'text' && $fieldInstance['widget']['type'] == 'osf_field_entity_reference'))
  {
    $fieldset['rdf_'. $field_name .'_predicates'] = array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'osf_fieldstorage/relation_predicates/autocomplete',
      '#title' => t('RDF Predicates'),
      '#default_value' => (empty($mapping[$field_name]['osf_fieldstorage_ui']) ? '' : $mapping[$field_name]['osf_fieldstorage_ui']),
      '#description' => t('Select the properties to use for mapping this field to in RDF. The list of properties can be managed by the <a target="_blank" href="/osf/ontology">OSF Ontology module</a>. Create the new Datatype properties that is required for the mapping. If you don\'t have access to this tool, ask your system administrator to add it for you.',
        array(
          '%label' => $label,
          '%predicates' => 'foaf:familyName, foaf:lastName'
        )
      ),
      '#resizable' => FALSE,
    );
  }
  else            
  {
    $fieldset['rdf_'. $field_name .'_predicates'] = array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'osf_fieldstorage/literal_predicates/autocomplete',
      '#title' => t('RDF Predicates'),
      '#default_value' => (empty($mapping[$field_name]['osf_fieldstorage_ui']) ? '' : $mapping[$field_name]['osf_fieldstorage_ui']),
      '#description' => t('Select the properties to use for mapping this field to in RDF. The list of properties can be managed by the <a target="_blank" href="/osf/ontology">OSF Ontology module</a>. Create the new Datatype properties that is required for the mapping. If you don\'t have access to this tool, ask your system administrator to add it for you.',
        array(
          '%label' => $label,
          '%predicates' => 'foaf:familyName, foaf:lastName'
        )
      ),
      '#resizable' => FALSE,
    );
  }
}

/**
 * Appends the RDF type fieldset. Because this fieldset is always on the same
 * page as fields created by osf_fieldstorage_predicate_fieldset(), which add the css and
 * js files, we do not add the files again.
 *
 */
function osf_fieldstorage_rdftype_fieldset(&$fieldset, $mapping) {
  $fieldset = array(
    '#type' => 'textfield',
    '#autocomplete_path' => 'osf_fieldstorage/classes/autocomplete',
    '#title' => t('RDF Type'),
    '#default_value' => (empty($mapping['rdftype']['osf_fieldstorage_ui']) ? '' : $mapping['rdftype']['osf_fieldstorage_ui']),
    '#description' => t('Select the type to use for mapping this bundle to in RDF. The list of types can be managed by the <a href="/osf/entities/ontologies/sync/classes">OSF Entities synchronization process</a>. The possible types exposed are constrained to the <a href="/osf/entities/ontologies/select">list of selected ontology configured here</a>.'),
  );
}

/**
 * Sets a form error if predicates don't validate.
 */
/*
function _osf_fieldstorage_validate_terms($form_state, $field_name = '') {
}
*/

/**
 * Sets a form error if datatype don't validate.
 */
/* 
function _osf_fieldstorage_validate_datatype($form_state, $field_name) {
}
*/

function osf_fieldstorage_extract_uri($value, $prefixed = TRUE)
{
  $namespaces = new Namespaces();

  $uri = substr($value, strpos($value, '(') + 1, strpos($value, ')') - strpos($value, '(') - 1);
    
  if($prefixed === TRUE)  
  {
    return $uri;
  }
  else
  {
    return($namespaces->getUnprefixedUri($uri));
  }
}

function osf_fieldstorage_extract_property_mapping_type($value)
{
  $namespaces = new Namespaces();

  $mapping_type = substr($value, strpos($value, '[') + 1, strpos($value, ']') - strpos($value, '[') - 1);
  
  return($mapping_type);
}

/**
 * Saves the mapping
 */
function _osf_fieldstorage_mapping_save($form_state, $entity_type, $bundle, $field_name) {
  $prevMapping = rdf_mapping_load($entity_type, $bundle);
  $mapping = rdf_mapping_load($entity_type, $bundle);
  
  // Set the RDF type if it is in this form.
  if (!empty($form_state['values']['rdf_rdftype'])) {
    
    $typeClass = osf_fieldstorage_extract_uri($form_state['values']['rdf_rdftype']);
    
    // Check if the type mapping changed and record that change in the osf_fieldstorage_pending_opts_fields table
    if(isset($prevMapping['rdftype']) && $prevMapping['rdftype'][0] != $typeClass)
    {
      db_insert('osf_fieldstorage_pending_opts_bundles')
                ->fields(array(
                  'bundle' => $bundle,
                  'rdfmapping' => $typeClass,
                  'prev_rdfmapping' => $prevMapping['rdftype'][0],
                  'operation' => 'changed',
                  'date' => REQUEST_TIME,
                  'executed' => 0
                ))
                ->execute();            
    }    
    
    $mapping['rdftype'] = array($typeClass);
    $mapping['rdftype']['osf_fieldstorage_ui'] = $form_state['values']['rdf_rdftype'];     
  }  
  
  foreach(array('type', 'datatype', 'predicates') as $key) 
  {
    // If there is form input for this key, set it in the mapping.
    if(!empty($form_state['values']['rdf_'. $field_name .'_' . $key])) 
    {
      $mappedPropertyUIString = $form_state['values']['rdf_'. $field_name .'_' . $key];
      $mappedProperty = osf_fieldstorage_extract_uri($mappedPropertyUIString);
      
      $mapping[$field_name][$key] = array($mappedProperty);
      
      if(osf_fieldstorage_extract_property_mapping_type($mappedPropertyUIString) == 'relation')
      {
        $mapping[$field_name]['type'] = 'rel';
      } 
      
      $mapping[$field_name]['osf_fieldstorage_ui'] = $mappedPropertyUIString;     
      
      // Check if the property mapping changed and record that change in the osf_fieldstorage_pending_opts_bundles table
      if(isset($prevMapping['rdftype']) && 
         isset($prevMapping[$field_name]) && 
         $prevMapping[$field_name]['predicates'][0] != $mappedProperty)
      {
        db_insert('osf_fieldstorage_pending_opts_fields')
                  ->fields(array(
                    'bundle' => $bundle,
                    'field' => $field_name,
                    'rdfmapping' => $mappedProperty,
                    'prev_rdfmapping' => $prevMapping[$field_name]['predicates'][0],
                    'operation' => 'changed',
                    'date' => REQUEST_TIME,
                    'executed' => 0
                  ))
                  ->execute();            
      }
    }
    // If there is no form input for this key, we need to remove it from the
    // mapping.
    else 
    {
      // If there are no predicates, there should be no mapping at all for the
      // field.
      if ($key == 'predicates') {
        unset($mapping[$field_name]);
      }
      else {
        unset($mapping[$field_name][$key]);
      }
    }
  }  

  $mapping_info = array(
    'mapping' => $mapping,
    'type' => $entity_type,
    'bundle' => $bundle
  );

  rdf_mapping_save($mapping_info);  
}   

function osf_fieldstorage_admin_sync_form($form, &$form_state, $entity_type, $bundle, $instance) 
{
  $form['fields_fieldstorage_synchronize'] = array(
    '#type' => 'fieldset',
    '#title' => t('Synchronize Content Type'),
    '#description' => t('This section let you synchronize all the instances of this content type into OSF.
                         Once you clicked on the <b>synchronize</b> button bellow, a batch process will
                         start to index all the content into the default OSF instance configured on this
                         Drupal instance.
                         
                         <br /><br />'),
    '#weight' => 4,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );  
  
  $form['fields_fieldstorage_synchronize']['osf_fieldstorage_synchronize_content_type'] = array(
    '#type' => 'submit', 
    '#value' => t('Synchronize Content Type'),
  );  
  
  $form['#submit'][] = 'osf_fieldstorage_admin_sync_form_submit';
  
  $form['bundle'] = array(
    '#type' => 'hidden',
    '#value' => $bundle,
  );    
  
  return system_settings_form($form);
}  
  
function osf_fieldstorage_admin_sync_form_submit(&$form, &$form_state) 
{
  // The first thing we have to check here, is to make sure that there is no pending 
  // RDF mapping modifications. If there are, then we have to stop this synchronization
  // process and tell the user that he has to first run Cron, and then to re-synchronize.
  // Otherwise, information may, and probably will, be lost in the process.
  
  $operations = db_query('SELECT id FROM {osf_fieldstorage_pending_opts_fields} 
                          WHERE executed = :executed', array('executed' => 0))->fetchAssoc();

  if(!empty($operations))
  {
    drupal_set_message(t("The RDF Mapping of this content type did change and the related
                          modifications that needs to be applied in OSF haven't yet been
                          run by Cron. Before being able to run this synchronization
                          operation, you will first have to <a href=\"/admin/config/system/cron\">run Cron manually</a>
                          to apply the changes on OSF and then to retry synchronizing this
                          content type."), 'warning', TRUE);
    
    return;
  }
  
  $operations = db_query('SELECT id FROM {osf_fieldstorage_pending_opts_bundles} 
                          WHERE executed = :executed', array('executed' => 0))->fetchAssoc();

  if(!empty($operations))
  {
    drupal_set_message(t("The RDF Mapping of this content type did change and the related
                          modifications that needs to be applied in OSF haven't yet been
                          run by Cron. Before being able to run this synchronization
                          operation, you will first have to <a href=\"/admin/config/system/cron\">run Cron manually</a>
                          to apply the changes on OSF and then to retry synchronizing this
                          content type."), 'warning', TRUE);
                          
    return;
  }
  
  
  $batch = array('title' => t('Synchronizing nodes in OSF...'),
                 'operations' => array(
                   array('osf_fieldstorage_batch_synchronize', array($form['bundle']['#value']->type)),
                 ),
                 'progress_message' => t(''),
                 'finished' => 'osf_fieldstorage_batch_synchronize_finished',
                 'file' => drupal_get_path('module', 'osf_fieldstorage').'/osf_fieldstorage.admin.inc');
                 
  batch_set($batch);
}

function osf_fieldstorage_batch_synchronize($bundle, &$context) 
{
  if(empty($context['sandbox'])) 
  {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = db_query('SELECT COUNT(DISTINCT nid) FROM {node} WHERE type = :bundle', array(':bundle' => $bundle))->fetchField();
  }
  
  $limit = 1;
  
  $result = db_select('node')->fields('node', array('nid'))
                             ->condition('nid', $context['sandbox']['current_node'], '>')
                             ->condition('type', $bundle, '=')
                             ->orderBy('nid')
                             ->range(0, $limit)
                             ->execute();
    
  foreach($result as $row) 
  {
    // Load the node to save
    $node = node_load($row->nid, NULL, TRUE);
    
    // Save the node such that the OSF FieldStorage field get called and that the node
    // get saved into the OSF instance
    node_save($node);
    
    $context['results'][] = $node->nid;
    $context['sandbox']['progress']++;
    $context['sandbox']['current_node'] = $node->nid;
    $context['message'] = '<span style="padding-left: 3px;" />('.$context['sandbox']['progress'].'/'.$context['sandbox']['max'].')<span style="padding-right: 30px;" />'.check_plain($node->title);
  }  
  
  if($context['sandbox']['progress'] != $context['sandbox']['max']) 
  {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}  

function osf_fieldstorage_batch_synchronize_finished($success, $results, $operations) 
{
  // The 'success' parameter means no fatal PHP errors were detected. All
  // other error management should be handled using 'results'.
  if($success) 
  {
    $message = format_plural(count($results), 'One node processed.', '@count nodes processed.');
  }
  else 
  {
    $message = t('Finished with an error.');
  }
  
  drupal_set_message($message);
  
  // Providing data for the redirected page is done through $_SESSION.
  foreach ($results as $result) 
  {
    $items[] = t('Node synchronized: %title.', array('%title' => $result));
  }
  
  $_SESSION['osf_fieldstorage_batch_results'] = $items;
}
  
?>
