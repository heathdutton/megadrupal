(function ($) {
  $.fn.adjustZindex = function(adjustment) {
    this.each(function(index) {
      // Default to 0 if z-index is NaN
      var zInd = $(this).css('z-index') || 0;
      $(this).css('z-index', zInd + adjustment);
    });
    return this;
  };
  Drupal.behaviors.lights_out = {
    toggle_switch: function (clicked_anchor) {
      clicked_anchor = clicked_anchor || $('.lights_out_switch a').first();
      // Get the corresponding anchor label from Drupal
      var switch_anchor_label = Drupal.settings.lights_out_label_off;
      if (clicked_anchor.text() == Drupal.settings.lights_out_label_off) {
        switch_anchor_label = Drupal.settings.lights_out_label_on;
      }
      // Update anchor label and classes
      $('.lights_out_switch a').toggleClass('is-off').toggleClass('is-on').text(switch_anchor_label);
    },
    shadow_show: function (region) {
      // Adjust height of overlay to fill screen when page loads
      $('#lights_out_shadow').css('height', $(document).height());
      // Update z-indexes of elements contained in the targeted region
      $(region).find('*').adjustZindex(200);
      $(region).addClass('lights_out_highlight');
      $('#lights_out_shadow').fadeIn();
    },
    shadow_hide: function (region) {
      $('#lights_out_shadow').fadeOut(400, function() {
        // Reset height of overlay to prevent document resizing
        $('#lights_out_shadow').css('height', 0);
      });
      $(region).removeClass('lights_out_highlight');
      // Update z-indexes of elements contained in the targeted region
      $(region).find('*').adjustZindex(-200);
    },
    attach: function (context) {
      // Create overlay element
      var newdiv = $('<div id="lights_out_shadow"/>');
      $('body').append(newdiv);
      // Get region classname from Drupal
      var region = '.region-' + Drupal.settings.lights_out_region;
      $('.lights_out_switch a').click(function(event) {
        event.preventDefault();
        var clicked_anchor = $(this);
        var state = clicked_anchor.attr('class');
        switch (state) {
          case 'is-on':
            Drupal.behaviors.lights_out.shadow_show(region);
            Drupal.behaviors.lights_out.toggle_switch(clicked_anchor);
            break;

          case 'is-off':
            Drupal.behaviors.lights_out.shadow_hide(region);
            Drupal.behaviors.lights_out.toggle_switch(clicked_anchor);
            break;

          default:
            break;

        }
      });
      $('#lights_out_shadow').click(function() {
        Drupal.behaviors.lights_out.shadow_hide(region);
        Drupal.behaviors.lights_out.toggle_switch();
      });
      return false;
    }
  };
}(jQuery));
