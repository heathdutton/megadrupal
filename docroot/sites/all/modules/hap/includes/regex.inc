<?php

// @todo pull in these regex patterns from http://html5pattern.com/

define('HAP_REGEX_SUPPORTED_PROTOCOLS', 'http|https|ssh|ftp|git|svn');

define('HAP_REGEX_FILE', '
  /
    (?P<filename>                 # Capture the entire filename
      (?P<basename>[\w\d\-_]+)    # Capture the base name of the file
      \.(?P<extension>(?:\w)+)    # Capture the file extension, but without the leading dot (.)
    )$                            # Anchor to the end of string
  /x');


define('HAP_REGEX_URL', '
  /^                                          # Anchor to beginning of string
    (?P<url>                                  # Capture entire URL @see RFC 3986
      (?P<protocol>(?:[\w][\w\d+\-.]*)):\/\/  # Capture the protocol protocol
      (?P<subdomains>(?:[\w\d\-_]+\.)*)       # Capture any subdomains
      (?P<hostname>[\w\d\-_]+)\/              # Capture the hostname
      (?P<uri>[\S]*)                          # Capture any remaining end of the URL as the URI.
    )$                                        # Anchor to end of string
  /x');


/**
 * Similar to preg_match, but $matches only contains named captures.
 *
 * Note: this function is almost 4 times slower than simply running preg_match,
 * because we are taking the extra time to filter through the matches to remove
 * numeric keys. You should only really use this if the numeric indexes annoy
 * you.
 */
function hap_regex_capture($pattern, $subject, &$matches) {
  if ($results = preg_match($pattern, $subject, $matches)) {
    foreach (array_keys($matches) as $i) {
      if (is_numeric($i)) {
        unset($matches[$i]);
      }
    }
    return $results;
  }
}
