(function ($) {

Drupal.behaviors.tocLoader = {
  attach: function (context, settings) {
    var toc = $('<ul id="toc"></ul>');
    toc = toc.tableOfContents($(Drupal.settings.toc_scope), { 
      startLevel: Drupal.settings.toc_start_level, 
      depth: Drupal.settings.toc_depth,
      topLinks: Drupal.settings.toc_topLinks
    });
    if ($('li', toc).length > 0) {
      $(Drupal.settings.toc_dest).prepend(toc);
      $('#toc').localScroll();
      $('.content').localScroll(); // For toplinks.
    }    
  }
};

})(jQuery);