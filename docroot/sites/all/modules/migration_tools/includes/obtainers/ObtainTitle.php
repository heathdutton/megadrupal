<?php

/**
 * @file
 * Class ObtainTitle
 *
 * Contains a collection of stackable finders that can be arranged
 * as needed to obtain a title/heading and possible subtitle/subheading.
 */

/**
 * {@inheritdoc}
 */
class ObtainTitle extends ObtainHtml {

  /**
   * {@inheritdoc}
   */
  protected function processString($string) {
    return $this->truncateString($string);
  }

  /**
   * Truncates and sets the discarded if there is a remainder.
   */
  protected function truncateString($string) {
    $split = $this->truncateThisWithoutHTML($string, 255, 2);

    // @todo Add debugging to display $split['remaining'].
    // $this->setTextDiscarded($split['remaining']);

    return $split['truncated'];
  }


  /**
   * Finder method to find the content sub-banner alt.
   * @return string
   *   The text found.
   */
  protected function findSubBannerAlt() {
    return $this->findSubBannerAttr('alt');
  }

  /**
   * Finder method to find the content sub-banner title.
   * @return string
   *   The text found.
   */
  protected function findSubBannerTitle() {
    return $this->findSubBannerAttr('title');
  }

/**
   * Grab method to find the content sub-banner attribute.
   * @return string
   *   The text found.
   */
  protected function findSubBannerAttr($attribute = 'alt') {
    $title = $this->findSubBannerString($attribute);
    // Remove the text 'banner'.
    $title = str_ireplace('banner', '', $title);
    // Check to see if alt is just placeholder to discard.
    $placeholder_texts = array(
      'placeholder',
      'place-holder',
      'place_holder',
    );
    foreach ($placeholder_texts as $needle) {
      if (stristr($title, $needle)) {
        // Just placeholder text, so ignore this text.
        $title = '';
      }
    }

    return $title;
  }

  /**
   * Get subbanner image.
   */
  protected function findSubBannerString($attribute = 'alt') {
    $subbanner = NULL;
    $images = $this->queryPath->find('img');
    foreach ($images as $image) {
      $src = $image->attr('src');
      if (stristr($src, 'subbanner')) {
        return $image->attr($attribute);
      }
    }
    return '';
  }


  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    // Breaks need to be converted to spaces to avoid lines running together.
    // @codingStandardsIgnoreStart
    $break_tags = array('<br>', '<br/>', '<br />', '</br>');
    // @codingStandardsIgnoreEnd
    $text = str_ireplace($break_tags, ' ', $text);
    $text = strip_tags($text);
    // Titles can not have html entities.
    $text = html_entity_decode($text, ENT_COMPAT, 'UTF-8');

    // There are also numeric html special chars, let's change those.
    module_load_include('inc', 'migration_tools', 'includes/migration_tools');
    $text = strongcleanup::decodehtmlentitynumeric($text);

    // We want out titles to be only digits and ascii chars so we can produce
    // clean aliases.
    $text = StringCleanUp::convertNonASCIItoASCII($text);
    // Remove undesirable chars and strings.
    $remove = array(
      '&raquo;',
      '»',
    );
    $text = str_ireplace($remove, ' ', $text);

    // Remove white space-like things from the ends and decodes html entities.
    $text = StringCleanUp::superTrim($text);
    // Remove multiple spaces.
    $text = preg_replace('!\s+!', ' ', $text);
    // Convert to ucwords If the entire thing is caps. Otherwise leave it alone
    // for preservation of acronyms.
    // Caveat: will obliterate acronyms if the entire title is caps.
    $uppercase_version = strtoupper($text);
    similar_text($uppercase_version, $text, $percent);
    if ($percent > 95.5) {
      // Nearly the entire thing is caps.
      $text = strtolower($text);
    }
    $text = StringCleanUp::makeWordsFirstCapital($text);

    return $text;
  }
}
