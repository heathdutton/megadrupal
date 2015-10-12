(function($) {
  $('document').ready(function() {
    /**
     * Sets values for the blob_relay_input for ready for submission.
     *
     * TODO: Currently we assume just one form per page.
     *
     * @argument object options
     *   mimetype: what mimetype to use : default: image/png
     *   filename: the name to save on the browser side. default: download.png
     *   data: the raw data string to relay: should contain something
     *   encoding: how is data encoded : default to no encoding
     */
    var blobFillForm = function(options) {
      if (!options) {
        return;
      }
      var settings = {
        // We assume a text file
        mimetype: 'text/plain',
        filename: 'text.txt',
        data: 'Content of fallback file',
        encoding: '',
        prepend: '',
        selector: '#blob-relay-input'
      };
      $.extend(settings, options);

      var $form = $(settings.selector);
      if ($form.length === 0) {
        if (window.console) {
          console.log("blob_relay.js: no form found.");
          return false;
        }
      }

      var $input = $("input[name=mimetype]", $form);
      $input.val(settings.mimetype);

      $input = $("input[name=filename]", $form);
      $input.val(settings.filename);

      $input = $("input[name=encoding]", $form);
      $input.val(settings.encoding);

      $input = $("input[name=data]", $form);
      $input.val(settings.data);

      $input = $("input[name=prepend]", $form);
      $input.val(settings.prepend);

      $form.submit();
    };

    Drupal.blobRelay = Drupal.blobRelay || {};
    Drupal.blobRelay.FillForm = blobFillForm;

  });
})(jQuery);
