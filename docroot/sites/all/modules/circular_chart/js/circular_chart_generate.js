/**
 * @file
 * Javascript to generate the circular charts.
 */

var circles = [];

(function ($) {
  Drupal.behaviors.circular_chart_generate = {
    attach: function (context) {
      if ($('div .circle-graph', context).length) {
        var circle;
        $('div .circle-graph', context).once('circle-graph', function () {
          $(this).each(function () {
            circle = Circles.create({
              id:         $(this).attr('id'),
              radius:     parseInt($(this).attr('circle_radius')),
              value:      parseInt($(this).attr('value')),
              maxValue:   parseInt($(this).attr('max_value')),
              width:      parseInt($(this).attr('circle_width')),
              colors:     [$(this).attr('circle_fg_color'), $(this).attr('circle_bg_color')],
              duration:   parseInt($(this).attr('circle_anim_duration')),
              wrpClass:   $(this).attr('wrapper-class'),
              textClass:  'circles-text',
              text     : $(this).attr('inner-text')
            });
            circles.push(circle);
          });
        });
      }
    }
  }
})(jQuery);
