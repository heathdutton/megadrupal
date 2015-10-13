<?php
/**
 * @file
 * Extension for freebase_api - Retrieves Freebase object schemas and
 * converts them to Drupal content types.
 *
 * $schema_definition = freebase_api_fetch_schema_definition('Person/Person');
 *
 * Convert it to something like a node bundle definition.
 * You probably want to filter the values a bit at this point, as the full
 * freebase schema often has too many fields you don't want.
 *
 * $content  = freebase_api_schema_to_cck_content_type($schema_definition);
 *
 * $content is now an array as used by bundle import.
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 * @version 2010
 */

/**
 * Retrieves the schema definition for the given ID.
 *
 * Make a service lookup on the freebase server.
 *
 * @param string $type_id
 *   A Freebase path identifying a Freebase type, such as
 * '/book/author'
 *
 * We will use and get returned qualified (namespaced) values
 *
 * @return array
 *   A schema data object, parsed from the returned JSON. Probably nested
 * arrays of useful info. NULL on failure.
 */
function freebase_api_fetch_schema_definition($type_id) {
  if (empty($type_id)) {
    drupal_set_message("No type_id passed in to the freebase schema query", 'error');
    return NULL;
  }
  // Quick version:
  /*
  $flat_json =
  file_get_contents('https://www.googleapis.com/freebase/v1/topic' . $type_id);
  $fb_schema = json_decode($flat_json, TRUE);
  return $fb_schema;
  */

  // This is a query to retrieve certain useful things about a Freebase 'Type',
  // The properties it may support, as well as the 'included_types' the type
  // definition inherits from and their respective properties.
  //
  // Together, this becomes a class definition, including the expected datatypes
  // of the attributes themselves.

  // Beware trailing commas!!
  $query_string = '
{
  "/type/object/id": "' . $type_id . '",
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

}';
  // I can find the ID of the article to use as the description,
  // But cannot yet figure how to retrieve the article content
  // "/common/topic/article" : { "id" : null, "optional":"optional" }
  // almost worked, but I can't actually get at the text.

  // It seems that adding 'optional' to subqueries
  // (which otherwise have the effect of requiring at least one match)
  // makes them not break the lookup when there are no matches.
  // EG, there was a topic type that was only a collation of included types
  // with no properties on its own.
  // Making a lookup on saying we wanted results to include
  // properties meant NOTHING was returned, not even base data.
  // http://www.freebase.com/docs/mql/ch03.html#mqltutorial 3.5.5

  // Although we don't want or expect more than one /common/topic/article
  // (or documentation) we need to ask for it as an array or the query breaks
  // in rare cases.
  // eg /film/film had two articles attached to it.

  drupal_set_message("Making query to Freebase now...");

  try {
    $fb_schema = freebase_api_run_mql_query($query_string);
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    return NULL;
  }

  if (!$fb_schema) {
    drupal_set_message("No schema was returned from running query: " . '<pre>' . print_r($query_string, 1) . '</pre>', 'error');
  }

  // Retrieving fulltext descriptions requeres a second lookup,
  // using the blob retrieval service.
  if (isset($fb_schema['/common/topic/article'][0]['/type/object/id'])) {
    $description_text_guid = $fb_schema['/common/topic/article'][0]['/type/object/id'];
    if (!empty($description_text_guid)) {
      $fb_schema['description'] = $fb_schema['/common/topic/article'][0]['value'] = freebase_api_fetch_text($description_text_guid);
    }
  }

  return $fb_schema;
}


/**
 * Given a Freebase JSON string, convert it to a schema, and a content type def.
 *
 * Used if sourcing fb data from direct URL or upload.
 *
 * @param string $fb_json_string
 *   JSON as returned by Freebase schema lookup.
 *
 * @return array
 *   A $content type def array.
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
 * Fetches schema from FB and returns a content definition.
 *
 * Convert a set of type statements from freebase into a framework php type def
 * object as used for cck import etc.
 *
 * Returns a '$content' object that holds both type and fields - as fields
 * definitions are not nested within the type object itself, but parallal with
 * it. This is how cck export does it.
 *
 *
 * @param array $fb_schema
 *   A php array containing the results of the Freebase query on types.
 *
 * @return array
 *   Structure similar to that used by bundle_copy.
 */
function freebase_api_schema_to_cck_content_type($fb_schema) {
  if (empty($fb_schema)) {
    drupal_set_message('Null freebase schema to convert');
    return NULL;
  }

  $data = array();

  // Start with a placeholder bundle definition.
  $bundle = field_schema_importer_content_type_defaults($fb_schema['/type/object/name'], $fb_schema['/type/object/id']);
  $bundle_id = $bundle['type'];
  $entity_type = 'node';
  if (!empty($fb_schema['description'])) {
    $bundle['description'] = $fb_schema['description'];
  }
  // Bundle_copy needs this or it just refuses to play.
  $bundle['bc_entity_type'] = $entity_type;

  // Sneak more meta attributes in, We want to be like RDF.
  $rdf_mapping = array();
  $rdf_mapping['rdftype'] = array(
    freebase_api_freebase_id_to_rdf_curie($fb_schema['/type/object/id']),
  );
  // This value MAY be saved against the content type
  // if you also have rdf_mapping.module enabled.
  $bundle['rdf_mapping'] = $rdf_mapping;

  // $bundle['rdf_rdftype'] =
  // freebase_api_freebase_id_to_rdf_curie($fb_schema['/type/object/id']);
  // $bundle['uri'] =
  // 'http://freebase.com/type/schema'. $fb_schema['/type/object/id'];

  $bundle['freebase_mapping'] = $fb_schema['/type/object/id'];

  $data['bundles'][$bundle_id] = (object) $bundle;

  // Now the fields (instances).
  $data['fields'] = array();
  foreach ((array) $fb_schema['/type/type/properties'] as $fb_property) {
    list($field, $instance) = freebase_api_schema_to_cck_field($fb_property, $bundle_id);
    $data['fields'][$field['field_name']] = $field;
    $data['instances'][$field['field_name']][$bundle_id] = $instance;

    // Pull the RDF Mapping data up from the field and onto the bundle, because
    // that's where rdf.module may expect it to be.
    $data['bundles'][$bundle_id]->rdf_mapping[$instance['field_name']] = $instance['rdf_mapping'];
  }

  // Schema included types (inherited classes) will be made as fieldgroups
  // and their respective fields added to that group
  // This will closely mimic the freebase UI, as well as help manage repetition.
  foreach ((array) $fb_schema['/freebase/type_hints/included_types'] as $fb_type) {
    $group_name = freebase_api_machine_name($fb_type['/type/object/id']);
    $group_label = freebase_api_machine_name($fb_type['/type/object/name']);
    // Group is an object, says field_group.module.
    $group = field_schema_importer_group_defaults($group_label, $group_name, $entity_type, $bundle_id, 'form');
    $group_id = $group->identifier;
    foreach ($fb_type['/type/type/properties'] as $fb_property) {

      // Tweak: If it's a /common/topic/article
      // just set the body on instead and discard this field.
      if ($fb_property['/type/object/id'] == '/common/topic/article') {
        $content['body_label'] = $fb_property['/type/object/name'];
        continue;
      }

      list($field, $instance) = freebase_api_schema_to_cck_field($fb_property, $bundle_id);
      $data['fields'][$field['field_name']] = $field;
      $data['instances'][$field['field_name']][$bundle_id] = $instance;
      $group->children[] = $field['field_name'];

      $data['bundles'][$bundle_id]->rdf_mapping[$instance['field_name']] = $instance['rdf_mapping'];
    }
    $data['fieldgroups'][$group_id] = $group;
  }

  return $data;
}

/**
 * Transform a FB 'property' definition into a bundle_copy style field def.
 *
 * Note that bundle_copy treats fields and field instances separately.
 * However, all other schemas provide field definition info together,
 * so we have to build the field and instance at the same time.
 *
 * Most of the domain-specific logic is here.
 *
 * As well as the default properties, and the ones that I've discovered useful
 * for certain field types (eg an image field required default_image to be set)
 * I also add a fields 'rdf_mapping' info about the predicate being used here.
 * This needs to be copied up into the $bundle-rdf_mapping if it is to be
 * saved, as field_api doesn't deal with rdf predicates.
 *
 * @see freebase_api_schema_to_cck_content_type()
 * @see field_schema_importer_field_defaults()
 * @see field_schema_importer_field_instance_defaults()
 *
 * @param array $fb_property
 *   Freebase property definition from the schema array.
 * @param string $bundle_id
 *   Bundle ID.
 *
 * @return array
 *   ($field, $instance)
 *   A field definition AND a field instance. NULL if invalid or unwanted.
 */
function freebase_api_schema_to_cck_field($fb_property, $bundle_id) {

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
  $field_id = freebase_api_machine_name($id);
  $expected_type = $fb_property['/type/property/expected_type'];

  $node_types = node_type_get_types();

  // START BUILDING the field definitions.
  $field = field_schema_importer_field_defaults($field_id);
  $instance = field_schema_importer_field_instance_defaults($field_id, $bundle_id);

  // Additional non-default settings, eg
  // $field['field_name'] = freebase_api_machine_name($id);
  $instance['label'] = $fb_property['/type/object/name'];
  $instance += array(
    // Sneak more meta attributes in.
    // These are not cck proper, but come along for the ride
    // for the benefit of other tools.
    'uri'           => 'http://freebase.com/type/schema' . $id,
    'rdf_mapping'   => array(
      'predicates' => array(freebase_api_freebase_id_to_rdf_curie($fb_property['/type/object/id'])),
      'datatype'   => array(freebase_api_freebase_id_to_rdf_curie($expected_type)),
    ),
    'expected_type' => freebase_api_machine_name($expected_type),
  );

  // Some other required properties will be filled in during validation.

  // Do some extra massaging for known Freebase properties..

  // Assume multiple unless explicitly 'unique'
  if (isset($fb_property['/type/property/unique']) && $fb_property['/type/property/unique'] === TRUE) {
    $field['cardinality'] = 1;
  }
  else {
    $field['cardinality'] = -1;
  }

  // Pretty much everything has a 'referenceable type', as most data objects
  // in freebase are links to more structured data.
  // Set the expected referenceable_type as a hint to the user,
  // even if we can't always use it.
  // This is a field attribute of our own, not part of Drupal Field API.
  if ($expected_type) {
    $referenceable_type = freebase_api_machine_name($expected_type);
    $field['referenceable_types'] = array(
      freebase_api_machine_name($referenceable_type) => 'http://www.freebase.com' . $expected_type,
    );
  }

  // UNIQUE PROPERTIES.

  // All freebase 'types' are
  // http://www.freebase.com/app/queryeditor/?q=[{%20%22type%22:%20%22/type/type%22,%20%22name%22:%20null,%20%22id%22:%20null%20}]
  // eg:
  // /common/topic - should sometimes become a noderef
  // /common/image, /common/webpage
  // /type/int , /type/uri, /type/datetime

  // If we do not have enough widgets turned on (for date, or filefield)
  // the suggestion here will still be for the *desired* type we want to see,
  // but before continuing, the UI validator may replace all the unavailable
  // types with "text".
  // To review, it's not my job to fill in the widget here!
  // Some later validator should add the defaults.

  // Further module and widget settings are required later on,
  // but will be filled in automatically by the validation process.

  switch ($expected_type) {

    case '/type/datetime':
      $field['type'] = 'datetime';
      $instance['widget']['type'] = 'date_select';
      break;

    case '/common/webpage':
      $field['multiple'] = FALSE;
      $field['type'] = 'link_field';
      $instance['widget']['type'] = 'link';
      break;

    case '/common/weblink':
      $field['type'] = 'link_field';
      $instance['widget']['type'] = 'link';
      break;

    case '/common/image':
      $field['type'] = 'image';
      $field['module'] = 'image';
      $instance['widget']['type'] = 'image_image';
      $instance['widget']['module'] = 'image';
      $instance['settings']['file_extensions'] = 'png gif jpg jpeg';
      $instance['default_image'] = NULL;
      break;

    case '/type/float':
      $field['type'] = 'number_decimal';
      $instance['widget']['type'] = 'number';
      break;

    case '/type/int':
      $field['type'] = 'number_integer';
      $instance['widget']['type'] = 'number';
      break;

    case '/type/text':
      $field['type'] = 'text';
      $instance['widget']['type'] = 'text_textfield';
      break;

    case '/type/enumeration':
      $field['type'] = 'text';
      $instance['widget']['type'] = 'optionwidgets_select';
      break;

    default:
      // References to other value types are PROBABLY expected to become
      // references to other data objects. That's great, but may get hard.
      if (!@empty($node_types[$referenceable_type])) {
        // YAY. An appropriate target content type exists already.
        $field['type'] = 'entityreference';
        $instance['widget']['type'] = 'entityreference_autocomplete';
      }
      else {
        // Default to text, and let them sort it out later
        // They can reset this to a nodereference to one of their custom
        // content types if they wish.
        $field['type'] = 'text';
        $instance['widget']['type'] = 'text_textfield';
      }
      break;
  }

  // Also, tweak some known issues out,
  // turning these boring default fields off by default.
  // This runs after the basic field settings have gone OK.
  switch ($fb_property['/type/object/id']) {
    case '/common/topic/subjects':
      // This is often better handled by taxonomy.
    case '/common/topic/subject_of':
      // This is useless.
    case '/common/topic/properties':
      // This is unusable. attributes_field?
    case '/common/topic/weblink':
      // This is a duplicate of webpage?
    case '/common/topic/alias':
      // We are not going to use this.
    case '/common/topic/notable_for':
    case '/common/topic/notable_types':
    case '/common/topic/topical_webpage':
    case '/common/topic/official_website':
    case '/common/topic/topic_equivalent_webpage':
    case '/common/topic/social_media_presence':
      // These get added to everything.

      $instance['create'] = FALSE;
      break;
  }
  // Also a common number of items I want to turn off all the time end in _id
  // eg people_person_tvrage_id film_actor_netflix_id etc
  // Turn them off by default.
  if (preg_match('@_id$@', $fb_property['/type/object/id'])) {
    $instance['create'] = FALSE;
  }

  return array($field, $instance);
}

