<?php

/**
 * @file
 * Class ObtainBody
 *
 * Contains a collection of stackable finders that can be arranged
 * as needed to obtain a body or other long html content.
 */

/**
 * Class ObtainBody
 *
 * Obtains the HTML body.
 */
class ObtainBody extends ObtainHtml {

  /**
   * Finder method to find the top body.
   *
   * @return string
   *   The string that was found
   */
  protected function findTopBodyHtml() {
    $element = $this->queryPath->top('body');

    return $element->innerHtml();
  }
}
