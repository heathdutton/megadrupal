<?php

/**
 * @file
 * Providing extra functionality for the Eventbrite UI via views.
 */

/**
 * Implements hook_views_data()
 */
function eventbrite_api_views_data() {
  $data = array();

  /**
   * Integrate the events table.
   */
  $data['eventbrite_api_events']['table']['group'] = t('Eventbrite events');
  $data['eventbrite_api_events']['table']['entity type'] = 'event';

  $data['eventbrite_api_events']['table']['base'] = array(
    'field' => 'eid',
    'title' => t('Eventbrite Event'),
    'help' => t('Eventbrite Events.'),
  );

  // Expose the event ID.
  $data['eventbrite_api_events']['eid'] = array(
    'title' => t('Event ID'),
    'help' => t('The unique internal identifier of the event.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['eventbrite_api_events']['tid'] = array(
    'title' => t('Tickets'),
    'help' => t('The unique internal identifier of event tickets.'),
    'relationship' => array(
      'base' => 'eventbrite_api_tickets',
      'base field' => 'eid',
      'field' => 'eid',
      'handler' => 'views_handler_relationship',
      'label' => t('Event tickets'),
      'title' => t('Tickets'),
      'help' => t('The tickets associated with the event.'),
    ),
  );

  // Expose the organizer id.
  $data['eventbrite_api_events']['oid'] = array(
    'title' => t('Organizer ID'),
    'help' => t('The unique internal identifier of the event organizer.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'eventbrite_api_organizers',
      'base field' => 'oid',
      'label' => t('Event organizer'),
      'title' => t('Organizer'),
      'help' => t('The organizer associated with the event.'),
    ),
  );

  // Expose the venue id.
  $data['eventbrite_api_events']['vid'] = array(
    'title' => t('Venue ID'),
    'help' => t('The unique internal identifier of the event venue.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'eventbrite_api_venues',
      'base field' => 'vid',
      'label' => t('Event venue'),
      'title' => t('Venue'),
      'help' => t('The venue associated with the event.'),
    ),
  );

  if (module_exists('locale')) {
    // Expose the language.
    $data['eventbrite_api_events']['locale'] = array(
      'title' => t('Language'),
      'help' => t('The language the event is in.'),
      'field' => array(
        'handler' => 'views_handler_field_locale_language',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_locale_language',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_locale_language',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );
  }

  // Expose the timezone.
  $data['eventbrite_api_events']['timezone'] = array(
    'title' => t('Timezone'),
    'help' => t('The event timezone.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the category.
  $data['eventbrite_api_events']['category'] = array(
    'title' => t('Category'),
    'help' => t('The event category.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the event capacity.
  $data['eventbrite_api_events']['capacity'] = array(
    'title' => t('Capacity'),
    'help' => t('The capacity of the event.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the number of registered event attendees.
  $data['eventbrite_api_events']['num_attendee_rows'] = array(
    'title' => t('Registered attendees'),
    'help' => t('The umber of registered event attendees.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the event title.
  $data['eventbrite_api_events']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the event.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the event start date.
  $data['eventbrite_api_events']['start_date'] = array(
    'title' => t('Start date'),
    'help' => t('The date the event starts.'),
    'field' => array(
      'handler' => 'eventbrite_api_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'eventbrite_api_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Expose the event end date.
  $data['eventbrite_api_events']['end_date'] = array(
    'title' => t('End date'),
    'help' => t('The date the event ends.'),
    'field' => array(
      'handler' => 'eventbrite_api_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'eventbrite_api_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Expose the event description.
  $data['eventbrite_api_events']['description'] = array(
    'title' => t('Description'),
    'help' => t('The description of the event.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the event status.
  $data['eventbrite_api_events']['status'] = array(
    'title' => t('Status'),
    'help' => t('The event status.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the event tags.
  $data['eventbrite_api_events']['tags'] = array(
    'title' => t('Tags'),
    'help' => t('The event tags.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the event timezone offset.
  $data['eventbrite_api_events']['timezone_offset'] = array(
    'title' => t('Timezone offset'),
    'help' => t('The event timezone offset.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the event password.
  $data['eventbrite_api_events']['password'] = array(
    'title' => t('Password'),
    'help' => t('The event password.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the date the event was created.
  $data['eventbrite_api_events']['created'] = array(
    'title' => t('Created'),
    'help' => t('The date the event was created.'),
    'field' => array(
      'handler' => 'eventbrite_api_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'eventbrite_api_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Expose the event URL.
  $data['eventbrite_api_events']['url'] = array(
    'title' => t('URL'),
    'help' => t('The event URL.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the event privacy setting.
  $data['eventbrite_api_events']['privacy'] = array(
    'title' => t('Privacy'),
    'help' => t('Whether the event is public or private.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the date the event was modified.
  $data['eventbrite_api_events']['modified'] = array(
    'title' => t('Modified'),
    'help' => t('The date the event was modified.'),
    'field' => array(
      'handler' => 'eventbrite_api_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'eventbrite_api_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Expose the event repetition setting.
  $data['eventbrite_api_events']['repeats'] = array(
    'title' => t('Repeats'),
    'help' => t('Whether the event repeats or not.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose links to operate on the event.
  $data['eventbrite_api_events']['edit_event'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to the edit event page.'),
      'handler' => 'eventbrite_api_handler_field_event_link',
    ),
  );

  $data['eventbrite_api_events']['delete_event'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to the administrator view of the event.'),
      'handler' => 'eventbrite_api_handler_field_event_link_delete',
    ),
  );

  /**
   * Integrate the organizers table.
   */
  $data['eventbrite_api_organizers']['table']['group'] = t('Eventbrite organizer');
  $data['eventbrite_api_organizers']['table']['entity type'] = 'organizer';

  $data['eventbrite_api_organizers']['table']['base'] = array(
    'field' => 'oid',
    'title' => t('Eventbrite Organizers'),
    'help' => t('Organizers of Eventbrite events.'),
  );

  // Expose the organizer ID.
  $data['eventbrite_api_organizers']['oid'] = array(
    'title' => t('Organizer ID'),
    'help' => t('The unique internal identifier of the event organizer.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the organizer name.
  $data['eventbrite_api_organizers']['name'] = array(
    'title' => t('Name'),
    'help' => t('The name of the event organizer.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['eventbrite_api_organizers']['table']['default_relationship'] = array(
    'events' => array(
      'table' => 'events',
      'field' => 'oid',
    ),
  );

  // Expose the organizer description.
  $data['eventbrite_api_organizers']['description'] = array(
    'title' => t('Description'),
    'help' => t('The description of the event organizer.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the organizer long description.
  $data['eventbrite_api_organizers']['long_description'] = array(
    'title' => t('Long description'),
    'help' => t('The long description of the event organizer.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the organizer's URL.
  $data['eventbrite_api_organizers']['url'] = array(
    'title' => t('URL'),
    'help' => t('The URL of the event organizer.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose links to operate on the organizer.
  $data['eventbrite_api_organizers']['edit_organizer'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to the edit organizer page.'),
      'handler' => 'eventbrite_api_handler_field_organizer_link_edit',
    ),
  );

  /**
   * Integrate the tickets table.
   */
  $data['eventbrite_api_tickets']['table']['group'] = t('Eventbrite ticket');
  $data['eventbrite_api_tickets']['table']['entity type'] = 'ticket';

  $data['eventbrite_api_tickets']['table']['base'] = array(
    'field' => 'tid',
    'title' => t('Eventbrite Tickets'),
    'help' => t('Tickets for Eventbrite events.'),
    'defaults' => array(
      'field' => 'name',
    ),
  );

  $data['eventbrite_api_tickets']['table']['default_relationship'] = array(
    'events' => array(
      'table' => 'events',
      'field' => 'eid',
    ),
  );

  // Expose the ticket ID.
  $data['eventbrite_api_tickets']['tid'] = array(
    'title' => t('Ticket ID'),
    'help' => t('The unique internal identifier of the event ticket.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the event id.
  $data['eventbrite_api_tickets']['eid'] = array(
    'title' => t('Event ID'),
    'help' => t('The unique internal identifier of the event.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'eventbrite_api_events',
      'base field' => 'eid',
      'label' => t('Event'),
    ),
  );

  // Expose the ticket name.
  $data['eventbrite_api_tickets']['name'] = array(
    'title' => t('Name'),
    'help' => t('The name of the ticket.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the ticket description.
  $data['eventbrite_api_tickets']['description'] = array(
    'title' => t('Description'),
    'help' => t('The description of the ticket.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the ticket start date.
  $data['eventbrite_api_tickets']['start_date'] = array(
    'title' => t('Start date'),
    'help' => t('The date the ticket starts.'),
    'field' => array(
      'handler' => 'eventbrite_api_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'eventbrite_api_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Expose the ticket end date.
  $data['eventbrite_api_tickets']['end_date'] = array(
    'title' => t('End date'),
    'help' => t('The date the ticket ends.'),
    'field' => array(
      'handler' => 'eventbrite_api_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'eventbrite_api_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Expose the ticket minimum purchase.
  $data['eventbrite_api_tickets']['min'] = array(
    'title' => t('Minimum purchase'),
    'help' => t('The minimum number of tickets to be purchased.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the ticket maximum purchase.
  $data['eventbrite_api_tickets']['max'] = array(
    'title' => t('Maximum purchase'),
    'help' => t('The maximum number of tickets to be purchased.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the ticket price.
  $data['eventbrite_api_tickets']['price'] = array(
    'title' => t('Price'),
    'help' => t('The price of the ticket.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the quantity sold.
  $data['eventbrite_api_tickets']['quantity_sold'] = array(
    'title' => t('Quantity sold'),
    'help' => t('The quantity of tickets sold.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the visibility.
  $data['eventbrite_api_tickets']['visible'] = array(
    'title' => t('Visible'),
    'help' => t('The visibility of the ticket.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'type' => 'yes-no',
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the ticket currency.
  $data['eventbrite_api_tickets']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('The currency of the ticket.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the quantity available.
  $data['eventbrite_api_tickets']['quantity_available'] = array(
    'title' => t('Quantity available'),
    'help' => t('The quantity of tickets available.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the ticket display price.
  $data['eventbrite_api_tickets']['display_price'] = array(
    'title' => t('Display price'),
    'help' => t('The display price of the ticket.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose links to operate on the ticket.
  $data['eventbrite_api_tickets']['edit_ticket'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to the edit ticket page.'),
      'handler' => 'eventbrite_api_handler_field_ticket_link_edit',
    ),
  );

  // Expose links to operate on the ticket.
  $data['eventbrite_api_tickets']['clone_ticket'] = array(
    'field' => array(
      'title' => t('Clone link'),
      'help' => t('Provide a simple link to the clone ticket page.'),
      'handler' => 'eventbrite_api_handler_field_ticket_link_clone',
    ),
  );

  // Expose links to operate on the ticket.
  $data['eventbrite_api_tickets']['clone_other'] = array(
    'field' => array(
      'title' => t('Clone to other event'),
      'help' => t('Provide a simple link to the clone ticket to other event page.'),
      'handler' => 'eventbrite_api_handler_field_ticket_link_clone_other',
    ),
  );

  /**
   * Integrate the venues table.
   */
  $data['eventbrite_api_venues']['table']['group'] = t('Eventbrite venue');
  $data['eventbrite_api_venues']['table']['entity type'] = 'venue';

  $data['eventbrite_api_venues']['table']['base'] = array(
    'field' => 'vid',
    'title' => t('Eventbrite Venues'),
    'help' => t('Venues for Eventbrite events.'),
    'defaults' => array(
      'field' => 'name',
    ),
  );

  // Expose the venue ID.
  $data['eventbrite_api_venues']['vid'] = array(
    'title' => t('Venue ID'),
    'help' => t('The unique internal identifier of the event venue.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the venue city.
  $data['eventbrite_api_venues']['city'] = array(
    'title' => t('City'),
    'help' => t('The city of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the venue name.
  $data['eventbrite_api_venues']['name'] = array(
    'title' => t('Name'),
    'help' => t('The name of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the venue country.
  $data['eventbrite_api_venues']['country'] = array(
    'title' => t('Country'),
    'help' => t('The country of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the venue region.
  $data['eventbrite_api_venues']['region'] = array(
    'title' => t('Region'),
    'help' => t('The region of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the venue address.
  $data['eventbrite_api_venues']['address'] = array(
    'title' => t('Address'),
    'help' => t('The address of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the venue address_2.
  $data['eventbrite_api_venues']['address_2'] = array(
    'title' => t('Address 2'),
    'help' => t('The address 2 of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the venue postal code.
  $data['eventbrite_api_venues']['postal_code'] = array(
    'title' => t('Postal code'),
    'help' => t('The postal code of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the venue country code.
  $data['eventbrite_api_venues']['country_code'] = array(
    'title' => t('Country code'),
    'help' => t('The country code of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the venue longitude.
  $data['eventbrite_api_venues']['longitude'] = array(
    'title' => t('Longitude'),
    'help' => t('The longitude of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the venue latitude.
  $data['eventbrite_api_venues']['latitude'] = array(
    'title' => t('Latitude'),
    'help' => t('The latitude of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the lat/long.
  $data['eventbrite_api_venues']['lat_long'] = array(
    'title' => t('Latitude/longitude'),
    'help' => t('The latitude/longitude of the venue.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose links to operate on the venue.
  $data['eventbrite_api_venues']['edit_venue'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to the edit venue page.'),
      'handler' => 'eventbrite_api_handler_field_venue_link_edit',
    ),
  );

  /**
   * Integrate the attendees table.
   */
  $data['eventbrite_api_attendees']['table']['group'] = t('Eventbrite attendee');
  $data['eventbrite_api_attendees']['table']['entity type'] = 'attendee';

  $data['eventbrite_api_attendees']['table']['base'] = array(
    'field' => 'aid',
    'title' => t('Eventbrite Attendees'),
    'help' => t('Attendees of Eventbrite events.'),
  );

  // Expose the attendee ID.
  $data['eventbrite_api_attendees']['aid'] = array(
    'title' => t('Attendee ID'),
    'help' => t('The unique internal identifier of the event attendee.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'eventbrite_api_attendees_answers',
      'base field' => 'aid',
      'title' => t('Attendee ID'),
      'label' => t('Get the questions and answers for an attendee.'),
    ),
  );

  $data['eventbrite_api_attendees']['table']['default_relationship'] = array(
    'eventbrite_api_attendees_answers' => array(
      'table' => 'eventbrite_api_attendees_answers',
      'field' => 'aid',
    ),
  );

  // Expose the attendee work postal code.
  $data['eventbrite_api_attendees']['work_postal_code'] = array(
    'title' => t('Work postal code'),
    'help' => t('The work postal code of the event attendee.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee last name.
  $data['eventbrite_api_attendees']['last_name'] = array(
    'title' => t('Last name'),
    'help' => t('The last name of the event attendee.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee home address.
  $data['eventbrite_api_attendees']['home_address'] = array(
    'title' => t('Home address'),
    'help' => t('The home address of the event attendee.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee suffix.
  $data['eventbrite_api_attendees']['suffix'] = array(
    'title' => t('Suffix'),
    'help' => t('The suffix of the event attendee.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee home postal code.
  $data['eventbrite_api_attendees']['home_postal_code'] = array(
    'title' => t('Home postal code'),
    'help' => t('The home postal code of the event attendee.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee order tax.
  $data['eventbrite_api_attendees']['tax'] = array(
    'title' => t('Tax'),
    'help' => t('The tax on this order.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the order currency.
  $data['eventbrite_api_attendees']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('The currency of the order.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the prefix.
  $data['eventbrite_api_attendees']['prefix'] = array(
    'title' => t('Prefix'),
    'help' => t('The attendee prefix.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the first name.
  $data['eventbrite_api_attendees']['first_name'] = array(
    'title' => t('First name'),
    'help' => t('The attendee first name.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the order type.
  $data['eventbrite_api_attendees']['order_type'] = array(
    'title' => t('Order type'),
    'help' => t('The order type.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee work phone.
  $data['eventbrite_api_attendees']['work_phone'] = array(
    'title' => t('Work phone'),
    'help' => t('The attendee work phone.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee home address 2.
  $data['eventbrite_api_attendees']['home_address_2'] = array(
    'title' => t('Home address 2'),
    'help' => t('The attendee home address 2.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee work region.
  $data['eventbrite_api_attendees']['work_region'] = array(
    'title' => t('Work region'),
    'help' => t('The attendee work region.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee work address 2.
  $data['eventbrite_api_attendees']['work_address_2'] = array(
    'title' => t('Work address 2'),
    'help' => t('The attendee work address 2.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee blog.
  $data['eventbrite_api_attendees']['blog'] = array(
    'title' => t('Blog'),
    'help' => t('The attendee work blog.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee email.
  $data['eventbrite_api_attendees']['email'] = array(
    'title' => t('Email'),
    'help' => t('The attendee email.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the ticket ID.
  $data['eventbrite_api_attendees']['tid'] = array(
    'title' => t('Ticket ID'),
    'help' => t('The unique internal identifier of event tickets.'),
    'relationship' => array(
      'base' => 'eventbrite_api_tickets',
      'base field' => 'tid',
      'field' => 'tid',
      'handler' => 'views_handler_relationship',
      'label' => t('Event tickets'),
      'title' => t('Tickets'),
      'help' => t('The tickets associated with the event.'),
    ),
  );

  // Expose the attendee work address.
  $data['eventbrite_api_attendees']['work_address'] = array(
    'title' => t('Work address'),
    'help' => t('The attendee work address.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the event date.
  $data['eventbrite_api_attendees']['event_date'] = array(
    'title' => t('Event date'),
    'help' => t('The date of the event.'),
    'field' => array(
      'handler' => 'eventbrite_api_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'eventbrite_api_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Expose the order Eventbrite fee.
  $data['eventbrite_api_attendees']['eventbrite_fee'] = array(
    'title' => t('Eventbrite fee'),
    'help' => t('The order Eventbrite fee.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the order amount paid.
  $data['eventbrite_api_attendees']['amount_paid'] = array(
    'title' => t('Amount paid'),
    'help' => t('The amount paid on this order.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the attendee job title.
  $data['eventbrite_api_attendees']['job_title'] = array(
    'title' => t('Job title'),
    'help' => t('The attendee job title.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee work website.
  $data['eventbrite_api_attendees']['website'] = array(
    'title' => t('Website'),
    'help' => t('The attendee work website.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee home country code.
  $data['eventbrite_api_attendees']['home_country_code'] = array(
    'title' => t('Home country code'),
    'help' => t('The attendee home country code.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee home city.
  $data['eventbrite_api_attendees']['home_city'] = array(
    'title' => t('Home city'),
    'help' => t('The attendee home city.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee home phone.
  $data['eventbrite_api_attendees']['home_phone'] = array(
    'title' => t('Home phone'),
    'help' => t('The attendee home phone.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the ticket ID.
  $data['eventbrite_api_attendees']['orid'] = array(
    'title' => t('OrID'),
    'help' => t('The unique internal identifier of event ticket orders.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the attendee company.
  $data['eventbrite_api_attendees']['company'] = array(
    'title' => t('Company'),
    'help' => t('The attendee company.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee ticket barcode.
  $data['eventbrite_api_attendees']['barcode'] = array(
    'title' => t('Barcode'),
    'help' => t('The attendee barcode.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee home region.
  $data['eventbrite_api_attendees']['home_region'] = array(
    'title' => t('Home region'),
    'help' => t('The attendee home region.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee work country.
  $data['eventbrite_api_attendees']['work_country'] = array(
    'title' => t('Work country'),
    'help' => t('The attendee work country.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the order discount.
  $data['eventbrite_api_attendees']['discount'] = array(
    'title' => t('Discount'),
    'help' => t('The amount of discount on this order.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      //'float' => TRUE,?
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the attendee work city.
  $data['eventbrite_api_attendees']['work_city'] = array(
    'title' => t('Work country'),
    'help' => t('The attendee work city.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee home country.
  $data['eventbrite_api_attendees']['work_city'] = array(
    'title' => t('Work country'),
    'help' => t('The attendee work city.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the order created date.
  $data['eventbrite_api_attendees']['created'] = array(
    'title' => t('Order created date'),
    'help' => t('The date the order was created.'),
    'field' => array(
      'handler' => 'eventbrite_api_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'eventbrite_api_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Expose the attendee gender.
  $data['eventbrite_api_attendees']['gender'] = array(
    'title' => t('Gender'),
    'help' => t('The attendee gender.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee notes.
  $data['eventbrite_api_attendees']['notes'] = array(
    'title' => t('Notes'),
    'help' => t('The attendee notes.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the order modified date.
  $data['eventbrite_api_attendees']['modified'] = array(
    'title' => t('Order modified date'),
    'help' => t('The date the order was modified.'),
    'field' => array(
      'handler' => 'eventbrite_api_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'eventbrite_api_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Expose the order credit card fee.
  $data['eventbrite_api_attendees']['cc_fee'] = array(
    'title' => t('Credit card fee'),
    'help' => t('The amount of credit card fee on this order.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      //'float' => TRUE,?
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the event ID.
  $data['eventbrite_api_attendees']['eid'] = array(
    'title' => t('Event ID'),
    'help' => t('The unique internal identifier of events.'),
    'relationship' => array(
      'base' => 'eventbrite_api_events',
      'base field' => 'eid',
      'field' => 'eid',
      'handler' => 'views_handler_relationship',
      'label' => t('Events'),
      'title' => t('Events'),
      'help' => t('The events associated with the attendee.'),
    ),
  );

  // Expose the attendee affiliate.
  $data['eventbrite_api_attendees']['affiliate'] = array(
    'title' => t('Affiliate'),
    'help' => t('The attendee affiliate.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee work country code.
  $data['eventbrite_api_attendees']['work_country_code'] = array(
    'title' => t('Work country code'),
    'help' => t('The attendee work country code.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee birth date.
  $data['eventbrite_api_attendees']['birth_date'] = array(
    'title' => t('Birth date'),
    'help' => t('The attendee birth date.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee cell/mobile phone number.
  $data['eventbrite_api_attendees']['cell_phone'] = array(
    'title' => t('Cell phone'),
    'help' => t('The attendee cell/mobile phone number.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee age.
  $data['eventbrite_api_attendees']['age'] = array(
    'title' => t('Age'),
    'help' => t('The attendee age.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the attendee quantity.
  $data['eventbrite_api_attendees']['quantity'] = array(
    'title' => t('Quantity'),
    'help' => t('The attendee quantity.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose link to view the full attendee record.
  $data['eventbrite_api_attendees']['view_attendee'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the view attendee page.'),
      'handler' => 'eventbrite_api_handler_field_attendee_link',
    ),
  );

  /**
   * Integrate the attendees answers table.
   */
  $data['eventbrite_api_attendees_answers']['table']['group'] = t('Eventbrite attendee answers');
  $data['eventbrite_api_attendees_answers']['table']['entity type'] = 'attendee';

  $data['eventbrite_api_attendees_answers']['table']['base'] = array(
    'field' => 'aid',
    'title' => t('Eventbrite Attendee Answers'),
    'help' => t('Answers to Eventbrite questions.'),
  );

  // Expose the attendee ID.
  $data['eventbrite_api_attendees_answers']['aid'] = array(
    'title' => t('Attendee ID'),
    'help' => t('The unique internal identifier of attendees.'),
    'title' => t('Attendee ID'),
    'help' => t('The unique internal identifier of the event attendee.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'base' => 'eventbrite_api_attendees',
      'base field' => 'aid',
      'field' => 'aid',
      'handler' => 'views_handler_relationship',
      'label' => t('Attendees'),
      'title' => t('Attendees'),
      'help' => t('The attendees associated with the question/answer.'),
    ),
  );

  // Expose the attendee question ID.
  $data['eventbrite_api_attendees_answers']['qid'] = array(
    'title' => t('Question ID'),
    'help' => t('The unique internal identifier of the attendee question.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the attendee answer text.
  $data['eventbrite_api_attendees_answers']['answer_text'] = array(
    'title' => t('Answer'),
    'help' => t('The answer text to the attendee question.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee answer question.
  $data['eventbrite_api_attendees_answers']['question'] = array(
    'title' => t('Question'),
    'help' => t('The attendee question.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the attendee answer question type.
  $data['eventbrite_api_attendees_answers']['question_type'] = array(
    'title' => t('Question type'),
    'help' => t('The attendee question type.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;

}
