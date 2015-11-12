/**
 * @file
 * This file contains the necessary javascript to handle the WYSIWYG map
 * token builder plugin.
 */

(function ($) {

  Drupal.wysiwyg.plugins['wysiwyg_map_tokenbuilder'] = {

    /**
     * Return whether the passed node belongs to this plugin.
     */
    isNode: function(node) {
      return ($(node).is('img.wysiwyg-map-tokenbuilder-pin'));
    },

    /**
     * Invoke is called when the toolbar button is clicked.
     */
    invoke: function(data, settings, instanceId) {
      if (this.isNode(data.node)) {
        data.mapSettings = this._getMapSettings(data.node);
      } else {
        data.mapSettings = null;
      }
      Drupal.wysiwyg.plugins.wysiwyg_map_tokenbuilder.add_form(data, settings, instanceId);
    },

    /**
     * Attach function, called when the WYSIWYG editor loads.
     * This will replace, in a non-destructive way, all the map tokens
     * with map pin images.
     */
    attach: function (content, settings, instanceId) {
      var matches = content.match(/lat=.*?\;/g);
      if (matches) {
        for (i = 0; i < matches.length; i++) {
          var inlineTag = matches[i];
          var toInsert = this._getPlaceholder(settings, inlineTag);
          content = content.replace(inlineTag, toInsert);
        }
      }
      return content;
    },

    /**
     * Detach function, called when a WYSIWYG editor detaches
     */
    detach: function (content, settings, instanceId) {
      // Replace all Map pin image placeholders with the actual tokens
      if (matches = content.match(/<img[^>]+class=([\'"])wysiwyg-map-tokenbuilder-pin[^>]*>/gi)) {
        for (var i = 0; i < matches.length; i++) {
          var imgNode = matches[i];
          token = $(imgNode).attr('title') + ';';
          content = content.replace(imgNode, token);
        }
      }
      return content;
    },

    /**
     *  Open a dialog and present the map field token builder.
     */
    add_form: function (data, settings, instanceId) {
      // Location, where to fetch the dialog.
      var aurl = Drupal.settings.basePath + 'index.php?q=wysiwyg-map/token-builder';
      dialogdiv = jQuery('<div id="insert-wysiwyg-map-token"></div>');
      dialogdiv.load(aurl + " #wysiwyg-map-tokenbuilder-form", function(){
        var dialogClose = function () {
          try {
            dialogdiv.dialog('destroy').remove();
          } catch (e) {};
        };
        // check if an existing map is being edited.
        if (data.mapSettings != null) {
          var lat = data.mapSettings.lat;
          var lon = data.mapSettings.lon;
          var zoom = data.mapSettings.zoom;
          var width = data.mapSettings.map_width;
          var height = data.mapSettings.map_height;
          var css_class = data.mapSettings.css_class;
          var map_type = ((data.mapSettings.map_type) ? data.mapSettings.map_type : 'roadmap');
          dialogdiv.contents().find('#edit-css-class').val(css_class);
          dialogdiv.contents().find('#edit-width').val(width);
          dialogdiv.contents().find('#edit-height').val(height);
          dialogdiv.contents().find('#edit-zoom').val(zoom);
          dialogdiv.contents().find('#edit-map-type').val(map_type);
        }
        btns = {};
        btns[Drupal.t('Insert map')] = function () {
          wysiwyg_map_buildToken();
          var token = dialogdiv.contents().find('#edit-token').val();
          var mapWidth = dialogdiv.contents().find('#edit-width').val();
          var mapHeight = dialogdiv.contents().find('#edit-height').val();
          if (isNaN(mapWidth) || mapWidth > 600 || mapWidth < 100) {
            alert('Your map width must be between 100 and 600 pixles.');
            return false;
          }
          if (isNaN(mapHeight) || mapHeight > 600 || mapHeight < 100) {
            alert('Your map height must be between 100 and 600 pixles.');
            return false;
          }
          var editor_id = instanceId;
          Drupal.wysiwyg.plugins.wysiwyg_map_tokenbuilder.insertIntoEditor(settings, token, editor_id);
          jQuery(this).dialog("close");
        };

        btns[Drupal.t('Cancel')] = function () {
          jQuery(this).dialog("close");
        };

        dialogdiv.dialog({
          modal: true,
          autoOpen: false,
          closeOnEscape: true,
          resizable: false,
          draggable: false,
          autoresize: true,
          namespace: 'jquery_ui_dialog_default_ns',
          dialogClass: 'jquery_ui_dialog-dialog',
          title: Drupal.t('Insert WYSIWYG Map Token'),
          buttons: btns,
          width: 550,
          close: dialogClose
        });
        dialogdiv.dialog("open");
        if (data.mapSettings == null) {
          wysiwyg_map_getMap(null, null, null, null, null);
        } else {
          wysiwyg_map_getMap(data.mapSettings.lat, data.mapSettings.lon, parseInt(data.mapSettings.zoom), data.mapSettings.map_type, data.mapSettings.css_class);
          wysiwyg_map_buildToken();
        }
      });
    },

    insertIntoEditor: function (settings, token, editor_id) {
      tag = "<img class='wysiwyg-map-tokenbuilder-pin' src='" + settings.path + "/images/wysiwyg_map.toolbar_icon.png' title='" + token + "'>";
      Drupal.wysiwyg.instances[editor_id].insert(tag);
    },

    /**
     * Helper function to return a HTML placeholder.
     */
    _getPlaceholder: function (settings, token) {
      var attrs = this._getAttrs(token);
      return "<img class='wysiwyg-map-tokenbuilder-pin' src='" + settings.path + "/images/wysiwyg_map.toolbar_icon.png' title='" + attrs + "'>";
    },

    /**
     * Helper function to extract token settings from image place holder
     */
    _getMapSettings: function (data) {
      attrs = $(data).attr('title');
      attrs = attrs.split('~');
      var mapSettings = {};
      for (var i = 0; i < attrs.length; i++) {
        var tmp = attrs[i].split('=');
        if (tmp[0] == 'map_type' || tmp[0] == 'css_class') {
          eval('mapSettings.' + tmp[0] + ' = "' + tmp[1] + '"');
        } else {
          eval('mapSettings.' + tmp[0] + ' = ' + tmp[1]);
        }
      }
      return mapSettings;
    },

    /**
     * Helper function to convert an inline tag to usable attributes.
     */
    _getAttrs: function (data) {
      var mapSettings = {};
      token = data.replace(';', '');
      var nvPairs = token.split('~');
      for (var i = 0; i < nvPairs.length; i++) {
        var tmp = nvPairs[i].split('=');
        if (tmp[0] == 'map_type' || tmp[0] == 'css_class') {
          eval('mapSettings.' + tmp[0] + ' = "' + tmp[1] + '"');
        } else {
          eval('mapSettings.'+tmp[0] + ' = ' + tmp[1]);
        }
      }
      var token = '';
      var lat = mapSettings.lat;
      var lon = mapSettings.lon;
      var zoom = mapSettings.zoom;
      var width = mapSettings.map_width;
      var height = mapSettings.map_height;
      var map_type = ((mapSettings.map_type) ? mapSettings.map_type : 'roadmap');
      var css_class = ((mapSettings.css_class) ? mapSettings.css_class : '');
      return "lat=" + lat + "~lon=" + lon + "~map_width=" + width + "~map_height=" + height + "~zoom=" + zoom + "~map_type=" + map_type + "~css_class=" + css_class;
    }

  };

})(jQuery);
