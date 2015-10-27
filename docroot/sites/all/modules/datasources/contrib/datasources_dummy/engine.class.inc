<?php

/**
 * @file
 * Engine classes.
 */

/**
 * Dummy datasource engine.
 */
class DatasourceEngineDummy1 extends DatasourceEngineAbstract {

  protected $i;
  
  public function initImport() {
    $this->i=0;
    return parent::initImport();
  }
  
  protected function getRecord() {
    module_load_include('inc', 'devel_generate');
    module_load_include('inc', 'devel_generate', 'image.devel_generate');
    $input = datasources_inputs_info($this->input_id);
    if ($input['max']<0 && $this->i>50) return FALSE; // just to be sure not to loop
    $this->i++;
    
    $item=array();
    foreach ($input['fields'] as $field_id => $field_info) {
      if ($field_id=='key1'||$field_id=='key2') {
        $item[$field_id] = array(
          'source' => 'dummy_datasource_1',
          'id' => rand(1, 50),
        );
      }
      elseif ($field_info['type']=='field_item_image') {
        // TODO
      }
      elseif ($field_info['type']=='file_image') {
        $item[$field_id]=devel_generate_image('png', '100x100', '300x300');
      }
      elseif (datasources_is_list_type($field_info['type'])) {
        $n=rand(1, 5);
        for ($i=0;$i<5;$i++) {
          $item[$field_id][] = devel_create_greeking(10);
        }
      }
      else {
        $item[$field_id] = devel_create_greeking(10);
      }
    }
    
    return $item;
  }
}

