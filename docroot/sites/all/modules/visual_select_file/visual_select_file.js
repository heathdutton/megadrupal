
(function($) {

  /**
   * Global, extendable functions.
   */

  Drupal.visualSelectFile || (Drupal.visualSelectFile = {});

  // The label form the button that opens the Views grid.
  Drupal.visualSelectFile.openButtonLabel = Drupal.t('Choose file');

  // Create the button that opens the Views grid.
  Drupal.visualSelectFile.createOpenButton = function($div) {
    var $textfield = $div.find('.form-autocomplete');
    var $button = $('<input>')
      .attr('type', 'button')
      .attr('style', 'margin-left: 0.25em;')
      .addClass('visual-select-file-open-link')
      .addClass('form-submit')
      .val(Drupal.visualSelectFile.openButtonLabel);

    $button.click(function(e) {
      e.preventDefault();

      var id = $textfield.attr('id');
      var field = $textfield.data('vsf-field');
      var path = Drupal.settings.basePath + 'admin/visual_select_file?filefield&iframe&vsf_field=' + field + '#' + id;

      Drupal.visualSelectFile.openModal(path, {
        "type": 'filefield',
        "field": field,
      });
    });

    return $button;
  };

  // Position/insert the button that opens the Views grid.
  Drupal.visualSelectFile.insertOpenButton = function($div, $button) {
    $div
      // Mark this instance as processed.
      .addClass('visual-select-file-processed')
      // Insert button right after FileField Sources' "Select" button.
      .find(Drupal.settings.visualSelectFile.submitButtonSelector).after($button);
  };

  // Extend dialog options.
  Drupal.visualSelectFile.dialogOptions = function(options, context) {
    return options;
  };

  // The handler that opens the Views grid in a modal.
  Drupal.visualSelectFile.openModal = function(path, context) {
    // Prep options.
    var
      height = 0.8 * document.documentElement.clientHeight,
      width = 0.8 * document.documentElement.clientWidth,
      cancelText = Drupal.t('Cancel'),
      buttons = {};
    buttons[cancelText] = function(e) {
      var $modal = Drupal.visualSelectFile.$modal;
      $modal.dialog('close');
    };

    // Create and extend options.
    var options = {
      "dialogClass": 'vsf-modal',
      "width": width,
      "height": height,
      "draggable": false,
      "modal": true,
      "overlay": {
        "backgroundColor": '#000000',
        "opacity": 0.4,
      },
      "resizable": false,
      "buttons": buttons,
      "titlebar": false,
    };
    options = Drupal.visualSelectFile.dialogOptions(options, context);

    // Create modal.
    var $modal = $('<div><iframe width="99%" height="99%" src="' + path + '"></iframe></div>');
    $('body').append($modal);
    $modal.dialog(options);

    // Remove title bar and reset height.
    if (!options.titlebar) {
      $modal.closest(".ui-dialog").find(".ui-dialog-titlebar").remove();
      $modal.dialog('option', 'height', height);
    }

    return Drupal.visualSelectFile.$modal = $modal;
  };

  // To extract a file <td>'s meta info from Drupal.settings.
  Drupal.visualSelectFile.extractTDMetaInfo = function($vsfRow) {
    var match = $vsfRow[0].className.match(/\bvsf-fid-(\d+)\b/);
    var fid = match[1]; // If match is NULL, someone somewhere fucked up and it wasn't me!
    var file = Drupal.settings.visual_select_file.results[fid];

    return file;
  };

  // Find the FFS submit button that belongs to the FFS textfield.
  Drupal.visualSelectFile.submitFromTextfield = function($textfield) {
    return $textfield.closest('.filefield-source-reference').find('input[type="submit"]');
  };

  // After clicking an image in the Views grid.
  Drupal.visualSelectFile.onSelect = function($vsfRow, $textfield, $submit, owner) {
    var file = Drupal.visualSelectFile.extractTDMetaInfo($vsfRow);
    var fid = file[0];
    var name = file[2];

    // Set value.
    $textfield.val(name + ' [fid:' + fid + ']').removeClass('hint');

    // Submit field.
    $submit.trigger('mousedown');

    // Close modal.
    var $modal = owner.Drupal.visualSelectFile.$modal;
    $modal.dialog('close');
  };

  // After clicking an image in the Views grid.
  Drupal.visualSelectFile.onEmbed = function($vsfRow, editor, owner) {
    var file = Drupal.visualSelectFile.extractTDMetaInfo($vsfRow);

    var format = $vsfRow.closest('.view').find('#edit-vsf-formatter').val();
    if (!format) {
      $vsfRow.closest('.view').find('.form-item-vsf-formatter').addClass('error').find('select').addClass('error');
      return;
    }

    var img = Drupal.visualSelectFile.ckeditorEmbeddable(file, format, editor, owner);
    if (!img) {
      return;
    }

    // Insert img.
    editor.insertElement(img);

    // Close modal.
    var $modal = owner.Drupal.visualSelectFile.$modal;
    $modal.dialog('close');
  };

  // To create a ckeditor image element from a file.
  Drupal.visualSelectFile.ckeditorEmbeddable = function(file, format, editor, owner) {
    // 'Parse' format.
    var
      args = format.split(':'),
      format = args.shift();

    // Existing formatter. Maybe custom.
    var formatters = Drupal.visualSelectFile.ckeditorFormatters;
    if (formatters[format]) {
      return formatters[format](file, args, editor, owner);
    }

    // Default (image style) formatter.
    return formatters.style(file, format, editor, owner);
  };

  // The default ckeditor element formatters.
  Drupal.visualSelectFile.ckeditorFormatters = {
    "style": function(file, style, editor, owner) {
      var
        src = file[1].replace(/\/IMAGESTYLE\//, '/' + style + '/'),
        img = document.createElement('img'),
        element = new owner.CKEDITOR.dom.element(img);
      img.src = src;
      img.alt = file[2];

      return element;
    },
    "original": function(file, args, editor, owner) {
      var
        img = document.createElement('img'),
        element = new owner.CKEDITOR.dom.element(img);
      img.src = file[3];
      img.alt = file[2];

      return element;
    },
    "link": function(file, args, editor, owner) {
      var
        style = args[0] || 'original',
        href = style == 'original' ? file[3] : file[1].replace(/\/IMAGESTYLE\//, '/' + style + '/'),
        a = document.createElement('a'),
        element = new owner.CKEDITOR.dom.element(a);

      var selection = editor.getSelection();
      if (selection.getSelectedElement()) {
        a.appendChild(selection.getSelectedElement().$);
      }
      else if (selection.getSelectedText()) {
        a.textContent = selection.getSelectedText();
      }
      else {
        a.textContent = $.trim(file[2]);
      }
      a.href = href;

      return element;
    }
  };



  /**
   * Define global behavior.
   */

  Drupal.behaviors.visualSelectFile = {
    attach: function(context, settings) {

      /**
       * Picker page.
       */

      // Get grid TD's.
      var owner = opener || parent;
      if (location.hash && owner && owner != window) {
        var $context = $(context);
        var SELECTOR = '.view.visual-select-file';
        var $view = $context.is(SELECTOR) ? $context : $context.find(SELECTOR);
        $view.once('vsfGridClick', function() {
          $view.delegate('.vsf-row', 'click', function(e) {
            var $vsfRow = $(this);

            // CKEditor context: insert image?
            if (location.hash == '#modal') {
              var editor = owner.Drupal.visualSelectFile.editor;
              Drupal.visualSelectFile.onEmbed($vsfRow, editor, owner);
            }
            // Popup (file field) context: set value & submit.
            else {
              var $textfield = owner.jQuery(location.hash);
              var $submit = Drupal.visualSelectFile.submitFromTextfield($textfield);

              Drupal.visualSelectFile.onSelect($vsfRow, $textfield, $submit, owner);
            }
          });
        });

        if (Drupal.settings.visual_select_file.selected_file) {
          $('.vsf-fid-' + Drupal.settings.visual_select_file.selected_file).addClass('vsf-selected');
        }
      }



      /**
       * Node edit form.
       */

      // Get file fields.
      var fileFields = $('div.filefield-source-reference:not(.visual-select-file-processed)', context);
      fileFields.each(function(i, div) {
        var $div = $(div);

        // Create link to open popup.
        var $button = Drupal.visualSelectFile.createOpenButton($div);

        // Add link to submit button area.
        Drupal.visualSelectFile.insertOpenButton($div, $button);
      });
    }
  };

})(jQuery);
