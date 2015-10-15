<?php

/**
 * @file
 * includes/json.inc
 */

/**
 * Provides a way to load objects from a sub-array.
 *
 * The Migrate module's streaming JSON parser starts loading objects from the
 * beginning of the file. We need our parsers to start loading further in. This
 * class just adds a function to select an array to load. In order to be used,
 * an extension to this class must be written that uses
 * SalsifyJSONReader::salsifyNext() to override the next() method.
 *
 * Example usage:
 * @code
 * public function next() {
 *   $this->salsifyNext('products');
 * }
 * @endcode
 */
class SalsifyJSONReader extends MigrateJSONReader {

  /**
   * {@inheritdoc}
   */
  public function __construct($json_url, $id_field) {
    // Our ID field must be an array in case more than one field is needed to
    // uniquely identify an element.
    parent::__construct($json_url, (array) $id_field);
  }

  /**
   * Iterates over the members of a specific array.
   */
  protected function salsifyNext($key) {
    migrate_instrument_start('SalsifyJSONReader::salsifyNext');

    $this->currentElement = $this->currentId = NULL;

    // Open the file and position it if necessary.
    if (!$this->fileHandle) {
      $this->fileHandle = fopen($this->url, 'r');
      // Support gzip compression.
      $meta_data = stream_get_meta_data($this->fileHandle);
      if (in_array('Content-Type: application/x-gzip', $meta_data['wrapper_data'])) {
        $this->fileHandle = fopen("compress.zlib://$this->url", 'r');
      }
      if (!$this->fileHandle) {
        Migration::displayMessage(t('Could not open JSON file !url',
                                  array('!url' => $this->url)));
        return;
      }

      // We're expecting an array of characters, so the first character should
      // be [.
      $char = $this->getNonBlank();
      // Ning exports are wrapped in bogus (), so skip a leading (.
      if ($char == '(') {
        $char = $this->getNonBlank();
      }
      if ($char != '[') {
        Migration::displayMessage(t('!url is not a JSON file containing an array of objects',
                                  array('!url' => $this->url)));
        return;
      }
      // Start looking for the key array. This code is copied from the JSON
      // parsing code further down.
      $char = $this->getNonBlank();
      if ($char == ',') {
        $char = $this->getNonBlank();
      }
      elseif ($char == ']') {
        $char = $this->getNonBlank();
        if ($char != '{') {
          $char = NULL;
        }
      }
      if ($char == '{') {
        $json = $char;
        $depth = 1;
        $in_string = FALSE;
        $in_escape = FALSE;
        while (($char = $this->getNonBlank()) !== FALSE) {
          if ($in_string) {
            if ($in_escape) {
              $in_escape = FALSE;
            }
            else {
              switch ($char) {
                case '"':
                  $in_string = FALSE;
                  break;

                case '\\':
                  $in_escape = TRUE;
                  break;
              }
            }
          }
          else {
            switch ($char) {
              case '{':
                $json = $char;
                ++$depth;
                break;

              case '}':
                $json = '';
                --$depth;
                break;

              case '"':
                $in_string = TRUE;
                break;
            }
          }
          if ($depth == 1 && $char != '{' && substr($json, 0, 1) == '{') {
            $json .= $char;
            if ($json == '{"' . $key . '":[') {
              break;
            }
          }
        }
      }
    }

    // We expect to be positioned either at an object (beginning with {) or the
    // end of the file (we should see a ] indicating this). Or, an
    // object-separating comma, to be skipped. Note that this treats commas as
    // optional between objects, which helps with processing malformed JSON with
    // missing commas (as in Ning exports).
    $c = $this->getNonBlank();
    if ($c == ',') {
      $c = $this->getNonBlank();
    }
    // Ning sometimes emits a ] where there should be a comma.
    elseif ($c == ']') {
      $c = $this->getNonBlank();
      if ($c != '{') {
        $c = NULL;
      }
    }
    // We expect to be at the first character of an object now.
    if ($c == '{') {
      // Start building a JSON string for this object.
      $json = $c;
      // Look for the closing }, ignoring brackets in strings, tracking nested
      // brackets. Watch out for escaped quotes, but also note that \\" is not
      // an escaped quote.
      $depth = 1;
      $in_string = FALSE;
      $in_escape = FALSE;
      while (($c = fgetc($this->fileHandle)) !== FALSE) {
        $json .= $c;
        if ($in_string) {
          // Quietly accept an escaped character.
          if ($in_escape) {
            $in_escape = FALSE;
          }
          else {
            switch ($c) {
              // Unescaped " means end of string.
              case '"':
                $in_string = FALSE;
                break;

              // Unescaped \\ means start of escape.
              case '\\':
                $in_escape = TRUE;
                break;
            }
          }
        }
        else {
          // Outside of strings, recognize {} as depth changes, " as start of
          // string.
          switch ($c) {
            case '{':
              ++$depth;
              break;

            case '}':
              --$depth;
              break;

            case '"':
              $in_string = TRUE;
              break;
          }
          // We've found our match, exit the loop.
          if ($depth < 1) {
            break;
          }
        }
      }

      // Turn the JSON string into an object.
      $this->currentElement = json_decode($json);
      // Set current ID as an array in case more than one field is needed to
      // uniquely identify the element.
      $this->currentId = array();
      foreach ($this->idField as $id_field) {
        $this->currentId[] = $this->currentElement->$id_field;
      }
    }
    else {
      $this->currentElement = NULL;
      $this->currentId = NULL;
    }
    migrate_instrument_stop('SalsifyJSONReader::salsifyNext');
  }

  /**
   * {@inheritdoc}
   */
  public function key() {
    if (isset($this->currentId)) {
      // We use arrays to identify records. The iterator requires that the key
      // be a string. To avoid long strings using up memory, we hash the
      // serialized key.
      return md5(serialize($this->currentId));
    }
  }

}
