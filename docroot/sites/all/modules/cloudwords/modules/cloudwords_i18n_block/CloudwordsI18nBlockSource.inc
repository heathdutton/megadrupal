<?php

/**
 * @file
 * Contains CloudwordsI18nBlockSource.
 */

class CloudwordsI18nBlockSource extends CloudwordsI18nStringSource {

  /**
   * Overrides CloudwordsI18nStringSource::loadObject().
   */
  protected function loadObject($bid) {
    return block_load('block', $bid);
  }

}
