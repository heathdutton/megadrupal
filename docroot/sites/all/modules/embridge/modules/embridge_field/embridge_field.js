/**
 * @file
 * Provides JavaScript additions to the managed file field type.
 *
 * This file provides progress bar support (if available), popup windows for
 * file previews, and disabling of other file fields during AJAX uploads (which
 * prevents separate file fields from accidentally uploading files).
 */

/**
 * Attach behaviors to div with to add iframe there.
 *
 */
(function ($) {
    Drupal.behaviors.entermedia_field_search = {
        attach: function() {
          jQuery('div[id*="search_form_"]').dialog({
          autoOpen: false,
          modal: true,
            height: 800,
            width: 600,
          draggable: false,
            resizable: true,
            title: 'EnterMedia Search',
            zIndex: 9999,
          });
        }
    }
    Drupal.behaviors.entermedia_field_preview_dialog = {
        attach: function() {
          jQuery('.field-type-entermediaasset .preview-dialog').dialog({
            autoOpen: false,
            modal: true,
            height: 540,
            width: 680,
            draggable: false,
            resizable: false,
            closeOnEscape: true,
            zIndex: 9999,
          });
          $('.field-type-entermediaasset .entermediaasset').click(function() {
            var parent_div = $(this).parents('div.file-widget');
            var field_id = '#' + parent_div.attr('fid') + '_preview_dialog';
            var preview = $(field_id).attr('img_src');
            $(field_id).html('<img src="' + preview + '"/>').dialog('open');
          });
        }
    }
/**
 * Attach behaviors to managed file element upload fields.
 */
Drupal.behaviors.fileValidateAutoAttach = {
  attach: function (context, settings) {
    if (settings.file && settings.file.elements) {
      $.each(settings.file.elements, function(selector) {
        var extensions = settings.file.elements[selector];
        $(selector, context).bind('change', {extensions: extensions}, Drupal.file.validateExtension);
      });
    }
  },
  detach: function (context, settings) {
    if (settings.file && settings.file.elements) {
      $.each(settings.file.elements, function(selector) {
        $(selector, context).unbind('change', Drupal.file.validateExtension);
      });
    }
  }
};

/**
 * Attach behaviors to the file upload and remove buttons.
 */
Drupal.behaviors.fileButtons = {
  attach: function (context) {
    $('input.form-submit', context).bind('mousedown', Drupal.file.disableFields);
    $('div.form-managed-file input.form-submit', context).bind('mousedown', Drupal.file.progressBar);
  },
  detach: function (context) {
    $('input.form-submit', context).unbind('mousedown', Drupal.file.disableFields);
    $('div.form-managed-file input.form-submit', context).unbind('mousedown', Drupal.file.progressBar);
  }
};

/**
 * Attach behaviors to links within managed file elements.
 */
Drupal.behaviors.filePreviewLinks = {
  attach: function (context) {
    $('div.form-managed-file .file a, .file-widget .file a', context).bind('click',Drupal.file.openInNewWindow);
  },
  detach: function (context){
    $('div.form-managed-file .file a, .file-widget .file a', context).unbind('click', Drupal.file.openInNewWindow);
  }
};

/**
 * File upload utility functions.
 */
Drupal.file = Drupal.file || {
  /**
   * Client-side file input validation of file extensions.
   */
  validateExtension: function (event) {
    // Remove any previous errors.
    $('.file-upload-js-error').remove();

    // Add client side validation for the input[type=file].
    var extensionPattern = event.data.extensions.replace(/,\s*/g, '|');
    if (extensionPattern.length > 1 && this.value.length > 0) {
      var acceptableMatch = new RegExp('\\.(' + extensionPattern + ')$', 'gi');
      if (!acceptableMatch.test(this.value)) {
        var error = Drupal.t("The selected file %filename cannot be uploaded. Only files with the following extensions are allowed: %extensions.", {
          '%filename': this.value,
          '%extensions': extensionPattern.replace(/\|/g, ', ')
        });
        $(this).parents('div.form-managed-file').prepend('<div class="messages error file-upload-js-error">' + error + '</div>');
        this.value = '';
        return false;
      }
    }
  },
  /**
   * Prevent file uploads when using buttons not intended to upload.
   */
  disableFields: function (event){
    var clickedButton = this;

    // Only disable upload fields for AJAX buttons.
    if (!$(clickedButton).hasClass('ajax-processed')) {
      return;
    }

    // Check if we're working with an "Upload" button.
    var $enabledFields = [];
    if ($(this).parents('div.form-managed-file').size() > 0) {
      $enabledFields = $(this).parents('div.form-managed-file').find('input.form-file');
    }

    // Temporarily disable upload fields other than the one we're currently
    // working with. Filter out fields that are already disabled so that they
    // do not get enabled when we re-enable these fields at the end of behavior
    // processing. Re-enable in a setTimeout set to a relatively short amount
    // of time (1 second). All the other mousedown handlers (like Drupal's AJAX
    // behaviors) are excuted before any timeout functions are called, so we
    // don't have to worry about the fields being re-enabled too soon.
    // @todo If the previous sentence is true, why not set the timeout to 0?
    var $fieldsToTemporarilyDisable = $('div.form-managed-file input.form-file').not($enabledFields).not(':disabled');
    $fieldsToTemporarilyDisable.attr('disabled', 'disabled');
    setTimeout(function (){
      $fieldsToTemporarilyDisable.attr('disabled', '');
    }, 1000);
  },
  /**
   * Add progress bar support if possible.
   */
  progressBar: function (event) {
    var clickedButton = this;
    var $progressId = $(clickedButton).parents('div.form-managed-file').find('input.file-progress');
    if ($progressId.size()) {
      var originalName = $progressId.attr('name');

      // Replace the name with the required identifier.
      $progressId.attr('name', originalName.match(/APC_UPLOAD_PROGRESS|UPLOAD_IDENTIFIER/)[0]);

      // Restore the original name after the upload begins.
      setTimeout(function () {
        $progressId.attr('name', originalName);
      }, 1000);
    }
    // Show the progress bar if the upload takes longer than half a second.
    setTimeout(function () {
      $(clickedButton).parents('div.form-managed-file').find('div.ajax-progress-bar').slideDown();
    }, 500);
  },
  /**
   * Open links to files within forms in a new window.
   */
  openInNewWindow: function (event) {
    $(this).attr('target', '_blank');
    window.open(this.href, 'filePreview', 'toolbar=0,scrollbars=1,location=1,statusbar=1,menubar=0,resizable=1,width=500,height=550');
    return false;
  }
};

})(jQuery);

/**
 * Process search result. Add asset details to the form, close dialog window and reload field.
 *
 */
function entermedia_process_search_result(assets) {
  if (assets.length > 0) {
    var index = item_id;
    var i = 0;
    entermedia_process_asset(assets, index, i);
    jQuery("#search_form_" + field_id).dialog('close');
  }
}

/**
 * Recursive method to add assets to the embridge field.
 */
function entermedia_process_asset(assets, index, i) {
  var field_id_dashes = field_id.replace(/_/g,'-');
  var button_name = field_id + '_' + lang + '_' + index + '_upload_button';
  if (i >= assets.length && jQuery('input[name="' + button_name + '"]').length > 0) {
    return;
  }
  if (jQuery('form').find('input[name="' + field_id + '[' + lang + '][' + index + '][aid]"]').length > 0) {
    jQuery('form').find('input[name="' + field_id + '[' + lang + '][' + index + '][aid]"]').val(assets[i]['aid']);
    jQuery('form').find('input[name="' + field_id + '[' + lang + '][' + index + '][from_search]"]').val('1');
  }
  else {
    jQuery('#' + field_id + '_' + lang + '_' + index + '_asset_details').append('<input type="hidden" name="' + field_id + '[' + lang + '][' + index + '][aid]" value="' + assets[i]['aid'] + '">');
    jQuery('#' + field_id + '_' + lang + '_' + index + '_asset_details').append('<input type="hidden" name="' + field_id + '[' + lang + '][' + index + '][_weight]" value="0">');
    jQuery('#' + field_id + '_' + lang + '_' + index + '_asset_details').append('<input type="hidden" name="' + field_id + '[' + lang + '][' + index + '][display]" value="1">');
  }
  // Remove the default fields.
  jQuery('#edit-' + field_id_dashes + '-' + lang + '-' + index + '-ajax-wrapper div.file-widget div.form-item').remove();
  for (k in assets[i]) {
    if(k !== 'aid') {
      jQuery('#' + field_id + '_' + lang + '_' + index + '_asset_details').remove('input[name="' + field_id + '[' + lang + '][' + index + '][' + k + ']"]');
      jQuery('#' + field_id + '_' + lang + '_' + index + '_asset_details').append('<input type="hidden" name="' + field_id + '[' + lang + '][' + index + '][' + k + ']" value="' + assets[i][k] + '">');
    }
  }
  jQuery('input[name="' + button_name + '"]').mousedown();
  index ++;
  i++;
  var next_button_name = field_id + '_' + lang + '_' + index + '_upload_button';

  // To ensure there new asset will be added one after one.
  setTimeout(function() {
    if (jQuery('input[name="' + next_button_name + '"]').length > 0) {
      entermedia_process_asset(assets, index, i);
    }
    else {
      setTimeout(function() {
        if (jQuery('input[name="' + next_button_name + '"]').length > 0) {
          entermedia_process_asset(assets, index, i);
        }
        else {
          setTimeout(function() {
            if (jQuery('input[name="' + next_button_name + '"]').length > 0) {
              entermedia_process_asset(assets, index, i);
            }
          }, 1500);
        }
      }, 500);
    }
  }, 300);
}

function entermedia_search(field_name, catalog_name, field_id_local, lang_local, item_id_local, multiselect) {
    current_field = field_name;
    field_id = field_id_local;
    lang = lang_local;
    item_id = item_id_local;
    var url = Drupal.settings.basePath + 'embridge/search/' + catalog_name + '?clear_frame=true&display_type=thumbnail';
    if (multiselect) {
      url += '&multiselect=true';
    }
    jQuery("#search_form_" + field_id_local).dialog('option', 'height', window.innerHeight * 80/100);
    jQuery("#search_form_" + field_id_local).dialog('option', 'width', window.innerWidth * 80/100);
    jQuery("#search_form_" + field_id_local).html('<iframe id="search_iframe" width="100%" height="100%" marginWidth="0" marginHeight="0" frameBorder="0" scrolling="auto" />').dialog('open');
    jQuery("#search_form_" + field_id_local).find("#search_iframe").attr("src", url);
    jQuery("#search_form_" + field_id_local).find("#search_iframe").load(function() {
        this.style.height = this.contentWindow.document.body.offsetHeight + 50 + 'px';
    });
}

function entermedia_cancel_search() {
    jQuery('div[id*="search_form_"]').dialog('close');
}
