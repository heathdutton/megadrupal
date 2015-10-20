<?php

/**
 * @file
 * import coverages from rameau
 * french national libary
 */
class ArchibaldCoverageRameau extends ArchibaldAbstractCoverageType {

  /**
   * An array with codes which we want
   * @var Array
   */
  private $usedCodes = array(
    "301" => array(),
    "302" => array(),
    "502" => array(),
  );

  public function getLanguage() {
    return "fr";
  }

  protected function convert(&$source) {
    //Charset bug couse Trema and Umlaute are the same (U0308)
    //so cause same char, replace c9 to c8
    $line = preg_replace("/\xc9(.)/", "\xc8\\1", $line);
    //Killing not sorting chars
    $line = preg_replace("/(\x88|\x89)/", "", $line);
  }

  protected function getRecords() {
    return preg_split("/\x1D/", $this->sourceContent);
  }

  protected function getFields($record) {

    //Get all lines for this record
    $lines = preg_split("/\x1E/", $record);

    //Get all identifer numbers for the lines
    $header = array_shift($lines);
    preg_match_all("/([0-9]{3})[0-9]{9}/iUs", $header, $header_match);

    $return = $results = array();
    //Loop through every line
    foreach ($lines as $line_index => $line) {

      //Skip the lines with line index which are not within header
      if (!isset($header_match[1][$line_index])) {
        continue;
      }

      // Get the line number identifier
      $identifer_nr = $header_match[1][$line_index];
      if (!isset($results[$identifer_nr])) {
        $results[$identifer_nr] = array();
      }

      $value_array = $this->parseField(preg_split("/\x1F/", $line), $identifer_nr);
      switch ($identifer_nr) {
        case '301':
        case '302':
        case '502':
          #echo "+";
          $return = array_merge($return, $this->getKeywords($value_array, $value_array['id']));
      }

      if ($this->fieldIsMulti($identifer_nr)) {
        $results[$record_id][$identifer_nr][] = $value_array;
      }
      else {
        $results[$record_id][$identifer_nr] = $value_array;
      }
    }

    $k = "";
    for ($i = 160; $i <= 168; $i++) {
      if (empty($results[$record_id][$i . ""])) {
        continue;
      }
      $k = $i;
    }
    if (empty($k)) {
      return;
    }
    $return = array_merge($return, $this->getKeywords($results[$record_id][$k], $results[$record_id]["001"]));
    return $return;
  }

  private function getKeywords($values, $id) {
    if (!isset($values['a'])) {
      return array();
    }
    $keyword = array($id => $values['a']);

    if (!empty($values['g'])) {
      $keyword[$id] .= " (" . $values['g'] . ")";
    }
    if (isset($values['x']) && !empty($values['x'])) {
      if (!is_array($values['x'])) {
        $values['x'] = array($values['x']);
      }
      $keyword[$id] .= ' -- ' . implode(' -- ', $values['x']);
    }
    return $keyword;
  }

  private function parseField($values, $identifier) {
    switch ($identifier) {
      case '001':
        return current(str_replace('FRBNF', '', $values));

      case '202':
      case '624':
        array_shift($values);
        return drupal_substr(current($values), 1);

      case '160':
      case '161':
      case '162':
      case '163':
      case '164':
      case '165':
      case '166':
      case '167':
      case '168':
        $return = array();
        $return['x'] = array();
        foreach ($values as $v) {
          if (preg_match("/\.\.+/", $v, $matches)) {
            $return['indicator'] = str_replace(".", "", $v);
          }
          elseif (preg_match("/^a(.+)$/", $v, $matches)) {
            $return['a'] = $matches[1];
          }
          elseif (preg_match("/^x(.+)$/", $v, $matches)) {
            $return['x'][] = $matches[1];
          }
          elseif (preg_match("/^g(.+)$/", $v, $matches)) {
            $return['g'] = $matches[1];
          }
        }

        return $return;

      case '466':
      case '467':
        $used_for = "";
        foreach ($values as $v) {
          if (preg_match("/\.\.+/", $v, $matches)) {
            continue;
          }
          elseif (preg_match("/^.(.+)$/", $v, $matches)) {
            $used_for .= $matches[1];
          }
        }
        return $used_for;

      case '302':
      case '301':
      case '502':

        $return = array();
        $return['x'] = array();
        foreach ($values as $v) {
          if (preg_match("/^3([0-9]+)$/", $v, $matches)) {
            $return['id'] = $matches[1];
          }
          elseif (preg_match("/\.\.+/", $v, $matches)) {
            $return['indicator'] = str_replace(".", "", $v);
          }
          elseif (preg_match("/^a(.+)$/", $v, $matches)) {
            $return['a'] = $matches[1];
          }
          elseif (preg_match("/^g(.+)$/", $v, $matches)) {
            $return['g'] = $matches[1];
          }
          elseif (preg_match("/^x(.+)$/", $v, $matches)) {
            $return['x'][] = $matches[1];
          }
        }

        return $return;

      default:
        return $values;
    }
  }

  /**
   * check if intifier is multi or single item
   *
   * @param string $identifer_nr
   *
   * @return bollean
   */
  public function fieldIsMulti($identifer_nr) {
    switch ($identifer_nr) {
      case '202':
      case '001':
      case '160':
      case '161':
      case '162':
      case '163':
      case '164':
      case '165':
      case '166':
      case '167':
      case '168':
      case '610':
        return FALSE;
    }

    return TRUE;
  }
}

