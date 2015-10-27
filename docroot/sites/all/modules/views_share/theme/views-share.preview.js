(function ($) {

  $(document).delegate('#edit-embed-share', 'focus', function() {
    var that = this;
    setTimeout(function(){ that.select() }, 100);
  });

  Drupal.behaviors.viewsSharePreview = {
    attach: function(context) {
      // http://tutorialzine.com/2013/07/quick-tip-parse-urls/
      var a = $('<a>', { href:location.href } )[0];
      $('#edit-embed-width, #edit-embed-height').change(function() {
        var embedCode = Drupal.settings.viewsSharePreview.embedCode
          .replace('%width', $('#edit-embed-width').val())
          .replace('%height', $('#edit-embed-height').val());
        $('#edit-embed-share').val(embedCode); // update embed code
        $('iframe').replaceWith(embedCode); // update preview
        window.opener.postMessage({embedCode: embedCode}, a.baseURI); // update parent window
      });
    }
  }

})(jQuery);
