(function ($) {

  Drupal.behaviors.viewsLinker = {

    attach: function (context, settings) {

      var searchLabel = settings.viewsLinker.search_label;
      var backPath = settings.viewsLinker.back_path;
      var delta = settings.viewsLinker.views_linker_block_delta;

      var query = document.URL.split('?')[1];
      query = query.split('#')[0];

      // Check we are coming from a search.
      if (query) {
        $('#views-linker-'+ delta, context).show();
        $('#views-linker-'+ delta + ' a', context).text(searchLabel);
        $('#views-linker-'+ delta + ' a', context).attr('href', '/' + backPath + '?' + query);
      }

    }
  };

}(jQuery));
