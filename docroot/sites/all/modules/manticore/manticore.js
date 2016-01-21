(function ($) {

Drupal.behaviors.manticore = {
  attach: function (context, settings) {
    // Loop through each Manticore file type.
    $.each(settings.manticore || {}, function(index, type) {
      // Act on any hyperlink to the file types.
      $("a[href$='." + type + "']", context).click(function() {
        // Call the Manticore tracking on the link when its clicked.
        if (window.mtcDownload) {
          // @TODO: Force href to be absolute URL?
          var href = $(this).attr('href');
          var title = $(this).html();
          mtcDownload(href, title);
          // Manticore will do the redirect for us.
          return false;
        }
        // If Manticore isn't around, then just redirect the browser.
        return true;
      });
    });
  }
};

})(jQuery);
