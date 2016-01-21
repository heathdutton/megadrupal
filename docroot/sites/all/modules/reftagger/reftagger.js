(function($, Drupal, document, window) {
  // This variable must be set to global scope.
  window['refTagger'] = window['refTagger'] || {
    settings: {}
  };

  /**
   * Attaches RefTagger extenral JS, and attach settings.
   */
  Drupal.behaviors.refTagger = {
    attach: function(context, settings) {
      window['refTagger'].settings = Drupal.settings.refTagger;

      var g = document.createElement('script'),
        s = document.getElementsByTagName('script')[0];
      g.src = "//api.reftagger.com/v2/RefTagger.js";
      s.parentNode.insertBefore(g, s);
    }
  }
}) (jQuery, Drupal, document, window);
