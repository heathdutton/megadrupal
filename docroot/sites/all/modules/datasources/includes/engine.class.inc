<?php

/**
 * @file
 * Engine classes.
 */

/**
 * Interface defining the methods of datasource engine.
 */
interface DatasourceEngineInterface {
  
  /**
   * The method to import a record of the datasource input.
   * 
   * @return boolean
   *   - self::IMPORTED : record imported
   *   - self::NOT_IMPORTED : record not imported
   *   - self::NOT_IMPORTED_ERROR : record not imported because of errors
   *   - self::EOF : no more record
   */
  public function importRecord();
   
  /**
   * Initialize import.
   * 
   * @return boolean
   *   - TRUE : import can begin.
   *   - FALSE : import cannot begin.
   */
  public function initImport();
  
  /**
   * Some cleanup stuff.
   */
  public function completeImport();
}

/**
 * Abstract datasource engine.
 */
abstract class DatasourceEngineAbstract implements DatasourceEngineInterface {

  const IMPORTED = 2;
  const NOT_IMPORTED = 1;
  const NOT_IMPORTED_ERROR = 0;
  const EOF = -1;
  
  public $input_id;

  protected $input = NULL;
  
  public function __construct($input_id) {
    $this->input_id = $input_id;
    $this->input = datasources_inputs_info($this->input_id);
  }

  protected $process_api_live;
  
  public function initImport() {
    if (module_exists('process_api')&&(!$this->input['process_api_live'])) {
      $this->process_api_live = process_api_live();
      process_api_live(FALSE);
    }
    return TRUE;
  }
  
  protected $flags = array();
  
  /**
   * Flags can be used to alter engine behaviour.
   * @param string $name
   * @param mixed $value
   */
  public function setFlag($name, $value) {
    $this->flags[$name] = $value;
  }
  
  public function completeImport() {
    if (module_exists('process_api')&&(!$this->input['process_api_live'])) {
      process_api_live($this->process_api_live);
    }
  }
  
  /**
   * Default importRecord().
   * Call getRecord() and try to import te datas matching with udid fields.
   * 
   * @see DatasourceEngineInterface::importRecord()
   * @see getRecord()
   * 
   * @return integer
   *   The status of the import.
   *   - self::IMPORTED : record imported
   *   - self::NOT_IMPORTED : record not imported
   *   - self::NOT_IMPORTED_ERROR : record not imported because of errors
   *   - self::EOF : no more record
   */
  public function importRecord() {
    try {
      // keep track of new entities
      $new_entities = array();
      
      $to_save_context=array();
      $record=$this->getRecord();
      
      if (!$record) return self::EOF;
      
      $record = (array) $record;
      
      $input = datasources_inputs_info($this->input_id);
      
      // add some meta/technical fields
      $record['#datasource_name'] = $input['name'];
      $record['#datasource_id'] = $this->input_id;
      
      //add custom fields values
      if (!empty($this->input['custom_fields'])) {
        foreach ($this->input['custom_fields'] as $k => $v) {
          $record['#custom_' . $k] = $v;
        }
      }
  
      $entities=array();
      $exists = FALSE;
      
      if ($input['has_key']) {
      
        $target_fields_info = datasources_entity_fields($this->input_id);
        $ids = array();
        // we try to match existing entity in the specified order
        $flat_udid=NULL;
        foreach ($input['keys'] as $field => $key_info) {
          // use only active key
          if ($key_info['use']) {
            
            $udid_value = $record[$field];
            // preprocess value to get udid
            if ($key_info['udid_preprocessor']) {
              $target_field_info = $target_fields_info['fields'][$key_info['udid_target_field']];
              
              $context = array(
                  'source_type' => $input['fields'][$field]['type'],
                  'target_type' => $target_field_info['type'],
                  'source_field' => $field,
                  'target_field' => $key_info['udid_target_field'],
                  'input' => $input,
                  'record' => $record,
                  'engine' => $this,
                  'udid_preprocessor' => $key_info['udid_preprocessor'],
              );
              $udid_value = datasources_apply_preprocessor($key_info['udid_preprocessor'], $udid_value, $context);
            }
                      
            if (!udid_check($udid_value)) {
              watchdog('datasource', 'Datasource !input : not a valid udid (!field).', array('!input' => $this->input_id, '!field' => $field), WATCHDOG_WARNING);
              return self::NOT_IMPORTED_ERROR;
            }

            $res = udid_entity_ids($udid_value, array('entities' => $input['entity']));         
            if (isset($res[$input['entity']])) {
              $exists = TRUE;
              if (!$flat_udid) {
                // store the first udid for watchdog...
                $flat_udid="{$udid_value['source']}/{$udid_value['id']}";
              }
              // foreach matching entity we have to test if we are alowed to update
              foreach ($res[$input['entity']] as $id => $item) {
                $entity=entity_load($input['entity'], array($id));
                if ($entity) {
                  $entity = reset($entity);
                  $ids[$id] = array('entity' => $entity);
                  
                  // $item->fields is an array of the fields that triggerd the match in udid_entity_ids()
                  foreach ($item->fields as $triggering_key) {
                    if (isset($input['keys'][$triggering_key])) {           
                      // smart keys stuff, modes are:
                      // DUMB (0): always update
                      // PROTECT (1): do not update if target was modified
                      // NEW (2): update only if source is new version
                      // If an entity has multiple smart policies NEW will take over PROTECT and PROTECT will take over DUMB.
                      $smart_mode = max($key_info['smart'], $input['keys'][$triggering_key]['smart']);
                    }
                    else {
                      // be paranoid
                      $smart_mode = $key_info['smart'];
                    }
                    // something to be smart about ?
                    if ($smart_mode > 0) {
                      $current = field_get_items($input['entity'], $entity, $triggering_key);
                      if ($current) {
                        if (!isset($ids[$id]['smart'][$triggering_key])) $ids[$id]['smart'][$triggering_key] = TRUE;
                        $cmp = udid_compare($current[0], $udid_value);
                        if ($cmp!==FALSE) {
                          if ($smart_mode == DATASOURCES_SMART_PROTECT) {
                            // update if new record is newer or equal than current (aka current was not modified)
                            $ids[$id]['smart'][$triggering_key] = ($cmp <= 0) && $ids[$id]['smart'][$triggering_key];
                          }
                          elseif ($smart_mode == DATASOURCES_SMART_NEW) {
                            // only update if new record is newer than current
                            $ids[$id]['smart'][$triggering_key] = ($cmp < 0) && $ids[$id]['smart'][$triggering_key];
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }

    
        $n=0;
        foreach($ids as $id => $item) {
          $entity = $item['entity'];
          list(, , $bundle)=entity_extract_ids($input['entity'], $entity);
          if ($input['bundle']==$bundle || $input['policies']['bad_bundle'] = 'update') {
            // entity is updatable
            
            // test if all smart keys are ok
            $allsmart = TRUE;
            if (isset($item['smart'])) {
              foreach($item['smart'] as $smart) {
                if (!$smart) $allsmart = FALSE;
              }
            }
            
            if ($allsmart) {
              // all is smart: updating is ok
              $entities[$id]=$entity;
            }
            else {
              watchdog('datasource', 'Smart key policy : a record from datasource input !input was not imported/updated (!udid).', array('!input' => $this->input_id, '!udid' => $flat_udid), WATCHDOG_WARNING);
            }
            
            if ($input['policies']['duplicate']=='first') {
              // retrieve and update only the first entity
              break;
            }
            elseif ($n>0 && $input['policies']['duplicate']=='abort') {
              watchdog('datasource', 'Duplicate policy : a record from datasource input !input was not imported because it matches multiple entities (!udid).', array('!input' => $this->input_id, '!udid' => $flat_udid), WATCHDOG_WARNING);
              return self::NOT_IMPORTED;
            }
            $n++;
          }
        }
      
        if ($exists && !$input['policies']['update']) {
          watchdog('datasource', 'Update policy : a record from datasource input !input was not imported (!udid).', array('!input' => $this->input_id, '!udid' => $flat_udid), WATCHDOG_WARNING);
          return self::NOT_IMPORTED;
        }
      }

      if (!$exists && !$input['policies']['insert']) {   
        watchdog('datasource', 'Insert policy : a record from datasource input !input was not imported (!udid).', array('!input' => $this->input_id, '!udid' => $flat_udid), WATCHDOG_WARNING);
        return self::NOT_IMPORTED;
      }
      
      if (!$exists) {
        $entities=array();
        // create an empty entity
        $entity=entity_create_stub_entity($input['entity'], array(0, NULL, $input['bundle']));
        datasources_set_language($input['entity'], $entity);
        entity_save($input['entity'], $entity); // saving seams easier to handle id
        list($id, , ) = entity_extract_ids($input['entity'], $entity);      
        // get a cached version for consistency with later wrapper acces to entity
        $entity = entity_load($input['entity'], array($id));
        $entity = reset($entity);
        $entities[$id]=$entity;
        $new_entities[$id]=$entity;
      }
      
      $to_save=array();
      
      foreach ($entities as $id => $entity) {
        $fields_info=datasources_entity_fields($this->input_id, $entity, TRUE);
        
        foreach ($fields_info['fields'] as $field => $field_info) {
          
          if (isset($field_info['map']['field']) && isset($record[$field_info['map']['field']])) {
            // TODO add EntityDrupalWrapper true support currently only basic structure are supported
            $value=$record[$field_info['map']['field']];
            $context = array(
              'source_type' => $input['fields'][$field_info['map']['field']]['type'],
              'target_type' => $field_info['type'],
              'source_field' => $field_info['map']['field'],
              'target_field' => $field,
              'input' => $input,
              'record' => $record,
              'engine' => $this,
            );
            
            $w=$field_info['wrapper'];
            
            // try to get the underlaying entity
            do {
              $info = $w->info();
              if (isset($info['parent'])) {
                $w = $info['parent'];
              }
              else {
                $w = NULL;
                break;
              }
            } while(!$w instanceof EntityDrupalWrapper);
            
            if ($w) {
              
              // got it !
              $w->language(language_default('language'));
              
              $list_mode = isset($field_info['map']['list']) ? $field_info['map']['list'] : '';
  
              $l_target = datasources_list_nesting_level($field_info['type']);
              $l_source = datasources_list_nesting_level($input['fields'][$field_info['map']['field']]['type']);

              $setmode = 'single';
              if ($l_target>0 && $list_mode) {
                // single|list to list
                $context['list'] = $list_mode;
                if ($l_target > 1||$l_source > 1) {
                   watchdog('datasource', 'Datasource !input : can only manage 1 level list field (!field).', array('!input' => $this->input_id, '!field' => $field_info['map']['field']), WATCHDOG_WARNING);
                   return self::NOT_IMPORTED_ERROR;
                 }
                 if ($l_target<$l_source) {
                   watchdog('datasource', 'Datasource !input : cannot put a list source field in a non list target field (!field)', array('!input' => $this->input_id, '!field' => $field_info['map']['field']), WATCHDOG_WARNING);
                   return self::NOT_IMPORTED_ERROR;
                 }
                 if ($list_mode == 'replace' || $list_mode == 'first' || $list_mode == 'last') {
                   // reset existing value
                   $field_info['wrapper']->set(array());
                 }
                 if ($l_source == 0) {
                   $values = array($value);
                 }
                 else {
                   if ($list_mode == 'first') {
                     $values = count($value) ? array(reset($value)) : array();
                   }
                   elseif ($list_mode == 'last') {
                     $values = count($value) ? array(end($value)) : array();
                   }
                   else {
                     $values = $value;
                   }
                 }
                 $setmode = 'list';
              }
              elseif ($l_target==0 && $l_source > 0 && $list_mode) {
                // list to single
                if ($l_target > 1||$l_source > 1) {
                  watchdog('datasource', 'Datasource !input : can only manage 1 level list field (!field).', array('!input' => $this->input_id, '!field' => $field_info['map']['field']), WATCHDOG_WARNING);
                  return self::NOT_IMPORTED_ERROR;
                }
                if (!($list_mode=='first'||$list_mode=='last')) {
                  watchdog('datasource', 'Datasource !input : cannot put a list source field in a non list target field, choose first or last (!field)', array('!input' => $this->input_id, '!field' => $field_info['map']['field']), WATCHDOG_WARNING);
                  return self::NOT_IMPORTED_ERROR;
                }
                if ($list_mode=='first') {
                  $value = count($value) ? reset($value) : NULL;
                }
                else {
                  $value = count($value) ? end($value) : NULL;
                }
                $setmode = 'single';
              }
              
              if ($setmode == 'list') {
                // list value set
                foreach ($values as $value) {
                  // append a new value
                  $value = datasources_apply_preprocessor($field_info['map']['preprocessor'], $value, $context);
                  // NULL means nothing so it's not added
                  if ($value!==NULL) $field_info['wrapper']->get(NULL)->set($value);
                }
              }
              else {
                // single value set by default
                $value = datasources_apply_preprocessor($field_info['map']['preprocessor'], $value, $context);
                $field_info['wrapper']->set($value);
              }
  
              // store wrapper of updated entities
              if (!in_array($w, $to_save, TRUE)) {
                $to_save[]=$w;
              }
              
              // store some usefull (?) infos
              if (!isset($to_save_context[$id])) {
                $to_save_context[$id]=array(
                  'entity' => $entity,
                  'fields' => array(),
                );
              }
              $to_save_context[$id]['fields'][$field] = $field_info['map'];
            }
            else {
              // oops
              watchdog('datasource', 'Error : while importing datasource input field !input::!field, cannot find underlaying entity for on record (!udid).', array('!input' => $this->input_id, '!field' => $field, '!udid' => $flat_udid), WATCHDOG_ERROR);
              return self::NOT_IMPORTED_ERROR;
            }
          }
        }     
      }

      // last chance to modify entities
      foreach ($to_save_context as $id => $context) {
        $entity = $context['entity'];
        $context['record'] = $record;
        $this->beforeSaveEntity($entity, $context);
      }
      
      try {
                
        // save underlaying entities using wrapper
        foreach ($to_save as $w) {
          $w->save();
        }
        
        if (count($to_save) > 0) {
          $status = self::IMPORTED;
          // allow module to do more stuff on succesfull import
          $this->postImportRecord($record, $to_save_context);
        }
        else {
          $status = self::NOT_IMPORTED;
        }
      }
      catch(EntityMetadataWrapperException $e) {
        throw new DatasourceEngineException('EntityMetadataWrapperException : ' . $e->getMessage());
      }
    }
    catch (DatasourceEngineException $e) {
      watchdog('datasource', 'DatasourceEngineException while importing !input::!field (udid = !udid) : !msg', array('!msg' => $e->getMessage(), '!input' => $this->input_id, '!field' => isset($field) ? $field : '*unknown*', '!udid' => isset($flat_udid) ? $flat_udid : '*unknown*'), WATCHDOG_ERROR);
      $to_save_context['exception'] = $e;
      
      $this->onErrorImportRecord(isset($record) ? $record : NULL, $to_save_context);
      
      // some cleanup
      if (!empty($new_entities)) entity_delete_multiple($input['entity'], array_keys($new_entities));

      return self::NOT_IMPORTED_ERROR;
    }
    
    // allow module to do more stuff
    $status = $this->postProcessRecord($record, $to_save_context, $status);
    
    return $status;
  }

  /**
   * Last chance to modify the imported entity.
   * @param stdClass $entity
   * @param array $context
   */
  protected function beforeSaveEntity($entity, array $context) {
    // nothing by default
  }
  
  /**
   * Some post import record stuff.
   * Only executed if record import was successfull.
   * 
   * @param array $record
   * @param array $context
   *  A "entity ids" keyed array of arrays with keys :
   *    - entity : the updated entity
   *    - fields : the updated fields (target => source)
   *  
   */
  protected function postImportRecord(array $record, array $context) {
    // nothing by default
  }
  
  /**
   * Some post process record stuff.
   *
   * @param array $record
   * @param array $context
   *  A "entity ids" keyed array of arrays with keys :
   *    - entity : the updated entity
   *    - fields : the updated fields (target => source)
   * @param integer status
   */
  protected function postProcessRecord(array $record, array $context, $status) {
    return $status;
  }
  
  /**
   * Some stuff to do on error.
   * Only executed if record import was not successfull.
   * 
   * @param array $record
   * @param array $context
   *  A "entity ids" keyed array of arrays with keys :
   *    - entity : the updated entity
   *    - fields : the updated fields (target => source)
   *    - exception : the raised exception if any
   *  
   */
  protected function onErrorImportRecord($record, array $context) {
    // nothing by default
  }  
  
  /**
   * Must provide an input record.
   * The input record is an array with values keyed with fields defined in the datasource input.
   * 
   * @see hook_datasource_inputs()
   * 
   * @return array|stdClass|FALSE the record or FALSE if no more record
   */
  abstract protected function getRecord();
  
}

class DatasourceEngineException extends Exception {}
