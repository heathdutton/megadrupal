(function ($) {
  
  // Bind on document ready
  $(document).ready(function() {
    widecode();
  });
  
  // Bind on jQuery Mobile init
  $(document).bind('pageinit', function() {
    widecode();
  });


/**
 * Enable events to trigger the expansion/compression of wide code blocks
 */
function widecode() {
  // Set the overflow to hidden
  // This allows you to leave the scroll bar to be used on devices
  // which don't support javascript.
  $("pre").css('overflow', 'hidden');
  
  // Add the mouse based event handlers
  $("pre").hover(
    function () {
      if (widecode_checkoverflow($(this).get(0))) {
        widecode_grow($(this));
      }
    },
    function () {
      widecode_shrink($(this));
    }
  );

  // Add the touch based handlers
  $('pre').live('tap', function() {
    widecode_touch($(this));
  });
}

// Expand the element
function widecode_grow(element) {
  $.data(element.get(0), 'widecode-width', element.width());
  // @todo determine how wide the expander needs to go
  element.animate({ width: "950px"}, 250, function() { $(this).css('overflow-x', 'visible'); });  
}

// Shrink the element
function widecode_shrink(element) {
  if ($.data(element.get(0), 'widecode-width')) {
    element.css('overflow-x', 'hidden').animate({ width: $.data(element.get(0), 'widecode-width') }, 250);
    $.data(element.get(0), 'widecode-width', false);
  }
}

function widecode_touch(element) {
  if ($.data(element.get(0), 'widecode-width')) {
    element.css('overflow-x', 'hidden').animate({ width: $.data(element.get(0), 'widecode-width') }, 250);    
  }
  else {
    $.data(element.get(0), 'widecode-width', element.width());
    element.animate({ width: "950px"}, 250, function() { $(this).css('overflow-x', 'visible'); });  
  }
}

/** 
 * Determines if the passed element is overflowing its bounds,
 * either vertically or horizontally.
 * Will temporarily modify the "overflow" style to detect this
 * if necessary.
 *
 * Based on
 * @see http://stackoverflow.com/questions/143815/how-to-determine-from-javascript-if-an-html-element-has-overflowing-content
 */
function widecode_checkoverflow(element) {
   var curOverflow = element.style.overflow;
   if ( !curOverflow || curOverflow === "visible" )
      element.style.overflow = "hidden";

   var isOverflowing = element.clientWidth < element.scrollWidth 
      || element.clientHeight < element.scrollHeight;

   element.style.overflow = curOverflow;

   return isOverflowing;
}


}(jQuery));