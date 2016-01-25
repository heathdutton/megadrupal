/**
 * @file
 * Theme function and wysiwyg plugin for formforall module.
 */

(function ($, Drupal) {

  /**
   * Theme function for FormForAll form.
   * See theme_formforall_form() in formforall.module
   */
  Drupal.theme.prototype.formForAllForm = function(formId, tag) {
    tag = tag || 'div';
    // Add <br> after and before <div> to prevent user writing content into that div.
    return '<br><'+tag+' id="formforall-' + formId + '" class="formforall-form"></'+tag+'><br>';
  };


  /**
   * Wysiwyg plugin button implementation for FormForAll plugin.
   */
  Drupal.wysiwyg.plugins.formforall = {
    /**
     * Return whether the passed node belongs to this plugin.
     *
     * @param node
     *   The currently focused DOM element in the editor content.
     */
    isNode: function(node) {
      return ($(node).is('img.formforall-formforall'));
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
      Drupal.wysiwyg.plugins.formforall.insert_form(data, settings, instanceId);    
    },
    
    
    insert_form: function (data, settings, instanceId) {
      // Location, where to fetch the dialog.
      var aurl = Drupal.settings.basePath + 'index.php?q=ajax/formforall';

      // Create dialog
      dialogdiv = jQuery('<div id="formforall-dialog"></div>');
      dialogdiv.load(aurl + " #formforall-wysiwyg-form", function(){
        var dialogClose = function () {
          try {
            dialogdiv.dialog('destroy').remove();
          } catch (e) {};
        };

        btns = {};
        btns[Drupal.t('Insert')] = function () {
          var FFAFormId = dialogdiv.contents().find('#edit-formforall option:selected').val(),
              // FFAFormTitle = dialogdiv.contents().find('#edit-formforall option:selected')[0].innerHTML,
              editor_id = instanceId,
              FFAForm = Drupal.theme('formForAllForm', FFAFormId);

          // Only insert content if a form was selected
          if (FFAFormId) {
            Drupal.wysiwyg.plugins.formforall.insertIntoEditor(FFAForm, editor_id);
          }

          // Close dialog
          jQuery(this).dialog("close");
        };

        btns[Drupal.t('Cancel')] = function () {
          jQuery(this).dialog("close");
        };

        // Dialog settings
        dialogdiv.dialog({
          modal: true,
          autoOpen: false,
          closeOnEscape: true,
          resizable: false,
          draggable: false,
          autoresize: true,
          namespace: 'jquery_ui_dialog_default_ns',
          dialogClass: 'jquery_ui_dialog-dialog',
          title: Drupal.t('Insert FormForAll form'),
          buttons: btns,
          width: 700,
          close: dialogClose
        });
        dialogdiv.dialog("open");
      });
    },

    // Insert form into wysiwyg editor
    insertIntoEditor: function (ffaform, editor_id) {
      Drupal.wysiwyg.instances[editor_id].insert(ffaform);
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
      return content;

      // content = content.replace(/<!--formforall_wysiwyg-->/g, this._getPlaceholder(settings));
      $.each($('.formforall-formforall', $(content)), function (i, elem) {
        var FFAPlaceholder = $('<span />')
                        .attr('id', 'formforall-'+$(elem).attr('id'))
                        .addClass('formforall-placeholder')
        content = content.replace($(elem).get(0).outerHTML, FFAPlaceholder.get(0).outerHTML);
      });

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
      return content;

      $.each($('.formforall-placeholder', $(content)), function (i, elem) {
        var FFAForm = $('<div />')
                        .attr('id', 'formforall-'+$(elem).attr('id'))
                        .addClass('formforall-form')
        content = content.replace($(elem).get(0).outerHTML+'<br />', FFAForm.get(0).outerHTML);
      });
      return content;
    },

    /**
     * Helper function to return a HTML placeholder.
     *
     * The 'drupal-content' CSS class is required for HTML elements in the editor
     * content that shall not trigger any editor's native buttons (such as the
     * image button for this example placeholder markup).
     */
    _getPlaceholder: function (settings) {
      return '<img src="' + settings.path + '/images/spacer.gif" alt="&lt;--formforall-&gt;" title="&lt;--formforall--&gt;" class="formforall-formforall drupal-content" />';
    }
  };

})(jQuery, Drupal);