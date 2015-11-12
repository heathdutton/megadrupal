(function ($) {

  Drupal.behaviors.viewsShare = {
    attach: function() {
      $(document).delegate('.views-share-embed, .views-share-link', 'focus', function() {
        var that = this;
        setTimeout(function(){ that.select() }, 100);
      });

      $('a.views-share-preview').live('click', function(e) {
      	e.preventDefault();

        // Open popup in a new window center screen and listen to its messages.
        var width = 1024,
            height = 768,
            left  = ($(window).width()-width)/2,
            top   = ($(window).height()-height)/2,
            popup = window.open(
              $(this).attr('href'), 
              'views-share-preview',
              'scrollbars=1,width='+width+',height='+height+',top='+top+',left='+left
            );

        // http://stackoverflow.com/questions/8822907/html5-cross-browser-iframe-postmessage-child-to-parent
        var eventMethod = window.addEventListener ? 'addEventListener' : 'attachEvent';
        var eventWindow = window[eventMethod];
        var eventMessage = eventMethod === 'attachEvent' ? 'onmessage' : 'message';
        eventWindow(eventMessage, function(event) {
          $('#edit-share-embed').val(event.data.embedCode);
        }, false);
      });

      $('#edit-short-url').live('click', function() {
        $('#edit-share-link').val(this.checked ? 
          Drupal.settings.viewsShare.shortURL : Drupal.settings.viewsShare.originalURL
        );
      });
    }
  };

})(jQuery);
