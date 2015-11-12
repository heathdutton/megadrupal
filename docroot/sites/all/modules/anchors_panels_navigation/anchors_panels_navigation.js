/**
 * @file
 * JS file of Anchors Panels Navigation module.
 */
var old_hash = window.location.hash;
(function($){
$(document).ready(function() {
  var top_offset = Drupal.settings.anchors_panels_navigation.top_offset;  
  if(Drupal.settings.anchors_panels_navigation.fix_panel_height) {
    var window_height = $(window).height();
    Drupal.settings.anchors_panels_navigation.hashes.forEach(function(entry) {
      $('#' + entry).css('min-height', window_height);
    });
  }	
  var hash = window.location.hash.substr(1);
  if(hash){
    var destination = $("#" + hash).offset().top - top_offset;
    $("html:not(:animated),body:not(:animated)").animate({ scrollTop: destination}, 500, function() {
      return false;
    });
  }
  if($.inArray(hash, Drupal.settings.anchors_panels_navigation.hashes) !== -1){
    anchors_panels_navigation_classes_fix(hash);	  
  }

  var previos_object_height = $(window).height();

  $(document.body).on('appear', '.panel-pane', function(e, $affected) {

    var hash = $(this).attr('id');
    var offset = $(this).offset().top - $(window).scrollTop();

    if(old_hash != hash && offset > top_offset - 1 && offset < previos_object_height) {
      old_hash = hash;
      previos_object_height = $(this).height();
      anchors_panels_navigation_classes_fix(hash);
      var stateObj = { foo: "hash" };
      history.pushState(stateObj, "hash", "#" + hash);
    }
  });
  $('.panel-pane').appear({force_process: true});

	$("a").click(function(event){
		//check if it has a hash (i.e. if it's an anchor link)
		if(this.hash){
			var hash = this.hash.substr(1);	
			if($.inArray(hash, Drupal.settings.anchors_panels_navigation.hashes) !== -1){
				event.preventDefault();
				var destination = $("#" + hash).offset().top - top_offset;
				$("html:not(:animated),body:not(:animated)").animate({ scrollTop: destination}, 500, function() {
					return false;
				});
			}
			return true;	
		}
		return true;
	});
});

function anchors_panels_navigation_classes_fix(hash){
  $('a').removeClass(Drupal.settings.anchors_panels_navigation.classes_remove);
  $('a[href$="#' + hash + '"]').addClass(Drupal.settings.anchors_panels_navigation.classes_set);
}
})(jQuery);