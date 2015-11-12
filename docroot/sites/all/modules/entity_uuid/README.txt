Introduction
------------
With module entity_uuid you can assign uuid for another entities not only for 
Drupal core entities that are defined via hook_entity_info.

In this module, we add a new properties for hook_entity_info.
<code>
  $info['your_entity'] = array(
    'uuid' => TRUE,
    'entity keys' => array(
      'uuid' => 'uuid',
    ),
  );
</code>

Installation
-------------
	Copy the whole entity_uuid directory to your modules directory and activate 
	the module.

Usage
-----
* Developer:
	In this module, we add a new properties for hook_entity_info.
	<code>
	  $info['uuid'] = TRUE;
	</code>

* Webmaster
	Go to admin/config/system/entity-uuid for assign uuid into another entities.
