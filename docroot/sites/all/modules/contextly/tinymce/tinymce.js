/**
 * @file
 * Plugin for inserting links with Linkit.
 */

(function ($) {

  tinymce.create('tinymce.plugins.Contextly', {
    init: function (editor, url) {
      editor.contentCSS.push(url + '/tinymce.css');

      // Register commands.
      editor.addCommand('ContextlyLink', function () {
        var selection = editor.selection;
        var selectedNode;

        // If the selection is collapsed, try to find existing link at the
        // caret position and select it.
        if (selection.isCollapsed()) {
          selectedNode = selection.getNode();
          var existingLink = tinymce.DOM.getParent(selectedNode, 'A');
          if (existingLink) {
            selection.select(existingLink, true);
          }
        }

        var selectedText = selection.getContent({format: 'text'});

        // If only an image is selected, use its "alt" property as the text.
        // Make sure we're not going to use internal placeholder images for
        // media (flash, iframe, etc.) or sidebar placeholder.
        if (!selectedText.length) {
          selectedNode = selection.getNode();
          if (tinymce.DOM.is(selectedNode, 'img') && editor.dom.getAttrib(selectedNode, 'class', '').indexOf('mceItem') === -1) {
            selectedText = tinymce.DOM.getAttrib(selectedNode, 'alt', '');
          }
        }

        // Make sure we have some text selected.
        if (selectedText) {
          // Bookmark the selection, so we don't loose it while the link is
          // edited inside the Contextly window.
          var bookmark = selection.getBookmark();

          // Open Contextly editor for the selected text.
          Contextly.editor.open('link', selectedText, function (result) {
            // Move to the saved bookmark first.
            selection.moveToBookmark(bookmark);

            // Remove any existing links at the selection and create a new one.
            editor.execCommand('mceInsertLink', false, {
              href: result.link_url,
              title: result.link_title
            }, {skip_undo: true});
          });
        }
      });
      editor.addCommand('ContextlySidebar', function () {
        var existingId = null;

        // Try to get ID of existing sidebar inside the selection.
        var selection = editor.selection;
        var selectedNode = selection.getNode();
        if (selectedNode && tinymce.DOM.is(selectedNode, 'img.mce-contextly-sidebar')) {
          existingId = tinymce.DOM.getAttrib(selectedNode, 'data-contextly-sidebar-id', '');
        }

        Contextly.editor.open('sidebar', existingId, function (savedSidebar) {
          var newId = savedSidebar.id;

          if (existingId) {
            // Sidebar exists, just update its ID if necessary.
            if (existingId !== newId) {
              tinymce.DOM.setAttrib(selectedNode, 'data-contextly-sidebar-id', newId);
            }
          }
          else {
            // New sidebar, make sure selection is collapsed and insert it to
            // the caret position
            if (!selection.isCollapsed()) {
              selection.collapse();
            }
            var element = editor.dom.create('img', getPlaceholderAttributes(editor, newId));
            selection.setNode(element);
          }
        });
      });

      // Register buttons.
      editor.addButton('contextlylink', {
        title: 'Insert/edit link',
        cmd: 'ContextlyLink',
        image: url + '/icons/link.png'
      });
      editor.addButton('contextlysidebar', {
        title: 'Insert/edit sidebar',
        cmd: 'ContextlySidebar',
        image: url + '/icons/sidebar.png'
      });

      // Reaction on the node change to update link button state.
      editor.onNodeChange.add(function(editor, controlManager, node) {
        // Search for the link and update the active state of the button.
        var link = tinymce.DOM.getParent(node, 'A');
        controlManager.setActive('contextlylink', !!link);

        // Set selection criterion based on the selection.
        if (
          link ||
          editor.selection.getContent({format: 'text'}) ||
          (tinymce.DOM.is(node, 'img') && editor.dom.getAttrib(node, 'class', '').indexOf('mceItem') === -1 && editor.dom.getAttrib(node, 'alt', '').length > 0)
        ) {
          // There is a link, text or an non-placeholder image with non-empty
          // "alt" attribute selected.
          buttonsEnabled.link.selection = true;
        }
        else {
          buttonsEnabled.link.selection = false;
        }

        refreshButtonState('link');
      });

      // Reaction on the node change to update sidebar button state.
      editor.onNodeChange.add(function (editor, controlManager, node) {
        // Search for the link and update the active state of the button.
        var sidebar = editor.dom.is(node, 'img.mce-contextly-sidebar');
        controlManager.setActive('contextlysidebar', !!sidebar);
      });

      // Enabled state conditions for all buttons. When all conditions are true
      // the button should have enabled state.
      var buttonsEnabled = {
        link: {
          settings: false,
          selection: false
        },
        sidebar: {
          settings: false
        }
      };

      // Refreshes enabled/disabled state of the specified button or all buttons
      // if argument is omitted.
      var refreshButtonState = function(button) {
        // If the button is not set, iterate through all buttons.
        if (!button) {
          for (var key in buttonsEnabled) {
            refreshButtonState(key);
          }
          return;
        }

        var disabled = false;
        for (var criterion in buttonsEnabled[button]) {
          if (!buttonsEnabled[button][criterion]) {
            disabled = true;
            break;
          }
        }

        editor.controlManager.setDisabled('contextly' + button, disabled);
      };

      /**
       * Returns all sidebars placeholders inside the editor.
       */
      var getSidebars = function() {
        return editor.dom.select('img.mce-contextly-sidebar');
      };

      /**
       * Returns placeholder of the specified sidebar inside the editor.
       *
       * @param {string} [id]
       */
      var getSidebarsById = function(id) {
        var allSidebars = getSidebars();
        var result = [];
        for (var i in allSidebars) {
          if (editor.dom.getAttrib(allSidebars[i], 'data-contextly-sidebar-id', '') === id) {
            result.push(allSidebars[i]);
          }
        }

        return result;
      };

      var cleanupSidebarClasses = function (sidebar) {
        var classes = editor.dom.getAttrib(sidebar, 'class', '');
        var regex = /(?:^|\s+)mce-contextly-sidebar-\S+(?=\s|$)/i;
        if (regex.test(classes)) {
          classes = classes.replace(regex, '').replace(/^\s+/, '');
          editor.dom.setAttrib(sidebar, 'class', classes);
        }
      };

      /**
       * Applies settings to the passed sidebar.
       *
       * @param sidebar
       *   Sidebar placeholder element inside the editor.
       * @param settings
       *   Settings of all sidebars.
       */
      var applySidebarSettings = function(sidebar, settings) {
        cleanupSidebarClasses(sidebar);

        var sidebarId = editor.dom.getAttrib(sidebar, 'data-contextly-sidebar-id', '');
        if (sidebarId && settings[sidebarId] && settings[sidebarId].layout) {
          // Set up left/right/wide classes.
          switch (settings[sidebarId].layout) {
            case 'left':
            case 'right':
            case 'wide':
              editor.dom.addClass(sidebar, 'mce-contextly-sidebar-' + settings[sidebarId].layout);
              break;

            default:
              // Unknown alignment type. Mark as broken.
              editor.dom.addClass(sidebar, 'mce-contextly-sidebar-broken');
              break;
          }

          // Render title of the sidebar on a canvas and set it as a background.
          var dataUrl = Contextly.editor.renderSidebarPlaceholderBackground(sidebarId, settings);
          if (dataUrl) {
            editor.dom.setStyle(sidebar, 'background-image', "url('" + dataUrl + "')");
          }
        }
        else {
          editor.dom.addClass(sidebar, 'mce-contextly-sidebar-broken');
        }
      };

      // Settings loading started callback.
      var onSettingsLoading = function() {
        var key;

        // Update buttons state.
        for (key in buttonsEnabled) {
          buttonsEnabled[key].settings = false;
        }
        refreshButtonState();

        // Update sidebars classes.
        var sidebars = getSidebars();
        var sidebar;
        for (key in sidebars) {
          sidebar = sidebars[key];
          cleanupSidebarClasses(sidebar);
          editor.dom.addClass(sidebar, 'mce-contextly-sidebar-loading');
        }
      };

      // Settings loading successfully finished callback.
      var onSettingsLoaded = function() {
        // Update buttons state.
        for (var key in buttonsEnabled) {
          buttonsEnabled[key].settings = true;
        }
        refreshButtonState();

        // Get loaded sidebars and update their classes.
        var settings = Contextly.editor.getSettings().sidebars;
        var sidebars = getSidebars();
        for (key in sidebars) {
          applySidebarSettings(sidebars[key], settings);
        }
      };

      // Settings loading failed callback.
      var onSettingsFailed = function() {
        // TODO: Message to the user?
      };

      // The widget was updated by this or any other editor instance.
      var onWidgetUpdated = function(e, widgetType, id) {
        switch (widgetType) {
          case 'sidebar':
            // Apply settings to the updated sidebar.
            var sidebars = getSidebarsById(id);
            for (var index in sidebars) {
              var sidebar = sidebars[index];
              var settings = Contextly.editor.getSettings().sidebars;
              applySidebarSettings(sidebar, settings);
            }
            break;
        }
      };

      // The widget was removed by this or any other editor instance.
      var onWidgetRemoved = function(e, widgetType, id) {
        switch (widgetType) {
          case 'sidebar':
            // Remove all instances of the sidebar.
            var sidebars = getSidebarsById(id);
            for (var index in sidebars) {
              editor.dom.remove(sidebars[index]);
            }
            break;
        }
      };

      // Adds unique namespace to the event type.
      var eventNamespace = function(type) {
        if (!type) {
          type = '';
        }
        return type + '.contextlyTinymce' + editor.id;
      };

      // Init buttons state right after the editor initialization has been
      // finished.
      editor.onInit.add(function() {
        // Set up event handlers to enable/disable buttons on the settings
        // loading events.
        var eventHandlers = {
          'contextlySettingsLoading': onSettingsLoading,
          'contextlySettingsLoaded': onSettingsLoaded,
          'contextlySettingsFailed': onSettingsFailed,
          'contextlyWidgetUpdated': onWidgetUpdated,
          'contextlyWidgetRemoved': onWidgetRemoved
        };
        var $window = $(window);
        for (var key in eventHandlers) {
          $window.bind(eventNamespace(key), eventHandlers[key]);
        }

        // Refresh buttons/sidebars state depending on the settings state.
        if (window.Contextly && Contextly.editor) {
          if (Contextly.editor.isLoaded) {
            onSettingsLoaded.call(window);
          }
          else if (Contextly.editor.isLoading) {
            onSettingsLoading.call(window);
          }
        }
        else {
          refreshButtonState();
        }
      });

      // Unbind the events on editor removing.
      editor.onRemove.add(function() {
        $(window).unbind(eventNamespace());
      });

      // On setting content (text to HTML) replace sidebar comment with the
      // placeholder image.
      editor.onBeforeSetContent.add(function (ed, o) {
        if (!o || !o.content) {
          return;
        }

        var commentRegexp = /<!--contextly-sidebar\s+([^\->]+)-->/gi;
        o.content = o.content.replace(commentRegexp, function(full, id) {
          return ed.dom.createHTML('img', getPlaceholderAttributes(ed, id));
        });
      });

      // On getting content (HTML to text) replace the placeholder image with
      // the comment.
      editor.onPostProcess.add(function (ed, o) {
        if (!o || !o.content) {
          return;
        }

        var imgRegexp = /<img[^>]+data-contextly-sidebar-id="([^">]+)"[^>]*>/gi;
        o.content = o.content.replace(imgRegexp, function(full, id) {
          return '<!--contextly-sidebar ' + id + '-->';
        });
      });
    },

    getInfo: function () {
      return {
        longname: 'Contextly',
        author: 'Contextly',
        authorurl: 'http://contextly.com',
        infourl: 'http://contextly.com',
        version: "1.0"
      };
    }
  });

  // Register the plugin.
  tinymce.PluginManager.add('contextly', tinymce.plugins.Contextly);

  /**
   * Returns attributes for the sidebar placeholder.
   *
   * Class "mceItem" is added here to prevent active state of the standard image
   * button of the editor on sidebar node selection.
   *
   * @param editor
   * @param sidebarId
   *
   * @returns {object}
   */
  var getPlaceholderAttributes = function (editor, sidebarId) {
    return {
      'alt': Drupal.t('Contextly sidebar'),
      'data-contextly-sidebar-id': sidebarId,
      'class': 'mce-contextly-sidebar mceItem',
      'src': editor.theme.url + '/img/trans.gif'
    };
  };

})(jQuery);