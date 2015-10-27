(function ($) {
  /**
   * Wysiwyg plugin button implementation for Awesome plugin.
   */
  Drupal.wysiwyg.plugins['html5ls'] = {
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
      $('#' + instanceId).html5lsWysiwyg().invoke();
    },

    /**
     * Attach plugin.
     *
     * @param content
     * @param settings
     * @param instanceId
     * @returns {*}
     */
    attach: function (content, settings, instanceId) {
      $('#' + instanceId).html5lsWysiwyg().attach();
      return content;
    },

    /**
     * Detach plugin.
     *
     * @param content
     * @param settings
     * @param instanceId
     * @returns {*}
     */
    detach: function (content, settings, instanceId) {
      $('#' + instanceId).html5lsWysiwyg().detach();
      return content;
    }
  };
})(jQuery);
