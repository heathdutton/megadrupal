<?php

/**
 * @file
 * The HTML Tidy hook implementations for Drupal's Node system.
 */

/**
 * Implementation of hook_node_prepare().
 */
function htmltidy_node_prepare($node) {

  // We only have to tidy things on the edit form, not the create form.
  // As such, if this is new content, simply return.  There's nothing to tidy.
  if (!isset($node->nid)) {
    return;
  }

  // Get a list of all formats that have this filter enabled.
  $formats = htmltidy_get_formats();

  // Iterate through each field.
  $field_list = field_info_instances('node', $node->type);
  foreach ($field_list as $field_name => $field_data) {

    // Iterate through the language of each field.
    foreach ($node->$field_name as $language => $instances) {

      // Iterate through each language's instance.
      foreach ($instances as $instance_id => $instance) {

        // Skip through non-numerically keyed values to avoid items like
        // "[add_more] => Add another item".
        if (!is_numeric($instance_id)) {
          continue;
        }

        // Get a reference to the contents of this instance.
        $contents =& $node->{$field_name}[$language][$instance_id]['value'];

        // Act only on instances that are set to something.
        if(isset($contents)){

          // Determine the text format of the instance.
          $format =& $node->{$field_name}[$language][$instance_id]['format'];

          // It's possible that the format may not be set because some field
          // types (like plain text) don't specify text formats.  As such, they
          // won't want any filters run on them.  We can simply ignore these
          // instances.

          // Check if this format has our filter enabled.
          if (isset($format) && array_key_exists($format, $formats)) {

            // If there are any settings, format them properly and remove
            // any extraneous data elements.
            if (isset($formats[$format])) {
              $settings = unserialize($formats[$format]);
              $settings = $settings['htmltidy_filter_' . $format];
            }

            // If not, create an empty container.
            $settings = isset($settings) ? $settings : array();

            // Replace the contents with a tidied version of itself.
            $contents = htmltidy_fragment($contents, FALSE, $settings, $errors, $warnings);

            // Report any errors.
            if (!empty($errors)) {
              $errors = array_map('htmlentities', $errors);
              form_set_error($field_name, theme('item_list', $errors));
            }
          }
        }
      }
    }
  }
}

/**
 * Implementation of hook_node_validate().
 */
function htmltidy_node_validate($node) {

  // Get a list of all formats that have this filter enabled.
  $formats = htmltidy_get_formats();

  // Iterate through each field.
  $field_list = field_info_instances('node', $node->type);
  foreach ($field_list as $field_name => $field_data) {

    // Ignore empty fields.
    if (isset($node->$field_name)) {

      // Iterate through the language of each field.
      foreach ($node->$field_name as $language => $instances) {

        // Iterate through each language's instance.
        foreach ($instances as $instance_id => $instance) {

          // Skip through non-numerically keyed values to avoid items like
          // "[add_more] => Add another item".
          if (!is_numeric($instance_id)) {
            continue;
          }

          // Get a reference to the contents of this instance.
          $contents =& $node->{$field_name}[$language][$instance_id]['value'];

          // Act only on instances that are set to something.
          if(isset($contents)){

            // Determine the text format of the instance.
            $format =& $node->{$field_name}[$language][$instance_id]['format'];

            // It's possible that the format may not be set because some field
            // types (like plain text) don't specify text formats.  As such,
            // they won't want any filters run on them.  We can simply ignore
            // these instances.

            // Check if this format has our filter enabled.
            if (isset($format) && array_key_exists($format, $formats)) {

              // Call all of the filters. If they're using this one, it'll fall
              // into the proper order when they all run.  When we get to ours,
              // it'll set any warnings or errors for us.
              global $_htmltidy_filter;
              check_markup($contents, $format);

              // Collect the errors and warnings if our filter was run.
              $filtered =& $_htmltidy_filter['filtered'];
              if (isset($filtered) && $filtered) {
                $errors = $_htmltidy_filter['errors'];
                $warnings = $_htmltidy_filter['warnings'];
              }
              else {
                // Our filter isn't being run, so we have to run it through
                // ourselves.

                // If there are any settings, format them properly and remove
                // any extraneous data elements.
                if (isset($formats[$format])) {
                  $settings = unserialize($formats[$format]);
                  $settings = $settings['htmltidy_filter_' . $format];
                }

                // If not, create an empty container.
                $settings = isset($settings) ? $settings : array();

                // Get the tidied text and set the field value to it.
                $clean = htmltidy_fragment(
                  $contents, TRUE, $settings, $errors, $warnings
                );
                form_set_value(
                  array('#parents' => array($field_name)),
                  $clean,
                  $form_state
                );
              }

              // If there were any errors or warnings, act on them.
              if ($errors || $warnings) {

                // As a courtesy, display the original text to the editor.
                $message = '<p>Original body:</p><pre>' .
                             htmlentities($contents) .
                           '</pre>';

                // Report any errors.
                if ($errors) {
                  $message .= theme(
                    'item_list', array_map('htmlentities', $errors)
                  );
                  form_set_error($field_name, $message);
                }

                // Report any warnings.
                if ($warnings) {
                  drupal_set_message(
"The following HTML errors have been cleaned up automatically for you: " .
                    theme(
                      'item_list',
                      array('items' => array_map('htmlentities', $warnings))
                    )
                  );
                }
              }
            }
          }
        }
      }
    }
  }
}

