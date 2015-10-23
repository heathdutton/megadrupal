
(function($) {
  $(document).ready(function() {
    // Hide upload Widget.
    if (Drupal.settings.dragNDropUploads.hide == true) {
      $(Drupal.settings.dragNDropUploads.dropzones['default'].wrapper).hide();
    }
    // Attach standard dropzones.
    $.each(Drupal.settings.dragNDropUploads.dropzones, function() {
      Drupal.dragNDropUploads.attachDropzone(this);
    });
    // Wysiwyg module support.
    if (Drupal.wysiwyg !== undefined) {
      // Attach Wysiwyg dropzones.
      _wysiwygInit = function() {
        _this = this;
        $.each(Drupal.wysiwyg.instances, function() {
          if (Drupal.settings.dragNDropUploads.dropzones['default'].iframe !== undefined) {
            delete Drupal.settings.dragNDropUploads.dropzones['default'].iframe;
          }
          if (this.editor != 'none') {
            Drupal.dragNDropUploads.wysiwygAttachDropzones(this.field);
            // Send null data for TinyMCE & Safari/Chrome issue.
            if (Drupal.wysiwyg.instances[this.field].editor == 'tinymce') {
              Drupal.wysiwyg.instances[this.field].insert('');
            }
          }
        });
      }
      // Re-attach WYSIWYG on format change.
      $('INPUT[name="format"]').bind('change', function() {
        format = 'format' + $(this).val();
        if (Drupal.wysiwyg !== undefined) {
          $.each(Drupal.settings.wysiwyg.configs, function(editor) {
            if (this[format]) {
              Drupal.wysiwyg.instances['edit-body'].editor = editor;
              _wysiwygInit();
              return false;
            }
            return true;
          });
        }
      })
      // Re-attach WYSIWYG on Enable rich text.
      $('#wysiwyg-toggle-edit-body').bind('blur', _wysiwygInit);
      // Initialize WYSIWYG support.
      _wysiwygInit();
    }
  });

  Drupal.dragNDropUploads = {
    // Standard dropzone attachment function.
    attachDropzone: function(dropzone) {
      $(dropzone.selector || dropzone.wrapper).each(function() {
        $(this).addClass('dropzone dropzone-' + dropzone.id);
        // Safari/Chrome support.
        if ($.browser.safari) {
          $(this).bind("dragover", Drupal.dragNDropUploads.safariUpload);
        }
        // Firefox 3.6 support.
        else if ($.browser.mozilla && $.browser.version >= '1.9.2') {
          $(this).bind("dragover drop", function(e) { e.stopPropagation(); e.preventDefault(); });
          this.addEventListener("drop", Drupal.dragNDropUploads.firefoxUpload, false);
        }
      });
    },

    // WYSIWYG dropzone attachment function.
    wysiwygAttachDropzones: function(field) {
      dropzone = Drupal.settings.dragNDropUploads.dropzones['default'];
      dropzone.selector = null;
      switch (Drupal.wysiwyg.instances[field].editor) {
        // CKeditor support.
        case 'ckeditor':
          if (CKEDITOR.instances[field].container !== undefined) {
            dropzone.iframe = $(CKEDITOR.instances[field].container.$).find('iframe');
            dropzone.selector = CKEDITOR.instances[field].document.$;
          }
          break;
        // FCKeditor support.
        case 'fckeditor':
          if (typeof(FCKeditorAPI) !== 'undefined' && typeof(FCKeditorAPI.Instances[field]) !== 'undefined') {
            dropzone.iframe = $('#' + field + '___Frame');
            dropzone.selector = FCKeditorAPI.Instances[field].EditingArea.Document;
            dropzone.toolbar = $(FCKeditorAPI.Instances[field].ToolbarSet._TargetElement);
          }
          break;
        // jWYSIWYG support.
        // No insert method.
        case 'jwysiwyg':
          dropzone.iframe = $("#" + field + "IFrame");
          dropzone.selector = dropzone.iframe.get(0).contentDocument;
          break;
        // NicEdit support.
        // No insert method.
        case 'nicedit':
          dropzone.selector = nicEditors.findEditor(field).elm;
          break;
        // TinyMCE support.
        case 'tinymce':
          if (tinyMCE.editors[field] !== undefined) {
            dropzone.iframe = $('#' + field + '_ifr');
            dropzone.selector = tinyMCE.editors[field].contentDocument;
          }
          break;
        // Whizzywig support.
        //case 'whizzywig':
        //  dropzone.selector = $('#whizzy' + field).get(0).contentDocument;
        //  break;
        // WYMEditor support.
        case 'wymeditor':
          dropzone.iframe = $('#' + field + '-wrapper .wym_iframe IFRAME');
          dropzone.selector = dropzone.iframe.get(0).contentDocument;
          break;
        // YUI editor support.
        case 'yui':
          if (YAHOO.widget.EditorInfo._instances[field]._getDoc() !== false) {
            dropzone.iframe = $('#edit-body_editor');
            dropzone.selector = YAHOO.widget.EditorInfo._instances[field]._getDoc();
          }
          break;
      }
      // WYSIWYG selector unavailable, loop.
      if (dropzone.selector == null && Drupal.wysiwyg.instances[field].editor !== 'none') {
        setTimeout("Drupal.dragNDropUploads.wysiwygAttachDropzones('" + field + "')", 1000);
      }
      // Attach WYSIWYG dropzone.
      else if (Drupal.wysiwyg.instances[field].editor !== 'none') {
        Drupal.dragNDropUploads.attachDropzone(dropzone);
      }
    },

    // Safari/Chrome Upload function.
    safariUpload: function(e) {
      e.stopPropagation();
      e.preventDefault();
      id = ($(this).attr('class') !== undefined) ? $(this).attr('class').match(/dropzone-([^\s]*)/) : new Array('default', 'default');
      Drupal.settings.dragNDropUploads.dropzone = Drupal.settings.dragNDropUploads.dropzones[id[1]];
      Drupal.settings.dragNDropUploads.target = (Drupal.settings.dragNDropUploads.dropzone.target == true) ? $(this) : null;
      if ($('#dragndrop-uploads').find('input').length < 1) {
        var origFile = $(Drupal.settings.dragNDropUploads.dropzone.wrapper + ' .form-file:first');
        var dropFile = $(origFile).clone().prependTo('#dragndrop-uploads');
        // Upload and cleanup.
        $(dropFile).change(function() {
          $('#dragndrop-uploads').hide();
          $(origFile).replaceWith(dropFile);
          Drupal.settings.dragNDropUploads.trigger = true;
          $(Drupal.settings.dragNDropUploads.dropzone.wrapper + ' .form-submit[value="' + Drupal.settings.dragNDropUploads.dropzone.submit + '"]:first').trigger('mousedown');
          // Drupal.dragNDropUploads.uploadProgress(null);
        });
        // Cleanup on no upload.
        $(dropFile).mousemove(function() {
          setTimeout(function() {
            $('#dragndrop-uploads').hide();
            $(dropFile).remove();
          }, '100');
        });
      }
      // Move dropzone underneath cursor.
      if (Drupal.settings.dragNDropUploads.offset == null) {
        $('#dragndrop-uploads').show();
        Drupal.settings.dragNDropUploads.offset = $('#dragndrop-uploads').offset();
      }
      $('#dragndrop-uploads').show().css({
        top: (
          e.pageY - Drupal.settings.dragNDropUploads.offset.top - 50 + (Drupal.settings.dragNDropUploads.dropzone.iframe !== undefined
            ? Drupal.settings.dragNDropUploads.dropzone.iframe.offset().top + (Drupal.settings.dragNDropUploads.dropzone.toolbar !== undefined
              ? Drupal.settings.dragNDropUploads.dropzone.toolbar.height() : 0
            ) : 0
          )) + "px",
        left: (
          e.pageX - Drupal.settings.dragNDropUploads.offset.left - 50 + (Drupal.settings.dragNDropUploads.dropzone.iframe !== undefined
            ? Drupal.settings.dragNDropUploads.dropzone.iframe.offset().left
            : 0
          )) + "px"
      });
    },

    // Firefox 3.6 Upload function.
    firefoxUpload: function(e) {
      id = ($(this).attr('class') !== undefined) ? $(this).attr('class').match(/dropzone-([^\s]*)/) : new Array('default', 'default');
      Drupal.settings.dragNDropUploads.dropzone = Drupal.settings.dragNDropUploads.dropzones[id[1]];
      if ($(Drupal.settings.dragNDropUploads.dropzone.wrapper + ' .form-file').length > 0) {
        Drupal.settings.dragNDropUploads.target = (Drupal.settings.dragNDropUploads.dropzone.target == true) ? $(this) : null;
        Drupal.settings.dragNDropUploads.trigger = true;
        ajaxField = Drupal.ajax[$(Drupal.settings.dragNDropUploads.dropzone.wrapper + ' .form-submit[value="' + Drupal.settings.dragNDropUploads.dropzone.submit + '"]:first').attr('id')];
        if (e.dataTransfer.files !== null) {
          var file = e.dataTransfer.files[0];
          var fileReader = FileReader();
          fileReader.addEventListener("loadend", function(e) {
            // Build RFC2388 string.
            var boundary = '------multipartformboundary' + (new Date).getTime();
            var data = 'Content-Type: multipart/form-data; boundary=' + boundary + '\r\n\r\n';
            data += '--' + boundary;
            $(':input:not(:submit), ' + ajaxField.selector).each(function() {
              data += '\r\nContent-Disposition: form-data; name="' + $(this).attr('name') + '"';
              if ($(this).attr('name') == $(Drupal.settings.dragNDropUploads.dropzone.wrapper + ' .form-file:first').attr('name')) {
                Drupal.settings.dragNDropUploads.file = file.name;
                data += '; filename="' + file.name + '"\r\n';
                data += 'Content-Type: ' + file.type + '\r\n\r\n';
                data += e.target.result + '\r\n'; // Append binary data.
              }
              else {
                data += '\r\n\r\n' + ($(this).attr('type') == 'checkbox' ? ($(this).attr('checked') == true ? 1 : 0) : $(this).val()) + '\r\n';
              }
              data += '--' + boundary; // Write boundary.
            });
            data += '--'; // Mark end of the request.
            // Send XMLHttpRequest.
            var xhr = new XMLHttpRequest();
            xhr.upload.onprogress = function(e) { Drupal.dragNDropUploads.uploadProgress(e) }
            xhr.onreadystatechange = function() {
              if (xhr.readyState == 4 && xhr.status == 200) {
                response = $.parseJSON(xhr.responseText);
                Drupal.ajax[ajaxField.selector.substr(1)].success(response, 'success');
              }
            }
            xhr.open('POST', ajaxField.url, true);
            xhr.setRequestHeader('content-type', 'multipart/form-data; boundary=' + boundary);
            xhr.sendAsBinary(data);
          }, false);
          fileReader.readAsBinaryString(file);
        }
      }
    },

    // Upload progress.
    uploadProgress: function(e) {
      // if (Drupal.settings.dragNDropUploads.target !== null) {
      //   if (!Drupal.settings.dragNDropUploads.progressBar) {
      //     // Initialize/Reset progress bar.
      //     Drupal.settings.dragNDropUploads.progressBar = $('#progress').clone().insertAfter('#progress').show();
      //     $(Drupal.settings.dragNDropUploads.progressBar).find('.percentage').empty();
      //     // Position progress bar in the center of dropzone target.
      //     target = (Drupal.settings.dragNDropUploads.target.get(0).body == undefined) ? $(Drupal.settings.dragNDropUploads.target) : Drupal.settings.dragNDropUploads.dropzone.iframe;
      //     $(Drupal.settings.dragNDropUploads.progressBar)
      //       .css('width', target.width() / 2 + 'px')
      //       .css({
      //         'top': target.offset().top + ((Drupal.settings.dragNDropUploads.target.get(0).body !== undefined && Drupal.settings.dragNDropUploads.dropzone.toolbar !== undefined)
      //           ? Drupal.settings.dragNDropUploads.dropzone.toolbar.height() : 0) - $(Drupal.settings.dragNDropUploads.progressBar).offset().top + (target.height() / 2) - ($(Drupal.settings.dragNDropUploads.progressBar).height() / 2) + 'px',
      //         'left': target.offset().left - $(Drupal.settings.dragNDropUploads.progressBar).offset().left + (target.width() / 2) - ($(Drupal.settings.dragNDropUploads.progressBar).width() / 2) + 'px'
      //       });
      //   }
      //   // Update progress bar.
      //   if (e !== null) {
      //     percentage = Math.round((e.loaded / e.total) * 100);
      //     $(Drupal.settings.dragNDropUploads.progressBar)
      //       .find('.filled').css('width', percentage + '%').end()
      //       .find('.percentage').html(percentage + '%');
      //   }
      // }
    }
  }

  // Post upload behaviour.
  Drupal.behaviors.dragNDropUploads = {
    attach: function(context) {
      if (Drupal.settings.dragNDropUploads.trigger) {
        // Remove progress bar.
        if (Drupal.settings.dragNDropUploads.progressBar) {
          $(Drupal.settings.dragNDropUploads.progressBar).remove();
          Drupal.settings.dragNDropUploads.progressBar = null;
        }
        // Return HTML reference to new upload.
        var output = $(context).find(Drupal.settings.dragNDropUploads.dropzone.result).val() || $(context).find(Drupal.settings.dragNDropUploads.dropzone.result).html();
        if (output !== '' && output !== null && Drupal.settings.dragNDropUploads.target !== null) {
          if ($(Drupal.settings.dragNDropUploads.target).get(0).tagName == 'TEXTAREA') {
            $(Drupal.settings.dragNDropUploads.target).val($(Drupal.settings.dragNDropUploads.target).val() + output);
          }
          // Wysiwyg API support.
          else if ($.isFunction(Drupal.wysiwyg.instances[Drupal.wysiwyg.activeId].insert)) {
            // Send null data for FCKeditor/CKeditor & Safari/Chrome issue.
            if ($.browser.safari && (Drupal.wysiwyg.instances[Drupal.wysiwyg.activeId].editor == 'fckeditor' || Drupal.wysiwyg.instances[Drupal.wysiwyg.activeId].editor == 'ckeditor')) {
              Drupal.wysiwyg.instances[Drupal.wysiwyg.activeId].insert('');
            }
            Drupal.wysiwyg.instances[Drupal.wysiwyg.activeId].insert(output);
            // Cleanup references to local file.
            if ($.browser.mozilla) {
              $(Drupal.settings.dragNDropUploads.dropzone.selector.body).html(
                $(Drupal.settings.dragNDropUploads.dropzone.selector.body).html()
                  .replace(/<img[^>]+file:\/\/\/.*?>/g, '')
                  .replace(/<a[^>]+file:\/\/\/.*?<\/a>/g, '')
              );
            }
          }
        }
        // Add another item.
        if ($(Drupal.settings.dragNDropUploads.dropzone.wrapper + ' .form-file:first').length == 0) {
          if ($(Drupal.settings.dragNDropUploads.dropzone.wrapper + ' .form-submit:last').val() == 'Add another item') {
            $(Drupal.settings.dragNDropUploads.dropzone.wrapper + ' .form-submit:last').trigger('mousedown');
          }
        }
        // Reset variables.
        Drupal.settings.dragNDropUploads.target = null;
        Drupal.settings.dragNDropUploads.trigger = false;
      }
    },
  };
})(jQuery);
