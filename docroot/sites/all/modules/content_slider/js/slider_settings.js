
(function($){
$(document).ready(function(){
  var del = Drupal.settings.delta;
  var next = Drupal.settings.next;
  var previous = Drupal.settings.previous;
  var auto_rotate = Drupal.settings.auto_rotate;
  var speed = parseInt(Drupal.settings.speed);
    featuredcontentslider.init({
	   id: "slider"+del,  //id of main slider DIV
	   contentsource: ["inline", ""],  //Valid values: ["inline", ""] or ["ajax", "path_to_file"]
	   toc: "#increment",  //Valid values: "#increment", "markup", ["label1", "label2", etc]
	   nextprev: [previous, next],  //labels for "prev" and "next" links. Set to "" to hide.
	   revealtype: "click", //Behavior of pagination links to reveal the slides: "click" or "mouseover"
	   enablefade: [true, 0.2],  //[true/false, fadedegree]
	   autorotate: [auto_rotate, speed]  //[true/false, pausetime]
	 });
  });
})(jQuery);
