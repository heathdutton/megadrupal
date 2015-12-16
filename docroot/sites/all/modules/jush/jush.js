(function($) {

// Register the jQuery JUSH plugin.
$.jush = window.jush;

/** Highlight element content
* @param [string]
* @return jQuery
* @this jQuery
*/
$.fn.jush = function (language) {
  return this.each(function () {
    var lang = language;
    var $this = $(this);
    if (!lang) {
      var match = /(^|\s)(?:jush-|language-)(\S+)/.exec($this.attr('class'));
      lang = (match ? match[2] : 'htm');
    }
    $this.html(window.jush.highlight(lang, $this.text()));
  });
};

/**
 * Provide a Drupal-specific wrapper for JUSH Framework.
 */
Drupal.behaviors.jush = {
  attach: function(context, settings) {
    // Act on anything that is classed with "jush".
    $('.jush').once('jush').jush();
  }
};

})(jQuery);
