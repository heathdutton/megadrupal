<?php 

/**
 * Check if entity is ready to send to vgwort
 * 
 * No return - Everything is fine
 * Array - Return error message return array('message',t('My error message'));
 * 
 * @param String $entity_type
 * @param Object $entity
 * 
 * @return Array $message | empty
 */
function hook_vgwort_check($entity_type, $entity) {
	
}

/**
 * Hook for fill data send to VGWort WS
 * 
 * @param String $entity_type
 * @param Object $entity
 * @param Array $data
 * 
 * @return Array $data
 */
function hook_vgwort_message_alter( $message, $context ) {
}

function hook_vgwort_message_plaintext_alter( $content ) {
}