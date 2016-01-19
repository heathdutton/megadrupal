/**
 * @file
 * Attaches the behaviors for the Karmacracy module.
 */

(function ($) {

Drupal.behaviors.karmacracyWidget = {
  attach: function(context) {
    var farb = $.farbtastic('.colorpicker');
    var c = $('.colorpicker').css('opacity', 0.25); 
    var selected;
    
    $('.field-colorpicker').each(function() {
      farb.linkTo(this);
      $(this).css('opacity', 0.75);
    });
    
    $('.field-colorpicker').focus(function() {
      var input = this;
      if (selected) {
        $(selected).css('opacity', 0.75).removeClass('field-selected');
      }
      farb.linkTo(this);
      c.css('opacity', 1);
      $(input).css('opacity', 1).addClass('field-selected');
      selected = input;
    });
    farb.linkTo(function () {});
    
    var preview = function() {
      var settings = {};
      $.each($('#edit-widget').serializeArray(), function(i, field) {
        var name = field.name.replace(/karmacracy_widget_settings\[(.*)\]/, '$1');
        settings[name] = field.value.replace(/\#/, '');
      });
      $('.karmacracy-preview').html(Drupal.theme('karmacracyPreview', settings));
    };
    
    $('.widget-settings .form-item input').change(preview);
    $('.widget-settings .field-colorpicker').focusout(preview);
    preview();
  }
};

Drupal.theme.prototype.karmacracyPreview = function(settings) {
  // @todo Use KARMACRACY_WIDGET_VERSION constant
  var kcyJsUrl = "http://rodney.karmacracy.com/widget-1.3/?id=ID";
  kcyJsUrl += "&type=h";
  kcyJsUrl += "&width=" + settings['width'];
  kcyJsUrl += "&sc=" + settings['sc'];
  kcyJsUrl += "&rb=" + settings['rb'];
  kcyJsUrl += "&np=" + settings['np'];
  kcyJsUrl += "&c1=" + settings['color1'];
  kcyJsUrl += "&c2=" + settings['color2'];
  kcyJsUrl += "&c3=" + settings['color3'];
  kcyJsUrl += "&c4=" + settings['color4'];
  kcyJsUrl += "&c5=" + settings['color5'];
  kcyJsUrl += "&c6=" + settings['color6'];
  kcyJsUrl += "&c7=" + settings['color7'];
  kcyJsUrl += "&c8=" + settings['color8'];
  kcyJsUrl += "&c9=" + settings['color9'];
  kcyJsUrl += "&url=http://karmacracy.com";  
  return '<div class="kcy_karmacracy_widget_h_ID"></div><script defer="defer" src="' + kcyJsUrl + '"></script>';
};

})(jQuery);
