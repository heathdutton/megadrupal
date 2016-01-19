<?php 

/**
 * Hook to declare hidden group
 * You need to know their machine name (as displayed in hook_form_alter) and they need to have fieldset type
 * 
 * @return array of string
 */
function hook_hide_group_declare(){
  return array(
    'revision_information',
    'menu',
  );
}