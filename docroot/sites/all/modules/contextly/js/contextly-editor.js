(function ($) {

  Contextly = window.Contextly || {};

  /**
   * Common features for the links/sidebars editing.
   */
  Contextly.editor = {
    isLoaded: false,
    isLoading: false,
    hasFailed: false,

    open: function(type) {
      var func = '';
      switch (type) {
        case 'snippet':
          func = this.openGeneralEditor;
          break;

        case 'link':
          func = this.openLinkEditor;
          break;

        case 'sidebar':
          func = this.openSidebarEditor;
          break;

        default:
          return;
      }

      // Remove the type from the arguments.
      var args = Array.prototype.slice.call(arguments, 1);

      // Avoid editor start when loading is still in progress or has failed.
      if (!this.hasFailed) {
        if (!this.isLoaded) {
          // Load settings if not yet started.
          if (!this.isLoading) {
            this.loadSettings();
          }
        }
        else {
          func.apply(this, args);
        }
      }
    },

    openGeneralEditor: function() {
      var s = this.getSettings();
      var url = this.buildEditorUrl('snippet', s.nid);
      this.openOverlay(url, {
        setSnippet: this.proxy(this.setSnippet),
        removeSnippet: this.proxy(this.removeSnippet)
      });
    },

    openLinkEditor: function(text, callback, context) {
      var s = this.getSettings();
      var url = this.buildEditorUrl('link', s.nid);
      this.openOverlay(url, {
        callback: this.proxy(callback, context),
        setSnippet: this.proxy(this.setSnippet),
        getText: function() {
          return text;
        }
      });
    },

    openSidebarEditor: function(sidebarId, callback, context) {
      var s = this.getSettings();
      var url = this.buildEditorUrl('sidebar', s.nid);
      this.openOverlay(url, {
        getSidebarId: function() {
          return sidebarId;
        },
        callback: this.proxy(callback, context),
        setSidebar: this.proxy(this.setSidebar),
        removeSidebar: this.proxy(this.removeSidebar),
        setSnippet: this.proxy(this.setSnippet)
      });
    },

    openOverlay: function(url, api, options) {
      // Extend API with default methods (caller is able to overwrite them).
      api = $.extend({
        getSettings: this.proxy(this.getSettings),
        buildAjaxConfig: this.proxy(this.buildAjaxConfig),
        setOverlayCloseButtonHandler: function(callback) {
          Contextly.overlay.Editor.setOptions({
            extend: true,
            onClose: callback
          });
        },
        closeOverlay: function() {
          Contextly.overlay.Editor.close();
        }
      }, api);

      // Set up event handler to respond on overlay ready events with an API.
      $(window)
        .unbind('contextlyOverlayReady')
        .bind('contextlyOverlayReady', function() {
          return api;
        });

      // Load an iframe inside modal.
      Contextly.overlay.Editor.open(url, options);
    },

    loadSettings: function() {
      this.isLoaded = false;
      this.isLoading = true;
      this.hasFailed = false;

      $.ajax(this.buildAjaxConfig('get-editor-data', {
        success: this.proxy(this.onSettingsLoadingSuccess),
        error: this.proxy(this.onSettingsLoadingFailure)
      }));

      this.fireEvent('contextlySettingsLoading');
    },

    getSettings: function() {
      return $.extend({
        nid: 0,
        token: '',
        baseUrl: '',
        sidebars: {},
        snippet: null,
        annotations: [],
        sidebarsSearchLinksLimit: 4
      }, Drupal.settings.contextlyEditor);
    },

    buildAjaxConfig: function(method, addon) {
      var s = this.getSettings();

      var url = Drupal.settings.basePath + 'contextly-ajax/' + method + '/' + s.nid;
      url = this.appendUrlToken(url);

      var result = $.extend(true, {
        url: url,
        type: 'POST',
        dataType: 'json'
      }, addon);
      return result;
    },

    buildEditorUrl: function(type, nid) {
      var url = Drupal.settings.basePath + 'contextly-editor/' + type + '/' + nid;
      return this.appendUrlToken(url);
    },

    appendUrlToken: function(url) {
      if (url.indexOf('?') === -1) {
        url += '?';
      }
      else {
        url += '&';
      }

      var s = this.getSettings();
      url += 'token=' + encodeURIComponent(s.token);

      return url;
    },

    onSettingsLoadingSuccess: function(data) {
      // Set flags.
      this.isLoaded = true;
      this.isLoading = false;
      this.hasFailed = false;

      // Put new data into the settings.
      $.extend(Drupal.settings.contextlyEditor, data);

      // Let the other scripts to know about loaded settings.
      this.fireEvent('contextlySettingsLoaded');
    },

    onSettingsLoadingFailure: function() {
      this.isLoaded = false;
      this.isLoading = false;
      this.hasFailed = true;
      this.fireEvent('contextlySettingsFailed');
    },

    fireEvent: function(type) {
      // Remove the type of event.
      var args = Array.prototype.slice.call(arguments, 1);

      $(window).triggerHandler(type, args);
    },

    setSidebar: function(entry) {
      Drupal.settings.contextlyEditor.sidebars[entry.id] = entry;
      this.fireEvent('contextlyWidgetUpdated', 'sidebar', entry.id);
    },

    setSnippet: function(entry) {
      Drupal.settings.contextlyEditor.snippet = entry;
      this.fireEvent('contextlyWidgetUpdated', 'snippet', entry.id);
    },

    removeSnippet: function(id) {
      // On empty snippet we still need settings for editor to work properly.
      var emptySnippet = {
        settings: Drupal.settings.contextlyEditor.snippet.settings
      };
      Drupal.settings.contextlyEditor.snippet = emptySnippet;
      this.fireEvent('contextlyWidgetRemoved', 'snippet', id);
    },

    removeSidebar: function(id) {
      delete Drupal.settings.contextlyEditor.sidebars[id];
      this.fireEvent('contextlyWidgetRemoved', 'sidebar', id);
    },

    renderSidebarPlaceholderBackground: function(sidebarId, sidebars) {
      sidebars = sidebars || this.getSettings().sidebars;

      if (!sidebars[sidebarId]) {
        return '';
      }
      var sidebar = sidebars[sidebarId];

      try {
        var text = Drupal.t('- Unnamed sidebar -');
        var linksCount = 0;
        if (sidebar.name) {
          text = sidebar.name;
        }

        if (sidebar.links && sidebar.links.previous) {
          linksCount = sidebar.links.previous.length || 0;
        }

        var canvas = document.createElement('canvas');
        canvas.width = 400;
        canvas.height = 40;

        var ctx = canvas.getContext('2d');
        ctx.textAlign = 'center';
        ctx.textBaseline = 'middle';

        ctx.fillStyle = '#000000';
        ctx.font = '16px Helvetica, Arial, sans-serif';
        ctx.fillText(text, 200, 12, 400);

        ctx.fillStyle = '#666666';
        ctx.font = '12px Helvetica, Arial, sans-serif';
        ctx.fillText(Drupal.formatPlural(linksCount, '@count link', '@count links'), 200, 32, 400);

        return canvas.toDataURL();
      }
      catch (e) {
        if (window.console && typeof console.log === 'function') {
          console.log('Unable to render sidebar placeholder background.');
          console.log(e);
        }
      }
    },

    proxy: function(func, context) {
      if (typeof context === 'undefined') {
        context = this;
      }

      return function() {
        return func.apply(context, arguments);
      };
    }

  };

  /**
   * Behavior to load the settings on the DOM ready.
   */
  Drupal.behaviors.contextlyEditor = {
    attach: function(context, settings) {
      $('body', context).once('contextly-editor', function() {
        Contextly.editor.loadSettings();
      });
    }
  };

})(jQuery);
