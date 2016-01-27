
// Copied from filter.admin.js
(function ($) {
Drupal.behaviors.opacEditMenu = {
  attach: function (context, settings) {
    $('.opac-edit-menu-toggle', context).click(function (e) {
      $menu = $(this).parent().find('.opac-edit-menu');
      if ($menu.is('.collapsed')) {
        $menu.removeClass('collapsed');
      }
      else {
        $menu.addClass('collapsed');
      }
      return false;
    });
  }
};

})(jQuery);
