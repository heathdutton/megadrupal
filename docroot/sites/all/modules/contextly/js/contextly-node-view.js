(function ($) {

  Contextly.Settings = Contextly.createClass({

    extend: Contextly.BaseSettings,

    statics: {

      /**
       * @param key
       * @param [fallback]
       */
      getDrupalSetting: function(key, fallback) {
        if (!Drupal.settings.contextlyWidgets) {
          return fallback;
        }

        if (typeof Drupal.settings.contextlyWidgets[key] === 'undefined') {
          return fallback;
        }

        return Drupal.settings.contextlyWidgets[key];
      },

      /**
       * Returns API server URL.
       *
       * @function
       */
      getAPIServerUrl: function() {
        return this.getDrupalSetting('apiServerURL');
      },

      /**
       * Returns main Contextly server URL.
       *
       * @function
       */
      getMainServerUrl: function() {
        return this.getDrupalSetting('mainServerURL');
      },

      /**
       * Returns WP plug-in version.
       *
       * @todo Replace with Kit version and drop it.
       *
       * @function
       */
      getPluginVersion: function() {
        return this.getDrupalSetting('version');
      },

      /**
       * @function
       */
      getAppId: function() {
        return this.getDrupalSetting('appId');
      }

    }

  });

  // Override page view method to control which widgets must be displayed.
  (function() {
    var original = Contextly.PageView.getDisplayableWidgetCollections;
    Contextly.PageView.getDisplayableWidgetCollections = function(response) {
      if (Contextly.Settings.getDrupalSetting('renderLinkWidgets')) {
        return original.apply(this, arguments);
      }
      else {
        // Still display storyline subscribe button on turned off link widgets.
        return [
          response.entry.storyline_subscribe
        ];
      }
    };
  })();

  /**
   * Loads settings of all page snippets on the node page and displays them.
   */
  Drupal.behaviors.contextlyWidgets = {
    attach: function (context, settings) {
      if (!settings.contextlyWidgets) {
        return;
      }

      // Make sure we initialize widgets only once on the page.
      var isFirstTime = $('body', context)
        .once('contextly-widgets')
        .length;
      if (!isFirstTime) {
        return;
      }

      // Get widget containers and make sure they exist.
      var containers = $('#ctx-module, .ctx-sidebar, #ctx-sl-subscribe', context);
      if (!containers.length) {
        return;
      }

      Contextly.load();
    }
  };

})(jQuery);
