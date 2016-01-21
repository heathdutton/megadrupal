/**
 * Overwrites menu links with anchor tags and adds "fixed" class to menu if necessary.
 */

(function ($) {
  Drupal.behaviors.singlePageMenu = {
    init: function (context, settings) {
      var menu_element = null;
      if (Drupal.settings.singlePage.className === 'li') {
        menu_element = Drupal.settings.singlePage.menuClass + ' li a';
      } else {
        menu_element = Drupal.settings.singlePage.menuClass + ' .' + Drupal.settings.singlePage.className;
      }
      var basePath = window.location.host + Drupal.settings.basePath + Drupal.settings.pathPrefix;
      $(menu_element).each(function (index) {
        var hr = this.href.split(basePath);
        // To support "no clean" urls, replace q param by empty string.
        var anchor = decodeURI(hr[hr.length - 1].replace('?q=', '').split('/').join(''));
        // Now replace all weirdo chars from anchor.
        anchor = anchor.replace(/\W/g, '');
        if ($(document).find('#single-page-overall-wrapper').length) {
          // We are on the single page, just add anchor.
          this.href = "#" + anchor;
        } else {
          if (Drupal.settings.singlePage.isFrontpage) {
            // Go to homepage, with anchor.
            this.href = Drupal.settings.basePath + Drupal.settings.pathPrefix + "#" + anchor;
          } else {
            // Go to single-page-site (or alias) with anchor.
            this.href = Drupal.settings.basePath + Drupal.settings.pathPrefix + Drupal.settings.singlePage.urlAlias + "#" + anchor;
          }
        }
        $(this).attr('data-active-item', anchor);
      });
      // Remove menu items with class "hide" from DOM.
      $(Drupal.settings.singlePage.menuClass + ' li .hide').parent().remove();
      // Add "fixed" class to menu when it disappears form viewport.
      if ($(document).find(Drupal.settings.singlePage.menuClass).length) {
        var top = $(Drupal.settings.singlePage.menuClass).offset().top;
        $(window).scroll(function (event) {
          // What the y position of the scroll is.
          var y = $(this).scrollTop();
          // Whether that's below the form.
          if (y > top) {
            // If so, ad the fixed class.
            $(Drupal.settings.singlePage.menuClass).addClass('fixed');
          } else {
            // Otherwise remove it.
            $(Drupal.settings.singlePage.menuClass).removeClass('fixed');
          }
        });
      }
    }
  };

  $(function () {
    // Init menu behaviour.
    Drupal.behaviors.singlePageMenu.init();
  });
})(jQuery);
