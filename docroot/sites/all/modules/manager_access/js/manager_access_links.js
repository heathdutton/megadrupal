window.$ = jQuery;
Drupal.managerLinks = Drupal.managerLinks || {};

/**
 * Attach outline behavior for regions associated with manager links.
 */
Drupal.behaviors.managerLinks = {
  attach: function (context) {
    $('div.manager-links-wrapper', context).once('manager-links', function () {
      var wrapper = $(this);
      var region = wrapper.closest('.manager-links-region');
      var links = wrapper.find('ul.manager-links');
      var trigger = $('<a class="manager-links-trigger" href="#" />').text(Drupal.t('Configure')).click(
        function () {
          links.stop(true, true).slideToggle(100);
          wrapper.toggleClass('manager-links-active');
          return false;
        }
        );
      // Attach hover behavior to trigger and ul.manager-links.
      trigger.add(links).hover(
        function () {
          region.addClass('manager-links-region-active');
        },
        function () {
          region.removeClass('manager-links-region-active');
        }
        );
      // Hide the manager links when user clicks a link or rolls out of the .manager-links-region.
      region.bind('mouseleave click', Drupal.managerLinks.mouseleave);
      // Prepend the trigger.
      wrapper.prepend(trigger);
    });
  }
}

/**
 * Disables outline for the region manager links are associated with.
 */
Drupal.managerLinks.mouseleave = function () {
  $(this)
  .find('.manager-links-active').removeClass('manager-links-active')
  .find('ul.manager-links').hide();
};
