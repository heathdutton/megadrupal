<?php

/**
 * @file
 * Feeds parser plugin that parses well formed HTML files.
 */
class HtmlImportParser extends FeedsParser {

  const UTF8_META = '<meta http-equiv="content-type" content="text/html; charset=utf-8">';
  const HTML_IMPORT_BUMP_DEPTH = 3;

  protected $html;
  protected $footnotes = array();
  protected $sections = array();
  protected $imagesFile = array();
  protected $tempImageDirectory;
  protected $hasImages = FALSE;
  protected $headingLevel;
  protected $pageStartHeadingLevel;
  protected $footnoteLinkPattern;
  protected $titleAllCaps;
  protected $footnoteClass;
  protected $sourceConfig;
  public $parentNode;
  public $langcode = LANGUAGE_NONE;

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $config = $source->getConfig();
    $this->sourceConfig = $config['HtmlImportFetcher'];

    $temp_directory = file_directory_temp();
    $this->tempImageDirectory = $temp_directory . DIRECTORY_SEPARATOR . 'html_import' . time() . rand(0, 1000) . DIRECTORY_SEPARATOR;

    $this->setParentNode($source->feed_nid);
    if (!empty($this->parentNode->field_images[$this->langcode])) {
      $imageFileUri = $this->parentNode->field_images[$this->langcode][0]['uri'];
      $imageFileName = $this->parentNode->field_images[$this->langcode][0]['filename'];
      if (!empty($imageFileUri)) {
        $this->imagesFile['realpath'] = drupal_realpath($imageFileUri);
        $this->imagesFile['filename'] = $imageFileName;
        $this->hasImages = TRUE;
      }
    }
    $this->headingLevel = $this->sourceConfig['heading_level'];
    $this->pageStartHeadingLevel = $this->sourceConfig['page_start_heading_level'];
    $this->footnoteLinkPattern = trim($this->sourceConfig['footnote_link_pattern']);
    $this->allowedHtmlTags = trim($this->sourceConfig['allowed_html_tags']);
    $this->footnoteClass = trim($this->sourceConfig['footnote_class']);
    $this->titleAllCaps = $this->sourceConfig['title_all_caps'];

    $this->html = $fetcher_result->getRaw();
    //split footnotes in footnotes object and return raw HTML without footnotes
    $this->html = $this->split_footnotes();

    $this->preprocess_html();
    $this->extract_images();
    $this->create_sections();
    $this->process_sections();

    $result = new FeedsParserResult();
    $result->title = t('Batch import large HTML document');
    $result->items = $this->sections;
    $result->files = array(
      'tempImageDirectory' => $this->tempImageDirectory,
    );
    // Release parser.
    unset($parser);
    // Set error reporting back to its previous value.
    return $result;
  }

  /**
   * Implementation of FeedsParser::getMappingSources.
   */
  public function getMappingSources() {
    return array(
      'title' => array(
        'name' => t('Title'),
        'description' => t('Title of an imported section. Map this to the main title.'),
      ),
      'content' => array(
        'name' => t('Body'),
        'description' => t('Body content of an imported section. Map this to the main body.'),
      ),
      'footnotes' => array(
        'name' => t('Footnotes'),
        'description' => t('Footnotes referenced by an imported section. Map this to the footnotes field (long-text).'),
      ),
      'book_id' => array(
        'name' => t('Book ID'),
        'description' => t('ID of the entire imported structure. Map this to a node reference field for URL alias generation.'),
      ),

    );
  }

  /**
   * Run the initial imported HTML through a series of cleaning functions.
   *
   * This performs the cleanup on the HMTL property of the object.
   */
  protected function preprocess_html() {
    $this->convert_to_utf8($this->html);
    $this->strip_element($this->html, 'head');
    $this->strip_tags($this->html);
    $this->strip_nbsp($this->html);
    $this->strip_spaces($this->html);
  }

  /**
   * Split the imported document into sections.
   *
   * Splits the imported HMTL into sections based upon heading elements. This
   * is configurable via a source configuration on import.
   */
  protected function create_sections() {
    $pattern = '~(<h[1-' . $this->headingLevel . '])~';
    $this->sections = preg_split($pattern, $this->html, -1, PREG_SPLIT_DELIM_CAPTURE);
  }

  /**
   * Parse the HTML, separate foot notes from the body text and store in a footnotes array
   */
  protected function split_footnotes() {
    // Move footnotes out of the body
    $pattern = '#<p\b[^>]*class="' . $this->footnoteClass . '".*?>.*?</p>#s';
    preg_match_all($pattern, $this->html, $this->footnotes);
    return preg_replace($pattern, '', $this->html);

  }

  /**
   * Loop though all sections and convert them into batch item arrays
   */
  protected function process_sections() {
    $this->build_sections($this->sections);
    foreach ($this->sections as $key => $section) {
      //we MAY NEED TO clean the html late in the process, as we need the classes to extract semantics
      $this->clean_html($section['content']);
      //process images, footnotes and tables
      $this->build_element_indexes($section);

      $this->sections[$key] = $section;
    }
  }

  /**
   * Extract any attached zip file into a temporary directory and remove the
   * original file
   */
  protected function extract_images() {
    if ($this->hasImages) {

      file_prepare_directory($this->tempImageDirectory, FILE_CREATE_DIRECTORY);

      $zip = new ZipArchive();
      if ($zip->open($this->imagesFile['realpath']) === TRUE) {
        $success = $zip->extractTo($this->tempImageDirectory);
      }

      $fileArchiveFolder = array_shift(explode('.', $this->imagesFile['filename']));
      $this->tempImageDirectory .= DIRECTORY_SEPARATOR . $fileArchiveFolder;
    }
  }

  /**
   * Parse the HTML to make indexes of embedded assets, such as
   * images, footnotes, and tables
   */
  protected function build_element_indexes(&$section) {

    //build images index
    if ($this->hasImages) {
      $section_qp = $this->getQpObject($section['content']);
      $section_qp->find('img');
      $search = array();
      $replace = array();
      $section['log']['counts']['image_count'] = $section_qp->size();

      foreach ($section_qp as $img) {
        $img_src_original = $img->attr('src');
        $img_src = array_pop(explode('/', ($img->attr('src'))));
        $mime_type = file_get_mimetype($img_src);
        $img_location = $this->tempImageDirectory . DIRECTORY_SEPARATOR . $img_src;
        // TODO: use image_get_info to get dimensions to create wdith height attribute
        $img_info = image_get_info($img_location);
        $enclosure = new FeedsEnclosure($img_location, $mime_type);
        if ($enclosure) {
          $section['images'][] = $enclosure;
        }

        $search[] = 'src="' . $img_src_original . '"';
        $replace[] = 'src="' . $img_src . '" width="' . $img_info['width'] . '" height="' . $img_info['height'] . '"';

        // Log missing attributes
        if (!$img->attr('alt')) {
          $section['log']['warnings'][] = 'The image ' . $img_src . ' is missing the alt attribute';
        }

      }

      $section['book_id'] = $this->parentNode->nid;
      $section['content'] = str_replace($search, $replace, $section['content']);

    }

  }

  /**
   * Convert the original HTML document to UTF8.
   *
   * @param string $html
   * The html to be converted (passed by reference).
   */
  protected function convert_to_utf8(&$html) {
    $newline_chars = array(chr(13), "\r\n", '\r', '\n', '&nbsp;');
    $html = str_replace($newline_chars, " ", $html);
    $dash_chars = array(chr(226));
    $html = str_replace($dash_chars, chr(45), $html);
    preg_match('/charset=([\w-]+)/', $html, $matches);
    if ($matches[1] == 'windows-1256') {
      $html = iconv('windows-1256', 'utf-8', $html);
    }
    if ($matches[1] == 'windows-1252') {
      $html = iconv('windows-1252', 'utf-8', $html);
    }
    $html = iconv('utf-8', 'utf-8//IGNORE', $html);
  }

  /**
   * Replace all type of "Smart Quotes" using standard ASCII characters
   *
   * @param string $str
   * The string to be processed.
   */
  protected function replace_smart_quotes($str) {
    $chr_map = array(
      // Windows codepage 1252
      "\xC2\x82" => "'",
      // U+0082⇒U+201A single low-9 quotation mark
      "\xC2\x84" => '"',
      // U+0084⇒U+201E double low-9 quotation mark
      "\xC2\x8B" => "'",
      // U+008B⇒U+2039 single left-pointing angle quotation mark
      "\xC2\x91" => "'",
      // U+0091⇒U+2018 left single quotation mark
      "\xC2\x92" => "'",
      // U+0092⇒U+2019 right single quotation mark
      "\xC2\x93" => '"',
      // U+0093⇒U+201C left double quotation mark
      "\xC2\x94" => '"',
      // U+0094⇒U+201D right double quotation mark
      "\xC2\x9B" => "'",
      // U+009B⇒U+203A single right-pointing angle quotation mark
      // Regular Unicode     // U+0022 quotation mark (")
      // U+0027 apostrophe     (')
      "\xC2\xAB" => '"',
      // U+00AB left-pointing double angle quotation mark
      "\xC2\xBB" => '"',
      // U+00BB right-pointing double angle quotation mark
      "\xE2\x80\x98" => "'",
      // U+2018 left single quotation mark
      "\xE2\x80\x99" => "'",
      // U+2019 right single quotation mark
      "\xE2\x80\x9A" => "'",
      // U+201A single low-9 quotation mark
      "\xE2\x80\x9B" => "'",
      // U+201B single high-reversed-9 quotation mark
      "\xE2\x80\x9C" => '"',
      // U+201C left double quotation mark
      "\xE2\x80\x9D" => '"',
      // U+201D right double quotation mark
      "\xE2\x80\x9E" => '"',
      // U+201E double low-9 quotation mark
      "\xE2\x80\x9F" => '"',
      // U+201F double high-reversed-9 quotation mark
      "\xE2\x80\xB9" => "'",
      // U+2039 single left-pointing angle quotation mark
      "\xE2\x80\xBA" => "'",
      // U+203A single right-pointing angle quotation mark
    );
    $chr = array_keys($chr_map); // but: for efficiency you should
    $rpl = array_values($chr_map); // pre-calculate these two arrays
    $str = str_replace($chr, $rpl, html_entity_decode($str, ENT_QUOTES, "UTF-8"));

    return $str;
  }

  /**
   * Strip away an element and its contents.
   *
   * @param string $html
   * The html to process.
   * @param string $element
   * The element name (eg img).
   * @param string $replacement
   * The replacement string.
   */
  protected function strip_element(&$html, $element, $replacement = '') {
    $pattern = '~<' . $element . '[\s\S]*?</' . $element . '>~';
    $html = preg_replace($pattern, $replacement, $html);
  }

  /**
   * Strip any non-permitted tags.
   *
   * @param string $html
   * The html to be processed (passed by reference).
   * @param string $extra_tags
   * Any extra tags to be left in the HMTL.
   */
  protected function strip_tags(&$html, $extra_tags = '') {
    $allowable_tags = empty($this->allowedHtmlTags) ? '<h1><h2><h3><h4><h5><h6><p><em><strong><img><a><table><caption><thead><tbody><tr><th><td><blockquote><sup><sub><div><b><ol><ul><li><i><br>' : $this->allowedHtmlTags;
    $allowable_tags .= $extra_tags;
    $html = strip_tags($html, $allowable_tags);
  }

  /**
   * Strip any non-allowable attributes.
   *
   * @param string $html
   * The HTML to be processed.
   */
  protected function strip_attributes(&$html) {
    $stripped_attributes = array(
      'lang',
      'style',
      'size',
      'height',
      'width',
      'face',
      'valign'
    );
    $qp = $this->getQpObject($html);

    foreach ($stripped_attributes as $attribute) {
      // Remove all class attributes except class "box"
      if ($attribute == 'class') {
        $qp->find('[class]')
          ->not('.box')
          ->not('.table-overflow')
          ->removeAttr('class')
          ->top();
      }
      else {
        $qp->find('[' . $attribute . ']')->removeAttr($attribute)->top();
      }

    }
    // Note: we need to get the body contents because of the way qp creates objects
    $html = $qp->find('body')->innerHTML();
  }

  /**
   * Remove any empty p or a elements.
   *
   * @param string $html
   * The HTML to be processed
   *
   * @TODO: make this configurable
   */
  protected function remove_empty_elements(&$html) {
    $pattern = '~<([p|a])[^>]*?></\1>~';
    $html = preg_replace($pattern, '', $html);
  }

  /**
   * Convert any nbsp to whitespace.
   *
   * Note, the standard replacement doesn't always catch the nbsp, so this
   * function was added to use extra character definitions.
   *
   * @param string $html
   * The HTML to be processed
   */
  protected function strip_nbsp(&$html) {
    $html = str_replace(array('&nbsp;', HTML_IMPORT_NBSP), ' ', $html);
  }

  /**
   * Strip additional whitespace.
   *
   * Word HTML has an unfortunate tendency to insert extra linebreak mid tag.
   * This function resolves that issue.
   *
   * @param string $html
   * The HTML to be processed
   */
  protected function strip_spaces(&$html) {
    $pattern = '~[\s]+~';
    $replacement = ' ';
    $html = preg_replace($pattern, $replacement, $html);
  }

  /**
   * Wrapper function to call various cleansing functions.
   *
   * @param string $html
   * The HMTL to be processed.
   */
  protected function clean_html(&$html) {
    if (html_import_perm()) {
      $this->strip_nbsp($html);
      $this->strip_attributes($html);
      $this->remove_empty_elements($html);
    }
  }

  /**
   * Parse section text strings into batch item arrays
   */
  protected function build_sections() {
    $parents = array(0 => 0);
    $sections = array();

    foreach ($this->sections as $key => $item) {
      if ($key % 2) {
        // Odd, so it just holds information of the match such as "<h3". Setup parent info
        $section = array();
        $prefix = $item;
        $section['level'] = substr($item, 2, 1);
        $parents[$section['level']] = $key + 1;
        $section['parent_id'] = $parents[$section['level'] - 1];
      }
      else {
        // First item didn't match, so don't prepend
        if ($key && isset($section['level']) && $item) {
          // Reconstruct item by combining the match and the rest of the HTML content	
          $item = $prefix . $item;

          // Record all anchors and references to anchors in this section. This will be used to rebuild links
          if (html_import_perm()) {
            $section['anchors'] = $this->find_anchors($item);
            $section['anchor_refs'] = $this->find_anchor_refs($item, $section['anchors']);
          }

          $section_qp = $this->getQpObject($item);
          $title_qp = $section_qp->branch();

          $heading_element = 'h' . $section['level'];
          $section_title = $title_qp->remove($heading_element)->text();
          $section_contents = $section_qp->innerHTML();

          $section['title'] = trim($this->replace_smart_quotes($section_title));

          // Transform title if it is in ALL CAPS
          if ((mb_strtoupper($section['title'], 'utf-8') == $section['title']) && ($this->titleAllCaps > 0)) {
            // Transform title to Sentence case
            if ($this->titleAllCaps == 1) {
              $section['title'] = ucfirst(strtolower($section['title']));
            }
            // Transform title to Title Case
            if ($this->titleAllCaps == 2) {
              $section['title'] = ucwords(strtolower($section['title']));
            }
          }

          // When HTML content is imported, the heading levels of non-title headings should be bumped to match the start heading level
          if (($this->pageStartHeadingLevel != 0) && html_import_perm()) {
            $bump_heading_level = $this->headingLevel + 1;
            $bumped_heading_level = $this->pageStartHeadingLevel + 1;
            $bump_depth = $bump_heading_level + HtmlImportParser::HTML_IMPORT_BUMP_DEPTH;;
            while ($bump_heading_level <= $bump_depth) {
              $section_contents = str_replace('<h' . $bump_heading_level, '<h' . $bumped_heading_level, $section_contents);
              $section_contents = str_replace('</h' . $bump_heading_level . '>', '</h' . $bumped_heading_level . '>', $section_contents);
              $bump_heading_level++;
              $bumped_heading_level++;
            }
          }

          $section['content'] = $section_contents;

          $section['footnotes'] = $this->process_footnotes($section['content']);

          $section['index'] = $key;
          $section['log'] = array('count' => array(), 'warnings' => array());

          // Sections without titles indicate we are dealing with the beginning
          // of the document, so we don't include.
          if ($section['title']) {
            $sections[$key] = $section;
          }
        }
      }
    }
    $this->sections = $sections;
  }

  /**
   * Find all anchors (using the ID attribite), we only care about what anchors a given section have
   */
  protected function find_anchors($item) {
    $section_anchors = array();
    $pattern = '~<[a-zA-Z0-9]+ [^>]*id="([^">]+)">~';
    preg_match_all($pattern, $item, $section_anchors);
    if (!empty($section_anchors[1])) {
      return $section_anchors[1];
    }
    else {
      return NULL;
    }
  }

  /**
   * Find all references to anchors (using the ID attribite),
   * we only care about what references to anchors a given section have.
   * Note the result of this function may be different from find_anchors() returns
   * because a section can make references to anchors in other sections
   */
  protected function find_anchor_refs($item, $section_anchors = array()) {
    $section_anchor_refs = array();
    $pattern = '~<a [^>]*href="\#([^">]+)".*?>~';
    preg_match_all($pattern, $item, $section_anchor_refs);
    if (!empty($section_anchor_refs[1])) {
      // We only want to record references to other sections as they will refer to different pages/URLs after import                           	
      if (!empty($section_anchors)) {
        return array_diff($section_anchor_refs[1], $section_anchors);
      }
      else {
        return $section_anchor_refs[1];
      }
    }
    else {
      return NULL;
    }
  }

  /**
   * Add footnotes referenced by the current section to section['footnotes']
   *
   * @param $section_content
   * HTML content of the section being processed
   */
  protected function process_footnotes($section_content) {
    $section_footnote_references = array();
    $section_footnote_content = '';
    $pattern = '#\#' . $this->footnoteLinkPattern . '[a-z0-9-_.]+#s';
    preg_match_all($pattern, $section_content, $section_footnote_references);
    if (!empty($section_footnote_references) && html_import_perm()) {
      $section_footnote_content = '<div class="section-footnotes">';
      foreach ($this->footnotes[0] as $footnote) {
        foreach ($section_footnote_references[0] as $section_footnote_reference) {
          $footnote_id = str_replace('#', '', $section_footnote_reference);
          if (preg_match('#id="' . $footnote_id . '"#s', $footnote)) {
            $section_footnote_content .= $footnote;
          }
        }
      }
      $section_footnote_content .= '</div>';
      return $section_footnote_content;
    }
    else {
      return NULL;
    }
  }

  /**
   * Wrapper function to create a qp html object, and respect the character
   * encoding
   *
   * This function is required, because the Dom parser in PHP needs to be
   * tricked into creating a UTF8 document. This is done by adding a meta
   * element
   *
   * @param string $string
   * The String to be converted to a qp object
   *
   * @return qpObject
   */
  protected function getQpObject($string) {
    $options = array(
      'convert_from_encoding' => 'utf-8',
      'replace_entities' => FALSE,
    );
    $string = HtmlImportParser::UTF8_META . $string;
    $string = utf8_encode($string);
    $qp = htmlqp($string, 'body', $options)
      ->remove('meta')
      ->top()
      ->find('body');
    return $qp;
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

}
