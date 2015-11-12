<?php
/**
 * Extends EntityDefaultViewsController to add our own Views data.
 */
class AvailabilityCalendarViewsController extends EntityDefaultViewsController {
  /**
   * Overrides  the result for hook_views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    // Define a fake field that can be used as a filter or argument on
    // availability.
    $entity = 'availability_calendar_calendar';
    $field = 'available';
    $data[$entity][$field] = array(
      'title' => t('Available'),
      'help' => t('Filters on availability during the defined period.'),
      'real field' => 'cid',
      'filter' => array(
        'real field' => 'cid',
        'handler' => 'availability_calendar_handler_filter_availability',
        'default_state' => availability_calendar_get_field_setting('default_state'),
        'allocation_type' => availability_calendar_get_field_setting('allocation_type'),
      ),
    );
    // Copy filter handler settings to argument handler.
    $data[$entity][$field]['argument'] = $data[$entity][$field]['filter'];
    // But with another handler type.
    $data[$entity][$field]['argument']['handler'] = 'availability_calendar_handler_argument_availability';

    // We do add the other tables here as well as they should be seen as part
    // of the calendar entity.
    // Define calendar availability table.
    $data['availability_calendar_availability'] = array(
      'table' => array(
        'group' => t('Availability'),
        'join' => array(
          $entity => array(
            'field' => 'cid',
            'left_field' => 'cid',
          ),
          'availability_calendar_state' => array(
            'field' => 'sid',
            'left_field' => 'sid',
          ),
        ),
      ),
      'cid' => array(
        'title' => t('Calendar ID'),
        'help' => t('Reference to an Availability Calendar.'),
        'field' => array(
          'handler' => 'views_handler_field_entity',
          'table' => $entity,
        ),
      ),
      'date' => array(
        'title' => t('Date'),
        'help' => t('The date for this availability state.'),
        'field' => array(
          'handler' => 'availability_calendar_handler_field_sql_date',
        ),
        'filter' => array(
          'handler' => 'availability_calendar_handler_filter_sql_date',
        ),
        'argument' => array(
          'handler' => 'availability_calendar_handler_argument_sql_date',
        ),
        'sort' => array(
          'handler' => 'availability_calendar_handler_sort_sql_date',
        ),
      ),
      'sid' => array(
        'title' => t('Availability state'),
        'help' => t('Reference to the availability state.'),
        'field' => array(
          'handler' => 'views_handler_field_numeric',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'availability_calendar_state',
          'base field' => 'sid',
          'label' => t('Reference to the availability state'),
        ),
      ),
    );

    // Define availability states table.
    // @todo: define this as a separate config entity.
    $data['availability_calendar_state'] = array(
      'table' => array(
        'group' => t('Availability States'),
      ),
      'sid' => array(
        'title' => t('State ID'),
        'help' => t('The unique ID of this availability state.'),
        'field' => array(
          'handler' => 'views_handler_field_entity',
        ),
      ),
      'css_class' => array(
        'title' => t('CSS class'),
        'help' => t('The CSS class used for this state.'),
        'field' => array(
          'handler' => 'views_handler_field',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'label' => array(
        'title' => t('State'),
        'help' => t('The name for this state.'),
        'field' => array(
          'handler' => 'views_handler_field',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_string',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'weight' => array(
        'title' => t('Weight'),
        'help' => t('The weight of this state.'),
        'field' => array(
          'handler' => 'views_handler_field_numeric',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),

      ),
      'is_available' => array(
        'title' => t('Treat as available'),
        'help' => t('Indicates if this state is to be treated as available.'),
        'field' => array(
          'handler' => 'views_handler_field_boolean',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_boolean',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_boolean',
        ),
      ),
    );

    return $data;
  }
}
