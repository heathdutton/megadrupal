<?php

class time_entry_views_handler_field_time extends views_handler_field_date {
 public $field_info = array('field_name' => 'time', 'module' => 'time_entry');

 public function get_items($row) {
   return $row->{$this->field_alias};
 }

 function render(&$values) {
   if(!isset($values->_field_data)) {
     $values->_field_data = array();
   }
   $entity = array_shift(entity_load('time_entry', array($values->id))); //entity_load($entity_type, array($entity_id), array($entity_info['entity keys']['revision'] => $revision_id));


   foreach($this->view->field as $field_name => $field) {
     if (isset($field->definition['real field']) && in_array($field->definition['real field'], array('time', 'id')) && $field->table_alias == 'time_entry') {
       $values->_field_data[$field->field_alias] =  array(
         'entity_type' => 'time_entry',
         'entity' => $entity,
       );
     }

   }

   $datetime = $entity->toDateTime();
   $values->time_entry_time = array('rendered' => $values->time_entry_time);
   $values->time_entry_time['raw']['value'] = $datetime['value']->format('Y-m-d H:i:s');
   $values->time_entry_time['raw']['value2'] = $datetime['value2']->format('Y-m-d H:i:s');
   $values->time_entry_time = array($values->time_entry_time);
 }
}
