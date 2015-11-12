/**
 * @file
 * Functionality that provides style switching without page reloading.
 */

(function ($) {

Drupal.styleSwitcher = Drupal.styleSwitcher || {};

/**
 * Given the style object, switches stylesheets.
 *
 * @param style
 *   A style array.
 */
Drupal.styleSwitcher.switchStyle = function (style) {
  // Update the cookie first.
  $.cookie('styleswitcher[' + style.theme + ']', style.name, {
    path: Drupal.settings.basePath,
    // Number of days the cookie must live.
    expires: Drupal.settings.styleSwitcher.cookieExpire / 86400
  });

  // Now switch the stylesheet. Path is absolute URL with scheme.
  $('#styleswitcher-css').attr('href', style.path);

  // Cosmetic changes.
  Drupal.styleSwitcher.switchActiveLink(style.name);
};

/**
 * Switches active style link.
 *
 * @param styleName
 *   Machine name of the active style.
 */
Drupal.styleSwitcher.switchActiveLink = function (styleName) {
  $('.style-switcher').removeClass('active')
    .filter('[data-rel="' + styleName + '"]').addClass('active');
};

/**
 * Builds an overlay for transition from one style to another.
 *
 * @return
 *   Dom object of overlay.
 */
Drupal.styleSwitcher.buildOverlay = function () {
  var $overlay = $('<div>')
    .attr('id', 'style-switcher-overlay')
    .appendTo($('body'))
    .hide();

  return $overlay;
};

/**
 * Removes overlay.
 */
Drupal.styleSwitcher.killOverlay = function () {
  // This is more useful than just "$(this).remove()".
  $('#style-switcher-overlay').remove();
};

/**
 * Binds a switch behavior on links clicking.
 */
Drupal.behaviors.styleSwitcher = {
  attach: function (context, settings) {
    // Set active link. It is not set from PHP, because of pages caching.
    var theme = settings.styleSwitcher.theme;
    var activeStyle = $.cookie('styleswitcher[' + theme + ']') || settings.styleSwitcher.default;
    Drupal.styleSwitcher.switchActiveLink(activeStyle);

    $('.style-switcher', context).once('styleswitcher').click(function () {
      var $link = $(this).blur();
      var name = $link.attr('data-rel');
      var style = settings.styleSwitcher.styles[name];

      if (settings.styleSwitcher.enableOverlay) {
        var $overlay = Drupal.styleSwitcher.buildOverlay();

        $overlay.fadeIn('slow', function () {
          Drupal.styleSwitcher.switchStyle(style);
          $overlay.fadeOut('slow', Drupal.styleSwitcher.killOverlay);
        });
      }
      else {
        Drupal.styleSwitcher.switchStyle(style);
      }

      return false;
    });
  }
};

})(jQuery);
