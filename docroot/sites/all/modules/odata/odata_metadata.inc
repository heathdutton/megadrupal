<?php 
/**
 *  @file
 *  Contains all the functionality that is needed to retrieve matadata from a OData endpoint
 */

/**
 *  It takes the $metadata array and retrieves the Collections, the fields of them and their properties.
 */
function odata_get_entity_properties($xmlarray) {
  $type_container = odata_get_entity_container($xmlarray);

  $ret = array();

  foreach (odata_get_entity_hrefs( $xmlarray['@@uri']) as $i => $collection ) {
    $ret[$collection] = odata_get_collection_entity_name($xmlarray, $type_container, $collection);
  }
  return $ret;
}

/**
 *  It return the fields and properties of a collection
 */
function odata_get_collection_entity_name($xmlarray, $type_container, $collection) {
  $packed = $type_container[$collection];
  $unpacked=explode('.', $packed);
  $namespace = $unpacked[0];
  for ($i=1;$i<(count($unpacked)-1);$i++) {
    $namespace .= '.' . $unpacked[$i];
  }
  $properties = odata_get_namespace_properties($xmlarray);
  return $properties[$namespace][$unpacked[count($unpacked)-1]];
}

/**
 *  It return the fields and properties of a complex property
 */
function odata_get_complex_entity(&$complexes_array, $complextype) {
  $packed = $complextype;

  $unpacked = explode('.', $packed);
  $namespace = $unpacked[0];
  for ($i = 1; $i < (count($unpacked)-1); $i++) {
    $namespace .= '.' . $unpacked[$i];
  }
  $properties = $complexes_array;
  return $properties[$namespace][$unpacked[count($unpacked)-1]];
}

/**
 *  Returns every entity provided in the main page of the web service
 */
function odata_get_entity_hrefs($uri) {
  $ret = odata_request($uri . '/');
  return $ret['EntitySets'];
}

/**
 *  Returns every entity name with it's asociated entity type
 *  eg [Products] => ODataDemo.Product, ...
 */
function odata_get_entity_container($xmlarray) {
  $ret=array();
  if ( !empty($xmlarray['Schema'])) {
    foreach ( $xmlarray['Schema'] as $i => $schema ) {
      if ( !empty($schema['EntityContainer'])) {
        foreach ($schema['EntityContainer'] as $j => $entity_container) {
            if ( !empty($entity_container['EntitySet'])) {
            foreach ($entity_container['EntitySet'] as $k => $entityset) {
              $ret[$entityset['@@']['Name']]= $entityset['@@']['EntityType'];
            }//foreach
          }
        }//foreach
      }
    }//foreach
  }
  return $ret;
}

/**
 *  It returns an array with every Schema Namespace and it's properties
 */
function odata_get_namespace_properties($xmlarray) {
  $namespaces=array();
  foreach ( $xmlarray['Schema'] as $i => $schema ) {
    $ret = array();
    if (!empty($schema['EntityType']))
    foreach ($schema['EntityType'] as $key => $value) {
      $ret[$value['@@']['Name']]=array();
      if (!empty($value['Key'][0]['PropertyRef']))
      foreach ($value['Key'][0]['PropertyRef'] as $k => $v) {
        $ret[$value['@@']['Name']]['Keys'][]= $v['@@']['Name'];
      }//foreach
      if (!empty($value['Property']))
      foreach ($value['Property'] as $k => $v) {
        if (!empty($ret[$value['@@']['Name']]['Keys']))
        foreach ( $ret[$value['@@']['Name']]['Keys'] as $kkey => $kval  ) {
          if ( $kval == $v['@@']['Name'] ) {
            $v['@@']['is_key']= TRUE;
          }
          else {
            $v['@@']['is_key']= FALSE;
          }
        }
        $ret[$value['@@']['Name']]['Values'][]= $v['@@'];
      }
    }
    $namespaces[ $schema['@@']['Namespace'] ] = $ret;
  }
  return $namespaces;
}

/**
 *  It returns an array with every Schema Namespace and it's Complex Types
 */
function odata_get_complex_properties($xmlarray) {
  $namespaces=array();
  foreach ( $xmlarray['Schema'] as $i => $schema ) {
    $ret = array();
    if (!empty($schema['ComplexType']))
    foreach ($schema['ComplexType'] as $key => $value) {
      $ret[$value['@@']['Name']]=array();
      if (!empty($value['Property']))
      foreach ($value['Property'] as $k => $v) {
        $ret[$value['@@']['Name'] ] [ $v['@@']['Name']  ]= $v['@@'];
      }
    }
    $namespaces[ $schema['@@']['Namespace'] ] = $ret;
  }
  return $namespaces;
}


/**
 *  It parse an XML text into an array
 */
function odata_xml2array( $text_xml, $uri=NULL , $attr =NULL ) {
  $reg_ex_elements = '/<(\w+)([^>]*)((>(.*?)<\/\\1>)|(\/>))/is';
  $reg_ex_attributes = '/(\w+)=[\'"]([^"\']*)[\'"]/';
  preg_match_all( $reg_ex_elements, $text_xml, $match_elements);
  foreach ($match_elements[1] as $key_elements => $val_elements) {
    if ($match_elements[2][$key_elements]) {
      preg_match_all( $reg_ex_attributes, $match_elements[2][$key_elements], $match_attributes );
      foreach ( $match_attributes[0] as $key_attributes => $val_attributes ) {
        $array_attributes[ $val_elements . ' attributes' ][$match_attributes[1][ $key_attributes ] ] = $match_attributes[2][ $key_attributes ];
      }
    }
    else {
      $array_attributes = NULL;
    }
    if ( preg_match( $reg_ex_elements, $match_elements[3][$key_elements]) ) {
          if ( $array_attributes ) {
            $array_final[ $val_elements ][] = odata_xml2array( $match_elements[3][$key_elements], NULL, $array_attributes[ $val_elements . ' attributes' ]);
          }
          else {
            $array_final[ $val_elements ][] = odata_xml2array( $match_elements[3][$key_elements] );
          }
      }
      else {
        if ( $array_attributes ) {
          $array_final[ $val_elements ][] =array('>>' => $match_elements[3][ $key_elements ] , '@@' => $array_attributes[ $val_elements . ' attributes' ]);
        }
        else {
          $array_final[ $val_elements ][] = $match_elements[3][ $key_elements ];
        }
      }
  }
  if ( !empty($attr) )$array_final['@@'] = $attr;
  if ( !empty($uri) )$array_final['@@uri'] = $uri;
  return $array_final;
}//

function odata_xml2array2( $text_xml, $uri=NULL ) {
  $final_array=array();
  
  $text_xml =drupal_substr($text_xml, strpos($text_xml, '<Schema ') );
  $text_xml =drupal_substr($text_xml, 0, strripos($text_xml, '</Schema>', -1)+ drupal_strlen('</Schema>') );

  $text_xml = '<global>' . $text_xml . '</global>';

  $all_obj = new SimpleXMLElement($text_xml);
  
  $return_array = odata_private_xml_parser($all_obj);
  $return_array = $return_array;
  $return_array['@@uri'] = $uri;
  
  return $return_array;
}

function odata_private_xml_parser($node ) {         
  $current=array();
  
  //Current node name
  $node_name = $node->getName();

  //Get attributes of current node
  foreach ($node->attributes() as $k => $v) {
    $current['@@'][$k] = "$v";
  }

  //Get children
  $i = 0;
  foreach ($node->children() as $name => $value) {
    $current[$name][$i] = odata_private_xml_parser($value );
    $i++;
  }
  return $current;  
}

/**
 *  Converts the type from Open Data Type to Drupal Type
 */
function odata_get_type($type) {
  switch ($type) {
    case 'Edm.Int16':
    case 'Edm.Int32':
    case 'Edm.Int64':
    case 'Edm.SByte':
    case 'Edm.Single':
    case 'Edm.Double':
    case 'Edm.Decimal':
    case 'Edm.Byte':
      return 'numeric';
      break;
    case 'Edm.DateTime':
    case 'Edm.DateTimeOffset':
    case 'Edm.Time':
      return 'date';
      break;
    case 'Edm.Boolean':
      return 'boolean';
      break;
    case 'Edm.String':
    case 'Edm.Guid ':
    case 'Edm.Binary':
    case 'Null':
      return 'markup';
      break;
    default:
      return '';
  }
}
