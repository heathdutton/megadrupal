<?php
/**
 * Defines an argument handler for sql date types.
 *
 * For now we don't override anything.
 */
class availability_calendar_handler_argument_sql_date extends views_handler_argument_date {
  // @todo: look at base class what needs to be overridden?
  // @todo: should we override from date_views module classes?
  // @todo: see availability argument handler: allow 2 dates?
  // @todo: should we derive from that handler, or copy its code and extend the other way round?
}
