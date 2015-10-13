<?php
/**
 * @file
 * The actual processing functions are here. Only included when needed.
 */

/**
 * Scans the body text for embedded elements and triggers actions on them.
 *
 * A DOM Iterator way of filtering markup.
 *
 * This subroutine does NOT include a re-save of the given node.
 * Do that yourself.
 *
 * Implements hook_ACTION_ID_action().
 */
function file_ownage_scan_embeds_process_node(&$node, $settings = array()) {

  $strings = array(
    '!node' => l($node->title, 'node/' . $node->nid),
    '%function' => __FUNCTION__,
  );
  watchdog('file_ownage', "Running %function on !node", $strings, WATCHDOG_INFO);
  // Return this flag if a change needs to be made.
  $modified = FALSE;

  // Use the default global settings if none is defined.
  if (empty($settings)) {
    $settings = variable_get('file_ownage_settings', file_ownage_default_settings());
  }

  if (empty($node->body)) {
    // watchdog("file_ownage",
    // "No body content found in this node. Skipping !node.",
    // array('!node' => l($node->title, 'node/' . $node->nid)), WATCHDOG_DEBUG);
    return FALSE;
  }

  // Need to run this on each textfield.
  $field_instances = field_info_instances('node', $node->type);

  if (empty($field_instances)) {
    // This content type has no fields at all.
    return NULL;
  }

  // Look for text fields to process. This is expected to include the 'body',
  // but any others can get got also.
  foreach ($field_instances as $field_id => $field_def) {
    // Skip empty fields.
    if (empty($node->{$field_id})) {
      continue;
    }
    // Not sure where there is a generic handler for 'text' in this field_def
    // array. Use this.
    $field_type = $field_def['widget']['type'];
    switch ($field_type) {

      case 'text_textarea_with_summary':
          case 'text_textarea':
            // May be multiple. body is not usually multiple,
            // but the structure allows for it to be.
            foreach ($node->{$field_id}[$node->language] as $delta => $field_content) {
          $input = $field_content['value'];
          $output = file_ownage_scan_embeds_process_text($input, $node, $settings);
          if ($output === FALSE) {
            // Failed to process.
            watchdog('image_ownage', "Trouble processing the field $field_id - no output.");
            continue;
          }
          else {
            if ($input != $output) {
              $modified = TRUE;
              $node->{$field_id}[$node->language][$delta]['value'] = $output;
            }
          }

            }
        break;

      default:
            // unsupported;
    }
  }

  if ($modified) {
    // Remember to rebuild the teaser if changing the body,
    // or strange double-ups occur.
    unset($node->teaser);
  }
  return $modified;
}

/**
 * Subroutine for file_ownage_scan_embeds_process_node().
 *
 * Run our replacement on the given text string.
 *
 * @param string $input
 *   HTML markup.
 * @param object $node
 *   Owning Entity.
 * @param array $settings
 *   Context & Configs.
 *
 * @return bool|string
 *   The resulting text, FALSE on failure.
 */
function file_ownage_scan_embeds_process_text($input, $node, $settings = array()) {

  $output = $input;

  $xmldoc = filter_dom_load($input);
  if (!$xmldoc) {
    watchdog("file_ownage", "Text to be processed by file_ownage was <em>not valid XML</em>. Cannot proceed with scanning !node.", array('!node' => l($node->title, 'node/' . $node->nid)), WATCHDOG_WARNING);
    return FALSE;
  }
  $modified = FALSE;

  // Find all images in the page.
  $images = $xmldoc->getElementsByTagName('img');
  // Need to collect the elements in a pre-loop, otherwise changing the document
  // may throw off the indexing of the nodelist. This was a subtle trap.
  $imagelist = array();
  foreach ($images as $element) {
    $imagelist[] = $element;
  }
  foreach ($imagelist as $element) {
    watchdog('file_ownage', 'Found an img element in the body text, triggering the file_ownage processing rule.', array(), WATCHDOG_INFO);
    // todo, shift some of this logic into rules and actions?
    // $arguments = array(
    //  'element' => &$element,
    //  'node' => &$node,
    //  'settings' => $settings
    // );
    // rules_invoke_event('embed_element_found', $arguments);
    if (file_ownage_process_img_element($element, $node, $settings)) {
      $modified = TRUE;
    }
  }

  // Also scan all the <a> links for references to files and resources.
  if ($settings['file_ownage']['import_files']) {
    $links = $xmldoc->getElementsByTagName('a');
    // Need to collect the elements in a pre-loop,
    // otherwise changing the document
    // may throw off the indexing of the nodelist. This was a subtle trap.
    /** @var $linklist DOMElement[] */
    $linklist = array();
    foreach ($links as $element) {
      $linklist[] = $element;
    }
    foreach ($linklist as $element) {
      $strings['%href'] = $element->getAttribute('href');
      watchdog('file_ownage', 'Found a link element in the body text, triggering the file_ownage processing rule on %href.', $strings, WATCHDOG_INFO);
      if (file_ownage_process_link_element($element, $node, $settings)) {
        $modified = TRUE;
      }
    }
  }

  if ($modified) {
    $output = filter_dom_serialize($xmldoc);
  }

  return $output;
}

/**
 * Process an img element.
 *
 * @param domElement $element
 *   DOM tag currently being worked on.
 * @param object &$node
 *   Owning entity.
 * @param array $settings
 *   Configs & Context.
 *
 * @return bool
 *   Success.
 */
function file_ownage_process_img_element(domElement $element, stdClass &$node, array $settings) {

  $success = FALSE;
  $img = array('element' => $element);

  // Extract expected attributes from the HTML element.
  $atts = array('src', 'title', 'alt', 'width', 'height', 'class', 'style');
  foreach ($atts as $att) {
    if ($val = $element->getAttribute($att)) {
      $img[$att] = $val;
    }
  }
  $working_src = ltrim($img['src'], '/');
  // This may get slightly munged in the process. Leave original $src alone.
  // If the remote URL has &amp; in it, then we need to convert that back to '&'
  // before attempting a file_copy. Maybe?
  // however that's the problem for the copy routine.
  // We need to keep the URL as encoded as it came.
  // these two worked against each other.
  // $working_src = urldecode($working_src);
  // If the remote URL has space in it,
  // then we need to encode that or later retrieval will fail.
  $working_src = str_replace(' ', '%20', $working_src);

  // Do we know it?
  // Ensure file exists and has a DB entry.
  // This func will search in other paths
  // and copy it across to the right location if needed.
  $file = file_ownage_get_file_entry($working_src, $settings, $node->uid);
  // The $file['filepath'] may have been updated to something better than the
  // $working_src. Don't forget to update that if needed.
  if (empty($file->fid) || !file_exists(drupal_realpath($file->uri))) {
    // Still failing? Give up.
    watchdog('file_ownage', 'Failed to deal with image %filepath. File could not be retrieved. Aborting.', array('%filepath' => $working_src), WATCHDOG_ERROR);
    return FALSE;
  }

  // We now have a DB entry, which contains what we need to know from here on
  // Merge the data array. $file overwrites our own data.
  $img = array_merge($img, (array) $file);

  // Deduce any extra data from context or other methods.
  // @see meta_inspector for this hook definition.
  $img['meta'] = module_invoke_all('metadata_from_file', $img['uri'], $img);

  // It's likely that this file needs a title to identify itself
  // so we can find it in the system later.
  if (empty($img['title'])) {
    if (isset($img['meta']['title'])) {
      $img['title'] = reset($img['meta']['title']);
    }
    else {
      // If the meta extraction failed to provide a title...
      // make a sensible one up.
      $img['title'] = $node->title . " : " . basename($img['src']);
    }
  }

  // We now know what we needed to know about the file.
  // Optionally, use the appropriate method to attach the file to the node.
  // (file attach, filefield, media module)
  // It may be a callback defined explicitly in the method info array,
  // or just be a magic function named directly after the method.
  $methods = file_ownage_attachment_methods();
  $method = $settings['image_ownage']['attach_method'];
  $strings['%method'] = $method;
  $method_definition = $methods[$method];
  $process_callback = isset($method_definition['process_callback']) ? $method_definition['process_callback'] : $method;
  if (function_exists($process_callback)) {
    // Shift the per-method subsettings array up into global scope.
    // This makes lazy variable-name re-use possible. EG filefield_name
    if (!empty($settings['image_ownage'][$method]) && is_array($settings['image_ownage'][$method])) {
      $settings += $settings['image_ownage'][$method];
    }
    // Call the callback!
    // drupal_set_message('Invoking ' . $process_callback);
    $success = $process_callback($node, $img, $settings) || $success;
  }
  else {
    watchdog('file_ownage', 'No process function callback available for method %method', $strings, WATCHDOG_ERROR);
  }
  watchdog('file_ownage', 'Finished processing the file absorbtion callback for one file, used method: %method', $strings, WATCHDOG_DEBUG);
  return $success;
}


/**
 * Process a link element.
 *
 * Most links will be to html pages, either local or remote.
 * We only care about 'files' and downloadable resources, so filter for them.
 * If found, those files can be owned.
 *
 * @param domElement $element
 *   The anchor tag.
 * @param object $node
 *   The owner entity.
 * @param array $settings
 *   Configs and context.
 *
 * @return bool
 *   Success.
 */
function file_ownage_process_link_element(domElement $element, &$node, array $settings) {

  $success = FALSE;
  $link = array('element' => $element);

  // Extract expected attributes from the HTML element.
  $atts = array('href', 'title');
  foreach ($atts as $att) {
    $link[$att] = $element->getAttribute($att);
  }
  $working_src = ltrim($link['href'], '/');
  // This may get slightly munged in the process. Leave original $src alone.
  // If the remote URL has &amp; in it, then we need to convert that back to '&'
  // before attempting a file_copy. Maybe?
  // however that's the problem for the copy routine.
  // We need to keep the URL as encoded as it came.
  // these two worked against each other.
  // $working_src = urldecode($working_src);
  // If the remote URL has space in it,
  // then we need to encode that or later retrieval will fail.
  $working_src = str_replace(' ', '%20', $working_src);

  // Do we care?
  if (!file_ownage_wanted_filetype($working_src, $settings['file_ownage']['suffix_list'])) {
    return FALSE;
  }
  watchdog('file_ownage', 'linked file %filepath seems to be something we should own.', array('%filepath' => $working_src), WATCHDOG_INFO);

  // Do we know it?
  // Ensure file exists and has a DB entry.
  // This func will search in other paths and copy it across to the right
  // location if needed.
  $file = file_ownage_get_file_entry($working_src, $settings, $node->uid);
  // The $file->uri may have been updated to something
  // better than the $working_src. Don't forget to update that if needed.
  if (empty($file->fid) || !file_exists(drupal_realpath($file->uri))) {
    // Still failing? Give up.
    watchdog('file_ownage', 'Failed to deal with linked file %filepath. File could not be retrieved.', array('%filepath' => $working_src), WATCHDOG_ERROR);
    return FALSE;
  }

  // We now have a DB entry, which contains what we need to know from here on
  // Merge the data array. The data from the $file needs to take precedence.
  $link = array_merge($link, (array) $file);

  // Deduce any extra data from context or other methods.
  // @see meta_inspector for this hook definition.
  $link['meta'] = module_invoke_all('metadata_from_file', $link['uri'], $link);

  // We now know what we needed to know about the file.
  // TODO, should we jazz up the link with a title and filesize?
  // Use the appropriate method to attach the file.
  // It may be a callback defined explicitly in the method info array,
  // or just be a magic function named directly after the method.
  $methods = file_ownage_file_attachment_methods();
  $method = $settings['file_ownage']['attach_method'];
  $method_definition = $methods[$method];
  $process_callback = isset($method_definition['process_callback']) ? $method_definition['process_callback'] : $method;
  if (function_exists($process_callback)) {
    // Shift the per-method subsettings array up into global scope.
    // This makes lazy variable-name re-use possible. EG filefield_name
    if (!empty($settings['file_ownage'][$method]) && is_array($settings['file_ownage'][$method])) {
      $settings += $settings['file_ownage'][$method];
    }
    // Call the callback!
    // drupal_set_message('Invoking ' . $process_callback);
    $success = $process_callback($node, $link, $settings) || $success;
  }
  else {
    watchdog('file_ownage', 'No process function callback available for method ' . $settings['file_ownage']['attach_method'], array(), WATCHDOG_ERROR);
  }
  watchdog('file_ownage', 'Finished processing the file absorbtion callback for one file ' . $settings['file_ownage']['attach_method'], array(), WATCHDOG_DEBUG);
  return $success;
}


/**
 * Creates the named file as an filefield attachment, returns success.
 *
 * Attaches it as the chosen supplimentary filefield on the node.
 *
 * @param object $node
 *   Entity to work on.
 * @param array $file
 *   Representing a drupal prepared file entry.
 * @param array $settings
 *   Context and configs.
 *
 * @return bool
 *   Whether the attachment happened.
 *   If the file is already attached, no change is made, so FALSE.
 */
function file_ownage_attach_filefield(&$node, array $file, array $settings = array()) {

  // Last-minute paranoia.
  if (!module_exists('file')) {
    drupal_set_message(t('file module not enabled - not attaching file to node'), 'warning');
    return FALSE;
  }

  $filefield_name = $settings['filefield_name'];
  $strings = array(
    '!uri' => $file['uri'],
    '%filefield_name' => $filefield_name,
    '!node' => l($node->title, 'node/' . $node->nid),
  );
  watchdog('file_ownage', "Checking if we need to attach !uri to the node !node as a filefield upload %filefield_name", $strings, WATCHDOG_INFO);

  // Confirm the field exists and is valid for this node type.
  $strings['%type'] = $node->type;
  $fields = field_info_instances('node', $node->type);
  if (empty($fields[$filefield_name])) {
    watchdog('file_ownage', 'Filefield %filefield_name is not available on this %type content type. Check the file_ownage settings', $strings, WATCHDOG_ERROR);
    return FALSE;
  }
  // Can't use the field definition from the $fields array,
  // $field_instance = $fields[$filefield_name];
  // It doesn't know its own cardinality - instead use.
  $field = field_info_field($filefield_name);

  $modified = FALSE;
  // Avoid attaching the same file twice.
  // Because of the recycling we are doing, this is possible (but unwanted).
  $already_attached = FALSE;
  foreach ((array) $node->{$filefield_name}[LANGUAGE_NONE] as $existing_entry) {
    if ($existing_entry['fid'] == $file['fid']) {
      watchdog('file_ownage', "Looks like !uri is already attached to this node !node. Fine, move along.", $strings);
      $already_attached = TRUE;
    }
  }

  if (!$already_attached) {
    // If this field is restricted, and already populated,
    // we won't replace it.
    if (($field['cardinality'] != FIELD_CARDINALITY_UNLIMITED) && !empty($node->$filefield_name)) {
      $current_attachment = reset($node->$filefield_name);
      if (empty($current_attachment)) {
        // An empty $node->$field_name should be null,
        // but sometimes it's an array(0). Check carefully.
      }
      else {
        watchdog('file_ownage', "This node !node already has a %filefield_name assigned and cardinality is limited. Not attaching a new one.", $strings, WATCHDOG_NOTICE);
        return FALSE;
      }
    }
  }

  if (!$already_attached) {
    // Add it. Update the filefield data and attach to the node.
    $file['display'] = isset($field['settings']['display_default']) ? $field['settings']['display_default'] : NULL;
    $file['description'] = (!empty($alt)) ? $alt : $file['filename'];
    $file['weight'] = 0;
    $file['new'] = TRUE;
    $file['title'] = isset($file['title']) ? $file['title'] : basename($file->filename);

    // Programatically attach a filefield to a node.
    if (empty($node->$filefield_name)) {
      $node->$filefield_name = array();
    }
    $node->{$filefield_name}[LANGUAGE_NONE][] = $file;

    watchdog('file_ownage', "Successfully attached !uri to the node !node as a filefield upload %filefield_name", $strings, WATCHDOG_INFO);
    $modified = TRUE;
  }

  // In case we moved or renamed the file during this process,
  // the link in the source will need to be updated
  // to point at the new location.
  $modified |= file_ownage_update_element_path($file, $settings);

  return $modified;
}

/**
 * Dummy 'process' function.
 *
 * What subroutine to do when 'no attachment' is selected.
 *
 * A remote file may still have been imported, or a local one renamed.
 * This means the tag in the HTML source needs updating, but nothing more.
 *
 * @param object $node
 *   Entity to work on.
 * @param array $tag
 *   Tag being processed.
 * @param array $settings
 *   Context and Configs.
 *
 * @return bool
 *   Modified or not.
 */
function file_ownage_attach_none(&$node, array $tag, array $settings = array()) {
  $modified = FALSE;
  $modified |= file_ownage_update_element_path($tag, $settings);
  return $modified;
}


/**
 * Find or create the file entry for the given file path. Fetching it if needed.
 *
 * If there is a valid entry already in the DB, return that.
 *
 * Otherwise, check if alternatives for that filepath are available
 * or if file needs to be imported to the filesystem,
 * copying it into the right place.
 * Do that, and make a file entry.
 *
 * $settings['path'] is relative to the drupal file system root.
 *
 * @param string $filepath
 *   Filepath to look up.
 * @param array $settings
 *   Configs & Context.
 * @param int $uid
 *   ID to make the file table entry as, defaults to current user.
 *   user ID to make the file table entry as, defaults to current user.
 *
 * @return object
 *   $file object (not an array)
 */
function file_ownage_get_file_entry($filepath, array $settings, $uid = NULL) {

  $strings = array('%filepath' => $filepath);

  // First see if the database already knows about this file.
  // Lookahead, expect failure first time.
  $file = file_ownage_load_file_by_filepath($filepath);
  if ($file) {
    watchdog('file_ownage', 'File entry %filepath already exists in the DB, re-using it.', $strings, WATCHDOG_INFO);
    return $file;
  }
  // Try harder.
  // See if the path is correct and valid.
  // Where SHOULD it be stored?
  $expected_fileuri = file_ownage_correct_path($filepath, $settings);
  $file = file_ownage_load_file_by_filepath($expected_fileuri);
  if ($file) {
    return $file;
  }

  // Looks like we may need to import it.
  // Check we have the biggest/best alternative version of the file
  // HOW, todo - this links in, so far, only to our custom
  // niwa_import_html_file_ownage_best_version() special algorithm.
  // This algorithmic search may support local files or remote URLs.
  watchdog('file_ownage', 'Seeing if I can guess a better place to look for the file %filepath. (Optional feature that can expand found thumbs into bigger originals)', $strings, WATCHDOG_INFO);
  $largers = module_invoke_all('file_ownage_best_version', $filepath);
  if (!empty($largers)) {
    $filepath = reset($largers);
    $expected_fileuri = file_ownage_correct_path($filepath, $settings);
    $strings['%larger'] = $filepath;
    watchdog('file_ownage', 'There is a better source path for this file. Using %larger instead of %filepath.', $strings, WATCHDOG_INFO);
    $strings['%filepath'] = $filepath;
    $file = file_ownage_load_file_by_filepath($expected_fileuri);
    if ($file) {
      return $file;
    }
    $filepath = $filepath['file'];
  }

  // Decide if it's local, nearby or remote.
  $path_status = file_ownage_path_status($filepath);

  if (!$path_status) {
    // File wasn't where we expected to find it.
    // Time to start looking for alternatives.
    $search_status = file_ownage_search_for_lost_files($filepath, $settings);
    if ($search_status) {
      $path_status |= $search_status;
    }

    // That may have updated the filepath by reference
    // to point at the found target.
    $strings['%filepath'] = $filepath;

    // If searching for it successfully found the file in another location
    // then maybe that location already has an entry and we don't have to
    // copy. return the result immediately.
    if ($path_status == FILE_OWNAGE_IS_REGISTERED) {
      if ($file = file_ownage_load_file_by_filepath($filepath)) {
        watchdog('file_ownage', 'File entry %filepath already exists in the DB, re-using it.', $strings, WATCHDOG_INFO);
        return $file;
      }
    }
  }
  if (!$path_status) {
    // Couldn't find it anywhere local or remote yet.
    // Basically, there is nothing I can do.
    watchdog('file_ownage', 'Haven\'t found file %filepath anywhere. Giving up.', $strings, WATCHDOG_NOTICE);
    return FALSE;
  }

  $strings['%expected_fileuri'] = $expected_fileuri;
  $copied = FALSE;

  // Maybe move it to where it should be stored, and create a DB entry.
  // Import or move it if needed.
  if ($filepath != $expected_fileuri) {
    if ($path_status == FILE_OWNAGE_IS_REMOTE) {
      // Handle our conditional import remote conditions.
      $import_domain_handler = 'ownage';
      $import_domain_filters = array();

      // Add blacklist and whitelist handling
      // And default domains for guessing where to find missing content.
      if (!empty($settings['domains'])) {
        if (in_array($settings['domains']['domain_handler'], array('include', 'exclude'))) {
          $import_domain_filters = $settings['domains']['domain_list'];
          array_map('trim', $import_domain_filters);
          $import_domain_handler = $settings['domains']['domain_handler'];
        }
      }

      // Default import remote action is True.
      $import_remote = TRUE;

      // Do we need to modify the import_remote option?
      switch ($import_domain_handler) {
        case 'include':
          // Only including if triggered. Set default state to False.
          $import_remote = FALSE;
          foreach ($import_domain_filters as $filter) {
            $pos = strpos($filepath, $filter);
            if (0 === $pos) {
              // Found one.
              $import_remote = TRUE;
            }
          }
          break;

        case 'exclude':
          foreach ($import_domain_filters as $filter) {
            $pos = strpos($filepath, $filter);
            if (0 === $pos) {
              $import_remote = FALSE;
            }
          }
          break;

        case 'ownage':
          $import_remote = TRUE;
          break;

        default:
          $import_remote = FALSE;
          break;
      }

      // Looks like we need to move it.
      if ($import_remote) {
        $copied = file_ownage_file_copy($filepath, $expected_fileuri);
        if ($copied) {
          watchdog('file_ownage', "Imported remote file from %filepath to %expected_fileuri. 0WNAGE!", $strings, WATCHDOG_NOTICE);
        }
        else {
          watchdog('file_ownage', "Failed remote file retrieval of %filepath in file_ownage_get_file_entry(). It probably wasn't there", $strings, WATCHDOG_ERROR);
        }
      }
      else {
        watchdog('file_ownage', "Not copying from remote %filepath due to domain restriction rules", $strings, WATCHDOG_NOTICE);
      }
    }
    elseif ($path_status == FILE_OWNAGE_IS_NEARBY && $settings['import_nearby']) {
      // Needs to be moved into the files dir.
      $copied = file_ownage_file_copy($filepath, $expected_fileuri);
    }
    elseif ($path_status == FILE_OWNAGE_IS_LOCAL) {
      // Is already in the files dir, but we have a better destination for it.
      $copied = file_ownage_file_copy($filepath, $expected_fileuri);
    }
    elseif ($path_status == FILE_OWNAGE_IS_REGISTERED) {
      // ?? I think we cannot get here. if IS_REGISTERED,
      // would have returned earlier after the search happened.
      $strings['%function'] = __FUNCTION__;
      watchdog('file_ownage', "I don't think I should get to this part of the function %function", $strings, WATCHDOG_ALERT);
      // It is already in the files dir, and the system is already using it.
      // re-use that entry.
      if ($file = file_ownage_load_file_by_filepath($filepath)) {
        watchdog('file_ownage', 'File entry %filepath already exists in the DB, re-using it.', $strings, WATCHDOG_INFO);
        return $file;
      }
    }
    else {
      // This would be the case if the source was found on the system,
      // but outside the Drupal root.
      watchdog('file_ownage', 'File may need moving from %filepath to %expected_fileuri but preferences do not allow that right now.', $strings, WATCHDOG_DEBUG);
      $copied = file_ownage_file_copy($filepath, $expected_fileuri);
    }

    if ($copied) {
      watchdog('file_ownage', 'I have a copy of %filepath at %expected_fileuri', $strings, WATCHDOG_INFO);
    }
    elseif (file_exists(drupal_realpath($expected_fileuri))) {
      watchdog('file_ownage', 'Expected file was found already at %expected_fileuri', $strings, WATCHDOG_INFO);
    }
    else {
      watchdog('file_ownage', 'Failed to copy file from %filepath to %expected_fileuri. Giving up.', $strings, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Paranoia - Really check that it all worked before proceeding.
  if (file_exists(drupal_realpath($expected_fileuri))) {
    // Make (or retrieve) DB entry now.
    $file = file_ownage_insert_filename_into_db($expected_fileuri, $uid);
    // Core returns $file as an object.
  }
  else {
    $strings['%realpath'] = drupal_realpath($expected_fileuri);
    watchdog('file_ownage', 'File entry %expected_fileuri is still not where I wanted it (%expected_fileuri = "%realpath"). Why?', $strings, WATCHDOG_ERROR);
    return FALSE;
  }

  if (empty($file->fid)) {
    // This means that inserting it into the database failed. Have to give up.
    watchdog('file_ownage', 'File entry %expected_fileuri exists, but could not be inserted in the db. Why?', $strings, WATCHDOG_WARNING);
    return FALSE;
  }
  return $file;
}

/**
 * Utility lookup.
 *
 * @param string $filepath
 *   Real filepath, not a URI yet.
 *
 * @return array
 *   $file data on success
 */
function file_ownage_load_file_by_filepath($filepath) {

  // Assert valid input.
  if (empty($filepath)) {
    watchdog('file_ownage', "Empty filepath given to %function", array('%function' => __FUNCTION__), WATCHDOG_ERROR);
    return FALSE;
  }
  // Due to upgrade, sometimes this is run on filepaths, sometimes
  // on already fixed URIs.
  if (file_valid_uri($filepath)) {
    // cool.
    $file_uri = $filepath;
  }
  else {
    // Need to figure what the URI would be for this file.
    // No API for this? best guess was based on system_update_7034
    $file_uri_path = str_replace(drupal_realpath('public://') . '/', '', drupal_realpath($filepath));
    $file_uri = file_build_uri($file_uri_path);
  }

  // Checking db for {$file->uri}
  if ($file = file_ownage_get_file_by_uri($file_uri)) {
    $strings = array(
      '%filepath' => $file->filepath,
      '%file_uri' => $file_uri,
      '%fid' => $file->fid,
    );
    watchdog('file_ownage', "%filepath (%file_uri) is already registered in the DB as fid:%fid", $strings, WATCHDOG_DEBUG);
    // BUT, one strange occasions someone may have even just deleted the
    // file we expect. Double-check.
    if (!file_exists($file->uri)) {
      watchdog('file_ownage', "%file_uri was registered in the DB as fid:%fid but has mysteriously vanished. I'd better remove that entry.", array('%file_uri' => $file->uri, '%fid' => $file->fid), WATCHDOG_WARNING);
      file_delete($file);
      return FALSE;
    }
    return $file;
  }

  watchdog('file_ownage', "%filepath (%file_uri) is so far unknown in the DB", array('%filepath' => $filepath, '%file_uri' => $file_uri), WATCHDOG_INFO);
  return FALSE;
}

/**
 * Util to lookupfile entities.
 *
 * @param string $path
 *   File URI.
 *
 * @return NULL|object
 *   The found $file object.
 */
function file_ownage_get_file_by_uri($uri) {
  $found = file_load_multiple(array(), array('uri' => $uri));
  // There can only be one, surely.
  return reset($found);
}

/**
 * Create a database file entry and get the new fid.
 *
 * @param string $fileuri
 *   Filepath or URL.
 * @param int $uid
 *   User ID.
 *
 * @return object
 *   The $file description data, should include a $file->fid;
 */
function file_ownage_insert_filename_into_db($fileuri, $uid = NULL) {

  $strings = array('!filepath' => l($fileuri, $fileuri));
  if (!file_exists(drupal_realpath($fileuri))) {
    watchdog('file_ownage', "NOT inserting file '!filepath' to db as it's invalid/unavailable", $strings, WATCHDOG_WARNING);
  }

  // $file = file_uri_to_object($fileuri);
  $file = (object) array(
    'uri' => $fileuri,
    'filename' => basename($fileuri),
    'filemime' => file_get_mimetype($fileuri),
    'status' => FILE_STATUS_PERMANENT,
  );

  // Almost ready to make a DB record.
  if ($uid) {
    $file->uid = $uid;
  }
  else {
    global $user;
    $file->uid = $user->uid;
  }
  drupal_chmod($file->uri);
  file_save($file);

  if (empty($file->fid)) {
    // This means that inserting it into the database failed. Have to give up
    // Unknown cause, can this even happen?
    watchdog('file_ownage', 'File entry !filepath could not be inserted in the db. Something low happened', $strings, WATCHDOG_WARNING);
    return FALSE;
  }

  watchdog('file_ownage', "Registered !filepath as a new entry in the DB", $strings, WATCHDOG_INFO);
  return $file;
}

/**
 * Ensure that the DOM Element link attribute is set to the current filepath.
 *
 * The $file data array we are given must have an element attached, which
 * will be part of the DOM being worked on.
 * We update it directly and the update happens by reference.
 *
 * The DOM Element may be an img or an a. Or others?
 * So we figure it out on the fly.
 *
 * @param array $tag
 *   Tag info extracted from the DOM, including an amount of deductions
 *   made so far.
 * @param array $settings
 *   Context & Config.
 *
 * @return bool
 *   Whether any change was actually made.
 */
function file_ownage_update_element_path(array $tag, $settings = array()) {

  $attr = '';
  // We may be dealing with img src or a hrefs here. Don't actually know.
  if ($tag['element']->nodeName == 'a') {
    $attr = 'href';
  }
  if ($tag['element']->nodeName == 'img') {
    $attr = 'src';
  }
  // TODO: NOT WANT this to be absolute
  // $url = file_create_url($tag['uri']);
  $url = url(file_ownage_create_root_relative($tag['uri'], $settings));

  if ($attr && ($tag['element']->getAttribute($attr) != $url)) {
    $tag['element']->setAttribute($attr, $url);
    watchdog('file_ownage', 'Relinked element %attr to point at <a href="!url">!url</a>.', array('!url' => $url, '%attr' => $attr), WATCHDOG_NOTICE);
    return TRUE;
  }
  return FALSE;
}


/**
 * Returns true if the given filepath seems to have a suffix matching the list.
 *
 * @param string $filepath
 *   File URI or URL.
 * @param string $suffixes
 *   List of suffixes we want to process.
 *
 * @return bool
 *   Is this a useful file?
 */
function file_ownage_wanted_filetype($filepath, $suffixes) {
  $suffix_list = explode(',', $suffixes);
  $suffix_list = array_map('trim', $suffix_list);
  $suffix = pathinfo($filepath, PATHINFO_EXTENSION);
  if (in_array($suffix, $suffix_list)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Search in alternative locations for the expected file.
 *
 * If a better location is found, $filepath will be updated by reference.
 *
 * The settings should define a list of paths - these can be
 * remote URLs,
 * local filesystem paths either inside or outside webroot,
 * or local SEARCH paths to try scanning from.
 *
 * eg
 * http://example.com/downloads/
 * /var/backups/oldsite/images/
 * sites/default/files/imported/
 * /var/backups/files/*
 *
 * If it's not a search, the exact path will be checked.
 * If it is a search, the exact path, and then anything
 * approximating it (by truncating the start of the search string)
 * will be looked for.
 *
 * TODO start thinking about security implications of allowing a
 * filesystem scan that grabs files from elsewhere on the server
 * and makes them public!
 * Currently, this is by design, as we want /var/backups/oldsite/files/*
 * to be a valid source. But...
 *
 * @param string $filepath
 *   Gets modified by reference.
 * @param array $settings
 *   Configs & Context.
 *
 * @return string|bool
 *   The location status. FALSE if not found.
 */
function file_ownage_search_for_lost_files(&$filepath, array $settings) {

  if (empty($settings['seek_paths'])) {
    return FALSE;
  }
  watchdog('file_ownage', 'File %filepath is not found yet. Starting to scan alternate fallback locations it may be.', array('%filepath' => $filepath), WATCHDOG_NOTICE);
  // This process MAY produce false matches, but is often correct,
  // and better than the broken link you currently have.
  $seek_paths = $settings['seek_paths'];
  $seek_paths = array_map('trim', $seek_paths);
  $status = FALSE;
  foreach ($seek_paths as $search_path) {
    // There are two types of scan, absolute and wildcard glob.
    // See if the file is *anywhere* under the given filepath.
    if (preg_match('%\*$%', $search_path)) {
      // Need to glob
      // Try to use as much of the path as we can,
      // then chop it back to get more random.
      // Use the shell find, because otherwise we have to do a LOT of work.
      $dir = escapeshellarg(preg_replace('%/\*%', '', $search_path));
      $excludes = implode(
        ' ',
        array(
          "! -path '*/imagecache/*'",
          "! -path '*/tmp/*'",
        )
      );
      // Trim the search string until something turns up.
      $search_for = $filepath;
      while (!empty($search_for)) {
        $strings = array(
          '%search_for' => $search_for,
          '%search_path' => $search_path,
        );
        watchdog('file_ownage', 'Searching filesystem under %search_path to try and find something matching %search_for', $strings, WATCHDOG_DEBUG);

        $search = " -path " . escapeshellarg('*/' . $search_for);
        $command = "find $dir $excludes $search";
        $output = array();
        $return = exec($command . ' 2>&1', $output, $result);

        // If the $output has anything in, use the shortest one we find.
        if ($result) {
          $strings = array(
            '%command' => $command,
            '%output' => implode("\n", $output),
            '%return' => $return,
          );
          watchdog('file_ownage', 'File search failed. I ran <pre>%command</pre> and the response was: <pre>%output</pre>', $strings, WATCHDOG_WARNING);
        }
        elseif (!empty($output)) {
          // Cheap order sort. there may be collisions, don't care
          // Create a list indexed by string length,
          // sort by keys and pop the top.
          $ordered_output = array_combine(array_map('strlen', $output), $output);
          ksort($ordered_output);
          $filepath = reset($ordered_output);
          $filepath = realpath($filepath);
          watchdog('file_ownage', 'Looks like we found %filepath at %search_path after searching!!', array('%filepath' => $filepath, '%search_path' => $search_path), WATCHDOG_INFO);
          return file_ownage_path_status($filepath);
        }
        // If not found yet, shorten the search pattern from the left.
        // Eventually we will just end with the filename.
        $search_for = preg_replace('%^[^/]*/?%', '', $search_for);
      } // end loop that shortens the search parameter
    }
    else {
      $possible_path = $search_path . $filepath;
      // We can't yet be sure if remote files exist.
      if ($status = file_ownage_path_status($possible_path)) {
        if ($status == FILE_OWNAGE_IS_REMOTE) {
          watchdog('file_ownage', 'We are now hoping that %filepath is at remote location %possible_path', array('%filepath' => $filepath, '%possible_path' => $possible_path), WATCHDOG_INFO);
        }
        else {
          watchdog('file_ownage', 'Looks like we found %filepath at %possible_path after all', array('%filepath' => $filepath, '%possible_path' => $possible_path), WATCHDOG_INFO);
        }
        $filepath = $possible_path;
        return $status;
      }
    }
  }

  return $status;
}


/**
 * Return the path we ACTUALLY want to save the named file in.
 *
 * TODO - extend this to maybe support more directories for different filetypes
 * or other logic.
 *
 * Repair silly characters while we are here.
 *
 * @param string $src
 *   probably a full URL. Definately a filename of some sort.
 * @param array $settings
 *   Maybe useful for context.
 *
 * @return string
 *   A cleaned up file Drupal URI, defining where we want this to go.
 */
function file_ownage_correct_path($src, array $settings) {

  // First, strip of the files folder if it's there. (D6 legacy or hard-coded)
  $safe_filename = str_replace(variable_get('file_public_path', conf_path() . '/files'), '', $src);

  // Settings may be able to tell us about redundant folder names etc
  // that we can remove.
  foreach ((array) $settings['strip_paths'] as $pattern) {
    $safe_filename = preg_replace('%' . $pattern . '%', '', $safe_filename);
  }
  // Get URL-encoded characters.
  $safe_filename = preg_replace('/\%\d\d/', '-', $safe_filename);
  // Get double slashes.
  $safe_filename = preg_replace('@/+@', '/', $safe_filename);
  // Get wierd characters.
  $safe_filename = preg_replace('%[^a-zA-Z0-9_\./]%', '-', $safe_filename);
  // Save to a designated subfolder.
  if (!empty($settings['storage_path'])) {
    $safe_filename = trim($settings['storage_path'], '/') . '/' . $safe_filename;
  }
  $ret = file_build_uri(ltrim($safe_filename, '/'));
  return $ret;
}

/**
 * Return a path suitable for linking locally.
 *
 * File_create_url() is always absolute,
 * but I don't want absolute links in my HTML.
 *
 * Takes 'public://path/file.ext' and returns
 * '/sites/default/files/path/file.ext'
 *
 * @param string $uri
 *   Given file URI.
 * @param array $settings
 *   Context and configs.
 *
 * @return string
 *   New path.
 */
function file_ownage_create_root_relative($uri, array $settings) {

  if (!empty($settings['prettyfiles'])) {
    // We actually want to create an URL like /files/filename.jpg
    // and chop off the sites/{sitename} bit. See Aegir prettyfiles.
    // This could probably be done globally via hook_file_url_alter()
    // but we really just need to chop off the scheme.
    return str_replace('public://', 'files/', $uri);
  }
  return str_replace(url('', array('absolute' => TRUE)), '', file_create_url($uri));
}


/**
 * Inspects the given path to see if it's remote, nearby or already local.
 *
 * As a special case, aegir-style prettyfiles count as local and correct.
 *
 * This has a bunch of error catching to make good guesses when given
 * almost-valid strings, as it's from a 'src' as found in markup.
 * Semi-invalid or relative URLs should still be resolved if possible.
 *
 * @param string $src
 *   May be updated by reference to correct some URL encoding
 *   or relative path fixes.
 *
 * @return int
 *   Status.
 */
function file_ownage_path_status(&$src) {

  $is_remote = FALSE;
  $is_nearby = FALSE;
  $strings['!src'] = l($src, $src);

  // Drupal valid_url() does not validate URLs with spaces.
  // Which is correct , but I have to allow these in from bad input
  // (I fix them locally)
  // if (parse_url($src, PHP_URL_SCHEME)) {
  if (valid_url($src, TRUE)) {
    watchdog('file_ownage', "The file '!src' is <b>absolute</b>.", $strings, WATCHDOG_DEBUG);
    $localized = str_replace(url('', array('absolute' => TRUE)), '', $src);
    if ($localized != $src) {
      // Localizing succeeded - which means it's really local.
      watchdog('file_ownage', "'!src' is really just <b>local and absolute</b>.", $strings, WATCHDOG_DEBUG);
      $src = $localized;
    }
    else {
      watchdog('file_ownage', "The file '!src' is <b>remote</b>.", $strings, WATCHDOG_DEBUG);
      return FILE_OWNAGE_IS_REMOTE;
    }
  }

  if (substr($src, 0, 1) == '/' && is_readable($src)) {
    watchdog('file_ownage', "The file '!src' is <b>on the filesystem</b>.", $strings, WATCHDOG_DEBUG);
    return FILE_OWNAGE_IS_ON_FILESYSTEM;
  }

  // Non-URL links are surely server-root-relative? I can't do relative.
  $working_src = ltrim($src, '/');

  // If it's already inside our files dir, tag it and move on.
  // Drupal core file_check_location went away. No replacement?
  // Question is - is it in our public files directory?
  $realpath = realpath($working_src);
  $container = drupal_realpath('public://');
  $is_local = (strpos($realpath, $container) == 0);

  if ($is_local) {
    watchdog('file_ownage', "The file '!src' is <b>local</b>.", $strings, WATCHDOG_DEBUG);
    // The src we should work with is the local version.
    $working_src = str_replace(drupal_realpath('public://') . '/', '', drupal_realpath($realpath));

    // The least we can do is double-check the file exists.
    if (is_file($realpath) && is_readable($realpath)) {
      watchdog('file_ownage', "The file '!src' is <b>local</b> and stored right. Carry on", $strings, WATCHDOG_DEBUG);
      $src = $working_src;

      // And is it already registered in the files table?
      $file = file_ownage_load_file_by_filepath($src);
      if ($file) {
        return FILE_OWNAGE_IS_REGISTERED;
      }
      // Not registered, but still in the right folders.
      return FILE_OWNAGE_IS_LOCAL;
    }
    else {
      watchdog('file_ownage', "Looks like trouble with the files dir link to '!src'. It wasn't readable or found at the expected location inside the files directory.", $strings, WATCHDOG_WARNING);
      return FALSE;
    }
  }
  else {
    // Path was not underneath files.
    // Still should check it exists.
    if (!is_readable($working_src) && !is_readable($src)) {
      watchdog('file_ownage', "Looks like trouble with the nearby link to '!src'. It wasn't readable or found at the expected location. ", $strings, WATCHDOG_WARNING);
      return FALSE;
    }
    watchdog('file_ownage', "'!src' is apparently <b>local</b>, but not stored in the $container dir.", $strings, WATCHDOG_DEBUG);
    return FILE_OWNAGE_IS_NEARBY;
  }

}


/**
 * Check if a file uri is considered local or not.
 *
 * Used in my local wrappers to file_exists() and file_copy()
 *
 * Stolen from file_entity.module.
 *
 * @param string $uri
 *   A file uri, likely either public://something or http://something.
 *
 * @return bool
 *   TRUE if the file is using a local stream wrapper, or FALSE otherwise.
 */
function file_ownage_file_is_local($uri) {
  $scheme = file_uri_scheme($uri);
  $wrappers = file_get_stream_wrappers(STREAM_WRAPPERS_LOCAL);
  return !empty($wrappers[$scheme]) && empty($wrappers[$scheme]['remote']);
}


/**
 * Adaptation of file_exists(), also checks remote URLs.
 *
 * @param string $uri
 *   Local or remote place to look for a file.
 *
 * @returns bool:string
 *   FALSE on failure, the URI on success.
 */
function file_ownage_file_exists($uri) {
  $found_uri = FALSE;
  if (file_ownage_file_is_local($uri)) {
    if (file_exists($uri)) {
      $found_uri = $uri;
    }
  }
  else {
    // Non-local URL scheme.
    if (file_ownage_remote_file_exists($uri)) {
      $found_uri = $uri;
    }
  }
  return $found_uri;
}


/**
 * Can we fetch the given URL?
 *
 * A quick(?) stand-in for file_exists to use on external URLs.
 * Your network behaviour may well make this a very slow call!
 * To avoid getting stuck with that,
 * it's got a 2 second timeout to fail fast.
 *
 * @param string $uri
 *   URI to check.
 *
 * @return bool
 *   Found?
 */
function file_ownage_remote_file_exists($uri) {
  // Why is cleaning up a URI MY job? Sheesh!.
  $uri = preg_replace('| |', '%20', $uri);
  $response = drupal_http_request($uri, array('method' => 'HEAD', 'timeout' => 2));
  if (empty($response)) {
    return FALSE;
  }
  if ($response->code != 200) {
    $file_exists = FALSE;
  }
  else {
    $file_exists = TRUE;
  }
  return $file_exists;
}


/**
 * Copy a possibly remote file to our local destination.
 *
 * Possibly renaming it as we go.
 *
 * Actually downloads to a temp file, then compares the temp with anything at
 * the planned destination.
 * If it matches, the path is already OK and the copy discarded.
 * This returns TRUE, as if a copy really happened.
 *
 * If not, a duplicate filename is generated and new content placed there. $dst
 * is modified by ref.
 *
 * @param string $src
 *   Source filepath, uri or whatever.
 * @param string $dst
 *   Destination filepath.
 *
 * @return bool
 *   Success. $dst may be updated to the real path that got saved.
 */
function file_ownage_file_copy($src, &$dst) {
  $local = file_ownage_file_is_local($src);
  if (empty($src) || empty($dst)) {
    watchdog(
      'file_ownage', 'Bad parameters to file_ownage_file_copy. $src = "!src" and $dst = "!dst". ' .
      'Neither of these can be empty.', array('!src' => $src, '!dst' => $dst), WATCHDOG_ERROR
    );
    return FALSE;
  }
  $strings['!src'] = l($src, $src);
  $strings['!dst'] = l($dst, $dst);
  $strings['%local'] = '%local' ? 'local' : 'remotely sourced';
  watchdog('file_ownage', "Copying! (unless we already have it) %local !src to !dst.", $strings, WATCHDOG_INFO);

  // Drupal file_copy will not handle URLs.
  // Could try drupal_http_request for remotes, but trying by hand today.
  // Need to make it local before we try that.
  $temppath = tempnam(file_directory_temp(), 'tmp_');

  if ($local) {
    watchdog('file_ownage', "Making a local copy of %src", $strings, WATCHDOG_DEBUG);
    if (!copy($src, $temppath)) {
      watchdog('file_ownage', "Copy of %src to %temppath failed", $strings, WATCHDOG_ERROR);
      return FALSE;
    }
  }
  else {
    // REMOTE.
    watchdog('file_ownage', "Taking a copy of remote !src", $strings, WATCHDOG_DEBUG);
    if (!file_ownage_remote_file_copy($src, $temppath)) {
      watchdog('file_ownage', "Copy of %src to %temppath failed", $strings, WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // So far so good.
  // Should check the file a little, ensure that it's valid and all.
  // If we expected an image, check it's an image.
  // If we don't know what to expect, can't validate more than that.
  $suffix = strtolower(pathinfo($temppath, PATHINFO_EXTENSION));
  if (in_array($suffix, array('jpg', 'gif', 'png', 'jpeg'))) {
    $image_info = image_get_info($temppath);
    if (!$image_info || !isset($image_info['extension'])) {
      watchdog('file_ownage', "Retrieved file from $src ($temppath) is not a valid image. Only JPG, PNG and GIF files are allowed.", array(), WATCHDOG_ERROR);
      unlink($temppath);
      return FALSE;
    }
  }

  // Move with possible renaming.
  // ONLY IF THE EXISTING FILE DOES NOT LOOK LIKE A DUPE.
  if (file_exists($dst)) {
    if (@filesize($dst) == @filesize($temppath)) {
      watchdog('file_ownage', "Name and filesize match between %src and the pre-existing %dst. I'll call that a duplicate and link to it.", array('%src' => $src, '%dst' => $dst), WATCHDOG_ERROR);
      // Can just return and pretend the $dst is corrent.
      file_unmanaged_delete($temppath);
      return TRUE;
    }
    else {
      watchdog('file_ownage', "Possible conflict of names. %src is attempting to overwrite %dst. They are not identical, so a renamed COPY will be made instead.", array('%src' => $src, '%dst' => $dst), WATCHDOG_ERROR);
    }
  }

  // By now we guess it is NOT a dupe.
  // Use drupal file move because it handles auto-renaming
  $target_dir = dirname($dst);
  file_prepare_directory($target_dir, FILE_CREATE_DIRECTORY);
  if ($dst = file_unmanaged_move($temppath, $dst, FILE_EXISTS_RENAME)) {
    $strings = array('!src' => l($src, $src), '!dst' => l($dst, $dst));
    watchdog('file_ownage', "Moved downloaded copy from !src over to !dst.", $strings, WATCHDOG_INFO);
    return TRUE;
  }
  else {
    watchdog('file_ownage', "Failed to transfer our local copy of $src ($temppath) over to $dst.", array(), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Remote-compatible version of file_copy().
 *
 * Uses drupal_http_request and checks the response code.
 *
 * @param string $source
 *   Probably an http URL.
 * @param string $destination
 *   Local storage filepath.
 *
 * @return bool
 *   Success.
 */
function file_ownage_remote_file_copy($source, $destination) {
  // Why is cleaning up a URI MY job? Sheesh!.
  $source = preg_replace('| |', '%20', $source);
  // And why doesn't native file_copy() work for me?
  // I guess we need to respect proxy, timeouts and things.
  $response = drupal_http_request($source, array('timeout' => 10));
  if ($response->code == 200) {
    // Prepare local target directory and save downloaded file.
    $target_dir = dirname($destination);
    if (file_prepare_directory($target_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      file_put_contents($destination, $response->data);
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Older super-manual method to file_ownage_remote_file_copy().
 *
 * @param string $source
 *   Probably an http URL.
 * @param string $destination
 *   Local storage filepath.
 *
 * @return bool
 *   Success.
 *
 * @deprecated
 */
function file_ownage_remote_file_copy_stream($source, $destination) {

  // Don't use fopen, it doesn't do context until PHP5.3
  // Retrieve, with context to fudge the headers and bypass some anti-leech
  // DOES NOT work on all antileech.
  $opts = array(
    'http' => array(
      'header' => "Referer: $source\r\n",
    ),
  );
  $context = stream_context_create($opts);
  // Why is cleaning up a URI MY job? Sheesh!.
  $source = preg_replace('| |', '%20', $source);
  $file_src = fopen($source, 'r', FALSE, $context);
  if (!$file_src) {
    watchdog('file_ownage', "Problem in file_ownage_file_copy fetching remote file %source", array('%source' => $source), WATCHDOG_ERROR);
    return FALSE;
  }
  $file_dst = fopen($destination, 'w');
  while (!feof($file_src)) {
    fwrite($file_dst, fread($file_src, 8192));
  }
  fclose($file_src);
  fclose($file_dst);
  return TRUE;
}


/**
 * Make Imageholder Node, a node that exists mainly to contain a filefield.
 *
 * Details of the content type and target filefield name
 * should be in the settings.
 *
 * @var object $file
 *   A file object
 * @var stdClass $template_node
 *   Entity with some settings on to use as a template when creating
 *   a fileholder. Passing this in allows you to define some useful values.
 * @var array $settings
 *   Context and configs.
 *
 * @return object
 *   The fileholder node
 */
function file_ownage_make_fileholder_node($file, $template_node, $settings) {

  // Assert valid input;
  if (empty($settings['fileholderholder_type'])
        || empty($settings['filefield_name'])
    ) {
    watchdog('file_ownage', 'null settings. This function requires a settings array that defines a fileholder_type and a filefield_name. They were not passed in.', array(), WATCHDOG_ERROR);
    return NULL;
  }
  $imageholder_type = $settings['fileholder_type'];

  if (empty($file['fid']) || !file_exists($file['filepath'])) {
    watchdog('file_ownage', 'Invalid file entry. fid or filepath did not represent a real file. Not making a fileholder node.', array(), WATCHDOG_ERROR);
    return NULL;
  }

  // We have a file object.  Make an image node from it and return
  // Non-standard additional strings title, caption, description, alt
  // may be available on the file object.
  //
  // Values from the template node MAY be copied down,
  // node->uid and maybe node->taxonomy
  $title = !empty($file['title']) ? $file['title'] : $file['filename'];
  $caption = isset($file['caption']) ? $file['caption'] : '';

  if (empty($title)) {
    trigger_error('Cant save an image node without a title');
    return NULL;
  }
  watchdog('file_ownage', "Creating an fileholder node to manage the file %file", array('%file' => $file['filepath']), WATCHDOG_DEBUG);

  $node = new stdClass();
  $node->type = $imageholder_type;
  $node->status = 1;
  // $node->uid = $uid;
  $node->title = $title;
  $node->body = $caption;
  $node->created = REQUEST_TIME;
  $node->changed = $node->created;
  $node->promote = 1;
  $node->sticky = 0;
  $node->format = 2;
  $node->pathauto_perform_alias = FALSE;

  if ($template_node) {
    // Copy any safe values into it.
    foreach ($template_node as $var => $val) {
      $node->$var = $val;
    }
  }
  else {
    global $user;
    $node->uid = $user->uid;
  }

  // $node->$filefield_name = array(0 => field_file_load($file['fid']));
  // Call our own subroutine to attach the actual filefield,
  // it's got more error-checking
  file_ownage_attach_filefield($node, $file, $settings);

  node_save($node);
  $strings = array(
    '!node' => l($node->title, 'node/' . $node->nid),
    '%file' => $file['filepath'],
  );
  watchdog('file_ownage', "New fileholder node !node created to manage the file %file", $strings, WATCHDOG_DEBUG);
  return $node;
}


/**
 * Returns a preset.
 *
 * @param array $img
 *   Array containing image.
 *
 * @return string
 *   Preset identifier.
 */
function file_ownage_select_preset(array $img) {
  // At this point we only deal with one preset value.
  $preset = reset($img['meta']['preset']);
  watchdog('file_ownage', 'Selected a preset to use for this image, Returning %preset', array('%preset' => $preset), WATCHDOG_DEBUG);
  return $preset;
}
