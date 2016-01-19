jQuery(function(){
  jQuery('#slides').slides({
    play: 5000,
    pause: 2500,
    hoverPause: true 
  });
});

jQuery(document).ready(function() {
  jQuery("#slides").hover(function() {
    jQuery(".slides_nav").css("display", "block");
  },
  function() {
    jQuery(".slides_nav").css("display", "none");
  });
}); 