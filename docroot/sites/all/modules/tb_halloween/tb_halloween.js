(function ($) {
  Drupal.behaviors.tbHalloweenAction = {
    attach: function (context) {  
	  $(window).load(function() {
		var items = Drupal.TBHalloween.items;
        for(x in items) {
          var item = items[x];
          for(y in item) {
            var pumpkin = item[y];
            var options = {
              id: 'id_' + item['id'] + "_number_" + y,
              image: pumpkin['image'],
              width: pumpkin['width'],
              height: pumpkin['height'],
              tip: pumpkin['hover_message'],
              frame: pumpkin['frame'],
              duration: pumpkin['flying_speed'],
              fps: pumpkin['swing_speed'],
              delay: pumpkin['delay_time'],
              delaystart: pumpkin['delaystart_time'],
              framestart: pumpkin['start_frame'],
              closeable: pumpkin['closable'],
              'class': pumpkin['extend_class'],
              type: pumpkin['animation_type']
            };
            if (pumpkin['animation_type'] == 'random') {
              if (pumpkin['animation_area']) {
                options.data = [pumpkin['area_left'], pumpkin['area_top'], pumpkin['area_right'], pumpkin['are_bottom']];
              }
            }
            else if (pumpkin['animation_type'] == 'preset') {
              options.data = pumpkin['pos_array'];
            }
            new Frame(options);
          }
        }
	  });
    }
  };
  Drupal.TBHalloween = Drupal.TBHalloween || {};
  Drupal.TBHalloween.selectImage = function(element) {
    $('input[name="pumpkin_image"]').val($(element).find('input[type="hidden"]').val());
    $(element).parent().children().removeClass('selected');
    $(element).addClass('selected');
  };
  
  Drupal.TBHalloween.closePumpkin = function(ele) {
	var closed_pumpkins = $.cookie("closed_pumpkin") ? $.cookie("closed_pumpkin") : [];
	closed_pumpkins.push('1');
    $.cookie("closed_pumpkin", array(), {path: "/"});
  }
})(jQuery);
