/**
 * @file
 * Used to initialize the plugin when the page has finished loading.
 */

(function ($) {
  Drupal.behaviors.FLUID = {
    attach: function (context, settings) {

      var origin = Drupal.settings.basePath;
      var modulePath = Drupal.settings.modulePath;
      var libraryPath = Drupal.settings.libraryPath;

      fluid.uiOptions.prefsEditor(".flc-prefsEditor-separatedPanel", {
        "templatePrefix": libraryPath + "/framework/preferences/html/",
        "messagePrefix": origin + modulePath + "/messages/",
        "tocTemplate": libraryPath + "/components/tableOfContents/html/TableOfContents.html"
      });
      }
  };
}(jQuery));
