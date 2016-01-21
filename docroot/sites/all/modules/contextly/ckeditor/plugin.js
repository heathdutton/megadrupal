(function($) {

  CKEDITOR.plugins.add('contextly', {
    onLoad: function() {
      // Style for the sidebar fake element.
      // Starting from CKEditor 4.0 styles are applied for whole document on
      // plugin loading.
      if (editorVersionMatches({min: '4'})) {
        CKEDITOR.addCss(getEditorCss());
      }
    },

    init: function (editor) {
      // Style for the sidebar fake element.
      // Starting from CKEditor 4.0 styles are applied for whole document on
      // plugin loading.
      if (editorVersionMatches({max: '4'})) {
        editor.addCss(getEditorCss());
      }

      // Add commands.
      editor.addCommand('contextlylink', {
        startDisabled: true,
        allowedContent: 'a[!href]',
        requiredContent: 'a[href]',
        exec: function (editor) {
          var selection = editor.getSelection();

          // If selection is inside the existing link, use whole link. If we
          // don't do it, link replacing will fail. Core link plugin does
          // the same.
          var existingLink = findLinkAtSelection();
          if (existingLink) {
            selection.selectElement(existingLink);
          }

          // Get selected text. The function selection.getSelectedText()
          // requires CKEditor 3.6.1.
          var text = selection.getSelectedText();

          // If only an image is selected, use its "alt" property as the text.
          if (!text) {
            // Get selected image if any. Make sure we're not going to use
            // fake object (placeholders for iframe, flash, etc.) or our sidebar
            // placeholder.
            var selectedElement = getSelectedElement(selection);
            if (selectedElement && selectedElement.is('img') && !selectedElement.data('contextly-sidebar-id') && !selectedElement.data('cke-realelement')) {
              var alt = selectedElement.getAttribute('alt');
              if (alt !== null) {
                text = alt;
              }
            }
          }

          if (!text) {
            // No text selected, popup window won't switch to the link mode.
            // TODO: Message to the user?
            return;
          }

          // Save selection for future.
          var bookmarks = selection.createBookmarks();

          // Open dialog to let the user select the link.
          Contextly.editor.open('link', text, function (result) {
            // First of all, return to the saved selection.
            selection.selectBookmarks(bookmarks);

            // Unlink first.
            unlinkSelection();

            // Apply the A style to the editor, so CKEditor handles all the edge
            // situations by itself (like selection across multiple paragraphs).
            var style = new CKEDITOR.style({
              element: 'a',
              attributes: {
                href: result.link_url,
                title: result.link_title
              }
            });
            style.type = CKEDITOR.STYLE_INLINE;
            style.apply(editor.document);
          });
        }
      });
      editor.addCommand('contextlysidebar', {
        startDisabled: true,
        exec: function (editor) {
          var existingId = null;

          // Try to get ID of existing sidebar inside the selection.
          var selection = editor.getSelection();
          var selectedElement = getSelectedElement(selection);
          if (selectedElement && selectedElement.is('img')) {
            existingId = selectedElement.data('contextly-sidebar-id');
          }

          Contextly.editor.open('sidebar', existingId, function(savedSidebar) {
            var newId = savedSidebar.id;
            if (existingId) {
              // Sidebar exists, just update its ID if necessary.
              if (existingId !== newId) {
                selectedElement.data('contextly-sidebar-id', newId);
              }
            }
            else {
              // New sidebar, insert it to the caret position.
              var imageElement = editor.document.createElement('img');
              var attributes = getPlaceholderAttributes(newId);
              imageElement.setAttributes(attributes);
              editor.insertElement(imageElement);
            }
          });
        }
      });

      // Add buttons.
      if (editor.ui.addButton) {
        editor.ui.addButton('ContextlyLink', {
          label: Drupal.t('Insert/edit link'),
          command: 'contextlylink',
          icon: this.path + 'icons/link.png'
        });
        editor.ui.addButton('ContextlySidebar', {
          label: Drupal.t('Insert/edit Contextly sidebar'),
          command: 'contextlysidebar',
          icon: this.path + 'icons/sidebar.png'
        });
      }

      // Register the menu items if "menu" plugin is loaded.
      if (editor.addMenuItem && editor.addMenuGroup) {
        editor.addMenuGroup('contextly');
        editor.addMenuItem('ContextlyInsertLink', {
          label: Drupal.t('Insert link'),
          command: 'contextlylink',
          group: 'contextly',
          order: 1,
          icon: this.path + 'icons/link.png'
        });
        editor.addMenuItem('ContextlyEditLink', {
          label: Drupal.t('Edit link'),
          command: 'contextlylink',
          group: 'contextly',
          order: 1,
          icon: this.path + 'icons/link.png'
        });
        editor.addMenuItem('ContextlyEditSidebar', {
          label: Drupal.t('Edit sidebar'),
          command: 'contextlysidebar',
          group: 'contextly',
          order: 2,
          icon: this.path + 'icons/sidebar.png'
        });
      }

      // If the "contextmenu" plugin is loaded, register the listeners.
      if (editor.contextMenu) {
        // Link menu items.
        editor.contextMenu.addListener(function (element, selection) {
          if (!element || element.isReadOnly()) {
            return null;
          }

          var link = findLinkAtSelection();
          if (link) {
            return {ContextlyEditLink: CKEDITOR.TRISTATE_OFF};
          }
          else {
            // Depends on the CKEditor version >= 3.6.1
            var text = selection.getSelectedText();
            if (text) {
              return {ContextlyInsertLink: CKEDITOR.TRISTATE_OFF};
            }
          }

          return null;
        });

        // Sidebar menu items.
        editor.contextMenu.addListener(function (element, selection) {
          if (!element || element.isReadOnly()) {
            return null;
          }

          if (element.is('img') && element.data('contextly-sidebar-id')) {
            return {ContextlyEditSidebar: CKEDITOR.TRISTATE_OFF};
          }

          return null;
        });
      }

      // Handle selection change.
      editor.on('selectionChange', function (evt) {
        if (editor.readOnly) {
          return;
        }

        // Link button state.
        var element = evt.data.path.lastElement && evt.data.path.lastElement.getAscendant('a', true);
        if (element && element.getName() == 'a' && element.getAttribute('href')) {
          buttonsState.link.selection = CKEDITOR.TRISTATE_ON;
        }
        else {
          // This event is not always fired on caret position change, so
          // it is quite useless to enable/disable commands. We use it for
          // changing the command "active" state only.
          // See http://dev.ckeditor.com/ticket/6443
          buttonsState.link.selection = CKEDITOR.TRISTATE_OFF;
        }
        refreshButtonState('link');

        // Sidebar button state.
        element = evt.data.path.lastElement;
        if (element && element.is('img') && element.data('contextly-sidebar-id')) {
          buttonsState.sidebar.selection = CKEDITOR.TRISTATE_ON;
        }
        else {
          buttonsState.sidebar.selection = CKEDITOR.TRISTATE_OFF;
        }
        refreshButtonState('sidebar');
      });

      // Handle mode switch to get buttons back to life on source -> html mode
      // change.
      editor.on('mode', function(evt) {
        var is_wysiwyg = (evt.editor.mode == 'wysiwyg');
        for (var key in buttonsState) {
          // General editor button is available on all modes, the rest buttons
          // are only available on the WYSIWYG mode.
          if (is_wysiwyg || key == 'editor') {
            buttonsState[key].mode = CKEDITOR.TRISTATE_OFF;
          }
          else {
            buttonsState[key].mode = CKEDITOR.TRISTATE_DISABLED;
          }
        }
        refreshButtonState();
      });

      // Bind double click event to open sidebar editor. Bind with lower
      // priority (default is 10) to be called before standard image handler,
      // so we could prevent its dialog from appearing.
      editor.on('doubleclick', function (evt) {
        var element = evt.data.element;

        if (element.is('img') && element.data('contextly-sidebar-id') && !element.isReadOnly()) {
          // Prevent image editing dialog from appearing.
          evt.stop();
          editor.execCommand('contextlysidebar');
        }
      }, null, null, 5);

      // Init commands and events state on the editor instanceReady event.
      editor.on('instanceReady', function () {
        // Set up event handlers for the settings events.
        setupSettingsEvents();

        // Check current settings state and init buttons/sidebars.
        initSettingsState();
      });

      // Remove event handlers of the settings events on instance destroy.
      editor.on('destroy', function (evt) {
        $(window).unbind(eventNamespace());
      });

      // Buttons state criteria.
      var buttonsState = {
        link: {
          mode: CKEDITOR.TRISTATE_DISABLED,
          settings: CKEDITOR.TRISTATE_DISABLED,
          selection: CKEDITOR.TRISTATE_OFF
        },
        sidebar: {
          mode: CKEDITOR.TRISTATE_DISABLED,
          settings: CKEDITOR.TRISTATE_DISABLED,
          selection: CKEDITOR.TRISTATE_OFF
        }
      };

      // Returns current state of the passed button.
      var getButtonState = function(button) {
        var state = CKEDITOR.TRISTATE_OFF;

        for (var criterion in buttonsState[button]) {
          switch (buttonsState[button][criterion]) {
            case CKEDITOR.TRISTATE_DISABLED:
              // Return disabled state if any criterion says it should be
              // disabled.
              return CKEDITOR.TRISTATE_DISABLED;

            case CKEDITOR.TRISTATE_ON:
              // Switch to the active state and continue the loop to search for
              // possible disabled state.
              state = CKEDITOR.TRISTATE_ON;
              break;
          }
        }

        return state;
      };

      // Refreshes enabled/disabled/active state of the specified button or all
      // buttons if argument is omitted.
      var refreshButtonState = function (button) {
        // If the button is not set, iterate through all buttons.
        if (!button) {
          for (var key in buttonsState) {
            refreshButtonState(key);
          }
          return;
        }

        var state = getButtonState(button);
        var command = editor.getCommand('contextly' + button);
        command.setState(state);
      };

      // Checks current settings state and launches corresponding event handler.
      var initSettingsState = function () {
        if (window.Contextly && Contextly.editor) {
          if (Contextly.editor.isLoaded) {
            onSettingsLoaded.call(window);
          }
          else if (Contextly.editor.isLoading) {
            onSettingsLoading.call(window);
          }
        }
      };

      /**
       * Returns placeholder of the specified sidebar or null.
       *
       * @param {string} id
       */
      var getSidebarsById = function(id) {
        var images = editor.document.getElementsByTag('img');
        var sidebars = [];
        for (var i = 0; i < images.count(); i++) {
          var image = images.getItem(i);
          var currentId = image.getAttribute('data-contextly-sidebar-id');
          if (currentId && currentId === id) {
            sidebars.push(image);
          }
        }
        return sidebars;
      };

      /**
       * Returns all sidebar placeholders.
       */
      var getSidebars = function() {
        var images = editor.document.getElementsByTag('img');
        var sidebars = [];
        var image;
        for (var i = 0; i < images.count(); i++) {
          image = images.getItem(i);
          if (image.getAttribute('data-contextly-sidebar-id')) {
            sidebars.push(image);
          }
        }
        return sidebars;
      };

      // Settings loading started callback.
      var onSettingsLoading = function () {
        var key;

        // Refresh buttons status.
        for (key in buttonsState) {
          buttonsState[key].settings = CKEDITOR.TRISTATE_DISABLED;
        }
        refreshButtonState();

        // Assign class to the sidebars.
        var sidebars = getSidebars();
        for (key in sidebars) {
          applySidebarSettings(sidebars[key]);
        }
      };

      // Settings loading successfully finished callback.
      var onSettingsLoaded = function () {
        var key;

        for (key in buttonsState) {
          buttonsState[key].settings = CKEDITOR.TRISTATE_OFF;
        }
        refreshButtonState();

        // Get loaded sidebars and update their classes.
        var sidebars = getSidebars();
        for (key in sidebars) {
          applySidebarSettings(sidebars[key]);
        }
      };

      // Settings loading failed callback.
      var onSettingsFailed = function () {
        // TODO: Message to the user?
      };

      // Callback for the widget update.
      var onWidgetUpdated = function(e, widgetType, id) {
        switch (widgetType) {
          case 'sidebar':
            // Apply settings to the updated sidebar.
            var sidebars = getSidebarsById(id);
            for (var index in sidebars) {
              applySidebarSettings(sidebars[index]);
            }
            break;
        }
      };

      // Callback for the widget removal.
      var onWidgetRemoved = function(e, widgetType, id) {
        switch (widgetType) {
          case 'sidebar':
            // Remove all instances of the sidebar.
            var sidebars = getSidebarsById(id);
            for (var index in sidebars) {
              sidebars[index].remove();
            }
            break;
        }
      };

      // Adds unique namespace to the event type.
      var eventNamespace = function (type) {
        if (!type) {
          type = '';
        }
        return type + '.contextlyCKEditor' + editor.id;
      };

      var setupSettingsEvents = function() {
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
      };

      // Removes the links from the selection.
      // It's a copy of CKEDITOR.unlinkCommand.prototype.exec(). We can't be
      // sure that the link plugin is loaded, so use our own copy.
      var unlinkSelection = function () {
        var selection = editor.getSelection(),
          bookmarks = selection.createBookmarks(),
          ranges = selection.getRanges(),
          rangeRoot,
          element;

        for (var i = 0; i < ranges.length; i++) {
          rangeRoot = ranges[i].getCommonAncestor(true);
          element = rangeRoot.getAscendant('a', true);
          if (!element)
            continue;
          ranges[i].selectNodeContents(element);
        }

        selection.selectRanges(ranges);
        editor.document.$.execCommand('unlink', false, null);
        selection.selectBookmarks(bookmarks);
      };

      // Returns the link element around the current selection if any.
      var findLinkAtSelection = function() {
        var commonAncestor = editor.getSelection().getCommonAncestor();
        if (!commonAncestor) {
          return null;
        }

        var existingLink = commonAncestor.getAscendant('a', true);
        if (existingLink) {
          return existingLink;
        }
        else {
          return null;
        }
      };
    },

    afterInit: function(editor) {
      var dataProcessor = editor.dataProcessor,
        dataFilter = dataProcessor && dataProcessor.dataFilter,
        htmlFilter = dataProcessor && dataProcessor.htmlFilter;

      // Add data filter (text to HTML processing).
      if (dataFilter) {
        // Convert from <!--contextly-sidebar ID--> to <img data-contextly-sidebar-id="ID" />
        var sidebarRegexp = /^contextly-sidebar\s+(\S+)$/i;
        dataFilter.addRules({
          comment: function (value, node) {
            var matches = value.match(sidebarRegexp);
            if (matches) {
              var attributes = getPlaceholderAttributes(matches[1]);
              var sidebar = new CKEDITOR.htmlParser.element('img', attributes);
              applySidebarSettings(sidebar, true);
              return sidebar;
            }
            else {
              return value;
            }
          }
        });
      }

      // Add HTML filter (HTML to text processing).
      if (htmlFilter) {
        // Convert fake image element back to <!--contextly-sidebar ID-->
        htmlFilter.addRules({
          elements: {
            img: function (element) {
              if (element.name === 'img' && element.attributes && element.attributes['data-contextly-sidebar-id']) {
                return new CKEDITOR.htmlParser.comment('contextly-sidebar ' + element.attributes['data-contextly-sidebar-id']);
              }
              else {
                return element;
              }
            }
          }
        });
      }
    }
  });

  /**
   * Contextly plugin definition registered earlier.
   *
   * It is used to get plugin path for resources.
   */
  var plugin = CKEDITOR.plugins.get('contextly');

  /**
   * Returns attributes for the sidebar placeholder.
   *
   * @param sidebarId
   *
   * @returns {object}
   */
  var getPlaceholderAttributes = function(sidebarId) {
    var attributes = {
      'alt': Drupal.t('Contextly sidebar'),
      'data-contextly-sidebar-id': sidebarId,
      'class': 'cke-contextly-sidebar'
    };

    // Do not set "src" on high-contrast so the alt text is displayed.
    if (!CKEDITOR.env.hc) {
      attributes.src = CKEDITOR.getUrl(plugin.path + 'images/spacer.gif');
    }

    return attributes;
  };

  /**
   * Returns selected element.
   *
   * Sometimes selection type falls back to the text selection when it is
   * suppose to be the element selection. This function uses different methods
   * according to the selection type.
   *
   * The problem is reproducible with the selected sidebar sometimes not
   * editable.
   */
  var getSelectedElement = function(selection) {
    switch (selection.getType()) {
      case CKEDITOR.SELECTION_ELEMENT:
        return selection.getSelectedElement();

      case CKEDITOR.SELECTION_TEXT:
        return selection.getStartElement();

      default:
        return null;
    }
  };

  var getEditorCss = function() {
    return 'img.cke-contextly-sidebar' +
      '{' +
        'background-position: center center;' +
        'background-repeat: no-repeat;' +
        'border: 1px solid #a9a9a9;' +
        'height: 100px;' +
      "}\n" +
    'img.cke-contextly-sidebar-loading,' +
    'img.cke-contextly-sidebar-broken' +
      '{' +
        'display: none;' +
      "}\n" +
    'img.cke-contextly-sidebar-left' +
      '{' +
        'width: 50%;' +
        'float: left;' +
        'margin-right: 10px;' +
      "}\n" +
    'img.cke-contextly-sidebar-right' +
      '{' +
        'width: 50%;' +
        'float: right;' +
        'margin-left: 10px;' +
      "}\n" +
    'img.cke-contextly-sidebar-wide' +
      '{' +
        'width: 95%;' +
        'display: block;' +
        'margin-left: auto;' +
        'margin-right: auto;' +
      "}\n";
  };

  var compareVersion = function (v1, v2) {
    v1 = v1.toString().split('.');
    v2 = v2.toString().split('.');

    for (var i = 0; i < (Math.max(v1.length, v2.length)); i++) {
      if (typeof v1[i] == 'undefined') {
        v1[i] = -1;
      }
      else {
        v1[i] = parseInt(v1[i]);
      }
      if (typeof v2[i] == 'undefined') {
        v2[i] = -1;
      }
      else {
        v2[i] = parseInt(v2[i]);
      }

      if (v1[i] > v2[i]) {
        return '>';
      }
      else if (v1[i] < v2[i]) {
        return '<';
      }
    }

    return '=';
  };

  var editorVersionMatches = function(spec) {
    if (typeof spec.min !== 'undefined') {
      if (compareVersion(CKEDITOR.version, spec.min) == '<') {
        return false;
      }
    }
    if (typeof spec.max !== 'undefined') {
      if (compareVersion(CKEDITOR.version, spec.max) == '>') {
        return false;
      }
    }

    return true;
  };

  /**
   * Applies settings to the sidebar.
   *
   * @param sidebar
   *   Sidebar placeholder.
   * @param [fromParser]
   *   When true, the passed element is a CKEDITOR.htmlParser.element and not
   *   the full DOM element.
   */
  var applySidebarSettings = function (sidebar, fromParser) {
    var sidebarId, classes;
    if (fromParser) {
      sidebarId = sidebar.attributes['data-contextly-sidebar-id'];
      classes = sidebar.attributes['class'];
    }
    else {
      sidebarId = sidebar.data('contextly-sidebar-id');
      classes = sidebar.getAttribute('class');
    }

    // Get class changes and apply them to the classes list.
    var changes = getSidebarClassChanges(sidebarId);
    var i;
    for (i = 0; i < changes.remove.length; i++) {
      classes = classes.replace(changes.remove[i], '');
    }
    for (i = 0; i < changes.add.length; i++) {
      classes += ' ' + changes.add[i];
    }

    // Render placeholder background.
    var backgroundImage = '';
    var dataUrl = Contextly.editor.renderSidebarPlaceholderBackground(sidebarId);
    if (dataUrl) {
      backgroundImage = "url('" + dataUrl + "')";
    }

    // Set modified classes back to sidebar.
    if (fromParser) {
      if (classes) {
        sidebar.attributes['class'] = classes;
      }
      else {
        delete sidebar.attributes['class'];
      }

      if (backgroundImage) {
        sidebar.attributes['style'] = 'background-image: ' + backgroundImage;
      }
    }
    else {
      if (classes) {
        sidebar.setAttribute('class', classes);
      }
      else {
        sidebar.removeAttribute('class');
      }

      if (backgroundImage) {
        sidebar.setStyle('background-image', backgroundImage);
      }
    }
  };

  var getSidebarClassChanges = function(sidebarId) {
    var result = {
      remove: [ /(?:^|\s+)cke-contextly-sidebar-\S+(?=\s|$)/ig ],
      add: []
    };

    if (Contextly.editor.isLoading) {
      result.add.push('cke-contextly-sidebar-loading');
    }
    else {
      var settings = Contextly.editor.getSettings().sidebars;
      if (sidebarId && settings[sidebarId] && settings[sidebarId].layout) {
        switch (settings[sidebarId].layout) {
          case 'left':
          case 'right':
          case 'wide':
            result.add.push('cke-contextly-sidebar-' + settings[sidebarId].layout);
            break;

          default:
            // Unknown alignment type. Mark as broken.
            result.add.push('cke-contextly-sidebar-broken');
            break;
        }
      }
      else {
        result.add.push('cke-contextly-sidebar-broken');
      }
    }

    return result;
  };

})(jQuery);