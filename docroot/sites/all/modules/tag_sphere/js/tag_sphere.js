(function ($) {
  Drupal.behaviors.tag_sphere = {
    attach: function (context, settings) {
      $(".block-tag-sphere .item-list").css({background: '#CCCCCC', padding: '5px'});
      $(".block-tag-sphere .item-list li").css("list-style", "none");
      var tag_sphere_settings = Drupal.settings.tag_sphere.tag_sphere_settings;

      $(".block-tag-sphere", context).each(function(i) {
        $(this).find(".item-list").attr("id","ts" + i);
        var tag_sphere_block = $(this).attr('id').split('tag-sphere');
        var vocab_id = tag_sphere_block[1].substring(1);

        if ((tag_sphere_settings[vocab_id]['font_color'] == '') || (!tag_sphere_settings[vocab_id]['font_color'])) {
          tag_sphere_settings[vocab_id]['font_color'] = '#FFFFFF';
        }
        $("#block-tag-sphere-" + vocab_id + " .item-list a").css("color", tag_sphere_settings[vocab_id]['font_color']);
        $("#block-tag-sphere-" + vocab_id + " .item-list")
          .tagcloud(
          {
            centrex: parseInt(tag_sphere_settings[vocab_id]['centrex']),
            centrey: parseInt(tag_sphere_settings[vocab_id]['centrey']),
            init_motion_x: parseInt(tag_sphere_settings[vocab_id]['init_motion_x']),
            init_motion_y: parseInt(tag_sphere_settings[vocab_id]['init_motion_y']),
            min_font_size: parseInt(tag_sphere_settings[vocab_id]['min_font_size']),
            max_font_size: parseInt(tag_sphere_settings[vocab_id]['max_font_size']),
            rotate_factor: parseInt(tag_sphere_settings[vocab_id]['rotate_factor'])
          })
           .css(
          {
            height: tag_sphere_settings[vocab_id]['height'],
            width: tag_sphere_settings[vocab_id]['width'],
            background: tag_sphere_settings[vocab_id]['background_color']
          });
      });
    }
  };
})(jQuery);
