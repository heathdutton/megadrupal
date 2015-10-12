<?php
/**
 * @file Extension for freebase_api - Retrieves Freebase object schemas and
 * converts them to Drupal content types.
 *
 * $schema_definition = freebase_api_fetch_schema_definition('Person/Person');
 *
 * Convert it to something like a cck definition.
 * You probably want to filter the values a bit at this point, as the full
 * freebase schema often has too many fields you don't want.
 *
 * $content  = freebase_api_schema_to_cck_content_type($schema_definition);
 *
 * $content is now an array as used by CCK import.
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 * @version 2010
 */

/**
 * Make a service lookup on the freebase server, retrieving the schema
 * definition for the given ID.
 *
 * @param $type_id will be a Freebase path identifying a Freebase type, such as
 * '/book/author'
 *
 * We will use and get returned qualified (namespaced) values
 *
 * @return a schema data object, parsed from the returned JSON. Probably nested
 * arrays of useful info. NULL on failure.
 */
function freebase_api_fetch_schema_definition($type_id) {
  if (empty($type_id)) {
    drupal_set_message("No type_id passed in to the freebase schema query", 'error');
    return NULL;
  }
  // This is a query to retrieve certain useful things about a Freebase 'Type',
  // The properties it may support, as well as the 'included_types' the type
  // definition inherits from and their respective properties.
  //
  // Together, this becomes a class definition, including the expected datatypes
  // of the attributes themselves.

  // Beware trailing commas!!
  $query_string = '
[{
  "/type/object/id": "'. $type_id .'",
  "/type/object/name": null,
  "/type/object/type": "/type/type",
  "/type/type/properties": [{
    "/type/object/id": null,
    "/type/object/name": null,
    "/type/property/expected_type": null,
    "/type/property/reverse_property": null,
    "/type/property/unique": null,
    "optional":"optional"
  }],
  "/freebase/type_hints/included_types": [{
    "/type/object/id": null,
    "/type/object/name": null,
    "/type/property/expected_type": null,
    "/type/type/properties": [{
      "/type/object/id": null,
      "/type/object/name": null,
      "/type/property/expected_type": null,
      "/type/property/reverse_property": null,
      "/type/property/unique": null,
      "optional":"optional"
    }],
    "optional":"optional"
  }],

  "/common/topic/article" : [{
    "/type/object/id" : null,
    "optional":"optional"
  }],

  "/freebase/documented_object/documentation": [{
    "/common/document/content" : null,
    "optional":"optional"
  }]

}]';
  // I can find the ID of the article to use as the description,
  // But cannot yet figure how to retrieve the article content
  // "/common/topic/article" : { "id" : null, "optional":"optional" }
  // almost worked, but I can't actually get at the text

  // It seems that adding 'optional' to subqueries
  // (which otherwise have the effect of requiring at least one match)
  // makes them not break the lookup when there are no matches.
  // EG, there was a topic type that was only a collation of included types with no
  // properties on its own. Making a lookup on saying we wanted results to include
  // properties meant NOTHING was returned, not even base data.
  // http://www.freebase.com/docs/mql/ch03.html#mqltutorial 3.5.5

  // Although we don't want or expect more than one /common/topic/article
  // (or documentation) we need to ask for it as an array or the query breaks
  // in rare cases.
  // eg /film/film had two articles attached to it.

  drupal_set_message("Making query to Freebase now!");
  $fb_schema = freebase_api_run_freebase_query($query_string, TRUE) ;
  if (! $fb_schema ) {
    drupal_set_message("No schema was returned from running query: " . '<pre>' . print_r($query_string, 1) . '</pre>', 'error');
  }

  // Retrieving fulltext descriptions requeres a second lookup,
  // using the blob retrieval service.
  if (isset($fb_schema['/freebase/documented_object/documentation']['/common/document/content'])) {
    $description_text_guid = $fb_schema['/freebase/documented_object/documentation']['/common/document/content'];
  }
  elseif (isset($fb_schema['/common/topic/article']['/type/object/id'])) {
    $description_text_guid = $fb_schema['/common/topic/article']['/type/object/id'];
  }
  if (! empty($description_text_guid)) {
    $fb_schema['description'] = freebase_api_fetch_trans('raw', $description_text_guid) ;
  }

  return $fb_schema;
}

/**
 * Given a JSON string, as from Freebase convert it to a schema, and thence to a
 * CCK content type def.
 *
 * Used if sourcing fb data from direct URL or upload.
 *
 * @return a $content type def array.
 */
function freebase_api_json_to_cck_content_type($fb_json_string) {
  $fb_schema = json_decode($fb_json_string);

  if (empty($fb_schema)) {
    drupal_set_message('Failed to parse input as JSON', 'error');
    return NULL;
  }

  return freebase_api_schema_to_cck_content_type($fb_schema);
}

/**
 * Convert a set of type statements from freebase into a framework php type def
 * object as used for cck import etc
 *
 * Returns a '$content' object that holds both type and fields - as fields
 * definitions are not nested within the type object itself, but parallal with
 * it. This is how cck export does it.
 *
 *
 * @param $fb_schema a php array containing the results of the Freebase query on
 * types.
 * @return $content array similar to that used by CCK content_copy
 */
function freebase_api_schema_to_cck_content_type($fb_schema) {
  if (empty($fb_schema)) {
    drupal_set_message('Null freebase schema to convert');
    return NULL;
  }

  module_load_include('inc', 'content', 'includes/content.admin');
  module_load_include('inc', 'content', 'includes/content.crud');

  $content = array();
  $type = field_schema_importer_content_type_defaults($fb_schema['/type/object/name'], $fb_schema['/type/object/id']);
  if (!empty($fb_schema['description'])) {
    $type['description'] = $fb_schema['description'];
  }
  $content['type'] = $type;
  // Sneak more meta attributes in, We want to be like RDF
  $content['type']['rdf_rdftype'] = freebase_api_freebase_id_to_rdf_curie($fb_schema['/type/object/id']);
  // This value MAY be saved against the content type if you also have rdf_mapping.module enabled.

  $content['type']['uri'] = 'http://freebase.com/type/schema'. $fb_schema['/type/object/id'];
  $content['type']['freebase_mapping'] = $fb_schema['/type/object/id'];

  // Now the fields
  $content['fields'] = array();
  foreach ((array)$fb_schema['/type/type/properties'] as $fb_property) {
    $field = freebase_api_schema_to_cck_field($fb_property);
    if ($field) {
      $content['fields'][] = $field;
    }
  }

  // Schema included types (inherited classes) will be made as fieldgroups
  // and their respective fields added to that group
  // This will closely mimic the freebase UI, as well as help manage repetition.
  foreach ((array)$fb_schema['/freebase/type_hints/included_types'] as $fb_type) {
    $group_name = freebase_api_machine_name($fb_type['/type/object/id']);
    $group = field_schema_importer_group_defaults($fb_type['/type/object/name'], $group_name);
    $content['groups'][$group_name]  = $group;
    foreach ($fb_type['/type/type/properties'] as $fb_property) {

       // Tweak: If it's a /common/topic/article
       // just set the body on instead and discard this field
       if ($fb_property['/type/object/id'] == '/common/topic/article') {
         $content['body_label'] = $fb_property['/type/object/name'];
         continue;
       }

       $grouped_field = freebase_api_schema_to_cck_field($fb_property);
       if ($grouped_field) {
         $grouped_field['group'] = $group_name;
         $content['fields'][] = $grouped_field;
       }
    }
  }
  return $content;
}

/**
 * Transform a freebase 'property' definition into a cck field as used by cck
 * import.
 *
 * Most of the domain-specific logic is here.
 *
 * @ingroup cck_schema format conversion
 * @see freebase_api_schema_to_cck_content_type()
 *
 * @return a CCK field definition. NULL if invalid or unwanted.
 */
function freebase_api_schema_to_cck_field($fb_property) {

  // Hack to trim some Freebase noise - specifically
  // /type/object/id: "/m/0cc4bm5"xp
  // /type/object/name: "Notable professions"
  // Which seems to get bundled with every topic.
  // Looks like a bug. 2010-09
  $id = $fb_property['/type/object/id'];
  if (substr($id, 0, 3) == '/m/') {
    // Discard this un-ratified field
    return NULL;
  }

  $expected_type = $fb_property['/type/property/expected_type'];
  $node_types = node_get_types();

  // START BUILDING the cck field definition
  $field = array(
    'label' => $fb_property['/type/object/name'],
    'field_name' => freebase_api_machine_name($id),
    // Sneak more meta attributes in
    // These are not cck proper, but come along for the ride for the benefit of other tools.
    'uri' => 'http://freebase.com/type/schema'. $id,
    'rdf_mapping' => freebase_api_freebase_id_to_rdf_curie($fb_property['/type/object/id']),
    'expected_type' => freebase_api_machine_name($expected_type),
    'datatype' => array(freebase_api_freebase_id_to_rdf_curie($expected_type)),
  );
  // Other required properties will be filled in during validation

  #dpm(get_defined_vars());
  // Do some extra massaging for Known Freebase topics

  // Assume multiple unless explicitly 'unique'
  if (isset($fb_property['/type/property/unique']) && $fb_property['/type/property/unique'] === TRUE) {
    $field['multiple'] = FALSE;
  }
  else {
    $field['multiple'] = TRUE;
  }

  // Pretty much everything has a 'referenceable type', as most data objects
  // in freebase are links to more structured data.
  // Set the expected referenceable_type as a hint to the user,
  // even if we can't always use it
  if ($expected_type) {
    $referenceable_type = freebase_api_machine_name($expected_type);
    $field['referenceable_types'] = array(
      #freebase_api_machine_name($referenceable_type) => 'http://freebase.com/type/schema'. $expected_type,
      freebase_api_machine_name($referenceable_type) => freebase_api_freebase_id_to_rdf_curie($expected_type),
    );
  }

  // UNIQUE PROPERTIES

  // All freebase 'types' are
  // http://www.freebase.com/app/queryeditor/?q=[{%20%22type%22:%20%22/type/type%22,%20%22name%22:%20null,%20%22id%22:%20null%20}]
  // eg:
  // /common/topic - should sometimes become a noderef
  // /common/image, /common/webpage
  // /type/int , /type/uri, /type/datetime

  // If we do not have enough widgets turned on (for date, or filefield)
  // the suggestion here will still be for the *desired* type we want to see,
  // but before continuing, the cck_importer may replace all the unavailable
  // types with "text".

  // Module and widget_module settings are required later on, but will be filled in automatically
  // by the validation process

  switch ($expected_type) {

    case '/type/datetime' :
      $field += array(
        'type' => 'datetime',
        'widget_type' => 'date_select',
      );
      break;

    case '/common/webpage' :
      $field['multiple'] = FALSE;
    case '/common/weblink' :
      $field += array(
        'type' => 'link',
        'widget_type' => 'link',
      );
      break;

    case '/common/image' :
      $field += array(
        'type' => 'filefield',
        'widget_type' => 'imagefield_widget',
        'file_extensions' => 'png gif jpg jpeg',
      );
      break;

    case '/type/float' :
      $field += array(
        'type' => 'number_decimal',
        'widget_type' => 'number',
      );

    case '/type/int' :
      $field += array(
        'type' => 'number_integer',
        'widget_type' => 'number',
      );

    case '/type/text' :
      $field += array(
        'type' => 'text',
        'widget_type' => 'text_textfield',
      );
      break;

    case '/type/enumeration' :
      $field += array(
      'type' => 'text',
      'widget_type' => 'optionwidgets_select',
      );
      break;

    default :
      // References to other value types are PROBABLY expected to become
      // references to other data objects. That's great, but may get hard.
      if (!@empty($node_types[$referenceable_type])) {
        // YAY. An appropriate target content type exists already
        $field += array(
          'type' => 'nodereference',
          'widget_type' => 'nodereference_autocomplete',
        );
      }
      else {
        // Default to text, and let them sort it out later
        // They can reset this to a nodereference to one of their custom
        // content types if they wish
        $field += array(
          'type' => 'text',
          'widget_type' => 'text_textfield',
        );
      }
      break;
  }

  // Also, tweak some known issues out,
  // turning these boring default fields off by default.
  // This runs after the basic field settings have gone OK
  switch ($fb_property['/type/object/id']) {
    case '/common/topic/subjects' :
      // This is often better handled by taxonomy
    case '/common/topic/subject_of' :
      // This is useless
    case '/common/topic/properties' :
      // This is unusable. attributes_field?
    case '/common/topic/weblink' :
      // This is a duplicate of webpage?
    case '/common/topic/alias' :
      // We are not going to use this
    case '/common/topic/notable_for' :
    case '/common/topic/notable_types' :
    case '/common/topic/topical_webpage' :
    case '/common/topic/official_website' :
    case '/common/topic/official_website' :
    case '/common/topic/topic_equivalent_webpage' :
    case '/common/topic/social_media_presence' :

      // These get added to everything.

      $field['create'] = FALSE;
      break;
  }
  // Also a common number of items I want to turn off all the time end in _id
  // eg people_person_tvrage_id film_actor_netflix_id etc
  // Turn them off by default.
  if (preg_match('@_id$@', $fb_property['/type/object/id'])) {
    $field['create'] = FALSE;
  }

  return $field;
}

