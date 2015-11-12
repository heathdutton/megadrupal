<?php

/**
 * Views for the default Eventbrite UI.
 */

/**
 * Implements hook_views_default_views().
 */
function eventbrite_api_views_default_views() {

  // Events.

  $view = new view();
  $view->name = 'events_list';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eventbrite_api_events';
  $view->human_name = 'Events';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Events';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer eventbrite';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'description' => 'description',
    'eid' => 'eid',
    'modified' => 'modified',
    'title' => 'title',
    'url' => 'url',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'eid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'modified' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'url' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No events';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no events yet. Please <a href="/admin/eventbrite/event/add">add an event</a>.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Eventbrite events: Event ID */
  $handler->display->display_options['fields']['eid']['id'] = 'eid';
  $handler->display->display_options['fields']['eid']['table'] = 'eventbrite_api_events';
  $handler->display->display_options['fields']['eid']['field'] = 'eid';
  $handler->display->display_options['fields']['eid']['separator'] = '';
  /* Field: Eventbrite events: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'eventbrite_api_events';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Eventbrite events: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'eventbrite_api_events';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['url']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['url']['alter']['external'] = TRUE;
  /* Field: Eventbrite events: Modified */
  $handler->display->display_options['fields']['modified']['id'] = 'modified';
  $handler->display->display_options['fields']['modified']['table'] = 'eventbrite_api_events';
  $handler->display->display_options['fields']['modified']['field'] = 'modified';
  $handler->display->display_options['fields']['modified']['date_format'] = 'short';
  /* Field: Eventbrite events: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'eventbrite_api_events';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Eventbrite events: Edit link */
  $handler->display->display_options['fields']['edit_event']['id'] = 'edit_event';
  $handler->display->display_options['fields']['edit_event']['table'] = 'eventbrite_api_events';
  $handler->display->display_options['fields']['edit_event']['field'] = 'edit_event';
  /* Field: Eventbrite events: Delete link */
  $handler->display->display_options['fields']['delete_event']['id'] = 'delete_event';
  $handler->display->display_options['fields']['delete_event']['table'] = 'eventbrite_api_events';
  $handler->display->display_options['fields']['delete_event']['field'] = 'delete_event';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/eventbrite/event/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Events';
  $handler->display->display_options['menu']['description'] = 'Manage Eventbrite event data.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Events';
  $handler->display->display_options['tab_options']['description'] = 'Manage Eventbrite event data.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  // Organizers.

  $view = new view();
  $view->name = 'organizers';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eventbrite_api_organizers';
  $view->human_name = 'Organizers';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Organizers';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer eventbrite';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No organizers';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no organizers yet. <a href="/admin/eventbrite/organizer/add">Add an organizer</a>.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Eventbrite organizer: Organizer ID */
  $handler->display->display_options['fields']['oid']['id'] = 'oid';
  $handler->display->display_options['fields']['oid']['table'] = 'eventbrite_api_organizers';
  $handler->display->display_options['fields']['oid']['field'] = 'oid';
  $handler->display->display_options['fields']['oid']['separator'] = '';
  /* Field: Eventbrite organizer: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'eventbrite_api_organizers';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Eventbrite organizer: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'eventbrite_api_organizers';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  /* Field: Eventbrite organizer: Edit link */
  $handler->display->display_options['fields']['edit_organizer']['id'] = 'edit_organizer';
  $handler->display->display_options['fields']['edit_organizer']['table'] = 'eventbrite_api_organizers';
  $handler->display->display_options['fields']['edit_organizer']['field'] = 'edit_organizer';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/eventbrite/organizer/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Organizers';
  $handler->display->display_options['menu']['description'] = 'Manage Eventbrite organizers.';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Organizers';
  $handler->display->display_options['tab_options']['description'] = 'Manage Eventbrite organizers.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  // Tickets.

  $view = new view();
  $view->name = 'tickets';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eventbrite_api_tickets';
  $view->human_name = 'Tickets';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Tickets';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No tickets';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no tickets. <a href="/admin/eventbrite/ticket/add">Add a ticket</a>.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Eventbrite ticket: Event ID */
  $handler->display->display_options['relationships']['eid']['id'] = 'eid';
  $handler->display->display_options['relationships']['eid']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['relationships']['eid']['field'] = 'eid';
  /* Field: Eventbrite ticket: Ticket ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['separator'] = '';
  /* Field: Eventbrite ticket: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Eventbrite events: Event ID */
  $handler->display->display_options['fields']['eid']['id'] = 'eid';
  $handler->display->display_options['fields']['eid']['table'] = 'eventbrite_api_events';
  $handler->display->display_options['fields']['eid']['field'] = 'eid';
  $handler->display->display_options['fields']['eid']['relationship'] = 'eid';
  $handler->display->display_options['fields']['eid']['separator'] = '';
  /* Field: Eventbrite events: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'eventbrite_api_events';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'eid';
  /* Field: Eventbrite ticket: Quantity available */
  $handler->display->display_options['fields']['quantity_available']['id'] = 'quantity_available';
  $handler->display->display_options['fields']['quantity_available']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['fields']['quantity_available']['field'] = 'quantity_available';
  /* Field: Eventbrite ticket: Quantity sold */
  $handler->display->display_options['fields']['quantity_sold']['id'] = 'quantity_sold';
  $handler->display->display_options['fields']['quantity_sold']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['fields']['quantity_sold']['field'] = 'quantity_sold';
  /* Field: Eventbrite ticket: Visible */
  $handler->display->display_options['fields']['visible']['id'] = 'visible';
  $handler->display->display_options['fields']['visible']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['fields']['visible']['field'] = 'visible';
  $handler->display->display_options['fields']['visible']['not'] = 0;
  /* Field: Eventbrite ticket: Price */
  $handler->display->display_options['fields']['price']['id'] = 'price';
  $handler->display->display_options['fields']['price']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['fields']['price']['field'] = 'price';
  $handler->display->display_options['fields']['price']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['price']['precision'] = '2';
  $handler->display->display_options['fields']['price']['prefix'] = '$';
  /* Field: Eventbrite ticket: Edit link */
  $handler->display->display_options['fields']['edit_ticket']['id'] = 'edit_ticket';
  $handler->display->display_options['fields']['edit_ticket']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['fields']['edit_ticket']['field'] = 'edit_ticket';
  /* Field: Eventbrite ticket: Clone link */
  $handler->display->display_options['fields']['clone_ticket']['id'] = 'clone_ticket';
  $handler->display->display_options['fields']['clone_ticket']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['fields']['clone_ticket']['field'] = 'clone_ticket';
  /* Field: Eventbrite ticket: Clone to other event */
  $handler->display->display_options['fields']['clone_other']['id'] = 'clone_other';
  $handler->display->display_options['fields']['clone_other']['table'] = 'eventbrite_api_tickets';
  $handler->display->display_options['fields']['clone_other']['field'] = 'clone_other';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/eventbrite/ticket/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Tickets';
  $handler->display->display_options['menu']['description'] = 'Manage Eventbrite tickets.';
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Tickets';
  $handler->display->display_options['tab_options']['description'] = 'Manage Eventbrite tickets.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  // Venues.

  $view = new view();
  $view->name = 'venues';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eventbrite_api_venues';
  $view->human_name = 'Venues';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Venues';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer eventbrite';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No venues';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no venues yet. <a href="/admin/eventbrite/venue/add">Add a venue</a>.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Eventbrite venue: Venue ID */
  $handler->display->display_options['fields']['vid']['id'] = 'vid';
  $handler->display->display_options['fields']['vid']['table'] = 'eventbrite_api_venues';
  $handler->display->display_options['fields']['vid']['field'] = 'vid';
  $handler->display->display_options['fields']['vid']['separator'] = '';
  /* Field: Eventbrite venue: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'eventbrite_api_venues';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Eventbrite venue: City */
  $handler->display->display_options['fields']['city']['id'] = 'city';
  $handler->display->display_options['fields']['city']['table'] = 'eventbrite_api_venues';
  $handler->display->display_options['fields']['city']['field'] = 'city';
  /* Field: Eventbrite venue: Region */
  $handler->display->display_options['fields']['region']['id'] = 'region';
  $handler->display->display_options['fields']['region']['table'] = 'eventbrite_api_venues';
  $handler->display->display_options['fields']['region']['field'] = 'region';
  /* Field: Eventbrite venue: Edit link */
  $handler->display->display_options['fields']['edit_venue']['id'] = 'edit_venue';
  $handler->display->display_options['fields']['edit_venue']['table'] = 'eventbrite_api_venues';
  $handler->display->display_options['fields']['edit_venue']['field'] = 'edit_venue';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/eventbrite/venue/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Venues';
  $handler->display->display_options['menu']['description'] = 'Manage Eventbrite venues.';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Venues';
  $handler->display->display_options['tab_options']['description'] = 'Manage Eventbrite venues.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  // Attendees.

  $view = new view();
  $view->name = 'attendees';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eventbrite_api_attendees';
  $view->human_name = 'Attendees';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Attendees';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer eventbrite';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'aid' => 'aid',
    'first_name' => 'first_name',
    'last_name' => 'last_name',
    'company' => 'company',
    'quantity' => 'quantity',
    'amount_paid' => 'amount_paid',
    'modified' => 'modified',
    'created' => 'created',
    'view_attendee' => 'view_attendee',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'aid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'first_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'company' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'quantity' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount_paid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'modified' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view_attendee' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Eventbrite attendee: Attendee ID */
  $handler->display->display_options['fields']['aid']['id'] = 'aid';
  $handler->display->display_options['fields']['aid']['table'] = 'eventbrite_api_attendees';
  $handler->display->display_options['fields']['aid']['field'] = 'aid';
  $handler->display->display_options['fields']['aid']['separator'] = '';
  /* Field: Eventbrite attendee: First name */
  $handler->display->display_options['fields']['first_name']['id'] = 'first_name';
  $handler->display->display_options['fields']['first_name']['table'] = 'eventbrite_api_attendees';
  $handler->display->display_options['fields']['first_name']['field'] = 'first_name';
  /* Field: Eventbrite attendee: Last name */
  $handler->display->display_options['fields']['last_name']['id'] = 'last_name';
  $handler->display->display_options['fields']['last_name']['table'] = 'eventbrite_api_attendees';
  $handler->display->display_options['fields']['last_name']['field'] = 'last_name';
  /* Field: Eventbrite attendee: Company */
  $handler->display->display_options['fields']['company']['id'] = 'company';
  $handler->display->display_options['fields']['company']['table'] = 'eventbrite_api_attendees';
  $handler->display->display_options['fields']['company']['field'] = 'company';
  /* Field: Eventbrite attendee: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'eventbrite_api_attendees';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  /* Field: Eventbrite attendee: Amount paid */
  $handler->display->display_options['fields']['amount_paid']['id'] = 'amount_paid';
  $handler->display->display_options['fields']['amount_paid']['table'] = 'eventbrite_api_attendees';
  $handler->display->display_options['fields']['amount_paid']['field'] = 'amount_paid';
  $handler->display->display_options['fields']['amount_paid']['precision'] = '0';
  /* Field: Eventbrite attendee: Order modified date */
  $handler->display->display_options['fields']['modified']['id'] = 'modified';
  $handler->display->display_options['fields']['modified']['table'] = 'eventbrite_api_attendees';
  $handler->display->display_options['fields']['modified']['field'] = 'modified';
  $handler->display->display_options['fields']['modified']['label'] = 'Order modified';
  $handler->display->display_options['fields']['modified']['date_format'] = 'custom';
  $handler->display->display_options['fields']['modified']['custom_date_format'] = 'd/m/Y h:i:s';
  /* Field: Eventbrite attendee: Order created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'eventbrite_api_attendees';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Order created';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd/m/Y h:i:s';
  /* Field: Eventbrite attendee: Link */
  $handler->display->display_options['fields']['view_attendee']['id'] = 'view_attendee';
  $handler->display->display_options['fields']['view_attendee']['table'] = 'eventbrite_api_attendees';
  $handler->display->display_options['fields']['view_attendee']['field'] = 'view_attendee';
  $handler->display->display_options['fields']['view_attendee']['label'] = 'View';
  $handler->display->display_options['fields']['view_attendee']['element_label_colon'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/eventbrite/attendee/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Attendees';
  $handler->display->display_options['menu']['description'] = 'View Eventbrite attendee data.';
  $handler->display->display_options['menu']['weight'] = '49';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Attendees';
  $handler->display->display_options['tab_options']['description'] = 'View Eventbrite attendee data.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  return $views;
}
