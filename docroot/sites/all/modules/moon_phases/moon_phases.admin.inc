<?php
/**
 * @file
 * Administrative forms and functions for the moon_phases module
 */

/**
 * Form constructor for the Moon Phases admin
 * 
 * @see moon_phases_admin_form_validate()
 * @see moon_phases_admin_form_submit()
 * 
 * @ingroup forms
 */
function moon_phases_admin_form( $form, &$form_state ) {

  $lat = variable_get( 'moon_lat', NULL );
  $lon = variable_get( 'moon_lon', NULL );

  if ( $lat && $lon ) {
    $form[ 'latlon' ] = array(
      '#value' => t( 'Latitude %lat, Longitude %lon', array( '%lat' => $lat, '%lon' => $lon ) )
    );
  }

  $form[ 'moon_zip' ] = array(
    '#type' => 'textfield',
    '#title' => t( 'Enter Zip Code' ),
    '#default_value' => variable_get( 'moon_zip', NULL ),
    '#size' => 11,
    '#maxlength' => 10,
    '#description' => t( 'Format: 10001 or 10001-1234 or 10001 1234' ),
  );

  $form[ 'submit' ] = array(
    '#type' => 'submit',
    '#value' => t( 'Submit' )
  );

  return $form;

} // function moon_phases_admin_form()


/**
 * Form validation for moon_phases_admin_form()
 * 
 * @see moon_phases_admin_form_submit()
 */
function moon_phases_admin_form_validate( $form, &$form_state ) {

  if ( strlen( $form_state[ 'values' ][ 'moon_zip' ] ) < 5 || strlen( $form_state[ 'values' ][ 'moon_zip' ] ) > 10 || ( strlen( $form_state[ 'values' ][ 'moon_zip' ] ) < 10 && strlen( $form_state[ 'values' ][ 'moon_zip' ] ) > 5 )  ) {
    form_set_error( 'moon_zip', t( 'The zip code you entered is not valid.' ) );
    $valid = FALSE;
  }
  elseif ( is_numeric( $form_state[ 'values' ][ 'moon_zip' ] ) && strlen( $form_state[ 'values' ][ 'moon_zip' ] ) == 5 ) {
    $valid = TRUE;
  }
  elseif ( strlen( $form_state[ 'values' ][ 'moon_zip' ] ) == 10 ) {
    if ( preg_match( '/^[0-9]{5}([- ]?[0-9]{4})?$/', $form_state[ 'values' ][ 'moon_zip' ] ) ) {
      $valid = TRUE;
    }
    else {
      form_set_error( 'moon_zip', t( 'The zip code you entered is not valid.' ) );
      $valid = FALSE;
    }
  }

  if ( $valid ) {
    drupal_set_message( t( 'Zip Code set to %z.', array( '%z' => $form_state[ 'values' ][ 'moon_zip' ] ) ), 'status', FALSE );
    variable_set( 'moon_zip', str_ireplace( ' ', '-', $form_state[ 'values' ][ 'moon_zip' ] ) );
  }

} // function moon_phases_admin_form_validate


/**
 * Form submission handler for moon_phases_admin_form_submit()
 * 
 * @see moon_phases_admin_form_validate()
 */
function moon_phases_admin_form_submit( $form, &$form_state ) {

  moon_phases_admin_get_latlon( $form_state[ 'values' ][ 'moon_zip' ] );

} // function moon_phases_admin_form_submit


/**
 * Get the latitude and longitude for the zip code entered from NOAA
 * 
 * @param $zip
 *   The zip code to query for lat lon
 */
function moon_phases_admin_get_latlon( $zip ) {

  $base_url = 'http://www.weather.gov/forecasts/xml/DWMLgen/wsdl/ndfdXML.wsdl';
  $parameters = array( 'zipCodeList' => str_ireplace( ' ', '-', $zip ) );

  $client = new SoapClient( $base_url );

  try {
    $results = $client->__soapCall( 'LatLonListZipCode', $parameters );
    $data = get_object_vars( simplexml_load_string( $results ) );
    $latLon = explode( ',', $data[ 'latLonList' ] );

    variable_set( 'moon_lat', $latLon[ 0 ] );
    variable_set( 'moon_lon', $latLon[ 1 ] );
    drupal_set_message( t( 'Latitude set to %lat and longitude set to %lon.', array( '%lat' => $latLon[ 0 ], '%lon' => $latLon[ 1 ] ) ), 'status', FALSE );
  }
  catch( Exception $e ) {
    drupal_set_message( t( 'Error retrieving data. Exception caught: %e', array( '%e' => $e->getMessage() ) ) );
  }

} // function moon_phases_admin_get_latlon


/**
 * Form constructor for Phase descriptions form
 * 
 * @see moon_phases_descriptions_form_validate()
 * @see moon_phases_descriptions_form_submit()
 * 
 * @ingroup forms
 */
function moon_phases_descriptions_form( $form, &$form_state ) {
  
  $form = array();
  
  $data = variable_get( 'moon_phase_new_moon', NULL );
  $text = ( $data ) ? $data[ 'value' ] : MOON_PHASE_NEW_MOON;
  $format = ( $data ) ? $data[ 'format' ] : ' filtered_html';
  $form[ 'moon_phase_new_moon' ] = array(
    '#title' => t( 'New moon' ),
    '#type' => 'text_format',
    '#default_value' => $text,
    '#format' => $format,
  );
  
  $data = variable_get( 'moon_phase_waxing_cresent', NULL );
  $text = ( $data ) ? $data[ 'value' ] : MOON_PHASE_WAXING_CRESCENT;
  $format = ( $data ) ? $data[ 'format' ] : ' filtered_html';
  $form[ 'moon_phase_waxing_cresent' ] = array(
    '#title' => t( 'Waxing cresent' ),
    '#type' => 'text_format',
    '#default_value' => $text,
    '#format' => $format,
  );
  
  $data = variable_get( 'moon_phase_first_quarter', NULL );
  $text = ( $data ) ? $data[ 'value' ] : MOON_PHASE_FIRST_QUARTER;
  $format = ( $data ) ? $data[ 'format' ] : ' filtered_html';
  $form[ 'moon_phase_first_quarter' ] = array(
    '#title' => t( 'First quarter' ),
    '#type' => 'text_format',
    '#default_value' => $text,
    '#format' => $format,
  );
  
  $data = variable_get( 'moon_phase_waxing_gibbous', NULL );
  $text = ( $data ) ? $data[ 'value' ] : MOON_PHASE_WAXING_GIBBOUS;
  $format = ( $data ) ? $data[ 'format' ] : ' filtered_html';
  $form[ 'moon_phase_waxing_gibbous' ] = array(
    '#title' => t( 'Waxing gibbous' ),
    '#type' => 'text_format',
    '#default_value' => $text,
    '#format' => $format,
  );
  
  $data = variable_get( 'moon_phase_full_moon', NULL );
  $text = ( $data ) ? $data[ 'value' ] : MOON_PHASE_FULL_MOON;
  $format = ( $data ) ? $data[ 'format' ] : ' filtered_html';
  $form[ 'moon_phase_full_moon' ] = array(
    '#title' => t( 'Full moon' ),
    '#type' => 'text_format',
    '#default_value' => $text,
    '#format' => $format,
  );
  
  $data = variable_get( 'moon_phase_waning_gibbous', NULL );
  $text = ( $data ) ? $data[ 'value' ] : MOON_PHASE_WANING_GIBBOUS;
  $format = ( $data ) ? $data[ 'format' ] : ' filtered_html';
  $form[ 'moon_phase_waning_gibbous' ] = array(
    '#title' => t( 'Waning gibbous' ),
    '#type' => 'text_format',
    '#default_value' => $text,
    '#format' => $format,
  );
  
  $data = variable_get( 'moon_phase_last_quarter', NULL );
  $text = ( $data ) ? $data[ 'value' ] : MOON_PHASE_LAST_QUARTER;
  $format = ( $data ) ? $data[ 'format' ] : ' filtered_html';
  $form[ 'moon_phase_last_quarter' ] = array(
    '#title' => t( 'Last quarter' ),
    '#type' => 'text_format',
    '#default_value' => $text,
    '#format' => $format,
  );
  
  $data = variable_get( 'moon_phase_waning_crescent', NULL );
  $text = ( $data ) ? $data[ 'value' ] : MOON_PHASE_WANING_CRESCENT;
  $format = ( $data ) ? $data[ 'format' ] : ' filtered_html';
  $form[ 'moon_phase_waning_crescent' ] = array(
    '#title' => t( 'Waning crescent' ),
    '#type' => 'text_format',
    '#default_value' => $text,
    '#format' => $format,
  );
  
  $form[ 'submit' ] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  
  $form[ 'reset' ] = array(
    '#type' => 'submit',
    '#value' => t( 'Reset default descriptions' ),
    '#submit' => array( 'moon_phases_reset_default' ),
  );
  
  return $form;
  
} // function moon_phases_descriptions_form()


/**
 * Form validation handler for moon_phases_descriptions_form()
 * 
 * @see moon_phases_descriptions_form_submit()
 */
function moon_phases_descriptions_form_validate( $form, &$form_state ) {

  variable_set( 'moon_phase_new_moon', $form_state[ 'values' ][ 'moon_phase_new_moon' ] );
  variable_set( 'moon_phase_waxing_cresent', $form_state[ 'values' ][ 'moon_phase_waxing_cresent' ] );
  variable_set( 'moon_phase_first_quarter', $form_state[ 'values' ][ 'moon_phase_first_quarter' ] );
  variable_set( 'moon_phase_waxing_gibbous', $form_state[ 'values' ][ 'moon_phase_waxing_gibbous' ] );
  variable_set( 'moon_phase_full_moon', $form_state[ 'values' ][ 'moon_phase_full_moon' ] );
  variable_set( 'moon_phase_waning_gibbous', $form_state[ 'values' ][ 'moon_phase_waning_gibbous' ] );
  variable_set( 'moon_phase_last_quarter', $form_state[ 'values' ][ 'moon_phase_last_quarter' ] );
  variable_set( 'moon_phase_waning_crescent', $form_state[ 'values' ][ 'moon_phase_waning_crescent' ] );
  
} // function moon_phases_descriptions_form_submit()


/**
 * Form submission handler for moon_phases_descriptions_form()
 * 
 * @see moon_phases_description_form_validate()
 */
function moon_phases_descriptions_form_submit( $form, &$form_state ) {

  variable_set( 'phase_new_moon', $form_state[ 'values' ][ 'moon_phase_new_moon' ][ 'value' ] );
  variable_set( 'phase_waxing_cresent', $form_state[ 'values' ][ 'moon_phase_waxing_cresent' ][ 'value' ] );
  variable_set( 'phase_first_quarter', $form_state[ 'values' ][ 'moon_phase_first_quarter' ][ 'value' ] );
  variable_set( 'phase_waxing_gibbous', $form_state[ 'values' ][ 'moon_phase_waxing_gibbous' ][ 'value' ] );
  variable_set( 'phase_full_moon', $form_state[ 'values' ][ 'moon_phase_full_moon' ][ 'value' ] );
  variable_set( 'phase_waning_gibbous', $form_state[ 'values' ][ 'moon_phase_waning_gibbous' ][ 'value' ] );
  variable_set( 'phase_last_quarter', $form_state[ 'values' ][ 'moon_phase_last_quarter' ][ 'value' ] );
  variable_set( 'phase_waning_crescent', $form_state[ 'values' ][ 'moon_phase_waning_crescent' ][ 'value' ] );
  
} // function moon_phases_descriptions_form_submit()


/**
 * Resets the phase description form to the predefined defaults
 */
function moon_phases_reset_default() {

  variable_set( 'moon_phase_new_moon', array( 'value' => MOON_PHASE_NEW_MOON, 'format' => 'filtered_html' ) );
  variable_set( 'moon_phase_waxing_cresent', array( 'value' => MOON_PHASE_WAXING_CRESCENT, 'format' => 'filtered_html' ) );
  variable_set( 'moon_phase_first_quarter', array( 'value' => MOON_PHASE_FIRST_QUARTER, 'format' => 'filtered_html' ) );
  variable_set( 'moon_phase_waxing_gibbous', array( 'value' => MOON_PHASE_WAXING_GIBBOUS, 'format' => 'filtered_html' ) );
  variable_set( 'moon_phase_full_moon', array( 'value' => MOON_PHASE_FULL_MOON, 'format' => 'filtered_html' ) );
  variable_set( 'moon_phase_waning_gibbous', array( 'value' => MOON_PHASE_WANING_GIBBOUS, 'format' => 'filtered_html' ) );
  variable_set( 'moon_phase_last_quarter', array( 'value' => MOON_PHASE_LAST_QUARTER, 'format' => 'filtered_html' ) );
  variable_set( 'moon_phase_waning_crescent', array( 'value' => MOON_PHASE_WANING_CRESCENT, 'format' => 'filtered_html' ) );
  variable_set( 'phase_new_moon', MOON_PHASE_NEW_MOON );
  variable_set( 'phase_waxing_cresent', MOON_PHASE_WAXING_CRESCENT );
  variable_set( 'phase_first_quarter', MOON_PHASE_FIRST_QUARTER );
  variable_set( 'phase_waxing_gibbous', MOON_PHASE_WAXING_GIBBOUS );
  variable_set( 'phase_full_moon', MOON_PHASE_FULL_MOON );
  variable_set( 'phase_waning_gibbous', MOON_PHASE_WANING_GIBBOUS );
  variable_set( 'phase_last_quarter', MOON_PHASE_LAST_QUARTER );
  variable_set( 'phase_waning_crescent', MOON_PHASE_WANING_CRESCENT );
  
} // function moon_phases_reset_default()