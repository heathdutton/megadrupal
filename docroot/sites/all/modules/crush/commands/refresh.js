(function($) {

Drupal.crush.commands.refresh = function(parsed) {
  $('iframe').attr('src', $('iframe').attr('src'));
};

})(jQuery);
