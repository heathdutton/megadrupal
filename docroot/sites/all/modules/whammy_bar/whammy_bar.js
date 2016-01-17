(function ($) {

Drupal.behaviors.whammyBar = {
  attach: function (context) {
    var $whammy = $('#whammy-bar-container', context);
    if ($whammy.length == 0) {
      return;
    }

    var $body = $('body', context);
    var body_margin = $body.css('margin-top');
    body_margin = parseInt(body_margin);
    $body.css({marginTop: '30px'});

    $whammy.css({
      position: 'absolute',
      top: body_margin,
      left: 0,
      display: 'block'
    });

    $('ul li', $whammy).hover(function() {
        var $menu = $(this);
        $menu.addClass('whammy-bar-keep-open');
        $('div.item-list', $menu).show().css({
          top: $menu.height() + 'px'
        });
      },
      function() {
        $(this).removeClass('whammy-bar-keep-open');
        setTimeout(whammy_bar_hide_submenu, 835, this);
      }
    );
  }
};

var whammy_bar_hide_submenu = function (obj) {
  if(!$(obj).hasClass('whammy-bar-keep-open')) {
    $('div.item-list', $(obj)).hide();
  }
}

})(jQuery);
