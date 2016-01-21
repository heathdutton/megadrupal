<?php
/**
 *  @file
 * This file contains the information of date arguments
 */

require_once('odata_handler_filter.inc');

class odata_handler_argument_date extends views_handler_argument_string {

  /**
   * Constructor
   */
  function construct() {
    parent::construct();
  }
  
  
  function query($group_by = FALSE) {
  $valuestring = urldecode( $this->argument );
    $thisrealfield = _odata_toreal($this->real_field);
  $datetime=NULL;
  try{
    $datetime = new DateTime($valuestring, new DateTimeZone( 'UTC') );
    //If you uncomment the lines above, the time will be tranformed from the default Timezone to the UTC
    //$datetime = new DateTime($valuestring );
    //$datetime->setTimezone(new DateTimeZone( 'UTC'));
    $this->query->add_ready_contextual_filter(  _odata_date_equal($thisrealfield, $datetime )    );
  } catch (Exception $e) {
      $this->query->add_ready_contextual_filter( 'false'  );
  }
  }
}
