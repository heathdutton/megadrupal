Creating Custom Entity Types with Mongo Entity
===

Mongo Entity works with custom entity types. Developers should use
`hook_entity_info` to declare their entity type, and configure it as described
below to store their entities in a Mongo collection.

### Entity and controller classes
Use the `MongoEntity` and `MongoEntityController` classes as the 'entity class'
and 'controller class' values, respectively.

### Collection name
Mongo Entity will automatically create a collection in your Mongo database
using the entity type as the name. Alternatively, you can set the `collection`
value to specify a different Mongo collection.

### Entity keys
Mongo collections require documents to have a `_id` field. This will always be
the value for the `id` entity key. It is not necessary to set this. The `label`
property will be saved with the entity.

### Bundles
To enable bundles, set the 'bundleable' key to TRUE in hook_entity_info. The bundle key
will always be '_bundle'.

### Revisions
TK

### Indexes
For performance optimization, set the `indexes` value to an array of indexes
for the collection. Each item in the array will be passed to
[MongoCollection::ensureIndex](http://php.net/manual/en/mongocollection.ensureindex.php).

    'indexes' => array(
      array('name' => 1),
      array('user' => 1),
    ),

`indexes` also supports options with a multidimensional array. To create a unique
index, pass the keys as the first element of the array, and the options as the second.

    'indexes' => array(
      array(array('name' => 1), array('unique' => TRUE)),
      array('user' => 1),
    ),

This creates two indexes, the first of which is unique.

### Embedded entities
Embedded entities can automatically be saved and loaded with their parent entity.
The `embedded entities` value should be an array entity types, keyed by the
name of the property that stores the array of entities.

Example:

    function bookstore_entity_info() {
      return array(
        'book' => array(
          'label' => t('Book'),
          'entity class' => 'MongoEntity',
          'controller class' => 'MongoEntityController',
          'embedded entities' => array(
            'chapters' => 'chapter',
          ),
          'load hook' => 'book_load',
          'uri callback' => 'book_uri',
          'fieldable' => TRUE,
          'entity keys' => array(
            'label' => 'title',
          ),
        ),
        'chapter' => array(
          'label' => t('Chapter'),
          'entity class' => 'MongoEmbeddedEntity',
          'controller class' => 'MongoEmbeddedEntityController',
          'collection' => 'book',
          'collection field' => 'chapters',
          'entity keys' => array(
            'label' => 'title',
            'parent' => 'book',
          ),
        ),
      )
    }

### EntityFieldQuery with embedded entity fields
To add a fieldCondition that queries a field on an embedded entity, pass the field names as an array

    $entity->fieldCondition(array('field_embedded_entity', 'field_name'), 'value', 'Hello World'));

This queries the 'field_name' field of the 'field_embedded_entity' embedded entity field.

Storing Custom Properties
===

Mongo Entity will write any declared class properties in its `entity class`
into the collection's document. To add properties to your entity, simply
override the `MongoEntity` class and declare your properites.

Example:

    class BookMongoEntity extends MongoEntity {

      // $title would be saved automatically, because it is the property name set in $entityInfo['entity keys']['label']
      public $title;
      public $isbn;
      public $author;

    }


Using Embedded Entities
===

TK
