/*!
 * jQuery Plugin: Highlight Search Terms - version 0.3
 * http://github.com/objectoriented/jquery.highlight-search-terms
 * Highlight search terms in referrer URL from Google, Yahoo!, Bing and custom site.
 *
 * Copyright (c) 2009 Kyo Nagashima <kyo@hail2u.net>
 * This library licensed under MIT license:
 * http://opensource.org/licenses/mit-license.php
 */
(function ($) {
  $.fn.highlightSearchTerms = function (options) {
    var o = $.extend({}, $.fn.highlightSearchTerms.defaults, options);

    //$.merge(o.referrerPatterns, $.fn.highlightSearchTerms.builtinReferrerPatterns);
    var ref = o.referrer || document.referrer;
    if (ref) {
      var searchTerms = extractSearchTerms(ref, o);
      
      // Replace search terms
      var stopWordRegex = new RegExp("\\b(" + o.stopWords.join("|") + "\\|?)\\b", "gi");
      var highlightedTerms = searchTerms.replace(stopWordRegex, "").replace(/[\\|]+/g, '|').replace(/\s/g, '|').replace(/^[\\|]/, '').replace(/[\\|]$/, '');

      // Highlight terms
      if (highlightedTerms !== "") {
        $(Drupal.settings.highlight.area).highlight(highlightedTerms.split('|'), { wordsOnly: Drupal.settings.highlight.wordsonly});
      }
    }

    return this;
  };

  // Private: Extract terms from referrer
  function extractSearchTerms (ref, o) {
    var terms = "";

    $.each(o.referrerPatterns, function () {
      var pattern = new RegExp(this, "i");

      if (pattern.exec(ref)) {
        var unsafe = new RegExp(o.unsafeChars, "g");
        terms = decodeURIComponent(RegExp.$1).replace(unsafe, "+").replace(/^\+*(.*?)\+*$/, "$1").replace(/\++/g, "|");

        return false; // break $.each
      }
    });

    return terms;
  }

  // Private: Encode entities
  function encodeEntities (s) {
    return $("<u/>").text(s).html(); // jQuery magic
  }

  // Public: default options
  $.fn.highlightSearchTerms.defaults = {
    referrer:         '',
    className:        "highlight",
    referrerPatterns: [],
    unsafeChars:      "[!-*,-/:-@[-`{-~]",
    stopWords:        Drupal.settings.highlight.stopwords.split(',')
  };

  // Public: built-in referrer patterns for Google(com|co.jp), Yahoo!(com|co.jp), Bing.
  //$.fn.highlightSearchTerms.builtinReferrerPatterns = [];
  
  Drupal.behaviors.highlight = {
    'attach': function(context, settings) {      
      $(Drupal.settings.highlight.area).highlightSearchTerms({referrerPatterns: Drupal.settings.highlight.referrerPatterns.split(/\r\n|\r|\n/)});
      $(Drupal.settings.highlight.area).highlightSearchTerms({referrerPatterns: Drupal.settings.highlight.patterns.split(/\r\n|\r|\n/), referrer: document.URL});
      $('.highlight').css('background-color', Drupal.settings.highlight.color);
    }
  };
})(jQuery);
