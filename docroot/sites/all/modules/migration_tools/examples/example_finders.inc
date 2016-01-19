

/**
 * @file
 * A collection of unique finders or pluckers that demonstrate customization.
 *
 * Each finder is designed to find text, while each plucker is designed to find
 * text and set it for removal in the event it passes validation.
 */

/**
   * Finder method to find #Layer4 and the 5th paragraph on the page first line.
   *
   * @return string
   *   The text found.
   */
  protected function findIdLayer4P5Firstline() {
    $elem = $this->queryPath->find("#Layer4")->siblings('p:nth-of-type(5)');
    $title = $elem->innerHTML();
    $title = self::trimAtBr($title);
    // Since this is a substring we can not remove the entire element,
    // so we have to evaluate the title and if it checks out, then remove the
    // the text and put the rest back.
    $this->extractAndPutBack($title, $elem);

    return $title;
  }


   /**
   * Finder method to find #Layer4 and the 5th paragraph on the page if short.
   *
   * @return string
   *   The text found.
   */
  protected function pluckIdLayer4P5ShortEnough() {
    $elem = $this->queryPath->find("#Layer4")->siblings('p:nth-of-type(5)');
    $this->setElementToRemove($elem);
    $title = $elem->innerHTML();
    // If this value is fairly short, we can use the whole thing.
    $length = drupal_strlen($title);
    if ($length > 210) {
      // Too long, so return empty so it will move on to the next finder.
      $title = '';
    }

    return $title;
  }


  /**
   * Method to find #Layer4 and the 5th paragraph up to the first empty br.
   *
   * @return string
   *   The text found.
   */
  protected function findIdLayer4P5UptoEmptyBr() {
    $elem = $this->queryPath->find("#Layer4")->siblings('p:nth-of-type(5)');
    $title = $elem->innerHTML();
    $title = $this->trimAtBrBlank($title, $elem, 210);

    return $title;
  }


  /**
   * Find the first image in the very specific contentSub div, and get the alt.
   */
  protected function findFirstContentSubImageAlt() {
    $elem = $this->queryPath->find('.contentSub > div > img')->first();
    if ($elem) {
      return $elem->attr('alt');
    }
    return "";
  }


  /**
   * Find first centered aligned paragraph after the first hr.
   */
  protected function pluckFirstCenteredAlignPAfterHr() {
    $hr = $this->queryPath->find("hr")->first();

    if (!empty($hr)) {
      $elem = $hr->next();
      if ($elem->is('p') && ($elem->attr('align') == 'center'
        || $elem->attr('style') == "text-align:center;")) {
        $this->setElementToRemove($elem);
        $text = $elem->text();
        return $text;
      }
    }
    return "";
  }