(function ($) {

Drupal.behaviors.viewportClasses = {
  attach: function(context) {
    setScreenClass();
    $(window).resize(
      function() {
        setScreenClass();
      }
    );
  }
}

function setScreenClass() {
  // Setup classes and maximum widths
  var classes = Drupal.settings.viewportClasses;
  
  // Get current width
  var width = $(window).width();
  
  // Determine which class should be set for current width
  for (var i in classes) {
    if (((width >= classes[i]['min width']) || (classes[i]['min width'] == undefined))
     && ((width <= classes[i]['max width']) || (classes[i]['max width'] == undefined) || (classes[i]['max width'] == 0))) {
      $('body').addClass(i);
    }
    else {
      $('body').removeClass(i);
    }
  }
}

})(jQuery);
