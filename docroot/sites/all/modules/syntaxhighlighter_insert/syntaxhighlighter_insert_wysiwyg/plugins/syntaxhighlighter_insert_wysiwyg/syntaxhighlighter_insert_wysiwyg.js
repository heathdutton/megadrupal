// $Id
(function ($) {
/**
 * Wysiwyg plugin button implementation for syntaxhighlighter_insert plugin.
 */
Drupal.wysiwyg.plugins.syntaxhighlighter_insert_wysiwyg = {
  /**
   * Return whether the passed node belongs to this plugin.
   *
   * @param node
   *   The currently focused DOM element in the editor content.
   */
  isNode: function(node) {
    return ($(node).is('img.syntaxhighlighter_insert_wysiwyg-syntaxhighlighter_insert_wysiwyg'));
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
    Drupal.wysiwyg.plugins.syntaxhighlighter_insert_wysiwyg.insert_form(data, settings, instanceId);
  },


  insert_form: function (data, settings, instanceId) {
    Drupal.syntaxhighlighterinsert.hideDescriptions();
    var form_id = Drupal.settings.syntaxhighlighter_insert_wysiwyg.current_form;

    // Location, where to fetch the dialog.
    var aurl = Drupal.settings.basePath + 'index.php?q=syntaxhighlighter_insert_wysiwyg/insert/' + form_id;
    var dialogdiv = jQuery('<div id="syntaxhighlighter-insert-dialog"></div>');
    dialogdiv.load(aurl + " #syntaxhighlighter-insert-wysiwyg-form", function(){
      var dialogClose = function () {
        try {
          dialogdiv.dialog('destroy').remove();
        } catch (e) {};
      };
      var btns = {};
      btns[Drupal.t('Insert syntaxhighlighter tag')] = function () {

        var editor_id = instanceId;
        var field_id = 'syntaxhighlighter-insert-wysiwyg-*field*-wysiwyg';

        var title = dialogdiv.contents().find('#' + field_id.replace('*field*', 'title')).val()
        var brush = dialogdiv.contents().find('#' + field_id.replace('*field*', 'brush')).val();
        var tag = dialogdiv.contents().find('#' + field_id.replace('*field*', 'tag')).val();
        var autolinks = dialogdiv.contents().find('#' + field_id.replace('*field*', 'auto-links')).is(':checked');
        var classname = dialogdiv.contents().find('#' + field_id.replace('*field*', 'class-name')).val();
        var collapse = dialogdiv.contents().find('#' + field_id.replace('*field*', 'collapse')).is(':checked');
        var firstline = dialogdiv.contents().find('#' + field_id.replace('*field*', 'first-line')).val();
        var highlight = dialogdiv.contents().find('#' + field_id.replace('*field*', 'highlight')).val();
        var htmlscript = dialogdiv.contents().find('#' + field_id.replace('*field*', 'html-script')).is(':checked');
        var smarttabs = dialogdiv.contents().find('#' + field_id.replace('*field*', 'smart-tabs')).is(':checked');
        var tabsize = dialogdiv.contents().find('#' + field_id.replace('*field*', 'tab-size')).val();
        var toolbar = dialogdiv.contents().find('#' + field_id.replace('*field*', 'toolbar')).is(':checked');
        var wrapper = dialogdiv.contents().find('#' + field_id.replace('*field*', 'form-wrapper'));
        var content = '<' + tag + ' class="';
        content += 'brush: ' + brush + '; ';
        content += 'auto-links: ' + new Boolean(autolinks).toString() + '; ';
        if (classname.length) content += "class-name: '" + classname + "'; ";
        content += 'collapse: ' + new Boolean(collapse).toString() + '; ';
        if (firstline.length) content += 'first-line: ' + firstline + '; ';
        if (highlight.length) content += 'highlight: ' + highlight + '; ';
        content += 'html-script: ' + new Boolean(htmlscript).toString() + '; ';
        content += 'smart-tabs: ' + new Boolean(smarttabs).toString() + '; ';
        if (tabsize.length) content += 'tab-size: ' + tabsize + '; ';
        content += 'toolbar: ' + new Boolean(toolbar).toString() + '; ';
        content += 'codetag" ';
        if (title.length) content += 'title="' + title + '" ';
        var message = Drupal.t('Type your code in the box. To create a new line within the box use SHIFT + ENTER.');
        content += ' id="shinsert-current-tag"> ' + message + ' </' + tag + '>';
        Drupal.wysiwyg.plugins.syntaxhighlighter_insert_wysiwyg.insertIntoEditor(content, editor_id);
        jQuery(this).dialog("close");
        Drupal.wysiwyg.plugins.syntaxhighlighter_insert_wysiwyg.selectTagContents(editor_id);


      };

      btns[Drupal.t('Cancel')] = function () {
        jQuery(this).dialog("close");
      };

      dialogdiv.dialog({
        modal: true,
        autoOpen: false,
        closeOnEscape: true,
        resizable: true,
        draggable: true,
        autoresize: true,
        namespace: 'jquery_ui_dialog_default_ns',
        dialogClass: 'jquery_ui_dialog-dialog',
        title: Drupal.t('Insert'),
        buttons: btns,
        width: '70%',
        close: dialogClose
      });
      dialogdiv.dialog("open");
      $('#syntaxhighlighter-insert-wysiwyg-form .description').hide();
    });
  },

  insertIntoEditor: function (syntaxhighlighter, editor_id) {
    Drupal.wysiwyg.instances[editor_id].insert(syntaxhighlighter);
  },
  selectTagContents: function(editor_id) {
    if (typeof Drupal.wysiwyg == 'undefined') {
      // nothing to do here.
      return;
    }
    var tag, rng
    switch (Drupal.wysiwyg.instances[editor_id].editor) {
      case 'tinymce':
        rng = tinyMCE.activeEditor.dom.createRng();
        tag = tinyMCE.activeEditor.dom.select('#shinsert-current-tag')[0];
        rng.selectNodeContents(tinyMCE.activeEditor.selection.select(tag));
        tinyMCE.activeEditor.selection.setRng(rng);
        // append an empty tag so you can get out of the syntaxhighlighter tags (editor specific)
        tinyMCE.activeEditor.dom.setOuterHTML('shinsert-current-tag', $(tag).removeAttr('id').clone().wrapAll("<div />").parent().html() + '<p></p>');
        break;
      case 'ckeditor':
        rng = new CKEDITOR.dom.range(CKEDITOR.currentInstance.document);
        tag = CKEDITOR.currentInstance.document.getById('shinsert-current-tag');
        rng.selectNodeContents(tag);
        CKEDITOR.currentInstance.getSelection().selectRanges([rng]);
        tag.removeAttribute('id');
        break;
    }
  },

  /**
   * Prepare all plain-text contents of this plugin with HTML representations.
   *
   * Optional; only required for "inline macro tag-processing" plugins.
   *
   * @param content
   *   The plain-text contents of a textarea.
   * @param settings
   *   The plugin settings, as provided in the plugin's PHP include file.
   * @param instanceId
   *   The ID of the current editor instance.
   */
  attach: function(content, settings, instanceId) {
    content = content.replace(/<!--syntaxhighlighter_insert_wysiwyg-->/g, this._getPlaceholder(settings));
    return content;
  },

  /**
   * Process all HTML placeholders of this plugin with plain-text contents.
   *
   * Optional; only required for "inline macro tag-processing" plugins.
   *
   * @param content
   *   The HTML content string of the editor.
   * @param settings
   *   The plugin settings, as provided in the plugin's PHP include file.
   * @param instanceId
   *   The ID of the current editor instance.
   */
  detach: function(content, settings, instanceId) {
    var $content = $('<div>' + content + '</div>');
    $.each($('img.syntaxhighlighter_insert_wysiwyg-syntaxhighlighter_insert_wysiwyg', $content), function (i, elem) {
      //...
      });
    return $content.html();
  },

  /**
   * Helper function to return a HTML placeholder.
   *
   * The 'drupal-content' CSS class is required for HTML elements in the editor
   * content that shall not trigger any editor's native buttons (such as the
   * image button for this example placeholder markup).
   */
  _getPlaceholder: function (settings) {
    return '<img src="' + settings.path + '/images/spacer.gif" alt="&lt;--syntaxhighlighter_insert_wysiwyg-&gt;" title="&lt;--syntaxhighlighter_insert_wysiwyg--&gt;" class="syntaxhighlighter_insert_wysiwyg-syntaxhighlighter_insert_wysiwyg drupal-content" />';
  }
};
})(jQuery);