Clients SuiteCRM
================

Provides a [Clients] connection type for connecting to SuiteCRM over [SugarCRM REST].
As well as an integration into [Remote Entity API]

### Features: ###

* Supports connection read-only mode.
* Supports debug mode.
* Integrates with [Remote Entity API] to integrate SuiteCRM Data entities into Drupal entities.
* Allows customizing callbacks - to allow calling a customized SuiteCRM API. 

### Configuration: ###

1. Configure a clients connection to your CRM here: `admin/structure/clients`
2. If necessary modifiy the automatically built clients resources here:
`admin/structure/clients/resources`
3. If necessary enable resources as entities here: `admin/structure/suitecrm`.
The entity type names consist of `{connection_name}__{suiteCRM_ModuleName}`. 
A rudimentary UI for entities is available here `admin/structure/suitecrm/{entity_type}`
e.g. `admin/structure/suitecrm/suitecrm__campaigns`

### Examples: ###

1. Raw call, only using a connection:
You can use the [Service Method Calls] as methods of `$connection`:
```php
$connection = clients_connection_load('suitecrm');
$connection->get_available_modules();
```
2. Using an entity:
The entity type names consist of `{connection_name}__{suiteCRM_ModuleName}`. 
You can use remote entities pretty much transparently.
  * Normal load (won't load the entity directly from remote):
```php
$entity = entity_load('suitecrm__campaigns', 'SuiteCRMId or local id');
```
  * Load explicitely using the remote id (will load from remote if necessary):
```php
$entity = remote_entity_load_by_remote_id('suitecrm__campaigns', 'SuiteCRMId');
```
  * Forcefully load from remote, bypassing local caches:
```php
$resource = clients_resource_get_for_component('remote_entity', 'suitecrm__campaigns');
$entity = $resource->remote_entity_load('SuiteCRMId');
```
  * Saving entities - remote properties are prefixed by "crm_":
```php
$entity = remote_entity_load_by_remote_id('suitecrm__campaigns', 'SuiteCRMId');
// Setting remote properties.
$entity->crm_name = 'New Name';
$entity->crm_budget = '20000';
// Implicit remote save. Will check if a remote property was changed and trigger
// a remote save if necessary.
$entity->save();
// Explicit remote save.
$entity->remoteSave();
// Mark entity as needing a remote save.
$entity->needs_remote_save = TRUE;
$entity->save();
```
  
### Security ###

You can overwrite the default insert / update callbacks globally by using
following variables:

* `clients_suitecrm_default_insert_callback`
* `clients_suitecrm_default_update_callback`

Setting them to a dead end will prevent accidental writes. 
To enable "writing" configure the proper callback path in the clients resources  
you'd like to have writable.

[SugarCRM REST]: http://support.sugarcrm.com/02_Documentation/04_Sugar_Developer/Sugar_Developer_Guide_6.7/02_Application_Framework/Web_Services/01_REST/
[Service Method Calls]:  http://support.sugarcrm.com/02_Documentation/04_Sugar_Developer/Sugar_Developer_Guide_6.7/02_Application_Framework/Web_Services/05_Method_Calls/
[Clients]: https://www.drupal.org/project/clients
[Remote Entity API]: https://www.drupal.org/project/remote_entity