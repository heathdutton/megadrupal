/**
 * @file
 * JavaScript provided to enhance UI elements of the Recurly Roles module.
 */
(function ($) {
  /**
   * Behavior to change the radio buttons on the user/register form.
   *
   * This is a modified version of Drupal.behavrious.recurlyPlanSelect() behavior
   * from the Recurly module which toggles a radio button when clicking on an
   * element.
   */

  // Since we're using the theme_recurly_subscription_plan_select() output
  // which adds the normal recurly.js and the method below we just wipe it out
  // for this page request. This allows us to handle all the events in our jS
  // and not have to worry about what the defaults are trying to do.
  Drupal.behaviors.recurlyPlanSelect = {};

  Drupal.behaviors.recurlyRolesPlanSelect = {};
  Drupal.behaviors.recurlyRolesPlanSelect.attach = function(context, settings) {
    // Hide the radio buttons.
    $('#edit-recurly-roles-radios').addClass('element-hidden');
    // Set the appropriate option to selected if there is #default_value for the
    // radio buttons.
    var val = $('input:radio[name="recurly_roles_radios"]:checked', context).val();
    $('input[name="recurly_roles_radios"]', context).val();
    if (val != 'undefined') {
      $('.plan-' + val, context).addClass('plan-selected');
    }

    // Do this on the user/register page only so that it doesn't interfere
    // with the version of this in the recurly module.
    $('.plan-signup a.plan-select', context).hide().each(function() {
      var link = this;
      var $link = $(this);
      $link.unbind('click');
      $link.parents('.plan:first').click(function(e) {
        // Select the correct radio button.
        plan = $(this).find('a').attr('href').split('/').pop();
        $('input[value="' + plan + '"]').attr('checked', 'checked');

        // Highlight the selected plan.
        $('.plan-selected').removeClass('plan-selected');
        $(this).addClass('plan-selected');

        return false;
      });
      $link.parents('.plan:first').hover(function() {
        $(this).addClass('plan-hover');
      }, function() {
        $(this).removeClass('plan-hover');
      });
    });
  };

})(jQuery);
