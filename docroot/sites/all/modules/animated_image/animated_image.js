/**
 * @file
 * JavaScript to animate a static images.
 */

(function ($) {

/**
 * Behaviour to animate a static images.
 */
Drupal.behaviors.animated_image = {};
Drupal.behaviors.animated_image.attach = function(context) {

  $("img.animated-image").one("load", function() {
    var image = $(this);

    // Ensure no padding or margin are present in the image.
    // This could break the animation.
    image.css({'margin': 0, 'padding': 0});
    Drupal.animated_image.add_wrapper(image);

    var interval = image.attr("interval") || 1;
    image.timer = setTimeout(
      function() {
        Drupal.animated_image.animate(image);
      },
      interval * 1000);
  })
  .each(function() {
    if (this.complete) {
      $(this).load();
    }
  });

  $("img.animated-image").click(function() {
    Drupal.animated_image.toggle_animation($(this));
  });

};


Drupal.animated_image = {

  add_wrapper: function(image) {
    var wrapper = '<div class="animated-image-wrapper"></div>';

    image.wrap(wrapper).parent().css({
      "height": parseInt(image[0].clientHeight / image.attr("images")) + "px",
      "overflow": "hidden"
    });

  },

  move_image: function(image) {
    var interval = image.attr("interval") || 1;
    var images = parseInt(image.attr("images"));
    var counter = parseInt(image.attr("counter")) || 1;

    var image_size = parseInt(image[0].clientHeight / images);
    image.parent().css({"height": image_size});
    image.css({"margin-top": -1 * counter * image_size});
    counter++;
    if (counter > images) {
      counter = 1;
      image.css({"margin-top": 0});
      Drupal.animated_image.trigger_event('loop_completed', image);
    }
    image.attr("counter", counter);
  },

  animate: function(image) {
    if (!image.hasClass("paused")) {
      Drupal.animated_image.move_image(image);
      var interval = image.attr("interval") || 1;
      image.timer = setTimeout(function() {
        Drupal.animated_image.animate(image);
      }, interval * 1000);
    }
  },

  toggle_animation: function(image) {
    if (image.hasClass("pause-on-click")) {
      clearTimeout(image.timer);
      image.toggleClass("paused");
      Drupal.animated_image.animate(image);
      Drupal.animated_image.trigger_event('toggle_animation', image);
    }
  },

  trigger_event: function(event_name, image) {
    $(document).trigger('animated_image.' + event_name, image);
  }
}

})(jQuery);
