(function ($) {

  Drupal.easychart = Drupal.easychart || {};

  /**
   * Utility to deal with editor placeholder.
   */
  Drupal.easychart.filter = {
    /**
     * Replaces Easychart tokens with the placeholders for html editing.
     * @param content
     */
    replaceTokenWithPlaceholder: function(content) {

      var matches = content.match(/\[\[chart-nid.*?\]\]/g);

      if (matches) {
        $.each(matches, function(i) {
          var chartNid = matches[i].replace('[[chart-nid:', '').replace(']]', '');

          // Set a default placeholder
          content = content.replace(matches[i], '<img class="easychart-placeholder" src="' + Drupal.settings.basePath + Drupal.settings.easychart.module_path + '/plugins/images/placeholder.png'  + '" alt="" title="' + Drupal.t('Chart placeholder') + '" id="' + chartNid + '" />');

          // Replace with a live preview
          if (Drupal.settings.easychart.existingCharts) {
            var options = Drupal.settings.easychart.existingCharts[chartNid];

            var obj = {},
              exportUrl = 'http://export.highcharts.com/';
            obj.options = options;
            obj.type = 'image/png';
            obj.async = true;

            var newURL = '';
            $.ajax({
              type: 'post',
              url: exportUrl,
              data: obj,
              async: false,
              success: function (data) {
                // Replace the entire placeholder
                // TODO: only replace the required part, but beware of the $.each().
                content = content.replace('<img class="easychart-placeholder" src="' + Drupal.settings.basePath + Drupal.settings.easychart.module_path + '/plugins/images/placeholder.png'  + '" alt="" title="' + Drupal.t('Chart placeholder') + '" id="' + chartNid + '" />', '<img class="easychart-placeholder" src="' + exportUrl + data + '" alt="" title="' + Drupal.t('Chart placeholder') + '" id="' + chartNid + '" />');
              }
            });
          }

        })
      }
      return content;
    },

    /**
     * Replaces the placeholders for html editing with the chart tokens to store.
     * @param content
     */
    replacePlaceholderWithToken: function(content) {
      var $markup = $('<div>').append(content);

      $('.easychart-placeholder', $markup).each(function(i, element) {
        $(this).replaceWith('[[chart-nid:' + $(this).attr('id') + ']]');
        content = $markup.html();
      });

      return content;
    }
  };


  /**
   * Wysiwyg plugin button implementation for Easychart plugin.
   */
  Drupal.wysiwyg.plugins.easychart = {
    /**
     * Return whether the passed node belongs to this plugin.
     *
     * @param node
     *   The currently focused DOM element in the editor content.
     */
    isNode: function(node) {
      return ($(node).is('img.wysiwyg-easychart'));
    },

    /**
     * Execute the button.
     *
     * @param data
     *   An object containing data about the current selection:
     *   - format: 'html' when the passed data is HTML content, 'text' when the
     *     passed data is plain-text content.
     *   - node: When 'format' is 'html', the focused DOM element in the editor.
     *   - content: The textual representation of the focused/selected editor
     *     content.
     * @param settings
     *   The plugin settings, as provided in the plugin's PHP include file.
     * @param instanceId
     *   The ID of the current editor instance.
     */
    invoke: function(data, settings, instanceId) {


      //Show the node selection Dialog
      var iframeSrc = Drupal.settings.basePath + 'easychart-charts';
      var dialogMarkup = '<div id="easyChartDialog" title="' + Drupal.t('Select a chart') + '">' +
        '<iframe frameborder="no" style="width:600px; height:350px; border:0" src="'+iframeSrc+'"></iframe>' +
        '<input class="form-submit" id="easyChartFromDialog" type="submit" value="' + Drupal.t('Select') + '"><a id="buttonCancelDialog">' + Drupal.t('Cancel') + '</a>' +
        '</div>';

      var dialog = $(dialogMarkup).dialog(
        {
          autoOpen: false,
          height: 470,
          width: 650,
          modal: true,
          dialogClass: 'easychart_dialog'
        }
      );

      $(dialog).bind( "dialogclose", function(event, ui) {
        $(this).remove();
      });

      $('#easyChartFromDialog').click(function(e) {
        var node_id = window.currentActiveNid; //set or updated whenever a node is selected
        if( node_id != null && node_id != "" ) {
          dialog.editor_content = '[[chart-nid:' + node_id + ']]';
        }

        if(window.currentActiveNid && window.currentActiveNid != ""){
          var edit_content = "[[chart-nid:"+window.currentActiveNid+"]]";
          if (data.format == 'html') {
            var content = Drupal.easychart.filter.replaceTokenWithPlaceholder(edit_content);
          }
          else {
            var content = edit_content;
          }
          //write the content to the editor
          if (typeof content != 'undefined') {
            Drupal.wysiwyg.instances[instanceId].insert(content);
          }
        }

        $(dialog).dialog('close');
      });

      $('#buttonCancelDialog').click(function(e) {
        $(dialog).dialog('close');
      });

      $(dialog).dialog('open');

    },

    // Generate HTML markup.
    /**
     * Attach function, called when a rich text editor loads.
     * This finds all [[tags]] and replaces them with the html
     * that needs to show in the editor.
     *
     * This finds all charts and replaces them with the HTML placeholder
     * that will show in the editor.
     */
    attach: function (content, settings, instanceId) {
      content = Drupal.easychart.filter.replaceTokenWithPlaceholder(content);
      return content;
    },

    /**
     * Detach function, called when a rich text editor detaches
     */
    detach: function (content, settings, instanceId) {
      content = Drupal.easychart.filter.replacePlaceholderWithToken(content);
      return content;
    }

  };

})(jQuery);