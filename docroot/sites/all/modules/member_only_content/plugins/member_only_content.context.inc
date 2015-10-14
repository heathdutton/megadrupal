<?php

/**
 *
 *  This file defines the context condition
 *
 */

class member_only_content_context_condition extends context_condition_node{

  function condition_values() {
    $values = array();
    foreach (node_type_get_types() as $type) {
      $values[$type->type] = check_plain($type->name);
    }
    return $values;
  }

  //Just to get rid of the condition_node option additions
  function options_form(){

  }

  function execute($node) {
    foreach($this->get_contexts() as $context){
      $options = $this->fetch_from_context($context, 'values');
      foreach($options as $node_type){
        if($node->type == $node_type){
          $this->condition_met($context);
        }
      }
    }
  }
}