<?php
/**
 * Makes sclib_enkoder compatible with your own custom formatters
 *
 * @return array
 *  An array that is simply a key-value pairing of:
 *    "module name" => array of keys,
 *      "type" => the name of the type as in hook_formatter_info
 *      "custom callback" => not used yet, can be left out, used to
 *        future-proof some functionality.
 */
function hook_sclib_enkoder_compatible_formats() {
  $formats = array();

  // Built-in support for email module
  if (module_exists("email")) {
    $formats["email"] = array();
    foreach (array("default", "contact", "plain") as $type) {
      $formats["email"][] = array(
        "type" => "email_$type",
        "custom_callback" => null,
      );
    }
  }

  return $formats;
}

?>
