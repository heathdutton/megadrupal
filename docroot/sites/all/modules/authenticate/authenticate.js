(function($){

Drupal.behaviors.authenticate = {
attach : function(settings, context) {
  $.ajax({
    url: Drupal.settings['authenticate_url'],
    type: "POST",
    dataType: "xml/html/script/json",
    data: Drupal.settings, 
    complete: function() {     }
  });
}
}
})(jQuery);



