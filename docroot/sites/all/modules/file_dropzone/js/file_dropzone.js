/**
 * @file
 * Drupal Dropzone integration.
 */

(function($) {

  /**
   * Drupal Dropzone Integration.
   */
  Drupal.behaviors.drupalDropzone = {

    attach: function (context, settings) {
      Dropzone.autoDiscover = false;

      // Find the next container outside of the ajax-wrapper we are in.
      //
      // Note: This intentionally uses .parent().closest('.form-wrapper') as
      //       there can be a .form-wrapper at the same level, which however is
      //       inside the ajax-wrapper.
      var $dropzoneParentContainer = $('.drupal-dropzone', context).parent().closest('.form-wrapper');

      $dropzoneParentContainer.once('drupal-dropzone').each(function() {
        var dropzoneInstance;

        var $field = $(this);
        var $form = $(this).closest('form');
        var uploads = 0;

        var id = $(this).find('.file-dropzone-upload-button').attr('id');
        var $fid = $('#' + id).siblings('input.file-dropzone-fid');
        var options = Drupal.ajax[id].options;
        var beforeSerialize = options.beforeSerialize;

        var config = {
          url: options.url,
          paramName: 'files[dropzone_files]',
          addRemoveLinks: true,
          uploadMultiple: true,
          parallelUploads: 5,
        };

        var fileValidateSize = $fid.data('file-validate-size');
        if (fileValidateSize) {
          config.maxFilesize = fileValidateSize / (1024 * 1024);
        }

        var fileValidateExtensions = $fid.data('file-validate-extensions');
        if (fileValidateExtensions) {
          config.acceptedFiles = '.' + fileValidateExtensions.replace(/ /g,",.") + ',application/x-drupal-dropzone';
        }

        // Append empty .dz-message container to prevent dropzone from creating.
        $field.find('.fieldset-wrapper').append('<div class="dz-default dz-message"></div>');

        $(this).addClass('dropzone');
        $(this).dropzone(config);
        dropzoneInstance = this.dropzone;

        // Create a canonical data store in the field.
        $field.data('filesInfo', Drupal.behaviors.drupalDropzone.getFilesInfo($field));

        dropzoneInstance.on('successmultiple', function(files, response) {
          var results = [];
          for (var i in response) {
            if (response.hasOwnProperty(i) && response[i]['command'] == 'dropzoneFiles') {
              results = response[i]['results'];
            }
          }
          if (results.length == 0) {
            // @todo emit error on all files
            return;
          }

          for (var i in results) {
            if (results[i].status == 201) {
              dropzoneInstance.removeFile(files[i]);

              if (results[i].fid != '') {
                // Update the fid of the uploaded file element.
                var filesInfo = $field.data('filesInfo');
                for (var j=0; j < filesInfo.length; j++) {
                  if (filesInfo[j].file && filesInfo[j].file == files[i]) {
                    filesInfo[j].fid = results[i].fid;
                    filesInfo[j].file = null;
                    break;
                  }
                }

                $field.data('filesInfo', filesInfo);
              }
            }
            else {
              dropzoneInstance.emit("error", files[i], results[i].message, null);
              $(files[i].previewElement).find('[data-dz-errormessage]').html(results[i].message);
            }
          }

          var $previews = $(dropzoneInstance.previewsContainer).find('.dz-preview').remove();

          options.success(response);
          $previews.appendTo(dropzoneInstance.previewsContainer);

          if (uploads > 0) {
            $field.find('.file-dropzone-browse-element').parent().hide();
          }
        });

        dropzoneInstance.on('drop', function(e) {
          fid = e.dataTransfer.getData('fid');
          if (fid != '') {
            var file = {
              'type': 'application/x-drupal-dropzone',
              'dropzoneAction': 'attach',
              'name': 'Attach ' + e.dataTransfer.getData('name'),
              'size': 1,
              'fid': fid,
              'preview': e.dataTransfer.getData('preview'),
              'status': Dropzone.ACCEPTED
            };

            dropzoneInstance.addFile(file);
          }
        });

        dropzoneInstance.on('addedfile', function(file) {
          var fid = file.fid || '';

          if (fid != '' && file.dropzoneAction == 'remove') {
            $(file.previewElement).attr('drupal-dropzone-remove', '1');
            $(file.previewElement).attr('drupal-dropzone-fid', file.fid);
          }
          else {
            // Add the file to the filesInfo.
            var filesInfo = $field.data('filesInfo');
            var fileInfo = {
              'label': 'file',
              'fid': fid,
              'preview': null,
              'weight': null,
              'row': null,
              'previewElement': null,
              'file': file,
            };
            filesInfo.push(fileInfo);
            $field.data('filesInfo', filesInfo);
          }

          if (fid != '' && file.preview) {
            var div = $('<div>').html(file.preview);
            var img = div.find('img').get(0);
            // Defer call.
            setTimeout( function() {
              dropzoneInstance.createThumbnailFromUrl(file, img.src);
            }, 0);
          }
        });

        dropzoneInstance.on('sendingmultiple', function(files, xhr, formData) {
          if (uploads == 0) {
            $field.find('.file-dropzone-browse-element').parent().hide();
          }
          uploads++;

          // @todo
          beforeSerialize($form.get(0), options);

          var extraData = options.data;
          var data = $form.formToArray();

          for (var i=0; i < data.length; i++) {
            formData.append(data[i].name, data[i].value);
          }
          for (var property in extraData) {
            formData.append(property, extraData[property]);
          }
          xhr.url = options.url;

          for (var i=0; i < files.length; i++) {
            var fid = files[i].fid || '';
            var $preview = $(files[i].previewElement);
            $preview.find('.dz-remove').text('');

            if (fid != '') {
              var action = {
                'type': files[i].dropzoneAction,
                'fid': fid,
              };
              formData.append('dropzone_actions[]', JSON.stringify(action));
              $preview.find('.dz-size').text('');
            }
            else {
              var action = {
                'type': 'upload',
                'name': files[i].name,
              };

              formData.append('dropzone_actions[]', JSON.stringify(action));
            }
          }
        });

        dropzoneInstance.on('completemultiple', function(files, response) {
          uploads--;

          if (uploads == 0) {
            $field.find('.file-dropzone-browse-element').parent().show();
          }
        });
      });

      $dropzoneParentContainer.each(function() {
        var dropzoneInstance = this.dropzone;

        var $field = $(this);
        var $form = $(this).closest('form');

        // Setup the previews container.
        var previewsContainer = $field.find('.dz-previews').get(0);
        if (!previewsContainer) {
          previewsContainer = $('<div class="dz-previews"></div>').get(0)
          $field.find('.fieldset-wrapper:first').append(previewsContainer);

          // Update dropzone with the new container.
          dropzoneInstance.previewsContainer = previewsContainer;
        }

        // Replace the message container with a fresh copy. This goes last so
        // it consistently appears after the preview container.
        $field.find('.fieldset-wrapper > .dz-message').remove();
        $field.find('.fieldset-wrapper').append('<div class="dz-default dz-message"><span>' + Drupal.t('Drop files here to upload.') + '</span></div>');

        Drupal.behaviors.drupalDropzone.synchronizeFiles(this);

        // Add a handler for remove button.
        $('input.file-dropzone-remove-button', $field).unbind('mousedown').bind('mousedown', function(ev) {
          ev.preventDefault();

          var $row = $(this).closest('tr');
          var fileInfo = Drupal.behaviors.drupalDropzone.getRowFileInfo($row);

          var file = {
            'type': 'application/x-drupal-dropzone',
            'dropzoneAction': 'remove',
            'name': 'Remove ' + fileInfo.label,
            'size': 1,
            'fid': fileInfo.fid,
            'preview': fileInfo.preview,
            'status': Dropzone.ACCEPTED
          };
          dropzoneInstance.addFile(file);

          // Remove the file from the filesInfo.
          var filesInfo = $field.data('filesInfo');

          for (var i=0; i < filesInfo.length; i++) {
            if (filesInfo[i].fid == fileInfo.fid) {
              filesInfo.splice(i, 1);
              break;
            }
          }
          $field.data('filesInfo', filesInfo);

          Drupal.behaviors.drupalDropzone.synchronizeFiles($field.get(0));

        });

        $('input.file-dropzone-attach-button', $field).unbind('mousedown').bind('mousedown', function(ev) {
          ev.preventDefault();

          var $row = $(this).closest('tr');
          var $input = $row.find('input.file-dropzone-upload-fid');
          if ($input.length == 0) {
            $input = $(this).siblings('input.file-dropzone-upload-fid');
          }

          var name = $input.data('file-dropzone-name');
          if (name == '') {
            name = 'file';
          }
          var preview = $input.data('file-dropzone-preview');

          var file = {
            'type': 'application/x-drupal-dropzone',
            'dropzoneAction': 'attach',
            'name': 'Attach file',
            'name': 'Attach ' + name,
            'size': 1,
            'fid': $input.val(),
            'preview': preview,
            'status': Dropzone.ACCEPTED
          };

          dropzoneInstance.addFile(file);
        });

        $('input.file-dropzone-upload-button', $field).unbind('mousedown').bind('mousedown', function(ev) {
          ev.preventDefault();

          var $row = $(this).closest('tr');

          var $input = $row.find('input.file-dropzone-upload-element');
          if ($input.length == 0) {
            $input = $(this).siblings('input.file-dropzone-upload-element');
          }

          var files = $input.get(0).files;
          if (files.length > 0) {
            dropzoneInstance.addFile(files[0]);
          }
        });
      });
    },
    synchronizeFiles: function(element) {
      var dropzoneInstance = element.dropzone;

      var $field = $(element);
      var $form = $(element).closest('form');

      var previewsContainer = $field.find('.dz-previews').get(0);

      var filesInfo = $field.data('filesInfo');
      var newFilesInfo = Drupal.behaviors.drupalDropzone.getFilesInfo($field);

      var mergedFilesInfo = [];
      var $table;

      // Find new and removed items.
      for (var i=0; i < newFilesInfo.length; i++) {
        var fileInfo = newFilesInfo[i];

        var found = false;
        var oldFileInfo = false;

        for (var j=0; j < filesInfo.length; j++) {
          oldFileInfo = filesInfo[j];
          if (oldFileInfo.fid == fileInfo.fid) {
            found = true;
            break;
          }
        }

        if (found) {
          fileInfo.weight = oldFileInfo.weight;
        }
        else {
          var $row = $(fileInfo.row);
          $table = $row.closest('table');
          $row.remove();
          continue;
        }
        mergedFilesInfo.push(fileInfo);
      }
      for (var i=0; i < filesInfo.length; i++) {
        var fileInfo = filesInfo[i];
        if (fileInfo.fid == 0) {
          mergedFilesInfo.push(fileInfo);
        }
      }

      if ($table !== undefined) {
          Drupal.tableDrag[$table.attr('id')].restripeTable();
      }

      $field.data('filesInfo', mergedFilesInfo);

/*      // Add each image to the list and associate the weight input id.
      $field.find('tbody > tr').each(function() {
        var $list_item = $('<div class="dz-preview"></div>');

        // Clone the preview from the row.
        var $thumbnail_preview = $(this).find('.preview').clone();

        // Get the remove button name from the table row.
        var $remove_name = $(this).find('input.remove').attr('name');
        // Prepend a delete link with a data attribute containing this name, so
        // it can be used to delegate to the real remove AJAX form input.
        var fid = $(this).find('input.fid').attr('value');
        $thumbnail_preview.prepend('<a style="display:block;" href="#" data-remove-name="' + $remove_name  + '" data-fid="' + fid + '">delete</a>');

        $list_item.append($thumbnail_preview);

        var $weight_name = $(this).find('.tabledrag-hide select').attr('name');
        $list_item.attr('data-weight-name', $weight_name);

        $(previewsContainer).append($list_item);
      });*/
    },
    getFilesInfo: function($field) {
      // Create a canonical data store in the field.
      var filesInfo = [];

      $field.find('tbody tr').each(function() {
        var $row = $(this);
        filesInfo.push(Drupal.behaviors.drupalDropzone.getRowFileInfo($row));
      });

      return filesInfo;
    },
    getRowFileInfo: function($row) {
      var fileInfo = {
        'label': 'file',
        'fid': '',
        'preview': null,
        'weight': null,
        'row': $row.get(0),
        'previewElement': null,
        'file': null,
      };

      var $input = $row.find('input.file-dropzone-fid');

      fileInfo.fid = $input.val();

      var $preview = $row.find('.preview, .image-preview').find('img').parent();
      if ($preview.length > 0) {
        fileInfo.preview = $($preview.get(0)).html();
      }

      var $label = $input.parent().find('label');
      if ($label.length == 0) {
        $label = $input.parent().find('.file > a');
      }
      if ($label.length > 0) {
        fileInfo.label = $label.text();
      }
      fileInfo.weight = $row.find('.tabledrag-hide select').val();

      return fileInfo;
    }
  }

})(jQuery);
