/**
 * @file
 * Javascript functions used by Verticrawl modules.
 */

(function ($){
  // Document ready.
  $(function(){
    // Prevent showing URL TRACK in window status bar for ANSWORD LINKS.
    $('a.verticrawl-answord-link').click(function(e) {
        if ($(this).attr('data-redirect').length) {
          e.preventDefault();
          e.stopPropagation();

          var redirectPath = $(this).attr('data-redirect');

          if ($(this).attr('target').length && $(this).attr('target') == '_blank') {
            window.open(redirectPath, "_blank");
          }
          else {
            window.location.href = redirectPath;
          }

          return false;
        }
      });
  });
})(jQuery);
