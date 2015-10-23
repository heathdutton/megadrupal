
// General Insert API functions.
(function ($) {
Drupal.syntaxhighlighterinsert = {
  /**
   * Insert content into a textarea at the current cursor position.
   *
   * @param editor
   *   The DOM object of the textarea that will receive the text.
   * @param content
   *   The string to be inserted.
   */
  insertAtCursor: function(editor, content) {
    // IE support.
    if (document.selection) {
      editor.focus();
      sel = document.selection.createRange();
      sel.text = content;
    }

    // Mozilla/Firefox/Netscape 7+ support.
    else if (editor.selectionStart || editor.selectionStart == '0') {
      var startPos = editor.selectionStart;
      var endPos = editor.selectionEnd;
      editor.val(editor.val().substring(0, startPos)+ content + editor.val().substring(endPos, editor.val().length));
    }

    // Fallback, just add to the end of the content.
    else {
      editor.val(editor.val() + content);
    }
  },

  hideDescriptions: function() {
    var descriptions_hidden = true;
    jQuery('.syntaxhighlighter-insert-text-description-link, .syntaxhighlighter-insert-wysiwyg-description-link').live('click', function() {
      var link = jQuery(this);
      var id = link.attr('id');
      var form_id = id.replace('description-link', 'form-wrapper');
      if (descriptions_hidden) {
        jQuery('#' + form_id + ' .description').show();
        descriptions_hidden = false;
        link.text(Drupal.t('Hide descriptions'));
      }
      else {
        jQuery('#' + form_id + ' .description').hide();
        descriptions_hidden = true;
        link.text(Drupal.t('Show descriptions'));
      }
      return false;
    });
  }
};
})(jQuery);
