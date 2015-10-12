/**
 * @file
 * Parsedown Media JS functions.
 */

(function($) {

  /**
   * Insert text at cursor position.
   *
   * @param texFieldId (string)
   *   The element id of the textfield.
   *
   * @param txtToAdd (string)
   *   The text to be added at the cursor position
   */
  function insertAtCaret(texFieldId, txtToAdd) {
    var caretPos = document.getElementById(texFieldId).selectionStart;
    var textAreaTxt = $('#' + texFieldId).val();
    $('#' + texFieldId).val(textAreaTxt.substring(0, caretPos) + txtToAdd + textAreaTxt.substring(caretPos));
  }

  var ParsedownMedia = function (fieldElement) {
    this.fieldElement = fieldElement;
    return this;
  };

  ParsedownMedia.prototype = {
    /**
     * Prompt user to select a media item with the media browser.
     * 
     * @see Drupal.media.popups.mediaBrowser()
     * @see Drupal.media.popups.mediaStyleSelector()
     */
    prompt: function () {
      // Open media pop-up.
      Drupal.media.popups.mediaBrowser($.proxy(this, 'onSelect'), {});
    },

    /**
     * On selection of a media item, display item's display configuration form.
     */
    onSelect: function (mediaFiles) {
      this.mediaFile = mediaFiles[0];
      Drupal.media.popups.mediaStyleSelector(this.mediaFile, $.proxy(this, 'insert'), {});
    },

    /**
     * When display config has been set, insert the json macro into the field.
     */
    insert: function (formattedMedia) {
      var element = Drupal.media.filter.create_element(formattedMedia.html, {
        fid: this.mediaFile.fid,
        view_mode: formattedMedia.type,
        attributes: formattedMedia.options,
        fields: formattedMedia.options
      });

      var stringifyiedMedia = Drupal.media.filter.create_macro(element);

      // Insert markup content at cursor position.
      insertAtCaret(this.fieldElement, stringifyiedMedia);
    }
  };

  Drupal.behaviors.parsedownMedia = {
    attach: function(context) {
      // Add Media link action.
      $("div.form-type-textarea a.add-media-button").click(function(e) {
        var textFormat = $('textarea.text-full', $(this).closest('div.text-format-wrapper')).attr('id');

        // Insert new media.
        var insert = new ParsedownMedia(textFormat);
        insert.prompt();

        // Cancel the default link action.
        e.preventDefault();
      });
    }
  };

})(jQuery);
