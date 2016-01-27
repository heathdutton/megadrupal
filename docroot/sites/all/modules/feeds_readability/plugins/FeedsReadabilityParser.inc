<?php

/**
 * @file
 * Readability Feeds Parser class, uses the FiveFilters PHP Readability library.
 */

class FeedsReadabilityParser extends FeedsParser {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    $library = libraries_load('php-readability');

    if (!$library['loaded']) {
      throw new Exception(t('FeedsReadabilityParser was unable to load required library php-readability.'));
    }

    $html = $fetcher_result->getRaw();

    $config = $this->getConfig();

    $encoding = mb_detect_encoding($html, $config['encodings']);
    if ($encoding != 'UTF-8') {
      $html = mb_convert_encoding($html, 'UTF-8', $encoding);
    }

    $readability = new Readability($html);
    if ($readability->init()) {

      $title = $readability->getTitle()->textContent;
      $content = $readability->getContent()->innerHTML;

      // Grab the first image from Readability content.
      $imgurl = NULL;
      $doc = new DOMDocument();
      if ($doc->loadHTML($content)) {
        $images = $doc->getElementsByTagName('img');
        foreach ($images as $image) {
          $imgurl = $image->getAttribute('src');
          if ($imgurl) {
            break;
          }
        }
      }

      $item = array(
        'title' => $title,
        'content' => $content,
        'image' => $imgurl,
      );

      return new FeedsParserResult(array($item));
    }
    else {
      throw new Exception(t('Readability failed to parse the document at !url',
                            array('!url' => $fetcher_result->getFilePath())));
    }
  }

  /**
   * Override parent::getMappingSources().
   */
  public function getMappingSources() {
    return array(
      'title' => array(
        'name' => t('Article title'),
        'description' => t('Title of the article.'),
      ),
      'content' => array(
        'name' => t('Article content'),
        'description' => t('Content of the article.'),
      ),
      'image' => array(
        'name' => t('Article image'),
        'description' => t('The first image found in the article content'),
      ),
    ) + parent::getMappingSources();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $config = $this->getConfig();

    $form = array(
      'encodings' => array(
        '#type' => 'textfield',
        '#title' => t('Detect encodings'),
        '#description' => t('Enter an ordered, comma-separated list of encodings
                            to detect.  This probably needs to be reconfigured
                            if source webpages become garbled when parsing.
                            See !url for a list of supported character
                            encodings.',
                            array(
                              '!url' => l(t('the PHP documentation'),
                                          'http://www.php.net/manual/en/mbstring.supported-encodings.php'),
                            )),
        '#default_value' => $config['encodings'],
      ),
    );

    return $form;
  }

  /**
   * Define defaults.
   */
  public function configDefaults() {
    return array(
      'encodings' => 'EUC-CN, ASCII, UTF-8',
    );
  }

}
