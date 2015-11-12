/**
 * @file
 * Initializes Outdated Browser library.
 */

(function ($) {
Drupal.behaviors.initOutdatedbrowser = {
  attach: function (context, settings) {
    outdatedBrowser({
      bgColor: settings.outdatedbrowser.bgColor,
      color: settings.outdatedbrowser.color,
      lowerThan: settings.outdatedbrowser.lowerThan,
      languagePath: settings.outdatedbrowser.languagePath
    });
  }
};
})(jQuery);
