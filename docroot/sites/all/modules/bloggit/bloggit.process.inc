<?php

/**
 * @file
 * Processing code, handles the process of turning files into blog posts.
 */

/**
 * Process available blog files, turning them into bloggit nodes.
 */
function _bloggit_process_files() {
  $configuration = variable_get('bloggit_blog', array());
  $entries = array();

  foreach (bloggit_find_entries($configuration) as $key => $entry) {
    bloggit_load_entry($entry);
    bloggit_save_entry($entry, $configuration);

    $entries[$key] = $entry;
  }

  return $entries;
}

/**
 * Find blog entries in a blog directory.
 *
 * @param $configuration
 *   Array with configuration for a bloggit instance.
 * @return array
 *   Dirnames of blogposts.
 */
function bloggit_find_entries($configuration) {
  $entries = array();

  if (empty($configuration['path'])) {
    watchdog('bloggit', 'bloggit_find_entries was run without a path set in bloggit configuration.', array(), WATCHDOG_ERROR);
    return array();
  }

  $path = realpath($configuration['path']);

  if (is_dir($path)) {
    $handle = opendir($path);

    while (false !== ($dir = readdir($handle))) {
      // Filter out hidden files and CVS dirs.
      if ($dir == 'CVS' || strpos($dir, '.') === 0) {
        continue;
      }

      $full_path = $path . '/' . $dir;

      if (is_dir($full_path)) {
        $entries[$dir] = array(
          'dirname' => $dir,
          'path' => $full_path,
        );
      }
    }

    closedir($handle);
  }
  else {
    watchdog('bloggit', 'Path %path does not exist, cannot be scanned for new blog posts.', array('%path' => $path), WATCHDOG_ERROR);
  }

  return $entries;
}

/**
 * Load an entry from its directory.
 */
function bloggit_load_entry(&$entry) {
  // Find metadata files. Currently, only JSON and YAML is supported.
  foreach (glob($entry['path'] . '/metadata.*') as $filename) {
    $entry += bloggit_load_metadata($filename);
  }

  // Find content files.
  foreach (glob($entry['path'] . '/content.*') as $filename) {
    $extension = array_pop(explode('.', $filename));

    // Interpret the contents as markdown.
    if (in_array($extension, array('md', 'mkd', 'mkdn', 'mdown', 'markdown')) && function_exists('_filter_markdown')) {
      $entry['content'][] = _filter_markdown(file_get_contents($filename), NULL);
    }
    // If file format is not well-known, just include as-is.
    else {
      $entry['content'][] = file_get_contents($filename);
    }
  }

  // Load the stored metadata from the databases.
  $query = db_query('SELECT * FROM {bloggit} WHERE dirname = :dirname LIMIT 1;', array(
    ':dirname' => $entry['dirname'],
  ));
  $result = $query->fetchAssoc();

  if ($result) {
    $entry += $result;
  }

  // Convert publish to Unix timestamp.
  if (!empty($entry['publish'])) {
    $entry['publish'] = strtotime($entry['publish']);
  }
}

/**
 * Load Bloggit entry metadata from file.
 *
 * @param $metafile
 *   Path/filename for the metadata file.
 * @return
 *   Array containing the metadata loaded from the file.
 */
function bloggit_load_metadata($metafile) {
  $extension = array_pop(explode('.', $metafile));

  // For JSON files, we can just use PHPs built-in json_decode.
  if ($extension == 'json') {
    $contents = file_get_contents($metafile);

    if (!$contents) {
      watchdog('bloggit', 'Failed to load metadata from %file.', array('%file' => $metafile), WATCHDOG_ERROR);
    }
    else {
      $json = json_decode($contents, TRUE);
      if (is_array($json)) {
        return $json;
      }
      elseif (is_null($json)) {
        watchdog('bloggit', 'Failed to decode JSON from %file. Check it for syntax errors.', array('%file' => $metafile), WATCHDOG_ERROR);
      }
    }
  }
  // YAML files require a library. We ship one.
  elseif (in_array($extension, array('yml', 'yaml'))) {
    if (!class_exists('Spyc')) {
      $path = drupal_get_path('module', 'bloggit');
      include($path . '/lib/spyc.php');
    }

    return Spyc::YAMLLoad($metafile);
  }

  // If we failed to load anything, just return an array.
  return array();
}

/**
 * Save a loaded entry to the database.
 *
 * Updates existing nodes and creates new for non-existing ones.
 */
function bloggit_save_entry(&$entry, $configuration) {
  $created = FALSE;

  // If there is not a node for the blog post already and it has a 
  // title, create a node and save it to the database.
  if (!isset($entry['nid']) && !empty($entry['title'])) {
    $node = (object) array(
      'title' => $entry['title'],
      'type' => 'bloggit',
      'language' => LANGUAGE_NONE,
      'is_new' => TRUE,
    );

    // Invokes hook_node_prepare to get default values, etc.
    node_object_prepare($node);

    $node->created = (isset($entry['created'])) ? strtotime($entry['created']) : REQUEST_TIME;
    $node->changed = (isset($entry['changed'])) ? strtotime($entry['changed']) : $node->created;

    node_save($node);

    $record = array(
      'dirname' => $entry['dirname'],
      'nid' => $node->nid,
      'rendered_content' => implode("\n", $entry['content']),
    );

    if (!empty($entry['slug'])) {
      $record['slug'] = $entry['slug'];
    }

    if (!empty($entry['teaser'])) {
      $record['teaser'] = $entry['teaser'];
    }

    // If there is a default author set, then it gets to own the node.
    if (isset($configuration['author_uid'])) {
      $node->uid = $configuration['author_uid'];
    }

    $created = drupal_write_record('bloggit', $record);
  }
  // If entry is connected to a node already, just load it.
  else {
    $node = node_load($entry['nid']);

    // Bail if node could not be loaded.
    if (!$node) {
      watchdog('bloggit', 'Failed to load node for entry %title from %dirname.', array(
        '%title' => $entry['title'],
        '%dirname' => $entry['dirname'],
      ), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  if (isset($configuration['categories_field']) && isset($configuration['categories_vid'])) {
    bloggit_process_convert_terms_array($node, $entry['categories'], $configuration['categories_vid'], $configuration['categories_field']);
  }

  if (isset($configuration['tags_field']) && isset($configuration['tags_vid'])) {
    bloggit_process_convert_terms_array($node, $entry['tags'], $configuration['tags_vid'], $configuration['tags_field']);
  }

  // Update title and content if created already.
  if (!$created) {
    $node->title = $entry['title'];
    $entry['rendered_content'] = implode("\n", $entry['content']);
  }

  // Update creation timestamp if applicable.
  $node->created = (isset($entry['created'])) ? strtotime($entry['created']) : $node->created;

  // Save the node with the updated data.
  node_save($node);

  // Update the bloggit entry as well.
  drupal_write_record('bloggit', $entry, array('dirname'));

  return $node;
}

/**
 * Find or create a taxonomy term.
 *
 * Most of this stuff is grabbed from taxonomy_field_validate().
 *
 * @param $name
 *   The name of the term we want to find.
 * @param $vid
 *   The vocabulary id the term is to be found/created in.
 * @return
 *   An array of term data, ready for putting on a node to be saved.
 */
function bloggit_get_or_create_term($name, $vid) {
  // Basic condition is the name of the term.
  $conditions = array(
    'name' => trim($name),
    'vid' => (integer) $vid,
  );
  
  // See if the term exists in the chosen vocabulary and return the tid;
  // otherwise, create a new 'autocreate' term for insert/update.
  if ($possibilities = taxonomy_term_load_multiple(array(), $conditions)) {
    $term = array_pop($possibilities);

    return (array) $term;
  }
  else {
    $vocabulary = taxonomy_vocabulary_load($vid);
    return array(
      'tid' => 'autocreate',
      'vid' => $vid,
      'name' => trim($name),
      'vocabulary_machine_name' => $vocabulary->machine_name,
    );
  }
}

/**
 * Convert array of term names to savable data on a node.
 *
 * Made to fit the data that should be stuffed into node_save().
 *
 * @param $node
 *   The node we're working on.
 * @param $names
 *   Array of term names.
 * @param $vid
 *   Vocabulary ID for the terms.
 * @param $field_name
 *   Programmatic name of the field the nodes should be put into.
 */
function bloggit_process_convert_terms_array($node, $names, $vid, $field_name) {
  foreach (array_filter($names) as $name) {
    $term = bloggit_get_or_create_term($name, $vid);

    $node->{$field_name}[$node->language][] = $term;
  }
}

