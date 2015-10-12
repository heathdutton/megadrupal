$ = jQuery.noConflict(); // Make sure jQuery owns $ here

jQuery(document).ready(function() {

  $(Drupal.settings.sooperfish.invoke+' ul').hide().css('left','0'); //Remove the default CSS behaviour of hiding menu outside the viewport, so that we can have a slideUp animation in a visible position
  $(Drupal.settings.sooperfish.invoke).sooperfish({
    sooperfishWidth: Drupal.settings.sooperfish.sooperfishWidth,
    hoverClass:  'over',           // hover class
    delay:     Drupal.settings.sooperfish.delay,                // 500ms delay on mouseout as per Jacob Nielsen advice
    dualColumn:     Drupal.settings.sooperfish.dualColumn,
    tripleColumn:     Drupal.settings.sooperfish.tripleColumn,
    animationShow:   Drupal.settings.sooperfish.animationShow,
    speedShow:     parseInt(Drupal.settings.sooperfish.speedShow),
    easingShow:    Drupal.settings.sooperfish.easingShow,
    animationHide:   Drupal.settings.sooperfish.animationHide,
    speedHide:     parseInt(Drupal.settings.sooperfish.speedHide),
    easingHide:    Drupal.settings.sooperfish.easingHide,
    autoArrows:  false,              // generation of arrow mark-up
    dropShadows: false               // drop shadows
  });
});