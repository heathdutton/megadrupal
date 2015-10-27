(function($) {

ChiliBook.lineNumbers = false;

Drupal.behaviors.chiliHighlighter = {
  attach: function(context, settings) {
    $(settings.chiliHighlighter.selector, context).once('chili-highlighter', function() {
      $(this).chili();
    });
  }
};

})(jQuery);
