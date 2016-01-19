(function($) {

  // Get the undefined type string.
  var TypeUndefined = typeof (undefined);

  /**
   * Declare the Widget Block module as behavior capable.
   */
  Drupal.behaviors.WidgetBlock = {
    // Register for Drupal attach behavior.
    "attach": function(context, settings) {
      // Check if the Widget Core API is available.
      if (typeof (WidgetApi) !== TypeUndefined && typeof (WidgetApi.Core) !== TypeUndefined && typeof (WidgetApi.Core.detach) !== TypeUndefined) {
        // Attach the widgets.
        WidgetApi.Core.attach();
      }
      // Check if the Widget Block settings are present.
      if (typeof (settings.WidgetBlock) !== TypeUndefined) {
        // Ensure the WidgetApi.Language namespace exists.
        window.WidgetApi = window.WidgetApi || {};
        window.WidgetApi.Language = window.WidgetApi.Language || {};
        // Overwrite the getCurrentLanguage function.
        window.WidgetApi.Language.getCurrentLanguage = function() {
          return settings.WidgetBlock.currentLanguage;
        };
      }
    },
    // Register for Drupal detach behavior.
    "detach": function() {
      // Check if the Widget Core API is available.
      if (typeof (WidgetApi) !== TypeUndefined && typeof (WidgetApi.Core) !== TypeUndefined && typeof (WidgetApi.Core.detach) !== TypeUndefined) {
        // Detach the widgets.
        WidgetApi.Core.detach();
      }
    }
  };

})(jQuery);
