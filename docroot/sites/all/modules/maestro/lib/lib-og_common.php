<?php

/*
 * Common class for organic groups functions to work with either v1 or v2
 */

class MaestroOgCommon {

  private static $ogVersion = NULL;

  const OG_VERSION_1 = 1;
  const OG_VERSION_2 = 2;

  /**
   * Return the version of OG currently installed MaestroOgCommon::OG_VERSION_1 or OG_VERSION_2
   * @return type
   */
  public static function getOGVersion() {
    if (self::$ogVersion === NULL) {
      self::$ogVersion = (function_exists('og_get_group')) ? MaestroOgCommon::OG_VERSION_1 : MaestroOgCommon::OG_VERSION_2;
    }

    return self::$ogVersion;
  }

  /* $og_group_ids = og_get_all_group('node');
    foreach ($og_group_ids as $og_group_id) {
    $og_group = node_load($og_group_id);
    if ($og_group->title == $group_variable) {
    $userTaskRecord->assign_id =
    }
    }


    $og_gids = og_get_group_ids();
    $og_groups = og_label_multiple($og_gids); */
  
  /**
   * Return an array of the table and column names for og 
   * return Array(
   *  'table' => table_name,
   *  'title_column' => '',
   *  'gid_column' => ''
   * )  
   */
  public static function getOgTableValues() {
    $table_values = array();
    self::getOGVersion();
    
    if (self::$ogVersion === self::OG_VERSION_1) {
      $table_values = array(
        'table' => "og",  
        'title_column' => "label",  
        'gid_column' => "gid",  
      );
    }
    else {
      $table_values = array(
        'table' => "node",  
        'title_column' => "title",  
        'gid_column' => "nid",            
      );
    }
    
    return $table_values;
  }
  
  /**
   * Return a label for a group id
   * @param type $id
   */
  public static function getLabel($id) {
    self::getOGVersion();
    
    if (self::$ogVersion === self::OG_VERSION_1) {
      return db_query("SELECT label FROM {og} WHERE gid = :gid", array(':gid' => $id))->fetchField();
    }
    else {
      return db_query("SELECT title FROM {node} WHERE nid = :gid", array(':gid' => $id))->fetchField();      
    }
  }
  
  /**
   * Return a group specified by ID
   * @param type $id
   * @return  MaestroCommonOgObject
   */
  public static function getGroup($id) {
    self::getOGVersion();
    $group = NULL;
    
    if (self::$ogVersion === self::OG_VERSION_1) {
      $og = og_load($id);
      $group = new MaestroCommonOgObject($og->label, $id);
    }
    else {
      $node = node_load($id);
      $group = new MaestroCommonOgObject($node->title, $node->nid);
    }
    
    return $group;
  }
  
  /**
   * Return a list of all groups
   * 
   * @return Array          An array of MaestroCommonOgObject
   */
  public static function getAllGroups() {
    $group_array = array();
    self::getOGVersion();
    
    if (self::$ogVersion === self::OG_VERSION_1) {
      $og_gids = og_get_group_ids();
      foreach ($og_gids as $og_gid) {
        $og_group = og_load($og_gid);
        $group_array[] = new MaestroCommonOgObject($og_group->label, $og_gid);
      }
    } 
    else {  
      $og_group_ids = og_get_all_group('node');
      foreach ($og_group_ids as $og_group_id) {
        $og_group = node_load($og_group_id);
        $group_array[] = new MaestroCommonOgObject($og_group->title, $og_group_id);
      }
    }
    
    return $group_array;
  }

}

/**
 * Common class for containing og objects
 */
class MaestroCommonOgObject {

  /**
   * Group label
   * @var type 
   */
  public $label = NULL;

  /**
   * Group Id
   * @var type 
   */
  public $gid = NULL;

  public function __construct($label, $gid) {
    $this->label = $label;
    $this->gid = $gid;
  }

}

