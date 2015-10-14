================================================================================
ABOUT SDBO
================================================================================
SDBO is a relational database object mapper. It provides you with convenient way
to handle CRUD operations as well as with a factory to load and init objects.
Advanced feature is REL interface which handles child and/or related objects.
It moreover provides a unified way to handle and manipulate persisted data
within your code so that the code will be more uniform and better readable.

Drupal provides FieldAPI to handle persisted data structures. However if you
consider FieldAPI to be too much overhead for a simple data manipulation SDBO is
the way to go.

================================================================================
FEATURES
================================================================================
- Uniform CRUD
- Factory to load, init and cache allready loaded objects
- REL interface to manage child and related objects
- Flexible way to alter data before and after CRUD operations

================================================================================
INSTALLATION AND CONFIGURATION
================================================================================
SDBO is a library, so by itself it does not provide any functionality. Therefore
there is no configuration or installation besides enabling the module in the
module list.

================================================================================
BASIC USAGE
================================================================================
To start it is enough to define Drupal schema, make sure Drupal has created the
table and extend the SDBObject class. Make sure the schema defines primary key
named "id". By doing so the SDBObject will take care of this attribute.

// /*Code*/

// Drupal schema for example objects.
$schema['sdbo_example_objects'] = array(
  'description' => 'SDB Example objects table.',
  'fields' => array(
    'id' => array(
      'description' => 'Object id',
      'type' => 'serial',
      'not null' => TRUE,
      'unsigned' => TRUE),
    'created' => array(
      'description' => 'Timestamp when the object was created',
      'type' => 'int',
      'not null' => TRUE,
      'default' => 0),
    'title' => array(
      'description' => 'Object title',
      'type' => 'varchar',
      'length' => 124,
      'not null' => TRUE,
      'default' => ''),
    'body' => array(
      'description' => 'Object body',
      'type' => 'text',
      'not null' => FALSE),
    ),
  'primary key' => array('id'),
);

/**
 * Example SDBObject.
 */
class SDBExampleObject extends SDBObject {

  // Persisted attributes as defined in the schema.
  // Note that you do not need care about id attribute.
  public $created;
  public $title;
  public $body;

  const TABLE = 'sdbo_example_objects';

  function getTable() {
    return self::TABLE;
  }
}

// /*End code*/

Create new object
--------------------------------------------------------------------------------

// /*Code*/

$object = SDBFactory::initInstanceOf('SDBExampleChild', $form_state['values']);
$object->save();

// /*End code*/

Update existing object
--------------------------------------------------------------------------------

// /*Code*/

// Load from db.
$object = SDBFactory::loadInstanceOf('SDBExampleChild',
  array('id' => $form_state['values']['id']));
// Set new value for title attribute.
$object->title = $form_state['values']['title'];
// Call save operation - the internal logic will handle updates/inserts.
$object->save();

// /*End code*/

Update existing object - more convenient way
--------------------------------------------------------------------------------

// /*Code*/

// Init object. Make sure $form_state['values'] contains primary key of the
// object that will be updated.
$object = SDBFactory::initInstanceOf('SDBExampleChild', $form_state['values'],
  NULL, TRUE);

// Call save operation - by setting 4th parameter of initInstanceOf to TRUE you
// explicitly tell the object is persisted and therefore SDBO will go for
// update.
$object->save();

// /*End code*/

Delete object
--------------------------------------------------------------------------------

// /*Code*/

// Load from db.
$object = SDBFactory::loadInstanceOf('SDBExampleChild',
  array('id' => $form_state['values']['id']));
// Call delete operation. This will query database with delete query as well as
// will remove the object from internal cache registry, so the object is no more
// available in the SDBFactory registry.
$object->delete();

// /*End code*/

================================================================================
ADVANCED USAGE
================================================================================
For advanced usage see the example module. The module provides five real life
examples that cover all the features listed above. You can access them on
/sdbo-example path It contains additional documentation and code examples that
actually get executed so you can follow changes in the db.

================================================================================
SPONSOR
================================================================================
This module is a product of blueMinds (http://blueminds.eu) web development
company.
