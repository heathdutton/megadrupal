(function ($) {
  "use strict";

  // Declared here because it is used in Drupal.wysiwyg.plugins.embed_external
  // and Drupal.embed_external.wysiwygAppendMarkup.
  var embed_external_edit_element;
  var embed_external_edit_element_editor;
  Drupal.wysiwyg = Drupal.wysiwyg || {};
  Drupal.wysiwyg.plugins = Drupal.wysiwyg.plugins || {};
  Drupal.wysiwyg.plugins.embed_external = {

    /**
     * Return whether the passed node belongs to this plugin.
     */
    isNode: function(node) {
      return ($(node).is('iframe') && $(node).hasClass('embed-external'));
    },

    /**
     * Execute the button.
     */
    invoke: function(data, settings, instanceId) {
      // We don't need to do anything here because the content is inserted by an
      // AJAX command.
      // @see Drupal.embed_external.wysiwygAppendMarkup
    },

    attach: function(content, settings, instanceId) {
      // Grab the current editor and format settings.
      var currentFormat = Drupal.wysiwyg.instances[instanceId].format;
      var currentEditor = Drupal.wysiwyg.instances[instanceId].editor;
      var currentPluginset = Drupal.settings.wysiwyg.configs[currentEditor][currentFormat].embed_external_pluginset;
      var $embed_external_plugin_button = $('.cke_button__embed_external');
      var embed_external_ajax_form_href = '/embed_external/' + currentPluginset + '/' + instanceId + '/ajax/form?editor_id=' + instanceId;

      // @TODO: Does this need to work for other editors? Class is ckeditor specific.
      $embed_external_plugin_button.attr('href', embed_external_ajax_form_href);
      $embed_external_plugin_button.addClass('ctools-use-modal');
      // Manually re-run ctools modal attach function to pick up the new link.
      Drupal.behaviors.ZZCToolsModal.attach();

      // Find paths for embed_external_preview.js and iframeResizer.min.js...
      var resizerScript = Drupal.settings.wysiwyg.configs[currentEditor][currentFormat].iframeresizer_location;
      var previewScript = Drupal.settings.wysiwyg.configs[currentEditor][currentFormat].previewscript_location;
      // ...and load them into CKeditor.
      // @TODO: This is definitely CKeditor specific. Generalize it eventually.
      CKEDITOR.on('instanceReady', function(ev) {
        // Add the resizer script...
        var $script = document.createElement('script');
        var $editor_instance = CKEDITOR.instances[ev.editor.name];
        $script.src = resizerScript;
        $editor_instance.document.getHead().$.appendChild($script);
        // ...and the preview script.
        $script = document.createElement('script');
        $editor_instance = CKEDITOR.instances[ev.editor.name];
        $script.src = previewScript;
        $editor_instance.document.getHead().$.appendChild($script);

        $editor_instance.addCommand('embed_external_edit', {
          state: CKEDITOR.TRISTATE_DISABLED,
          exec: function (editor) {
            // Fire off a modal much like the External Embed plugin button does,
            // but while POSTing data for the already existing element.
            var $anonymous_link = $('<a></a>').attr('href', embed_external_ajax_form_href).addClass('ctools-use-modal');
            // Bind the CTools Ajax link handler.
            $anonymous_link.click(Drupal.CTools.Modal.clickAjaxLink);
            // Create a Drupal ajax object - borrowed from Drupal.behaviors.ZZCToolsModal.attach().
            var element_settings = {};
            if ($anonymous_link.attr('href')) {
              element_settings.url = $anonymous_link.attr('href');
              element_settings.event = 'click';
              element_settings.progress = { type: 'throbber' };
            }
            var base = $anonymous_link.attr('href');
            Drupal.ajax[base] = new Drupal.ajax(base, $anonymous_link, element_settings);
            // Include the edit element's data in the Ajax POST.
            Drupal.ajax[base].options.data.embed_external_edit = {};
            // $(element.$) is needed to retrieve the jQuery object for a CKEditor DOM element.
            var edit_element_data = $(embed_external_edit_element.$).children('iframe.embed-external').data();
            for (var p in edit_element_data) {
              Drupal.ajax[base].options.data.embed_external_edit[p] = edit_element_data[p];
            }
            // Click the constructed link to fire off the modal.
            $anonymous_link.click();
          }
        });
        $editor_instance.addCommand('embedExternalDelete', {
          state: CKEDITOR.TRISTATE_DISABLED,
          exec: function (editor) {
            var parent = embed_external_edit_element.getParent();
            parent.remove();
          }
        });

        if ($editor_instance.contextMenu) {
          $editor_instance.addMenuGroup('embedExternalGroup');
          $editor_instance.addMenuItem('embedExternalEdit', {
              label: 'Edit embedded content',
              icon: Drupal.settings.wysiwyg.plugins.drupal.embed_external.path + '/images/glyphicons-31-pencil.png',
              command: 'embed_external_edit',
              group: 'embedExternalGroup'
          });
          $editor_instance.addMenuItem('embedExternalDelete', {
            label: 'Delete embedded content',
            icon: Drupal.settings.wysiwyg.plugins.drupal.embed_external.path + '/images/glyphicons-208-remove-2.png',
            command: 'embedExternalDelete',
            group: 'embedExternalGroup'
          });
          $editor_instance.contextMenu.addListener(function(element, selection, path) {
            // The getChild() could need some tweaking if there are ever more
            // children inside the <div> wrapper.
            var embedIframe = element.getChild(0);

            // Big hack due to bug in addListener() passing the wrong element
            // depending on which OS you are using. Works as expected in Linux
            // & Windows but not on Mac on any browser.
            // @see http://dev.ckeditor.com/ticket/11842 (bug report)
            // @see http://www.javascripter.net/faq/operatin.htm (workaround)
            if (navigator.appVersion.indexOf("Mac")!=-1) {
              embedIframe = element;
            }

            if (embedIframe && embedIframe.hasClass('embed-external-wrapper')) {
              embed_external_edit_element = embedIframe ;
              embed_external_edit_element_editor = CKEDITOR.instances[ev.editor.name];

              return {
                embedExternalEdit: CKEDITOR.TRISTATE_OFF,
                embedExternalDelete: CKEDITOR.TRISTATE_OFF
              };
            }
          });
        }
      });

      return content;
    },

    detach: function(content, settings, instanceId) {
      return content;
    }
  };

  /**
   * Simple AJAX command to insert markup into a specified WYSIWYG editor.
   *
   * @param ajax
   * @param response
   * @param status
   */
  Drupal.embed_external = Drupal.embed_external || {};

  // Clear the embed_external_edit_element variables after moving the CKEditor
  // cursor to the end of the element.
  Drupal.embed_external.clearEditElement = function () {
    if (embed_external_edit_element) {
      // Move the cursor to the end of the element being edited.
      var range = embed_external_edit_element_editor.createRange();
      range.moveToClosestEditablePosition(embed_external_edit_element, true);
      range.select();
      // Now clear it.
      embed_external_edit_element = null;
    }
  };

  Drupal.embed_external.wysiwygAppendMarkup = function(ajax, response, status) {
    var instanceId = response.instanceId;
    var content = response.content;

    if (embed_external_edit_element) {
      embed_external_edit_element.remove();
      embed_external_edit_element = null;
    }

    Drupal.wysiwyg.instances[instanceId].insert(content, true);

    // Upon insertion of the embed iframe we need to resize it. Note that
    // the below code is only necessary because on initial editing, the
    // code below doesn't run in embed_external_preview.js.
    var ckeFrame = document.querySelector('.cke_wysiwyg_frame');
    ckeFrame.contentWindow.iFrameResize({
      checkOrigin: false,
      heightCalculationMethod: 'bodyScroll',
      sizeHeight: true,
      sizeWidth: true
    });
  };

  Drupal.embed_external.wysiwygCancel = function(ajax, response, status) {
    Drupal.embed_external.clearEditElement();
  };

  // Add the command to Drupal's command collection.
  Drupal.ajax.prototype.commands.wysiwygAppendMarkup = Drupal.embed_external.wysiwygAppendMarkup;
  Drupal.ajax.prototype.commands.wysiwygCancel = Drupal.embed_external.wysiwygCancel;

  // Clear the edited element when a modal is closed.
  $(document).bind('CToolsDetachBehaviors', function ($modalContent) {
    Drupal.embed_external.clearEditElement();
  });

})(jQuery);
