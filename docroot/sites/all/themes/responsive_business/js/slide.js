jQuery(function($){
  $(window).load(function() {
    
    //homepage slides
    $('.flexslider').flexslider({
      animation: "fade", //Select your animation type (fade/slide)
      slideshow: true, //Should the slider animate automatically by default? (true/false)
      slideshowSpeed: 6000, //Set the speed of the slideshow cycling, in milliseconds
      animationDuration: 600, //Set the speed of animations, in milliseconds
      directionNav: true, //Create navigation for previous/next navigation? (true/false)
      controlNav: false, //Create navigation for paging control of each slide? (true/false)
      keyboardNav: true, //Allow for keyboard navigation using left/right keys (true/false)
      touchSwipe: true, //Touch swipe gestures for left/right slide navigation (true/false)
      prevText: '<span class="awesome-icon-chevron-left"></span>', //Set the text for the "previous" directionNav item
      nextText: '<span class="awesome-icon-chevron-right"></span>', //Set the text for the "next" directionNav item
      randomize: false, //Randomize slide order on page load? (true/false)
      animationLoop: true, //Should the animation loop? If false, directionNav will received disabled classes when at either end (true/false)
      pauseOnAction: true, //Pause the slideshow when interacting with control elements, highly recommended. (true/false)
      pauseOnHover: false, //Pause the slideshow when hovering over slider, then resume when no longer hovering (true/false)
    });
  
  });// END window load
});// END function