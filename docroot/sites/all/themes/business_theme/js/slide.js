jQuery(window).load(function() {
               
  var transition_effect = "fade";
  var transition_delay = "4000";
  var transition_duration = "1000";
  jQuery('.featured-slider').cycle({ 
    fx: transition_effect, // name of transition effect (or comma separated names, ex: 'fade,scrollUp,shuffle') 
    pager: '#controllers',  // element, jQuery object, or jQuery selector string for the element to use as pager container 
    activePagerClass: 'active',  // class name used for the active pager element
    timeout: transition_delay,  // milliseconds between slide transitions (0 to disable auto advance) 
    speed: transition_duration,  // speed of the transition (any valid fx speed value) 
    pause: 1,     // true to enable "pause on hover" 
    pauseOnPagerHover: 1, // true to pause when hovering over pager link 
    cleartypeNoBg: true
  });

});