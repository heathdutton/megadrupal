var old_hash = window.location.hash;
(function($){
  $(document).ready(function() {
    var hash = window.location.hash.substr(1);
    if(hash){
      var destination = $("#" + hash).offset().top;
      $("html:not(:animated),body:not(:animated)").animate({ scrollTop: destination}, 500, function() {
        return false;
      });
    }

    var previos_object_height = $(window).height();

    $(document.body).on('appear', 'section', function(e, $affected) {
   
      var hash = $(this).attr('id');
      var offset = $(this).offset().top - $(window).scrollTop();

      if(old_hash != hash && offset < previos_object_height) {
        old_hash = hash;
        previos_object_height = $(this).height();
        var stateObj = { foo: "hash" };
        history.pushState(stateObj, "hash", "#" + hash);
      }
    });
    $('body > section').appear({force_process: true});

    $("a").click(function(event){
      //check if it has a hash (i.e. if it's an anchor link)
      if(this.hash){
          var hash = this.hash.substr(1); 
      //  if($.inArray(hash, Drupal.settings.anchors_regions_navigation.hashes) !== -1){
          event.preventDefault();
          var destination = $("#" + hash).offset().top;
          $("html:not(:animated),body:not(:animated)").animate({ scrollTop: destination}, 500, function() {
            return false;
          });
      //  }
        return true;  
      }
      return true;
    });
  });
})(jQuery);