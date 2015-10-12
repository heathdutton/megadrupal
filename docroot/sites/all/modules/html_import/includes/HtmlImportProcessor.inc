<?php

/**
 * @file
 * Class definition of HtmlImportProcessor.
 */

define('HTML_IMPORT_NBSP', chr(0xC2) . chr(0xA0));

/**
 * Creates nodes from feed items.
 */
class HtmlImportProcessor extends FeedsNodeProcessor {

  protected $nidMap = array(); //mapping b/t item index and entity nid
  protected $parentMap = array(); //mapping b/t entity nid and parent entity nid 
  protected $bookId;
  protected $log;
  protected $sourceConfig;
  public $parentNode;
  public $langcode = LANGUAGE_NONE;

  /**
   * Implementation of FeedsProcessor::process().
   */
  public function process(FeedsSource $source, FeedsParserResult $parser_result) {
    $state = $source->state(FEEDS_PROCESS);

    // Set the parent of the book which is always the feed page
    $this->setParentNode($source->feed_nid);
    // Save parent feed node and add it to a new book structure
    $this->saveParentNode();

    $config = $source->getConfig();
    $this->sourceConfig = $config['HtmlImportFetcher'];
    $parser_result_clone = clone $parser_result;
    while ($item = $parser_result->shiftItem()) {
      // Check if this item already exists.
      $entity_id = $this->existingEntityId($source, $parser_result);
      $skip_existing = $this->config['update_existing'] == FEEDS_SKIP_EXISTING;

      // If it exists, and we are not updating, pass onto the next item.
      if ($entity_id && $skip_existing) {
        continue;
      }

      $hash = $this->hash($item);
      $changed = ($hash !== $this->getHash($entity_id));
      $force_update = $this->config['skip_hash_check'];

      // Do not proceed if the item exists, has not changed, and we're not
      // forcing the update.
      if ($entity_id && !$changed && !$force_update) {
        continue;
      }

      try {

        // Build a new entity.
        if (empty($entity_id)) {
          $entity = $this->newEntity($source);
          $this->newItemInfo($entity, $source->feed_nid, $hash);
        }

        // Load an existing entity.
        else {
          $entity = $this->entityLoad($source, $entity_id);
          // The feeds_item table is always updated with the info for the most recently processed entity.
          // The only carryover is the entity_id.
          $this->newItemInfo($entity, $source->feed_nid, $hash);
          $entity->feeds_item->entity_id = $entity_id;
        }

        // Set property and field values.
        $this->map($source, $parser_result, $entity);
        $this->entityValidate($entity);

        // Allow modules to alter the entity before saving.
        module_invoke_all('feeds_presave', $source, $entity, $item);
        if (module_exists('rules')) {
          rules_invoke_event('feeds_import_' . $source->importer()->id, $entity);
        }

        // Enable modules to skip saving at all.
        if (!empty($entity->feeds_item->skip)) {
          continue;
        }

        // Add taxonomy terms applied to the parent feed node to an imported node
        $this->addTerms($entity);
        // Add images 
        $this->addImages($entity, $item);

        // This will throw an exception on failure.
        $this->entitySaveAccess($entity);

        // Save the entity as a node
        $this->entitySave($entity);
        // Add a mapping b/t item index and entity nid
        $this->setNidMap($entity, $item);
        // Add a mapping b/t item and parent entity/node nid
        $this->setParentMap($entity, $item);

        // Add new entity into a book structure
        $this->addBookStructure($entity, $item);

        // Update images src referenced by imported nodes
        $this->processImageLinks($entity, $item);

        // Track progress.
        if (empty($entity_id)) {
          $state->created++;
        }
        else {
          $state->updated++;
        }

        // Add linked warnings to the page
        $this->addWarnings($entity, $item);

        $state->progress($state->total, $state->total - count($parser_result));

      } // Something bad happened, log it.
      catch (Exception $e) {
        $state->failed++;
        drupal_set_message($e->getMessage(), 'warning');
        $message = $e->getMessage();
        $message .= '<h3>Original item</h3>';
        $message .= '<pre>' . var_export($item, TRUE) . '</pre>';
        $message .= '<h3>Entity</h3>';
        $message .= '<pre>' . var_export($entity, TRUE) . '</pre>';
        $source->log('import', $message, array(), WATCHDOG_ERROR);
      }

    }

    // All entities are saved, loop through all saved entities to rebuild anchors
    // We have to process anchors/refs after all entities are saved to deal with forward references
    /* temporarily bypassing anchorlink processing */
    $parser_result_clone_clone = clone $parser_result_clone;
    while ($item = $parser_result_clone->shiftItem()) {
      if (!empty($item['anchor_refs'])) { // Proceed if current item has references to anchors
        $entity_id = $this->nidMap[$item['index']];
        $entity = $this->entityLoad($source, $entity_id); // the method entityLoad exhibits issues with Postgres
        $entity->body[$this->langcode][0]['value'] = $this->processAnchorLinks($entity, $item, $parser_result_clone_clone);
        try {
          $this->entityValidate($entity);
          // Save the changed entity as a node
          $this->entitySave($entity);
          $state->updated++;
        } catch (Exception $e) {
          $state->failed++;
        }
      }
    }

    // Set messages if we're done.
    if ($source->progressImporting() != FEEDS_BATCH_COMPLETE) {
      return;
    }

    // Purge temp directory
    $this->purgeDirectory($parser_result->files['tempImageDirectory']);

    $info = $this->entityInfo();
    $tokens = array(
      '@entity' => strtolower($info['label']),
      '@entities' => strtolower($info['label plural']),
    );
    $messages = array();
    if ($state->created) {
      $messages[] = array(
        'message' => format_plural(
          $state->created,
          'Created @number @entity.',
          'Created @number @entities.',
          array('@number' => $state->created) + $tokens
        ),
      );
    }
    if ($state->updated) {
      $messages[] = array(
        'message' => format_plural(
          $state->updated,
          'Updated @number @entity.',
          'Updated @number @entities.',
          array('@number' => $state->updated) + $tokens
        ),
      );
    }
    if ($state->failed) {
      $messages[] = array(
        'message' => format_plural(
          $state->failed,
          'Failed importing @number @entity.',
          'Failed importing @number @entities.',
          array('@number' => $state->failed) + $tokens
        ),
        'level' => WATCHDOG_ERROR,
      );
    }
    if (empty($messages)) {
      $messages[] = array(
        'message' => t('There are no new @entities.', array('@entities' => strtolower($info['label plural']))),
      );
    }
    foreach ($messages as $message) {
      drupal_set_message($message['message']);
      $source->log('import', $message['message'], array(), isset($message['level']) ? $message['level'] : WATCHDOG_INFO);
    }
  }


  /**
   * Save a node.
   */
  public function entitySave($entity) {
    // If nid is set and a node with that id doesn't exist, flag as new.
    if (!empty($entity->nid) && !node_load($entity->nid)) {
      $entity->is_new = TRUE;
    }
    node_save($entity);
  }

  /**
   * Implementation of FeedsProcessor::clear().
   */
  public function clear(FeedsSource $source) {
    return;
  }

  /**
   * Update anchor links in HTML to point them to the correct imported nodes
   */
  protected function processAnchorLinks($entity, $item = array(), $parser_result) {
    // Because we are dealing with entities just saved from the above while loop, simply using nid in the map to load the entity        
    $search = array();
    $replace = array();
    foreach ($item['anchor_refs'] as $anchor_ref) {
      foreach ($parser_result->items as $item_2) {
        // Find which item contains the anchor 
        if (in_array($anchor_ref, $item_2['anchors']) && html_import_perm()) {
          // Update the anchor link in entity body to a reference with $item_2's nid
          $search[] = '/\#' . $anchor_ref . '/';
          $replace[] = '/node/' . $this->nidMap[$item_2['index']] . '#' . $anchor_ref;
        }
      }
    }
    if (!empty($search) && !empty($replace)) {
      $replaced_body_content = preg_replace($search, $replace, $entity->body[$this->langcode][0]['value']);
    }
    else {
      $replaced_body_content = $entity->body[$this->langcode][0]['value'];
    }
    return $replaced_body_content;
  }

  /**
   * Add the necessary book structure to the child pages.
   *
   */
  protected function addBookStructure(&$entity, $item = array()) {

    if (!isset($item['index'])) {
      $parent_id = -1;
      $item['index'] = 0;
    }
    else {
      $parent_id = $this->getParentlid($item);
    }

    if (isset($item['index'])) {
      $weight = (int) $item['index'] / 2;
    }
    else {
      $weight = 0;
    }

    // Book id is parent feed node's nid
    $book_id = $this->getBookId();

    $entity->book = array(
      'weight' => $weight,
      'bid' => $book_id,
      'options' => array(),
      'has_children' => 0,
      'module' => 'book',
      'plid' => $parent_id,
    );

    // Add master parent node
    // This is to handle the situation that the feed node is a child of a master node
    // If the feed node has a parent (ie master), all the children of the feed node will have the master's nid in the adv_publication_parent field 
    $parent = $this->parentNode;
    if (isset($parent->field_adv_publication_parent[$this->langcode])) {
      $entity->field_adv_publication_parent[$this->langcode][0] = array(
        'nid' => $parent->field_adv_publication_parent[$this->langcode],
        'access' => 1
      );
    }
    else {
      $entity->field_adv_publication_parent[$this->langcode][0] = array(
        'nid' => $this->parentNode->nid,
        'access' => 1
      );
    }

    // Update node entity
    node_save($entity);
  }

  /**
   * Get the current items parent id
   *
   * @param array $item
   * A batch item array
   *
   * @return int|boolean
   * The parents nid if it exists, else FALSE
   */
  public function getParentlid($item) {
    if (isset($item['parent_id']) && isset($this->parentMap[$item['index']])) {
      $pnid = $this->parentMap[$item['index']];
      $pmlid = db_query("SELECT mlid FROM {book} WHERE nid = :nid", array(':nid' => $pnid))->fetchfield(0);
      return $pmlid;
    }
    return FALSE;
  }

  /**
   * Pass taxonomy terms from the parent node to the child node.
   *
   * These items are taken from a hardcoded array matching cck objects. They
   * may need to be configurable at some point. We don't use the taxonomy array
   * as the site uses a cck taxonomy module.
   *
   * @param object $node A fully loaded node object
   */
  protected function addTerms(&$entity) {

    // TODO: Trigger autotagging
    /*
    if (isset($this->sourceConfig['autotag']) && $this->sourceConfig['autotag']) {
      // call the mighty robot
    }
    */

    // TODO: Make these configurable
    $cck_taxonomy_fields = $this->getInheritedFields();
    foreach ($cck_taxonomy_fields as $field) {
      if (isset($this->parentNode->$field)) {
        $entity->$field = $this->parentNode->$field;
      }
    }
  }

  public function getInheritedFields() {
    $defaults = array(
      'field_topics',
      'field_tags',
      'field_author',
    );
    return variable_get('html_import_inherited_fields', $defaults);
  }

  /**
   * Add warnings to a node log
   *
   * @param object $node
   * A fully loaded node object
   * @param array $item
   * A batch item
   */
  protected function addLog(&$node, $item) {
    foreach ($item['log']['counts'] as $k => $count) {
      if ($count) {
        $this->log['counts'][$k] += $count;
      }
    }
    $node->log = theme('item_list', $item['log']['warnings']);
  }

  /**
   * Add all individual item warnings to the main book container
   *
   * @param object $node
   * A fully loaded node object
   * @param array $item
   * A batch item
   */
  protected function addWarnings($node, $item) {
    if (count($item['log']['warnings'])) {
      $warnings = array();
      foreach ($item['log']['warnings'] as $warning) {
        $warnings[] = l($warning, 'node/' . $node->nid);
      }
      $this->log['warnings'] = array_merge($this->log['warnings'], $warnings);
    }
  }

  protected function addImages(&$entity, $item) {
    if (!empty($item['images'])) {
      foreach ($item['images'] as $image) {
        $file = (object) array(
          'display' => 1,
          'uri' => $image->getValue(),
          'filemime' => file_get_mimetype($image->getValue()),
          'filename' => basename($image->getValue()),
          'status' => 1,
        );

        //we will leave the images in the root of the public files directory and leave the file path management to token and pathauto
        $file = file_copy($file, 'public://');
        $entity->field_html_import_images[$this->langcode][] = (array) $file;
      }
    }
  }

  /**
   * Update the node contents to point the hardcoded links to the correct file
   * paths.
   *
   * @param object $entity
   * A fully loaded feed entity object
   * @param array $item
   * A batch item
   */
  protected function processImageLinks(&$entity, $item) {

    // If the current item being processed has images
    if (!empty($entity->field_html_import_images[$this->langcode]) && html_import_perm()) {
      $search = array();
      $replace = array();
      $originalFiles = $item['images'];
      $imagecache_preset = $this->sourceConfig['imagecache'];

      foreach ($entity->field_html_import_images[$this->langcode] as $key => $image) {
        $originalFileEnclosure = $originalFiles[$key]->getValue();
        $originalFile = array_pop(explode('/', $originalFileEnclosure));
        $originalFile = str_replace('.', '\.', $originalFile);

        /* simply replace file path reference with the new location of the images. 
         * $search_pattern = '~<img[^>]*?'. $originalFile .'[^>]*?>~'; 
         */
        // TODO: use theme() to format <img> tag so we can apply imagecache settings here
        $search_pattern = '~src="*?' . $originalFile . '"~';
        $search[] = $search_pattern;
        $movedFile = DIRECTORY_SEPARATOR . file_stream_wrapper_get_instance_by_scheme(file_default_scheme())->getDirectoryPath() . DIRECTORY_SEPARATOR . file_uri_target($image['uri']);
        $replace[] = 'src="' . $movedFile . '"';
      }

      $entity->body[$this->langcode][0]['value'] = preg_replace($search, $replace, $entity->body[$this->langcode][0]['value']);
      // Update node entity
      $save_result = node_save($entity);
    }
  }

  /**
   * Save the parent node to the current object
   *
   * @param integer $nid
   * The parent nodes nid
   *
   * @return object
   * Returns the fully loaded parent node
   */
  protected function setParentNode($nid) {
    $this->parentNode = node_load($nid);
    return $this->parentNode;
  }

  /**
   * Retrieve the fully loaded parent node for the current feed
   *
   * @return objectThe fully loaded parent node
   */
  protected function getParentNode() {
    if (!empty($this->parentNode)) {
      return $this->parentNode;
    }
  }

  /**
   * Save the fully loaded parent node to the current object.
   */
  protected function saveParentNode() {
    if (!empty($this->parentNode) && !isset($this->parentNode->book['bid'])) {
      $this->parentNode->book = array(
        'bid' => 'new',
        'options' => array(),
        'has_children' => 0,
        'module' => 'book',
      );
      node_save($this->parentNode);
    }
  }

  /**
   * Remove our temporary image files (if possible)
   *
   * @param string $tempDirectory
   * The path to the temporary directory
   */
  protected function purgeDirectory($tempDirectory) {
    if (is_dir($tempDirectory)) {
      // Remove temp files and directory
      $files = scandir($tempDirectory);
      foreach ($files as $file) {
        $file = $tempDirectory . '/' . $file;
        if (!is_dir($file)) {
          unlink($file);
        }
      }
      $success = rmdir($tempDirectory);
      $success = $success && rmdir(dirname($tempDirectory));
      if ($success === FALSE) {
        $contents = scandir($tempDirectory);
        watchdog('Html Import', 'The temporary directory %directory could not be removed. It currently contains the following files: @contents', array(
          '%directory' => $tempDirectory,
          '@contents' => implode(', ', $contents)
        ));
      }
    }
  }

  /**
   * Map the item parent to a node id
   */
  public function setParentMap($entity, $item) {
    if ($item['parent_id']) {
      $this->parentMap[$item['index']] = $this->nidMap[$item['parent_id']];
    }
    else {
      $this->parentMap[$item['index']] = $this->parentNode->nid;
    }
  }

  /**
   * Map an entity to a node id
   */
  public function setNidMap($entity, $item) {
    $this->nidMap[$item['index']] = $entity->nid;
  }

  /**
   * Get bid (book ID) for the current book
   *
   * @return int
   * The book id
   */
  public function getBookId() {
    return $this->parentNode->nid;
  }
}
