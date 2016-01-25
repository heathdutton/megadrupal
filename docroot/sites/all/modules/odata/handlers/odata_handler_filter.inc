<?php
/**
 * @file
 * Here you can find all the filter handlers 
 */

/**
 * Handler for an object.
 * Overrides default SQL behaviour
 */
class odata_handler_filter extends views_handler_filter {
  /**
  * Called to override SQL behavior
  */
  function query() {
  }
}

/**
 *  Extends views_handler_filter_numeric
 */  
class odata_handler_filter_numeric extends views_handler_filter_numeric {
  function query() {
    $thisrealfield = _odata_toreal($this->real_field);

    //Get variables from view parameters or exposure block
    $operator = $this->options['operator'];
    $value = $this->options['value']['value'];
    $min_string = $this->options['value']['min'];
    $max_string = $this->options['value']['max'];
    if ( !empty( $this->view->exposed_data[$this->options['id']])) {
      if (!empty( $this->view->exposed_data[$this->options['id']]['min'])) {
        $min_string = $this->view->exposed_data[$this->options['id']]['min'];
        $max_string = $this->view->exposed_data[$this->options['id']]['max'];
      }
      else {
        $value = $this->view->exposed_input[$this->options['id']];
      }
      if (!empty( $this->view->exposed_data[$this->options['id'] . '_op'] )) {
        $operator = $this->view->exposed_data[$this->options['id'] . '_op'];
      }
    }

    //Add parameters to the query of view plug-in
    if ( $operator != 'between' && $operator != 'not between' ) {
      if ( $this->options['operator'] == '=' ) {
        $operator = 'eq';
      }
      elseif ( $this->options['operator'] == '!=' ) {
        $operator= 'ne';
      }
      elseif ( $this->options['operator'] == '<' ) {
        $operator= 'lt';
      }
      elseif ( $this->options['operator'] == '>' ) {
        $operator= 'gt';
      }
      elseif ( $this->options['operator'] == '>=' ) {
        $operator= 'ge';
      }
      elseif ( $this->options['operator'] == '<=' ) {
        $operator= 'le';
      }//if
      $this->query->add_filter($thisrealfield , $operator, $value , $this->options['group'] );
    }
    elseif ($operator=='between') {
      $this->query->add_between_filter($thisrealfield , $min_string, $max_string, FALSE, $this->options['group']);
    }
    elseif ($operator=='not between') {
      $this->query->add_between_filter($thisrealfield , $min_string, $max_string, TRUE, $this->options['group']);
    }//if
  }
}

/**
 *  Extends views_handler_filter_boolean_operator
 */
class  odata_handler_filter_boolean_operator extends views_handler_filter_boolean_operator {
  function query() {
    $thisrealfield = _odata_toreal($this->real_field);

    //Get variables from view parameters or exposure block
    $valuestring = $this->options['value'];
    if ( !empty( $this->view->exposed_data[$this->options['id']])) {
      $valuestring = $this->view->exposed_input[$this->options['id']];
    }

    //Add parameters to the query of view plug-in
    $datequery ='(' . (($valuestring)?'':'not+') . $thisrealfield . ')' ;
    $this->query->add_ready_filter( $datequery, $this->options['group'] );
  }
}

/**
 *  Extends views_handler_filter_string
 */
class  odata_handler_filter_markup extends views_handler_filter_string {
  function query() {
    $thisrealfield = _odata_toreal($this->real_field);

    //Get variables from view parameters or exposure block
    $valuestring = $this->options['value'];
    $operator = $this->options['operator'];
    if ( !empty( $this->view->exposed_data[$this->options['id']])) {
      $valuestring = $this->view->exposed_input[$this->options['id']];
      if (!empty( $this->view->exposed_data[$this->options['id'] . '_op'] )) {
        $operator = $this->view->exposed_data[$this->options['id'] . '_op'];
      }
    }

    //Add parameters to the query of view plug-in
    if ( $operator == '=' ) {
        $operator= 'eq';
      $this->query->add_filter($thisrealfield , $operator, '\'' . urlencode($valuestring) . '\'' , $this->options['group'] );
    }
    elseif ( $operator == '!=' ) {
      $operator= 'ne';
      $this->query->add_filter($thisrealfield , $operator, '\'' . urlencode($valuestring) . '\'' , $this->options['group'] );
    }
    elseif ($operator == 'contains' ) {
      $this->query->add_ready_filter( '(substringof(' . '\'' . urlencode($valuestring) . '\',' . $thisrealfield . '))'  , $this->options['group'] );
    }
    elseif ($operator == 'starts' ) {
      $this->query->add_ready_filter( '(startswith(' . $thisrealfield . ',' . '\'' . urlencode($valuestring) . '\'' . '))'  , $this->options['group'] );
    }
    elseif ($operator == 'not_starts' ) {
      $this->query->add_ready_filter( '(startswith(' . $thisrealfield . ',' . '\'' . urlencode($valuestring) . '\'' . ')+eq+false)'  , $this->options['group'] );
    }
    elseif ($operator == 'ends' ) {
      $this->query->add_ready_filter( '(endswith(' . $thisrealfield . ',' . '\'' . urlencode($valuestring) . '\'' . '))'  , $this->options['group'] );
    }
    elseif ($operator == 'not_ends' ) {
      $this->query->add_ready_filter( '(endswith(' . $thisrealfield . ',' . '\'' . urlencode($valuestring) . '\'' . ')+eq+false)'  , $this->options['group'] );
    }
    elseif ($operator == 'not' ) {
      $this->query->add_ready_filter( '(substringof(' . '\'' . urlencode($valuestring) . '\',' . $thisrealfield . ')+eq+false)'  , $this->options['group'] );
    }
    elseif ($operator == 'shorterthan' ) {
      $this->query->add_ready_filter( '(length(' . $thisrealfield . ')+lt+' . urlencode($valuestring) . ')'  , $this->options['group'] );
    }
    elseif ($operator == 'longerthan' ) {
      $this->query->add_ready_filter( '(length(' . $thisrealfield . ')+gt+' . urlencode($valuestring) . ')'  , $this->options['group'] );
    }
    elseif ($operator == 'word' ) {//contains any
      $operation = '+or+';
      $params = explode(' ' , $valuestring);
      $query = '(' . 'false';
      foreach ($params as $key => $value) {
        $query .= $operation . '(substringof(' . '\'' . $value . '\',' . $thisrealfield . '))' ;
      }//foreach
      $query .= ')';
      $this->query->add_ready_filter($query, $this->options['group'] );
    }
    elseif ($operator == 'allwords' ) {//contains any
      $operation = '+and+';
      $params = explode(' ' , $valuestring);
      $query = '(' . 'true';
      foreach ($params as $key => $value) {
        $query .= $operation . '(substringof(' . '\'' . $value . '\'' . ',' . $thisrealfield . '))' ;
      }//foreach
      $query .= ')';
      $this->query->add_ready_filter($query, $this->options['group'] );
    }//if
  }
}

/**
 *  Extends views_handler_filter_date
 */
class  odata_handler_filter_date extends views_handler_filter_date {

  function query() {
    $thisrealfield = _odata_toreal($this->real_field);

    //Get variables from view parameters or exposure block
    $value = $this->options['value']['value'];
    $min_string = $this->options['value']['min'];
    $max_string = $this->options['value']['max'];
    $operator_string = $this->options['operator'];
    if ( !empty( $this->view->exposed_data[$this->options['id']])) {
      if (!empty( $this->view->exposed_data[$this->options['id']]['min'])) {
        $min_string = $this->view->exposed_data[$this->options['id']]['min'];
        $max_string = $this->view->exposed_data[$this->options['id']]['max'];
      }
      else {
        $value = $this->view->exposed_input[$this->options['id']];
      }
      if (!empty( $this->view->exposed_data[$this->options['id'] . '_op'] )) {
        $operator_string = $this->view->exposed_data[$this->options['id'] . '_op'];
      }
    }

    //Add parameters to the query of view plug-in
    if ( $operator_string == '=' ) {
      $datetime = new DateTime($value );
      $datetime->setTimezone(new DateTimeZone( 'UTC'));
        $datequery = _odata_date_equal( $thisrealfield , $datetime );
      $this->query->add_ready_filter( $datequery, $this->options['group'] );
    }
    elseif ( $operator_string == '!=' ) {
      $datetime = new DateTime($value );
      $datetime->setTimezone(new DateTimeZone( 'UTC'));
      $datequery = _odata_date_not_equal( $thisrealfield , $datetime );
      $this->query->add_ready_filter( $datequery, $this->options['group'] );
    }
    elseif ( $operator_string == '<' ) {
      $operator= 'lt';
      $datetime = new DateTime($value );
      $datetime->setTimezone(new DateTimeZone( 'UTC'));
      $this->query->add_ready_filter( _odata_date_operant($thisrealfield, $datetime, $operator  ), $this->options['group']  );
    }
    elseif ( $operator_string == '>' ) {
      $operator= 'gt';
      $datetime = new DateTime($value );
      $datetime->setTimezone(new DateTimeZone( 'UTC'));
      $this->query->add_ready_filter( _odata_date_operant($thisrealfield, $datetime, $operator  ), $this->options['group']  );
    }
    elseif ( $operator_string == '>=' ) {
      $operator= 'gt';
      $datetime = new DateTime($value );
      $datetime->setTimezone(new DateTimeZone( 'UTC'));
      $this->query->add_ready_filter( _odata_date_operant($thisrealfield, $datetime, 'ge'  ), $this->options['group']  );
    }
    elseif ( $operator_string == '<=' ) {
      $operator= 'lt';
      $datetime = new DateTime($value );
      $datetime->setTimezone(new DateTimeZone( 'UTC'));
      $this->query->add_ready_filter( _odata_date_operant($thisrealfield, $datetime, 'le' ), $this->options['group']  );
    }
    elseif ($operator_string=='between') {
      $mintime = new DateTime($min_string );
      $mintime->setTimezone(new DateTimeZone( 'UTC'));
      $maxtime = new DateTime($max_string );
      $maxtime->setTimezone(new DateTimeZone( 'UTC'));
      $query = '((' . _odata_date_operant($thisrealfield, $mintime, 'ge'  ) . ')+and+(' . _odata_date_operant($thisrealfield, $maxtime, 'le'  ) . '))';
      $this->query->add_ready_filter( $query , $this->options['group']  );
    }
    elseif ($operator_string=='not between') {
      $mintime = new DateTime($min_string );
      $mintime->setTimezone(new DateTimeZone( 'UTC'));
      $maxtime = new DateTime($max_string );
      $maxtime->setTimezone(new DateTimeZone( 'UTC'));
      $query = 'not+((' . _odata_date_operant($thisrealfield, $mintime, 'gt'  ) . ')+and+(' . _odata_date_operant($thisrealfield, $maxtime, 'lt'  ) . '))';
      $this->query->add_ready_filter( $query , $this->options['group']  );
    }//if
  }
}

/**
 * Returns the checking string of date equality
 */
function _odata_date_equal($dbdate, $datetime) {
  $query = _odata_date_operant($dbdate, $datetime, 'eq');
  return $query;
}

/**
 * Returns the checking string of date non-equality
 */
function _odata_date_not_equal($dbdate, $datetime) {
  $query = 'not+(' . _odata_date_equal($dbdate, $datetime) . ')';
  return $query;
}


/**
 *  It returns the checking string of date that is larger or smaller or equal-larger or equal-smaller that the given.
 */
function _odata_date_operant($dbdate, $datetime, $operant) {
  $query = $dbdate . '+' . $operant . '+' . _odata_date_converter($datetime);
  return $query;
}

/**
 *  It takes a DateTime object and returns it in OData format
 */
function _odata_date_converter($datetime) {
  return "datetime'" . $datetime->format('Y-m-d\TH:i:s') . "'";
}
