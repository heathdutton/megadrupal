<?php

class ProBelMorpheusImport extends XMLWriter {
  //elements that appear to be the same for every Item and Instance
  public $instance_defaults = array (
    'Device_Id' => '#required',
    'Source_Device_Id' => '',
    'Long_File_Id' => '#required', // same as MaterialId/house number
    'Last_Aired_Date' => NULL,
    'Last_Used_Date' => NULL,
    'Usage_Count' => '0',
    'Timecode_In' => '#required',
    'Timecode_Out' => '#required',
    'Vitc' => '',
    'Ltc' => '',
    'Delete_After' => NULL,
    'Video_Compression' => '0.00',
    'Audio_Mode' => 'STEREO',
    'Audio_Sample_Rate' => '0.00',
    'Audio_Compression' => '0.00',
    'Quality_Check' => NULL,
    'Run_On' => '',
    'Recorded_Date' => NULL,
    'Clip_Ref' => '0',
    'Technical_Comment' => '',
  );
  public $item_defaults = array(
    'Material_Id' => '#required',
    'Title' => '#required',
    'Episode_Number' => '#required',
    'Status' => 'Active',
    'On_Air_Duration' => '#required',
    'Run_On_Duration' => '#required',
    'Reviewed_By' => '',
    'Number_Of_Tapes' => '',
    'Aspect_Ratio' => '4.3',
    'Material_Type' => 'PROGRAMME',
    'Purge_Date' => '#required',
    'Owner_Id' => '0',
    'Scavenge' => '0',
    'Archive' => '0',
    'Notes' => '',
  );
  function writeImportData($results) {
    $this->startElement('ImportItem');
    // write the <Item>s and <Instance>s
    foreach ( $results as $d ) {
      $e = $this->formatData($d);
      $this->writeItem($e);
      $this->writeInstance($e);
    }
    $this->endElement();
  }

  function writeInstance($instance_data) {
    $this->writeMorpheusElement('Instance', $instance_data, $this->instance_defaults);
  }
  function writeItem($item_data) {
    $this->writeMorpheusElement('Item', $item_data, $this->item_defaults);
  }
  function writeMorpheusElement($type, $data, $defaults) {
    $this->startElement($type);

    // if the $default_value is '#required', then there *is* no default value.
    // if a value is present at $data[$key], even if it is 0 or empty, use it
    //   as the value to write in xml
    // if the value is NULL (undefined) then write the default value in xml
    foreach ( $defaults as $key => $default_value ) {
      if ( $data[$key] !== NULL ) {
        $this->writeElement($key, $data[$key]);
      }
      else if ( '#required' == $default_value ) {
        trigger_error("ProBel Morpheus Import data missing a value for element of type $key");
      }
      else {
        $this->writeElement($key, $default_value);
      }
    }
    $this->endElement();
  }
  function formatData($d) {
    if ( $d['format_name'] === 'LIVE' ) {
      $livefeed = $d['livefeed_name'];
      $probel_device = mnn_programming_exports_xml_device_map($livefeed);
      if ( $probel_device ) {
	$d['ep_device'] = $probel_device;
      }
      else {
	trigger_error("Failed to create LIVE episode with device: ".
		      $livefeed);
      } 
    }
    else {
      $d['ep_device'] = "SUNFIRE";
    }
    $ret = array();
    $dict = array(
      'title' => 'Title',
      'project_id' => 'Material_Id',
      'ep_number' => 'Episode_Number',
      'duration' => 'On_Air_Duration',
      'field_transcoded_timecode_in_value' => 'Timecode_In',
      'field_transcoded_timecode_out_value' => 'Timecode_Out',
      'format_name' => 'Material_Type',
      'ep_device' => 'Device_Id',
    );

    if ( $d['field_transcoded_timecode_out_value'] ) {
      $d['duration'] =
        $d['field_transcoded_timecode_out_value'] -
          $d['field_transcoded_timecode_in_value'];
    }
    else if ( $d['field_timecode_out_value'] ) {
      $d['duration'] =
        $d['field_timecode_out_value'] -
          $d['field_timecode_in_value'];
    }
    else {
      $d['duration'] = $d['event_end'] - $d['event_start'];
    }

    foreach ( $d as $key => $value ) {
      // for each field in the data array, we either do a simple tranformation,
      //   or call a function that does a more complex transformation, such as
      //   date formatting.
      // Format of function names is programming_xml_FIELDNAME() (where FIELDNAME
      //   is a value in one of the key=>value pairs in $dict)
      $funk = 'mnn_programming_exports_xml_'.$dict[$key];
      
      if ( function_exists($funk) ) {
        $ret[$dict[$key]] = $funk($value, $d);
      }
      else {
	$ret[$dict[$key]] = $value;
      }
    }

    if ( $ret['Material_Id'] ) {
      $ret['Long_File_Id'] = $ret['Material_Id'];
      $purge_date = mnn_programming_exports_xml_calculate_purge_date();
      $ret['Purge_Date'] = $purge_date;
      if ( $ret['On_Air_Duration'] ) {
        //FIXME: dunno if this is right
        $ret['Run_On_Duration'] = $ret['On_Air_Duration'];
      }
    }
    return $ret;
  }
}
