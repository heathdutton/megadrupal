
(function ($) {



  // Browers that don't know File, won't have event.dataTransfer.files.
  if (!window.File || !File.prototype) {
    return;
  }



Drupal.behaviors.dragDropFile = {
  attach: function(context, settings) {

    // Override Drupal.file.progressBar to NOT wait 0.5 sec.
    Drupal.file && (Drupal.file.progressBar = function(e) {
      $(this).closest('div.form-managed-file').find('div.ajax-progress-bar').show();
    });

    settings.ajax && $.each(settings.ajax, function(base, cfg) {
      if (cfg.element && cfg.element.type === 'submit' && cfg.element.value == Drupal.t('Upload')) {
        cfg.overriddenByDragDropFile = true;
        var
          ajax = Drupal.ajax[base],
          options = ajax.options,
          $submit = $('#' + base);
        options.ajax = ajax;

        if (options.overriddenByDragDropFile || !$submit.length || !$submit.hasClass('i-am-dragdropfile')) return;

        options.overriddenByDragDropFile = true;
        var oldBeforeSend = options.beforeSend;
        options.beforeSend = function(xhr, options) {
          // Do Drupal magic here.
          oldBeforeSend.call(this, xhr, options);

          // Do my magic here.
          if (options.files) {
            $.each(options.files, function(i, file) {
              options.data.append(options.inputName, file);
            });
          }
        };

        var $wrapper = $('#' + cfg.wrapper);
        $wrapper.data('ajax-base', base);
        $wrapper.attr('data-ddf-title', $submit.attr('data-ddf-title'));
        $wrapper.bind({
          dragover: function(e) {
            e.preventDefault();
            var $this = $(this);

            $this.addClass('over');
          }, // dragover

          dragleave: function(e) {
            e.preventDefault();
            var $this = $(this);

            $this.removeClass('over');
          }, // dragleave

          drop: function(e) {
            var $this = $(this);
            $this.removeClass('over');
            var $wrapper = $this.parent();

            if ('INPUT' == e.target.nodeName) {
              console.log("DON'T handle drop event");
              return;
            }

            e.preventDefault();

            // Once per drop
            var
              base = $this.data('ajax-base'),
              $input = $wrapper.find('input.form-file').first(),
              input = $input[0],
              files = Array.prototype.slice.call(e.originalEvent.dataTransfer.files),
              $form = $input.closest('form'),
              inputName = $input.attr('name');

            // Append only as many files as are allowed.
            var max = !input.multiple ? 1 : parseFloat($input.attr('max'));
            if (max) {
              files = files.slice(0, max);
            }

            // Validate files
            if ($input.data('validate') == 'settings' && Drupal.settings.file && Drupal.settings.file.elements && Drupal.settings.file.elements['#' + input.id]) {
              var
                fakeInput = document.createElement('input'),
                fails = 0;
              for (var i=0, L=files.length; i<L; i++) {
                var
                  fileName = files[i].name,
                  extensions = Drupal.settings.file.elements['#' + input.id];
                fakeInput.value = fileName;
                var valid = Drupal.file.validateExtension.call(fakeInput, {data: {extensions: extensions}}) !== false;
                if (!valid) {
                  fails++;
                  delete files[i];
                }
              }

              if (fails) {
                $('.file-upload-js-error').remove();

                var error = Drupal.t('@fails of @total files invalid.', {
                  "@fails": fails,
                  "@total": L
                });
                $input.closest('div.form-managed-file').prepend('<div class="messages error file-upload-js-error">' + error + '</div>');
              }

              files = files.filter(function(el) { return !!el; });
              if (!files.length) {
                return;
              }
            }

            var numFiles = files.length;
            var progress;

            function startUpload() {
              // 'Unset' all file inputs.
              $form.find('input[type="file"]').val('').each(function() {
                var $this = $(this);
                $this.data('actualInputName', this.name);
                $this.removeAttr('name');
              });

              // Start uploading
              sendNextFile();
            }

            function sendNextFile() {
              // Find next file
              var file = files.pop();
              if (!file) {
                // All done
                var done = numFiles;
                progress.object.setProgress(100, Drupal.formatString(progress.message, {
                  "!done": done,
                  "!total": numFiles,
                }));
                return allFilesDone();
              }

              // For every file
              var
                settings = Drupal.settings.ajax[base],
                ajax = Drupal.ajax[base],
                options = ajax.options,
                $input = $wrapper.find('input.form-file').first(),
                input = $input[0],
                $submit = $input.closest('.form-item').find(Drupal.settings.dragDropFile.submitButtonSelector).first();

              setTimeout(function() {
                progress = ajax.progress;

                var done = numFiles - files.length - 1;
                var part = done / numFiles;
                progress.object.setProgress(Math.round(part * 100), Drupal.formatString(progress.message, {
                  "!done": done,
                  "!total": numFiles,
                }));
              });

              inputName = $input.attr('name') || inputName;

              // Override success callback to add something at the very end.
              var oldSuccess = options.success;
              options.success = function(response, status, xhr) {
                var result = oldSuccess.call(this, response, status);

                // 'base' will most likely have changed.
                var
                  oldBase = base,
                  regex = new RegExp('^' + oldBase.replace(/\-\d+\-/, '-\\\d+-') + '$');
                if (response[0] && response[0].settings && response[0].settings.ajax) {
                  $.each(response[0].settings.ajax, function(potentialBase, _settings) {
                    if (potentialBase.match(regex)) {
                      base = potentialBase;
                    }
                  });
                }

                // Send next file
                sendNextFile();

                return result;
              };

              // Prep upload
              options.files = [file];
              options.inputName = inputName;

              // Upload
              $submit.trigger(settings.event);
            }

            function allFilesDone() {
              // Reset all file inputs.
              $form.find('input[type="file"]').each(function() {
                var $this = $(this);
                $this.attr('name', $this.data('actualInputName'));
                $this.removeData('actualInputName');
              });

              // alert('Done!');
            }

            startUpload();

          } // drop
        }).addClass('dragdropfile-processed'); // bind events
      }
    });

  }
};



})(jQuery);
