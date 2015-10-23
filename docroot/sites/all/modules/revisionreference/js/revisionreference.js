
(function ($) {
  $(document).ready(function(){
    var settings = Drupal.settings.revisionreference;
    $.each(settings, function(key, value) { 
      var url = $("."+value.className+" a").attr("href");
  		$("."+value.className+" a").attr("href", url+value.path);
		}); 
  });
})(jQuery);

