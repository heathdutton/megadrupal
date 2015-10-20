<?php

/**
 * @file
 * import coverages from swd
 * german national libary
 */
class ArchibaldCoverageSwd extends ArchibaldAbstractCoverageType {

  /**
   * An array with codes which we want
   * @var Array
   */
  private $usedCodes = array(
    #HAUPTSCHLAGWORT
    "800" => array(),
    #1. UNTERSCHLAGWORT
    "801" => array(),
    #2. UNTERSCHLAGWORT
    "802" => array(),
    #3. UNTERSCHLAGWORT
    "803" => array(),
    #4. UNTERSCHLAGWORT
    "804" => array(),
    #5. UNTERSCHLAGWORT
    "805" => array(),
    #Aequivalente Bezeichnung
    "830" => array(),
    #OBERBEGRIFF ZU INDIVIDUALBEZEICHNUNGEN(SCHLAGWORTVERKNUEPFUNGEN)
    "845" => array(),
    #UEBERGEORDNETES SCHLAGWORT
    "850" => array(),
    #VERWANDTES SCHLAGWORT
    "860" => array(),
  );

  public function getLanguage() {
    return "de";
  }

  protected function convert(&$source) {
    // Charset bug couse Trema and Umlaute are the same (U0308) so couse same char, replace c9 to c8
    $source = preg_replace("/\xc9(.)/", "\xc8\\1", $source);
    // Killing not sorting chars
    $source = preg_replace("/(\x88|\x89)/", "", $source);
  }

  protected function getRecords() {
    return preg_split("/\x1D/", $this->sourceContent);
  }

  protected function getFields($record) {
    $lines = preg_split("/\x1E/", $record);
    $return = array();
    //Loop through every line
    foreach ($lines as $line_index => $line) {

      //Remove field seperator
      $line = str_replace("\x1F", "", trim($line));

      //Skip empty line
      if (empty($line)) {
        continue;
      }

      //Get code and data
      if (preg_match("/([0-9]{3})[0-9a-zA-Z]\|?(.*)$/is", $line, $matches)) {
        $code = $matches[1];
        $data = $matches[2];

        //Add the data to the represented code (unique)
        if (isset($this->usedCodes[$code])) {
          $return[md5($data)] = $data;
        }
      }
    }
    return $return;
  }
}

