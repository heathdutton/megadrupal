
Typical Entity Example module
-----------------
by Denis Borisov, denbor@bk.ru

Typical Entity Example module is an educational module which shows how to create
entities for Drupal in your modules.
This module contains some sub-modules, and each of these sub-modules implements
an educational Drupal entity.

--------------------------------------------------------------------------------
                          Typical Entity Example
--------------------------------------------------------------------------------

Typical Entity Example module is a parent module for all its sub-modules and 
implements the Drupal permissions, used by these sub-modules.

The permissions, implemented by this module, can be configured on 
<yoursite>/admin/people/permissions in "Typical Entity Example" section.

--------------------------------------------------------------------------------
                         Typical Entity Example 1
--------------------------------------------------------------------------------

Typical Entity Example 1 sub-module creates the smallest possible entity
with machine name "typical_entity_1".
This entity exists in Drupal, but you cannot even create the instances of it.

To verify that entity, created by the module, is working properly, enable the 
standard "testing" module, go to <yoursite>/admin/config/development/testing
and run "Typical entity 1 unit tests" in "Typical Entity Example" section.


--------------------------------------------------------------------------------
                         Typical Entity Example 2
--------------------------------------------------------------------------------

Typical Entity Example 2 sub-module creates a minimalistic entity, the instances
of which can be created, edited or deleted with Entity Metadata Wrappers.

To verify that entity, created by the module, is working properly, enable the 
standard "testing" module, go to <yoursite>/admin/config/development/testing
and run "Typical entity 2 unit tests" in "Typical Entity Example" section.

Example usage of typical_entity_example_2:
@code
// Create new entity.
$entity = entity_create('typical_entity_example_2', array());
$wrapper_1 = entity_metadata_wrapper('typical_entity_example_2', $entity);
$wrapper_1->title = 'Random name 1';
$wrapper_1->description = 'Random description 1';
$wrapper_1->save();

// Load and edit created entity.
$entities = entity_load(
              'typical_entity_example_2', 
              array($wrapper_1->teid->value())
            );
$wrapper_2 = entity_metadata_wrapper(
               'typical_entity_example_2', 
               reset($entities)
             );
$wrapper_2->title = 'Random name 2';
$wrapper_2->save();

// Delete created entity.
$wrapper_2->delete();
@endcode

--------------------------------------------------------------------------------
                         Typical Entity Example 3
--------------------------------------------------------------------------------

Typical Entity Example 3 sub-module creates a small entity, which has some
predefined bundles and attached fields.

To verify that entity, created by the module, is working properly, enable the 
standard "testing" module, go to <yoursite>/admin/config/development/testing
and run "Typical entity 3 unit tests" in "Typical Entity Example" section.

Example usage of typical_entity_example_3:
@code
$entity = entity_create(
            'typical_entity_example_3', 
            array('type' => 'typical_entity_example_3_bundle_1')
          );
$wrapper = entity_metadata_wrapper('typical_entity_example_3', $entity);
$wrapper->title = 'Random title 1.';
$wrapper->description = 'Random description 1.';
$wrapper->typical_entity_example_3_field_1 = 'Random field 1.';
$wrapper->save();
@endcode

--------------------------------------------------------------------------------
                         Typical Entity Example 4
--------------------------------------------------------------------------------

Typical Entity Example 4 sub-module creates the entity, the instances of which
can be created and edited with a simple user interface.

This user interface is available on <yoursite>/typical_entity_example_4.

--------------------------------------------------------------------------------
                         Typical Entity Example 5
--------------------------------------------------------------------------------

Typical Entity Example 5 sub-module creates the entity, which can be used in 
views.
Also created entity supports revisions.

User interface for working with instances of the entity is available on 
<yoursite>/typical_entity_example_5.

--------------------------------------------------------------------------------
                         Typical Entity Example 6
--------------------------------------------------------------------------------

Typical Entity Example 6 sub-module creates the entity, which bundles can be 
created and edited via Drupal user interface.

Bundles of the entity can be changed on 
<yoursite>/admin/structure/typical_entity_example_6.
User interface for working with instances of the entity is available on 
<yoursite>/typical_entity_example_6.

--------------------------------------------------------------------------------
                                 Credits
--------------------------------------------------------------------------------

Development of this module was sponsored by the OITS Co. Ltd.
http://www.oits.ru/
