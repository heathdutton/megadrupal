/**
 * @file
 * Linkit Markdown JS library for Markdown Editor for BUEditor.
 */
(function($) {

  // Scope level variables
  var linkit_markdown_link_text = '';

  if (window.markdownEditor){
    markdownEditor.linkit = function() {
      // Register the dialog helper
      linkit_markdown_register_dialog();

      if ('undefined' === typeof Drupal.settings.linkit) {
        alert(Drupal.t('Could not find the Linkit profile.'));
      }
      else {
        // Set the editor object.
        Drupal.settings.linkit.currentInstance.editor = editor;

        // Set profile.
        // Get the name of the active text area
        var activeTextArea = BUE.active.textArea.id;
        var profile = "";
        // Check if the name of the active text area has an assigned profile
        if ("undefined" !== typeof Drupal.settings.linkit.fields[activeTextArea]) {
          profile = Drupal.settings.linkit.fields[activeTextArea].profile;
        }
        else {
          alert(Drupal.t('Could not find the Linkit profile.'));
        }

        Drupal.settings.linkit.currentInstance.profile = profile;

        // Set the name of the source field - markdown editor currently only supports bueditor.
        Drupal.settings.linkit.currentInstance.source = 'bueditor';

        // Tell Linkit what dialog helper to use.  This needs to match the name
        // used to register the dialog helper in this case bueditor.
        Drupal.settings.linkit.currentInstance.helper = 'bueditor';

        // Create the modal.
        Drupal.linkit.createModal();
      }
    }
  }

  // If the linkit object exists on load, register the dialog helper
  if ("undefined" !== typeof Drupal.linkit) {
    linkit_markdown_register_dialog();
  }

  /**
   * Register the BUEditor dialog Helper object for the Linkit object.
   */
  function linkit_markdown_register_dialog() {
    // Make sure the dialog helper is not already registered
    if("undefined" === typeof Drupal.linkit.dialogHelper.bueditor) {
      Drupal.linkit.registerDialogHelper('bueditor', {
        init : function() {},

        /**
         * Prepare the dialog after init.
         */
        afterInit : function () {
          // Add code to get selected text.
          var original_selection = BUE.active.getSelection();

          // Check if the selection is markdown with a title.
          if (original_selection.match(/\[.*\]\(.*".*"\)/g)) {

            // Get all the text between the square brackets.
            linkit_markdown_link_text = original_selection.substring(original_selection.indexOf('[') + 1,
              original_selection.indexOf(']'));

            // Get the index of the character to the immediate right of the opening round bracket.
            var start_of_path = original_selection.indexOf('(') + 1;

            // Get the index of the first space found starting from the start of the path.
            var end_of_path = original_selection.indexOf(' ', start_of_path);

            // Get the path of the link which is all text between the 'start of' and 'end of' path variables.
            var link_path = original_selection.substring(start_of_path, end_of_path);

            // Get all the text between the first double quote after the path and the last double quote.
            var link_title = original_selection.substring(original_selection.indexOf('"', end_of_path) + 1,
              original_selection.lastIndexOf('"'));

            // Populate the dialog fields
            Drupal.linkit.populateFields({
              path: link_path,
              attributes: {
                title: link_title
              }
            });
          }

          // Check if the selection is markdown without a title.
          else if (original_selection.match(/\[.*\]\(.*\)/g)) {
            // Get all of the text between the square brackets.
            linkit_markdown_link_text = original_selection.substring(original_selection.indexOf('[') + 1,
              original_selection.indexOf(']'));
            // Get all of the text between the round brackets.
            var link_path = original_selection.substring(original_selection.indexOf('(') + 1,
              original_selection.indexOf(')'));
            // Populate the dialog fields
            Drupal.linkit.populateFields({
              path: link_path,
              attributes: {
              }
            });
          }

          // Check if the selection is markup.
          else if (original_selection.match(/<a.*>.*<\/a>/g)) {
            // Get the index of the last '<' character.
            var start_of_close_tag = original_selection.lastIndexOf('<');

            // Get the index of the first '>' character.
            var end_of_open_tag = original_selection.indexOf('>');

            // Get all of the text between the end of the opening tag and the start of the close tag.
            linkit_markdown_link_text = original_selection.substring(end_of_open_tag + 1, start_of_close_tag);

            // Array to store the additional values
            var field_values = {};

            // Get the access key
            var key_start = original_selection.indexOf('accesskey=');
            if (key_start > 0) {
              field_values.accesskey = original_selection.substring(key_start + 11,
                original_selection.indexOf('"', key_start + 11));
            }

            // Get the class
            var class_start = original_selection.indexOf('class=');
            if (class_start > 0) {
              field_values.class = original_selection.substring(class_start + 7,
                original_selection.indexOf('"', class_start + 7));
            }

            // Get the target
            var target_start = original_selection.indexOf('target=');
            if (target_start > 0) {
              field_values.target = original_selection.substring(target_start + 8,
                original_selection.indexOf('"', target_start + 8));
            }

            // Get the title
            var title_start = original_selection.indexOf('title=');
            if (title_start > 0) {
              field_values.title = original_selection.substring(title_start + 7,
                original_selection.indexOf('"', title_start + 7));
            }

            // Get the rel
            var rel_start = original_selection.indexOf('rel=');
            if (rel_start > 0) {
              field_values.rel = original_selection.substring(rel_start + 5,
                original_selection.indexOf('"', rel_start + 5));
            }

            // Get the id
            var id_start = original_selection.indexOf('id=');
            if (id_start > 0) {
              field_values.id = original_selection.substring(id_start + 4,
                original_selection.indexOf('"', id_start + 4));
            }

            // Get the link path
            var href_start = original_selection.indexOf('href=');
            if (href_start > 0) {
              link_path = original_selection.substring(href_start + 6,
                original_selection.indexOf('"', href_start + 6));
            }

            Drupal.linkit.populateFields({
              path: link_path,
              attributes: field_values
            });
          }

          // else {
            // We're dealing with a link text selection (or a bad selection,
            // either way we wont change it and just use it as the link text.)
          // }
        },

        /**
         * Insert the link into the editor.
         *
         * @param {Object} link
         *   The link object.
         */
        insertLink : function(link) {
          // Get the link attributes
          var attrs = link.attributes;

          // Stores the markdown / markup of the link to be returned to the editor.
          var link_element = '';
          // Stores the text of the link to be displayed on the web page.
          var link_text = '';

          // Check if the node title was passed in
          if ("undefined" !== typeof Drupal.settings.linkit.currentInstance.linkContent
              && "" === linkit_markdown_link_text) {
            link_text = Drupal.settings.linkit.currentInstance.linkContent;
          }
          else if ("" !== BUE.active.getSelection()) {
            link_text = linkit_markdown_link_text;
          }

          // Check if the link needs to be added as html
          if ( attrs.accesskey || attrs.class || attrs.id || attrs.rel || attrs.target ) {

            // Open the anchor tags
            link_element = '<a href="' + link.path + '" ';

            // Add the access key
            if ( attrs.accesskey ) {
              link_element += 'accesskey="' + attrs.accesskey + '" ';
            }

            // Add the class
            if ( attrs.class ) {
              link_element += 'class="' + attrs.class + '" ';
            }

            // Add the id
            if ( attrs.id ) {
              link_element += 'id="' + attrs.id + '" ';
            }

            // Add the rel
            if ( attrs.rel ) {
              link_element += 'rel="' + attrs.rel + '" ';
            }

            // Add the target
            if ( attrs.target ) {
              link_element += 'target="' + attrs.target + '" ';
            }

            // Add the title
            if ( attrs.title ) {
              link_element += 'title="' + attrs.title + '" ';
            }

            // Close the anchor tag, add the link text and add the closing tag.
            link_element += '>' + link_text + '</a>';
          }

          // Check if the link can be added as markdown but needs a title.
          else if ( attrs.title ) {
            link_element = '[' + link_text + '](' + link.path + ' "' + attrs.title + '")';
          }

          // Check if the link can be added as markdown without a title.
          else {
            link_element = '[' + link_text + '](' + link.path + ')';
          }
          // Replace the selection with the link.
          window.markdownEditor.selection.replaceAll(link_element);
        }
      });
    }
  }
})(jQuery);
