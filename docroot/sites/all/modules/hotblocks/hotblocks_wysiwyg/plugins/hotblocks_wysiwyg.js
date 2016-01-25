/**
 *  @file
 *  Attach Media WYSIWYG behaviors.
 */

(function ($) {

  Drupal.hotblocks = Drupal.hotblocks || {};

  /**
   * Register the plugin with WYSIWYG.
   */
  Drupal.wysiwyg.plugins.hotblocks = {

    /**
     * Determine whether a DOM element belongs to this plugin.
     *
     * @param node
     *   A DOM element
     */
    isNode: function (node) {
      return $(node).is('img[data-hotblocks]');
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
    invoke: function (data, settings, instanceId) {
      var self = this;

      HotblocksModal.open(Drupal.settings.basePath + 'hotblocks/assign/wysiwyg', function() {
        // Hide the hotblocks visibility options - they don't apply to the WYSIWYG context
        $('#hbmodal-wrapper #hotblocks-visibility').hide();

        // Make the 'create new content' links open in a new window - otherwise the editor will lose the work they're editing
        $('#hb-create-tab a').attr('target', '_blank');

        // Close the dialog when 'create new content' links are clicked - this way when the user comes back to this window
        // after creating the content, they will have to open the modal again - thereby refreshing the content list and
        // seeing their new content.
        $('#hb-create-tab a').click(function(e){
          HotblocksModal.close();
        });

        $('#hotblocks-item-list-wrapper a').click(function(e){
          e.preventDefault();

          // Get the data-hotblocks html attribute
          var dataAttribute = $(this).attr('data-hotblocks');

          // Parse data attribute json into a javascript object, but use jQuery HTML entity decoding trick first.
          var data = JSON.parse(self.decodeHTMLEntities(dataAttribute));

          // Insert WYSIWYG markup
          var $wysiwygElement = $('<img />')
            // Prepending a '/' before the data attribute prevents drupal_strip_dangerous_protocols() from breaking the
            // data on the server side when it's displayed and check_markup() is called. It's a bit of a hack, but it
            // works.
            .attr('data-hotblocks', '/' + dataAttribute)
            .addClass('hotblocks-wysiwyg')
            .attr('alt', 'Embedded ' + data.friendlytype + ': ' + data.title)
            .attr('title', 'Embedded ' + data.friendlytype + ': ' + data.title);

          // Insert placeholder markup into wysiwyg.
          Drupal.wysiwyg.instances[instanceId].insert(self.outerHTML($wysiwygElement));

          // Close the dialog
          HotblocksModal.close();
        });
      });

    },

    /**
     * Attach function, called when a rich text editor loads.
     * This finds all [[tags]] and replaces them with the html
     * that needs to show in the editor.
     *
     * This finds all JSON macros and replaces them with the HTML placeholder
     * that will show in the editor.
     */
    attach: function (content, settings, instanceId) {
      return content;
    },

    /**
     * Detach function, called when a rich text editor detaches
     */
    detach: function (content, settings, instanceId) {
      return content;
    },

    /**
     * Returns a string with HTML entities decoded
     */
    decodeHTMLEntities: function(string) {
      return $('<div />').html(string).text();
    },

    /**
     * Returns a string with HTML entities encoded
     */
    encodeHTMLEntities: function(string) {
      return $('<div />').text(string).html();
    },

    ////////////// Helper functions ////////
    /**
     * Gets the HTML content of an element.
     *
     * @param element (jQuery object)
     */
    outerHTML: function (element) {
      return element[0].outerHTML || $('<div>').append(element.eq(0).clone()).html();
    }
  };


})(jQuery);
