(function($){
  Drupal.behaviors.seed = {
    attach:function(context, settings) {
      // Hide/show all messages
      $("#messages-wrapper #messages-toggle").once('messages', function(context, settings) {
        $(this).click(function(e) {
          e.preventDefault();
          $('#messages-content').toggle();
        });
      });

      // Close a single message
      $('.messages .btn-close').click(function(e) {
        e.preventDefault();
        $(this).closest('.messages').fadeOut();
      });
    }
  }
}(jQuery));
