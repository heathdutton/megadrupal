/**
 * @file
 */
(function ($) {
  Drupal.behaviors.views_effects = {
    attach: function(context, settings) {
      var settings = Drupal.settings.views_list_effect;
      for (var instance in Drupal.settings.views_list_effect) { 
        settings = Drupal.settings.views_list_effect[instance];
        Drupal.behaviors.views_effects.process(instance, settings);
      }
    },
    process: function(instance, settings) {
      var random = settings.random;
      var delay = settings.delay;
      var elements = $("#" + instance + "[data-views-list-effect] li");
      if (random == 1) {
        elements = Drupal.behaviors.views_effects.shuffleElements(elements);
      }
      if ($.isFunction($.fn.waypoint)) {
        $("#" + instance + "[data-views-list-effect]").waypoint(function() {
          Drupal.behaviors.views_effects.animate(instance, delay, elements);
        }, {triggerOnce: true, offset: "90%"});
      }
      else {
        Drupal.behaviors.views_effects.animate(instance, delay, elements);
      }
    },
    animate: function(instance, delay, elements) {
      elements.each(function (i) {
        $(this).attr("style", "-webkit-animation-delay:" + i * delay + "ms;"
                + "-moz-animation-delay:" + i * delay + "ms;"
                + "-o-animation-delay:" + i * delay + "ms;"
                + "animation-delay:" + i * delay + "ms;");
        if (i == $("#" + instance + "[data-views-list-effect] li").size() -1) {
          $("#" + instance + "[data-views-list-effect]").addClass("play");
        }
      });
    },
    shuffleElements: function(o) {
      for (var j, x, i = o.length; i; j = parseInt(Math.random() * i), x = o[--i], o[i] = o[j], o[j] = x);
      return o;
    }
  }
}(jQuery));