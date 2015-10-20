(function ($) {

  Drupal.behaviors.gplus_comments_block = {
    attach: function (context, settings) {
      jQuery.each(settings.gplus_comments_block, function(id, data) {

        id = '#' + id;
        var width = jQuery(id).parent().width();
        var url = data.url;

        jQuery('<div/>', {
          'class':  'g-comments',
          'data-href': url,
          'data-width': width,
          'data-first_party_property': "BLOGGER",
          'data-view_type': "FILTERED_POSTMOD",
          'text': 'Loading Google+ Comments ...'
        }).appendTo(id);

      });
    }
  };

})(jQuery);
