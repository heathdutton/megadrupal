<?php

/**
 * @file
 * Contains classes for chunking HTML files.
 */

/**
 * Class MTChunkParser
 *
 * Parses a single HTML file into many chunks, each of which is mapped to a
 * separate destination node. Used for 'many to many' migrations where
 * each file contains content for more than one destination.
 *
 * @package migration_tools
 */
abstract class MTChunkParser extends MigrateContentParser {
  /**
   * $this->setContent() will be called by the migrate module once per file.
   * Other methods can rely on this being set.
   */
  protected $content;
  protected $sourceParser;
  protected $chunks;

  /**
   * {@inheritdoc}
   */
  public function setContent($content, $item_id) {
    parent::setContent($content, $item_id);

    // Create a new SourceParser to handle HTML content.
    $this->sourceParser = new SimpleSourceParser('placeholder', $content);

    // We don't need the full ID now. Migrate will generate it and make it
    // available in prepareRow().
    $this->setChunks();
  }

  /**
   * Gets $this->chunks.
   *
   * @return QueryPath
   *   A QueryPath object containing one or more elements.
   */
  public function getChunks() {
    if (!isset($this->chunks)) {
      $this->setChunks();
    }
    return $this->chunks;
  }

  /**
   * {@inheritdoc}
   */
  public function getChunkCount() {
    $chunks = $this->getChunks();
    return count($chunks);
  }

  /**
   * Sets $this->chunks.
   *
   * This method should parse that markup in $this->content and return an array
   * of discrete content chunks.
   */
  abstract public function setChunks();
}
