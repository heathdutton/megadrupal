/**
 * @file
 * Mobile navigation behavior definition.
 *
 * Get fron Drupal the settings specified on mobile navigation administration page
 * and place them in a parameters structures to run the Mobile Menu plugin.
 */

(function($) {

  // Execute mobile-navigation construction with the settings on mobile_navigation.
  Drupal.behaviors.mobile_navigation = {
    attach: function(context) {
      if ($(Drupal.settings.mobile_navigation.menuSelector, context).length) {
        var settings = {
          breakpoint       : Drupal.settings.mobile_navigation.breakpoint,
          menuPlugin       : Drupal.settings.mobile_navigation.menuPlugin,
          showEffect       : Drupal.settings.mobile_navigation.showEffect,
          showItems        : Drupal.settings.mobile_navigation.showItems,
          tabHandler       : Drupal.settings.mobile_navigation.tabHandler,
          menuWidth        : Drupal.settings.mobile_navigation.menuWidth,
          specialClasses   : Drupal.settings.mobile_navigation.specialClasses,
          mainPageSelector : Drupal.settings.mobile_navigation.mainPageSelector,
          useMask          : Drupal.settings.mobile_navigation.useMask,
          menuLabel        : Drupal.settings.mobile_navigation.menuLabel,
          expandActive : Drupal.settings.mobile_navigation.expandActive
        };
        $(Drupal.settings.mobile_navigation.menuSelector, context).mobile_menu(settings);
      }
    }
  }

})(jQuery);
