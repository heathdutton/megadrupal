<?php
/**
 * @file
 * fb_autopost_types.features.inc
 */

/**
 * Implements hook_default_facebook_publication_type().
 */
function fb_autopost_types_default_facebook_publication_type() {
  $items = array();
  $items['album'] = entity_import('facebook_publication_type', '{
    "type" : "album",
    "label" : "Album",
    "weight" : "0",
    "description" : "",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['event'] = entity_import('facebook_publication_type', '{
    "type" : "event",
    "label" : "Event",
    "weight" : "0",
    "description" : "Specifies information about an event, including the location, event name, and which invitees plan to attend. The User, Page, and A\\ufeffpplication objects have an events connection.\\r\\n\\r\\nhttp:\\/\\/developers.facebook.com\\/docs\\/reference\\/api\\/event\\/",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['link'] = entity_import('facebook_publication_type', '{
    "type" : "link",
    "label" : "Link",
    "weight" : "0",
    "description" : "You can post a link on a Page by issuing an HTTP POST request to PAGE_ID\\/feed with the publish_actions and manage_pages permissions and the following parameters.",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['note'] = entity_import('facebook_publication_type', '{
    "type" : "note",
    "label" : "Note",
    "weight" : "0",
    "description" : "You can create a note on a Page by issuing an HTTP POST request to PAGE_ID\\/notes with the publish_actions and manage_pages permissions and the following parameters.",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['photo'] = entity_import('facebook_publication_type', '{
    "type" : "photo",
    "label" : "Photo",
    "weight" : "0",
    "description" : "You can post photos to a Page\\u0027s Wall by issuing an HTTP POST request to PAGE_ID\\/photos with the publish_actions and manage_pages permissions and the following parameters.",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['post'] = entity_import('facebook_publication_type', '{
    "type" : "post",
    "label" : "Post",
    "weight" : "0",
    "description" : "You can create a post on a Page by issuing an HTTP POST request to PAGE_ID\\/feed with the publish_actions and manage_pages permissions and the following parameters. Additionally, Page posts can also be scheduled to go live at a future time up to 6 months or can be created in an \\u0022unpublished\\u0022 state where they are not visible on the pages timeline or in the page fan\\u0027s news feeds. Unpublished posts can be used as Sponsored Stories.",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['question'] = entity_import('facebook_publication_type', '{
    "type" : "question",
    "label" : "Question",
    "weight" : "0",
    "description" : "You can post a question on behalf of the Page by issuing an HTTP POST request to PROFILE_ID\\/questions with the publish_actions permissions and the following parameter",
    "data" : null,
    "rdf_mapping" : []
  }');
  $items['status'] = entity_import('facebook_publication_type', '{
    "type" : "status",
    "label" : "Status",
    "weight" : "0",
    "description" : "You can post a status message on a Page by issuing an HTTP POST request to PAGE_ID\\/feed with the publish_actions and manage_pages permissions and the following parameters.",
    "data" : null,
    "rdf_mapping" : []
  }');
  return $items;
}
