<?php
/**
 * @file
 * Helper function to clean up HTML.
 */

class HtmlCleanUp {

  /**
   * Removes legacy elements from HTML that are no longer needed.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @param array $arguments
   *   (optional). An array of arbitrary arguments to be used by HtmlCleanUp
   *   methods. Defaults to empty array.
   */
  public static function stripOrFixLegacyElements($query_path, $arguments = array()) {
    // Strip comments.
    foreach ($query_path->top()->xpath('//comment()')->get() as $comment) {
      $comment->parentNode->removeChild($comment);
    }

    // Remove elements and their children.
    HTMLCleanUp::removeElements($query_path, array(
      'a[name="sitemap"]',
      'a[name="maincontent"]',
      'img[src="/gif/sealmt.gif"]',
      'div.skip',
      'div.hdrwrpr',
      'div.breadcrumbmenu',
      'div.footer',
      // This is deleting the content from the Louisiana pr example.
      // @todo can we target it better?
      // 'div.clear',
      'div.lastupdate',
      'div.thick-bar',
      'div.rightcolumn',
      'div.leftcolmenu',
      // Remove all back to top links.
      'a[href="#top"]',
      'style',
      'script',
    ));

    // Remove external icon images.
    $matches = HtmlCleanUp::matchAll($query_path, "a > span > img", "externalicon.gif", "attr", 'src');
    foreach ($matches as $key => $match) {
      $match->parent()->parent()->remove();
    }


    // Remove extraneous html wrapping elements, leaving children intact.
    HTMLCleanUp::removeWrapperElements($query_path, array(
      'body > blockquote',
      '.bdywrpr',
      '.gridwrpr',
      '.leftcol-subpage',
      '.leftcol-subpage-content',
      '.bodytextbox',
      'body > div',
    ));

    // Remove style attribute from elements.
    $query_path->find('.narrow-bar')->removeAttr('style');

    // Remove matching elements containing only &nbsp; or nothing.
    HTMLCleanUp::removeEmptyElements($query_path, array(
      'div',
      'span',
      'p',
    ));

    // Remove black title bar with eagle image (if present).
    HTMLCleanUp::removeTitleBarImage($query_path);

    // FIX.
    // Empty anchors without name attribute will be stripped by ckEditor.
    HTMLCleanUp::fixNamedAnchors($query_path);

    // Some pages have images as subtitles. Turn those into html.
    $header_element = !empty($arguments['header_element']) ? $arguments['header_element'] : 'h2';
    HTMLCleanUp::changeSubTitleImagesForHtml($query_path, $header_element);

    // Removing scripts used when linking to outside sources.
    HtmlCleanUp::removeExtLinkJS($query_path);

    // Fix broken links to PDF anchors.
    HTMLCleanUp::fixPdfLinkAnchors($query_path);
  }

  /**
   * Removes elements matching CSS selectors.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @param array $selectors
   *   An array of selectors to remove.
   */
  public static function removeElements($query_path, array $selectors) {
    foreach ($selectors as $selector) {
      $query_path->find($selector)->remove();
    }
  }

  /**
   * Removes elements matching CSS selectors from html.
   *
   * @param string $html
   *   Html to get processed.
   * @param array $selectors
   *   An array of selectors to remove.
   *
   * @return string
   *   Processed html.
   */
  public static function removeElementsFromHtml($html, array $selectors) {
    // Put the shell on the html to extract with more certainty later.
    $html = '<div class="throw-away-parser-shell">' . $html . '</div>';
    $query_path = htmlqp($html, NULL, array());
    HTMLCleanUp::removeElements($query_path, $selectors);

    // Grab the html from the shell.
    $processed_html = $query_path->top('.throw-away-parser-shell')->innerHTML();
    return $processed_html;
  }

  /**
   * Get the first element matching the CSS selector from html.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @param string $selector
   *   A css selector.
   *
   * @return string
   *   The text from the first matching selector, if matched.
   */
  public static function getFirstElement($query_path, $selector) {
    $text = "";

    // Put the shell on the html to extract with more certainty later.
    $items = $query_path->find($selector);
    foreach ($items as $item) {
      $text = $item->text();
      break;
    }

    return $text;
  }

  /**
   * Extract the first elements with the CSS selector from html.
   *
   * Extraction means that we return the match, but we also return the
   * original html without the element that matched the search.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @param string $selector
   *   A CSS selector to extract.
   *
   * @return array
   *   The array contains the matched text, and the original html without the
   *   match.
   */
  public static function extractFirstElement($query_path, $selector) {

    $items = $query_path->find($selector);
    foreach ($items as $item) {
      $text = $item->text();
      $item->remove();
      break;
    }

    return $text;
  }


  /**
   * Removes a wrapping element, leaving child elements intact.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   * @param array $selectors
   *   An array of selectors for the wrapping element(s).
   */
  public static function removeWrapperElements($query_path, array $selectors) {
    foreach ($selectors as $selector) {
      $children = $query_path->find($selector)->children();
      $children->unwrap();
    }
  }


  /**
   * Rewraps an element, leaving child elements intact.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @param array $selectors
   *   An array of selectors for the wrapping element(s).
   *
   * @param string $new_wrapper
   *   A string of the leading wrapping element.
   *   - <h2 />
   *   - <h2 id="title" />
   *   - <div class="friends" />
   */
  public static function rewrapElements($query_path, array $selectors, $new_wrapper) {
    // There is something to wrap it in, so begin the hunt.
    foreach ($selectors as $selector) {
      $elements = $query_path->find($selector);
      foreach ($elements as $element) {
        $element->wrapInner($new_wrapper);
      }
    }
    HTMLCleanUp::removeWrapperElements($query_path, $selectors);
  }

  /**
   * Removes empty elements matching selectors.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @param array $selectors
   *   An array of selectors to remove.
   */
  public static function removeEmptyElements($query_path, array $selectors) {
    foreach ($selectors as $selector) {
      $elements = $query_path->find($selector);
      foreach ($elements as $element) {
        $contents = StringCleanUp::superTrim($element->innerXHTML());
        $empty_values = array(
          '&nbsp;',
          '',
        );
        if (in_array($contents, $empty_values)) {
          $element->remove();
        }
      }
    }
  }

  /**
   * Remove eagle image title bar divs.
   *
   * Eagle image bars are always inside '<div style="margin-bottom:(15|20)px">'.
   * It appears that they are the only elements with this style applied.
   * Nonetheless, if more than one match, remove only the first.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   */
  protected static function removeTitleBarImage($query_path) {
    // Find divs that are immediately followed by img tags.
    $elements = $query_path->find('div > img')->parent();
    foreach ($elements as $element) {
      // Eagle banner bars always preceed headlines.
      if (preg_match('/class=\"headline/', $element->html())) {
        break;
      }
      if (preg_match('/style=\"(margin|padding)-bottom:(\s)*(15|20)px/i', $element->html())) {
        // We found an eagle image title bar: remove it and we're done.
        $element->remove();
        break;
      }
    }
  }

  /**
   * Removes legacy usage of javascript:exitWinOpen() for external links.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   */
  protected static function removeExtLinkJS($query_path) {
    $elements = $query_path->find('a');

    // This should replace tags matching
    // <a href="javascript:exitWinOpen('http://example.com');">Example</a>
    // with <a href="http://example.com">Example</a>.
    $patterns[] = "|javascript:exitWinOpen\('([^']+)'\)|";

    // This should replace tags matching
    // <a href="/cgi-bin/outside.cgi?http://nccic.org/tribal/">Tribal</a>
    // with <a href="http://nccic.org/tribal/">Tribal</a>
    $patterns[] = "|/cgi-bin/outside.cgi\?([^']+)|";

    foreach ($elements as $element) {
      $href = $element->attr('href');
      if ($href) {
        foreach ($patterns as $pattern) {
          preg_match($pattern, $href, $matches);
          if (isset($matches) && !empty($matches[1])) {
            $new_url = $matches[1];
            $element->attr('href', $new_url);
          }
        }
      }
    }
  }

  /**
   * Fixes anchor links to PDFs so that they work in IE.
   *
   * Specifically replaces anchors like #_PAGE2 and #p2 with #page=2.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @see http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_open_parameters.pdf
   */
  public static function fixPdfLinkAnchors($query_path) {
    $anchors = $query_path->find('a');
    foreach ($anchors as $anchor) {
      $url = $anchor->attr('href');
      $contains_pdf_anchor = preg_match('/\.pdf#(p|_PAGE)([0-9]+)/i', $url, $matches);
      if ($contains_pdf_anchor) {
        $old_anchor = $matches[1];
        $page_num = $matches[3];
        $new_anchor = 'page=' . $page_num;
        $new_url = str_replace($old_anchor, $new_anchor, $url);
        $anchor->attr('href', $new_url);
      }
    }
  }

  /**
   * Empty anchors without name attribute will be stripped by ckEditor.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   */
  protected static function fixNamedAnchors($query_path) {
    $elements = $query_path->find('a');
    foreach ($elements as $element) {
      $contents = trim($element->innerXHTML());
      if ($contents == '') {
        if ($anchor_id = $element->attr('id')) {
          // Only set a name if there isn't one.
          if (!$element->hasAttr('name')) {
            $element->attr('name', $anchor_id);
          }
        }
      }
    }
  }

  /**
   * Makes relative sources values on <a> and <img> tags absolute.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @param string $file_id
   *   The full file path of the of the current file, used to determine
   *   location of relative links.
   */
  public static function convertRelativeSrcsToAbsolute($query_path, $file_id) {

    // A list of attributes to convert, keyed by HTML tag (NOT selector).
    $attributes = array(
      'img' => array('src', 'longdesc'),
      'a' => array('href'),
    );

    $elements = $query_path->find(implode(', ', array_keys($attributes)));
    foreach ($elements as $element) {
      $tag_attributes = $attributes[$element->tag()];
      foreach ($tag_attributes as $attribute) {

        $url = parse_url($element->attr($attribute));

        if ($url) {
          $is_relative = empty($url['scheme']) && !empty($url['path']) && substr($url['path'], 0, 1) !== '/';

          if ($is_relative) {
            $dir_path = dirname($file_id);

            $new_url = '/' . $dir_path . '/' . $url['path'];
            if (!empty($url['query'])) {
              $new_url .= '?' . $url['query'];
            }
            if (!empty($url['fragment'])) {
              $new_url .= '#' . $url['fragment'];
            }

            // We might get some double '//', let's clean them.
            $new_url = str_replace("//", "/", $new_url);

            $element->attr($attribute, $new_url);
          }
        }
      }
    }
  }

  /**
   * Change sub-header images to HTML headers. Defaults to <h2>.
   *
   * @param QueryPath $query_path
   *   The QueryPath object with HTML markup.
   *
   * @param string $header_element
   *   (optional). The HTML header element with which to replace the <img>.
   *   Defaults to h2.
   */
  protected static function changeSubTitleImagesForHtml($query_path, $header_element = 'h2') {
    // Find all headline divs with an image inside.
    $elements = $query_path->find('div.headline > img')->parent();

    foreach ($elements as $element) {
      $image = $element->find('img');
      $alt = $image->attr('alt');
      $element->html("<$header_element>$alt</$header_element>");
    }

    // Let's assume that images with 20 height are also subtitles.
    $images = $query_path->find('img');

    foreach ($images as $image) {
      $height = $image->attr("height");
      $alt = $image->attr('alt');

      if (strcasecmp($height, "20") == 0 && !empty($alt)) {
        $image->wrap("<div class='$header_element-wrapper'></div>");
      }
    }

    // Let's assume that images with 20 height are also subtitles.
    $wrappers = $query_path->find(".$header_element-wrapper");

    foreach ($wrappers as $wrapper) {
      foreach ($wrapper->find('img') as $img) {
        $alt = $img->attr('alt');
        $wrapper->html("<$header_element>{$alt}</$header_element>");
      }
    }
  }

  /**
   * Examines an uri and evaluates if it is an image.
   *
   * @param string $uri
   *   A uri.
   *
   * @return bool
   *   TRUE if this is an image uri, FALSE if it is not.
   */
  public static function isImageUri($uri) {
    if (preg_match('/.*\.(jpg|gif|png|jpeg)$/i', $uri) !== 0) {
      // Is an image uri.
      return TRUE;
    }
    return FALSE;
  }

  /**
   * General matching function.
   *
   * @param QueryPath $qp
   *   A QueryPath object.
   * @param string $selector
   *   The CSS selector for the element to be matched.
   * @param string $needle
   *   The text string for which to search.
   * @param string $function
   *   The function used to get the haystack. E.g., 'attr' if searching for
   *   a specific attribute value, 'html', 'txt'.
   * @param string $parameter
   *   A parameter to be passed into the defined $function.
   *
   * @return mixed
   *   The matched QueryPath element or FALSE.
   */
  public static function match($qp, $selector, $needle, $function, $parameter = NULL, $index = 0) {
    $elements = $qp->find($selector);
    $counter = 0;
    foreach ($elements as $key => $elem) {
      $haystack = $elem->$function($parameter);
      if (substr_count($haystack, $needle) > 0) {
        if ($counter == $index) {
          return $elem;
        }
        $counter++;
      }
    }
    return FALSE;
  }

  /**
   * Like match, but returns all matching elements.
   */
  public static function matchAll($qp, $selector, $needle, $function, $parameter = NULL) {
    $counter = 0;
    $matches = array();
    do {
      $match = HtmlCleanUp::match($qp, $selector, $needle, $function, $parameter, $counter);
      if ($match) {
        $matches[] = $match;
        $counter++;
      }
    } while ($match);
    return $matches;
  }

  /**
   * Like match, but removes all matching elements.
   */
  public static function matchRemoveAll($qp, $selector, $needle, $function, $parameter = NULL) {
    $matches = HtmlCleanUp::matchAll($qp, $selector, $needle, $function, $parameter);
    foreach ($matches as $match) {
      $match->remove();
    }
  }

  /**
   * Return an element if the text in the attribute matches a search needle.
   *
   * @param QueryPath $qp
   *   QueryPath object.
   * @param string $selector
   *   The CSS selector for the element to be matched.
   * @param string $needle
   *   The text string for which to search.
   * @param string $attribute
   *   The HTML attribute whose value will be searched.
   *
   * @return mixed
   *   The matched QueryPath element or FALSE.
   */
  public static function matchAttribute($qp, $selector, $needle, $attribute) {
    return HtmlCleanUp::match($qp, $selector, $needle, "attr", $attribute);
  }

  /**
   * Return an element if the text that it contains matches a search needle.
   *
   * @param QueryPath $qp
   *   A QueryPath object.
   * @param string $selector
   *   The selector to look into.
   * @param string $needle
   *   The text string for which to search.
   *
   * @return mixed
   *   The matched QueryPath element or FALSE.
   */
  public static function matchText($qp, $selector, $needle) {
    return HtmlCleanUp::match($qp, $selector, $needle, "text");
  }

  /**
   * Remove an element if the text that it contains matches a search needle.
   *
   * @param QueryPath $qp
   *   A QueryPath object.
   * @param string $selector
   *   The selector to look into.
   * @param string $needle
   *   The text string for which to search.
   */
  public static function matchTextRemoveElement($qp, $selector, $needle) {
    $element = HtmlCleanUp::match($qp, $selector, $needle, "text");
    if ($element) {
      $element->remove();
    }
  }

  /**
   * Return an element if the HMTL that it contains matches a search needle.
   *
   * @param QueryPath $qp
   *   A QueryPath object.
   * @param string $selector
   *   The selector to look into.
   * @param string $needle
   *   The text string for which to search.
   *
   * @return mixed
   *   The matched QueryPath element or FALSE.
   */
  public static function matchHtml($qp, $selector, $needle) {
    return HtmlCleanUp::match($qp, $selector, $needle, "html");
  }

  /**
   * Examine all img longdesc attr in qp and remove any that point to images.
   *
   * @param object $query_path
   *   A QueryPath object.
   */
  public static function removeFaultyImgLongdesc($query_path) {
    $imgs = $query_path->find('img[longdesc]');
    foreach ($imgs as $img) {
      $longdesc_uri = $img->attr('longdesc');
      // Longdesc can not be a uri to an image file.  Should be to txt or html.
      if (HtmlCleanup::isImageUri($longdesc_uri)) {
        $img->removeAttr('longdesc');
      }
    }
  }

  /**
   * Removes the background from tables in markup by adding class.
   *
   * @param QueryPath $query_path
   *   A QueryPath object.
   */
  public static function removeTableBackgrounds($query_path) {
    $tables = $query_path->find('table');
    foreach ($tables as $table) {
      $table->addClass('no-background');
    }
  }
}
