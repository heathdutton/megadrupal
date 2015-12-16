/**
 * @file
 * nicemessages.js
 *
 * Use jGrowl to display messages.
 */

/**
 *  Displays messages with jGrowl.
 */
Drupal.behaviors.nicemessages = {
  attach: function () {
    if (Drupal.settings.nicemessages) {
      jQuery.jGrowl.defaults.position = Drupal.settings.nicemessages.position;
      jQuery.jGrowl.defaults.closerTemplate = '<div>'+Drupal.t('[ close all ]')+'</div>';
      if (Drupal.settings.nicemessages.items) {
        for (i in Drupal.settings.nicemessages.items) {
          jQuery.jGrowl(Drupal.settings.nicemessages.items[i].content, {
            life: Drupal.settings.nicemessages.items[i].life,
            glue: Drupal.settings.nicemessages.items[i].glue,
            speed: Drupal.settings.nicemessages.items[i].speed,
            theme: Drupal.settings.nicemessages.items[i].type,
            sticky: Drupal.settings.nicemessages.items[i].life == 0
          });
        }
        delete Drupal.settings.nicemessages.items;
      }
    }
  }
}
