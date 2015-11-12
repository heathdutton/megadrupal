<?php
// $Id;

/**
 * @file
 *   Hooks for Views API integration
 */

/**
 * Implementation of hook_views_data().
 */
function lastfm_views_data() {
  $data = array();
  
  _lastfm_get_views_event_data($data);

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function lastfm_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'lastfm') . '/handlers',
    ),
    'handlers' => array(
      'lastfm_argument_text' => array(
        'parent' => 'views_handler_argument',
      ),
      'lastfm_argument_past_events' => array(
        'parent' => 'lastfm_argument_text',
      ),
      'lastfm_field_text' => array(
        'parent' => 'views_handler_field',
      ),
      'lastfm_field_description' => array(
        'parent' => 'lastfm_field_text',
      ),
      'lastfm_field_image' => array(
        'parent' => 'views_handler_field',
      ),
      'lastfm_field_venue_image' => array(
        'parent' => 'lastfm_field_image',
      ),
      'lastfm_field_date' => array(
        'parent' => 'views_handler_field_date',
      ),
      'lastfm_field_boolean' => array(
        'parent' => 'views_handler_field_boolean',
      ),
      'lastfm_filter_string' => array(
        'parent' => 'views_handler_filter_string',
      ),
      'lastfm_relationship' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function lastfm_views_plugins() {
  return array(
    'module' => 'lastfm',
    'query' => array(
      'lastfm_views_plugin_query' => array(
        'title' => t('Last.fm Query'),
        'help' => t('Last.fm API query object.'),
        'handler' => 'lastfm_views_plugin_query',
        'path' => drupal_get_path('module', 'lastfm') .'/plugins',
      ),
    ),
    'argument validator' => array(
      'lastfm_events' => array(
        'title' => t('Last.fm event arguments'),
        'handler' => 'lastfm_views_plugin_argument_validate_events',
        'path' => drupal_get_path('module', 'lastfm') .'/plugins',
      ),
    ),
  );
}

function _lastfm_get_views_event_data(&$data) {
  // Base data
  $data['lastfm_events']['table']['group']  = t('Last.fm');
  $data['lastfm_events']['table']['base'] = array(
    'title' => t('Last.fm Events'),
    'help' => t('Query Last.fm events.'),
    'query class' => 'lastfm_views_plugin_query'
  );
  
  // Arguments
  $data['lastfm_events']['artist'] = array(
    'title' => t('Artist'),
    'help' => t('The name of the artist whose events you want to show.'),
    'argument' => array(
      'handler' => 'lastfm_argument_past_events',
      'lastfm' => array(
        'arg' => 'artist',
        'method' => 'artist.getevents',
        'past_events_method' => 'artist.getpastevents',
      ),
    ), 
    'filter' => array(
      'handler' => 'lastfm_filter_string',
      'lastfm' => array(
        'arg' => 'artist',
        'method' => 'artist.getevents',
       ),
    ),
    'relationship' => array(
      'handler' => 'lastfm_relationship',
      'base' => 'lastfm_artists',
      'lastfm' => array(
        'arg' => 'artist',
        'method' => 'artist.getinfo',
       ),
      'label' => t('artist'),
    ),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['user'] = array(
    'title' => t('User'),
    'help' => t('The last.fm username of the user whose events you want to show.'),
    'argument' => array(
      'handler' => 'lastfm_argument_past_events',
      'lastfm' => array(
        'arg' => 'user',
        'method' => 'user.getevents',
        'past_events_method' => 'user.getpastevents',
       ),
    ),
    'filter' => array(
      'handler' => 'lastfm_filter_string',
      'lastfm' => array(
        'arg' => 'user',
        'method' => 'user.getevents',
       ),
    ),
  );
  $data['lastfm_events']['venue'] = array(
    'title' => t('Venue'),
    'help' => t('The last.fm Venue ID of the venue whose events you want to show.'),
    'argument' => array(
      'handler' => 'lastfm_argument_past_events',
      'lastfm' => array(
        'arg' => 'venue',
        'method' => 'venue.getevents',
        'past_events_method' => 'venue.getpastevents',
       ),
    ),
  );
  $data['lastfm_events']['location'] = array(
    'title' => t('Location'),
    'help' => t('The location where events are taking place. This argument returns nearby events unless combined with distance.'),
    'argument' => array(
      'handler' => 'lastfm_argument_text',
      'lastfm' => array(
        'arg' => 'location',
        'method' => 'geo.getevents',
       ),
    ),
    'filter' => array(
      'handler' => 'lastfm_filter_string',
      'lastfm' => array(
        'arg' => 'location',
        'method' => 'geo.getevents',
       ),
    ),
  );
  $data['lastfm_events']['lat'] = array(
    'title' => t('Latitude'),
    'help' => t('The latitude where events are taking place. Requires being combined with longitude. Lat/long returns nearby events unless combined with distance.'),
    'argument' => array(
      'handler' => 'lastfm_argument_text',
      'lastfm' => array(
        'arg' => 'lat',
        'method' => 'geo.getevents',
       ),
    ),
    'filter' => array(
      'handler' => 'lastfm_filter_string',
      'lastfm' => array(
        'arg' => 'lat',
        'method' => 'geo.getevents',
       ),
    ),
  );
  $data['lastfm_events']['long'] = array(
    'title' => t('Longitude'),
    'help' => t('The longitude where events are taking place. Requires being combined with latitude. Lat/long returns nearby events unless combined with distance.'),
    'argument' => array(
      'handler' => 'lastfm_argument_text',
      'lastfm' => array(
        'arg' => 'long',
        'method' => 'geo.getevents',
       ),
    ),
    'filter' => array(
      'handler' => 'lastfm_filter_string',
      'lastfm' => array(
        'arg' => 'long',
        'method' => 'geo.getevents',
       ),
    ),
  );
  $data['lastfm_events']['distance'] = array(
    'title' => t('Distance'),
    'help' => t('Find more events within a specified radius measured in kilometers.'),
    'argument' => array(
      'handler' => 'lastfm_argument_text',
      'lastfm' => array(
        'arg' => 'distance',
        'method' => 'geo.getevents',
       ),
    ),
    'filter' => array(
      'handler' => 'lastfm_filter_string',
      'lastfm' => array(
        'arg' => 'distance',
        'method' => 'geo.getevents',
       ),
    ),
  );
 
  // Fields 
  $data['lastfm_events']['title'] = array(
    'title' => t('Event name'),
    'help' => t('The name of this event.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['url'] = array(
    'title' => t('URL'),
    'help' => t('URL to this event on last.fm.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['id'] = array(
    'title' => t('Event ID'),
    'help' => t('The last.fm event ID.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['description'] = array(
    'title' => t('Description'),
    'help' => t('A text description of this event. Often contains HTML with last.fm-specific class names.'),
    'field' => array(
      'handler' => 'lastfm_field_description',
    ),
  );
  $data['lastfm_events']['attendance'] = array(
    'title' => t('Attendance'),
    'help' => t('The number of last.fm users indicating they are attending this event.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['reviews'] = array(
    'title' => t('Reviews'),
    'help' => t('The number of reviews this event has received on last.fm.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['tag'] = array(
    'title' => t('Machine tag'),
    'help' => t('The event\'s last.fm machine tag.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['website'] = array(
    'title' => t('Website'),
    'help' => t('The event\'s website.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_id'] = array(
    'title' => t('Venue Id'),
    'help' => t('The venue\'s ID on last.fm.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_name'] = array(
    'title' => t('Venue name'),
    'help' => t('The name of the venue.'),
    'real field' => 'venue->name',
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_url'] = array(
    'title' => t('Venue URL'),
    'help' => t('The venue\'s URL on last.fm.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_website'] = array(
    'title' => t('Venue website'),
    'help' => t('The venue\'s website.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_phonenumber'] = array(
    'title' => t('Venue phone number'),
    'help' => t('The venue\'s phone number.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_city'] = array(
    'title' => t('Venue city'),
    'help' => t('The venue\'s city.'),
    'real field' => 'venue->location->city',
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_street'] = array(
    'title' => t('Venue street address'),
    'help' => t('The venue\'s street address.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_country'] = array(
    'title' => t('Venue country'),
    'help' => t('The venue\'s country.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_postalcode'] = array(
    'title' => t('Venue postal code'),
    'help' => t('The venue\'s postal code.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_latitude'] = array(
    'title' => t('Venue latitude'),
    'help' => t('The venue\'s latitude coordinate.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['venue_longitude'] = array(
    'title' => t('Venue longitude'),
    'help' => t('The venue\'s longitude coordinate.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['event_image'] = array(
    'title' => t('Event Image'),
    'help' => t('Image for the event.'),
    'field' => array(
      'handler' => 'lastfm_field_image',
      'lastfm' => array(
        'image_type' => 'event_image',
       ),
    ),
  );
  $data['lastfm_events']['venue_image'] = array(
    'title' => t('Venue Image'),
    'help' => t('Image for the event\'s venue.'),
    'field' => array(
      'handler' => 'lastfm_field_venue_image',
      'lastfm' => array(
        'image_type' => 'venue_image',
       ),
    ),
  );
  $data['lastfm_events']['startDate'] = array(
    'title' => t('Date'),
    'help' => t('The date of the this event.'),
    'field' => array(
      'handler' => 'lastfm_field_date',
    ),
  );
  $data['lastfm_events']['ticket_supplier'] = array(
    'title' => t('Ticket supplier'),
    'help' => t('The vendor providing tickets for this event.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['ticket_url'] = array(
    'title' => t('Ticket URL'),
    'help' => t('The URL to buy tickets for this event.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['cancelled'] = array(
    'title' => t('Cancelled'),
    'help' => t('Whether or not this event is canceled.'),
    'field' => array(
      'handler' => 'lastfm_field_boolean',
    ),
  );

  $data['lastfm_artists']['table']['group']  = t('Last.fm');
  $data['lastfm_artists']['table']['base'] = array(
    'title' => t('Last.fm Artists'),
    'help' => t('Query Last.fm artists.'),
    'query class' => 'lastfm_views_plugin_query'
  );

  $data['lastfm_artists']['name'] = array(
    'title' => t('Artist Name'),
    'help' => t('The name of this artist.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
  $data['lastfm_events']['bio'] = array(
    'title' => t('Artist Bio'),
    'help' => t('The biography of this artist.'),
    'field' => array(
      'handler' => 'lastfm_field_text',
    ),
  );
}
