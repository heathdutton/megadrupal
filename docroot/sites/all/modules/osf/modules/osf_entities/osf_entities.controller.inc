<?php

  use \StructuredDynamics\osf\framework\Namespaces;
  use \StructuredDynamics\osf\framework\Resultset;
  use \StructuredDynamics\osf\framework\Subject;
  use \StructuredDynamics\osf\php\api\ws\crud\read\CrudReadQuery;
  use \StructuredDynamics\osf\php\api\ws\crud\create\CrudCreateQuery;
  use \StructuredDynamics\osf\php\api\ws\crud\update\CrudUpdateQuery;
  use \StructuredDynamics\osf\php\api\ws\crud\delete\CrudDeleteQuery;
  use \StructuredDynamics\osf\php\api\ws\ontology\delete\DeleteClassFunction;
  use \StructuredDynamics\osf\php\api\ws\ontology\delete\DeleteNamedIndividualFunction;
  use \StructuredDynamics\osf\php\api\ws\ontology\delete\DeletePropertyFunction;
  use \StructuredDynamics\osf\php\api\ws\ontology\delete\OntologyDeleteQuery;
  use \StructuredDynamics\osf\php\api\ws\ontology\update\OntologyUpdateQuery;
  use \StructuredDynamics\osf\php\api\ws\ontology\update\CreateOrUpdateEntityFunction;
  use \StructuredDynamics\osf\php\api\ws\revision\read\RevisionReadQuery;

  class Resource extends Entity
  {
    protected function defaultLabel()
    {
      return $this->title;
    }

    protected function defaultUri()
    {
      return array('path' => 'resource/' . $this->identifier());
    }
  }

  class ResourceController extends EntityAPIController
  {
    public function create(array $values = array())
    {
      global $user;

      $values += array(
        'title' => '',
        'description' => '',
        'created' => REQUEST_TIME,
        'changed' => REQUEST_TIME,
        'uid' => $user->uid,
      );

      return parent::create($values);
    }

    public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array())
    {
      $wrapper = entity_metadata_wrapper('resource', $entity);
      $content['author'] = array('#markup' => t('Created by: !author', array('!author' => $wrapper->uid->name->value(array('sanitize' => TRUE)))));

      // Make Description and Status themed like default fields.
      $content['description'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' =>t('Description'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_description',
        '#field_type' => 'text',
        '#entity_type' => 'resource',
        '#bundle' => $entity->type,
        '#items' => array(array('value' => $entity->description)),
        '#formatter' => 'text_default',
        0 => array('#markup' => check_plain($entity->description))
      );

      return parent::buildContent($entity, $view_mode, $langcode, $content);
    }
  }


  class ResourceType extends Entity
  {
    public $type;
    public $label;
    public $weight = 0;

    public function __construct($values = array())
    {
      parent::__construct($values, 'resource_type');
    }

    function isLocked()
    {
      return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
    }
  }

  /**
  * Implements EntityAPIControllerExportable
  *
  * @see http://drupalcontrib.org/api/drupal/contributions!entity!includes!entity.controller.inc/class/EntityAPIControllerExportable/7
  */
  class ResourceTypeController extends EntityAPIControllerExportable
  {
    public function resetCache(array $ids = NULL)
    {
      // Reset the persistent cache.
      if(is_array($ids)) {
        foreach($ids as $id)
        {
          cache_clear_all($id, 'cache_entity_resource_type');
        }
      }

      @parent::resetCache($ids);
    }

    /**
    * Implements create()
    *
    * @see http://drupalcontrib.org/api/drupal/contributions!entity!includes!entity.controller.inc/function/EntityAPIController%3A%3Acreate/7
    */
    public function create(array $values = array())
    {
      // Add default values in that array
      $values += array();
      
      $entity = parent::create($values);

      // Add RDF Mapping
      $fieldsByBundles = osf_entities_get_fields_by_bundles();
      $classesByBundles = variable_get("osf_entities_classes_by_bundles", NULL);
      $propertiesByFields = variable_get("osf_entities_properties_by_field", NULL);
      $properties = variable_get("osf_entities_properties", NULL);

      $mapping = array();

      $namespaces = new Namespaces();
      
      if(is_array($entity->type))
      {
        foreach($entity->type as $type)
        {
          if(!isset($classesByBundles[$type]) || !isset($fieldsByBundles[$type]))
          {
            continue;
          }

          $mapping['rdftype'] = $namespaces->getPrefixedUri($classesByBundles[$type]);

          foreach($fieldsByBundles[$type] as $field)
          {
            $isObjectProperty = FALSE;

            if(isset($propertiesByFields[$field]) && isset($properties[$propertiesByFields[$field]]))
            {
              $property = unserialize($properties[$propertiesByFields[$field]]);

              if($property)
              {
                foreach($property->getTypes() as $type)
                {
                  if($type == Namespaces::$owl."ObjectProperty")
                  {
                    $isObjectProperty = TRUE;
                    break;
                  }
                }
              }
            }

            if($isObjectProperty)
            {
              $mapping[$field] = array(
                'predicates' => array($namespaces->getPrefixedUri($propertiesByFields[$field])),
                'type' => "rel"

              );
            }
            else
            {
              $mapping[$field] = array(
                'predicates' => array($namespaces->getPrefixedUri($propertiesByFields[$field]))
              );
            }
          }
        }
      }

      $entity->{'rdf_mapping'} = $mapping;

      // Normalize the dataset's provenance using the 'dataset' entity property
      
      // If 'obj' doesn't exists in $entity, it means the user is creating a new entity.
      if(property_exists($entity, 'obj'))
      {
        $values = $entity->obj->getDataPropertyValues(Namespaces::$dcterms.'isPartOf');
        
        if(is_array($values) &&
           isset($values[0]['uri']))
        {
          $entity->{'dataset'} = $values[0]['uri'];                                        
        }
      }

      return $entity;
    }

    /**
     * @TODO implement save() to save an entity in OSF
     */
    public function save($entity, DatabaseTransaction $transaction = NULL){
      // For each ID, check if they exists. If they are, update them in OSF, otherwise
      // create them into OSF.
      if(isset($entity->module) && $entity->module == "osf_entities")
      {
        // If entity_save() is called by osf_entities, then it means that we
        // struct entities is synchronizing the ontologies and that it is currently
        // creating the "resource_type" bundles. In that case, we call the parent
        // save() method to save everything into Drupal
        parent::save($entity, $transaction);

        return;
      }
      
      // If we are trying to save a resource that has been defined in an ontology dataset,
      // en we have to handle the saving of that entity using the OntologyCreate and 
      // OntologyUpdate endpoints.
      if(osf_ontology_is_ontology_dataset($entity->dataset))
      {
        $defaultEndpoint = current(osf_configure_get_default_endpoint());
        
        $ontologyUpdate = new OntologyUpdateQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());

        $cuEntity = new CreateOrUpdateEntityFunction();
        
        $rdfDocument = osf_entities_get_rdf_entity($entity, "application/rdf+xml");
        
        if(empty($rdfDocument))
        {
          return(FALSE);
        }          

        $cuEntity->enableAdvancedIndexation()
                 ->document($rdfDocument);
        
        $ontologyUpdate->ontology($entity->dataset)
                       ->enableReasoner()
                       ->createOrUpdateEntity($cuEntity)
                       ->send(new DrupalQuerierExtension());

        if(!$ontologyUpdate->isSuccessful())
        {
          drupal_set_message(t("Can't save this ontology entity into this dataset: [@error] @errorMsg - @errorDescription",
                             array("@error" => $ontologyUpdate->getStatus(),
                                   "@errorMsg" => $ontologyUpdate->getStatusMessage(),
                                   "@errorDescription" => $ontologyUpdate->getStatusMessageDescription())),
                             "error",
                             TRUE);

          watchdog('osf_entities', 
                   'Can\'t save this ontology entity into this dataset: [@error] @errorMsg - @errorDescription.', 
                   array("@error" => $ontologyUpdate->getStatus(),
                         "@errorMsg" => $ontologyUpdate->getStatusMessage(),
                         "@errorDescription" => $ontologyUpdate->getStatusMessageDescription()));                       
                             
        }
        else
        {
          // We have to clear the entity cache first if we want the users
          // to see the update's changes in the view page where they will
          // get redirected.
          $this->resetCache(array($entity->uri));             
          
          return(SAVED_NEW);
        } 
      }
      else
      { 
        // Check if the entity is already existing. If it is then we use CRUD: Update,
        // otherwise we use CRUD: Create.
        
        $moderation = 'revision_no_moderation';
        
        if(property_exists($entity, 'moderation'))
        {
          $moderation = $entity->moderation;
          unset($entity->moderation);
        }
        
        $defaultEndpoint = osf_configure_get_endpoint_by_uri(osf_entities_get_network_from_dataset_uri($entity->dataset));
        
        $checkExistingEntity = entity_load('resource_type', array($entity->uri.'::'.$entity->dataset));
        
        if(empty($checkExistingEntity))
        {
          $crudCreate = new CrudCreateQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());

          $rdfDocument = osf_entities_get_rdf_entity($entity, "application/rdf+n3");

          if(empty($rdfDocument))
          {
            return(FALSE);
          }

          $crudCreate->dataset($entity->dataset)
                     ->documentMimeIsRdfN3()
                     ->document($rdfDocument)
                     ->enableFullIndexationMode()
                     ->send(new DrupalQuerierExtension());

          if(!$crudCreate->isSuccessful())
          {
            if($crudCreate->error->id === 'WS-CRUD-CREATE-312')
            {
              drupal_goto('admin/resource/republish/'.urlencode($entity->uri).'/'.urlencode($entity->dataset));
            }
            else
            {
              drupal_set_message(t("Can't save this entity into this dataset: [@error] @errorMsg - @errorDescription",
                                 array("@error" => $crudCreate->getStatus(),
                                       "@errorMsg" => $crudCreate->getStatusMessage(),
                                       "@errorDescription" => $crudCreate->getStatusMessageDescription())),
                                 "error",
                                 TRUE);
                                         
              watchdog('osf_entities', 
                       'Can\'t save this entity into this dataset: [@error] @errorMsg - @errorDescription.', 
                       array("@error" => $crudCreate->getStatus(),
                             "@errorMsg" => $crudCreate->getStatusMessage(),
                             "@errorDescription" => $crudCreate->getStatusMessageDescription()));                       
            }
          }
          else
          {
            // We have to clear the entity cache first if we want the users
            // to see the update's changes in the view page where they will
            // get redirected.
            $this->resetCache(array($entity->uri));               
            
            return(SAVED_NEW);
          }
        }
        else
        {
          $crudUpdate = new CrudUpdateQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());

          $rdfDocument = osf_entities_get_rdf_entity($entity, "application/rdf+n3");

          if(empty($rdfDocument))
          {
            return(FALSE);
          }

          $crudUpdate->dataset($entity->dataset)
                     ->documentMimeIsRdfN3()
                     ->document($rdfDocument);
                     
          switch($moderation)
          {
            case 'no_revision_no_moderation':     
              $crudUpdate->ignoreRevision();
            break;
            case 'revision_no_moderation':
              $crudUpdate->createRevision()
                         ->isPublished();
            break;
            case 'revision_moderation':
              $crudUpdate->createRevision()
                         ->isStaging();
            break;
          }                     
                     
          $crudUpdate->send(new DrupalQuerierExtension());

          if(!$crudUpdate->isSuccessful())
          {
            if($crudUpdate->error->id == 'WS-CRUD-UPDATE-313')
            {
              $crudUpdate->error->description = t('You cannot create a new published revision of a resource if a 
                more recent unpublished revision exists for that resource. What you can do to overcome this 
                situation is:
                
                <ol>
                  <li>When saving, specify the option "<b>Create new revision and moderate</b>" to send the modification to the moderation queue</li>
                  <li>Publish the latest revisions that exists</li>
                  <li>Delete all the unpublished revisions that exists before the latest published revision</li>
                </ol>');
            }
            
            drupal_set_message(t("Can't save this entity into this dataset: [@error] @errorName - !errorDescription",
                               array("@error" => $crudUpdate->error->id,
                                     "@errorName" => $crudUpdate->error->name,
                                     "!errorDescription" => $crudUpdate->error->description)),
                               "error",
                               TRUE);
                               
            watchdog('osf_entities', 
                     'Can\'t save this entity into this dataset: [@error] @errorMsg - @errorDescription.', 
                     array("@error" => $crudUpdate->getStatus(),
                           "@errorMsg" => $crudUpdate->getStatusMessage(),
                           "@errorDescription" => $crudUpdate->getStatusMessageDescription()));                       
                               
          }
          else
          {
            // We have to clear the entity cache first if we want the users
            // to see the update's changes in the view page where they will
            // get redirected.
            $this->resetCache(array($entity->uri));               
            
            return(SAVED_UPDATED);
          }
        } 
      }    
    }

    /**
     * @TODO implement delete() to delete an entity from OSF
     */
    public function delete($ids, DatabaseTransaction $transaction = NULL){

      $entities = $ids ? $this->load($ids) : FALSE;

      if (!$entities) {
        // Do nothing, in case invalid or no ids have been passed.
        return;
      }

      foreach($entities as $id => $entity)
      {
        if(!isset($entity->uri) || !$entity->dataset)
        {
          // This is probably a bundle that is being deleted,
          // So we call the parent function to process the
          // deletation of this entity.
          parent::delete(array($id), $transaction);

          continue;
        }

        // Purge this entity from cache
        $this->resetCache(array($entity->uri));
        
        // Delete the entity from OSF
        if(osf_ontology_is_ontology_dataset($entity->dataset))
        {
          $defaultEndpoint = current(osf_configure_get_default_endpoint());
          
          $ontologyDelete = new OntologyDeleteQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());

          if(array_search('owl_class', $entity->type) !== FALSE)
          {
            $deleteClassFunction = new DeleteClassFunction();
            
            $deleteClassFunction->uri($entity->uri);
            
            $ontologyDelete->deleteClass($deleteClassFunction);
          }
          elseif(array_search('owl_objectproperty', $entity->type) !== FALSE || 
                 array_search('owl_datatypeproperty', $entity->type) !== FALSE ||
                 array_search('owl_annotationproperty', $entity->type) !== FALSE)
          {
            $deletePropertyFunction = new DeletePropertyFunction();
            
            $deletePropertyFunction->uri($entity->uri);
            
            $ontologyDelete->deleteProperty($deletePropertyFunction);
          }
          else
          {
            $deleteNamedIndividualFunction = new DeleteNamedIndividualFunction();
            
            $deleteNamedIndividualFunction->uri($entity->uri);
            
            $ontologyDelete->deleteNamedIndividual($deleteNamedIndividualFunction);
          }          
          
          $ontologyDelete->ontology($entity->dataset)
                         ->send(new DrupalQuerierExtension());
          
          if(!$ontologyDelete->isSuccessful())
          {
            drupal_set_message(t("Can't delete this ontology entity from this dataset: [@error] @errorMsg - @errorDescription",
                               array("@error" => $ontologyDelete->getStatus(),
                                     "@errorMsg" => $ontologyDelete->getStatusMessage(),
                                     "@errorDescription" => $ontologyDelete->getStatusMessageDescription())),
                               "error",
                               TRUE);
                               
            watchdog('osf_entities', 
                     'Can\'t delete this ontology entity from this dataset: [@error] @errorMsg - @errorDescription.', 
                     array("@error" => $ontologyDelete->getStatus(),
                           "@errorMsg" => $ontologyDelete->getStatusMessage(),
                           "@errorDescription" => $ontologyDelete->getStatusMessageDescription()));                       
          }
        }
        else
        {        
          $defaultEndpoint = osf_configure_get_endpoint_by_uri(osf_entities_get_network_from_dataset_uri($entity->dataset));

          $crudDelete = new CrudDeleteQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());

          $crudDelete->uri($entity->uri)
                     ->dataset($entity->dataset)
                     ->send(new DrupalQuerierExtension());

          if(!$crudDelete->isSuccessful())
          {
            drupal_set_message(t("Can't delete this entity from this dataset: [@error] @errorMsg - @errorDescription",
                               array("@error" => $crudDelete->getStatus(),
                                     "@errorMsg" => $crudDelete->getStatusMessage(),
                                     "@errorDescription" => $crudDelete->getStatusMessageDescription())),
                               "error",
                               TRUE);
                               
            watchdog('osf_entities', 
                     'Can\'t delete this entity from this dataset: [@error] @errorMsg - @errorDescription.', 
                     array("@error" => $crudDelete->getStatus(),
                           "@errorMsg" => $crudDelete->getStatusMessage(),
                           "@errorDescription" => $crudDelete->getStatusMessageDescription()));                       
          }
        }
        
        // Finally, delete any possible DRID for this URI
        db_query("DELETE FROM {osf_drid_registry} WHERE uri = '".str_replace("'", "\\'", $entity->uri)."'");        
      }      
    }

    /**
     * Export a resource-type entity into RDF+n3.
     *
     * This can be used as document input for a CrudCreateQuery or CrudUpdateQuery.
     * However, this shouldn't be used for entity_import. In fact, entity_import
     * is not currently implemented because there is no way to specify in which
     * dataset we want to index the entity to import
     */
    public function export($entity, $prefix = ''){
      return(osf_entities_get_rdf_entity($entity, "application/rdf+n3"));
    }

    /**
     * @TODO implement view() to view an entity from OSF.
     */
    public function view($entities, $view_mode = 'full', $langcode = NULL, $page = NULL){
      parent::view($entity, $view_mode, $langcode, $page);
    }


    /**
    * Implement load()
    *
    * @see http://api.drupal.org/api/drupal/includes!entity.inc/function/DrupalDefaultEntityController%3A%3Aload/7
    */
    public function load($ids = array(), $conditions = array())
    { 
      if(!$ids || !is_array($ids) || count($ids) == 0)
      {
        if(!$ids)
        {
          return parent::load($ids, $conditions);
        }
        else
        {
          // Here we return an empty array if there is no IDs specified
          // for this request. We do not support "load all entities" via
          // entity_load since there would be too many entities to return.
          return(array());
        }
      }

      // Check if the ID(s) is a valid URI. If it is not, we try to load
      // it using the parent's load() function
      //
      // Also make sure that all the IDs are strings or integers otherwise
      // the parent's load function will return errors
      $loadParent = FALSE;
      
      foreach($ids as $key => $id)
      {
        // Make sure that the ID is a string or an integer, otherwise the parent's load() function will
        // return errors. If the $id is neither, then we remove it from the list of IDs to return.
        if(!is_string($id) && !is_int($id))
        {
          unset($ids[$key]);
        }
        
        if(!Namespaces::isValidIRI($id))
        {
          $loadParent = TRUE;
        }
      }
      
      if($loadParent)
      {
        return parent::load($ids, $conditions);
      }

      if(stripos($ids[key($ids)], '/revisions/'))
      {
        // Do not cache if we are loading a revision
        $entities = $this->_load($ids, $conditions);
      }
      else
      {
        $entities = $this->_entityCacheLoad($ids, $conditions);
      }

      if(variable_get('osf_enable_entities_load_debug_for_admins', 0) &&
         function_exists('dpm'))
      {
        foreach($entities as $uri => $entity)
        {
          dpm($entities, 'Entity: '.$uri);
        }
      }

      if(is_array($entities))
      {
        reset($entities);
      }

      return($entities);
    }

    private function _entityCacheLoad($ids = array(), $conditions = array()) {
      $entities = array();
      $cached_entities = array();
      $queried_entities = array();

      $uriIds = array();
      $uriIdsDatasets = array();

      foreach($ids as $recordDatasetUri)
      {
        if(strpos($ids[0],  "::") !== FALSE)
        {
          $uris = explode("::", $recordDatasetUri);

          $uriIds[] = $uris[0];
          $uriIdsDatasets[$uris[0]] = $uris[1];
        }
        else
        {
          $uriIds[] = $recordDatasetUri;
        }
      }

      // Create a new variable which is either a prepared version of the $ids
      // array for later comparison with the entity cache, or FALSE if no $ids
      // were passed. The $ids array is reduced as items are loaded from cache,
      // and we need to know if it's empty for this reason to avoid querying the
      // database when all requested entities are loaded from cache.
      $passed_ids = !empty($uriIds) ? array_flip($uriIds) : FALSE;
      // Try to load entities from the static cache, if the entity type supports
      // static caching.
      if ($this->cache) {
        $entities += $this->cacheGet($uriIds, $conditions);
        // If any entities were loaded, remove them from the ids still to load.
        if ($passed_ids) {
          $uriIds = array_keys(array_diff_key($passed_ids, $entities));
        }
      }

      if (!empty($this->entityInfo['entity cache']) && $uriIds && !$conditions) {

        $ces = array();
        if ($uriIds && !$conditions) {

          $cached = cache_get_multiple($uriIds, 'cache_entity_resource_type');
          if ($cached) {
            foreach ($cached as $item) {
              $ces[$item->cid] = $item->data;
            }
          }
        }

        $entities += $cached_entities = $ces;
        // If any entities were loaded, remove them from the ids still to load.
        $uriIds = array_diff($uriIds, array_keys($cached_entities));

        if ($this->cache) {
          // Add entities to the cache if we are not loading a revision.
          if (!empty($cached_entities)) {
            $this->cacheSet($cached_entities);
          }
        }
      }

      // Load any remaining entities from the database. This is the case if $ids
      // is set to FALSE (so we load all entities), if there are any ids left to
      // load, if loading a revision, or if $conditions was passed without $ids.
      if (!empty($uriIds) || ($conditions && !$passed_ids)) {
        
        if(!empty($uriIdsDatasets))
        {
          foreach($uriIds as $key => $id)
          {
            $uriIds[$key] = $id.'::'.$uriIdsDatasets[$id];
          }
        }
        
        $queried_entities = $this->_load($uriIds, $conditions);
      }

      // Pass all entities loaded from the database through $controller->attachLoad(),
      // which attaches fields (if supported by the entity type) and calls the
      // entity type specific load callback, for example hook_node_load().
      if (!empty($queried_entities)) {
        $entities += $queried_entities;
      }

      if (!empty($this->entityInfo['entity cache'])) {
        // Add entities to the entity cache if we are not loading a revision.
        if (!empty($queried_entities)) {
          foreach ($entities as $uri => $item) {            
            cache_set($uri, $item, 'cache_entity_resource_type');
          }
        }
      }

      if ($this->cache) {
        // Add entities to the cache
        if (!empty($queried_entities)) {
          $this->cacheSet($queried_entities);
        }
      }

      // Ensure that the returned array is ordered the same as the original
      // $ids array if this was passed in and remove any invalid ids.
      if ($passed_ids) {
        // Remove any invalid ids from the array.
        $passed_ids = array_intersect_key($passed_ids, $entities);

        foreach ($entities as $uri => $entity)
        {
          $passed_ids[$uri] = $entity;
        }

        $entities = $passed_ids;
      }

      return $entities;
    }

    /**
    * Implement load()
    *
    * @see http://api.drupal.org/api/drupal/includes!entity.inc/function/DrupalDefaultEntityController%3A%3Aload/7
    */
    private function _load($ids = array(), $conditions = array())
    {                 
      if(!is_array($ids) || count($ids) == 0)
      {
        // Here we return an empty array if there is no IDs specified
        // for this request. We do not support "load all entities" via
        // entity_load since there would be too many entities to return.
        return(array());
      }

      $networks = osf_configure_get_endpoints(NULL, FALSE, TRUE);

      $properties = variable_get('osf_entities_properties', array());
      $propertiesByField = variable_get('osf_entities_properties_by_field', array());
      
      // Array of found entities in the entities retrieval process.
      // Since entities from different OSF instances can be
      // added into the $ids array, we have to track them down
      // and continue the networks querying if needed, and stop if
      // not.
      $foundEntities = array();
      $results = array();

      foreach($ids as $key => $uri)
      {
        if(strpos($uri,  "::") !== FALSE)
        {
          $uri = explode("::", $uri);
          $uri = $uri[0];
        }

        $results[$uri] = array();
      }      
      
      foreach($networks as $network)
      {
        $read;
        
        // Determine if we read a revision, or other entity(ies)
        $defaultEndpoint = osf_configure_get_endpoint_by_uri($network->uri);
        
        if(isset($ids[key($ids)]) && stripos($ids[key($ids)], '/revisions/'))
        {
          $id = $ids[key($ids)];
          
          $uris = explode("::", $id);

          $revisionRecordsUri = $uris[0];
          $datasetsUri = $uris[1];          
          
          $read = new RevisionReadQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());
          
          $read->dataset($datasetsUri)
               ->getRecord()
               ->revisionUri($revisionRecordsUri)
               ->mime('resultset')
               ->send(new DrupalQuerierExtension());
        }
        else
        {
          $read = new CrudReadQuery($defaultEndpoint->uri, $defaultEndpoint->app_id, $defaultEndpoint->api_key, osf_configure_get_current_user_uri());

          $read->mime("resultset")
               ->lang('')
               ->excludeLinksback()
               ->includeReification();

          // Check if the dataset is specified for the URIs. If it is, then we create
          // two arrays, one for the records and one for the datasets. Then we feed them
          // to the endpoint.

          reset($ids);
          if(strpos($ids[key($ids)],  "::") !== FALSE)
          {
            $recordsUris = array();
            $datasetsUris = array();

            foreach($ids as $recordDatasetUri)
            {
              if(strpos($recordDatasetUri,  "::") !== FALSE)
              {
                $uris = explode("::", $recordDatasetUri);

                $recordsUris[] = $uris[0];
                $datasetsUris[] = $uris[1];
              }
              else
              {
                // Not all URIs have their dataset defined.
                // Since this would return an error in CRUD: Read,
                // we fake one and nothing will be returned for this
                // record.

                $recordsUris[] = $uris;
                $datasetsUris[] = "http://localhost/datasets/void/";
              }
            }

            $read->uri($recordsUris)
                 ->dataset($datasetsUris);
          }
          else
          {
            $read->uri($ids);
          }

          $read->send(new DrupalQuerierExtension());
        }

        if($read->isSuccessful())
        {
          $fieldsByBundles = osf_entities_get_fields_by_bundles();
          
          // Get the Resultset object instance
          $resultset = $read->getResultset()->getResultset();

          // From the search resultset, we create the array of Entity object instances
          foreach($resultset as $dataset => $subjects)
          {
            foreach($subjects as $uri => $subject)
            {
              // If it is not an array, it means that a description of that record has been found
              // on another network than the current one being queried. If it is the case
              // then we simply skip it.
              if(!is_array($results[$uri]))
              {
                continue;
              }
              
              $entityTypesUris = array();
              
              foreach($subject["type"] as $type)
              {
                if(!isset($results[$uri][resource_type_get_id(Namespaces::$rdf."type")]))
                {
                  $results[$uri][resource_type_get_id(Namespaces::$rdf."type")] = array();
                }

                array_push($results[$uri][resource_type_get_id(Namespaces::$rdf."type")], $type);
                
                // Make sure that the type of this entity is exposed as a bundle in Drupal.
                // If not, we skip it.
                $bundleEntities = entity_load('resource_type', array(resource_type_get_id($type)));
                if(empty($bundleEntities))
                {
                  continue;
                }                

                if(!isset($results[$uri]["type"]))
                {
                  $results[$uri]["type"] = array();
                }

                $entityTypesUris[] = $type;
                array_push($results[$uri]["type"], resource_type_get_id($type));
              }  

              // Keep the top (more specialized class) of the classes hierarchy currently loaded
              $results[$uri]["type"] = array(resource_type_get_id(osf_entities_get_top_class($entityTypesUris)));
              
              // If there is no type/bundle exposed in Drupal for this entity, we define its bundle type
              // to be "owl_thing". To see if the entity is mapped or not, we only have to check the
              // difference between the ['type'] and the ['rdf_type] arrays
              if(empty($results[$uri]["type"]))            
              {                
                $results[$uri]["type"] = array();

                array_push($results[$uri]["type"], resource_type_get_id(Namespaces::$owl."Thing"));
                
                if(!isset($fieldsByBundles['owl_thing']))              
                {
                  $fieldsByBundles['owl_thing'] = array('iron_preflabel', 'iron_description');
                }                
              }
              
              $obj = new Subject($uri);
              $obj->setSubject($subject);

              $results[$uri]["uri"] = $uri;

              $results[$uri]["drid"] = osf_entities_get_drid_from_uri($uri);

              $results[$uri]["obj"] = $obj;
              
              // Keep track of the record we found
              if(array_search($uri, $foundEntities) === FALSE)
              {
                array_push($foundEntities, $uri);
              }

              // Make sure that we create a property, in the Entity's object instance
              // for each field that got mapped in OSF Entities. If no data come from
              // OSF for one of those fields, then the field will simply be empty.
              //
              // This is to keep the OSF Entities API consistent with what is
              // exposed as fields in OSF Entities. All the exposed fields will be
              // returned for all the entity_load() calls, but not all of them will
              // be populated with values. But at least, they will appear in the 
              // object's instance description.              
              if(is_array($fieldsByBundles[$results[$uri]["type"][0]]))
              {
                foreach($fieldsByBundles[$results[$uri]["type"][0]] as $field_name)
                {
                  // Only define a language for the non-Object Properties.
                  // Object properties will use LANGUAGE_DEFAULT
                  $property = unserialize($properties[$propertiesByField[$field_name]]);
                  
                  if($property)
                  {
                    if(array_search(Namespaces::$owl.'ObjectProperty', $property->getTypes()) === FALSE)
                    {
                      foreach(get_enabled_languages() as $lang)
                      {
                        $results[$uri][$field_name][$lang] = array();  
                      }
                    }
                    else
                    {
                      $results[$uri][$field_name][LANGUAGE_NONE] = array(); 
                    }
                  }
                }
              }
              
              $default_lang = get_default_language();

              if(isset($subject["prefLabel"]) && 
                 $subject["prefLabel"] != "")
              { 
                $results[$uri]["label"] = $subject["prefLabel"];
                
                foreach(get_enabled_languages() as $lang)
                {
                  $results[$uri]["preflabel"][$lang] = array();
                  $results[$uri][resource_type_get_id(Namespaces::$iron."prefLabel")][$lang] = array();
                }                
                
                if(isset($subject[Namespaces::$iron."prefLabel"]))
                {
                  foreach($subject[Namespaces::$iron."prefLabel"] as $value)
                  {
                    if(isset($value["value"]))
                    {
                      $lang = $default_lang;
                      if(isset($value['lang']))
                      {
                        $lang = $value['lang'];
                      }

                      $results[$uri]["preflabel"][$lang][] = array('value' => $value["value"]);
                      $results[$uri][resource_type_get_id(Namespaces::$iron."prefLabel")][$lang][] = array('value' => $value["value"]);
                    }              
                  }
                }
              }

              if(isset($subject["description"]) && 
                 $subject["description"] != "")
              {
                foreach(get_enabled_languages() as $lang)
                {
                  $results[$uri]["description"][$lang] = array();
                  $results[$uri][resource_type_get_id(Namespaces::$iron."description")][$lang] = array();
                }                
                
                if(isset($subject[Namespaces::$iron."description"]))
                {
                  foreach($subject[Namespaces::$iron."description"] as $value)
                  {
                    if(isset($value["value"]))
                    {
                      $lang = $default_lang;
                      if(isset($value['lang']))
                      {
                        $lang = $value['lang'];
                      }

                      $results[$uri]["description"][$lang][] = array('value' => $value["value"]);
                      $results[$uri][resource_type_get_id(Namespaces::$iron."description")][$lang][] = array('value' => $value["value"]);
                    }              
                  }
                }
              }
              
              if(isset($fieldsByBundles[$results[$uri]["type"][0]]) &&
                 in_array(resource_type_get_id(Namespaces::$iron."altLabel"), $fieldsByBundles[$results[$uri]["type"][0]]))
              {
                foreach(get_enabled_languages() as $lang)
                {
                  $results[$uri]["altLabel"][$lang] = array();
                  $results[$uri][resource_type_get_id(Namespaces::$iron."altLabel")][$lang] = array();
                }                
                
                if(isset($subject[Namespaces::$iron."altLabel"]))
                {
                  foreach($subject[Namespaces::$iron."altLabel"] as $value)
                  {
                    if(isset($value["value"]))
                    {
                      $lang = $default_lang;
                      if(isset($value['lang']))
                      {
                        $lang = $value['lang'];
                      }

                      $results[$uri]["altLabel"][$lang][] = array('value' => $value["value"]);
                      $results[$uri][resource_type_get_id(Namespaces::$iron."altLabel")][$lang][] = array('value' => $value["value"]);
                    }              
                  }
                }
              }

              if(isset($fieldsByBundles[$results[$uri]["type"][0]]) &&
                 in_array(resource_type_get_id(Namespaces::$iron."prefURL"), $fieldsByBundles[$results[$uri]["type"][0]]))
              {
                foreach(get_enabled_languages() as $lang)
                {
                  $results[$uri]["prefURL"][$lang] = array();
                  $results[$uri][resource_type_get_id(Namespaces::$iron."prefURL")][$lang] = array();
                }                
                
                if(isset($subject[Namespaces::$iron."prefURL"]))
                {
                  foreach($subject[Namespaces::$iron."prefURL"] as $value)
                  {
                    if(isset($value["value"]))
                    {
                      $lang = $default_lang;
                      if(isset($value['lang']))
                      {
                        $lang = $value['lang'];
                      }

                      $results[$uri]["prefURL"][$lang][] = array('value' => $value["value"]);
                      $results[$uri][resource_type_get_id(Namespaces::$iron."prefURL")][$lang][] = array('value' => $value["value"]);
                    }              
                  }
                }
              }

              if(isset($fieldsByBundles[$results[$uri]["type"][0]]) &&
                 in_array(resource_type_get_id(Namespaces::$geo."lat"), $fieldsByBundles[$results[$uri]["type"][0]]))
              {
                $results[$uri]["lat"] = array($default_lang => array(array('value' => $subject[Namespaces::$geo."lat"][0]["value"])));
                $results[$uri][resource_type_get_id(Namespaces::$geo."lat")] = array($default_lang => array(array('value' => $subject[Namespaces::$geo."lat"][0]["value"])));
              }

              if(isset($fieldsByBundles[$results[$uri]["type"][0]]) &&
                 in_array(resource_type_get_id(Namespaces::$geo."long"), $fieldsByBundles[$results[$uri]["type"][0]]))
              {
                $results[$uri]["long"] = array($default_lang => array(array('value' => $subject[Namespaces::$geo."long"][0]["value"])));
                $results[$uri][resource_type_get_id(Namespaces::$geo."long")] = array($default_lang => array(array('value' => $subject[Namespaces::$geo."long"][0]["value"])));
              }

              foreach($subject as $attributeUri => $values)
              { 
                if((!isset($fieldsByBundles[$results[$uri]["type"][0]]) ||
                    !in_array(resource_type_get_id($attributeUri), $fieldsByBundles[$results[$uri]["type"][0]])))
                {                 
                  continue;
                }
                      
                if($attributeUri == "type" ||
                   $attributeUri == "prefLabel" ||
                   $attributeUri == "description" ||
                   $attributeUri == "altLabel" ||
                   $attributeUri == "prefURL" ||
                   $attributeUri == "long" ||
                   $attributeUri == "lat" ||
                   $attributeUri == Namespaces::$iron."prefLabel" ||
                   $attributeUri == Namespaces::$iron."altLabel" ||
                   $attributeUri == Namespaces::$iron."prefURL" ||
                   $attributeUri == Namespaces::$iron."description" ||
                   $attributeUri == Namespaces::$geo."lat" ||
                   $attributeUri == Namespaces::$geo."long")
                {
                  continue;
                }

                if(is_array($values))
                {
                  foreach($values as $value)
                  {
                    $isValueUri = FALSE;
                    if(isset($value["value"]))
                    {
                      if(isset($value['lang']))
                      {
                        if(!is_array($results[$uri][resource_type_get_id($attributeUri)][$value['lang']]))
                        {
                          // This handle the usecase where the ontology define a property to be an Object Property
                          // but that the value that has been serialized is a literal.
                          // What we do is to translate it back as a URI since the ontology has authority
                          $value["uri"] = $value["value"];
                          $isValueUri = TRUE;
                        }
                        else
                        {
                          array_push($results[$uri][resource_type_get_id($attributeUri)][$value['lang']], array('value' => $value["value"]));
                        }
                      }
                      else
                      { 
                        if(!is_array($results[$uri][resource_type_get_id($attributeUri)][$default_lang]))
                        {
                          // This handle the usecase where the ontology define a property to be an Object Property
                          // but that the value that has been serialized is a literal.
                          // What we do is to translate it back as a URI since the ontology has authority
                          $value["uri"] = $value["value"];
                          $isValueUri = TRUE;
                        }
                        else
                        {
                          array_push($results[$uri][resource_type_get_id($attributeUri)][$default_lang], array('value' => $value["value"]));
                        }
                      }
                    }
                    else
                    {
                      $isValueUri = TRUE;
                    }
                    
                    if($isValueUri)
                    { 
                      array_push($results[$uri][resource_type_get_id($attributeUri)][LANGUAGE_NONE], array('value' => $value["uri"]));

                      if(isset($value['reify']))
                      {
                        $results[$uri]["reify_".resource_type_get_id($attributeUri)][LANGUAGE_NONE] = array(array('value' => $value['reify'][Namespaces::$wsf.'objectLabel'][0]));
                      }
                    }
                  }
                }
              }

              // Use entity_create(...) to create the proper Entity object to feed to the requester
              $results[$uri] = $this->create($results[$uri]);
            }
          }
        }
        else
        {
          watchdog('osf_entities', 
                   'Can\'t read the entity in OSF: [@error] @errorMsg - @errorDescription.', 
                   array("@error" => $read->getStatus(),
                         "@errorMsg" => $read->getStatusMessage(),
                         "@errorDescription" => $read->getStatusMessageDescription()));                       
        }
        
        if(count($foundEntities) >= count($ids))
        {
          // If all requested records are found after querying this network
          // we return the results.
          return($results);
        }
      }

      if(count($foundEntities) == 0)
      {
        // If we can't find the records in OSF for the given IDs, then we try to load them
        // using the parent::load(...) function
        // This case normally happen when we try to list all the bundles available for the
        // Resource Type entity type.
        $entities = parent::load($ids, $conditions);

        return($entities);
      }
      
      return($results);
    }

    /**
    * Overridden.
    * @see DrupalDefaultEntityController::cacheSet()
    */
    protected function cacheSet($entities) {
      $this->entityCache += $entities;
    }
  }

  /**
   * UI controller for Resource Type.
   */
  class ResourceTypeUIController extends EntityDefaultUIController
  {
    /**
     * Overrides hook_menu() defaults.
     */
    public function hook_menu() {
      $items = parent::hook_menu();

      $items[$this->path]['description'] = t('Manage Resource types.');

      return $items;
    }
  }

?>
